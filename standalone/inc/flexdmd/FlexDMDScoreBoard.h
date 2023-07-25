#pragma once

#include "flexdmd_i.h"

#include "FlexDMDGroupActor.h"
#include "FlexDMDFont.h"
#include "FlexDMDLabelActor.h"
#include "FlexDMDImageActor.h"

class FlexDMDScoreBoard : public FlexDMDGroupActor
{
public:
   FlexDMDScoreBoard();
   ~FlexDMDScoreBoard();

   static CComObject<FlexDMDScoreBoard>* Create(FlexDMD* pFlexDMD, FlexDMDFont* pScoreFont, FlexDMDFont* pHighlightFont, FlexDMDFont* pTextFont);

   void SetBackground(FlexDMDActor* background);
   void SetNPlayers(int nPlayers);
   void SetFonts(FlexDMDFont* scoreFont, FlexDMDFont* highlightFont, FlexDMDFont* textFont);
   void SetHighlightedPlayer(int player);
   void SetScore(LONG score1, LONG score2, LONG score3, LONG score4);

   virtual void Update(float delta);
   virtual void Draw(SDL_Surface* graphics);

   FlexDMDFont* m_pScoreFont;
   FlexDMDFont* m_pHighlightFont;
   FlexDMDFont* m_pTextFont;
   FlexDMDLabelActor* m_pLowerLeft;
   FlexDMDLabelActor* m_pLowerRight;
   FlexDMDLabelActor* m_pScores[4];
   FlexDMDActor* m_pBackground;
   int m_highlightedPlayer;
};