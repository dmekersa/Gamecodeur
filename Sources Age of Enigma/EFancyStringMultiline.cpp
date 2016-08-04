#include "EFancyStringMultiline.h"
#include "KPTK.h"

EFancyStringMultiline::EFancyStringMultiline(const char* sz, KTextFace* font, float X, float width, float Y, float fblend, float r, float v, float b)
: EFancyString(sz, font, X, Y, fblend, r, v, b) {
      
      m_width = width;
}

EFancyStringMultiline::~EFancyStringMultiline() {
}

void EFancyStringMultiline::renderString() {
   m_pFont->drawMultiline(m_sz, x, x + m_width, y, TEXTSTYLE_LEFT, 1);
}

float EFancyStringMultiline::getHeight() const {
   return m_pFont->getMultilineHeight(m_sz, x, x + m_width, y, TEXTSTYLE_LEFT, 1);
}

float EFancyStringMultiline::getWidth() const {
   return m_pFont->getMultilineWidth(m_sz, x, x + m_width, y, TEXTSTYLE_LEFT, 1);
}