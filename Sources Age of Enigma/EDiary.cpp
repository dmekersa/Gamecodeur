#include "EDiary.h"
#include "KPTK.h"
#include "global.h"
#include "EFancyString.h"
#include "EFancyStringMultiline.h"
#include "ESceneDirector.h"
#include "EPointer.h"
#include "ImageBank.h"
#include "GraphicalStringTrad.h"

#define WIDTH_MAX_STRING_DIARY 330

const int MARGE_VERTICAL = 10;
const int MARGE_BORD_VERTICAL = 30;
const int POS_Y_ARROW = 545;
const int POS_X_ARROW_LEFT = 108;
const int POS_X_ARROW_RIGHT = 901;

FontColor::~FontColor() {XDELETE(font);}
GraphicPath::~GraphicPath() {if(img!=NULL) EImageBank::NotKeepInCache(img);}

Page::~Page() {
   for(unsigned int i = 0; i < lstElement.size(); ++i) {
         XDELETE(lstElement[i].string);
         XDELETE(lstElement[i].img);
      }
   XDELETE(imgBackground);
}

EDiary::EDiary()
: m_pCurrentPage(NULL), yOffset(0.0f), m_hasNewPageNoVisited(false), m_indexPage(0), m_isRestoring(false) {

   imgArrowLeft = EImageBank::getImage("diary/diary_arrow_left", "png");
   imgArrowRight = EImageBank::getImage("diary/diary_arrow_right", "png");
   test = true;
   mouseClick = false;

   addFontToList(EFontBank::getFont("armadillo.ttf", 24), 0.30f, 0.18f, 0.10f, 1.0f);
   addFontToList(EFontBank::getFont("armadillo.ttf", 24), 0.0f, 0.0f, 0.0f, 1.0f);
   addFontToList(EFontBank::getFont("tahoma.ttf", 12), 1.0f, 0.0f, 0.0f, 1.0f);
}

EDiary::~EDiary() {

   // ça veut dire que la page n'a pas été rajouté
   if (m_pCurrentPage) {
      XDELETE(m_pCurrentPage);
   }
   for(unsigned int i = 0; i < m_vectPage.size(); ++i) {
      XDELETE(m_vectPage[i]);
   }

   for(unsigned int i = 0; i < m_listFont.size(); ++i)
      delete m_listFont[i];
}

void EDiary::Clean() {
   
   mouseClick = false;

      // ça veut dire que la page n'a pas été rajouté
   XDELETE(m_pCurrentPage);

   for(unsigned int i = 0; i < m_vectPage.size(); ++i) {
      for(unsigned int j = 0; j < m_vectPage[i]->lstElement.size(); ++j) {
         XDELETE(m_vectPage[i]->lstElement[j].string);
         XDELETE(m_vectPage[i]->lstElement[j].img);
      }
      XDELETE(m_vectPage[i]);
   }
   m_vectPage.clear();
}

void EDiary::addFontToList(KTextFace* font, float r, float g, float b, float blend) {

   FontColor *fontColor = new FontColor();
   fontColor->font = font;
   fontColor->color[0] = r;
   fontColor->color[1] = g;
   fontColor->color[2] = b;
   fontColor->color[3] = blend;
   m_listFont.push_back(fontColor);
}

void EDiary::StorageInProfile() {

   // Chaine pour dumper le diary dans le profil
   char szDump[1024*4] = {0};

   // page séparée par ";"

   // parcourt chaque page
   for(unsigned int page = 0; page < m_vectPage.size(); ++page) {
      // enregistre chaque élément séparé par ","

      // enregistre le background si existe
      if(m_vectPage.at(page)->imgBackground) {
         strcat(szDump, "b:"); // b pour background
         strcat(szDump, m_vectPage.at(page)->imgBackground->path.c_str() );
         strcat(szDump, ",");
      }

      for(unsigned int i = 0; i < m_vectPage.at(page)->lstElement.size(); ++i) {
         
         if(m_vectPage.at(page)->lstElement.at(i).string != NULL) {
            strcat(szDump, "s:"); // s pour string
            int idFont = m_vectPage.at(page)->lstElement.at(i).string->getRenderer()->idFont;
      //      const KTextFace* font = m_vectPage.at(page)->lstElement.at(i).string->getRenderer()->getFont();
      //      for(unsigned int f = 0; f < m_listFont.size(); ++f) {
      //         if(font == m_listFont[f]->font) {
      //            idFont = f;
      //            break;
      //         }
       //     }
            strcat(szDump, m_vectPage.at(page)->lstElement.at(i).string->getString().c_str());
            strcat(szDump, ",");
            strcat(szDump, itos(idFont).c_str());
            strcat(szDump, ",");
            strcat(szDump, itos(m_vectPage.at(page)->lstElement.at(i).string->x).c_str());
            strcat(szDump, ",");
            strcat(szDump, itos(m_vectPage.at(page)->lstElement.at(i).string->y).c_str());
            strcat(szDump, ",");
         }
         else if(m_vectPage.at(page)->lstElement.at(i).img != NULL) {
            strcat(szDump, "i:"); // i pour image
            strcat(szDump, m_vectPage.at(page)->lstElement.at(i).img->path.c_str());
            strcat(szDump, ",");
         }
      }
      strcat(szDump, ";");
   }         

   // Sauvegarde du dump dans le profil
   SetGlobal("__diary__", szDump);
}

