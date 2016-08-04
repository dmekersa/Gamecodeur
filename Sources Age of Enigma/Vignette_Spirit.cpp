/*
 *  Vignette_Spirit.cpp
 *  enigma
 *
 *  Created by Rockford on 11/06/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Vignette_Spirit.h"
#include "ESceneDirector.h"

/* Constructeur */
Vignette_Spirit::Vignette_Spirit(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _lpCounterTiming = new KCounter();
}

/* Destructeur */
Vignette_Spirit::~Vignette_Spirit()
{
   XDELETE(_lpBgGraphic);
   XDELETE(_lpCounterTiming);
}

void Vignette_Spirit::Init()
{
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_POLTERGEIST);
//   int x,y;
//   GetObjectPosition("bg", x, y);
   _lpSceneDirector->CinematicModeStart();
   _lpCounterTiming->startCounter(0.0f, 1.0f, 0, 10000, K_COUNTER_LINEAR);
   StartAnimation("ashleyanim");
}

void Vignette_Spirit::Check()
{
   EScene::Check();
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
}

void Vignette_Spirit::Logic()
{
	EScene::Logic();
   if (_lpCounterTiming)
   {
      if (_lpCounterTiming->isCompleted()) {
         _lpSceneDirector->CinematicModeStop();
         XDELETE(_lpCounterTiming);
         ESoundBank::getSound("poltergeistgone")->playSample();
         _lpSceneDirector->GoToScene(_strSceneAdditionalName.c_str());
      }
   }
}

void Vignette_Spirit::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   EScene::Draw();
   if (_lpCounterTiming) {
      _lpCounterTiming->move(fElapsed);
   }
}

void Vignette_Spirit::Close()
{
}

bool Vignette_Spirit::ObjectClicked(const char *szObjectName, float x, float y)
{
   return false;
}

bool Vignette_Spirit::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}
