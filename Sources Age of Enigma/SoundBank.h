/*
 *  SoundBank.h
 *  enigma
 *
 *  Created by Rockford on 05/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include "KSound.h"
#include "global.h"

class ESoundBank : public KObjectListable {
public:
   char _szName[K_MAXPATH];
   KSound *_lpSound;
  
   // Demande un son à la banque
   static KSound* getSound(const char *szSoundName, bool bLoop = false, bool bPreload=false, bool bPriorityLoad=false);
   static void StopSample(const char *szSoundName);
   
   // Destructeur de chaque élement
   static void CleanUp();

   static void NormName(const char *szSndName, char *dst);
   static bool IsInCache(const char *szObjRef);
   static ESoundBank* GetCache(const char *szObjRef, bool bLoop=false);

   static void KeepInCache(KSound *snd);
   static void PurgeCache();
   static void CleanBankNeded(bool enabled=true);

   static int GetCount() { return g_ListSound.getCount();}
private:
   ESoundBank(const char* szSndName, bool bLoop=false);
   ~ESoundBank();

   bool CouldBeDrop();

   friend class Loader;

   int _nTry;
   bool _bLoaded;
   bool _bLoop;
   bool _bKeepInCache;
   int _nLastSceneInd;
   KSysLock _objLock;
   bool _bPriorityLoad;

   // Liste
   static KSysLock _listLock;
	static KList<ESoundBank> g_ListSound;
   static bool _bPurgeEnabled;
};

#endif