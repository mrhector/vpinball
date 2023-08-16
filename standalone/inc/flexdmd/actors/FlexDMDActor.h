#pragma once

#include "../FlexDMD.h"
#include "../Layout.h"
#include "../Graphics.h"

#include "FlexDMDActionFactory.h"
#include "Action.h"

class FlexDMDActor 
{
public:
   FlexDMDActor();
   ~FlexDMDActor();

   static FlexDMDActor* Create(FlexDMD* pFlexDMD, string name);

   STDMETHOD(get_Name)(BSTR *pRetVal);
   STDMETHOD(put_Name)(BSTR pRetVal);
   STDMETHOD(get_x)(single *pRetVal);
   STDMETHOD(put_x)(single pRetVal);
   STDMETHOD(get_y)(single *pRetVal);
   STDMETHOD(put_y)(single pRetVal);
   STDMETHOD(get_Width)(single *pRetVal);
   STDMETHOD(put_Width)(single pRetVal);
   STDMETHOD(get_Height)(single *pRetVal);
   STDMETHOD(put_Height)(single pRetVal);
   STDMETHOD(get_visible)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_visible)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_FillParent)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_FillParent)(VARIANT_BOOL pRetVal);
   STDMETHOD(get_ClearBackground)(VARIANT_BOOL *pRetVal);
   STDMETHOD(put_ClearBackground)(VARIANT_BOOL pRetVal);
   STDMETHOD(SetBounds)(single x, single y, single Width, single Height);
   STDMETHOD(SetPosition)(single x, single y);
   STDMETHOD(SetAlignedPosition)(single x, single y, Alignment alignment);
   STDMETHOD(SetSize)(single Width, single Height);
   STDMETHOD(get_PrefWidth)(single *pRetVal);
   STDMETHOD(get_PrefHeight)(single *pRetVal);
   STDMETHOD(Pack)();
   STDMETHOD(Remove)();
   STDMETHOD(get_ActionFactory)(IActionFactory **pRetVal);
   STDMETHOD(AddAction)(IUnknown *action);
   STDMETHOD(ClearActions)();

   void SetFlexDMD(FlexDMD* pFlexDMD) { m_pFlexDMD = pFlexDMD; }
   FlexDMD* GetFlexDMD() { return m_pFlexDMD; }
   void SetName(const string& szName) { m_szName = szName; }
   const string& GetName() { return m_szName; }
   single GetX() { return m_x; }
   void SetX(single x) { m_x = x; }
   single GetY() { return m_y; }
   void SetY(single y) { m_y = y; }
   single GetWidth() { return m_width; }
   void SetWidth(single width) { m_width = width; }
   single GetHeight() { return m_height; }
   void SetHeight(single height) { m_height = height; }
   virtual single GetPrefWidth() { return m_prefWidth; }
   void SetPrefWidth(single prefWidth) { m_prefWidth = prefWidth; }
   virtual single GetPrefHeight() { return m_prefHeight; }
   void SetPrefHeight(single prefHeight) { m_prefHeight = prefHeight; }
   bool GetVisible() { return m_visible; }
   virtual void SetVisible( bool visible ) { m_visible = visible; }
   bool GetOnStage() { return m_onStage; }
   void SetOnStage(bool onStage) { m_onStage = onStage; this->OnStageStateChanged(); }
   void SetFillParent(bool fillParent) { m_fillParent = fillParent; }
   FlexDMDGroupActor* GetParent() { return m_parent; }
   void SetParent(FlexDMDGroupActor* parent) { m_parent = parent; }

   virtual void OnStageStateChanged() {}
   virtual void Update(float secondsElapsed);
   virtual void Draw(Graphics* graphics);

protected:
   CComObject<FlexDMDActionFactory>* m_pActionFactory;

private:
   FlexDMD* m_pFlexDMD;
   vector<Action*> m_actions;
   bool m_onStage;
   string m_szName;
   single m_x;
   single m_y;
   single m_width;
   single m_height;
   FlexDMDGroupActor* m_parent;
   bool m_fillParent;
   bool m_clearBackground;
   single m_prefWidth;
   single m_prefHeight;
   bool m_visible;
};