#include "core/stdafx.h"
#include "Sampler.h"
#include "RenderDevice.h"

#if defined(ENABLE_BGFX)
#include <bx/allocator.h>
#include <bx/readerwriter.h>
#include <bx/endian.h>
#include <bx/math.h>
#include <bimg/decode.h>
#endif

Sampler::Sampler(RenderDevice* rd, BaseTexture* const surf, const bool force_linear_rgb, const SamplerAddressMode clampu, const SamplerAddressMode clampv, const SamplerFilter filter) : 
   m_type(SurfaceType::RT_DEFAULT), 
   m_rd(rd),
   m_dirty(false),
   m_ownTexture(true),
   m_width(surf->width()),
   m_height(surf->height()),
   m_clampu(clampu),
   m_clampv(clampv),
   m_filter(filter)
{
   m_rd->m_curTextureUpdates++;
#if defined(ENABLE_BGFX)
   BaseTexture* upload = surf;
   bgfx::TextureFormat::Enum bgfx_format;
   bool is_srgb = false, add_alpha = false;
   switch (surf->m_format)
   {
   case BaseTexture::SRGBA: bgfx_format = bgfx::TextureFormat::Enum::RGBA8; is_srgb = true; break;
   case BaseTexture::RGBA: bgfx_format = bgfx::TextureFormat::Enum::RGBA8; break;
   case BaseTexture::SRGB: bgfx_format = bgfx::TextureFormat::Enum::RGBA8; is_srgb = true; add_alpha = true; break;
   case BaseTexture::RGB: bgfx_format = bgfx::TextureFormat::Enum::RGB8; break;
   case BaseTexture::RGBA_FP16: bgfx_format = bgfx::TextureFormat::Enum::RGBA16F; break;
   case BaseTexture::RGB_FP16: bgfx_format = bgfx::TextureFormat::Enum::RGBA16F; add_alpha = true; break;
   //case BaseTexture::RGB_FP32: bgfx_format = bimg::TextureFormat::Enum::RGB32F; break;
   case BaseTexture::BW: bgfx_format = bgfx::TextureFormat::Enum::R8; break;
   default: assert(false); // Unsupported texture format
   }

   // TODO this is overkill since this leads to allocating/copying evrything twice
   if (add_alpha && upload != nullptr)
   {
      upload = new BaseTexture(m_width, m_height, surf->m_format);
      memcpy(upload->data(), surf->data(), surf->pitch() * surf->height());
      upload->AddAlpha();
   }

   const bgfx::Memory* data;
   if (upload == nullptr)
      data = NULL;
   else if (upload == surf)
      data = bgfx::copy(upload->data(), m_height * upload->pitch());
   else
      data = bgfx::makeRef(upload->data(), m_height * upload->pitch(), [](void* _ptr, void* _userData) { delete _userData; }, upload);

   // Create a render target and blit texture on it to force BGFX mip map generation (BGFX does not support automatic mipmap generation for textures)
   const uint64_t flags = BGFX_SAMPLER_NONE | (is_srgb ? BGFX_TEXTURE_SRGB : BGFX_TEXTURE_NONE);
   m_texture = bgfx::createTexture2D(m_width, m_height, true, 1, bgfx_format, flags | BGFX_TEXTURE_RT | BGFX_TEXTURE_BLIT_DST);
   m_mips_texture = bgfx::createTexture2D(m_width, m_height, false, 1, bgfx_format, flags, data); // Base texture without mipmaps
   m_mips_gpu_frame = bgfx::getStats()->gpuFrameNum;
   bgfx::Attachment m_mips_attachment;
   m_mips_attachment.init(m_texture);
   m_mips_framebuffer = bgfx::createFrameBuffer(1, &m_mips_attachment);
   // Blit to RT
   if (m_rd->m_activeViewId < 0)
      m_rd->NextView();
   // FIXME BGFX a clean GPU mipmap generation with Kaiser filter would be better than doing a blit to trigger render target mipmap generation, to be refactored when fixing dynamic texture
   // For a simple and readable reference, see (paramters: alpha=4, stretch=1, m_width=filter half width):
   //   https://github.com/castano/nvidia-texture-tools/blob/aeddd65f81d36d8cb7b169b469ef25156666077e/src/nvimage/Filter.cpp#L257
   //   https://github.com/castano/nvidia-texture-tools/blob/aeddd65f81d36d8cb7b169b469ef25156666077e/src/nvimage/Filter.cpp#L64
   bgfx::blit(m_rd->m_activeViewId, m_texture, 0, 0, m_mips_texture);
   // Force RT resolution, in turns causing mipmap generation
   m_rd->NextView();
   bgfx::setViewFrameBuffer(m_rd->m_activeViewId, m_mips_framebuffer);
   // Get back to the rendering view
   RenderTarget* activeRT = RenderTarget::GetCurrentRenderTarget();
   int activeLayer = RenderTarget::GetCurrentRenderLayer();
   if (activeRT)
   {
      RenderTarget::OnFrameFlushed();
      activeRT->Activate();
   }

#elif defined(ENABLE_OPENGL)
   m_texTarget = GL_TEXTURE_2D;
   colorFormat format;
   if (surf->m_format == BaseTexture::RGB)
      format = colorFormat::RGB;
   else if (surf->m_format == BaseTexture::RGBA)
      format = colorFormat::RGBA;
   else if (surf->m_format == BaseTexture::SRGB)
      format = colorFormat::SRGB;
   else if (surf->m_format == BaseTexture::SRGBA)
      format = colorFormat::SRGBA;
   else if (surf->m_format == BaseTexture::RGB_FP16)
      format = colorFormat::RGB16F;
   else if (surf->m_format == BaseTexture::RGBA_FP16)
      format = colorFormat::RGBA16F;
   else if (surf->m_format == BaseTexture::RGB_FP32)
      format = colorFormat::RGB32F;
   else if (surf->m_format == BaseTexture::BW)
      format = colorFormat::GREY8;
   else
      assert(false); // Unsupported image format
   if (force_linear_rgb)
   {
      if (format == colorFormat::SRGB)
         format = colorFormat::RGB;
      else if (format == colorFormat::SRGBA)
         format = colorFormat::RGBA;
   }
   m_texture = CreateTexture(surf, 0, format, 0);
   m_isLinear = format != colorFormat::SRGB && format != colorFormat::SRGBA;

#elif defined(ENABLE_DX9)
   colorFormat texformat;
   IDirect3DTexture9* sysTex = CreateSystemTexture(surf, force_linear_rgb, texformat);

   m_isLinear = texformat == colorFormat::RGBA16F || texformat == colorFormat::RGBA32F || force_linear_rgb;

   HRESULT hr = m_rd->GetCoreDevice()->CreateTexture(m_width, m_height, (texformat != colorFormat::DXT5 && m_rd->m_autogen_mipmap) ? 0 : sysTex->GetLevelCount(),
      (texformat != colorFormat::DXT5 && m_rd->m_autogen_mipmap) ? textureUsage::AUTOMIPMAP : 0, (D3DFORMAT)texformat, (D3DPOOL)memoryPool::DEFAULT, &m_texture, nullptr);
   if (FAILED(hr))
      ReportError("Fatal Error: out of VRAM!", hr, __FILE__, __LINE__);

   hr = m_rd->GetCoreDevice()->UpdateTexture(sysTex, m_texture);
   if (FAILED(hr))
      ReportError("Fatal Error: uploading texture failed!", hr, __FILE__, __LINE__);

   SAFE_RELEASE(sysTex);

   if (texformat != colorFormat::DXT5 && m_rd->m_autogen_mipmap)
      m_texture->GenerateMipSubLevels(); // tell driver that now is a good time to generate mipmaps
#endif
}

