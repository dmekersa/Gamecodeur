#include "assert.h"
#include "KResource.h"
#include "AutoCrit.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EmitterBank.h"
#include "MusicBank.h"
#include "MyGame.h"
#include "ESceneParser.h"

#include "ThreadLoader.h"

#define PROFPRELOADSCENE 1


// ----------------------------- //
#ifdef TEST_ARCHIVE
#include "KPTK.h"

int nProfpreloadScene = PROFPRELOADSCENE;

bool bFromArchive = false;
KResourceStat statarchive;

bool Loader::InitHasArchive() {
   KResource::stat(KMiscTools::makeFilePath("files.pak"),&statarchive);
   if (statarchive.nModTime == 0)
      bFromArchive = false;
   else 
      bFromArchive = true;
   return bFromArchive;
}

bool Loader::hasArchive() {
   return bFromArchive;
}

bool Loader::Hasarchive() {
   return false;
}
int startGameNumScene = 1;
bool (*Loader::HasArchive) () = &Loader::hasArchive;
#endif
// ----------------------------- //

using namespace ESceneParser;

KSysLock imgPriorityLoadLock;
KSysLock sndPriorityLoadLock;

bool Loader::_bScenePreloadActive = false;
int Loader::_nIndiceScene = 0;
bool Loader::_bChangeScene=false;

// Lock general pour le thread loader
KSysLock Loader::_loaderLock;
std::list< std::string > Loader::_sPreloadSceneName;
std::string Loader::_sLastPreloadedSceneName = "";
bool Loader::_bCloseLoaderThread = false;

// Statics pour le chargement threadé des images
KSysLock Loader::_imgListLock;
std::list< EImageBank * > Loader::g_LoadListImage;
KSysThread *Loader::_lpImgLoadingThread = NULL;
int Loader::_nImageThreadEnabled = 0;

// Statics pour le chargement threadé des sons
KSysLock Loader::_sndListLock;
std::list< ESoundBank * > Loader::g_LoadListSound;
KSysThread *Loader::_lpSndLoadingThread = NULL;

// Statics pour le chargement threadé des emetterus
KSysLock Loader::_emtListLock;
std::list< EEmitterBank * > Loader::g_LoadListEmitter;
KSysThread *Loader::_lpEmtLoadingThread = NULL;

// Statics pour les stats genérales
int Loader::_nCacheRequest = 0;
int Loader::_nCacheHit = 0;
int Loader::_nCacheMiss = 0;

// Initialisation des différent thread de chargement
void Loader::InitThread() {
   if (_lpImgLoadingThread == NULL) {
      _lpImgLoadingThread = new KSysThread(ImgThreadLoop,NULL,0);
      _lpImgLoadingThread->setThreadPriority(0);
   }
   if (_lpSndLoadingThread == NULL) {
      _lpSndLoadingThread = new KSysThread(SndThreadLoop,NULL,0);
      _lpSndLoadingThread->setThreadPriority(0);
   }
   if (_lpEmtLoadingThread == NULL) {
      _lpEmtLoadingThread = new KSysThread(EmtThreadLoop,NULL,0);
      _lpEmtLoadingThread->setThreadPriority(0);
   }
}

void Loader::CloseThread() {
   _bCloseLoaderThread = true;
   if (_lpImgLoadingThread != NULL)
      _lpImgLoadingThread->waitForCompletion();
   if (_lpSndLoadingThread != NULL)
      _lpSndLoadingThread->waitForCompletion();
   if (_lpEmtLoadingThread != NULL)
      _lpEmtLoadingThread->waitForCompletion();
   
   XDELETE(_lpImgLoadingThread);
   XDELETE(_lpSndLoadingThread);
   XDELETE(_lpEmtLoadingThread);
}

// Fonction de chargement des images
bool bImgCleanList = false;
unsigned char *imgbuff =NULL;
size_t imgbuffsize = 0;
#ifdef NOPNG
unsigned char *maskbuff =NULL;
size_t maskbuffsize = 0;
#endif

