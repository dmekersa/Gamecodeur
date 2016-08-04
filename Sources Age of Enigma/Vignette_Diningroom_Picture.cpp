/*
 *  Vignette_Diningroom_Picture.cpp
 *  enigma
 *
 *  Created by Rockford on 14/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Vignette_Diningroom_Picture.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuTemplate.h"

/* Constructeur */
Vignette_Diningroom_Picture::Vignette_Diningroom_Picture(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   EMiniJeuTemplate::Preload();
}

/* Destructeur */
Vignette_Diningroom_Picture::~Vignette_Diningroom_Picture()
{
}

void Vignette_Diningroom_Picture::Init()
{
   if (!TaskResolved("task_house_dining_cleanpicture"))
   {
      SetVisible("frame_picture_dirty", true,true);
      StartEmitter("flake");
   }
   else {
      StopEmitter("flake");
   }

   _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("DININGPICTURE_ASHLEY_INTRO"), "");
}

void Vignette_Diningroom_Picture::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Vignette_Diningroom_Picture::Logic()
{
	EScene::Logic();
}

void Vignette_Diningroom_Picture::Draw()
{
   EScene::Draw();
}

void Vignette_Diningroom_Picture::Close()
{
}

bool Vignette_Diningroom_Picture::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "close") == 0)
   {
      _lpSceneDirector->CloseVignette();
      return true;
   }
   if (strcmp(szObjectName, "frame_picture_dirty") == 0)
   {
      StopEmitter("flake");
      SetVisible("frame_picture_dirty", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGPICTURE_ASHLEY_CLOCK"), "", false);
      ESoundBank::getSound("success")->playSample();
      ResolveTask("task_house_dining_cleanpicture");
   }

   return false;
}

bool Vignette_Diningroom_Picture::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Vignette_Diningroom_Picture::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur utilise le torchon sur le bulbe cassé
   if (strcmp(szObjectName, "frame_picture_dirty") == 0)
      if ( strcmp(szItemName, "inv_house_basement_rag") == 0 ) {
      {
         StopEmitter("flake");
         SetVisible("frame_picture_dirty", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGPICTURE_ASHLEY_CLOCK"), "", false);
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_house_dining_cleanpicture");
         return true;
      }
   }
   return false;
}

void Vignette_Diningroom_Picture::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
}