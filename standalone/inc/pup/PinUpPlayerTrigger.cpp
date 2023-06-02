#include "stdafx.h"
#include "PinUpPlayerTrigger.h"
#include "PinUpPlayerUtils.h"

PinUpPlayerTrigger::PinUpPlayerTrigger()
{
}

PinUpPlayerTrigger::~PinUpPlayerTrigger()
{
}

PinUpPlayerTrigger* PinUpPlayerTrigger::CreateFromCSVLine(string line)
{
   vector<string> parts = PinUpPlayerUtils::ParseCSVLine(line);
   if (parts.size() != 14)
      return NULL;

   PinUpPlayerTrigger* trigger = new PinUpPlayerTrigger();

   trigger->m_id = PinUpPlayerUtils::StringToInt(parts[0], 0);
   trigger->m_active = PinUpPlayerUtils::StringToInt(parts[1], 0);
   trigger->m_descript = parts[2];
   trigger->m_trigger = parts[3];
   trigger->m_screenNum = PinUpPlayerUtils::StringToInt(parts[4], 0);
   trigger->m_playList = parts[5];
   trigger->m_playFile = parts[6];
   trigger->m_volume = PinUpPlayerUtils::StringToInt(parts[7], 0);
   trigger->m_priority = PinUpPlayerUtils::StringToInt(parts[8], 0);
   trigger->m_length = PinUpPlayerUtils::StringToInt(parts[9], 0);
   trigger->m_counter = PinUpPlayerUtils::StringToInt(parts[10], 0);
   trigger->m_restSeconds = PinUpPlayerUtils::StringToInt(parts[11], 0);
   trigger->m_loop = parts[12];
   trigger->m_defaults = PinUpPlayerUtils::StringToInt(parts[13], 0);

   return trigger;
}