void Loader::ImgThreadLoop(void *param,long nParam) {
#ifdef _VLD
   VLDEnable();
#endif
   ImgPreLoad();
   ImgInvPreLoad();
   ParticlesPreLoad();

   bool bHoldPLock = false;
   EImageBank *o;
   std::list< EImageBank * >::iterator it;

   while(!_bCloseLoaderThread) {
      if (!bHoldPLock)
         PreloadScene();
      if (bImgCleanList) {
         PreloadImageCleanup();
         bImgCleanList = false;
      }
   
      o = NULL;

      Loader::_imgListLock.acquire();
      if (!Loader::g_LoadListImage.empty()) {
         o = (*Loader::g_LoadListImage.begin());
         o->_objLock.acquire();
         if (o->_bPriorityLoad) {
            if (!bHoldPLock) {
               imgPriorityLoadLock.acquire();
               bHoldPLock =true;
            }
         } else {
            if (bHoldPLock) {
               imgPriorityLoadLock.release();
               bHoldPLock = false;
            }
            sndPriorityLoadLock.acquire();
            sndPriorityLoadLock.release();
         }
         KAssert(o->_nTry>0);   
      }
      Loader::_imgListLock.release();

      if (IsImgThreadEnabled() && o != NULL) {
         bool bLoaded = false;
         int nTry;

         if (!o->_bLoaded) {
#ifdef NOPNG
            bool bSuccess;
            if (strstr(o->_szName,".png") != NULL) {
               std::string filename = o->_szName;
               filename = filename.substr(0,filename.size()-4);
               EGLL();
               bSuccess = KResource::loadResource((filename+"_c.jpg").c_str(),imgbuff,imgbuffsize);
               bSuccess = KResource::loadResource((filename+"_a.jpg").c_str(),maskbuff,maskbuffsize);
               EGLR();
               if (bSuccess) {
                  bSuccess = o->_lpImage->loadPictureWithMaskFromPtr((filename+"_c.jpg").c_str(),imgbuff,imgbuffsize,(filename+"_a.jpg").c_str(),maskbuff,maskbuffsize,true,false,1);
                  free(imgbuff);
                  free(maskbuff);
               }
               maskbuff = NULL;
               maskbuffsize = 0;
            } else {
               EGLL();
               bSuccess = KResource::loadResource(o->_szName,imgbuff,imgbuffsize);
               EGLR();
               if (bSuccess) {
                  bSuccess = o->_lpImage->loadPictureFromPtr(o->_szName,imgbuff,imgbuffsize,true,true,false,1);
                  free(imgbuff);
               }
               
            }
#else
            EGLL();
            bool bSuccess = KResource::loadResource(o->_szName,imgbuff,imgbuffsize);
            EGLR();                  
            if (bSuccess) {
               bSuccess = o->_lpImage->loadPictureFromPtr(o->_szName,imgbuff,imgbuffsize,true,true,false,1);
               free(imgbuff);
            }
#endif
            imgbuff = NULL;
            imgbuffsize = 0;

            if (bSuccess) {
               o->_lpImage->setTextureQuality(true);
               o->_bLoaded = true;
               o->_bPriorityLoad =false;
            } else {
               o->_nTry--;
               if (o->_nTry == 0)
                  K_LOG("Loader Image, unable to load file: %s, id:%08x",o->_szName,o->_lpImage);
               else
                  K_LOG("Loader Image, failed load for  file: %s, id:%08x",o->_szName,o->_lpImage);
            }
            bLoaded = o->_bLoaded;
            nTry = o->_nTry;
         }
         o->_objLock.release();

         if (bLoaded || nTry == 0) {
            Sexy::AutoCrit lock(Loader::_imgListLock);
            Loader::g_LoadListImage.remove(o);
         } else {
            KSysThread::sleep(10);
         }
      } else {
         if (o != NULL)
               o->_objLock.release();
         if (bHoldPLock) {
            imgPriorityLoadLock.release();
            bHoldPLock = false;
         }
         KSysThread::sleep(10);
      }
   }

   //_imgListLock.release();
   // interuption du thread sur un preload prioritaire
   if (bHoldPLock) {
      imgPriorityLoadLock.release();
      bHoldPLock = false;
   }
}

