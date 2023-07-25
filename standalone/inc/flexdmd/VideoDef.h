#pragma once

#include "flexdmd_i.h"
#include "BaseDef.h"

class VideoDef : public BaseDef
{
public:
   VideoDef(const string& videoFilename, bool loop);
   ~VideoDef();

   bool operator==(const VideoDef& other) const;
   size_t hash() const;

   string m_videoFilename;
   int m_loop;
   Scaling m_scaling;
   Alignment m_alignment;
};
