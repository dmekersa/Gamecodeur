/*
 *  SceneVortexEgypt.cpp
 *  enigma
 *
 *  Created by Rockford on 29/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Vortex.h"
#include "ESceneDirector.h"
#include "MyGame.h"
#include "SoundBank.h"
#include "ImageBank.h"

/* Constructeur */
Scene_Vortex::Scene_Vortex(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = EImageBank::getImage("vortex.jpg");
   
   /* Load font */
   _lpFont = EFontBank::getFont("NITECLUB.TTF",40);
   
   /* Effect */
   _lpAnimEffect = new KCounter();
   
   memset(_szGhost1, 0, 256);
   memset(_szGhost2, 0, 256);
}

/* Destructeur */
Scene_Vortex::~Scene_Vortex()
{
   delete _lpFont;
   delete _lpAnimEffect;
}

void Scene_Vortex::Init()
{  
   /* Sounds & assets */
   if (_strSceneAdditionalName == "egypt") {
      _lpSndEgypt = ESoundBank::getSound("chapitre_egypt");
      strcpy(_szGhost1,"pharaon_standup1");
      strcpy(_szGhost2,"pharaon_standup2");
   }
   else if (_strSceneAdditionalName == "island") {
      _lpSndEgypt = ESoundBank::getSound("chapitre_island");
      strcpy(_szGhost1,"pirate_standup1");
      strcpy(_szGhost2,"pirate_standup2");
   }
   else if (_strSceneAdditionalName == "japan") {
      _lpSndEgypt = ESoundBank::getSound("chapitre_japan");
      strcpy(_szGhost1,"japan_standup1");
      strcpy(_szGhost2,"japan_standup2");
   } else {
      KAssert(false);
   }

   SetVisible(_szGhost1, true);
   SetVisible(_szGhost2, false);

   _lpSndEgypt->playSample();
   _lpAnimEffect->startCounter(1, 2, 0, 10000, K_COUNTER_LINEAR);
}

void Scene_Vortex::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_Vortex::Logic()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   if (FadeOutCompleted()) {
      _lpAnimEffect->move(fElapsed);
      if (GetObjectImageByName(_szGhost2)->isVisible() == false) {
         SetVisible(_szGhost1, false);
         SetVisible(_szGhost2, true);
         _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE);
      }
   }
	EScene::Logic();
}

void Scene_Vortex::Draw()
{
   EScene::Draw();
   
   /* Effet */
   if (FadeOutCompleted()) {
      //_lpBgGraphic->blitAlphaRectFx(0, 0, 1024, 768, 0, 0, 0, _lpAnimEffect->getCurrentValue(), .5, false, false);
   }
   
   if (_strSceneAdditionalName == "egypt") {
      _lpFont->drawStringCentered(KStr("CHAPTER_EGYPT_CAPTION"), 690, 690, int(768/2) - _lpFont->getHeightPix()/2);
   }
   else if (_strSceneAdditionalName == "island") {
      _lpFont->drawStringCentered(KStr("CHAPTER_ISLAND_CAPTION"), 690, 690, int(768/2) - _lpFont->getHeightPix()/2);
   }
   else if (_strSceneAdditionalName == "japan") {
      _lpFont->drawStringCentered(KStr("CHAPTER_JAPAN_CAPTION"), 690, 690, int(768/2) - _lpFont->getHeightPix()/2);
   }
}

void Scene_Vortex::Close()
{
}

bool Scene_Vortex::ObjectClicked(const char *szObjectName, float x, float y)
{
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
   if (_strSceneAdditionalName == "egypt") {
      _lpSceneDirector->GoToScene("egypt_land");
   }
   else if (_strSceneAdditionalName == "island") {
      _lpSceneDirector->GoToScene("island_beach");
   }
   else if (_strSceneAdditionalName == "japan") {
      _lpSceneDirector->GoToScene("japan_mainscreen");
   }
   return true;
}

bool Scene_Vortex::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}