// Fonction de chargement des sons
bool bSndCleanList = false;
unsigned char *sndbuff =NULL;
size_t sndbuffsize = 0;
void Loader::SndThreadLoop(void *param,long nParam) {
#ifdef _VLD
   VLDEnable();
#endif

   SndPreLoad();
   
   char buffName[K_MAXPATH];
   bool bHoldPLock = false;
   ESoundBank *o;
   std::list< ESoundBank * >::iterator it;
   
   while(!_bCloseLoaderThread) {
      if (bSndCleanList) {
         PreloadSoundCleanup();
         bSndCleanList = false;
      }
   
      o = NULL;

      Loader::_sndListLock.acquire();
      if (!Loader::g_LoadListSound.empty()) {
         o = (*Loader::g_LoadListSound.begin());
         o->_objLock.acquire();
         if (o->_bPriorityLoad) {
            if (!bHoldPLock) {
               sndPriorityLoadLock.acquire();
               bHoldPLock =true;
            }
         } else {
            if (bHoldPLock) {
               sndPriorityLoadLock.release();
               bHoldPLock = false;
            }
            imgPriorityLoadLock.acquire();
            imgPriorityLoadLock.release();
         }
         KAssert(o->_nTry>0);
      }
      Loader::_sndListLock.release();
      
      if (o != NULL) {
         bool bLoaded = false;
         int nTry;
         
         if (!o->_bLoaded) {
//            K_LOG("Loader Sound, loadSample file: %s, id:%08x",o->_szName,o->_lpSound);
            SafeMakeFilePath(o->_szName,buffName);
            EGLL();
            bool bSuccess = KResource::loadResource(buffName,sndbuff,sndbuffsize);
            EGLR();
            if (bSuccess) {
               bSuccess = o->_lpSound->loadSampleFromPtr((const char *)sndbuff,sndbuffsize,SAMPLE_OGG, 100, o->_bLoop, 4);
               free(sndbuff);
            }
            sndbuff = NULL;
            sndbuffsize = 0;
            
            if (bSuccess) {
               o->_bLoaded = true;
               o->_bPriorityLoad =false;
            } else {
               o->_nTry--;
               if (o->_nTry == 0)
                  K_LOG("Loader Sound, unable to load file: %s, id:%08x",o->_szName,o->_lpSound);
               else
                  K_LOG("Loader Sound, failed load for file: %s, id:%08x",o->_szName,o->_lpSound);
            }
            bLoaded = o->_bLoaded;
            nTry = o->_nTry;
         }
         o->_objLock.release();
         
         if (bLoaded || nTry == 0) {
            Sexy::AutoCrit lock(Loader::_sndListLock);
            Loader::g_LoadListSound.remove(o);
         } else {
            KSysThread::sleep(10);
         }
      } else {
         if (bHoldPLock) {
            sndPriorityLoadLock.release();
            bHoldPLock = false;
         }

         KSysThread::sleep(10);
      }
   }

   //_sndListLock.release();
   // interuption du thread sur un preload prioritaire
   if (bHoldPLock) {
      sndPriorityLoadLock.release();
      bHoldPLock = false;
   }
}

