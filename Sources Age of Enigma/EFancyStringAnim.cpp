#include "EFancyStringAnim.h"
#include "KPTK.h"
#include "MyGame.h"

EFancyStringAnim::EFancyStringAnim(const char* sz, KTextFace* font, float X, float width, float Y, ANIMATION typeAnim, float fblend, float r, float v, float b)
: EFancyStringMultiline(sz, font, X, width, Y, fblend, r, v, b) {
   
   compt = 0.0;
   heightMax = EFancyStringMultiline::getHeight();
   widthMax = EFancyStringMultiline::getWidth();
}

EFancyStringAnim::~EFancyStringAnim() {
}

void EFancyStringAnim::renderString() {
   m_pFont->drawMultilineClipped(m_sz, x, y, x + m_width, y + compt, TEXTSTYLE_LEFT, 0, 0);
}

void EFancyStringAnim::check() {

   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();

   if(animEnded() == false) {
      compt+=fElapsed/15.0;
      delete graphicsFancy;
      graphicsFancy = NULL;
   }
}

float EFancyStringAnim::getHeight() const {
   return compt;
}

bool EFancyStringAnim::animEnded() const {
   return (compt >= (heightMax+2));
}

void EFancyStringAnim::resetAnim() {
   compt = 0;
}