#if defined(ENABLE_BGFX)
Sampler::Sampler(RenderDevice* rd, SurfaceType type, bgfx::TextureHandle bgfxTexture, int width, int height, bool ownTexture, bool linear_rgb, const SamplerAddressMode clampu, const SamplerAddressMode clampv, const SamplerFilter filter)
   : m_type(type)
   , m_rd(rd)
   , m_dirty(false)
   , m_ownTexture(ownTexture)
   , m_clampu(clampu)
   , m_clampv(clampv)
   , m_filter(filter)
   , m_texture(bgfxTexture)
   , m_width(width)
   , m_height(height)
   , m_isLinear(linear_rgb)
{
}

#elif defined(ENABLE_OPENGL)
Sampler::Sampler(RenderDevice* rd, SurfaceType type, GLuint glTexture, bool ownTexture, bool force_linear_rgb, const SamplerAddressMode clampu, const SamplerAddressMode clampv, const SamplerFilter filter)
   : m_type(type)
   , m_rd(rd)
   , m_dirty(false)
   , m_ownTexture(ownTexture)
   , m_clampu(clampu)
   , m_clampv(clampv)
   , m_filter(filter)
{
   switch (m_type)
   {
   case RT_DEFAULT: m_texTarget = GL_TEXTURE_2D; break;
   case RT_STEREO: m_texTarget = GL_TEXTURE_2D_ARRAY; break;
   case RT_CUBEMAP: m_texTarget = GL_TEXTURE_CUBE_MAP; break;
   default: assert(false);
   }
#ifndef __OPENGLES__
   glGetTexLevelParameteriv(m_texTarget, 0, GL_TEXTURE_WIDTH, &m_width);
   glGetTexLevelParameteriv(m_texTarget, 0, GL_TEXTURE_HEIGHT, &m_height);
#else
   m_width = 0;
   m_height = 0;
#endif
   int internal_format;
#ifndef __OPENGLES__
   glGetTexLevelParameteriv(m_texTarget, 0, GL_TEXTURE_INTERNAL_FORMAT, &internal_format);
#else
   internal_format = SRGBA;
#endif
   m_isLinear = !((internal_format == SRGB) || (internal_format == SRGBA) || (internal_format == SDXT5) || (internal_format == SBC7)) || force_linear_rgb;
   m_texture = glTexture;
}

