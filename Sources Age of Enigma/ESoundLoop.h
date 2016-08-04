/*
 *  ESoundLoop.h
 *  enigma
 *
 *  Created by Rockford on 29/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */
#ifndef H_ESOUNDLOOP
#define H_ESOUNDLOOP

#include "KPTK.h"
#include "KCounter.h"
#include "MyGame.h"
#include "SoundBank.h"
#include "assert.h"

class ESoundLoop {
   
public:
	ESoundLoop(const char *szSound)
   {
      _lpCounterFade = new KCounter();
      _lpSound = new KSound();
      _lpSound = ESoundBank::getSound(szSound);
      KAssert(_lpSound != NULL);
   }
	~ESoundLoop()
   {
      XDELETE(_lpCounterFade);
      XDELETE(_lpSound);
   }
   
   void Start();
   void Stop();
   void Render();
   void Reset();
   
private:
   bool _bReset;
   KCounter *_lpCounterFade;
   KSound *_lpSound;
   
};

#endif