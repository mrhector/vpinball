#include "stdafx.h"
#include "PinUpPlayerScreen.h"
#include "PinUpPlayerUtils.h"

PinUpPlayerScreen::PinUpPlayerScreen()
{
}

PinUpPlayerScreen::~PinUpPlayerScreen()
{
}

PinUpPlayerScreen* PinUpPlayerScreen::CreateFromCSVLine(string line)
{
   vector<string> parts = PinUpPlayerUtils::ParseCSVLine(line);
   if (parts.size() != 8)
      return NULL;

   PinUpPlayerScreen* screen = new PinUpPlayerScreen();

   screen->m_screenNum = PinUpPlayerUtils::StringToInt(parts[0], 0);
   screen->m_screenDes = parts[1];
   screen->m_playList = parts[2];
   screen->m_playFile = parts[3];
   screen->m_loopit = parts[4];
   screen->m_active = PinUpPlayerUtils::StringToInt(parts[5], 0);
   screen->m_priority = PinUpPlayerUtils::StringToInt(parts[6], 0);
   screen->m_customPos = parts[7];

   return screen;
}