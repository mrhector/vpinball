#pragma once

#include "../actors/FlexDMDGroupActor.h"
#include "../tweeny-3.2.0.h"

typedef enum {
   AnimationType_FadeIn = 0, // Fade from black to scene
   AnimationType_FadeOut = 1, // Fade from scene to black
   AnimationType_ZoomIn = 2, // zoom from a centered small dmd to full size
   AnimationType_ZoomOut = 3, // zoom from a full sized dmd to an oversize one
   AnimationType_ScrollOffLeft = 4,
   AnimationType_ScrollOffRight = 5,
   AnimationType_ScrollOnLeft = 6,
   AnimationType_ScrollOnRight = 7,
   AnimationType_ScrollOffUp = 8,
   AnimationType_ScrollOffDown = 9,
   AnimationType_ScrollOnUp = 10,
   AnimationType_ScrollOnDown = 11,
   AnimationType_FillFadeIn = 12, // fade from black to white (the scene won't be seen)
   AnimationType_FillFadeOut = 13, // fade from white to black (the scene won't be seen)
   AnimationType_None = 14
} AnimationType;

class FlexDMDScene : public FlexDMDGroupActor
{
public:
   FlexDMDScene();
   ~FlexDMDScene();

   static CComObject<FlexDMDScene>* Create(FlexDMD* pFlexDMD, AnimationType animateIn, float pauseS, AnimationType animateOut, const string& id);

   void UpdateState();
   void Begin();
   float StartAnimation(AnimationType animation);
   bool IsFinished();

   virtual void SetVisible(bool visible) { FlexDMDGroupActor::SetVisible(visible); UpdateState(); }

   void SetAnimateIn(AnimationType animateIn) { m_animateIn = animateIn; };
   void SetPause(float pauseS) { m_pause = pauseS; };
   float GetTime() { return m_time; };
   void SetAnimateOut(AnimationType animateOut) { m_animateOut = animateOut; };
   tweeny::tween<float>& AddTween(float from, float to, float duration, std::function<bool (float)> callback);

   virtual void Update(float delta);
   virtual void OnStageStateChanged();

private:
   float m_inAnimLength;
   float m_outAnimLength;
   AnimationType m_animateIn;
   AnimationType m_animateOut;
   bool m_active;

   float m_time;
   float m_pause;

   vector<tweeny::tween<float>> m_tweens;
};