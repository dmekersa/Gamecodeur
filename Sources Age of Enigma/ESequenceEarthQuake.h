/*
 *  ESequenceEarthQuake.h
 *  enigma
 *
 *  Created by Rockford on 12/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef ESEQUENCEEARTHQUAKE_H
#define ESEQUENCEEARTHQUAKE_H

#include "ESequenceBase.h"
#include "ESceneDirector.h"
#include "KPTK.h"
#include "KCounter.h"

class ESequenceEarthQuake : public ESequenceBase {
public:
   ESequenceEarthQuake(bool b, const char *szSoundName, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceEarthQuake();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   bool _b;
   char _szSoundName[256];
   ESceneDirector *_lpSceneDirector;
};

#endif