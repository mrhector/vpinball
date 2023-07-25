#include "stdafx.h"
#include "FlexDMDGroupActor.h"
#include "FlexDMDActionFactory.h"
#include "FlexDMDLabelActor.h"
#include "FlexDMDVideoActor.h"
#include "FlexDMDFrameActor.h"
#include "FlexDMDImageActor.h"
#include "FlexDMD.h"

FlexDMDGroupActor::FlexDMDGroupActor()
{
}

FlexDMDGroupActor::~FlexDMDGroupActor()
{
}

CComObject<FlexDMDGroupActor>* FlexDMDGroupActor::Create(FlexDMD* pFlexDMD, string name)
{
   CComObject<FlexDMDGroupActor>* obj = nullptr;
   CComObject<FlexDMDGroupActor>::CreateInstance(&obj);
   obj->AddRef();

   obj->m_pFlexDMD = pFlexDMD;
   obj->SetName(name);

   return obj;
}

void FlexDMDGroupActor::OnStageStateChanged()
{
   for (FlexDMDActor* child : m_children)
      child->SetOnStage(GetOnStage());
}

void FlexDMDGroupActor::Update(float delta)
{
   FlexDMDActor::Update(delta);
   if (!GetOnStage()) return;
   int i = 0;
   while (i < m_children.size()) {
      FlexDMDActor* child = m_children[i];
      child->Update(delta);
      if (i < m_children.size() && child == m_children[i])
         i++;
   }
}

void FlexDMDGroupActor::Draw(SDL_Surface* graphics)
{
   if (GetVisible()) {
      //graphics.TranslateTransform(m_x, m_y);
      if (m_clip) {
         SDL_Rect clipRegion = { 0, 0, (int)GetWidth(), (int)GetHeight() };
         SDL_SetClipRect(graphics, &clipRegion);

         FlexDMDActor::Draw(graphics);

         for (FlexDMDActor* child : m_children)
            child->Draw(graphics);

         SDL_SetClipRect(graphics, NULL);
      }
      else {
         FlexDMDActor::Draw(graphics);

         for (FlexDMDActor* child : m_children)
            child->Draw(graphics);
      }
      //graphics.TranslateTransform(-m_x, -m_y);
   }
}

FlexDMDGroupActor* FlexDMDGroupActor::GetRoot()
{
   FlexDMDGroupActor* root = this;
   while (root->GetParent() != NULL)
      root = root->GetParent();
   return root;
}

FlexDMDActor* FlexDMDGroupActor::Get(string name)
{
   if (GetName() == name)
      return this;

   LONG runtimeVersion;
   m_pFlexDMD->get_RuntimeVersion(&runtimeVersion);

   if (runtimeVersion <= 1008) {
      for (FlexDMDActor* child : m_children) {
         if (child->GetName() == name)
            return child;

         FlexDMDGroupActor* group = dynamic_cast<FlexDMDGroupActor*>(child);
         if (group != NULL) {
            FlexDMDActor* found = group->Get(name);
            if (found)
               return found;
         }
      }
   }
   else {
      for (FlexDMDActor* child : m_children) {
         size_t pos = name.find('/');
         if (pos == std::string::npos) {
            // direct child node search 'xx'
            for (FlexDMDActor* child : m_children) {
               if (child->GetName() == name)
                  return child;
            }
         }
         else if (pos == 0) {
            // absolute path from root '/xx/yy/zz', note that stage node is named 'Stage'
            return GetRoot()->Get(name.substr(1));
         }
         else
         {
            // relative path from current group 'xx/yy/zz'
            string groupName = name.substr(0, pos);
            for (FlexDMDActor* child : m_children) {
               FlexDMDGroupActor* group = dynamic_cast<FlexDMDGroupActor*>(child);
               if (group != NULL && group->GetName() == groupName)
                  return group->Get(name.substr(pos + 1));
            }
         }
      }
   }

   return NULL;
}