void EDiary::RestoreFromProfile() {
   
   if(m_isRestoring)
        return;

   m_isRestoring = true;

   Clean();

   std::string bufGlobal(GetStrGlobal("__diary__",""));
   
   int indexFinCurrentString = 0;
   int indexCurrentDecoup = 0;
   
   while(indexFinCurrentString != std::string::npos) {
      indexFinCurrentString = bufGlobal.find_first_of(";", indexFinCurrentString);

      if(indexFinCurrentString != std::string::npos) {

         std::string chaineTemp(bufGlobal.substr(indexCurrentDecoup, indexFinCurrentString - indexCurrentDecoup));
         indexCurrentDecoup = ++indexFinCurrentString;
         int indexCurrent = 0;
         int sizeString = chaineTemp.length();

         // background
         if((&chaineTemp[indexCurrent])[0] == 'b' && (&chaineTemp[indexCurrent+1])[0] == ':') {
            
            indexCurrent+=2;
            int tailleTexte = chaineTemp.find_first_of(',', indexCurrent) - indexCurrent;
            std::string idBackground(chaineTemp.substr(indexCurrent, tailleTexte));
            beginCreatePage(idBackground.c_str());
            indexCurrent += tailleTexte +1;
         }
         else
            beginCreatePage();
         
         while(indexCurrent < sizeString -1) {
            
            if((&chaineTemp[indexCurrent])[0] == 's' && (&chaineTemp[indexCurrent+1])[0] == ':') {

               // se place sur le premiere caractere de l'id
               indexCurrent+=2;
               int tailleTexte = chaineTemp.find_first_of(',', indexCurrent) - indexCurrent;
               std::string IDText(chaineTemp.substr(indexCurrent, tailleTexte));
               
               // sur place sur le caractere apres l'id du texte
               indexCurrent += tailleTexte +1;

               char idFont = chaineTemp[indexCurrent];
               indexCurrent+=2;

               int tailleXRelativ = chaineTemp.find_first_of(",", indexCurrent) - indexCurrent;
               std::string xRelati(chaineTemp.substr(indexCurrent, tailleXRelativ));

               indexCurrent += tailleXRelativ +1;
               int tailleYRelativ = chaineTemp.find_first_of(",", indexCurrent) - indexCurrent;
               std::string yRelati(chaineTemp.substr(indexCurrent, tailleYRelativ));

               // corrige normalement le bug de l'id de la font, atoi attends une chaine fini par \0, non un char
               int idFontINT = idFont - '0';
               if(idFontINT < 0 || idFontINT >= m_listFont.size()) {
                  KAssert(false); // test pour vérifier que l'idFont ne bug plus
                  idFontINT = 0;
               }

               this->addStringToPage(IDText.c_str(), idFontINT, atoi(xRelati.c_str()), atoi(yRelati.c_str()));
            }
            else if((&chaineTemp[indexCurrent])[0] == 'i' && (&chaineTemp[indexCurrent+1])[0] == ':') {
               
               indexCurrent += 2;
               std::string IDImg(chaineTemp.substr(indexCurrent, chaineTemp.find_first_of(',', indexCurrent)-(indexCurrent)));
               this->addImageToPage(IDImg);
            }

            ++indexCurrent;
         }     

         endCreatePage();
      }
   }
   m_isRestoring = false;
}

void EDiary::beginCreatePage(const char* urlImgBackground) {

   // ça veut dire que la page n'a pas été rajouté
   XDELETE(m_pCurrentPage);

   m_pCurrentPage = new Page();
   m_pCurrentPage->imgBackground = NULL;

   if(urlImgBackground) {
      GraphicPath* graphicPath = new GraphicPath();

      std::string pathImg(urlImgBackground);
      graphicPath->path = pathImg;
      
      size_t posExt = pathImg.find_last_of(".");
      KAssert(posExt != std::string::npos);

      KGraphic *img = EImageBank::getImage(pathImg.substr(0, posExt).c_str(),pathImg.substr(posExt+1).c_str());
      EImageBank::KeepInCache(img);
      graphicPath->img = img;

      m_pCurrentPage->imgBackground = graphicPath;
   }
}

