/*
 *  Scene_Video_Player.cpp
 *  enigma
 *
 *  Created by Rockford on 03/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Video_Player.h"
#include "ESceneDirector.h"
#include "EPointer.h"
#include "FontBank.h"
#include "MusicBank.h"
#include "ImageBank.h"

/* Constructeur */
Scene_Video_Player::Scene_Video_Player(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   /* Open video */
   _lpVideo = NULL;
   _lpSpeech = NULL;
   
   _bEnd = false;
   _lpFont = EFontBank::getFont("tahoma.ttf", 18);

   for (int i=0; i<=10; i++) {
      _bSubtitleDone[i] = false;
   }
}

/* Destructeur */
Scene_Video_Player::~Scene_Video_Player()
{
   if (_lpSpeech)
      _lpSpeech->stopStream();
   XDELETE(_lpBgGraphic);
   if (_lpVideo != NULL)
      EImageBank::VideoStop(_lpVideo);
   XDELETE(_lpFont);
}

void Scene_Video_Player::Init()
{
   if (_lpSceneDirector->isLockMode()) {
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, false);
   }
   
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE);
   if (getAdditionalName() == "intro") {
      _lpVideo = EImageBank::getVideo("intro.ogv",false,false,false);
     
      // Speech
      _lpSpeech = EMusicBank::getMusic("intro_speech1");
      _lpSpeech->playStream(false);
   }
   else if (getAdditionalName() == "soul_tunnel_island"
            || getAdditionalName() == "soul_tunnel_inca"
            || getAdditionalName() == "soul_tunnel_japan"
            || getAdditionalName() == "soul_tunnel_middleage"
            || getAdditionalName() == "soul_tunnel_egypt"
            || getAdditionalName() == "soul_tunnel_hell"
            || getAdditionalName() == "soul_tunnel_backisland"
            || getAdditionalName() == "soul_tunnel_backinca"
            || getAdditionalName() == "soul_tunnel_backjapan"
            || getAdditionalName() == "soul_tunnel_backmiddleage"
            || getAdditionalName() == "soul_tunnel_backegypt"
            || getAdditionalName() == "soul_tunnel_backhell"
            ) {

      _lpVideo = EImageBank::getVideo("soul_tunnel.ogv",false,false,false);

      // Speech
      _lpSpeech = EMusicBank::getMusic("tunnel");
      _lpSpeech->playStream(false);
   }
   else if (getAdditionalName() == "styx") {
      _lpVideo = EImageBank::getVideo("hell_styx1.ogv",false,false,false);
     
      // Speech
      _lpSpeech = EMusicBank::getMusic("crossstyx");
      _lpSpeech->playStream(false);
   }
   else if (getAdditionalName() == "theend") {
      _lpVideo = EImageBank::getVideo("theend.ogv",false,false,false);
      
      // Speech
      _lpSpeech = EMusicBank::getMusic("theend");
      _lpSpeech->playStream(false);
   }
   else if (getAdditionalName() == "dream") {
      _lpVideo = EImageBank::getVideo("dream.ogv",false,false,false);
      
      // Speech
      _lpSpeech = EMusicBank::getMusic("dream_sd");
      _lpSpeech->playStream(false);
#ifdef DEBUG
   // defaut pour la scene de test
   } else {
      _lpVideo = EImageBank::getVideo("dream.ogv",false,false,false);
      
      // Speech
      _lpSpeech = EMusicBank::getMusic("dream_sd");
      _lpSpeech->playStream(false);
#endif
   }
   KAssert(_lpVideo != NULL);
   if (_lpVideo != NULL)
      EImageBank::VideoPlay(_lpVideo);
}

