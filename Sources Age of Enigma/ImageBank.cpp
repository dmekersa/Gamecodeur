/*
*  ImageBank.cpp
*  enigma
*
*  Created by Rockford on 14/05/10.
*  Copyright 2010 Casual Games France. All rights reserved.
*
*/

#define VIDEOQ0 0
#define VIDEOQ1 1
#define VIDEOQ2 2
#define VIDEOQN 3

#define VIDEOQLABEL0 ""
#define VIDEOQLABEL1 "_Q1"
#define VIDEOQLABEL2 "_Q2"

#ifndef  DEFAULT_VIDEO_QUALITY
#define DEFAULT_VIDEO_QUALITY 0
#endif

#include "KPTK.h"
#include "KMiscTools.h"
#include "MyGame.h"
#include "assert.h"
#include "AutoCrit.h"
#include "global.h"
#include "ThreadLoader.h"
#include "ESceneDirector.h"

#include "ImageBank.h"

int EImageBank::_nVideoQuality = DEFAULT_VIDEO_QUALITY;
KList<EImageBank> EImageBank::g_ListImage;
KSysLock EImageBank::_listLock;

#ifdef K_WIN32
#include <windows.h>

LPTOP_LEVEL_EXCEPTION_FILTER WindowsHandler;
LONG WINAPI ExceptionFilter(struct _EXCEPTION_POINTERS *lpTopLevelExceptionFilter) {
   throw(20);
}
#endif

// --------------------------------------------------------------- //

void EImageBank::LogAll()
{
   K_LOG("******************************** EImageBank::LogAll / START ********************************");
   K_LOG(" --- %d Images in the bank ---",GetCount());
   EImageBank *o;
   _listLock.acquire();
   bool bGotNext = g_ListImage.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (o->_lpImage != NULL) {
            K_LOG(o->_szName);
         }
         bGotNext = g_ListImage.getNext (o);
      }
   }
   _listLock.release();
   K_LOG("******************************** EImageBank::LogAll / END  ********************************");
}

bool EImageBank::LoadImage(const char *szImageName, bool bPreload,bool bPriorityLoad) {
   Sexy::AutoCrit lockA(Loader::_imgListLock);

   _lpImage = KPTK::createKGraphic();
   if (bPreload && !bPriorityLoad) {
      Loader::g_LoadListImage.push_back(this);
   } else {
      Loader::g_LoadListImage.push_front(this);
   }
   return true;
}

bool EImageBank::LoadVideo(const char *szVideoName, bool loop, bool bSeekable, bool bVideoPreload) {
   bool bSuccess;
   char tmpmask[K_MAXPATH];
   char tmp[K_MAXPATH];

   memset(tmpmask,0,K_MAXPATH*sizeof(char));
   memset(tmp,0,K_MAXPATH*sizeof(char));
   strcpy(tmpmask,szVideoName);
   *(strrchr(tmpmask, '.')) = 0;

   // Generation du nom de la video en fonction de la qualité
   switch (_nVideoQuality) {
      case VIDEOQ0:
         strcat(tmpmask,VIDEOQLABEL0);
         break;
      case VIDEOQ1:
         strcat(tmpmask,VIDEOQLABEL1);
         break;
      case VIDEOQ2:
         strcat(tmpmask,VIDEOQLABEL2);
         break;
      default:
         KAssert(false);
   }

   // Ajout des extensions
   strcpy(tmp,tmpmask);
   strcat(tmpmask,"_mask.ogv");
   strcat(tmp,".ogv");

#ifdef _VLD
   // VLD ne supporte pas d'etre actif au moment ou une video (probablement preloadé) es demandé
   VLDDisable();
#endif
   //
   _lpVideoMask = NULL;
   _lpVideoMask = new KVideo;
   EGLL();
   bSuccess = _lpVideoMask->openVideo(tmpmask,bVideoPreload,NULL,NULL,loop,bSeekable);
   EGLR();
   if (!bSuccess) {
      XDELETE(_lpVideoMask);
   }

   _lpVideo = new KVideo;
   EGLL();
   bSuccess = _lpVideo->openVideo(tmp,bVideoPreload,_lpVideoMask,NULL,loop,bSeekable);
   EGLR();
   if (!bSuccess) {
      KMiscTools::messageBox ("Example error", "Couldn't open video");
      XDELETE(_lpVideo);
      XDELETE(_lpVideoMask);
      _nTry = 0;
#ifdef _VLD
      VLDEnable();
#endif
      return bSuccess;
   }
#ifdef _VLD
   VLDEnable();
#endif
   _lpVideo->readFrame();
   _lpVideo->pauseTime();
   _bVideoPlaying = false;
   return bSuccess;
}

