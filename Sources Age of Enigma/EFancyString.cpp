#include "EFancyString.h"
#include "KPTK.h"
#include <string.h>
#include "global.h"

// si on place l'effet fancy dans une texture
#define FONT_CACHED false

EFancyString::EFancyString(const char* sz, KTextFace* font, float X, float Y, float fblend, float r, float v, float b) 
: m_sz(NULL), x(X), y(Y), m_pFont(font), m_fBlend(fblend), m_r(r), m_v(v), m_b(b), graphicsFancy(NULL)
{
   m_sz = new char[strlen(sz)+1];
   strcpy(m_sz, sz);
   m_useFancy = false;
   idFont = 0;
}

EFancyString::~EFancyString() {
   delete[] m_sz;
   m_sz = NULL;

   XDELETE(graphicsFancy);
}

void EFancyString::render() {
   
   check();

   bool createGraphics = false;

   if(FONT_CACHED) {
      // si le fond n'a pas été mis dans une texture.. on le fait maintenant
      if((graphicsFancy == NULL || graphicsFancy->isRenderTargetLost())) {
         
         graphicsFancy = KPTK::createKGraphic();
         long widthText, heightText = 0;
         getDimensionTexture(&widthText, &heightText);
         // +4 pour les dimensions du fancy
         if(graphicsFancy->makeRenderTarget(widthText+4, heightText+4, false, true)) {
            graphicsFancy->beginRenderToTarget();
            createGraphics = true;
         }
      }
   }
   
   // si on a réussi à créer la texture ou si on est pas en mode cache
   // on dessine l'effet fancy + la string (dans la texture ou sur l'écran)
   if(createGraphics || !FONT_CACHED) {
      
      if(m_useFancy) {
         float saveX = x;
         float saveY = y;
         m_pFont->setBlend(m_fBlend);
         m_pFont->setBlitColor(0, 0, 0, m_fBlend*0.125f);


         for (int posY = -2; posY <= 2; posY++) {
            for (int posX = -2; posX <= 2; posX++) {
               x += posX;
               y += posY;
               renderString();
               x = saveX;
               y = saveY;
            }
         }
      }

      m_pFont->setBlitColor (m_r, m_v, m_b, m_fBlend);
      renderString();

      m_pFont->setBlitColor (1, 1, 1, 1);
      m_pFont->setBlend(1);
   }

   // si on était en mode cache et qu'on avait réussi à créer la texture...
   if(FONT_CACHED && createGraphics)
      graphicsFancy->endRenderToTarget();

   // affiche la texture sur l'écran
   if(FONT_CACHED && graphicsFancy)
      graphicsFancy->blitAlphaRect(0, 0, graphicsFancy->getWidth(), graphicsFancy->getHeight(), x, y);
}

void EFancyString::check() {
}

void EFancyString::getDimensionTexture(long *width, long *height) {
   *width = getWidth();
   *height = getHeight(); 
}

float EFancyString::getHeight() const {
   return m_pFont->getHeightPix();
}

float EFancyString::getWidth() const {
   return m_pFont->getStringWidth(m_sz);;
}

void EFancyString::setColor(float* color) {

   if(color) {
      m_r = color[0];
      m_v = color[1];
      m_b = color[2];
   }
}

void EFancyString::renderString() {
   m_pFont->drawStringFromLeftF(m_sz, x, y);
}

void EFancyString::setBlend(float blend) {
   m_fBlend = blend;
}

const KTextFace* EFancyString::getFont() const {
   return m_pFont;
}