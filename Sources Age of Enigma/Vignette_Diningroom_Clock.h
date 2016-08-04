/*
 *  Vignette_Diningroom_Clock.h
 *  enigma
 *
 *  Created by Rockford on 11/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_VIGNETTE_DININGCLOCK_H
#define  _ENIGMA_VIGNETTE_DININGCLOCK_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Vignette_Diningroom_Clock : public EScene {
public:
   Vignette_Diningroom_Clock(ESceneDirector *lpSceneDirector);
   ~Vignette_Diningroom_Clock();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   
   bool IsNeedIHM()
   {
      return true;
   }
private:
   KGraphic *_lpBgGraphic;
   KCounter *_lpCounterTiming;
   int   _nSmallHand;
   int   _nBigHand;
   void DisplayHands(int mode);
   
protected:
   
};

#endif