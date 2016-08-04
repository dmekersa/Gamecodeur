#ifndef E_FANCY_STRING_ANIM_H
#define E_FANCY_STRING_ANIM_H

#include "EFancyStringMultiline.h"

// texte anim par clip (deroulement..)

class EFancyStringAnim : public EFancyStringMultiline {

public : 

   enum ANIMATION {
      UP_TO_DOWN
   };

   EFancyStringAnim(const char* sz, KTextFace* font, float X, float X2, float Y, ANIMATION typeAnim = UP_TO_DOWN, float fblend = 1.0f, float r = 1.0f, float v = 1.0f, float b = 1.0f);
   virtual ~EFancyStringAnim();
   
   virtual float getHeight() const;

   bool animEnded() const;
   void resetAnim();

protected : 

   virtual void check();

   virtual void renderString();
   double compt;
   int heightMax;
   int widthMax;
};
#endif