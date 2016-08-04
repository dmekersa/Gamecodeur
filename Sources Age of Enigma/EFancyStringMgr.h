#ifndef E_FANCY_STRING_H
#define E_FANCY_STRING_H

// singleton se chargeant d'afficher les fancy string
// stocke dans une texture


#include <map>

class KGraphic;

#define instEFancyStringMgr EFancyStringMgr::getInstance()

class EFancyStringMgr {

public :

   static EFancyStringMgr *getInstance();
   static void kill();

   KGraphic* getTextureForString(const char* chaine);

private : 

   EFancyStringMgr();
   ~EFancyStringMgr();

   static EFancyStringMgr* m_instance;

   std::map<std::string, KGraphic*> m_mapStringTexture; 
};
#endif