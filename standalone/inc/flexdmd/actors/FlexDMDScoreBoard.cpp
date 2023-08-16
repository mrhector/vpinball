#include "stdafx.h"

#include "FlexDMDScoreBoard.h"

FlexDMDScoreBoard::FlexDMDScoreBoard()
{
   m_pScoreFont = NULL;
   m_pHighlightFont = NULL;
   m_pTextFont = NULL;
   m_pLowerLeft = NULL;
   m_pLowerRight = NULL;
   m_pScores[0] = NULL;
   m_pScores[1] = NULL;
   m_pScores[2] = NULL;
   m_pScores[3] = NULL;
   m_pBackground = NULL;
   m_highlightedPlayer = 0;
}

FlexDMDScoreBoard::~FlexDMDScoreBoard()
{
   m_pLowerLeft->Release();
   m_pLowerRight->Release();

   for (int i = 0; i < 4; i++)
      m_pScores[i]->Release();
}

CComObject<FlexDMDScoreBoard>* FlexDMDScoreBoard::Create(FlexDMD* pFlexDMD, Font* pScoreFont, Font* pHighlightFont, Font* pTextFont)
{
   CComObject<FlexDMDScoreBoard>* obj = nullptr;
   CComObject<FlexDMDScoreBoard>::CreateInstance(&obj);
   obj->AddRef();

   obj->SetFlexDMD(pFlexDMD);
   obj->m_pScoreFont = pScoreFont;
   obj->m_pHighlightFont = pHighlightFont;
   obj->m_pTextFont = pTextFont;

   obj->m_pLowerLeft = FlexDMDLabelActor::Create(pFlexDMD, pTextFont, "", "");
   obj->m_pLowerRight = FlexDMDLabelActor::Create(pFlexDMD, pTextFont, "", "");

   obj->AddActor((FlexDMDActor*)obj->m_pLowerLeft);
   obj->AddActor((FlexDMDActor*)obj->m_pLowerRight);

   for (int i = 0; i < 4; i++) {
      obj->m_pScores[i] = FlexDMDLabelActor::Create(pFlexDMD, pScoreFont, "0", "");
      obj->AddActor((FlexDMDActor*)obj->m_pScores[i]);
   }

   return obj;
}

void FlexDMDScoreBoard::SetBackground(FlexDMDActor* pBackground)
{
   if (m_pBackground)
      RemoveActor(m_pBackground);
   m_pBackground = pBackground;
   if (m_pBackground)
      AddActorAt(m_pBackground, 0);
}

void FlexDMDScoreBoard::SetNPlayers(int nPlayers)
{
   for (int i = 0; i < 4; i++)
      m_pScores[i]->SetVisible(i < nPlayers);
}

void FlexDMDScoreBoard::SetFonts(Font* scoreFont, Font* highlightFont, Font* textFont)
{
   m_pScoreFont = scoreFont;
   m_pHighlightFont = highlightFont;
   m_pTextFont = textFont;
   SetHighlightedPlayer(m_highlightedPlayer);
   m_pLowerLeft->SetFont(textFont);
   m_pLowerRight->SetFont(textFont);
}

void FlexDMDScoreBoard::SetHighlightedPlayer(int player)
{
   m_highlightedPlayer = player;
   for (int i = 0; i < 4; i++) {
      if (i == player - 1)
         m_pScores[i]->SetFont(m_pHighlightFont);
      else
         m_pScores[i]->SetFont(m_pScoreFont);
    }
}

void FlexDMDScoreBoard::SetScore(LONG score1, LONG score2, LONG score3, LONG score4)
{
   char score[20];
    
   snprintf(score, sizeof(score), "%'d", score1);
   m_pScores[0]->SetText(score);

   snprintf(score, sizeof(score), "%'d", score2);
   m_pScores[1]->SetText(score);

   snprintf(score, sizeof(score), "%'d", score3);
   m_pScores[2]->SetText(score);

   snprintf(score, sizeof(score), "%'d", score4);
   m_pScores[3]->SetText(score);
}

void FlexDMDScoreBoard::Update(float delta)
{
    FlexDMDGroupActor::Update(delta);
    SetBounds(0, 0, GetParent()->GetWidth(), GetParent()->GetHeight());
    float yText = GetHeight() - m_pTextFont->GetBitmapFont()->GetBaseHeight() - 1;
    // float yLine2 = 1 + m_pHighlightFont->m_pBitmapFont->m_baseHeight + (GetHeight() - 2 -  m_pTextFont->m_pBitmapFont->m_baseHeight - 2 * m_pHighlightFont->m_pBitmapFont->m_baseHeight ) / 2;
    float yLine2 = (GetHeight() - m_pTextFont->GetBitmapFont()->GetBaseHeight()) / 2.0f;
    float dec = (m_pHighlightFont->GetBitmapFont()->GetBaseHeight() - m_pScoreFont->GetBitmapFont()->GetBaseHeight()) / 2.0f;
    // float yLine2 = (1 + yText) * 0.5f;
    m_pScores[0]->Pack();
    m_pScores[1]->Pack();
    m_pScores[2]->Pack();
    m_pScores[3]->Pack();
    m_pLowerLeft->Pack();
    m_pLowerRight->Pack();
    m_pScores[0]->SetAlignedPosition(1, 1 + (m_highlightedPlayer == 1 ? 0 : dec), Alignment_TopLeft);
    m_pScores[1]->SetAlignedPosition(GetWidth() - 1, 1 + (m_highlightedPlayer == 2 ? 0 : dec), Alignment_TopRight);
    m_pScores[2]->SetAlignedPosition(1, yLine2 + (m_highlightedPlayer == 3 ? 0 : dec), Alignment_TopLeft);
    m_pScores[3]->SetAlignedPosition(GetWidth() - 1, yLine2 + (m_highlightedPlayer == 4 ? 0 : dec), Alignment_TopRight);
    m_pLowerLeft->SetAlignedPosition(1, yText, Alignment_TopLeft);
    m_pLowerRight->SetAlignedPosition(GetWidth() - 1, yText, Alignment_TopRight);
}

void FlexDMDScoreBoard::Draw(Graphics* graphics)
{
   if (GetVisible()) {
      graphics->Clear(RGB(0, 0, 0));
      if (m_pBackground)
         m_pBackground->SetSize(GetWidth(), GetHeight());
      FlexDMDGroupActor::Draw(graphics);
   }
}