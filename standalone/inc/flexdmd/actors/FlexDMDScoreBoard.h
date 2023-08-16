#pragma once

#include "FlexDMDGroupActor.h"
#include "FlexDMDFont.h"
#include "FlexDMDLabelActor.h"
#include "FlexDMDImageActor.h"

class FlexDMDScoreBoard : public FlexDMDGroupActor
{
public:
   FlexDMDScoreBoard();
   ~FlexDMDScoreBoard();

   static CComObject<FlexDMDScoreBoard>* Create(FlexDMD* pFlexDMD, Font* pScoreFont, Font* pHighlightFont, Font* pTextFont);

   void SetBackground(FlexDMDActor* background);
   void SetNPlayers(int nPlayers);
   void SetFonts(Font* scoreFont, Font* highlightFont, Font* textFont);
   void SetHighlightedPlayer(int player);
   void SetScore(LONG score1, LONG score2, LONG score3, LONG score4);

   virtual void Update(float delta);
   virtual void Draw(Graphics* graphics);

   Font* m_pScoreFont;
   Font* m_pHighlightFont;
   Font* m_pTextFont;
   FlexDMDLabelActor* m_pLowerLeft;
   FlexDMDLabelActor* m_pLowerRight;
   FlexDMDLabelActor* m_pScores[4];
   FlexDMDActor* m_pBackground;
   int m_highlightedPlayer;
};