EImageBank::EImageBank(const char* szImageName) {
   _lpImage = NULL;
   _lpVideo = NULL;
   _lpVideoMask = NULL;
   _bVideoPlaying = false;
   _bLoaded = false;
   _bUpLoaded = false;
   _nTry = 3;
   _bPriorityLoad = false;

   _nKeepInCache = (int) !Loader::IsScenePreloadActive();
   _nLastSceneInd = Loader::GetIndiceScene();

   memset(_szName, 0, K_MAXPATH);
   strcpy(_szName, szImageName);
}

EImageBank::~EImageBank() {
   Sexy::AutoCrit lockA(_listLock);
   Sexy::AutoCrit lockB(Loader::_imgListLock);
   Sexy::AutoCrit lockC(_objLock);

   if (_lpImage != NULL && _bLoaded == false)
         Loader::g_LoadListImage.remove(this);
   g_ListImage.remove(this);

   KGraphic *tmp = _lpImage;
   XDELETE(_lpImage);
   XDELETE(_lpVideoMask);
   XDELETE(_lpVideo);
   K_LOG("EImageBank: object deleted: %s, scene indice: %04d, id:%08x", _szName,_nLastSceneInd,tmp);
}

KGraphic *EImageBank::getImage(const char *szImageName, const char *szExt, bool bQuiet, bool bPreload, bool bPriorityLoad, const char *baseDir)
{
   bool bSuccess;
   char szImageNameExt[K_MAXPATH];
   NormName(baseDir,szImageName,szExt,szImageNameExt);
   
   // Recherche l'image dans la banque, si il n'y est pas on la charge...
   _listLock.acquire();
   EImageBank *o = GetCache(szImageNameExt);  
   if (o == NULL) {
      // On crée et charge l'image en mémoire
      o = new EImageBank(szImageNameExt);
      o->_bPriorityLoad = !bPreload || bPriorityLoad;
      bSuccess = o->LoadImage(szImageNameExt,bPreload,bPriorityLoad);
      
      if (!bSuccess) {
         if (!bQuiet) {
            showFileError (o->_szName);
         }
         return NULL;
      }

      g_ListImage.addToTail(o);

#ifdef ENIGMA_STATS
      if (bPreload) {
         K_LOG("EImageBank -- create image, preload: %s, id:%08x", o->_szName,o->_lpImage);
         Loader::CacheRequestIncr();
      } else {
         Loader::CacheHitIncr(false);
         const char *sceneName ="none";
         if (SCENEDIRECTOR) {
            EScene *tmp = SCENEDIRECTOR->GetCurrentEScene();
            if (tmp != NULL)
               sceneName = tmp->_strSceneName.c_str();
         }
         K_LOG("EImageBank -- create image, cache miss:%s, scene:%s.scn, id:%08x",o->_szName,sceneName,o->_lpImage);
      }
#endif
   } else {
      o->_nLastSceneInd = Loader::GetIndiceScene();
   }
   _listLock.release();

   if (bPreload)
      return o->_lpImage;
   
   if ( o->_nTry == 0) {
      K_LOG("EImageBank -- Request for failed file: %s, id:%08x", o->_szName,o->_lpImage);
      return o->_lpImage;
   }

   if (!o->UploadImg(false)) {
      return NULL;
   }

   K_LOG("EImageBank -- return uploaded image: %s, id:%08x", o->_szName,o->_lpImage);
   return o->_lpImage;
}
bool EImageBank::UploadImg(bool bQuiet) {
   Loader::_imgListLock.acquire();
   _objLock.acquire();
   if (!_bLoaded && Loader::g_LoadListImage.size() && *(Loader::g_LoadListImage.begin()) != this) {
      // passe l'image en tete de liste de preload
      Loader::g_LoadListImage.remove(this);
      Loader::g_LoadListImage.push_front(this);
   }
   Loader::_imgListLock.release();

   while (!_bLoaded) {
      _bPriorityLoad = true;
      if (_nTry == 0) {
         _objLock.release();
         if (!bQuiet) {
            showFileError (_szName);
         }
         return false;
      }
      _objLock.release();
      KSysThread::sleep(1);
      _objLock.acquire();
   }      

   // Texture Upload
   if (!_bUpLoaded) {
#ifdef NOPNG
      bool bSuccess;
      if (strstr(_szName,".png") != NULL) {
         bSuccess = _lpImage->loadPictureWithMask(NULL, NULL, true,false,2);
      } else {
         bSuccess = _lpImage->loadPicture(NULL, true, true,false,2);
      }
#else
      bool bSuccess = _lpImage->loadPicture(NULL, true, true,false,2);
#endif

      if (!bSuccess) {
         _objLock.release();
         return false;
      }
      K_LOG("UploadImg - loadPicture(%s) done, id:%08x", _szName, _lpImage);
      _bUpLoaded = true;
   }
   _objLock.release();
//   _lpImage->blitRectF(0, 0, 1, 1, 2000, 2000);
   return true;
}