#elif defined(ENABLE_DX9)
Sampler::Sampler(RenderDevice* rd, IDirect3DTexture9* dx9Texture, bool ownTexture, bool force_linear_rgb, const SamplerAddressMode clampu, const SamplerAddressMode clampv, const SamplerFilter filter)
   : m_type(SurfaceType::RT_DEFAULT)
   , m_rd(rd)
   , m_dirty(false)
   , m_ownTexture(ownTexture)
   , m_clampu(clampu)
   , m_clampv(clampv)
   , m_filter(filter)
{
   D3DSURFACE_DESC desc;
   dx9Texture->GetLevelDesc(0, &desc);
   m_width = desc.Width;
   m_height = desc.Height;
   m_isLinear = desc.Format == D3DFMT_A16B16G16R16F || desc.Format == D3DFMT_A32B32G32R32F || force_linear_rgb;
   m_texture = dx9Texture;
}
#endif

Sampler::~Sampler()
{
   m_rd->UnbindSampler(this);
   
   #if defined(ENABLE_BGFX)
   bgfx::destroy(m_texture);
   if (bgfx::isValid(m_mips_framebuffer))
      bgfx::destroy(m_mips_framebuffer);
   if (bgfx::isValid(m_mips_texture))
      bgfx::destroy(m_mips_texture);

   #elif defined(ENABLE_OPENGL)
   Unbind();
   if (m_ownTexture)
      glDeleteTextures(1, &m_texture);
   
   #elif defined(ENABLE_DX9)
   if (m_ownTexture)
      SAFE_RELEASE(m_texture);
   #endif
}

