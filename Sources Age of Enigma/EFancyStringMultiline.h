#ifndef E_FANCY_STRING_MULTILINE_H
#define E_FANCY_STRING_MULTILINE_H

#include "EFancyString.h"

class EFancyStringMultiline : public EFancyString {

public : 
   
   // X, point gauche, X2 point droite
   EFancyStringMultiline(const char* sz, KTextFace* font, float X = 0.0f, float X2 = 0.0f, float Y = 0.0f, float fblend = 1.0f, float r = 1.0f, float v = 1.0f, float b = 1.0f);
   virtual ~EFancyStringMultiline();
   
   float m_width;

   virtual float getHeight() const;
   virtual float getWidth() const;

protected : 
   
   // rédefinit le render, ici effectue un dessin multiligne
   virtual void renderString();
};
#endif