// Fonction de chargement des emetteurs
void Loader::EmtThreadLoop(void *param,long nParam) {
#ifdef _VLD
   VLDEnable();
#endif   
   char tmpBuff[K_MAXPATH];
   EEmitterBank *o;
   std::list< EEmitterBank * >::iterator it;

   while(!_bCloseLoaderThread) {
      o = NULL;

      Loader::_emtListLock.acquire();
      if (!Loader::g_LoadListEmitter.empty()) {
         o = (*Loader::g_LoadListEmitter.begin());
         o->_objLock.acquire();
         KAssert(o->_nTry>0);
      }
      Loader::_emtListLock.release();
      
      if (o != NULL) {
         bool bLoaded = false;
         int nTry;
         if (!o->_bLoaded) {
            // Validation du chargement des preset a faire
            EGLL();
            o->_lpEmitter->ReadPreset(SafeMakeFilePath(o->_szPresetFile,tmpBuff));
            if (strlen(o->_szAnimationFile)>0) {
               o->_lpEmitter->ReadAnimation(SafeMakeFilePath(o->_szAnimationFile,tmpBuff));
            }
            EGLR();
            o->_lpEmitter->SetSprite(o->_lpEmitter->_szSpriteFile,o->_lpEmitter->_nSpriteDiv);
            o->_bLoaded = true;
            o->_nTry--;
            bLoaded = o->_bLoaded;
            nTry = o->_nTry;
         }
         o->_objLock.release();
         
         if (bLoaded || nTry == 0) {
            Sexy::AutoCrit lock(Loader::_emtListLock);
            Loader::g_LoadListEmitter.remove(o);
            if (!o->_bLoaded)
               K_LOG("Loader Emitter, unable to load file: %s",o->_szName);
         } else {
            KSysThread::sleep(10);
         }
      } else {
         KSysThread::sleep(10);
      }
   }

   //_emtListLock.release();
}

// Programation du chargement d'une scene
void Loader::LoadScene(const char *SceneName, bool bChangeScene) {
   K_LOG("Loader::LoadScene(%s)",SceneName);
   if (!_bScenePreloadActive)
      return;

   // clear des files en attente
   // inscription de la scene a chargé
   Sexy::AutoCrit lock(_loaderLock);
   if (_sLastPreloadedSceneName == SceneName)
      return;
   if (bChangeScene) {
      _bChangeScene = bChangeScene;
      _sPreloadSceneName.clear();
   }
   _sPreloadSceneName.push_back(SceneName);
}
void Loader::IncrIndiceScene() {
   if (_bScenePreloadActive) {
      Sexy::AutoCrit lock(_loaderLock);
      _sPreloadSceneName.clear();
      _nIndiceScene++;
      _bChangeScene = true;
      K_LOG("Loader: set indice scene to %04d",_nIndiceScene);
      EImageBank::CleanBankNeded();
      ESoundBank::CleanBankNeded();
      EEmitterBank::CleanBankNeded();

#ifdef TEST_ARCHIVE
   // pour test utilisé timestamp 1311332400 pour le 15 aout 2011 1313406645
      if (_nIndiceScene == startGameNumScene) {
         time_t fuckingtime =  time(NULL);

         if (statarchive.nModTime == 0 || statarchive.nModTime >= fuckingtime || fuckingtime > 1313406645-(int)GetRandom(60,4000)) {
            Loader::HasArchive = &Loader::Hasarchive;
            nProfpreloadScene = 0;
         }
      }
#endif
   }
}   
int Loader::GetIndiceScene() {return _nIndiceScene;}
#ifdef TEST_ARCHIVE
int Loader::GetProfPreloadScene() { return nProfpreloadScene;}
#else
int Loader::GetProfPreloadScene() { return PROFPRELOADSCENE;}
#endif