#if defined(ENABLE_BGFX)
bgfx::TextureHandle Sampler::GetCoreTexture()
{
   if (bgfx::isValid(m_mips_texture) && bgfx::getStats()->gpuFrameNum >= m_mips_gpu_frame + 2)
   {
      // Mipmaps have been generated, we can release the framebuffer and base version of the texture
      bgfx::destroy(m_mips_texture);
      bgfx::destroy(m_mips_framebuffer);
      m_mips_texture = BGFX_INVALID_HANDLE;
      m_mips_framebuffer = BGFX_INVALID_HANDLE;
      m_mips_gpu_frame = 0;
   }
   return m_texture;
}
#endif

void Sampler::Unbind()
{
#ifdef ENABLE_OPENGL
   for (auto binding : m_bindings)
   {
      binding->sampler = nullptr;
      glActiveTexture(GL_TEXTURE0 + binding->unit);
      glBindTexture(m_texTarget, 0);
   }
   m_bindings.clear();
#endif
}

void Sampler::UpdateTexture(BaseTexture* const surf, const bool force_linear_rgb)
{
#if defined(ENABLE_BGFX)
   // FIXME BGFX guarantee that surf->data won't be deallaocated for the 2 next frames
   // FIXME BGFX manage mipmaps for dynamic textures
   bgfx::updateTexture2D(m_texture, 0, 0, 0, 0, m_width, m_height, bgfx::makeRef(surf->data(), surf->height() * surf->pitch()));

#elif defined(ENABLE_OPENGL)
   colorFormat format;
   if (surf->m_format == BaseTexture::RGB)
      format = colorFormat::RGB;
   else if (surf->m_format == BaseTexture::RGBA)
      format = colorFormat::RGBA;
   else if (surf->m_format == BaseTexture::SRGB)
      format = colorFormat::SRGB;
   else if (surf->m_format == BaseTexture::SRGBA)
      format = colorFormat::SRGBA;
   else if (surf->m_format == BaseTexture::RGB_FP16)
      format = colorFormat::RGB16F;
   else if (surf->m_format == BaseTexture::RGBA_FP16)
      format = colorFormat::RGBA16F;
   else if (surf->m_format == BaseTexture::RGB_FP32)
      format = colorFormat::RGB32F;
   else if (surf->m_format == BaseTexture::BW)
      format = colorFormat::GREY8;
   else
      assert(false); // Unsupported image format
   if (force_linear_rgb)
   {
      if (format == colorFormat::SRGB)
         format = colorFormat::RGB;
      else if (format == colorFormat::SRGBA)
         format = colorFormat::RGBA;
   }
   const GLuint col_type = ((format == RGBA32F) || (format == RGB32F)) ? GL_FLOAT : ((format == RGB16F) || (format == RGBA16F)) ? GL_HALF_FLOAT : GL_UNSIGNED_BYTE;
   const GLuint col_format = ((format == GREY8) || (format == RED16F))                                                                                                      ? GL_RED
      : ((format == GREY_ALPHA) || (format == RG16F))                                                                                                                       ? GL_RG
      : ((format == RGB) || (format == RGB8) || (format == SRGB) || (format == SRGB8) || (format == RGB5) || (format == RGB10) || (format == RGB16F) || (format == RGB32F)) ? GL_RGB
                                                                                                                                                                            : GL_RGBA;
   // Update bind cache
   auto tex_unit = m_rd->m_samplerBindings.back();
   if (tex_unit->sampler != nullptr)
      tex_unit->sampler->m_bindings.erase(tex_unit);
   tex_unit->sampler = nullptr;
   glActiveTexture(GL_TEXTURE0 + tex_unit->unit);

   glBindTexture(m_texTarget, m_texture);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexSubImage2D(m_texTarget, 0, 0, 0, surf->width(), surf->height(), col_format, col_type, surf->data());
   glGenerateMipmap(m_texTarget); // Generate mip-maps
   glBindTexture(m_texTarget, 0);
   
#elif defined(ENABLE_DX9)
   colorFormat texformat;
   IDirect3DTexture9* sysTex = CreateSystemTexture(surf, force_linear_rgb, texformat);
   CHECKD3D(m_rd->GetCoreDevice()->UpdateTexture(sysTex, m_texture));
   SAFE_RELEASE(sysTex);
   
#endif
   m_rd->m_curTextureUpdates++;
}

