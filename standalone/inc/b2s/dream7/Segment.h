#pragma once

#include "../b2s_i.h"

#include "SegmentStyle.h"
#include "../graphics/Matrix.h"
#include "../graphics/Graphics.h"
#include "../graphics/GraphicsPath.h"

enum SegmentCap { 
   SegmentCap_Standard = 0,
   SegmentCap_Flat = 1,
   SegmentCap_MoreLeft = 2,
   SegmentCap_Left = 3,
   SegmentCap_MoreRight = 4,
   SegmentCap_Right = 5
};

class Segment 
{
public:
   Segment(const string& szName, float x, float y, float width, float height, float angle);
   Segment(const string& szName, float x, float y, float width, float height, float angle, SegmentCap topcap, SegmentCap bottomcap);
   Segment(const string& szName, float x, float y, float width, float height, float angle, SegmentCap topcap, SegmentCap bottomcap, float capangle);
   Segment(float x, float y, float radius);
   virtual ~Segment();

   void SetStyle(SegmentStyle* pStyle) { m_pStyle = pStyle; }
   bool IsOn() const { return m_on; }
   void SetOn(bool on) { m_on = on; }
   const string& GetName() const { return m_szName; }
   GraphicsPath* GetGlassPath();
   GraphicsPath* GetGlassPathTransformed();
   void AssignStyle();
   void Draw(B2S::Graphics* pGraphics);
   void DrawLight(B2S::Graphics* pGraphics);
   void Transform(B2S::Matrix* pMatrix);

private:
   void InitSegmentDot(float x, float y, float radius);
   void InitSegment(const string& szName, float x, float y, float width, float height, float angle, SegmentCap topcap, SegmentCap bottomcap, float capangle);
   void CreateLightData();
   void SetBulbSize();
   void LeftRightFromCap(SegmentCap nCap, float nWidth, float nCapangle, float& nLeft, float& nRight, float& nDelta);
   void PaintSegment(B2S::Graphics* pGraphics, GraphicsPath* pPath);
   void GetGlassData();
   void GetLightData();
   void ResetCacheData();
   void SetTransform(B2S::Graphics* pGraphics);

   SDL_FPoint m_focusScales;
   vector<SDL_FPoint> m_lights;
   SDL_FRect m_lightDot;
   vector<SDL_FPoint> m_points;
   SegmentStyle* m_pStyle;
   bool m_on;
   string m_szName;
   GraphicsPath* m_pGlassPath;
   float m_glow;
   float m_angle;
   float m_radius;
   GraphicsPath* m_pLightPath;
   B2S::Matrix* m_pOwnMatrix;
   B2S::Matrix* m_pExternMatrix;
};