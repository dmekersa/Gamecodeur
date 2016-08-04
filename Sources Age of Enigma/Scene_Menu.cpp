/*
 *  SceneMenu.cpp
 *  enigma
 *
 *  Created by Rockford on 20/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Menu.h"
#include "ESceneDirector.h"
#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "UserProfileMgt.h"
#include "ThreadLoader.h"
#include "EWatchDog.h"
#include "mmutils.h"

SceneMenu::SceneMenu(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _nCheatCode = 0;
   _nCheatCode2 = 0;

   /* Load font */
   _lpFont = EFontBank::getFont("tahoma.ttf",30);
   _lpFont->setColor(0.92, 0.88, 0.76, 1.0f);
   _bReplyRate = -1;
}

SceneMenu::~SceneMenu()
{
   XDELETE(_lpFont);
}

void SceneMenu::Init()
{
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_MENU);
   SetSmoothOverlap("ashley", true);
   StartAnimation("ashleyanim");
   StartAnimation("zeplight");
   StartAnimation("bg1anim");
   StartAnimation("bg2anim");
   StartAnimation("bg4anim");
   
#ifdef BFG_TEASING
   Loader::LoadScene("teasing");
#endif
   Loader::LoadScene(GetStrGlobal("__lastscene__","parvis"));
   EWatchDog::Enable();
}

void SceneMenu::_myUIMessageHandler (KUIMessage *lpUIMessage) {
   
}

void SceneMenu::Check()
{
   EScene::Check();
#ifndef DEBUG
   if (KInput::isPressed(K_VK_M)) {
      _nCheatCode = 1;
   }
   
   if (KInput::isPressed(K_VK_I) && _nCheatCode == 1) {
      _nCheatCode++;
   }

   if (KInput::isPressed(K_VK_L) && (_nCheatCode == 2 || _nCheatCode == 3)) {
      _nCheatCode++;
   }

   if (KInput::isPressed(K_VK_I) && _nCheatCode == 4) {
      _nCheatCode++;
   }

   if (KInput::isPressed(K_VK_O) && _nCheatCode == 5) {
      _nCheatCode++;
   }

   if (KInput::isPressed(K_VK_N) && _nCheatCode == 6) {
      _nCheatCode++;
   }
#else
   _nCheatCode = 7;
#endif
   if (_nCheatCode >= 7) {
      if (KInput::isPressed(K_VK_L_SHIFT))
      {
         if (KInput::isPressed(K_VK_1)) {
            _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands", "island", false);
            return;
         }
      }
      if (KInput::isPressed(K_VK_L_SHIFT))
      {
         if (KInput::isPressed(K_VK_2)) {
            _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands", "japan", false);
            return;
         }
      }
      if (KInput::isPressed(K_VK_L_SHIFT))
      {
         if (KInput::isPressed(K_VK_3)) {
            _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands", "inca", false);
            return;
         }
      }
      if (KInput::isPressed(K_VK_L_SHIFT))
      {
         if (KInput::isPressed(K_VK_4)) {
            _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands", "middleage", false);
            return;
         }
      }
      if (KInput::isPressed(K_VK_L_SHIFT))
      {
         if (KInput::isPressed(K_VK_5)) {
            _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands", "egypt", false);
            return;
         }
      }
      
      if (KInput::isPressed(K_VK_0))
      {
         _lpSceneDirector->SetHexagramme(false);
         EInventoryItem::CleanUp(NULL);
         EGlobalBank::CleanUp();
         _lpSceneDirector->getDiaryPtr()->Clean();
         _lpSceneDirector->getMap()->Clean();
         _lpSceneDirector->OpenDialogbox("Clean up done");
      }
      if (KInput::isPressed(K_VK_1))
      {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands", "island", false);
      }
      if (KInput::isPressed(K_VK_2))
      {
         AddObjective("house","helpnatsumi");
         AddHint("house","helpnatsumi","where");
         if (!_lpSceneDirector->getInventory()->getItemFromInventory("inv_basementkey")) {
            _lpSceneDirector->getInventory()->AddItem("house","inv_basementkey");
         }
         _lpSceneDirector->GoToScene("corridor");
      }
      if (KInput::isPressed(K_VK_3))
      {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands", "japan", false);
      }
      if (KInput::isPressed(K_VK_4))
      {
         ResolveTask("task_gethexagramme");
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "firstfloor", "house", false);      
      }
      if (KInput::isPressed(K_VK_5))
      {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands", "middleage", false);
      }
      if (KInput::isPressed(K_VK_6))
      {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands", "inca", false);
      }
      if (KInput::isPressed(K_VK_7))
      {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands", "egypt", false);
      }
      if (KInput::isPressed(K_VK_8))
      {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "hell_mainscreen", "", false);
      }
      if (KInput::isPressed(K_VK_9))
      {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_attic", "", false);
      }
      if (KInput::isPressed(K_VK_RIGHT))
      {
         ResolveTask("task_gethexagramme");
         ResolveObjective("house","helpnatsumi");
         ResolveTask("task_living_discover");
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_diningroom", "", false);
      }
      if (KInput::isPressed(K_VK_LEFT))
      {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "debug_david", "", false);
      }
      if (KInput::isPressed(K_VK_UP))
      {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "debug_preload", "", false);
      }
   }   
}

