#include "stdafx.h"
#include "PinUpPlayerUtils.h"

PinUpPlayerUtils::PinUpPlayerUtils()
{
}

PinUpPlayerUtils::~PinUpPlayerUtils()
{
}

vector<string> PinUpPlayerUtils::ParseCSVLine(string line)
{
   vector<string> parts;
   string field;
   enum State { Normal, Quoted };
   State currentState = Normal;

   for (char c : Trim(line)) {
      switch (currentState) {
         case Normal:
            if (c == '"') {
               currentState = Quoted;
            } else if (c == ',') {
               parts.push_back(field);
               field.clear();
            } else {
               field += c;
            }
            break;
         case Quoted:
            if (c == '"') {
               currentState = Normal;
            } else {
               field += c;
            }
            break;
      }
   }

   parts.push_back(field);

   return parts;
}

string PinUpPlayerUtils::Trim(string s)
{
   try {
      s = s.substr(s.find_first_not_of(" \t\r\n"), s.find_last_not_of(" \t\r\n") - s.find_first_not_of(" \t\r\n") + 1);
   }
   catch (...) { 
      s = "";   
   }
   return s;
}

int PinUpPlayerUtils::StringToInt(string s, int defaultValue)
{
   int value;
   try {
      value = std::stoi(Trim(s));
   }
   catch (...) {
      value = defaultValue;
   }
   return value;
}