bool EDiary::checkIfPageExist(Page* page) {
   
   std::vector<ElementPage> eltPageCompare = page->lstElement;

   for(int i = 0; i < m_vectPage.size(); ++i) {
      std::vector<ElementPage> eltPage1 = m_vectPage.at(i)->lstElement;

      // si meme nombre d'éléments, on compare
      if(eltPageCompare.size() == eltPage1.size()) {
         
         for(int j = 0; j < eltPage1.size(); ++j) {
            
            if(eltPage1.at(j).img && eltPageCompare.at(j).img) {
               if(eltPage1.at(j).img->path != eltPageCompare.at(j).img->path)
                  break;
               else if(j == eltPage1.size() -1)
                  return true;
            }
            else if(eltPage1.at(j).string && eltPageCompare.at(j).string) {
               if(eltPage1.at(j).string->getString() != eltPageCompare.at(j).string->getString())
                  break;
               else if(j == eltPage1.size() -1)
                  return true;
            }
         }
      }
   }
   return false;
}

void EDiary::endCreatePage() {

   KAssert(m_pCurrentPage);

   // verifie qu'une page semblable n'est pas deja presente
   
   if(checkIfPageExist(m_pCurrentPage))
      return;

   m_vectPage.push_back(m_pCurrentPage);
   m_pCurrentPage = NULL;
   m_hasNewPageNoVisited = true;

   // enregistre la page dans le profil
   // si pas en train detre restoré
   if(!m_isRestoring)
      StorageInProfile();
}

void EDiary::addImageToPage(const std::string & pathImg) {
   KAssert(m_pCurrentPage);
   
   size_t posExt = pathImg.find_last_of(".");
   KAssert(posExt != std::string::npos);

   KGraphic *img = EImageBank::getImage(pathImg.substr(0, posExt).c_str(),pathImg.substr(posExt+1).c_str());
   EImageBank::KeepInCache(img);

   GraphicPath* graphicPath = new GraphicPath();
   graphicPath->path = pathImg;
   graphicPath->img = img;

   ElementPage elt;
   elt.img = graphicPath;
   elt.string = NULL;
   m_pCurrentPage->lstElement.push_back(elt);
}

void EDiary::addStringToPage(const char* idText, int idFont, float xRelatif, float yRelatif) {
   KAssert(m_pCurrentPage && idText && idFont >= 0 && idFont < m_listFont.size());

   FontColor* fontColor = m_listFont[idFont];
   EFancyString* string = NULL;
   GraphicalStringTrad *graphString = new GraphicalStringTrad();
   const char* trad = KStr(idText);
   if(trad) {
      string = new EFancyStringMultiline(trad, fontColor->font, xRelatif, WIDTH_MAX_STRING_DIARY - xRelatif, yRelatif, fontColor->color[3], fontColor->color[0], fontColor->color[1], fontColor->color[2]);
      graphString->setTrad(std::string(trad));
      string->idFont = idFont;
   }
   else {
      string = new EFancyStringMultiline(idText, fontColor->font, xRelatif, WIDTH_MAX_STRING_DIARY - xRelatif, yRelatif, fontColor->color[3], fontColor->color[0], fontColor->color[1], fontColor->color[2]);
      graphString->setTrad(std::string(idText));
       string->idFont = idFont;
   }
   
   graphString->setString(idText);
   graphString->setRenderer(string);
   graphString->setColor(fontColor->color[0], fontColor->color[1], fontColor->color[2]);
   graphString->setFreezeColor(true);

   ElementPage elt;
   elt.img = NULL;
   elt.string = graphString;
   m_pCurrentPage->lstElement.push_back(elt);
}