void Scene_Video_Player::Check()
{
   EScene::Check();
   if (_lpVideo->getTime()>=8) {
      if (Mouse.LeftEvent()) {
         EndScene();
      }
   }
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_Video_Player::EndScene()
{
   if (_bEnd) {
      return;
   }
   _bEnd = true;
   if (_strSceneAdditionalName == "intro") {
      // :dmekersa:20110321 Après l'intro, on joue la scène d'introduction aux abords de la maison
      _lpSceneDirector->GoToScene("house_out");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_hell") {
      _lpSceneDirector->GoToScene("hell_mainscreen");
   }
   else if (_strSceneAdditionalName == "styx") {
      _lpSceneDirector->GoToScene("hell_door");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_island") {
      _lpSceneDirector->GoToScene("island_beach","arrival");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_inca") {
      _lpSceneDirector->GoToScene("inca_mainscreen","arrival");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_japan") {
      _lpSceneDirector->GoToScene("japan_mainscreen","arrival");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_middleage") {
      _lpSceneDirector->GoToScene("middleage_mainscreen","arrival");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_egypt") {
      _lpSceneDirector->GoToScene("egypt_land","arrival");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_backisland") {
      _lpSceneDirector->GoToScene("house_diningroom","backfrombeyond");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_backinca") {
      _lpSceneDirector->GoToScene("house_livingroom","backfrombeyond");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_backjapan") {
      _lpSceneDirector->GoToScene("basement","backfrombeyond");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_backmiddleage") {
      _lpSceneDirector->GoToScene("house_room2","backfrombeyond");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_backegypt") {
      _lpSceneDirector->GoToScene("bathroom","backfrombeyond");
   }
   else if (_strSceneAdditionalName == "soul_tunnel_backhell") {
      _lpSceneDirector->GoToScene("house_attic","backfrombeyond");
   }
   else if (_strSceneAdditionalName == "theend") {
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_cinematic", "credits", false);
   }
   else if (_strSceneAdditionalName == "dream") {
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "title", "", false);
   }
   else {
      _lpSceneDirector->GoToScene("menu");
   }
   Mouse.ReleaseEvent();
}

void Scene_Video_Player::Logic()
{
	EScene::Logic();
}

