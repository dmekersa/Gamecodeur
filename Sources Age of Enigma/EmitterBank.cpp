/*
 *  EmitterBank.cpp
 *  enigma
 *
 *  Created by Rockford on 05/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "KSysThread.h"
#include "AutoCrit.h"
#include "ThreadLoader.h"

#include "KPTK.h"
#include "MyGame.h"
#include "ImageBank.h"
#include "ESceneDirector.h"

#include "EmitterBank.h"

KList<EEmitterBank> EEmitterBank::g_ListEmitter;
KSysLock EEmitterBank::_listLock;

EEmitterBank::EEmitterBank(const char* szEmtName, const char *szPresetFile, const char *szAnimationFile) {
   memset(_szName,0,K_MAXPATH);
   memset(_szPresetFile,0,K_MAXPATH);
   memset(_szAnimationFile,0,K_MAXPATH);
   strcpy(_szName,szEmtName);
   strcpy(_szPresetFile,szPresetFile);
   if (szAnimationFile != NULL)
      strcpy(_szAnimationFile,szAnimationFile);

   _nTry = 3;
   _bLoaded = false;
   _bSpriteInit = false;
   _lpEmitter = new CreepingSmokeEmitter;
   _lpEmitter->SetWindow(MyGame::getGame()->getKWindow());
   _nLastSceneInd = Loader::GetIndiceScene();
   _bKeepInCache = !Loader::IsScenePreloadActive();
}
EEmitterBank::~EEmitterBank() {
   Sexy::AutoCrit lockA(_listLock);
   Sexy::AutoCrit lockB(Loader::_emtListLock);
   Sexy::AutoCrit lockC(_objLock);

   if (_lpEmitter != NULL && !_bLoaded)
         Loader::g_LoadListEmitter.remove(this);
   g_ListEmitter.remove(this);

   CreepingSmokeEmitter *tmp = _lpEmitter;
   XDELETE(_lpEmitter);
   K_LOG("EEmitterBank: object deleted: %s, scene indice: %04d, id:%08x", _szName,_nLastSceneInd,tmp);
}

CreepingSmokeEmitter *EEmitterBank::getEmitter(const char *szEmitterName, const char *szPresetFile, const char *szAnimationFile, bool bPreload)
{
   char szEmtNameExt[K_MAXPATH];
   NormName(szEmitterName,szEmtNameExt);

   _listLock.acquire();
   EEmitterBank *o = GetCache(szEmtNameExt);
   if (o == NULL) {
      o = new EEmitterBank(szEmtNameExt,szPresetFile,szAnimationFile);

      Loader::_emtListLock.acquire();
      Loader::g_LoadListEmitter.push_back(o);
      Loader::_emtListLock.release();

      g_ListEmitter.addToTail(o);

#ifdef ENIGMA_STATS
      if (bPreload) {
         K_LOG("EEmitterBank -- create emitter, preload: %s, id:%08x", o->_szName,o->_lpEmitter);
         Loader::CacheRequestIncr();
      } else {
         Loader::CacheHitIncr(false);
         K_LOG("EEmitterBank -- create emitter, cache miss: %s, id:%08x", o->_szName,o->_lpEmitter);
      }
#endif
   } else {
      o->_nLastSceneInd = Loader::GetIndiceScene();
   }
   _listLock.release();

   if (bPreload == true)
      return o->_lpEmitter;

   if (o->_nTry == 0) {
      K_LOG("EEmitterBank -- Request for failed file: %s",o->_szName);
      return o->_lpEmitter;
   }

   // Attend la fin du chargement si necessaire
   Loader::_emtListLock.acquire();
   o->_objLock.acquire();
   if (!o->_bLoaded && Loader::g_LoadListEmitter.size()>0 && *(Loader::g_LoadListEmitter.begin()) != o) {
      // passe l'image en tete de liste de preload
      Loader::g_LoadListEmitter.remove(o);
      Loader::g_LoadListEmitter.push_front(o);
   }
   Loader::_emtListLock.release();

   while (!o->_bLoaded) {
      if (o->_nTry == 0) {
         o->_objLock.release();
         K_LOG("EEmitterBank -- Can't load file: %s",o->_szName);
         return NULL;
      }
      o->_objLock.release();
      KSysThread::sleep(1);
      o->_objLock.acquire();
   }
   o->_objLock.release();

   // Attend la fin du chargement si necessaire
   if (!o->_bSpriteInit) {
      o->_objLock.acquire();
      EImageBank *tmp =EImageBank::GetCache(o->_lpEmitter->_lpSpriteGraphic);
      if (tmp!=NULL) {
         tmp->UploadImg();
         o->_lpEmitter->ResetSpriteDim();
         o->_bSpriteInit = true;
      } else {
         KAssert(false);
      }
      o->_objLock.release();
   }
   
   // Check d'equivalence d'emeteur
   if (szAnimationFile != NULL && strcmp(szAnimationFile,o->_szAnimationFile) != 0 ||  strcmp(szPresetFile,o->_szPresetFile) != 0) {
      std::string tmp="emitter name: ";
      tmp = tmp + o->_szName;
      tmp = tmp + "\nCurrent Parsed Scene: " + SCENEDIRECTOR->GetLastParsedSceneName();
      

      tmp = tmp +"\nemitter bank:\n\tPreset File:" + o->_szPresetFile + "\n\tAnimation File:" + o->_szAnimationFile;
      tmp = tmp +"\nemitter request:\n\tPreset File:" + szPresetFile + "\n\tAnimation File:";
      if (szAnimationFile != NULL)
         tmp = tmp + szAnimationFile;
      KMiscTools::messageBox("EEmitterBank -- Duplicated emitter with different param", tmp.c_str());
   }

   // Initialisation du buffer pour le KBatch
   
   return o->_lpEmitter;
}

void EEmitterBank::CleanUp() {
   // Parcours la liste pour en récupérer les valeurs et la vide
   EEmitterBank *o;
   EEmitterBank *otodelete;

   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListEmitter.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         otodelete = o;
         bGotNext = g_ListEmitter.getNext (o);
         XDELETE(otodelete);
      }
   }
}


void EEmitterBank::NormName(const char *szEmtName, char *dst) {
   KAssert(dst != NULL);

   memset(dst, 0, K_MAXPATH);
   strcpy(dst,szEmtName);
}

bool EEmitterBank::IsInCache(const char *szObjRef) {   
   EEmitterBank *o = NULL;
   
   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListEmitter.getHead (o);
   while (bGotNext) {
      if (strcmp(o->_szName,szObjRef) == 0)
         return true;
      bGotNext = g_ListEmitter.getNext(o);
   }
   return false;
}

EEmitterBank* EEmitterBank::GetCache(const char *szObjRef) {
   EEmitterBank *o = NULL;
   
   Sexy::AutoCrit lockA(_listLock);
   bool bGotNext = g_ListEmitter.getHead (o);
   while (bGotNext) {
      if (strcmp(o->_szName,szObjRef) == 0)
         return o;
      bGotNext = g_ListEmitter.getNext(o);
   }
   return NULL;
}


bool EEmitterBank::CouldBeDrop() {
   if (_bKeepInCache || !_bLoaded || Loader::GetIndiceScene() - _nLastSceneInd <= Loader::GetProfPreloadScene())
      return false;

   // Emetteur utilisŽ dans une scne
	EObjectContainer *obj;
	bool bGotNext = EObjectContainer::g_lpListObj.getHead (obj);
	if (bGotNext) {
		while (bGotNext) {
         if (obj->_type == SCENEOBJECT_PARTICLEEMITTER)	// Objet trouvŽ
			{
            if (obj->_lpObjectEmitter->getCreepingSmokeEmitter() == _lpEmitter) {
               K_LOG("EImageBank::CouldBeDrop ERROR: Trying to delete a referenced CreepingSmokeEmitter for %s",_szName);
               return false;
            }
			}
			// Suivant
			bGotNext = EObjectContainer::g_lpListObj.getNext (obj);
		}
	}   
   
   return true;
}
bool EEmitterBank::_bPurgeEnabled = false;
void EEmitterBank::CleanBankNeded(bool enabled) {
   _bPurgeEnabled = enabled;
   K_LOG("EEmitterBank: set clean enabled to %d",_bPurgeEnabled);
}
void EEmitterBank::PurgeCache() {
   if (!_bPurgeEnabled)
      return;
   
   EScene *tmp = SCENEDIRECTOR->GetCurrentEScene();
   if (tmp == NULL || tmp->GetSceneState() != SCENE_STABLE)
      return;

   EEmitterBank *o = NULL;
   EEmitterBank *to_delete = NULL;
   int nDelete = 0;
   
   _listLock.acquire();
   bool bGotNext = g_ListEmitter.getHead (o);
   while (bGotNext) {
      to_delete = o;
      bGotNext = g_ListEmitter.getNext(o);
      if (to_delete->CouldBeDrop() && nDelete == 0) {
         XDELETE(to_delete);
         nDelete++;
      }
      if (nDelete >0)
         break;
   }
   if (nDelete > 0) {
      K_LOG("EEmitterBank purge: current indice scene = %04d\nEEmitterBank purge:%04d deleted emitter",Loader::GetIndiceScene(),nDelete);
   } else {
      CleanBankNeded(false);
   }
   _listLock.release();
}