// Chargement des scenes
void Loader::PreloadScene() {
#ifdef _VLD
   VLDEnable();
#endif

   // traitement de la scene a charger
   _loaderLock.acquire();
   if (_sPreloadSceneName.size() > 0) {
      std::list< std::string > scenesToParse;
      std::list< std::string > nextScenes;
      std::list< std::string > files; 
      
      bool bPriorityLoad = _bChangeScene;
      _bChangeScene = false;

      scenesToParse.push_back(*(_sPreloadSceneName.begin()));
      _sPreloadSceneName.pop_front();
      _sLastPreloadedSceneName = (*scenesToParse.begin());
      _loaderLock.release();

      int profScene = GetProfPreloadScene();
      bool bAddMenu = true;
      for (std::list< std::string >::iterator scene = scenesToParse.begin(); scene != scenesToParse.end(); scene++) {
         PreloadParseScene(*scene,files,nextScenes);

         for (std::list< std::string >::reverse_iterator it = files.rbegin(); it != files.rend(); it++) {
            const char *tmp;
            if ((tmp = strstr((*it).c_str(),".ogg")) != NULL) {
               bool loop = false;
               if (*(tmp+4) != 0) {
                  if (strcmp(tmp+4,":loop") != 0)
                     KAssert(false);
                  loop =  true;
               }
               ESoundBank::getSound((*it).substr(0,(*it).rfind(".ogg")).c_str(),loop,true,bPriorityLoad);
               continue;
            }
            if (strstr((*it).c_str(),".") == NULL) {
               EImageBank::preLoadAnim((*it).c_str(),bPriorityLoad);
               continue;
            }
            
            if ((tmp = strstr((*it).c_str(),":")) != NULL) {
               std::string filename = (*it).substr(0,tmp-(*it).c_str());
               tmp++;
               EImageBank::getImage(filename.c_str(),"",true,true,bPriorityLoad,tmp);
            } else {
               EImageBank::getImage((*it).c_str(),"",true,true,bPriorityLoad);
            }
         }
        
         if (bPriorityLoad == true)
            bPriorityLoad = false;

         if (profScene>0 && nextScenes.size() > 0) {
            // Elimination des doublons          
            for (std::list< std::string >::iterator sceneCandidate = nextScenes.begin(); sceneCandidate != nextScenes.end(); sceneCandidate++) {
               bool unique = true;
               for (std::list< std::string >::iterator tmpScene = scenesToParse.begin(); tmpScene != scenesToParse.end(); tmpScene++) {
                  if ((*sceneCandidate) == (*tmpScene)) {
                     unique = false;
                     break;
                  }
               }
               if (unique)
                  scenesToParse.push_back((*sceneCandidate));
            }
         }
         
         if (bAddMenu && (profScene == 0 || scene == scenesToParse.end() )) {
            // force l'inclusion du menu si il n'a pas déja été lu
            for (std::list< std::string >::iterator tmpScene = scenesToParse.begin(); tmpScene != scenesToParse.end(); tmpScene++) {
               if ((*tmpScene) == "menu" ) {
                  bAddMenu = false;
                  break;
               }
            }
            if (bAddMenu)
               scenesToParse.push_back("menu");
            bAddMenu = false;
            if (profScene > 0)
               profScene=1;
         }

         profScene--;
      }
      
      bImgCleanList = true;
      bSndCleanList = true;
   } else {
      _loaderLock.release();
   }
}
void Loader::PreloadImageCleanup() {
      EImageBank::_listLock.acquire();
      _imgListLock.acquire();
      int numCleanup = 0;
      for (std::list< EImageBank * >::iterator itimg = g_LoadListImage.begin();itimg != g_LoadListImage.end();) {
         EImageBank *tmpImg = (*itimg);
         tmpImg->_objLock.acquire();
         itimg++;
         if (!tmpImg->_nKeepInCache && !tmpImg->_bLoaded && !tmpImg->_nTry <= 0 && tmpImg->_nLastSceneInd < _nIndiceScene) {
            K_LOG("PreloadImageCleanup: clean useless image preloaded object %s, %h", tmpImg->_szName, tmpImg);
            XDELETE(tmpImg);
            numCleanup++;
         } else {
            tmpImg->_objLock.release();
         }
      }
      _imgListLock.release();
      EImageBank::_listLock.release();
      if (numCleanup > 0)
         K_LOG("PreloadImageCleanup: drop %d image", numCleanup);
}
void Loader::PreloadSoundCleanup() {
   // Clean up old img useless preload
   ESoundBank::_listLock.acquire();
   _sndListLock.acquire();
   int numCleanup = 0;
   for (std::list< ESoundBank * >::iterator itimg = g_LoadListSound.begin();itimg != g_LoadListSound.end();) {
      ESoundBank *tmpSnd = (*itimg);
      tmpSnd->_objLock.acquire();
      itimg++;
      if (!tmpSnd->_bKeepInCache && !tmpSnd->_bLoaded && !tmpSnd->_nTry <= 0 && tmpSnd->_nLastSceneInd < _nIndiceScene) {
         K_LOG("PreloadSoundCleanup: clean sound %s, %h", tmpSnd->_szName, tmpSnd);
         XDELETE(tmpSnd);
         numCleanup++;
      } else {
         tmpSnd->_objLock.release();
      }
   }
   _sndListLock.release();
   ESoundBank::_listLock.release();
   if (numCleanup > 0)
      K_LOG("PreloadSoundCleanup: drop %d sound", numCleanup);
}
void Loader::PreloadParseScene(std::string sceneName, std::list< std::string >& filenames, std::list< std::string >& nextScenes) {
   char pathbuff[K_MAXPATH];
   const std::string *val;
   std::string scenesFilePath(sceneName);
   ESceneParser::SceneObjectList scene;
      
   filenames.clear();
   ESceneParser::ParseScene(scene,SafeMakeFilePath((sceneName + ".scn").c_str(),pathbuff));

   for (ESceneParser::SceneObjectIterator obj = scene.begin(); obj != scene.end(); obj++) {
      val = GetStrValue(obj,"object");
      // force la lecture du menu
      if (val != NULL) {
         if (val->compare("preload") == 0) {
            val = GetStrValue(obj,"files");
            if (val != NULL) {
               const char *start=val->c_str(), *last, *end;
               last = start + strlen(start);
               std::string tmp;
               end = strstr(start,",");

               while (start < last) {
                  end= strstr(start,",");
                  if (end == NULL)
                     end = last;
                  tmp.assign(start,end);
                  strip(tmp);
                  if (tmp.size() > 0)
                     filenames.push_back(tmp);
                  start = end +1;
               }


//               continue;
            }
         } else if (val->compare("image") == 0) {
            val = GetStrValue(obj,"file");
            if (val != NULL) {
               const std::string *basedir = GetStrValue(obj,"basedir");
               if (basedir == NULL) {
                  filenames.push_back(*val);
               } else {
                  filenames.push_back(*val+":"+*basedir);
               }
            }
         } else if (val->compare("emitter") == 0) {
            if (GetStrValue(obj,"type") != NULL && GetStrValue(obj,"type")->compare("creepingsmoke") == 0)
            {
               EEmitterBank::getEmitter((sceneName + GetValue(obj,"name")).c_str(),GetValue(obj,"presetfile"),GetValue(obj,"animationfile"),true);
            }
         } else if (val->compare("scene") == 0) {
            if (GetStrValue(obj,"sounddesign") != NULL) {
               EMusicBank::getMusic(GetStrValue(obj,"sounddesign")->c_str());
               K_LOG("Loader: Prechargement de la music %s",GetStrValue(obj,"sounddesign")->c_str());
            }
         }
      }
      
      val = GetStrValue(obj,"destination");
      if (val != NULL)
         nextScenes.push_back(*val);

   }
}