bool EImageBank::preLoadAnim(const char *szAnimName,bool bPriorityLoad) {
   char szImageName[K_MAXPATH];
   char tmpPath[K_MAXPATH];

   std::string fmt = szAnimName;
   fmt = fmt + "/" + szAnimName + "%04d.png";
   int i = 1;

   sprintf(szImageName,fmt.c_str(),i);
   NormName("pictures/",szImageName,"",tmpPath);

   // Préchargement de toutes les images de l'animation
   while(ImageExist(tmpPath)) {
      getImage(szImageName,"",false,true,bPriorityLoad);
      i++;
      sprintf(szImageName,fmt.c_str(),i);
      NormName("pictures/",szImageName,"",tmpPath);
   }
   if (i>1)
      return true;
   return false;
}
   
KVideo *EImageBank::getVideo(const char *szVideoName, bool loop, bool bSeekable, bool bVideoPreload, bool bQuiet)
{
   char szVideoNameExt[K_MAXPATH];
   NormName("videos/",szVideoName,".ogv",szVideoNameExt);

   // Recherche la video dans la banque, si il n'y est pas on la charge...
   _listLock.acquire();
   EImageBank *o = GetCache(szVideoNameExt);
   if (o != NULL) {
      if (o->_nTry == 0)
         return o->_lpVideo;

      o->_nLastSceneInd = Loader::GetIndiceScene();
      _listLock.release();
      if (!o->_bVideoPlaying)
         o->_lpVideo->resumeTime();
      o->_lpVideo->rewind();
      o->_lpVideo->readFrame();
      o->_lpVideo->pauseTime();
      o->_bVideoPlaying = false;
      return o->_lpVideo;
   }
   _listLock.release();
      
   // On crée et charge la video en mémoire
   o = new EImageBank(szVideoNameExt);
   bool bSuccess = o->LoadVideo(szVideoNameExt, loop, bSeekable, bVideoPreload);

   if (!bSuccess) {
      if (!bQuiet) {
         showFileError (o->_szName);
      }
      return o->_lpVideo;
   }
   
   o->_bLoaded = true;
   _listLock.acquire();
   g_ListImage.addToTail(o);
   _listLock.release();
   return o->_lpVideo;
}

void EImageBank::Release(KGraphic *img) {
   EImageBank *o;
   _listLock.acquire();
   bool bGotNext = g_ListImage.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (o->_lpImage == img) {
            XDELETE(o);
            return;
         }
         bGotNext = g_ListImage.getNext (o);
      }
   }
   _listLock.release();
}

void EImageBank::VideoPlay(KVideo *video) {
   EImageBank *tmp = GetCache(video);
   if (tmp != NULL && !tmp->_bVideoPlaying) {
      video->resumeTime();
      tmp->_bVideoPlaying = true;
      Loader::EnableImgThread(false);
   }
}

void EImageBank::VideoStop(KVideo *video) {
   KAssert(video != NULL);
   EImageBank *tmp = GetCache(video);
   if (tmp != NULL && tmp->_bVideoPlaying) {
      video->pauseTime();
      tmp->_bVideoPlaying = false;
      Loader::EnableImgThread(true);
   }
}

void EImageBank::SetVideoStatus(KVideo *video, bool isPlaying) {
   if (video == NULL)
      return;

   // Recherche la video dans la banque et initialise le status.
   Sexy::AutoCrit lockA(_listLock);
   EImageBank *o;
   bool bGotNext = g_ListImage.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (o->_lpVideo != NULL && o->_lpVideo == video) {
            o->_bVideoPlaying = isPlaying;
            return;
         }
         bGotNext = g_ListImage.getNext (o);
      }
   }
}

