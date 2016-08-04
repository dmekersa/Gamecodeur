/*
*  MusicBank.cpp
*  enigma
*
*  Created by MEKERSA David on 17/10/10.
*  Copyright 2010 Casual Games France. All rights reserved.
*
*/

#include "AutoCrit.h"
#include "MusicBank.h"
#include "KPTK.h"
#include "ThreadLoader.h"

#include "MyGame.h"

#ifdef WIN32
namespace {
   bool bTestSound = true;
   bool bStreamEnabled = true;
}

bool EMusicBank::IsSoundStreamEnabled() {
   if (!bTestSound)
      return bStreamEnabled;
   char soundTestName[K_MAXPATH];
   NormName("click",soundTestName);

   KSound *testSound = new KSound();
   EGLL();
   bStreamEnabled = testSound->loadSample(KMiscTools::makeFilePath(soundTestName));
   EGLR();
   XDELETE(testSound);

   bTestSound = false;
   return bStreamEnabled;
}
#endif

EMusicBank::EMusicBank(void) {
   _lpMusic = NULL;
   _bKeepInCache = !Loader::IsScenePreloadActive();
   _nLastSceneInd = Loader::GetIndiceScene();
}
EMusicBank::~EMusicBank(void) {
   g_ListMusic.remove(this);
   XDELETE(_lpMusic);
}

KSound *EMusicBank::getMusic(const char *szMusicName)
{
   // On construit un nom de fichier ogg à partir du nom du son demandé
   char szMusicNameExt[K_MAXPATH];
   NormName(szMusicName,szMusicNameExt);

   Sexy::AutoCrit lock(_listLock);
   // Recherche le son dans la banque, si il n'y est pas on le charge...
   EMusicBank *o;
   bool bGotNext = g_ListMusic.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (strcmp(o->_szName,szMusicNameExt) == 0) {
            K_LOG("EMusicBank: chargement depuis la banque %s",o->_szName);
            o->_nLastSceneInd = Loader::GetIndiceScene();
            return o->_lpMusic;
         }
         bGotNext = g_ListMusic.getNext (o);
      }
   }

   // A ce stade, le son n'a pas été trouvé
   // On le charge et on l'ajoute à la liste
   EMusicBank *lpMusicBank;
   lpMusicBank = new EMusicBank;
   strcpy(lpMusicBank->_szName, szMusicNameExt);
   lpMusicBank->_lpMusic = new KSound;

#ifdef WIN32
   if (IsSoundStreamEnabled()) {
#endif
   K_LOG("EMusicBank: Premier chargement de la musique %s",lpMusicBank->_szName);
   EGLL();
   bool bSuccess = lpMusicBank->_lpMusic->loadStream(KMiscTools::makeFilePath (szMusicNameExt));
   EGLR();
   if (!bSuccess) {
      K_LOG("EMusicBank: unable to load sound stream %s",lpMusicBank->_szName);
   }
#ifdef WIN32
   }
#endif
   //if (!bSuccess) {
   //   //      MyGame::showFileError (szSoundNameExt);
   //   return NULL;
   //}
   g_ListMusic.addToTail(lpMusicBank);
   return lpMusicBank->_lpMusic;
}

// Destructeur de chaque élement
void EMusicBank::CleanUp(){
   Sexy::AutoCrit lock(_listLock);

   // Parcours la liste pour en récupérer les valeurs et la vide
   EMusicBank *o;
   EMusicBank *otodelete;
   bool bGotNext = g_ListMusic.getHead (o);
   while (bGotNext) {
      otodelete = o;
      bGotNext = g_ListMusic.getNext (o);
      XDELETE(otodelete);
   }
   KSound::cleanup();
}


void EMusicBank::NormName(const char *szMusicName, char *dst) {
   KAssert(dst != NULL);

   memset(dst, 0, K_MAXPATH);
   strcpy(dst,"sounds/");
   strcat(dst, szMusicName);
   strcat(dst, ".ogg");
}

KList<EMusicBank> EMusicBank::g_ListMusic;
KSysLock EMusicBank::_listLock;