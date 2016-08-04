/*
 *  MusicArranger.cpp
 *  enigma
 *
 *  Created by MEKERSA David on 17/10/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */
#include "assert.h"

#include "MusicArranger.h"
#include "MusicBank.h"
#include "MyGame.h"

// Constructeur
EMusicArranger::EMusicArranger()
{
   _lpMusic1 = NULL;
   _lpMusic2 = NULL;
   _lpCounterFade1 = new KCounter();
   _lpCounterFade2 = new KCounter();
   _MusicState1 = MUSICARRANGER_SLEEP;
   _MusicState2 = MUSICARRANGER_SLEEP;
   _lVolume = 100.0;
   memset(_szMusicQueue, 0, 256);
}

// Destructeur
EMusicArranger::~EMusicArranger()
{
   XDELETE(_lpCounterFade1);
   XDELETE(_lpCounterFade2);
}

void EMusicArranger::FadeOut(bool bWithoutFadeout
                             )
{
   if (_MusicState1 == MUSICARRANGER_PLAYING) {
      _MusicState1 = MUSICARRANGER_FADEOUT;
      if (!bWithoutFadeout) {
         _lpCounterFade1->startCounter(_lVolume, 0, 0, MUSICARRANGER_FADEOUTDURATION, K_COUNTER_LINEAR);
      }
      else {
         _lpCounterFade1->startCounter(_lVolume, 0, 0, 0, K_COUNTER_LINEAR);
      }
   }
   if (_MusicState2 == MUSICARRANGER_PLAYING) {
      _MusicState2 = MUSICARRANGER_FADEOUT;
      if (!bWithoutFadeout) {
         _lpCounterFade2->startCounter(_lVolume, 0, 0, MUSICARRANGER_FADEOUTDURATION, K_COUNTER_LINEAR);
      }
      else {
         _lpCounterFade2->startCounter(_lVolume, 0, 0, 0, K_COUNTER_LINEAR);
      }
   }
}

void EMusicArranger::Render(float fElapsed)
{
   // On fade la musique en cours
   if (_MusicState1 == MUSICARRANGER_FADEOUT) {
      _lpCounterFade1->move(fElapsed);
      _lpMusic1->setVolume((long)_lpCounterFade1->getCurrentValue());
      if (_lpCounterFade1->isCompleted()) {
         _lpMusic1->stopStream();
         _MusicState1 = MUSICARRANGER_SLEEP;
         // Si il y avait une musique en attente, on la joue sur ce canal
         if (strlen(_szMusicQueue) > 0) {
            PlayMusic(_szMusicQueue, true);
            memset(_szMusicQueue, 0, 256);
         }
      }
   }
   if (_MusicState2 == MUSICARRANGER_FADEOUT) {
      _lpCounterFade1->move(fElapsed);
      _lpMusic2->setVolume((long)_lpCounterFade1->getCurrentValue());
      if (_lpCounterFade2->isCompleted()) {
         _lpMusic2->stopStream();
         _MusicState2 = MUSICARRANGER_SLEEP;
         // Si il y avait une musique en attente, on la joue sur ce canal
         if (strlen(_szMusicQueue) > 0) {
            PlayMusic(_szMusicQueue, true);
            memset(_szMusicQueue, 0, 256);
         }
      }
   }
}

void EMusicArranger::PlayMusic(const char *szMusicName, bool bCross)
{
   // Quelle est la musique en cours, la 1 ou la 2 ?
   int nCurrentMusic = 0;
   // On fade la musique en cours
   if (_MusicState1 == MUSICARRANGER_PLAYING) {
      _MusicState1 = MUSICARRANGER_FADEOUT;
      _lpCounterFade1->startCounter(_lVolume, 0, 0, MUSICARRANGER_FADEOUTDURATION, K_COUNTER_LINEAR);
      nCurrentMusic = 1;
   }
   if (_MusicState2 == MUSICARRANGER_PLAYING) {
      _MusicState2 = MUSICARRANGER_FADEOUT;
      _lpCounterFade2->startCounter(_lVolume, 0, 0, MUSICARRANGER_FADEOUTDURATION, K_COUNTER_LINEAR);
      nCurrentMusic = 2;
   }
   if (_MusicState1 == MUSICARRANGER_FADEOUT) {
      nCurrentMusic = 1;
   }
   if (_MusicState2 == MUSICARRANGER_FADEOUT) {
      nCurrentMusic = 2;
   }
   
   // Si on en est cross fading, on joue direct la nouvelle musique
   // (pendant que la courante fait un fadeout)
   if (bCross) {
      // Si la musique actuelle était sur le canal 1, on joue sur le 2
      if (nCurrentMusic == 1) {
         _lpMusic2->stopStream();
         _lpMusic2 = EMusicBank::getMusic(szMusicName);
         KAssert(_lpMusic2 != NULL);
         if (_lpMusic2) {
            _lpMusic2->setVolume(_lVolume);
            _lpMusic2->playStream(true);
            _MusicState2 = MUSICARRANGER_PLAYING;
         }
      }
      // Si la musique actuelle était sur le canal 2, on joue sur le 1
      if (nCurrentMusic == 2 || nCurrentMusic == 0) {
         _lpMusic1->stopStream();
         _lpMusic1 = EMusicBank::getMusic(szMusicName);
         KAssert(_lpMusic1 != NULL);
         if (_lpMusic1) {
            _lpMusic1->setVolume(_lVolume);
            _lpMusic1->playStream(true);
            _MusicState1 = MUSICARRANGER_PLAYING;
         }
      }
   }
   else {
      // En non cross, on met la nouvelle musique en file d'attente
      // Sauf si aucune musique n'était en cours
      if (nCurrentMusic == 0) {
         _lpMusic1 = EMusicBank::getMusic(szMusicName);
         KAssert(_lpMusic1 != NULL);
         if (_lpMusic1) {
            _lpMusic1->setVolume(_lVolume);
            _lpMusic1->playStream(true);
            _MusicState1 = MUSICARRANGER_PLAYING;
         }
      }
      else {
         strcpy(_szMusicQueue, szMusicName);
      }
   }
}
