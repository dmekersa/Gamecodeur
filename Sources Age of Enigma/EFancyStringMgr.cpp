#include "EFancyStringMgr.h"
#include "ESceneDirector.h"

EFancyStringMgr* EFancyStringMgr::m_instance = NULL;

EFancyStringMgr *EFancyStringMgr::getInstance() {
   
   if(!m_instance)
      m_instance = new EFancyStringMgr();
   return m_instance;
}

void EFancyStringMgr::kill() {
   XDELETE(m_instance);
}

KGraphic* EFancyStringMgr::getTextureForString(const char* chaine) {
   std::string chaineTemp(chaine);

   std::map<std::string, KGraphic*>::const_iterator it;

   it = m_mapStringTexture.find(chaineTemp);

   // trouvé
   if(it != m_mapStringTexture.end())
      return (*it).second;
   return NULL;
}

EFancyStringMgr::EFancyStringMgr() {

}

EFancyStringMgr::~EFancyStringMgr() {
   
   std::map<std::string, KGraphic*>::iterator it=m_mapStringTexture.begin();

   for(; it != m_mapStringTexture.end(); it++)
      XDELETE((*it).second);
}