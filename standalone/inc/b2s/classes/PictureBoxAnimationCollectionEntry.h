#pragma once

class PictureBoxAnimationCollectionEntry 
{
public:
   PictureBoxAnimationCollectionEntry(const string& szAnimationName, int loops, bool playReverse = false);

   const string& GetAnimationName() const { return m_szAnimationName; }
   int GetLoops() const { return m_loops; }
   bool IsPlayReverse() const { return m_playReverse; }

private:
   string m_szAnimationName;
   int m_loops;
   bool m_playReverse;
};