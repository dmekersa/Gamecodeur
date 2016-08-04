/*
 *  SceneStairsDown.cpp
 *  enigma
 *
 *  Created by Rockford on 29/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "SceneStairsDown.h"
#include "ESceneDirector.h"

/* Constructeur */
SceneStairsDown::SceneStairsDown(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
SceneStairsDown::~SceneStairsDown()
{
   delete _lpBgGraphic;
}

void SceneStairsDown::Init()
{
//   // Teasing !!
//#ifdef BFG_TEASING
//   if (TaskResolved("task_inca_universeresolved") && TaskResolved("task_island_universeresolved")) {
//      _lpSceneDirector->getSequencer()->GoToScene(NULL, "teasing", "", false);
//   }
//#endif
}

void SceneStairsDown::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void SceneStairsDown::Logic()
{
	EScene::Logic();
}

void SceneStairsDown::Draw()
{
   EScene::Draw();
}

void SceneStairsDown::Close()
{
   
}

bool SceneStairsDown::ObjectClicked(const char *szObjectName, float x, float y)
{
   return false;
}

bool SceneStairsDown::ObjectOver(char *szObjectName, float x, float y)
{
   if ( strcmp(szObjectName, "corridor") == 0 )
   {
      _lpSceneDirector->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_WALKBACK);
   }
   else {
      _lpSceneDirector->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
   }
   return false;
}
