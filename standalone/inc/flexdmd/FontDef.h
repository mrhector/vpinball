#pragma once

#include "flexdmd_i.h"
#include "BaseDef.h"

class FontDef : public BaseDef
{
public:
   FontDef(const string& path, OLE_COLOR tint, OLE_COLOR borderTint, int borderSize = 0);
   ~FontDef();

   bool operator==(const FontDef& other) const;
   size_t hash() const;

   string m_szPath;
   OLE_COLOR m_tint;
   OLE_COLOR m_borderTint;
   int m_borderSize;
};
