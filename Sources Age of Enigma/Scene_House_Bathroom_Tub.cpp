/*
 *  Scene_House_Bathroom_Tub.cpp
 *  enigma
 *
 *  Created by Rockford on 22/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_House_Bathroom_Tub.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"

/* Constructeur */
Scene_House_Bathroom_Tub::Scene_House_Bathroom_Tub(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_House_Bathroom_Tub::~Scene_House_Bathroom_Tub()
{
   delete _lpBgGraphic;
}

void Scene_House_Bathroom_Tub::Init()
{
   SetupItem("house_bathroom_flacon_blue[1]");
   SetupItem("house_bathroom_flacon_red[1]");
   SetupItem("house_bathroom_flacon_green[2]");
}

void Scene_House_Bathroom_Tub::Check()
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

void Scene_House_Bathroom_Tub::Logic()
{
	EScene::Logic();
}

void Scene_House_Bathroom_Tub::Draw()
{
   EScene::Draw();
}

void Scene_House_Bathroom_Tub::Close()
{
}

bool Scene_House_Bathroom_Tub::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Morceaux de flacons à ramasser
   if ( strncmp(szObjectName, "house_bathroom_flacon_red[", strlen("house_bathroom_flacon_red[")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_bath_flacon_red_part",2);
   }
   if ( strncmp(szObjectName, "house_bathroom_flacon_green[", strlen("house_bathroom_flacon_green[")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_bath_flacon_green_part",2);
   }
   if ( strncmp(szObjectName, "house_bathroom_flacon_blue[", strlen("house_bathroom_flacon_blue[")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_bath_flacon_blue_part",2);
   }
   
   // Visualisation du parchemin
   if (strcmp(szObjectName, "house_bathroom_parchment_zone") == 0) {
      AddTask("task_house_bathroom_disposeflacon");
      SetVisible("house_bathroom_tub_parchment", true);
//      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BATHROOM_PARCHMENT"), "", true, false);
      return true;
   }
   if (strcmp(szObjectName, "house_bathroom_tub_parchment") == 0) {
      SetVisible("house_bathroom_tub_parchment", false);
      return true;
   }
   return false;
}

bool Scene_House_Bathroom_Tub::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_House_Bathroom_Tub::ItemUsed(const char *szItemName, const char *szObjectName)
{
   
   return false;
}

void Scene_House_Bathroom_Tub::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
