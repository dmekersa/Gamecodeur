/*
 *  ESoundLoop.cpp
 *  enigma
 *
 *  Created by Rockford on 29/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "ESoundLoop.h"

void ESoundLoop::Start()
{
}

void ESoundLoop::Stop()
{
}

void ESoundLoop::Render()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   _lpCounterFade->move(fElapsed);
   if (_lpCounterFade->isCompleted()) {
      if (_lpCounterFade->getCurrentValue() == 0) {
         if (_bReset) {
            _lpSound->stopSample();
         }
      }
   }
}

void ESoundLoop::Reset()
{
   _bReset = true;
}
