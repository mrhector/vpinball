#include "stdafx.h"
#include "FlexDMDFont.h"
#include <sstream>

FlexDMDFont::FlexDMDFont()
{
}

FlexDMDFont::~FlexDMDFont()
{
   if (m_bitmapFont) {
      for (int i = 0; i < m_bitmapFont->m_pages.size(); ++i) {
         if (m_textures[i])
            SDL_FreeSurface(m_textures[i]);
      }

      delete m_bitmapFont;
   }
}

FlexDMDFont* FlexDMDFont::Create(FlexDMD* pFlexDMD, string font, OLE_COLOR tint, OLE_COLOR borderTint, LONG borderSize)
{
   FlexDMDFont* obj = new FlexDMDFont();
   obj->AddRef();

   obj->m_pFlexDMD = pFlexDMD;
   obj->m_font = font;
   obj->m_tint = tint;
   obj->m_borderTint = borderTint;
   obj->m_borderSize = borderSize;

   vector<string> parts;
   std::stringstream ss(font);
   string token;

   while (std::getline(ss, token, '&'))
      parts.push_back(token);

   string basePath;
   string path;

   if (parts[0].starts_with("FlexDMD.Resources.")) {
      basePath = g_pvp->m_szMyPath + "flexdmd" + PATH_SEPARATOR_CHAR;
      path = basePath + parts[0].substr(18);
   }
   else {
      string part = normalize_path_separators(parts[0]);

      basePath = g_pvp->m_currentTablePath + pFlexDMD->GetProjectFolder();
      path = basePath + part;

      std::size_t pos = part.find_last_of(PATH_SEPARATOR_CHAR);
      if (pos != std::string::npos)
         basePath += part.substr(0, pos + 1);
   }

   obj->m_bitmapFont = new BitmapFont();
   obj->m_bitmapFont->Load(path);

   obj->m_textures = new SDL_Surface*[obj->m_bitmapFont->m_pages.size()];
   memset((char*)obj->m_textures, 0, sizeof(SDL_Surface*) * obj->m_bitmapFont->m_pages.size());

   for (int i = 0; i < obj->m_bitmapFont->m_pages.size(); i++) {
      path = basePath + obj->m_bitmapFont->m_pages[i]->m_szFilename;

      SDL_Surface* pSurface = IMG_Load(path.c_str());
      obj->m_textures[i] = pSurface;

      if (pSurface) {
         PLOGD.printf("%s loaded", obj->m_bitmapFont->m_pages[i]->m_szFilename.c_str());
      }
      else {
         PLOGE.printf("Unable to load", obj->m_bitmapFont->m_pages[i]->m_szFilename.c_str());
      }
   }

   if (obj->m_borderSize > 0) {
      for (int i = 0; i < obj->m_bitmapFont->m_pages.size(); i++) {
         if (!obj->m_textures[i])
            continue;

         SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
         SDL_Surface* src = SDL_ConvertSurface(obj->m_textures[i], format, 0);
         SDL_FreeFormat(format);

         int w = src->w;
         int h = src->h;

         SDL_Surface* dst = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);

         UINT32 outline = SDL_MapRGBA(dst->format, GetRValue(borderTint), GetGValue(borderTint), GetBValue(borderTint), 255);
         SDL_LockSurface(src);
         SDL_LockSurface(dst);

         UINT32* pixels_src = (UINT32*)src->pixels;
         UINT32* pixels_dst = (UINT32*)dst->pixels;

         UINT8 r;
         UINT8 g;
         UINT8 b;
         UINT8 a;

         for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
               SDL_GetRGBA(pixels_src[y * w + x], src->format, &r, &g, &b, &a);

               if (a == 0)
                  continue;

               if (x > 0) {
                  if (y > 0)
                     pixels_dst[(y - 1) * w + (x - 1)] = outline;
                  pixels_dst[y * w + (x - 1)] = outline;
                  if (y < h - 1)
                     pixels_dst[(y + 1) * w + (x - 1)] = outline;
               }
               if (y > 0)
                  pixels_dst[(y - 1) * w + x] = outline;
               if (y < h - 1)
                  pixels_dst[(y + 1) * w + x] = outline;
               if (x < w - 1) {
                  if (y > 0)
                     pixels_dst[(y - 1) * w + (x + 1)] = outline;
                  pixels_dst[y * w + (x + 1)] = outline;
                  if (y < h - 1)
                     pixels_dst[(y + 1) * w + (x + 1)] = outline;
               }
            }
         }

         UINT8 tint_r = GetRValue(tint);
         UINT8 tint_g = GetGValue(tint);
         UINT8 tint_b = GetBValue(tint);

         for (int idx = 0; idx < dst->w * dst->h; ++idx) {
            SDL_GetRGBA(pixels_src[idx], src->format, &r, &g, &b, &a);

            if (a == 0)
               continue;

            r = (UINT8)SDL_min((r * tint_r) / 255, 255);
            g = (UINT8)SDL_min((g * tint_g) / 255, 255);
            b = (UINT8)SDL_min((b * tint_b) / 255, 255);

            pixels_dst[idx] = SDL_MapRGBA(dst->format, r, g, b, a);
         }

         SDL_UnlockSurface(src);
         SDL_UnlockSurface(dst);
         SDL_FreeSurface(src);
         SDL_FreeSurface(obj->m_textures[i]);

         obj->m_textures[i] = dst;
      }

      std::map<char, Character*> characters;

      for(const auto& pair : obj->m_bitmapFont->m_characters) {
         Character* character = pair.second;
         character->m_xadvance += 2;
         characters[pair.first] = character;
      }
      obj->m_bitmapFont->m_characters = characters;
   }
   else if (obj->m_tint != RGB(255, 255, 255)) {
      for (int i = 0; i < obj->m_bitmapFont->m_pages.size(); i++) {
         if (!obj->m_textures[i])
            continue;

         SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
         SDL_Surface* dst = SDL_ConvertSurface(obj->m_textures[i], format, 0);
         SDL_FreeFormat(format);

         int w = dst->w;
         int h = dst->h;

         SDL_LockSurface(dst);

         UINT32* pixels_dst = (UINT32*)dst->pixels;

         UINT8 r;
         UINT8 g;
         UINT8 b;
         UINT8 a;

         UINT8 tint_r = GetRValue(tint);
         UINT8 tint_g = GetGValue(tint);
         UINT8 tint_b = GetBValue(tint);

         for (int idx = 0; idx < dst->w * dst->h; ++idx) {
            SDL_GetRGBA(pixels_dst[idx], dst->format, &r, &g, &b, &a);

            r = (UINT8)SDL_min((r * tint_r) / 255, 255);
            g = (UINT8)SDL_min((g * tint_g) / 255, 255);
            b = (UINT8)SDL_min((b * tint_b) / 255, 255);

            pixels_dst[idx] = SDL_MapRGBA(dst->format, r, g, b, a);
         }

         SDL_UnlockSurface(dst);
         SDL_FreeSurface(obj->m_textures[i]);

         obj->m_textures[i] = dst;
      }
   }

   return obj;
}

