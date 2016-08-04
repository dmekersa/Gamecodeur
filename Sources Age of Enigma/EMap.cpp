#include "EMap.h"
#include "global.h"
#include "EFancyString.h"
#include "ESmartImage.h"
#include "ImageBank.h"
#include "ESceneDirector.h"
#include "FontBank.h"
#include "EPointer.h"

const int POS_Y_MAP_INIT = -768;
const int POS_X_MAP_INIT = 0;

const int POS_X_MAP_FINAL = 0;
const int POS_Y_MAP_FINAL = 0;

const int POS_X_IMG_CLOSEMAP = 839;
const int POS_Y_IMG_CLOSEMAP = 46;

EMap::EMap()
: _lpBgImg(NULL), _lpImgPerso(NULL), _bVisible(false), yOffset(0), xOffset(0), m_isClosing(false), m_isRestoring(false) {

   m_currentZone = "none";
   _lpFontToolTip = EFontBank::getFont("tahoma.ttf", 12); 

   // image affiché dans une zone non visitée
   _lpGraphInterrogation = EImageBank::getImage("interrogation","png");

   //image pour fermer la map
   _lpImgClose = EImageBank::getImage("map/house_map_closebutton", "png");
}

EMap::~EMap() {
   
   XDELETE(_lpBgImg);
//   XDELETE(_lpBgGraphic);

 /*  for (std::map<const std::string, EZone>::iterator it=m_listZone.begin() ; it != m_listZone.end(); it++ )
      for(unsigned int i = 0; i < (*it).second.vectImg.size(); ++i)
         delete (*it).second.vectImg[i];
*/
   XDELETE(_lpFontToolTip);
}

void EMap::Clean() {
   for (std::map<const std::string, EZone>::iterator it = m_listZone.begin(); it != m_listZone.end(); it++ ) {
      it->second.isVisited = false;
   }
   for(unsigned int i = 0; i < m_listLayers.size(); ++i) {
      m_listLayers[i].stateVisited = false;
   }
   m_currentZone = "none";
}

void EMap::RestoreFromProfile() {
   
   // si deja en train de se restorer
   if(m_isRestoring)
      return;

   Clean();

   m_isRestoring = true;

   std::string bufGlobal(GetStrGlobal("__map__",""));

   int indexFinCurrentString = 0;
   int indexCurrentDecoup = 0;

    while(indexFinCurrentString != std::string::npos) {

       // chaque element est séparé par un ";"
      indexFinCurrentString = bufGlobal.find_first_of(";", indexFinCurrentString);

      if(indexFinCurrentString != std::string::npos) {
         
         std::string chaineTemp(bufGlobal.substr(indexCurrentDecoup, indexFinCurrentString - indexCurrentDecoup));
         indexCurrentDecoup = ++indexFinCurrentString;

         // une zone
         if((&chaineTemp[0])[0] == 'z') {
            // 2 -> index de la premiere lettre de l'id de la zone
            int indexVirg = chaineTemp.find_first_of(",", 2);
            int tailleSz = indexVirg - 2;
            std::string idZone(chaineTemp.substr(2, tailleSz));
            setVisited(idZone.c_str(), (&chaineTemp[3+tailleSz])[0] == '1');
         }
         // univers
         else if((&chaineTemp[0])[0] == 'u') {
            // 2 -> index de la premiere lettre de l'id de l'univers
            int indexVirg = chaineTemp.find_first_of(",", 2);
            int tailleSz = indexVirg - 2;
            std::string idUniverse(chaineTemp.substr(2, tailleSz));
            this->setUniversVisited(idUniverse.c_str(), (&chaineTemp[3+tailleSz])[0] == '1');
         }
      }
    }

    m_isRestoring = false;
}

void EMap::StorageInProfile() {
   
   // Chaine pour dumper la map dans le profil
   char szDump[1024*4] = {0};
   
   // enregistre l'états des zones
   std::map<const std::string, EZone>::iterator it = m_listZone.begin();
   while(it != m_listZone.end()) {
      // enrgistre l'id et si la zone a été visité
      strcat(szDump, "z:");
      strcat(szDump, it->first.c_str());
      strcat(szDump, ",");
      if(it->second.isVisited)
         strcat(szDump, "1");
      else
         strcat(szDump, "0");
      strcat(szDump, ";");
      ++it;
   }
   // enregistre l'état des univers
   for(unsigned int i = 0; i < m_listLayers.size(); ++i) {
      strcat(szDump, "u:");
      strcat(szDump, m_listLayers[i].idUnivers.c_str());
      strcat(szDump, ",");
      if(m_listLayers[i].stateVisited)
         strcat(szDump, "1");
      else
         strcat(szDump, "0");
      strcat(szDump, ";");
   }

   // Sauvegarde du dump dans le profil
   SetGlobal("__map__", szDump);
}