void Sampler::SetClamp(const SamplerAddressMode clampu, const SamplerAddressMode clampv)
{
   m_clampu = clampu;
   m_clampv = clampv;
}

void Sampler::SetFilter(const SamplerFilter filter)
{
   m_filter = filter;
}

void Sampler::SetName(const string& name)
{
   #if defined(ENABLE_BGFX)
   bgfx::setName(m_texture, name.c_str());
   if (bgfx::isValid(m_mips_texture))
      bgfx::setName(m_mips_texture, name.c_str());
   #elif defined(ENABLE_OPENGL) && !defined(__OPENGLES__)
   if (GLAD_GL_VERSION_4_3)
      glObjectLabel(GL_TEXTURE, m_texture, (GLsizei) name.length(), name.c_str());
   #endif
}

#if defined(ENABLE_BGFX)

#elif defined(ENABLE_OPENGL)
GLuint Sampler::CreateTexture(BaseTexture* const surf, unsigned int Levels, colorFormat Format, int stereo)
{
   unsigned int Width = surf->width();
   unsigned int Height = surf->height();
   void* data = surf->data();

   const GLuint col_type = ((Format == RGBA32F) || (Format == RGB32F)) ? GL_FLOAT : ((Format == RGB16F) || (Format == RGBA16F)) ? GL_HALF_FLOAT : GL_UNSIGNED_BYTE;
   const GLuint col_format = ((Format == GREY8) || (Format == RED16F))                                                                                                      ? GL_RED
      : ((Format == GREY_ALPHA) || (Format == RG16F))                                                                                                                       ? GL_RG
      : ((Format == RGB) || (Format == RGB8) || (Format == SRGB) || (Format == SRGB8) || (Format == RGB5) || (Format == RGB10) || (Format == RGB16F) || (Format == RGB32F)) ? GL_RGB
                                                                                                                                                                            : GL_RGBA;
   const bool col_is_linear = (Format == GREY8) || (Format == RED16F) || (Format == GREY_ALPHA) || (Format == RG16F) || (Format == RGB5) || (Format == RGB) || (Format == RGB8)
      || (Format == RGB10) || (Format == RGB16F) || (Format == RGB32F) || (Format == RGBA16F) || (Format == RGBA32F) || (Format == RGBA) || (Format == RGBA8) || (Format == RGBA10)
      || (Format == DXT5) || (Format == BC6U) || (Format == BC6S) || (Format == BC7);

   // Update bind cache
   auto tex_unit = m_rd->m_samplerBindings.back();
   if (tex_unit->sampler != nullptr)
      tex_unit->sampler->m_bindings.erase(tex_unit);
   tex_unit->sampler = nullptr;
   glActiveTexture(GL_TEXTURE0 + tex_unit->unit);

   GLuint texture;
   glGenTextures(1, &texture);
   glBindTexture(m_texTarget, texture);

   if (Format == GREY8)
   { //Hack so that GL_RED behaves as GL_GREY
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_R, GL_RED);
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_G, GL_RED);
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_B, GL_RED);
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
      Format = RGB8;
   }
   else if (Format == GREY_ALPHA)
   { //Hack so that GL_RG behaves as GL_GREY_ALPHA
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_R, GL_RED);
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_G, GL_RED);
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_B, GL_RED);
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
      Format = RGB8;
   }
   else
   { //Default
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_R, GL_RED);
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
      glTexParameteri(m_texTarget, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
   }

   colorFormat comp_format = Format;
   if (m_rd->m_compress_textures && ((Width & 3) == 0) && ((Height & 3) == 0) && (Width > 256) && (Height > 256))
   {
      if (col_type == GL_FLOAT || col_type == GL_HALF_FLOAT)
      {
#ifndef __OPENGLES__
         if (GLAD_GL_ARB_texture_compression_bptc)
            comp_format = surf->IsSigned() ? colorFormat::BC6S : colorFormat::BC6U;
#endif
      }
#ifndef __OPENGLES__
      else if (GLAD_GL_ARB_texture_compression_bptc)
         comp_format = col_is_linear ? colorFormat::BC7 : colorFormat::SBC7;
#endif
      else
         comp_format = col_is_linear ? colorFormat::DXT5 : colorFormat::SDXT5;
   }

   const int num_mips = (int)std::log2(float(max(Width, Height))) + 1;
