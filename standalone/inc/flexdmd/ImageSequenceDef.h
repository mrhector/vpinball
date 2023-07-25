#pragma once

#include "flexdmd_i.h"
#include "BaseDef.h"

class ImageSequenceDef : public BaseDef
{
public:
   ImageSequenceDef(const string& images, int fps, bool loop);
   ~ImageSequenceDef();

   bool operator==(const ImageSequenceDef& other) const;
   size_t hash() const;

   string m_images;
   int m_fps;
   int m_loop;
   Scaling m_scaling;
   Alignment m_alignment;
};
