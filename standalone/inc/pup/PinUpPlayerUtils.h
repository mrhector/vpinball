#pragma once

class PinUpPlayerUtils {
public:
   PinUpPlayerUtils();
   ~PinUpPlayerUtils();

   static vector<string> ParseCSVLine(string line);
   static int StringToInt(string s, int defaultValue);
   static string Trim(string s);
};