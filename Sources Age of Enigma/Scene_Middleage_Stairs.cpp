/*
 *  Scene_Middleage_Stairs.cpp
 *  enigma
 *
 *  Created by Rockford on 06/10/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Middleage_Stairs.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "SoundBank.h"
#include "EMiniJeuMasterMind.h"
#include "MusicBank.h"

/* Constructeur */
Scene_Middleage_Stairs::Scene_Middleage_Stairs(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   
   EMiniJeuMasterMind::Preload();
}

/* Destructeur */
Scene_Middleage_Stairs::~Scene_Middleage_Stairs()
{
   delete _lpBgGraphic;
}

void Scene_Middleage_Stairs::Init()
{
   SetupItem("middleage_sickle");
}

void Scene_Middleage_Stairs::Check()
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

void Scene_Middleage_Stairs::Logic()
{
	EScene::Logic();
}

void Scene_Middleage_Stairs::Draw()
{
   EScene::Draw();
}

void Scene_Middleage_Stairs::Close()
{
}

bool Scene_Middleage_Stairs::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "middleage_sickle") == 0)
   {
      PickupSimple("middleage_sickle", "inv_middleage_sickle");
      return true;
   }
   return false;
}

bool Scene_Middleage_Stairs::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Middleage_Stairs::ItemUsed(const char *szItemName, const char *szObjectName)
{
   return false;
}

void Scene_Middleage_Stairs::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
}
