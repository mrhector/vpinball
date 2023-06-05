#include "stdafx.h"
#include "PinUpPlayerPlaylist.h"
#include "PinUpPlayerUtils.h"

PinUpPlayerPlaylist::PinUpPlayerPlaylist()
{
}

PinUpPlayerPlaylist::~PinUpPlayerPlaylist()
{
}

PinUpPlayerPlaylist* PinUpPlayerPlaylist::CreateFromCSVLine(string line)
{
   vector<string> parts = PinUpPlayerUtils::ParseCSVLine(line);
   if (parts.size() != 7)
      return NULL;

   PinUpPlayerPlaylist* playlist = new PinUpPlayerPlaylist();

   playlist->m_screenNum = PinUpPlayerUtils::StringToInt(parts[0], 0);
   playlist->m_folder = parts[1];
   playlist->m_des = parts[2];
   playlist->m_alphaSort = PinUpPlayerUtils::StringToInt(parts[3], 0);
   playlist->m_restSeconds = PinUpPlayerUtils::StringToInt(parts[4], 0);
   playlist->m_volume = PinUpPlayerUtils::StringToInt(parts[5], 0);
   playlist->m_priority = PinUpPlayerUtils::StringToInt(parts[6], 0);

   return playlist;
}