void EImageBank::PauseAllVideos() { 
   // Arrete toutes les videos etant en train de joués
   Sexy::AutoCrit lockA(_listLock);
   EImageBank *o;
   bool bGotNext = g_ListImage.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (o->_bVideoPlaying) {
            o->_lpVideo->pauseTime();
         }
         bGotNext = g_ListImage.getNext (o);
      }
   }
}

void EImageBank::ResumeAllVideos() {
   // Redémare toutes les videos etant en train de joués
   Sexy::AutoCrit lockA(_listLock);
   EImageBank *o;
   bool bGotNext = g_ListImage.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (o->_bVideoPlaying) {
            o->_lpVideo->resumeTime();
         }
         bGotNext = g_ListImage.getNext (o);
      }
   }
}

bool EImageBank::IsVideoPlaying(KVideo *video) {
   if (video = NULL)
      return false;

   Sexy::AutoCrit lockA(_listLock);
   EImageBank *o;
   bool bGotNext = g_ListImage.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (o->_lpVideo != NULL && o->_lpVideo == video) {
            return o->_bVideoPlaying;
         }
         bGotNext = g_ListImage.getNext (o);
      }
   }
   return false;
}

void EImageBank::ReleaseAllVideos() {
   EImageBank *o;
   EImageBank *old;

   _listLock.acquire();
   bool bGotNext = g_ListImage.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         old = o;
         bGotNext = g_ListImage.getNext (o);

         if (old->_lpVideo != NULL) {
            XDELETE(old);		
         }
      }
   }
   _listLock.release();

   KVideo::cleanup();
}
void EImageBank::CleanUp(){
   ReleaseAllVideos();

   // Parcours la liste pour en récupérer les valeurs et la vide
   _listLock.acquire();
   EImageBank *o;
   EImageBank *otodelete;
   bool bGotNext = g_ListImage.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         o->_objLock.acquire();
         otodelete = o;
         bGotNext = g_ListImage.getNext (o);
         XDELETE(otodelete);
      }
   }
   _listLock.release();
}

void EImageBank::showFileError (const char *lpszFileName) {
   static char szFileMsg[K_MAXPATH];

   KStringFmt (szFileMsg, K_MAXPATH, KStr ("ERR_BODY"), lpszFileName);
   KMiscTools::messageBox (KStr ("ERR_TITLE"), szFileMsg);
}

void EImageBank::NormName(const char *szRepName, const char *szImgName, const char *szExt, char *dst) {
   KAssert(dst != NULL);

   memset(dst, 0, K_MAXPATH);
   strcpy(dst, szRepName);
   strcat(dst, szImgName);
   // Si l'extension n'est pas dans le nom
   if (strstr(dst, ".")==NULL) {
      strcat(dst, ".");
      strcat(dst, szExt);
   }
   SafeMakeFilePath(dst,dst);
}

bool EImageBank::IsInCache(const char *szObjRef) {   
   EImageBank *o = NULL;
   
   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListImage.getHead (o);
   while (bGotNext) {
      if (strcmp(o->_szName,szObjRef) == 0)
         return true;
      bGotNext = g_ListImage.getNext(o);
   }
   return false;
}

EImageBank* EImageBank::GetCache(const char *szObjRef) {
   EImageBank *o = NULL;
   
   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListImage.getHead (o);
   while (bGotNext) {
      if (strcmp(o->_szName,szObjRef) == 0)
         return o;
      bGotNext = g_ListImage.getNext(o);
   }
   return NULL;
}

EImageBank* EImageBank::GetCache(KGraphic *img) {
   EImageBank *o = NULL;
   
   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListImage.getHead (o);
   while (bGotNext) {
      if (o->_lpImage == img)
         return o;
      bGotNext = g_ListImage.getNext(o);
   }
   return NULL;
}

EImageBank* EImageBank::GetCache(KVideo *video) {
   EImageBank *o = NULL;
   
   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListImage.getHead (o);
   while (bGotNext) {
      if (o->_lpVideo == video)
         return o;
      bGotNext = g_ListImage.getNext(o);
   }
   return NULL;
}

