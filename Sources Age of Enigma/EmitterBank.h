/*
 *  EmitterBank.h
 *  enigma
 *
 *  Created by Rockford on 05/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef EMITTERBANK_H
#define EMITTERBANK_H

#include "CreepingSmoke.h"
#include "global.h"

class EEmitterBank : public KObjectListable {
public:
   char _szName[K_MAXPATH];
   char _szPresetFile[K_MAXPATH];
   char _szAnimationFile[K_MAXPATH];

   CreepingSmokeEmitter *_lpEmitter;
   
   // Demande un son à la banque
   static CreepingSmokeEmitter* getEmitter(const char *szEmitterName, const char *szPresetFile, const char *szAnimationFile, bool bPreload=false);
  
   // Destructeur de chaque élement
   static void CleanUp();

   static void NormName(const char *szEmtName, char *dst);
   static bool IsInCache(const char *szObjRef);
   static void PurgeCache();
   static void CleanBankNeded(bool enable=true);
   static EEmitterBank* GetCache(const char *szObjRef);

   static int GetCount() {return g_ListEmitter.getCount();} 
private:
   EEmitterBank(const char* szEmtName, const char *szPresetFile, const char *szAnimationFile);
   ~EEmitterBank();

   friend class Loader;

   bool CouldBeDrop();

   int _nTry;
   bool _bLoaded;
   bool _bSpriteInit;
   int _nLastSceneInd;
   bool _bKeepInCache;
   KSysLock _objLock;

   // Liste
   static KSysLock _listLock;
	static KList<EEmitterBank> g_ListEmitter;
   static bool _bPurgeEnabled;
};

#endif