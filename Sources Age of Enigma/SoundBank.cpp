/*
 *  SoundBank.cpp
 *  enigma
 *
 *  Created by Rockford on 05/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "KSysThread.h"
#include "AutoCrit.h"
#include "ThreadLoader.h"
#include "Scene_Debug_Preload.h"

#include "KPTK.h"
#include "MyGame.h"
#include "ESceneDirector.h"

#include "SoundBank.h"

KList<ESoundBank> ESoundBank::g_ListSound;
KSysLock ESoundBank::_listLock;

// Lock de securité pour évité des comportement incohérent possible sur la création et la suppréssion de KSound
KSysLock _KSoundSafeLock;

ESoundBank::ESoundBank(const char* szSndName, bool bLoop) {
   Sexy::AutoCrit lock(_objLock);

   _nTry = 3;
   _bLoaded = false;
   _bLoop = bLoop;
   _KSoundSafeLock.acquire();
   _lpSound = new KSound;
   _KSoundSafeLock.release();
   _bKeepInCache = !Loader::IsScenePreloadActive();
   _nLastSceneInd = Loader::GetIndiceScene();
   _bPriorityLoad = false;

   memset(_szName,0,K_MAXPATH);
   strcpy(_szName,szSndName);
}
ESoundBank::~ESoundBank() {
   Sexy::AutoCrit lockA(_listLock);
   Sexy::AutoCrit lockB(Loader::_sndListLock);
   Sexy::AutoCrit lockC(_objLock);

   if (_lpSound != NULL && !_bLoaded)
         Loader::g_LoadListSound.remove(this);
   g_ListSound.remove(this);
   
   KSound *tmp = _lpSound;
   _KSoundSafeLock.acquire();
   XDELETE(_lpSound);
   _KSoundSafeLock.release();
   K_LOG("ESoundBank: object deleted %s, scene indice:%04d, id:%08x", _szName,_nLastSceneInd,tmp);
}

KSound *ESoundBank::getSound(const char *szSoundName, bool bLoop, bool bPreload, bool bPriorityLoad)
{
   K_LOG("ESoundBank::getSound(szSoundName=%s, bLoop=%d, bPreload=%d, bPriorityLoad=%d) -- Start",szSoundName,bLoop,bPreload,bPriorityLoad);
   char szSndNameExt[K_MAXPATH];
   NormName(szSoundName,szSndNameExt);
 
   _listLock.acquire();
   ESoundBank *o = GetCache(szSndNameExt,bLoop);
   if (o == NULL) {
      K_LOG("ESoundBank::getSound -- Call to ESoundBank *o = GetCache(szSndNameExt=%s,bLoop=%d) -> return NULL", szSndNameExt,bLoop);
      // A ce stade, le son n'a pas été trouvé
      // On le charge et on l'ajoute à la liste
      o = new ESoundBank(szSndNameExt,bLoop);
      o->_bPriorityLoad = !bPreload || bPriorityLoad;;
   
      Loader::_sndListLock.acquire();
      if (bPreload && !bPriorityLoad) {
         Loader::g_LoadListSound.push_back(o);
      } else {
         Loader::g_LoadListSound.push_front(o);
      }
      Loader::_sndListLock.release();
      
      g_ListSound.addToTail(o);
   
#ifdef ENIGMA_STATS
      if (bPreload) {
         K_LOG("ESoundBank -- create sound, preload:%s, loop:%d, id:%08x", o->_szName,o->_bLoop,o->_lpSound);
         Loader::CacheRequestIncr();
      } else {
         Loader::CacheHitIncr(false);
         const char *sceneName ="none";
         if (SCENEDIRECTOR) {
            K_LOG("ESoundBank -- create sound, cache miss:%s, loop:%d, scene:%s.scn, id:%08x", o->_szName,o->_bLoop,sceneName, o->_lpSound);
            EScene *tmp = SCENEDIRECTOR->GetCurrentEScene();
            if (tmp != NULL) {
               sceneName = tmp->_strSceneName.c_str();
               if (strcmp(sceneName,"debug_preload") == 0) {
                  sceneName = ((Scene_Debug_Preload *)tmp)->GetCurSceneName();
               }
            }
         }
      }      
#endif
   } else {
      o->_nLastSceneInd = Loader::GetIndiceScene();
   }
   _listLock.release();

   if (bPreload == true)
      return o->_lpSound;

   if ( o->_nTry == 0) {
      K_LOG("ESoundBank -- Request for failed file: %s, loop:%d, id:%08x", o->_szName,o->_bLoop,o->_lpSound);
      return o->_lpSound;
   }

   // Attend la fin du chargement si necessaire
   Loader::_sndListLock.acquire();
   o->_objLock.acquire();
   if (!o->_bLoaded && Loader::g_LoadListSound.size()>0 && *(Loader::g_LoadListSound.begin()) != o) {
      // passe l'image en tete de liste de preload
      Loader::g_LoadListSound.remove(o);
      Loader::g_LoadListSound.push_front(o);
   }
   Loader::_sndListLock.release();

   while (!o->_bLoaded) {
      o->_bPriorityLoad = true;
      if (o->_nTry == 0) {
         K_LOG("ESoundBank -- Can't load file: %s, loop:%d, id:%08x", o->_szName,o->_bLoop,o->_lpSound);
         o->_objLock.release();
         return o->_lpSound;
      }
      o->_objLock.release();
      KSysThread::sleep(1);
      o->_objLock.acquire();
   }
   o->_objLock.release();

   K_LOG("ESoundBank::getSound -- End -- return loaded sound: %s, loop:%d, id:%08x", o->_szName,o->_bLoop,o->_lpSound);
   return o->_lpSound;
}

void ESoundBank::CleanUp() {
   K_LOG("ESoundBank::CleanUp() -- Start");
   // Parcours la liste pour en récupérer les valeurs et la vide
   ESoundBank *o;
   ESoundBank *otodelete;

   _listLock.acquire();
   bool bGotNext = g_ListSound.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         otodelete = o;
         bGotNext = g_ListSound.getNext (o);
         XDELETE(otodelete);
      }
   }
   _listLock.release();
   K_LOG("ESoundBank::CleanUp() -- End");
}

void ESoundBank::NormName(const char *szSndName, char *dst) {
   KAssert(dst != NULL);

   memset(dst, 0, K_MAXPATH);
   strcpy(dst,"sounds/");
   strcat(dst, szSndName);
   strcat(dst, ".ogg");
//   SafeMakeFilePath(dst,dst);
}

bool ESoundBank::IsInCache(const char *szObjRef) {   
   ESoundBank *o = NULL;
   
   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListSound.getHead (o);
   while (bGotNext) {
      if (strcmp(o->_szName,szObjRef) == 0)
         return true;
      bGotNext = g_ListSound.getNext(o);
   }
   return false;
}

ESoundBank* ESoundBank::GetCache(const char *szObjRef, bool bLoop) {
   ESoundBank *o = NULL;
   
   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListSound.getHead (o);
   while (bGotNext) {
      if (strcmp(o->_szName,szObjRef) == 0) {
         if (o->_bLoop == bLoop) {
            return o;
         } else {
            K_LOG("ESoundBank: Possible Loop Mismatch file: %s, loop:%d",o->_szName,o->_bLoop);
         }
      }
      bGotNext = g_ListSound.getNext(o);
   }
   return NULL;
}

bool ESoundBank::CouldBeDrop() {
   if (_bKeepInCache || !_bLoaded || Loader::GetIndiceScene() - _nLastSceneInd <= Loader::GetProfPreloadScene())
      return false;
   return true;
}

void ESoundBank::KeepInCache(KSound *snd) {
   ESoundBank *o = NULL;
   
   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListSound.getHead (o);
   while (bGotNext) {
      if (snd == o->_lpSound) {
         o->_bKeepInCache = true;
         break;
      }
      bGotNext = g_ListSound.getNext(o);
   }
}

bool ESoundBank::_bPurgeEnabled = false;
void ESoundBank::CleanBankNeded(bool enabled) {
   _bPurgeEnabled = enabled;
   K_LOG("EImageBank: set clean enabled to %d",_bPurgeEnabled);
}
void ESoundBank::PurgeCache() {
   if (!_bPurgeEnabled)
      return;

   EScene *tmp = SCENEDIRECTOR->GetCurrentEScene();
   if (tmp == NULL || tmp->GetSceneState() != SCENE_STABLE || !Loader::IsImgThreadEnabled())
      return;

   ESoundBank *o = NULL;
   ESoundBank *to_delete = NULL;
   int nDelete = 0;

   _listLock.acquire();
   bool bGotNext = g_ListSound.getHead (o);
   while (bGotNext) {
      to_delete = o;
      bGotNext = g_ListSound.getNext(o);
      if (to_delete->CouldBeDrop()) {
         XDELETE(to_delete);
         nDelete++;
      }
      if (nDelete >0)
         break;
   }
   if (nDelete > 0) {
      K_LOG("ESoundBank purge: current indice scene = %04d\nESoundBank purge: %04d deleted sound",Loader::GetIndiceScene(),nDelete);
   } else {
      CleanBankNeded(false);
   }
   _listLock.release();
}

void ESoundBank::StopSample(const char *szSoundName) {
   char tmp[K_MAXPATH];
   NormName(szSoundName,tmp);

   Sexy::AutoCrit lock(_listLock);
   ESoundBank *o = NULL;
   bool bGotNext = g_ListSound.getHead (o);
   while (bGotNext) {
      if (strcmp(tmp,o->_szName) == 0) {
         Sexy::AutoCrit lockO(o->_objLock);         
         o->_lpSound->stopSample();
         return;
      }
      bGotNext = g_ListSound.getNext(o);
   }
}