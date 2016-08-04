/*
 *  Vignette_Spirit.h
 *  enigma
 *
 *  Created by Rockford on 11/06/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_VIGNETTE_SPIRIT_H
#define  _ENIGMA_VIGNETTE_SPIRIT_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Vignette_Spirit : public EScene {
public:
   Vignette_Spirit(ESceneDirector *lpSceneDirector);
   ~Vignette_Spirit();
   
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
   
protected:
   
};

#endif