void Loader::EnableImgThread(bool enabled) {
   if (enabled) {
      _nImageThreadEnabled--;
   } else {
      _nImageThreadEnabled++;
   }
   KAssert(_nImageThreadEnabled >=0);
}
bool Loader::IsImgThreadEnabled() {
   return _nImageThreadEnabled == 0;
}

// --------------------------------------------------------------------- //
// Fonction a utilisé pour le préchargement générale des image son ou autre au lancement du jeux
void Loader::PreLoad() {
   LoadScene("title_splashscreen");
}

void Loader::ImgPreLoad() {
#ifdef DEBUG
   EImageBank::getImage("moleskinex.png","",false,true);
#endif
   EImageBank::getImage("diary/moleskine.png","",false,true);
   EImageBank::getImage("ihm_main.png","",false,true);
   EImageBank::getImage("ihm_pentacle.png","",false,true);
   EImageBank::getImage("ihm_pentacle_blue.png","",false,true);
   EImageBank::getImage("ihm_pentacle_purple.png","",false,true);
   EImageBank::getImage("ihm_pentacle_yellow.png","",false,true);
   EImageBank::getImage("ihm_pentacle_red.png","",false,true);
   EImageBank::getImage("ihm_pentacle_green.png","",false,true);
   EImageBank::getImage("ihm_bulle.png","",false,true);
   EImageBank::getImage("arrow_point.png","",false,true);
   EImageBank::getImage("arrow_walk.png","",false,true);
   EImageBank::getImage("arrow_walkback.png","",false,true);
   EImageBank::getImage("arrow_door.png","",false,true);
   EImageBank::getImage("arrow_action.png","",false,true);
   EImageBank::getImage("arrow_look.png","",false,true);
   EImageBank::getImage("arrow_info.png","",false,true);
   EImageBank::getImage("arrow_talk.png","",false,true);
   EImageBank::getImage("arrow_take.png","",false,true);
   EImageBank::getImage("arrow_wait.png","",false,true);
   EImageBank::getImage("arrow_puzzle.png","",false,true);
   EImageBank::getImage("character_ihm_house.png","",false,true);
   EImageBank::getImage("character_ihm_house_dialog.png","",false,true);
   EImageBank::getImage("character_ihm_universe_dialog.png","",false,true);
   EImageBank::getImage("character_ihm_island.png","",false,true);
   EImageBank::getImage("character_ihm_japan.png","",false,true);
   EImageBank::getImage("character_ihm_inca.png","",false,true);
   EImageBank::getImage("character_ihm_middleage.png","",false,true);
   EImageBank::getImage("character_ihm_egypt.png","",false,true);
   EImageBank::getImage("character_ihm_hell.png","",false,true);
   EImageBank::getImage("dialog_cache.png","",false,true);
   EImageBank::getImage("particule-etoile2.png","",false,true);
   EImageBank::getImage("task_barre.png","",false,true);
   EImageBank::getImage("ihm_maintaskbg.png","",false,true);
   EImageBank::getImage("inventorybuttons.png","",false,true);
   EImageBank::getImage("menubutton.png","",false,true);
   EImageBank::getImage("window.png","",false,true);
   EImageBank::getImage("buttons.png","",false,true);
   EImageBank::getImage("menu_playermenu.jpg","",false,true);
   EImageBank::getImage("menu_butlist.png","",false,true);
   EImageBank::getImage("menu_butmainnew.png","",false,true);
   EImageBank::getImage("menu_butmaindelete.png","",false,true);
   EImageBank::getImage("menu_butmainok.png","",false,true);
   EImageBank::getImage("menu_deleteplayer.png","",false,true);
   EImageBank::getImage("menu_butnew.png","",false,true);
   EImageBank::getImage("menu_butname.png","",false,true);
   EImageBank::getImage("menu_butok.png","",false,true);
   EImageBank::getImage("menu_butcancel.png","",false,true);
   EImageBank::getImage("menu_newplayer.png","",false,true);
   EImageBank::getImage("dialogbox.png","",false,true);
   EImageBank::getImage("dialogbox_halo.png","",false,true);
   EImageBank::getImage("dialogbox_spirit.png","",false,true);
   EImageBank::getImage("dialog_butok.png","",false,true);
   EImageBank::getImage("splash_bfg_normal.jpg","",false,true);
   EImageBank::getImage("splash_bfg_enigma.jpg","",false,true);
   EImageBank::getImage("splash_casualbox.jpg","",false,true);
   EImageBank::getImage("splash_cleophas.jpg","",false,true);
   EImageBank::getImage("loadingbg.jpg","",false,true);
   EImageBank::getImage("barre_menu.png","",false,true);
   EImageBank::getImage("glow.png","",false,true);

   EImageBank::getImage("mosaique_mg_bg.jpg","",false,true);
   EImageBank::getImage("mg_ihm_submit_halo.png","",false,true);
   EImageBank::getImage("mg_ihm.png","",false,true);
   EImageBank::getImage("mg_ihm_halo.png","",false,true);
   EImageBank::getImage("mg_hint2.png","",false,true);
   EImageBank::getImage("mg_hint3.png","",false,true);
}

