/*
 *  Scene_Egypt_Mazeentry.cpp
 *  enigma
 *
 *  Created by MEKERSA David on 24/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Egypt_Keyroom_Zoom.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuDice.h"

/* Constructeur */
Scene_Egypt_Keyroom_Zoom::Scene_Egypt_Keyroom_Zoom(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   EMiniJeuDice::Preload();
}

/* Destructeur */
Scene_Egypt_Keyroom_Zoom::~Scene_Egypt_Keyroom_Zoom()
{
   delete _lpBgGraphic;
}

void Scene_Egypt_Keyroom_Zoom::Init()
{
}

void Scene_Egypt_Keyroom_Zoom::Check()
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

void Scene_Egypt_Keyroom_Zoom::Logic()
{
	EScene::Logic();
}

void Scene_Egypt_Keyroom_Zoom::Draw()
{
   EScene::Draw();
}

void Scene_Egypt_Keyroom_Zoom::Close()
{
}

bool Scene_Egypt_Keyroom_Zoom::ObjectClicked(const char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Egypt_Keyroom_Zoom::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Egypt_Keyroom_Zoom::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (strcmp(szItemName, "inv_egypt_key") == 0) {
      if (strcmp(szObjectName, "egypt_missingkey_zone") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_missingkey", true);
         EMiniJeuBase *lpMiniJeu = new EMiniJeuDice();
         _lpSceneDirector->getSequencer()->MiniGame(NULL, "dice", lpMiniJeu);
      }
   }
   
   return false;
}

void Scene_Egypt_Keyroom_Zoom::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (bIsRevolved) {
      ResolveTask("task_egypt_missingkey");
      _lpSceneDirector->DropItem("inv_egypt_key");
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "egypt_keyroom", "missingkeydone", false);
   }
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_missingkey", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
