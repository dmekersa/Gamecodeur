/*
 *  Scene_House_Kitchen.cpp
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Egypt_Shop.h"
#include "EMiniJeuSymbolMatch.h"
#include "ESceneDirector.h"

/* Constructeur */
Scene_Egypt_Shop::Scene_Egypt_Shop(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   EMiniJeuSymbolMatch::Preload();
}

/* Destructeur */
Scene_Egypt_Shop::~Scene_Egypt_Shop()
{
   delete _lpBgGraphic;
}

void Scene_Egypt_Shop::Init()
{
}

void Scene_Egypt_Shop::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_Egypt_Shop::Logic()
{
	EScene::Logic();
}

void Scene_Egypt_Shop::Draw()
{
   EScene::Draw();
}

void Scene_Egypt_Shop::Close()
{
   
}

bool Scene_Egypt_Shop::ObjectClicked(const char *szObjectName, float x, float y)
{
   if ( strcmp(szObjectName, "gameboard_zone") == 0)
   {
      EMiniJeuBase *lpMiniJeu = new EMiniJeuSymbolMatch();
      _lpSceneDirector->GoToMiniGame("match", lpMiniJeu);
      return true;
   }
   return false;
}

bool Scene_Egypt_Shop::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

void Scene_Egypt_Shop::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "match") == 0 && bIsRevolved) {
      ESoundBank::getSound("success")->playSample();
      ResolveTask("task_egypt_getkey");
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"egypt_land","completed",false);
   }
   
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}