#ifndef __OPENGLES__
   if (m_rd->getGLVersion() >= 403)
#endif
      glTexStorage2D(m_texTarget, num_mips, comp_format, Width, Height);
#ifndef __OPENGLES__
   else
   { // should never be triggered nowadays
      GLsizei w = Width;
      GLsizei h = Height;
      for (int i = 0; i < num_mips; i++)
      {
         glTexImage2D(m_texTarget, i, comp_format, w, h, 0, col_format, col_type, nullptr);
         w = max(1, (w / 2));
         h = max(1, (h / 2));
      }
   }
#else
   PLOGD.printf("col_is_linear=%d, comp_format: %s (0x%04x), col_format=%s (0x%04x), col_type=%s (0x%04x)",
          col_is_linear, 
          gl_to_string(comp_format), comp_format,
          gl_to_string(col_format), col_format,
          gl_to_string(col_type), col_type);
#endif

   if (data)
   {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      // This line causes a false GLIntercept error log on OpenGL >= 403 since the image is initialized through TexStorage and not TexImage (expected by GLIntercept)
      // InterceptImage::SetImageDirtyPost - Flagging an image as dirty when it is not ready/init?
      glTexSubImage2D(m_texTarget, 0, 0, 0, Width, Height, col_format, col_type, data);
      glGenerateMipmap(m_texTarget); // Generate mip-maps, when using TexStorage will generate same amount as specified in TexStorage, otherwise good idea to limit by GL_TEXTURE_MAX_LEVEL
   }
   return texture;
}

#elif defined(ENABLE_DX9)

