/*
 *  ESequenceEarthQuake.cpp
 *  enigma
 *
 *  Created by Rockford on 12/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "ESequenceEarthQuake.h"

#include "MyGame.h"
#include "SoundBank.h"

ESequenceEarthQuake::ESequenceEarthQuake(bool b, const char *szSoundName, ESceneDirector *lpSceneDirector)
{
   _b = b;
   memset(_szSoundName,0,256);
   if (szSoundName) {
      strcpy(_szSoundName, szSoundName);
   }
   _lpSceneDirector = lpSceneDirector;
}

ESequenceEarthQuake::~ESequenceEarthQuake()
{
}

void ESequenceEarthQuake::Logic()
{
}

void ESequenceEarthQuake::Check()
{
}

void ESequenceEarthQuake::Draw()
{
}

bool ESequenceEarthQuake::isCompleted()
{
   return true;
}

void ESequenceEarthQuake::Start()
{
   ESequenceBase::Start();
   
   if (strlen(_szSoundName) > 0) {
      _lpSceneDirector->EarthQuake(_b, _szSoundName);
   }
   else {
      _lpSceneDirector->EarthQuake(_b, NULL);
   }


}
