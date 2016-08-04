/*
 *  Scene_House_Bathroom_Cabinet.cpp
 *  enigma
 *
 *  Created by Rockford on 22/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_House_Bathroom_Cabinet.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"

/* Constructeur */
Scene_House_Bathroom_Cabinet::Scene_House_Bathroom_Cabinet(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_House_Bathroom_Cabinet::~Scene_House_Bathroom_Cabinet()
{
   delete _lpBgGraphic;
}

void Scene_House_Bathroom_Cabinet::Init()
{
}

void Scene_House_Bathroom_Cabinet::Check()
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

void Scene_House_Bathroom_Cabinet::Logic()
{
	EScene::Logic();
}

void Scene_House_Bathroom_Cabinet::Draw()
{
   EScene::Draw();
}

void Scene_House_Bathroom_Cabinet::Close()
{
}

bool Scene_House_Bathroom_Cabinet::AskForClose()
{
   return true;
}

bool Scene_House_Bathroom_Cabinet::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Morceaux de flacons
   if ( strncmp(szObjectName, "house_bathroom_flacon_blue[", strlen("house_bathroom_flacon_blue[")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_bath_flacon_blue_part",2);
   }
   if ( strncmp(szObjectName, "house_bathroom_flacon_green[", strlen("house_bathroom_flacon_green[")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_bath_flacon_green_part",2);
   }
   
   // Ouverture de l'armoire
   if (strcmp(szObjectName, "house_bathroom_door_zone") == 0) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_bathroom_cabinet_open", true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "bathcabinet");
      SetupItem("house_bathroom_flacon_blue[2]");
      SetupItem("house_bathroom_flacon_green[1]");
      return true;
   }
   
   // Back
   if (strcmp(szObjectName, "back") == 0) {
      if (isVisible("house_bathroom_cabinet_open")) {
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->getSequencer()->EarthQuake(NULL, true, "ghost_longvoice_egypt");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "bathcabinet");
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"surprise");
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_bathroom_cabinet_open", "house_bathroom_cabinet_closedfear");
         _lpSceneDirector->getSequencer()->Wait(NULL, 1500);
         _lpSceneDirector->getSequencer()->EarthQuake(NULL, false, "ghost_longvoice_egypt");
         _lpSceneDirector->getSequencer()->Wait(NULL, 500);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      }
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "bathroom", "", false);
   }
   return false;
}

bool Scene_House_Bathroom_Cabinet::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_House_Bathroom_Cabinet::ItemUsed(const char *szItemName, const char *szObjectName)
{
   
   return false;
}

void Scene_House_Bathroom_Cabinet::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
