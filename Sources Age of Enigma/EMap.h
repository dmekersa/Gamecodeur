#ifndef E_MAP_H
#define E_MAP_H

#include <map>
#include <string>
#include "KPTK.h"

class EFancyString;
class ESmartImage;

struct EZone {
   
   int x;
   int y;
   int width;
   int height;
   bool isVisited;
   KGraphic* img;
   std::string trad;

   EZone(): img(NULL) {;}
};

struct ELayer {
   
   int x;
   int y;
   std::string idUnivers;
   bool stateVisited;
   KGraphic *layerNoVisited;
   KGraphic *layerVisited;

   ELayer(): layerNoVisited(NULL), layerVisited(NULL) {;}
};

// classe definissant le plan
class EMap {

public : 

   EMap();
   ~EMap();

   void Clean();

   void RestoreFromProfile();
   void StorageInProfile();

   void Check();

   // efface les layers de la carte
   void clearAllLayerImg();
   // on estime que le point 0,0 de l'img est le point 0,0 de la carte
   // img de l'univers non visité et visité. Boolean pour dire si cet univers est déjà visité (utile si on est déjà dans cet univers -> maison)
   void addLayerImg(const char* idUnivers, KGraphic* imgNoVisited, KGraphic* imgVisited, int x, int y, bool isVisited = false);
   
   // rend cet univers visité, change l'image de son layer
   void setUniversVisited(const char* idUnivers, bool b = true);

   void Render();

   bool isClosing() const; // vrai si la map est en train de se fermer

   void setBackgroundImg(KGraphic *background);
   void setVisited(const char* idZone, bool b = true); // afficher symbole dans la pièce courante


   void setMarkerZone(const char* idZone, KGraphic* img);
   
   void Show();
   void Hide();
   bool isVisible() const;

   // zoneExist -> zone à renommer en newZone
   void renameZone(const char* zoneExist, const char* newZone);

   void addString(EFancyString* string);
   void addZone(const char* idZone, int x, int y, int width, int height);

   // image qui sera affiché dans la zone courrante
   void setImgPerso(KGraphic* imgPerso);

private : 

   // vrai si la map est en train de se charger du profil;
   bool m_isRestoring;
   bool m_isClosing;
   // defilement carte
   float yOffset;
   float xOffset;

   void renderString();

   KTextFace* _lpFontToolTip;
   KGraphic* _lpGraphInterrogation;
   
   // id, rect
   std::map<const std::string, EZone> m_listZone;
   std::vector<EFancyString*> m_listString;
   std::vector<ELayer> m_listLayers;
   std::string m_currentZone;
   KGraphic *_lpBgGraphic;
   ESmartImage *_lpBgImg;
   bool _bVisible;

   KGraphic *_lpImgPerso;
   KGraphic *_lpImgClose; // image où le clic ferme la carte
};
#endif