bool EImageBank::ImageExist(const char* szNormedImageName) {
   KResourceStat statfile;
   EImageBank *tmp;
   
   if ((tmp  = EImageBank::GetCache(szNormedImageName)) != NULL)
      return true;

#ifdef NOPNG
   bool bFileExist;
   if (strstr(szNormedImageName,".png")) {
      std::string filename = szNormedImageName;
      filename = filename.substr(0,filename.size()-4)+"_c.jpg";
      EGLL();
      bFileExist = KResource::stat(filename.c_str(),&statfile);
      EGLR();
   } else {
      EGLL();
      bFileExist = KResource::stat(szNormedImageName,&statfile);
      EGLR();
   }
#else
   EGLL();
   bool bFileExist = KResource::stat(szNormedImageName,&statfile);
   EGLR();
#endif

   return bFileExist;
}

bool EImageBank::CouldBeDrop() {
   // Critères de preload
   if (_nKeepInCache || !_bLoaded || Loader::GetIndiceScene() - _nLastSceneInd <= Loader::GetProfPreloadScene())
      return false;

   // Image utilisée dans une scène
	EObjectContainer *obj;
	bool bGotNext = EObjectContainer::g_lpListObj.getHead (obj);
	if (bGotNext) {
		while (bGotNext) {
         if (obj->_type == SCENEOBJECT_IMAGE)	// Objet trouvé
			{
            if (obj->_lpObjectImage->getGraphic() == _lpImage) {
               K_LOG("EImageBank::CouldBeDrop ERROR: Trying to delete a referenced KGraphic for %s",_szName);
               return false;
            }
			}
			// Suivant
			bGotNext = EObjectContainer::g_lpListObj.getNext (obj);
		}
	}   
   
   return true;
}

void EImageBank::KeepInCache(KGraphic *img) {
   EImageBank *o = NULL;
   
   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListImage.getHead (o);
   while (bGotNext) {
      if (img == o->_lpImage) {
         o->_nKeepInCache++;
         break;
      }
      bGotNext = g_ListImage.getNext(o);
   }
}
void EImageBank::NotKeepInCache(KGraphic *img) {
   EImageBank *o = NULL;
   
   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListImage.getHead (o);
   while (bGotNext) {
      if (img == o->_lpImage) {
         o->_nKeepInCache--;
         if (o->_nKeepInCache < 0) {
            K_LOG("NotKeepInCache: error %80h to much release",o);
            o->_nKeepInCache = 0;
         }
         break;
      }
      bGotNext = g_ListImage.getNext(o);
   }
}

bool EImageBank::_bPurgeEnabled = false;
void EImageBank::CleanBankNeded(bool enabled) {
   _bPurgeEnabled = enabled;
   K_LOG("EImageBank: set clean enabled to %d",_bPurgeEnabled);
}
void EImageBank::PurgeCache() {
   if (!_bPurgeEnabled)
      return;

   EScene *tmp = SCENEDIRECTOR->GetCurrentEScene();
   if (tmp == NULL || tmp->GetSceneState() != SCENE_STABLE || !Loader::IsImgThreadEnabled())
      return;

   EImageBank *o = NULL;
   EImageBank *to_delete = NULL;
   int nDelete = 0;
   int nVideoLeft =0;
   bool bVideoDeleted = false;
   
   _listLock.acquire();
   bool bGotNext = g_ListImage.getHead (o);
   while (bGotNext) {
      to_delete = o;
      if (to_delete->_lpVideo != NULL) {
         nVideoLeft++;

         if (o->_lpVideo->isEndReached() && o->_bVideoPlaying) {
            o->_lpVideo->pauseTime();
            o->_bVideoPlaying = false;
            Loader::EnableImgThread(true);
         }
      }

      bGotNext = g_ListImage.getNext(o);
      if (nDelete > 0)
         continue;
      if (to_delete->CouldBeDrop() && nDelete == 0) {
         if (to_delete->_lpVideo != NULL) {
            K_LOG("Drop video");
            bVideoDeleted = true;
            nVideoLeft--;
         }
         XDELETE(to_delete);
         nDelete++;
      }
   }
   if (nDelete > 0) {
      if (bVideoDeleted && nVideoLeft == 0) {
         K_LOG("EImageBank: Video ressource cleanup all");
         KVideo::cleanup();
      }
      K_LOG("EImageBank purge: current indice scene = %04d\nEImageBank purge:%04d deleted image",Loader::GetIndiceScene(),nDelete);
   } else {
      CleanBankNeded(false);
   }
   _listLock.release();
}