void EMap::Check() {
   
   if(this->isVisible() && Mouse.LeftEvent()) {

      // check sur l'image de fermeture
      if(_lpImgClose && (Mouse.x >= POS_X_IMG_CLOSEMAP + xOffset) && (Mouse.x <= POS_X_IMG_CLOSEMAP + xOffset + _lpImgClose->getWidth())
         && (Mouse.y >= POS_Y_IMG_CLOSEMAP + yOffset) && (Mouse.y <= POS_Y_IMG_CLOSEMAP + yOffset + _lpImgClose->getHeight())) {
         // fermeture
         ESoundBank::getSound("mapclose")->playSample();
            Hide();
            Mouse.LeftEventClose();
      }
      // check si en dehors du background
      if(_lpBgImg && _lpBgImg->getCurrentY() == POS_Y_MAP_FINAL && ((Mouse.x < POS_X_MAP_INIT + xOffset) || (Mouse.x > POS_X_MAP_INIT + xOffset + _lpBgImg->getWidth())
         || (Mouse.y < POS_Y_MAP_FINAL + yOffset) || (Mouse.y > POS_Y_MAP_FINAL + yOffset + _lpBgImg->getHeight()))) {
         // fermeture
         ESoundBank::getSound("mapclose")->playSample();
            Hide();
            Mouse.LeftEventClose();
      }
   }
}

bool EMap::isClosing() const {
   return m_isClosing;
}

void EMap::setBackgroundImg(KGraphic *background) {
   
   _lpBgImg = new ESmartImage(background, POS_X_MAP_INIT, POS_Y_MAP_INIT, 1, true, true, 0);
 //  if(_lpBgGraphic)
  //    XDELETE(_lpBgGraphic);
 //  _lpBgGraphic = background;
}

void EMap::Show() {
   _bVisible = true;
   m_isClosing = false;
}

void EMap::Hide() {
   _bVisible = false;
   m_isClosing = true;
//   if(_lpBgImg->getCurrentX() == POS_X_MAP_FINAL && _lpBgImg->getCurrentY() != POS_Y_MAP_INIT != POS_Y_MAP_INIT)
 //     _lpBgImg->AnimateP2P(POS_X_MAP_INIT, POS_Y_MAP_INIT, 1000, false, K_COUNTER_EASEOUT, 0);
}

bool EMap::isVisible() const {
   return _bVisible /*|| _lpBgImg->getCurrentY() > POS_Y_MAP_INIT*/;
}

void EMap::addZone(const char* idZone, int zoneX, int zoneY, int width, int height) {
   
   // essaye avec le "mo_"
   const char* trad = KStr((std::string("mo_") + std::string(idZone)).c_str());
   const char* trad2 = KStr((idZone));

   std::string chaine(idZone);
   std::map<const std::string, EZone>::iterator it = m_listZone.find(chaine);
   if(it != m_listZone.end()) {
      return;
   }
   EZone zone;
   zone.x = zoneX;
   zone.y = zoneY;
   zone.width = width;
   zone.height = height;
   zone.img = NULL;
   if(trad)
      zone.trad = std::string(trad);
   else if(trad2)
       zone.trad = std::string(trad2); 
   zone.isVisited = false;

   m_listZone[chaine] = zone;
}

