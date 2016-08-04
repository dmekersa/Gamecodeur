/*
 *  MusicBank.h
 *  enigma
 *
 *  Created by MEKERSA David on 17/10/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef MUSICBANK_H
#define MUSICBANK_H

#include "KSound.h"
#include "KSysLock.h"
#include "global.h"

class EMusicBank : public KObjectListable {
public:
   char _szName[K_MAXPATH];
   KSound *_lpMusic;

   // Demande un son à la banque
   static KSound* getMusic(const char *szMusicName);

   // Destructeur de chaque élement
   static void CleanUp();

#ifdef WIN32
   static bool IsSoundStreamEnabled();
#endif
   
   static int GetCount() {return g_ListMusic.getCount();}
private:
   EMusicBank(void);
   ~EMusicBank(void);

   static void NormName(const char *szMusicName, char *dst);

   bool _bKeepInCache;
   int _nLastSceneInd;

   // Liste
   static KList<EMusicBank> g_ListMusic;
   static KSysLock _listLock;
};

#endif