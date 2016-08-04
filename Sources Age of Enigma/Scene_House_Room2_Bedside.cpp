/*
 *  Scene_House_Room2_Bedside.cpp
 *  enigma
 *
 *  Created by Rockford on 21/10/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "SoundBank.h"
#include "Scene_House_Room2_Bedside.h"
#include "ESceneDirector.h"

/* Constructeur */
Scene_House_Room2_Bedside::Scene_House_Room2_Bedside(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
}

/* Destructeur */
Scene_House_Room2_Bedside::~Scene_House_Room2_Bedside()
{
}

void Scene_House_Room2_Bedside::Init()
{
   if (TaskResolved("task_house_room2_openbedside")) {
      SetVisible("house_bedside_drawerzone", false);
      SetVisible("house_bedside_drawer", true);
   }   
   else {
      SetVisible("house_bedside_drawerzone", true);
      SetVisible("house_bedside_light", true, false);
   }

}

void Scene_House_Room2_Bedside::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_House_Room2_Bedside::Logic()
{
	EScene::Logic();
}

void Scene_House_Room2_Bedside::Draw()
{
   EScene::Draw();
}

void Scene_House_Room2_Bedside::Close()
{
}

bool Scene_House_Room2_Bedside::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "house_bedside_drawerzone") == 0) {
      ESoundBank::getSound("success")->playSample();
      ResolveTask("task_house_room2_openbedside");
      SetVisible("house_bedside_drawerzone", false);
      SetVisible("house_bedside_light", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_bedside_drawer", true);
      // Start emitter
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      _lpSceneDirector->getSequencer()->Wait(NULL, 3000);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", false);
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_room2", "reveal_ghost");
   }
   return false;
}

bool Scene_House_Room2_Bedside::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_House_Room2_Bedside::ItemUsed(const char *szItemName, const char *szObjectName)
{
   return false;
}

void Scene_House_Room2_Bedside::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   
}
