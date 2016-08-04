#ifndef GRAPHICAL_STRING_TRAD_H
#define GRAPHICAL_STRING_TRAD_H

// classe représentant une chaine graphique accompagnée de sa traduction
// chaine : std::string
// chaine traduite : std::vector<std::string> vector pour pouvoir décomposer ici par ligne -> \n
// position (x,y)
// sa couleur (r,v,b)

#include <string>
#include <vector>

class KTextFace;
class EFancyString;

class GraphicalStringTrad {
   
public : 

   GraphicalStringTrad();
   ~GraphicalStringTrad();

   // effectue son rendu en utilisant son renderer. Si pas de renderer, ne fait rien
   void render();

   // pour empecher le changement de couleur
   void setFreezeColor(bool b);
   void setColor(float r, float g, float b);

   // si on veut que la chaine puisse faire son rendu elle même, il faut lui assigner une EFancyString
   void setRenderer(EFancyString* fancyString);
   EFancyString* getRenderer();

   // renvoit le tableau de couleurs
   const float* getColor() const;
   float r() const;
   float g() const;
   float b() const;
   
   const std::string & getString() const; 
   void setString(const std::string & string);
   void setString(const char* string);

   // va placer la chaine dans le vecteur 
   // ex : Hello \n World -> 
   // vect[0] -> Hello 
   // vect[1] -> World
   void setTrad(const char* string);
   void setTrad(const std::string & string);
   
   const std::string & getTrad() const;
   //const std::vector<std::string> * getVectTrad() const;

   bool isVisible() const;
   void setVisible(bool visible);

   float x;
   float y;

 //  std::vector<std::string> m_vectorTrad;

private : 

   KTextFace* m_pFont;
   EFancyString *m_pRenderer;

   // vrai si on autorise pas le changement de couleur
   bool m_freezeColor;
   float m_pcolor[3]; //r,v,b
   bool m_visible;
   std::string m_sz;
   std::string m_trad;

   

};
#endif