void SceneMenu::Logic()
{
	EScene::Logic();
   
   if (_bReplyRate != -1) {
            if (_bReplyRate == 1) {
               _bReplyRate = -1;
               if (MyGame::getGame()->getKWindow()->toggleFullScreen(false))
               {
                 KMiscTools::launchURL("macappstore://itunes.apple.com/us/app/id478290257?mt=12");
                  _lpSceneDirector->OpenDialogbox("The game is now windowed. Change to Fullscreen in the Option menu.");
               }
            }
   }
   
}

void SceneMenu::Draw()
{
   EScene::Draw();
   
   _lpFont->setBlend(getFade());
   char szWelcome[256];
   memset(szWelcome, 0, 256);
   strcpy(szWelcome, KStr("MENU_WELCOME"));
   strcat(szWelcome, " ");
   strcat(szWelcome, UserProfiles::_pPlayer->GetName());
   _lpFont->drawStringCentered(szWelcome, 666, 666+259, 500);
   
#ifdef TESTENDDATE
   KTextFace *lpFont;
   lpFont = EFontBank::getFont("tahoma.ttf",10);
#ifdef ENGLISH_BUILD
   _lpFont->drawStringCentered("BETA 4 - DO NOT DISCLOSE!", 0, 1024, (768/2));
   lpFont->drawStringCentered("This version, builded on 08/05/2011 was entrusted to you and includes a protection", 0, 1024, (768/2)+40);
#else
   _lpFont->drawStringCentered("BETA 4 - NE PAS DIFFUSER !", 0, 1024, (768/2));
   lpFont->drawStringCentered("Cette version conçue le 05/08/2011 vous a été confiée personnellement et contient un système de protection", 0, 1024, (768/2)+40);
#endif
#endif
}

void SceneMenu::Close()
{
}

bool SceneMenu::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Cheat code Secret bonus game

   if (strcmp(szObjectName, "but_cheat1") == 0)
   {
      _nCheatCode2 = 1;
   }
   if (strcmp(szObjectName, "but_cheat2") == 0)
   {
      _nCheatCode2++;
   }
   if (strcmp(szObjectName, "but_cheat3") == 0)
   {
      _nCheatCode2++;
   }
   if (strcmp(szObjectName, "but_cheat4") == 0)
   {
      _nCheatCode2++;
      if (_nCheatCode2 == 4) {
         _lpSceneDirector->OpenDialogbox("Hidden!");
      }
   }
   
   if (strcmp(szObjectName, "but_options") == 0)
   {
      _lpSceneDirector->_bOptionsRequest = true;
   }

   if (strcmp(szObjectName, "but_player") == 0)
   {
      _lpSceneDirector->_bPlayerRequest = true;
   }

   if (strcmp(szObjectName, "but_exit") == 0)
   {
#ifdef BFG_TEASING
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "teasing", "", false);
#else
	   MyGame::getGame()->getKWindow()->terminate();