void EDiary::Render() {

   if(m_vectPage.size() == 0)
      return;

   // si page ajouté depuis la maj, on affiche la derniere
   if(m_hasNewPageNoVisited) {
      m_indexPage = m_vectPage.size() - 1;
   }

   float currentY = MARGE_BORD_VERTICAL;
   float xBasePage = 170;

   int indexPage1 = -1;
   int indexPage2 = -1;

   int compt = 0;

   // si page de droite
   if(m_indexPage % 2 != 0) {
      compt = 1;
      indexPage2 = m_indexPage;
      indexPage1 = m_indexPage - 1;
      // la page souhaitée est à droite, alors on affiche aussi celle de gauche
   }
   else {
      indexPage1 = m_indexPage;
      indexPage2 = m_indexPage + 1;
      if(indexPage2 >= m_vectPage.size())
         indexPage2 = -1;
   }

   for(int k = 0; k < 2; ++k) {
      currentY = MARGE_BORD_VERTICAL;
      int indexCurrent = -1;
      // page de gauche
      if(k == 0) {
         xBasePage = 170;
         indexCurrent = indexPage1;
      }
      else {
         xBasePage = 560;
         indexCurrent = indexPage2;
      }

      if(indexCurrent < 0)
         continue;

      // commence par dessiner le bg
      GraphicPath *imgPath = m_vectPage.at(indexCurrent)->imgBackground;
      if(imgPath) {
         imgPath->img->blitAlphaRect(0, 0, imgPath->img->getWidth(), imgPath->img->getHeight(), xBasePage - 25, yOffset + currentY - 15);
      }

      for(unsigned int i = 0; i < m_vectPage.at(indexCurrent)->lstElement.size(); ++i) {
         if(m_vectPage.at(indexCurrent)->lstElement.at(i).string != NULL) {
            // ajouté l'offset
            float xTemp = m_vectPage.at(indexCurrent)->lstElement.at(i).string->x;
            float yTemp = m_vectPage.at(indexCurrent)->lstElement.at(i).string->y;

            m_vectPage.at(indexCurrent)->lstElement.at(i).string->getRenderer()->y += yOffset + currentY;
            m_vectPage.at(indexCurrent)->lstElement.at(i).string->getRenderer()->x += xBasePage;

            m_vectPage.at(indexCurrent)->lstElement.at(i).string->render();

            m_vectPage.at(indexCurrent)->lstElement.at(i).string->getRenderer()->y = yTemp;
            m_vectPage.at(indexCurrent)->lstElement.at(i).string->getRenderer()->x = xTemp;

            currentY += m_vectPage.at(indexCurrent)->lstElement.at(i).string->getRenderer()->getHeight() + MARGE_VERTICAL;
         }
         else if(m_vectPage.at(indexCurrent)->lstElement.at(i).img != NULL) {
            KGraphic* img = m_vectPage.at(indexCurrent)->lstElement.at(i).img->img;
            img->blitAlphaRect(0, 0, img->getWidth(), img->getHeight(), xBasePage + ((WIDTH_MAX_STRING_DIARY/2) - (img->getWidth()/2)), yOffset + currentY);

            currentY += img->getHeight() + MARGE_VERTICAL;
         }
      }
   }

   m_hasNewPageNoVisited = false;

   
   // affichage des flèches
   if((indexPage1 - 1) > -1)
      imgArrowLeft->blitAlphaRect(0, 0, imgArrowLeft->getWidth(), imgArrowLeft->getHeight(), POS_X_ARROW_LEFT, yOffset + POS_Y_ARROW);
   if(indexPage2 != -1 && (indexPage2 + 1) < m_vectPage.size())
      imgArrowRight->blitAlphaRect(0, 0, imgArrowLeft->getWidth(), imgArrowLeft->getHeight(), POS_X_ARROW_RIGHT, yOffset + POS_Y_ARROW);
}

void EDiary::Check() {
   
   short x = KInput::getMouseX();
   short y = KInput::getMouseY();
   if(KInput::getLeftButtonState()) {
      if(mouseClick)
         return;

      mouseClick = true;
      Mouse.EventClose();    

      // clic à gauche du livre pour tourner la page de gauche
      if(x >= POS_X_ARROW_LEFT && x <= POS_X_ARROW_LEFT + imgArrowLeft->getWidth() && y >= yOffset + POS_Y_ARROW && y <= yOffset + POS_Y_ARROW + imgArrowLeft->getHeight())
         turnPageLeft();
      else if(x >= POS_X_ARROW_RIGHT && x <= POS_X_ARROW_RIGHT + imgArrowLeft->getWidth() && y >= yOffset + POS_Y_ARROW && y <= yOffset + POS_Y_ARROW + imgArrowLeft->getHeight())
         turnPageRight();

      else if((x < 100 && y < 670) || x > 880 || (y > 660 && x > 150 && x < 950))
         SCENEDIRECTOR->ShowDiaryManager(false);
   }  
   else
      mouseClick = false;
//   if(Mouse.LeftEvent()) {
//      if(Mouse.x < 100 || Mouse.x > 950 || Mouse.y > 660)
//         SCENEDIRECTOR->ShowDiaryManager(false);
//   }
}

void EDiary::Logic() {

}

void EDiary::turnPageLeft() {

   // si page de droite
   if(m_indexPage % 2 != 0)
      m_indexPage -= 2;
   else
      m_indexPage--;

   if(m_indexPage < 0)
      m_indexPage = 0;
   else
      ESoundBank::getSound("book_pageturn")->playSample();
}

void EDiary::turnPageRight() {

   // si page de droite
   if(m_indexPage % 2 != 0)
      m_indexPage++;
   else
      m_indexPage += 2;

   if(m_indexPage >= m_vectPage.size())
      m_indexPage = m_vectPage.size() -1;
   else
      ESoundBank::getSound("book_pageturn")->playSample();
}