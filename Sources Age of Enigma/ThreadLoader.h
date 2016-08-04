/*
 *  ThreadLoader.h
 *  enigma
 *
 *  Created by Rockford on 14/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef LOADER_H
#define LOADER_H

#include "KSysThread.h"
#include "KSysLock.h"
#include <list>

class EImageBank;
class ESoundBank;
class EEmitterBank;

class Loader {
public:
   friend class EImageBank;
   friend class ESoundBank;
   friend class EEmitterBank;

#ifdef TEST_ARCHIVE
   static bool InitHasArchive();
   static bool (*HasArchive)();
   static bool hasArchive();
   static bool Hasarchive();
#endif

   static void InitThread();
   static void CloseThread();
   static void ActiveScenePreload(bool bEnable=true) {_bScenePreloadActive = bEnable;}
   static bool IsScenePreloadActive() {return _bScenePreloadActive;}
   
   static void LoadScene(const char *, bool bChangeScene=false);  // Clear des file d'attente preload
   static void IncrIndiceScene();
   static int GetIndiceScene();
   static int GetProfPreloadScene();

   static int GetCacheHit() { return _nCacheHit;}
   static int GetCacheMiss() { return _nCacheMiss;}
   static int GetCacheRequest() { return _nCacheRequest; }
   static int GetObjRequest() { return _nCacheHit + _nCacheMiss; }
   static int GetPreLoadCount() { return g_LoadListImage.size() + g_LoadListSound.size();}

   static void CacheRequestIncr() {_nCacheRequest++;}
   static void CacheHitIncr(bool bInCache) {
      if (bInCache) {
         _nCacheHit++;
      } else {
         _nCacheMiss++;
      }
   }

   static void PreLoad();
   static void SndPreLoad();

   static void EnableImgThread(bool enabled=true);
   static bool IsImgThreadEnabled();
   
protected:
   static bool _bScenePreloadActive;
   static int _nIndiceScene;
   static bool _bChangeScene;

   static KSysLock _loaderLock; 
   static std::list< std::string > _sPreloadSceneName;
   static std::string _sLastPreloadedSceneName;
   static bool _bCloseLoaderThread;
   
   static void PreloadScene();
   
   static void ImgThreadLoop(void *param,long nParam);
   static KSysLock _imgListLock;
   static std::list< EImageBank * > g_LoadListImage;
   static KSysThread *_lpImgLoadingThread;
   static int _nImageThreadEnabled;

   static void SndThreadLoop(void *param,long nParam);
   static KSysLock _sndListLock;
   static std::list< ESoundBank * > g_LoadListSound;
   static KSysThread *_lpSndLoadingThread;

   static void EmtThreadLoop(void *param,long nParam);
   static KSysLock _emtListLock;
   static std::list< EEmitterBank * > g_LoadListEmitter;
   static KSysThread *_lpEmtLoadingThread;

   static int _nCacheRequest;
   static int _nCacheHit;
   static int _nCacheMiss;

   // ------ Parse le .scn
   static void PreloadParseScene(std::string sceneName, std::list< std::string >& filenames, std::list< std::string >& nextScene);
   static void PreloadImageCleanup();
   static void PreloadSoundCleanup();

   // ------ Prechargement a effectuer au démarrage du jeux (pas de parse de scene)
   static void ImgPreLoad();
   static void ImgInvPreLoad();
   static void ParticlesPreLoad();
};

#endif