void FlexDMDFont::DrawCharacter(SDL_Surface* graphics, char character, char previousCharacter, float& x, float& y)
{
   if (character == '\n') {
      x = 0;
      y += m_bitmapFont->m_lineHeight;
   }
   else {
      auto it = m_bitmapFont->m_characters.find(character);
      if (it != m_bitmapFont->m_characters.end()) {
         Character* data = it->second;

         int kerning = m_bitmapFont->GetKerning(previousCharacter, character);

         if (graphics) {
            SDL_Surface* pSource = m_textures[data->m_texturePage];

            if (pSource) {
               SDL_Rect dstRect = { (int)(x + data->m_offset.x + kerning), (int)(y + data->m_offset.y), data->m_bounds.w, data->m_bounds.h };
               SDL_BlitScaled(pSource, &data->m_bounds, graphics, &dstRect);
            }
         }
         x += data->m_xadvance + kerning;
      }
      else if ('a' <= character && character <= 'z' && m_bitmapFont->m_characters.find(toupper(character)) != m_bitmapFont->m_characters.end()) {
         m_bitmapFont->m_characters[character] = m_bitmapFont->m_characters[toupper(character)];
         DrawCharacter(graphics, character, previousCharacter, x, y);
      }
      else if (m_bitmapFont->m_characters.find(' ') != m_bitmapFont->m_characters.end()) {
         PLOGD.printf("Missing character 0x%02X replaced by ' '", character);
         m_bitmapFont->m_characters[character] = m_bitmapFont->m_characters[' '];
         DrawCharacter(graphics, character, previousCharacter, x, y);
      }
   }
}

SDL_Rect FlexDMDFont::MeasureFont(string text)
{
   DrawText_(NULL, 0, 0, text);
   return m_bitmapFont->MeasureFont(text);
}

void FlexDMDFont::DrawText_(SDL_Surface* graphics, float x, float y, string text)
{
   char previousCharacter = ' ';
   for (int i = 0; i < text.length(); i++) {
      char character = text[i];
      DrawCharacter(graphics, character, previousCharacter, x, y);
      previousCharacter = character;
   }
}