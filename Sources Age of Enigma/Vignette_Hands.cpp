/*
 *  vignette_hands.cpp
 *  enigma
 *
 *  Created by Rockford on 19/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "vignette_hands.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "MusicBank.h"
#include "ThreadLoader.h"

#include "ImageBank.h"

/* Constructeur */
Vignette_Hands::Vignette_Hands(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   memset(_szHandName, 0, 256);
   _bGotoDone = false;
   _bSound = false;
}

/* Destructeur */
Vignette_Hands::~Vignette_Hands()
{
}

//object:preload | destination:egypt_land
//object:preload | destination:inca_mainscreen
//object:preload | destination:island_beach
//object:preload | destination:japan_mainscreen
//object:preload | destination:middleage_mainscreen

void Vignette_Hands::Init()
{
   SetVisible("hand_ashley", true);
   StartAnimation("handashley_animp2p");
   if (_strSceneAdditionalName == "island") {
      strcpy(_szHandName, "hand_pirate");
      Loader::LoadScene("island_beach");
   }
   if (_strSceneAdditionalName == "japan") {
      strcpy(_szHandName, "hand_japan");
      Loader::LoadScene("japan_mainscreen");
   }
   if (_strSceneAdditionalName == "inca") {
      strcpy(_szHandName, "hand_inca");
      Loader::LoadScene("inca_mainscreen");
   }
   if (_strSceneAdditionalName == "egypt") {
      strcpy(_szHandName, "hand_egypt");
      Loader::LoadScene("egypt_land");
   }
   if (_strSceneAdditionalName == "middleage") {
      strcpy(_szHandName, "hand_middleage");
      Loader::LoadScene("middleage_mainscreen");
   }

   _lpSceneDirector->getSequencer()->PreloadVideo("videos/soul_tunnel.ogv");     
}

void Vignette_Hands::Check()
{
   EScene::Check();
   if (KInput::isPressed(K_VK_F5))
   {
   }
}

void Vignette_Hands::Logic()
{
	EScene::Logic();

   SceneObjectImage *objimg;
   objimg = _lpSceneDirector->GetCurrentScene()->GetObjectImageByName(_szHandName);
   KAssert(objimg != NULL);
   if (objimg) {
      // Anime de la main terminée
      if (objimg->GetSmartImage()->isAnimateP2P() == false && _State == SCENE_STABLE && !_bGotoDone) {
         _lpSceneDirector->getSequencer()->Reset();
         _lpSceneDirector->getSequencer()->Wait(NULL, 500);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "vortex");
         SetVisible("blank", true, false);
         _bGotoDone = true;
         if (_strSceneAdditionalName == "island") {
            _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/soul_tunnel.ogv", "tunnel");
            _lpSceneDirector->getSequencer()->GoToScene(NULL,"island_beach","arrival",false);
         }
         else if (_strSceneAdditionalName == "japan") {
            _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/soul_tunnel.ogv", "tunnel");
            _lpSceneDirector->getSequencer()->GoToScene(NULL,"japan_mainscreen","arrival",false);
         }
         else if (_strSceneAdditionalName == "inca") {
            _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/soul_tunnel.ogv", "tunnel");
            _lpSceneDirector->getSequencer()->GoToScene(NULL,"inca_mainscreen","arrival",false);
         }
         else if (_strSceneAdditionalName == "egypt") {
            _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/soul_tunnel.ogv", "tunnel");
            _lpSceneDirector->getSequencer()->GoToScene(NULL,"egypt_land","arrival",false);
         }
         else if (_strSceneAdditionalName == "middleage") {
            _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/soul_tunnel.ogv", "tunnel");
            _lpSceneDirector->getSequencer()->GoToScene(NULL,"middleage_mainscreen","arrival",false);
         }
      }
   }
}

void Vignette_Hands::Draw()
{
   if (!_bSound) {
      _bSound = true;
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "hands_vortex");
      SetVisible("hand_ashley", true);
      StartAnimation("handashley_animp2p");
      if (_strSceneAdditionalName == "island") {
         SetVisible("hand_pirate", true);
         StartAnimation("handpirate_animp2p");
      }
      if (_strSceneAdditionalName == "japan") {
         SetVisible("hand_japan", true);
         StartAnimation("handjapan_animp2p");
      }
      if (_strSceneAdditionalName == "inca") {
         SetVisible("hand_inca", true);
         StartAnimation("handinca_animp2p");
      }
      if (_strSceneAdditionalName == "egypt") {
         SetVisible("hand_egypt", true);
         StartAnimation("handegypt_animp2p");
      }
      if (_strSceneAdditionalName == "middleage") {
         SetVisible("hand_middleage", true);
         StartAnimation("handmiddleage_animp2p");
      }
   }
   EScene::Draw();
}

void Vignette_Hands::Close()
{
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, false);
}

bool Vignette_Hands::ObjectClicked(const char *szObjectName, float x, float y)
{
   return false;
}