#endif
   }

   if (strcmp(szObjectName, "but_mg") == 0)
   {
      _lpSceneDirector->GoToScene("minigames");
      return true;
   }

   if (strcmp(szObjectName, "but_facebook") == 0)
   {
      if (MyGame::getGame()->getKWindow()->toggleFullScreen(false))
      {
         KMiscTools::launchURL("http://www.facebook.com/pages/Age-of-Enigma-Mac-App-Store-Edition/310642292285286");
         _lpSceneDirector->OpenDialogbox("The game is now windowed. Change to Fullscreen in the Option menu.");
      }
      return true;
   }

   if (strcmp(szObjectName, "but_twitter") == 0)
   {
      if (MyGame::getGame()->getKWindow()->toggleFullScreen(false))
      {
         KMiscTools::launchURL("http://twitter.com/?status=I%20just%20played%20Age%20of%20Enigma%20Special%20Edition,%20an%20Adventure%20game%20for%20Mac:%20http://bit.ly/s2SMxp");
         _lpSceneDirector->OpenDialogbox("The game is now windowed. Change to Fullscreen in the Option menu.");
      }
      return true;
   }

   if (strcmp(szObjectName, "but_itunes") == 0)
   {
      if (MyGame::getGame()->getKWindow()->toggleFullScreen(false))
      {
         KMiscTools::launchURL("http://itunes.apple.com/us/album/age-enigma-the-secret-sixth/id458605930");
         _lpSceneDirector->OpenDialogbox("The game is now windowed. Change to Fullscreen in the Option menu.");
      }
      return true;
   }

   if (strcmp(szObjectName, "but_geisha") == 0)
   {
      if (MyGame::getGame()->getKWindow()->toggleFullScreen(false))
      {
         char szLang[256];
         GetSystemLocale(szLang);
         if (strcmp(szLang, "fr") == 0) {
            KMiscTools::launchURL("macappstore://itunes.apple.com/us/app/id449451286?mt=12");
         }
         else
         {
            KMiscTools::launchURL("macappstore://itunes.apple.com/us/app/id415546622?mt=12");
         }
         _lpSceneDirector->OpenDialogbox("The game is now windowed. Change to Fullscreen in the Option menu.");
      }
      return true;
   }

   if (strcmp(szObjectName, "but_rate") == 0)
   {
      _bReplyRate = false;
      _lpSceneDirector->OpenDialogboxYN("Help us to create the sequel. Rate the game 5 stars. Do you want to rate the game now?", &_bReplyRate);
      return true;
   }
   
   if (strcmp(szObjectName, "but_play") == 0 || strcmp(szObjectName, "menu_house") == 0)
   {
      if (_State == SCENE_STABLE) {
         // :dmekersa:20110321 Joue la vidéo au 1er clic sur Play
         if (!TaskResolved("__mode_choice__")) {
            _lpSceneDirector->setZoomin(false);
            _lpSceneDirector->GoToScene("mode","", false);
         }
         else {
            SetVisible("barremenu", false);
            _lpSceneDirector->GoToScene(GetStrGlobal("__lastscene__","parvis"),GetStrGlobal("__lastadditionalname__",""));
         }
         return true;
      }
   }

   if (strcmp(szObjectName, "but_credits") == 0)
   {
      _lpSceneDirector->getSequencer()->GoToScene("credits", "vignette_cinematic", "credits", false);
   }
   return false;
}

bool SceneMenu::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}
