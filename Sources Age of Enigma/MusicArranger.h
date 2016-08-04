/*
 *  MusicArranger.h
 *  enigma
 *
 *  Created by MEKERSA David on 17/10/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef MUSICARRANGER_H
#define MUSICARRANGER_H

#include "KCounter.h"
#include "KSound.h"

/* Etats pour le crossfading */
typedef enum {
   MUSICARRANGER_SLEEP = 0,
   MUSICARRANGER_PLAYING,
   MUSICARRANGER_FADEOUT
} MusicArrangerMusicState;

#define MUSICARRANGER_FADEOUTDURATION 1000

class EMusicArranger {
public:
   EMusicArranger();
   ~EMusicArranger();
   
   // A appeler à chaque frame !
   void Render(float fElapsed);
   
   // bCross=true : la nouvelle musique est démarrée pendant le fadeout de la précédente
   // bCross=false : la nouvelle musique ne démarre que quand le fadeout de la précédente est terminé
   void PlayMusic(const char *szMusicName, bool bCross);
   
   // Fadeout de la musique courante
   void FadeOut(bool bWithoutFadeout = false);
   
   // Retourne true si une musique est actuellement en lecture
   bool isPlaying();
   
   void SetVolume(long lVolume)
   {
      _lVolume = lVolume;
      if (_lpMusic1 && _MusicState1 == MUSICARRANGER_PLAYING) {
         _lpMusic1->setVolume(lVolume);
      }
      if (_lpMusic2 && _MusicState2 == MUSICARRANGER_PLAYING) {
         _lpMusic2->setVolume(lVolume);
      }
   }
   
   void ResetQueue()
   {
      memset(_szMusicQueue, 0, 256);
   }

private:
   long _lVolume;
   // Première musique
   KSound *_lpMusic1;
   KCounter *_lpCounterFade1;
   MusicArrangerMusicState _MusicState1;
   // 2ème musique
   KSound *_lpMusic2;
   KCounter *_lpCounterFade2;
   MusicArrangerMusicState _MusicState2;
   // Musique en attente
   char _szMusicQueue[256];
};

#endif