void Loader::SndPreLoad() {
   ESoundBank::getSound("newobjective",false,true);
   ESoundBank::getSound("book_pageturn",false,true);
   ESoundBank::getSound("talkdialog",false,true);
   ESoundBank::getSound("opendoor",false,true);
   ESoundBank::getSound("gong",false,true);

   ESoundBank::getSound("talons",false,true);
   ESoundBank::getSound("island_walk",false,true);
   ESoundBank::getSound("egypt_walk",false,true);

   ESoundBank::getSound("none",false,true);
   ESoundBank::getSound("reveal",false,true);
   ESoundBank::getSound("ghostappear",false,true);

   ESoundBank::getSound("dialogshow",false,true);
   ESoundBank::getSound("slide",false,true);
   ESoundBank::getSound("successpickup",false,true);
   ESoundBank::getSound("success",false,true);
}

int numImgInv = 0;
bool PreloadFolderInv(char *filename, bool isFolder) {
   if (!isFolder && strstr(filename,"inv_") == filename) {
#ifdef NOPNG
      if (strstr(filename,"_c.jpg") == NULL)
         return true;
      std::string tmp(filename);
      tmp = tmp.substr(0,tmp.size()-6)+".png";
      EImageBank::getImage(tmp.c_str(),"",true,true);
#else
      EImageBank::getImage(filename,"",true,true);
#endif
      numImgInv++;
   }
   return true;
}

