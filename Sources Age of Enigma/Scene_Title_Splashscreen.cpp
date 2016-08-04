/*
 *  Scene_Title_Splashscreen.cpp
 *  enigma
 *
 *  Created by Rockford on 21/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Title_Splashscreen.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "MusicBank.h"
#include "EPointer.h"
#include "ThreadLoader.h"

/* Constructeur */
Scene_Title_Splashscreen::Scene_Title_Splashscreen(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_Title_Splashscreen::~Scene_Title_Splashscreen()
{
   delete _lpBgGraphic;
}

void Scene_Title_Splashscreen::Init()
{
//   _bPreloadDone = false;

   _lpSound = EMusicBank::getMusic("splashscreen");
   if (_lpSound != NULL)
	   _lpSound->playStream(false);
   
   _lpSceneDirector->getSequencer()->ShowImage(NULL, "splash1", true);
   _lpSceneDirector->getSequencer()->Wait(NULL, 4500);
   _lpSceneDirector->getSequencer()->ShowImage(NULL, "splash2", true);
   _lpSceneDirector->getSequencer()->Wait(NULL, 4500);
   _lpSceneDirector->getSequencer()->ShowImage(NULL, "splash3", true);
   _lpSceneDirector->getSequencer()->Wait(NULL, 4500);
   _lpSceneDirector->getSequencer()->ShowImage(NULL, "splash4", true);
   _lpSceneDirector->getSequencer()->Wait(NULL, 5000);
   _lpSceneDirector->getSequencer()->GoToScene(NULL, "loading", "", false);
}

void Scene_Title_Splashscreen::Check()
{
   //if (!_bPreloadDone)
   //   return;

   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
   // :dmekersa:20110321 On permet de skipper
   if (Mouse.LeftEvent()) {
      _lpSceneDirector->getSequencer()->Reset();
      _lpSceneDirector->setZoomin(false);
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "loading", "", false);
   }
}

void Scene_Title_Splashscreen::Logic()
{
	EScene::Logic();
}

void Scene_Title_Splashscreen::Draw()
{
   // Revert cosmetique pour le BFG
   //if (!_bPreloadDone && _State == SCENE_STABLE) {
   //   _bPreloadDone = true;
   //   Loader::LoadScene(GetStrGlobal("__lastscene__","parvis"));
   //   Loader::SndPreLoad();
   //}

   EScene::Draw();
}

void Scene_Title_Splashscreen::Close()
{
   if (_lpSound != NULL)
	  _lpSound->stopStream();
}

bool Scene_Title_Splashscreen::ObjectClicked(const char *szObjectName, float x, float y)
{
//   _lpSceneDirector->getSequencer()->Reset();
//   _lpSceneDirector->getSequencer()->GoToScene(NULL, "title", "loading", false);
   return false;
}

bool Scene_Title_Splashscreen::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}