STDMETHODIMP FlexDMDGroupActor::get_Clip(VARIANT_BOOL *pRetVal)
{
   *pRetVal = m_clip ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP FlexDMDGroupActor::put_Clip(VARIANT_BOOL pRetVal)
{
   m_clip = (pRetVal == VARIANT_TRUE);

   return S_OK;
}

STDMETHODIMP FlexDMDGroupActor::get_ChildCount(LONG *pRetVal)
{
   *pRetVal = m_children.size();

   return S_OK;
}

STDMETHODIMP FlexDMDGroupActor::HasChild(BSTR Name,VARIANT_BOOL *pRetVal)
{
   *pRetVal = Get(MakeString(Name)) ? VARIANT_TRUE : VARIANT_FALSE;

   return S_OK;
}

STDMETHODIMP FlexDMDGroupActor::GetGroup(BSTR Name,IGroupActor **pRetVal)
{
   FlexDMDActor* actor = Get(MakeString(Name));

   if (actor) {
      FlexDMDGroupActor* group = dynamic_cast<FlexDMDGroupActor*>(actor);
      if (group)
         return group->QueryInterface(IID_IGroupActor, (void**)pRetVal);
   }

   *pRetVal = NULL;

   return S_OK;
}

STDMETHODIMP FlexDMDGroupActor::GetFrame(BSTR Name,IFrameActor **pRetVal)
{
   FlexDMDActor* actor = Get(MakeString(Name));

   if (actor) {
      FlexDMDFrameActor* frame = dynamic_cast<FlexDMDFrameActor*>(actor);
      if (frame)
         return frame->QueryInterface(IID_IFrameActor, (void**)pRetVal);
   }

   *pRetVal = NULL;

   return S_OK;
}

STDMETHODIMP FlexDMDGroupActor::GetLabel(BSTR Name,ILabelActor **pRetVal)
{
   FlexDMDActor* actor = Get(MakeString(Name));

   if (actor) {
      FlexDMDLabelActor* label = dynamic_cast<FlexDMDLabelActor*>(actor);
      if (label)
         return label->QueryInterface(IID_ILabelActor, (void**)pRetVal);
   }

   *pRetVal = NULL;

   return S_OK;
}

STDMETHODIMP FlexDMDGroupActor::GetVideo(BSTR Name,IVideoActor **pRetVal)
{
   FlexDMDActor* actor = Get(MakeString(Name));

   if (actor) {
      FlexDMDAnimatedActor* video = dynamic_cast<FlexDMDAnimatedActor*>(actor);
      if (video)
         return video->QueryInterface(IID_IVideoActor, (void**)pRetVal);
   }

   *pRetVal = NULL;

   return S_OK;
}

STDMETHODIMP FlexDMDGroupActor::GetImage(BSTR Name,IImageActor **pRetVal)
{
   FlexDMDActor* actor = Get(MakeString(Name));

   if (actor) {
      FlexDMDImageActor* image = dynamic_cast<FlexDMDImageActor*>(actor);
      if (image)
         return image->QueryInterface(IID_IImageActor, (void**)pRetVal);
   }

   *pRetVal = NULL;

   return S_OK;
}

STDMETHODIMP FlexDMDGroupActor::RemoveAll()
{
   PLOGI.printf("Remove all children %s", GetName().c_str());

   for (FlexDMDActor* child : m_children) {
      child->SetParent(NULL);
      child->SetOnStage(false);
   }
   m_children.clear();

   return S_OK;
}

STDMETHODIMP FlexDMDGroupActor::AddActor(IUnknown *child)
{
   FlexDMDActor* actor = dynamic_cast<FlexDMDActor*>(child);

   if (!actor)
      return E_FAIL;

   actor->Remove();
   actor->SetParent(this);
   m_children.push_back(actor);
   actor->SetOnStage(GetOnStage());

   return S_OK;
}

STDMETHODIMP FlexDMDGroupActor::RemoveActor(IUnknown *child)
{
   FlexDMDActor* actor = dynamic_cast<FlexDMDActor*>(child);

   if (!actor)
      return E_FAIL;

   actor->SetParent(NULL);

   m_children.erase(std::remove_if(m_children.begin(), m_children.end(), [actor](FlexDMDActor* p) {
      return p == actor;
   }), m_children.end());

   actor->SetOnStage(false);

   return S_OK;
}

void FlexDMDGroupActor::AddActor(FlexDMDActor* actor)
{
   if (!actor)
      return;

   actor->Remove();
   actor->SetParent(this);
   m_children.push_back(actor);
   actor->SetOnStage(GetOnStage());
}

void FlexDMDGroupActor::AddActorAt(FlexDMDActor* actor, int index)
{
   if (!actor)
      return;

   actor->Remove();
   actor->SetParent(this);
   m_children.insert(m_children.begin() + index, actor);
   actor->SetOnStage(GetOnStage());
}

void FlexDMDGroupActor::RemoveActor(FlexDMDActor* actor)
{
   if (!actor)
      return;

   actor->SetParent(NULL);

   m_children.erase(std::remove_if(m_children.begin(), m_children.end(), [actor](FlexDMDActor* p) {
      return p == actor;
   }), m_children.end());

   actor->SetOnStage(false);
}