void Scene_Video_Player::Draw()
{
//   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   EScene::Draw();
   
   KGame::getGame()->_lpKWindow->setWorldView(0,0,0,1,true);
   float fFade = getFade();
   
   /* Update video */
   _lpVideo->readFrame ();
   
   if (_lpVideo->isVideoAvailable ()) {
      /* Draw video */
      if (_strSceneAdditionalName == "dream") {
         _lpVideo->getKGraphic()->stretchAlphaRect (0, 0, _lpVideo->getFrameWidth(), _lpVideo->getFrameHeight(), 0, (768-536)/2, 1024, ((768-536)/2)+536, fFade, 0.0f, false, false);
      }
      else if (_strSceneAdditionalName == "theend") {
         _lpVideo->getKGraphic()->stretchAlphaRect (0, 0, _lpVideo->getFrameWidth(), _lpVideo->getFrameHeight(), 0, (768-436)/2, 1024, ((768-536)/2)+436, fFade, 0.0f, false, false);
      }
      else if (_strSceneAdditionalName == "styx") {
         _lpVideo->getKGraphic()->stretchAlphaRect (0, 0, _lpVideo->getFrameWidth(), _lpVideo->getFrameHeight(), 0, 0, 1024, 768, fFade, 0.0f, false, false);
      }
      else {
         _lpVideo->getKGraphic()->stretchAlphaRect (0, 0, _lpVideo->getFrameWidth(), _lpVideo->getFrameHeight(), 0, 75, 1024, 75+589, fFade, 0.0f, false, false);
      }
   }

   if (_lpVideo->getTime()>=8) {
      _lpFont->setColor(1.0f, 1.0f, 1.0f, fFade);
      DrawFancyString(KStr("CINEMATIC_CLOSETXT"), _lpFont, 1024/2, 720, 1024, true, getFade());
   }

   if (!_bEnd) {
      if (_strSceneAdditionalName == "theend") {
         // Sous titres
         if (_lpVideo->getTime()>=11 && _bSubtitleDone[0] == false) {
            _bSubtitleDone[0] = true;
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("THEEND_SPEECH_1"), "");
         }
         if (_lpVideo->getTime()>=20 && _bSubtitleDone[1] == false) {
            _bSubtitleDone[1] = true;
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("THEEND_SPEECH_2"), "");
         }
         if (_lpVideo->getTime()>=24 && _bSubtitleDone[2] == false) {
            _bSubtitleDone[2] = true;
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("THEEND_SPEECH_3"), "");
         }
         if (_lpVideo->getTime()>=27 && _bSubtitleDone[3] == false) {
            _bSubtitleDone[3] = true;
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("THEEND_SPEECH_4"), "");
         }
         if (_lpVideo->getTime()>=32 && _bSubtitleDone[4] == false) {
            _bSubtitleDone[4] = true;
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("THEEND_SPEECH_5"), "");
         }
         if (_lpVideo->getTime()>=36 && _bSubtitleDone[5] == false) {
            _bSubtitleDone[5] = true;
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("THEEND_SPEECH_6"), "");
         }
         if (_lpVideo->getTime()>=41 && _bSubtitleDone[6] == false) {
            _bSubtitleDone[6] = true;
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("THEEND_SPEECH_7"), "");
         }
      }         
      if (_strSceneAdditionalName == "dream") {
         // Sous titres
         if (_lpVideo->getTime()>=0 && _bSubtitleDone[0] == false) {
            _bSubtitleDone[0] = true;
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("INTRO_SPEECH_1"), "");
         }
         if (_lpVideo->getTime()>=2.5 && _bSubtitleDone[1] == false) {
            _bSubtitleDone[1] = true;
            _lpSceneDirector->getSequencer()->Reset();
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("INTRO_SPEECH_2"), "");
         }
         if (_lpVideo->getTime()>=22 && _bSubtitleDone[2] == false) {
            _bSubtitleDone[2] = true;
            _lpSceneDirector->getSequencer()->Reset();
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("INTRO_SPEECH_3"), "");
         }
         if (_lpVideo->getTime()>=28 && _bSubtitleDone[3] == false) {
            _bSubtitleDone[3] = true;
            _lpSceneDirector->getSequencer()->Reset();
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("INTRO_SPEECH_5"), "");
         }
         if (_lpVideo->getTime()>=35 && _bSubtitleDone[4] == false) {
            _bSubtitleDone[4] = true;
            _lpSceneDirector->getSequencer()->Reset();
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("INTRO_SPEECH_6"), "");
         }
         if (_lpVideo->getTime()>=43 && _bSubtitleDone[5] == false) {
            _bSubtitleDone[5] = true;
            _lpSceneDirector->getSequencer()->Reset();
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("INTRO_SPEECH_7"), "");
         }
         if (_lpVideo->getTime()>=51 && _bSubtitleDone[6] == false) {
            _bSubtitleDone[6] = true;
            _lpSceneDirector->getSequencer()->Reset();
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("INTRO_SPEECH_8"), "");
         }
         if (_lpVideo->getTime()>=56 && _bSubtitleDone[7] == false) {
            _bSubtitleDone[7] = true;
            _lpSceneDirector->getSequencer()->Reset();
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("INTRO_SPEECH_9"), "");
         }
         if (_lpVideo->getTime()>=59 && _bSubtitleDone[8] == false) {
            _bSubtitleDone[8] = true;
            _lpSceneDirector->getSequencer()->Reset();
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("INTRO_SPEECH_10"), "");
         }
         if (_lpVideo->getTime()>=65 && _bSubtitleDone[9] == false) {
            _bSubtitleDone[9] = true;
            _lpSceneDirector->getSequencer()->Reset();
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("INTRO_SPEECH_11"), "");
         }
      }
   }
   
   if (_lpSpeech) {
      if (_lpVideo->isEndReached () && (!_lpSpeech->isPlaying())) {
         EndScene();
         return;
      }
   }
   else {
      if (_lpVideo->isEndReached ()) {
         EndScene();
         return;
      }
   }
   
}

void Scene_Video_Player::Close()
{
}

bool Scene_Video_Player::ObjectClicked(const char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Video_Player::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}
