/*
 *  Scene_House_Room1.cpp
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Room1.h"
#include "ESceneDirector.h"

/* Constructeur */
SceneRoom1::SceneRoom1(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
SceneRoom1::~SceneRoom1()
{
   delete _lpBgGraphic;
}

void SceneRoom1::Init()
{
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_HOUSE);
   //   _lpSceneDirector->getSequencer()->VoiceOver("parvisvo1", KStr("PARVISVO1"), "parvisvo1");
}

void SceneRoom1::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void SceneRoom1::Logic()
{
	EScene::Logic();
}

void SceneRoom1::Draw()
{
   EScene::Draw();
}

void SceneRoom1::Close()
{
   
}

bool SceneRoom1::ObjectClicked(const char *szObjectName, float x, float y)
{
   //   if ( strcmp(szObjectName, "") == 0)
   //   {
   //      int x,y;
   //      GetObjectPosition("", x, y);
   //      _lpSceneDirector->getSequencer()->Talk("", x, y, "", "");
   //      /* On prend la main, on ne laisse pas passer */
   //      return true;
   //   }
   return false;
}

bool SceneRoom1::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