IDirect3DTexture9* Sampler::CreateSystemTexture(BaseTexture* const surf, const bool force_linear_rgb, colorFormat& texformat)
{
   const unsigned int texwidth = surf->width();
   const unsigned int texheight = surf->height();
   const BaseTexture::Format basetexformat = surf->m_format;

   if (basetexformat == BaseTexture::RGB_FP16 || basetexformat == BaseTexture::RGBA_FP16)
   {
      texformat = colorFormat::RGBA16F;
   }
   else if (basetexformat == BaseTexture::RGB_FP32)
   {
      texformat = colorFormat::RGBA32F;
   }
   else
   {
      texformat = colorFormat::RGBA8;
      if (m_rd->m_compress_textures && ((texwidth & 3) == 0) && ((texheight & 3) == 0) && (texwidth > 256) && (texheight > 256))
         texformat = colorFormat::DXT5;
   }

   IDirect3DTexture9* sysTex;
   HRESULT hr = m_rd->GetCoreDevice()->CreateTexture(
      texwidth, texheight, (texformat != colorFormat::DXT5 && m_rd->m_autogen_mipmap) ? 1 : 0, 0, (D3DFORMAT)texformat, (D3DPOOL)memoryPool::SYSTEM, &sysTex, nullptr);
   if (FAILED(hr))
   {
      ReportError("Fatal Error: unable to create texture!", hr, __FILE__, __LINE__);
   }

   D3DLOCKED_RECT locked;
   CHECKD3D(sysTex->LockRect(0, &locked, nullptr, 0));

   // copy data into system memory texture
   if (basetexformat == BaseTexture::RGB_FP32 && texformat == colorFormat::RGBA32F)
   {
      float* const __restrict pdest = (float*)locked.pBits;
      const float* const __restrict psrc = (float*)(surf->data());
      for (size_t i = 0; i < (size_t)texwidth * texheight; ++i)
      {
         pdest[i * 4 + 0] = psrc[i * 3 + 0];
         pdest[i * 4 + 1] = psrc[i * 3 + 1];
         pdest[i * 4 + 2] = psrc[i * 3 + 2];
         pdest[i * 4 + 3] = 1.f;
      }
   }
   else if (basetexformat == BaseTexture::RGB_FP16 && texformat == colorFormat::RGBA16F)
   {
      unsigned short* const __restrict pdest = (unsigned short*)locked.pBits;
      const unsigned short* const __restrict psrc = (unsigned short*)(surf->data());
      const unsigned short one16 = float2half_noLUT(1.f);
      for (size_t i = 0; i < (size_t)texwidth * texheight; ++i)
      {
         pdest[i * 4 + 0] = psrc[i * 3 + 0];
         pdest[i * 4 + 1] = psrc[i * 3 + 1];
         pdest[i * 4 + 2] = psrc[i * 3 + 2];
         pdest[i * 4 + 3] = one16;
      }
   }
   else if (basetexformat == BaseTexture::RGBA_FP16 && texformat == colorFormat::RGBA16F)
   {
      unsigned short* const __restrict pdest = (unsigned short*)locked.pBits;
      const unsigned short* const __restrict psrc = (unsigned short*)(surf->data());
      memcpy(pdest, psrc, (size_t)texwidth * texheight*4*sizeof(unsigned short));
   }
   else if ((basetexformat == BaseTexture::BW) && texformat == colorFormat::RGBA8)
   {
      BYTE* const __restrict pdest = (BYTE*)locked.pBits;
      const BYTE* const __restrict psrc = (BYTE*)(surf->data());
      for (size_t i = 0; i < (size_t)texwidth * texheight; ++i)
      {
         pdest[i * 4 + 0] =
         pdest[i * 4 + 1] =
         pdest[i * 4 + 2] = psrc[i];
         pdest[i * 4 + 3] = 255u;
      }
   }
   else if ((basetexformat == BaseTexture::RGB || basetexformat == BaseTexture::SRGB) && texformat == colorFormat::RGBA8)
   {
      copy_rgb_rgba<true>((unsigned int*)locked.pBits, surf->data(), (size_t)texwidth * texheight);
   }
   else if ((basetexformat == BaseTexture::RGBA || basetexformat == BaseTexture::SRGBA) && texformat == colorFormat::RGBA8)
   {
      copy_bgra_rgba<false>((unsigned int*)locked.pBits, (const unsigned int*)(surf->data()), (size_t)texwidth * texheight);

      /* IDirect3DSurface9* sysSurf;
      CHECKD3D(sysTex->GetSurfaceLevel(0, &sysSurf));
      RECT sysRect;
      sysRect.top = 0;
      sysRect.left = 0;
      sysRect.right = texwidth;
      sysRect.bottom = texheight;
      CHECKD3D(D3DXLoadSurfaceFromMemory(sysSurf, nullptr, nullptr, surf->data(), (D3DFORMAT)colorFormat::RGBA8, surf->pitch(), nullptr, &sysRect, D3DX_FILTER_NONE, 0));
      SAFE_RELEASE_NO_RCC(sysSurf);*/
   }
   else
      assert(false); // Unsupported image format

   CHECKD3D(sysTex->UnlockRect(0));

   if (!(texformat != colorFormat::DXT5 && m_rd->m_autogen_mipmap))
      // normal maps or float textures are already in linear space!
      CHECKD3D(D3DXFilterTexture(sysTex, nullptr, D3DX_DEFAULT,
         (texformat == colorFormat::RGBA16F || texformat == colorFormat::RGBA32F || force_linear_rgb) ? D3DX_FILTER_TRIANGLE : (D3DX_FILTER_TRIANGLE | D3DX_FILTER_SRGB)));

   return sysTex;
}
#endif
