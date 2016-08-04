/*
 *  Scene_Egypt_Mazeentry.cpp
 *  enigma
 *
 *  Created by MEKERSA David on 24/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Egypt_Keyroom.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"

/* Constructeur */
Scene_Egypt_Keyroom::Scene_Egypt_Keyroom(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_Egypt_Keyroom::~Scene_Egypt_Keyroom()
{
   delete _lpBgGraphic;
}

void Scene_Egypt_Keyroom::Init()
{
   if (getAdditionalName() == "missingkeydone") {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_NEFERES_KEYAPPEAR"), "", true, false);
   }
   
   if (!TaskResolved("task_egypt_getkey")) {
      AddTask("task_egypt_getkey");
   }
   
   if (TaskResolved("task_egypt_missingkey")) {
      SetVisible("egypt_missingkey_zone", false);
      // Interrupteur "cube"
      SetVisible("egypt_keyroom_keyup", true, true);
      // Clé complète
      SetVisible("egypt_keyroom_completedkey", true, true);
   }
   
   if (TaskResolved("task_egypt_openmaze")) {
      SetVisible("egypt_keyroom_keyup",false,true);
      SetVisible("egypt_keyroom_keydown",true,true);
   }
}

void Scene_Egypt_Keyroom::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
   }
#endif
}

void Scene_Egypt_Keyroom::Logic()
{
	EScene::Logic();
}

void Scene_Egypt_Keyroom::Draw()
{
   EScene::Draw();
}

void Scene_Egypt_Keyroom::Close()
{
}

bool Scene_Egypt_Keyroom::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "egypt_missingkey_zone") == 0) {
      _lpSceneDirector->GoToScene("egypt_keyroom_zoom","",true);
      return true;
   }
   
   if (strcmp(szObjectName, "egypt_keyroom_keyup") == 0) {
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "drawer_open");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "egypt_keyroom_keyup", "egypt_keyroom_keydown");
      _lpSceneDirector->getSequencer()->Wait(NULL, 500);
      _lpSceneDirector->getSequencer()->EarthQuake(NULL, true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 4000);
      _lpSceneDirector->getSequencer()->EarthQuake(NULL, false);
      ResolveObjective("egypt","entertomb");
      ResolveTask("task_egypt_openmaze");
      AddTask("task_egypt_resolvemaze");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "egypt_mazeentry", "", false);
   }
   
   return false;
}

bool Scene_Egypt_Keyroom::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Egypt_Keyroom::ItemUsed(const char *szItemName, const char *szObjectName)
{
   
   return false;
}

void Scene_Egypt_Keyroom::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "drawer_close");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
