#ifndef E_DIARY_H
#define E_DIARY_H

#include <vector>
#include <string>

class EFancyString;
class KGraphic;
class KTextFace;
class GraphicalStringTrad;

const int FONT_DIARY_1 = 0;
const int FONT_DIARY_2 = 1;
const int FONT_DIARY_3 = 2;

struct FontColor {
   KTextFace* font;
   float color[4];

   FontColor(): font(NULL) {;}
   ~FontColor();
};

struct GraphicPath {
   KGraphic* img;
   std::string path;

   GraphicPath(): img(NULL) {;}
   ~GraphicPath();
};

struct ElementPage {
   
   GraphicPath *img;
   GraphicalStringTrad *string;

   ElementPage(): img(NULL),string(NULL) {;}
};

struct Page {
   std::vector<ElementPage> lstElement;
   GraphicPath *imgBackground;

   Page(): imgBackground(NULL) {;}
   ~Page();
};

class EDiary {
   
public : 

   EDiary();
   ~EDiary();

   void Check();
   void Logic();
   void Render();

   void Clean();

   void StorageInProfile();
   void RestoreFromProfile();

   void beginCreatePage(const char* urlImgBackground = NULL);
   void endCreatePage();
   void addImageToPage(const std::string & pathImg);
   void addStringToPage(const char* idText, int idFont, float xRelatif = 0.0f, float yRelatif = 0.0f);
   
   bool checkIfPageExist(Page* page);

   bool isUnread() {
      return m_hasNewPageNoVisited;
   }

   float yOffset;

private : 

   // vrai si le diary est en train de se charger du profil;
   bool m_isRestoring;

   void turnPageLeft();
   void turnPageRight();

   void addFontToList(KTextFace* font, float r, float g, float b, float blend);

   std::vector<Page*> m_vectPage;
   Page* m_pCurrentPage;
   bool m_hasNewPageNoVisited;
   int m_indexPage;

   KGraphic* imgArrowLeft;
   KGraphic* imgArrowRight;
   bool test;
   bool mouseClick;

   std::vector<FontColor*> m_listFont;
};
#endif