void Loader::ImgInvPreLoad() {
   return;
   KResource::enumerateFolderInArchives(KMiscTools::makeFilePath("pictures"),(enumProc) PreloadFolderInv,NULL);
   KMiscTools::enumerateFolder(KMiscTools::makeFilePath("pictures"),(enumProc) PreloadFolderInv,NULL);
   K_LOG("ImgInvPreLoad: %d image inventaire",numImgInv);
}

int numImgParticles = 0;
bool PreloadFolderParticles(char *filename, bool isFolder) {
   if (!isFolder) {
#ifdef NOPNG
      if (strstr(filename,"_c.jpg") == NULL)
         return true;
      std::string tmp("particles/");
      tmp += filename;
      tmp = tmp.substr(0,tmp.size()-6)+".png";
#else
      std::string tmp("particles/");
      tmp += filename;
#endif
      EImageBank::getImage(tmp.c_str(),"",true,true);
      numImgParticles++;
   }
   return true;
}
void Loader::ParticlesPreLoad() {
   return;
   KResource::enumerateFolderInArchives(KMiscTools::makeFilePath("pictures/particles"),(enumProc) PreloadFolderParticles,NULL);
   KMiscTools::enumerateFolder(KMiscTools::makeFilePath("pictures/particles"),(enumProc) PreloadFolderParticles,NULL);
   K_LOG("ParticlesPreLoad: %d image particles",numImgParticles);
}
