/*
 *  Scene_Mode.cpp
 *  enigma
 *
 *  Created by Rockford on 22/03/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Mode.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EPointer.h"

/* Constructeur */
Scene_Mode::Scene_Mode(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_Mode::~Scene_Mode()
{
   delete _lpBgGraphic;
}

void Scene_Mode::Init()
{
}

void Scene_Mode::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_Mode::Logic()
{
	EScene::Logic();
}

void Scene_Mode::Draw()
{
   EScene::Draw();
}

void Scene_Mode::Close()
{
}

bool Scene_Mode::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "but_mode_1") == 0)
   {
      SetVisible("selectmode_1", true);
      SetVisible("selectmode_2", false);
      return true;
   }   

   if (strcmp(szObjectName, "but_mode_2") == 0)
   {
      SetVisible("selectmode_1", false);
      SetVisible("selectmode_2", true);
      return true;
   }   
   
   if (strcmp(szObjectName, "but_cancel") == 0)
   {
      _lpSceneDirector->setZoomin(false);
      _lpSceneDirector->GoToScene("menu","", false);
      return true;
   }
   if (strcmp(szObjectName, "but_continue") == 0)
   {
      // Enregistrement du mode
      ResolveTask("__mode_choice__");
      if (isVisible("selectmode_1")) {
         SetGlobal("__mode_casual__","1");
      }
      else {
         SetGlobal("__mode_adventure__","1");
      }
      
      if (_State == SCENE_STABLE) {
         // :dmekersa:20110321 Joue la vidéo au 1er clic sur Play
         if (!TaskResolved("__storyline_intro_video__")) {
            ResolveTask("__storyline_intro_video__");
            _lpSceneDirector->setZoomin(false);
            _lpSceneDirector->GoToScene("video_player","dream", false);
         }
         else {
            _lpSceneDirector->GoToScene(GetStrGlobal("__lastscene__","parvis"));
         }
         return true;
      }
   }

   return false;
}

bool Scene_Mode::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