void EMap::Render() {
   
   // background
    if (_bVisible) {
      
      if(_lpBgImg->getCurrentY() == POS_Y_MAP_INIT)
         _lpBgImg->AnimateP2P(POS_X_MAP_FINAL, POS_Y_MAP_FINAL, 1000, false, K_COUNTER_EASEOUT, 0);

      // On force le curseur souris normal
      SCENEDIRECTOR->PrepareMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
      SCENEDIRECTOR->GetCurrentScene()->SetMouseOverInfo("", "");
   } else {
      // La liste doit s'enrouler
      if (_lpBgImg->getCurrentY() != POS_Y_MAP_INIT && !_lpBgImg->isAnimateP2P()) {
         _lpBgImg->AnimateP2P(POS_X_MAP_INIT, POS_Y_MAP_INIT, 1000, false, K_COUNTER_EASEOUT, 0);
         m_isClosing = true;
         if (SCENEDIRECTOR->GetCurrentScene()) {
            SCENEDIRECTOR->GetCurrentScene()->ResetMouseOver();
         }
      }
      else if(_lpBgImg->getCurrentY() == POS_Y_MAP_INIT) {
         m_isClosing = false;
         return;
      }
    }

      // affiche la carte
      _lpBgImg->Draw();

      yOffset = _lpBgImg->getCurrentY();
      xOffset = _lpBgImg->getCurrentX();

      // On force le curseur souris normal
      SCENEDIRECTOR->PrepareMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
      SCENEDIRECTOR->GetCurrentScene()->SetMouseOverInfo("", "");

      // affiche les images des univers
      for(unsigned int i = 0; i < m_listLayers.size(); ++i) {
         if((m_listLayers.at(i).stateVisited && m_listLayers.at(i).layerVisited) || m_listLayers.at(i).layerNoVisited == NULL) 
            m_listLayers.at(i).layerVisited->blitAlphaRectF(0.0f, 0.0f, m_listLayers.at(i).layerVisited->getWidth(), m_listLayers.at(i).layerVisited->getHeight(), m_listLayers.at(i).x + xOffset, m_listLayers.at(i).y + yOffset);
         else if(m_listLayers.at(i).layerNoVisited)
            m_listLayers.at(i).layerNoVisited->blitAlphaRectF(0.0f, 0.0f, m_listLayers.at(i).layerNoVisited->getWidth(), m_listLayers.at(i).layerNoVisited->getHeight(), m_listLayers.at(i).x + xOffset, m_listLayers.at(i).y + yOffset);
      }

      //affiche les chaines de caractere
      renderString();

      short xMouse = KInput::getMouseX();
      short yMouse = KInput::getMouseY();

      xMouse -= xOffset;
      yMouse -= yOffset;

      std::string textToDraw("");
   
      for (std::map<const std::string, EZone>::iterator it = m_listZone.begin(); it != m_listZone.end(); it++ ) {
         
         int xRect = (*it).second.x;
         int yRect = (*it).second.y;
         
         #ifdef DEBUG
            _lpGraphInterrogation->drawRect(xRect + xOffset, yRect + yOffset, xRect + (*it).second.width + xOffset, yRect + (*it).second.height + yOffset, 0.0f, 0.0f, 0.0f, 0.3f);
         #endif

         if((*it).second.isVisited == false) {
            _lpGraphInterrogation->blitAlphaRectF(0, 0, _lpGraphInterrogation->getWidth(), _lpGraphInterrogation->getHeight(), 
               (xRect + ((*it).second.width /2.0f) + xOffset - (_lpGraphInterrogation->getWidth()/2.0f)),
               (yRect + ((*it).second.height / 2.0f) + yOffset - (_lpGraphInterrogation->getHeight()/2.0f)));
         }

         // affichage marker zone en bas à droite
         if((*it).second.img != NULL) {
            (*it).second.img->blitAlphaRectF(0, 0, (*it).second.img->getWidth(), (*it).second.img->getHeight(), 
               (xRect + ((*it).second.width) + xOffset - ((*it).second.img->getWidth())),
               (yRect + ((*it).second.height) + yOffset - ((*it).second.img->getHeight())));
         }

         // affichage image perso si c'est la zone courrante
         if(_lpImgPerso && m_currentZone == (*it).first && m_currentZone != "none") {
            _lpImgPerso->blitAlphaRectF(0.0f, 0.0f, _lpImgPerso->getWidth(), _lpImgPerso->getHeight(), 
               (xRect + ((*it).second.width /2.0f) + xOffset - (_lpImgPerso->getWidth()/2.0f)),
               (yRect + ((*it).second.height / 2.0f) + yOffset - (_lpImgPerso->getHeight()/2.0f)));
         }

         if(textToDraw == "" && xMouse >= xRect && xMouse <= xRect + (*it).second.width && yMouse >= yRect && yMouse <= yRect + (*it).second.height && (*it).second.isVisited) {
            textToDraw = (*it).first;

            if((*it).second.trad != "")
               textToDraw = (*it).second.trad;
        //    const char* chaineTrad = KStr(textToDraw.c_str());
        //    if(chaineTrad)
          //     textToDraw = std::string(chaineTrad);
         }
      }
      
      // affichage tool tip
      if(textToDraw != "") {
         
         const char* textDraw = textToDraw.c_str();
      //   float widthText = _lpFontToolTip->getStringWidth(textDraw);
         float posX = xMouse + xOffset;
         float posY = yMouse + yOffset + 10;
      //   if(posX < widthText)
            posX += 18;
      //   else
      //      posX -= _lpFontToolTip->getStringWidth(textDraw);

      //    if(posY < 20)
            posY += 33;
      //   else
      //      posY -= 20;

            // dessine le rectangle
       //     _lpGraphInterrogation->drawRect(posX -1, posY -1, posX + widthText +1, posY + _lpFontToolTip->getHeightPix() +1, 1.0f, 1.0f, 1.0f, 0.8f);
        //    _lpGraphInterrogation->drawRect(posX, posY, posX + widthText, posY + _lpFontToolTip->getHeightPix(), 0.0f, 0.0f, 0.0f, 0.6f);
            SCENEDIRECTOR->GetCurrentScene()->DrawFancyString(textDraw, _lpFontToolTip, posX /*- _lpFontToolTip->getStringWidth(textDraw)/2*/, posY, 0, true, 1);

         //   _lpFontToolTip->drawStringFromLeft(textDraw, posX - _lpFontToolTip->getStringWidth(textDraw)/2, posY);
      //      EFancyString fancy(textDraw, _lpFontToolTip, posX, posY);
       //     fancy.render();
      }

      if(_lpImgClose)
         _lpImgClose->blitAlphaRectF(0.0f, 0.0f, _lpImgClose->getWidth(), _lpImgClose->getHeight(), xOffset + POS_X_IMG_CLOSEMAP, yOffset + POS_Y_IMG_CLOSEMAP);
  // }
 }

