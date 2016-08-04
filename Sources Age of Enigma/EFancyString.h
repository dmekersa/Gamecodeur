#ifndef E_FANCY_STRING_H
#define E_FANCY_STRING_H

// classe EFancyString de base

class KTextFace;
class KGraphic;

class EFancyString {
   
public : 

   EFancyString(const char* sz, KTextFace* font, float X, float Y, float fblend = 1.0f, float r = 1.0f, float v = 1.0f, float b = 1.0f);
   virtual ~EFancyString();

   // méthode à appeler pour le rendu
   // fait l'effet de "fancy" puis appele renderString pour afficher la chaine normalement par dessus
   void render();

   // float[3] -> 0/r, 1/g, 2/b
   void setColor(float* color);
   void setBlend(float blend);

   const KTextFace* getFont() const;

   virtual float getHeight() const;
   virtual float getWidth() const;

   float x;
   float y;

   int idFont;

protected : 

   virtual void check();
   
   // à redéfinir dans les sous classes pour gerer le multiLigne et autre cas spécial
   // la classe de base déssine la chaine de gauche à droite sur une ligne
   virtual void renderString();
   
   // definit la dimension de la texture
   void getDimensionTexture(long *width, long *height);

   char* m_sz;
   KTextFace* m_pFont;
   float m_r;
   float m_v;
   float m_b;
   float m_fBlend;
   KGraphic *graphicsFancy;
   bool m_useFancy;
   
};
#endif
