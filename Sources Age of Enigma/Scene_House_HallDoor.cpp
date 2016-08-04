/*
 *  Scene_House_HallDoor.cpp
 *  enigma
 *
 *  Created by Rockford on 23/06/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_HallDoor.h"
#include "ESceneDirector.h"

/* Constructeur */
SceneHallDoor::SceneHallDoor(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _bInitDone = false;
}

/* Destructeur */
SceneHallDoor::~SceneHallDoor()
{
   delete _lpBgGraphic;
}

void SceneHallDoor::Init()
{
}

void SceneHallDoor::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void SceneHallDoor::Logic()
{
	EScene::Logic();

   if (!_bInitDone)
   {
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "hall_door");
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->ShowImage("showclosedoor","closebg", true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("NATHAN1_SPEECH_1"), "ashley_hall_huhwhat", false, false);
      _lpSceneDirector->getSequencer()->Wait(NULL, 500);
//      _lpSceneDirector->getSequencer()->CloseVignette("closevignettehalldoor");
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_hall", "start", false);
//      _lpSceneDirector->getSequencer()->ShowImage(NULL,"halldestroyed", true);
//      _lpSceneDirector->getSequencer()->EarthQuake("earthquakehall",false,"hall_earthquake");
//      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_fragments", false);
//      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_plaster", false);
//      _lpSceneDirector->getSequencer()->PlaySound(NULL, "hall_earthquakeout");
//      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan9", "nathan14");
//      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan14", "nathan15");
//      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan15", "hall_isaac_waiting");
//      _lpSceneDirector->getSequencer()->ShowImage(NULL,"blank", true);
//      _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/nathanhall.ogv", "tunnel");
//      _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_hall", "start", false);
      _bInitDone = true;
   }
}

void SceneHallDoor::Draw()
{
   EScene::Draw();
}

void SceneHallDoor::Close()
{
   
}

bool SceneHallDoor::ObjectClicked(const char *szObjectName, float x, float y)
{
   return false;
}

bool SceneHallDoor::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