void EMap::renderString() {
   
   for(unsigned int i = 0; i < m_listString.size(); ++i) {
      float xTemp = m_listString[i]->x;
      float yTemp = m_listString[i]->y;

      m_listString[i]->x = xTemp + xOffset;
      m_listString[i]->y = yTemp + yOffset;
      m_listString.at(i)->render();
      m_listString[i]->x = xTemp;
      m_listString[i]->y = yTemp;
   }
}

void EMap::setVisited(const char* idZone, bool b) {
   std::string chaineToFind(idZone);

   if(chaineToFind == "house_livingroom") {
      setVisited("hall");
      setVisited("house_hall");
   }

   std::map<const std::string, EZone>::iterator it = m_listZone.find(chaineToFind);
   if(it != m_listZone.end()) {
      it->second.isVisited = b;
      if(b)
         m_currentZone = std::string(idZone);

      if(!m_isRestoring)
         StorageInProfile();
   }
}

void EMap::renameZone(const char* zoneExist, const char* newZone) {
   
   std::map<const std::string, EZone>::iterator it = m_listZone.find(std::string(zoneExist));
   if(it != m_listZone.end()) {
      EZone zone = it->second;
      m_listZone.erase(it);
      this->addZone(newZone, zone.x, zone.y, zone.width, zone.height);
   }
}

void EMap::addString(EFancyString* string) {
   m_listString.push_back(string);
}

void EMap::setMarkerZone(const char* idZone, KGraphic* img) {
   
   std::map<const std::string, EZone>::iterator it = m_listZone.find(idZone);
   if(it != m_listZone.end()) {
      it->second.img = img;
   }
}

void EMap::clearAllLayerImg() {
   m_listLayers.clear();
}

void EMap::addLayerImg(const char* idUnivers, KGraphic* imgNoVisited, KGraphic* imgVisited, int x, int y, bool isVisited) {

   ELayer layer;
   layer.idUnivers = std::string(idUnivers);
   layer.x = x;
   layer.y = y;
   layer.layerNoVisited = imgNoVisited;
   layer.layerVisited = imgVisited;
   layer.stateVisited = isVisited;
   m_listLayers.push_back(layer);
}

void EMap::setUniversVisited(const char* idUnivers, bool b) {
   
   std::string chaineToCompare(idUnivers);

   for(unsigned int i = 0; i < m_listLayers.size(); ++i) {
      if(m_listLayers[i].idUnivers == chaineToCompare)
         m_listLayers[i].stateVisited = b;
   }

   if(!m_isRestoring)
      StorageInProfile();
}

void EMap::setImgPerso(KGraphic* imgPerso) {
   _lpImgPerso = imgPerso;
}