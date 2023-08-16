#pragma once

#include "FlexDMD.h"
#include "bmfonts/BitmapFont.h"
#include "Graphics.h"

class AssetManager;

class Font {
public:
    Font();
    ~Font();

   static Font* Create(AssetManager* pAssetManager, AssetSrc* pAssetSrc);

   void DrawCharacter(Graphics* graphics, char character, char previousCharacter, float& x, float& y);
   SDL_Rect MeasureFont(const string& text);
   void DrawText_(Graphics* graphics, float x, float y, const string& text);

   BitmapFont* GetBitmapFont() { return m_pBitmapFont; }

private:
   BitmapFont* m_pBitmapFont;
   SDL_Surface** m_textures;
};