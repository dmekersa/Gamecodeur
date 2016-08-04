/*
 *  Scene_House_Closet.cpp
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Closet.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"

/* Constructeur */
SceneCloset::SceneCloset(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
SceneCloset::~SceneCloset()
{
   delete _lpBgGraphic;
}

void SceneCloset::Init()
{
   SetupItem("basement_bulb");
   SetupItem("house_zippo_fluid");
   
}

void SceneCloset::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void SceneCloset::Logic()
{
	EScene::Logic();
}

void SceneCloset::Draw()
{
   EScene::Draw();
}

void SceneCloset::Close()
{
   
}

bool SceneCloset::ObjectClicked(const char *szObjectName, float x, float y)
{
   if ( strcmp(szObjectName, "basement_bulb") == 0)
   {
      PickupSimple("basement_bulb","inv_house_basement_bulb");
      return true;
   }
   if ( strcmp(szObjectName, "house_zippo_fluid") == 0)
   {
      PickupMultiple("house_zippo_fluid","inv_house_zippo_fluid",-1);
      return true;
   }
   
   return false;
}

bool SceneCloset::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

