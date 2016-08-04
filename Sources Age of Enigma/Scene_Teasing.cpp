/*
 *  Scene_Teasing.cpp
 *  enigma
 *
 *  Created by Rockford on 14/02/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Teasing.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EWatchDog.h"

/* Constructeur */
Scene_Teasing::Scene_Teasing(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   EWatchDog::Disable();
}

/* Destructeur */
Scene_Teasing::~Scene_Teasing()
{
   delete _lpBgGraphic;
}

void Scene_Teasing::Init()
{
#ifdef BFG_TEASING
   if (TaskResolved("task_inca_universeresolved") && TaskResolved("task_island_universeresolved")) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "bg1", true);
   } else {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "bg2", true);
   }
   
   _lpSceneDirector->getSequencer()->Talk(NULL, 220, 155, "Will you resolve Age Of Enigma's six universes?","",true);
   _lpSceneDirector->getSequencer()->Talk(NULL, 220, 155, "Will you resolve the 25 challenging mini-games? (replayable at will)","",true);
   _lpSceneDirector->getSequencer()->Talk(NULL, 220, 155, "I'm sure you will!","",true);
   _lpSceneDirector->getSequencer()->Talk(NULL, 220, 155, "Listen carefully - can you guess the universe from the music you hear?","",true);
   _lpSceneDirector->getSequencer()->Talk(NULL, 220, 155, "See you again!","",true);
#else 
   _lpSceneDirector->getSequencer()->Talk(NULL, 220, 155, "Cette version Beta es périmée","",true);
   _lpSceneDirector->getSequencer()->Talk(NULL, 220, 155, "Merci d'avoir joué.","",true);
#endif
   StartAnimation("ashleyanim");
}

void Scene_Teasing::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
//      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_Teasing::Logic()
{
	EScene::Logic();
}

void Scene_Teasing::Draw()
{
   EScene::Draw();
   // Skip
   KTextFace *lpFont = EFontBank::getFont("tahoma.ttf", 20);
   lpFont->setColor(1.0f, 1.0f, 1.0f, 1.0f);
   lpFont->drawStringFromRight("Click to fill out the survey", 1018, 720);
}

void Scene_Teasing::Close()
{
}

bool Scene_Teasing::ObjectClicked(const char *szObjectName, float x, float y)
{
   MyGame::getGame()->getKWindow()->terminate();
   
   return false;
}

bool Scene_Teasing::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

