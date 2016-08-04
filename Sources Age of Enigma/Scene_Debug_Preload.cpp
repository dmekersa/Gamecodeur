/*
 *  Scene_Debug_Preload.cpp
 *  enigma
 *
 *  Created by Rockford on 14/06/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */
#include "ESequenceBase.h"

#include "MyGame.h"
#include "EScene.h"
#include "ESceneDirector.h"
#include "ESceneObject.h"
#include "GlobalBank.h"
#include "EWatchDog.h"
#include "ThreadLoader.h"
#include "ESceneSequencer.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EmitterBank.h"

#include "Scene_Debug_Preload.h"

std::vector< const char* > scenesNames;

/* Constructeur */
Scene_Debug_Preload::Scene_Debug_Preload(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   EWatchDog::Disable();

   _lpMiniJeu = NULL;
   _lpScene = NULL;
   _bTestScene = false;
   _bTestMiniJeu = false;
   _szCurrentScene = "";
   _nSceneIndex = 0;
   _numFrame = (int) GetRandom(0,120);
   _bRandomizeNextScene = false;
   
   scenesNames.push_back("menu");
   scenesNames.push_back("mode");
   //scenesNames.push_back("loading");
   scenesNames.push_back("title");
#ifdef DEBUG
   scenesNames.push_back("video_player");
#endif
   scenesNames.push_back("title_splashscreen");
   //scenesNames.push_back("pirate");   // scene test
   scenesNames.push_back("island_dive");
   scenesNames.push_back("house_out");
   scenesNames.push_back("house_out_car");
   scenesNames.push_back("parvis");
   scenesNames.push_back("hall");
   scenesNames.push_back("house_hall");
   scenesNames.push_back("halldoor");
   scenesNames.push_back("corridor");
   scenesNames.push_back("keyhole");
   scenesNames.push_back("stairsdown");
   scenesNames.push_back("house_livingroom");
   scenesNames.push_back("house_livingroom_table");
   scenesNames.push_back("house_livingroom_box");
   scenesNames.push_back("house_livingroom_piano");
   scenesNames.push_back("house_incabook");
   scenesNames.push_back("house_diningroom");
   scenesNames.push_back("closet");
   scenesNames.push_back("kitchen");
   scenesNames.push_back("basement");
   scenesNames.push_back("basement_grid");
   scenesNames.push_back("firstfloor");
   scenesNames.push_back("room1");
   scenesNames.push_back("house_room2");
   scenesNames.push_back("house_room2_bedside");
   scenesNames.push_back("house_room2_mirror");
   scenesNames.push_back("bathroom");
   scenesNames.push_back("bathroom_tub");
   scenesNames.push_back("bathroom_cabinet");
   scenesNames.push_back("house_atticdoor");
   scenesNames.push_back("house_attic");
   scenesNames.push_back("house_attic_pentacle");
   scenesNames.push_back("island_beach");
   //scenesNames.push_back("island_falaise"); //scene supprimé
   scenesNames.push_back("island_cave");
   scenesNames.push_back("island_creek");
   scenesNames.push_back("vignette_island_safe");
   scenesNames.push_back("japan_mainscreen");
   scenesNames.push_back("japan_garden");
   scenesNames.push_back("japan_bedroom");
   scenesNames.push_back("inca_mainscreen");
   scenesNames.push_back("inca_passage");
   scenesNames.push_back("inca_stairs");
   scenesNames.push_back("inca_final");
   scenesNames.push_back("inca_autel");
   scenesNames.push_back("vignette_inca_statuette");
   scenesNames.push_back("middleage_mainscreen");
   scenesNames.push_back("middleage_cloister");
   scenesNames.push_back("middleage_stairs");
   scenesNames.push_back("middleage_jail");
   scenesNames.push_back("egypt_land");
   scenesNames.push_back("egypt_shop");
   scenesNames.push_back("egypt_mazeentry");
   scenesNames.push_back("egypt_keyroom");
   scenesNames.push_back("egypt_keyroom_zoom");
   scenesNames.push_back("egypt_maze");
   scenesNames.push_back("egypt_boat");
   scenesNames.push_back("egypt_boatroom");
   scenesNames.push_back("egypt_tomb");
   scenesNames.push_back("egypt_tomb_connect");
   scenesNames.push_back("hell_mainscreen");
   scenesNames.push_back("hell_vale");
   scenesNames.push_back("hell_styx");
   scenesNames.push_back("hell_door");
   scenesNames.push_back("vignette_spirit");
   scenesNames.push_back("vignette_diningroom_clock");
   scenesNames.push_back("vignette_diningroom_boat");
   scenesNames.push_back("vignette_diningroom_book");
   scenesNames.push_back("vignette_diningroom_lib");
   scenesNames.push_back("vignette_diningroom_picture");
   scenesNames.push_back("vignette_cinematic");
   //scenesNames.push_back("vignette_hands");
   scenesNames.push_back("minigames");
}

/* Destructeur */
Scene_Debug_Preload::~Scene_Debug_Preload()
{
   if (_bTestScene || _bTestMiniJeu)
      K_LOG("SCENE_DEBUG_PRELOAD: fin des tests");

   if (_lpScene != NULL) {
      SCENEDIRECTOR->getSequencer()->Reset();
      _lpScene->Close();
      XDELETE(_lpScene);
   }
   if (_lpMiniJeu != NULL)
      XDELETE(_lpMiniJeu);
   XDELETE(_lpBgGraphic);

   Loader::ActiveScenePreload();
   EWatchDog::Enable();
}

void Scene_Debug_Preload::Init()
{
}

void Scene_Debug_Preload::Callback(const char *szParam)
{
}

void Scene_Debug_Preload::Check()
{
   EScene::Check();
   if (KInput::isPressed(K_VK_F4) || KInput::isPressed(K_VK_ESCAPE) )
   {
      _bTestScene = true;
      SCENEDIRECTOR->GoToScene("menu");
      return;
   }
   if (KInput::isPressed(K_VK_0))
   {
      _numFrame = 0;
      return;
   }
   if (KInput::isPressed(K_VK_1))
   {
      _bRandomizeNextScene = true;
      return;
   }
   if (KInput::isPressed(K_VK_2))
   {
      _bRandomizeNextScene = false;
      return;
   }
   if (!_bTestScene && !_bTestMiniJeu) {
      if (KInput::isPressed(K_VK_F5))
      {
         _bTestScene = true;
         Loader::ActiveScenePreload(false);
         K_LOG("SCENE_DEBUG_PRELOAD: test de chargement de scene lancer, preload desactivé");
         return;
      }
      if (KInput::isPressed(K_VK_F6))
      {
         _bTestScene = true;
         K_LOG("SCENE_DEBUG_PRELOAD: test de chargement de scene lancer");
         return;
      }
      if (KInput::isPressed(K_VK_F7))
      {
         K_LOG("SCENE_DEBUG_PRELOAD: test de chargement de minijeu lancer");
         _bTestMiniJeu = true;
      }
   }
}

void Scene_Debug_Preload::Logic()
{
	EScene::Logic();
   if (_lpScene != NULL) {
      cleanGotoScene();        
      _lpScene->Logic();
      cleanGotoScene();        
   }
}
void Scene_Debug_Preload::GenNextSceneIndex() {
   if (_bRandomizeNextScene)
      _nSceneIndex = (int)getRandom(0,scenesNames.size()) % scenesNames.size();
   else
      _nSceneIndex = (_nSceneIndex+1) % scenesNames.size();
}
void Scene_Debug_Preload::cleanGotoScene() {
#ifdef DEBUG    // evite l'appel a des fonctions de ESceneSequencer spécifique ou mode debug
   ESequenceContainer *o = NULL;
   ESequenceContainer *otodelete = NULL;
   bool bsuccess = ESequenceContainer::g_ListSequence.getHead(o);
   while (bsuccess) {
      otodelete = o;   
      bsuccess = ESequenceContainer::g_ListSequence.getNext(o);
      if (strcmp(o->_strSceneName.c_str(),_szCurrentScene)) {
         if (o->_lpSequenceGotoScene == NULL)
            otodelete = NULL;
      }
      if (otodelete != NULL) {
         ESequenceContainer::g_ListSequence.remove(otodelete);
         if (otodelete ==    SCENEDIRECTOR->getSequencer()->GetCurrentSequence()) 
               SCENEDIRECTOR->getSequencer()->DeleteCurrentSequence();
         else 
            XDELETE(otodelete);
      }
   }
#endif
}
void Scene_Debug_Preload::Draw()
{
   EScene::Draw();
   if (_bTestScene) {
      if (_lpScene != NULL) {
         cleanGotoScene();        
         _lpScene->Draw();
         if (_lpScene->GetSceneState() == SCENE_SHOWIN || _numFrame> 0) {
            _numFrame--;
         } else if (_lpScene->GetSceneState() == SCENE_STABLE) {
            _lpScene->StartFade(SCENE_SHOWOUT);
         } else if (_lpScene->GetSceneState() == SCENE_SHOWOUT && _lpScene->FadeOutCompleted()) {
            _lpScene->Close();
            XDELETE(_lpScene);
            _numFrame = (int) GetRandom(30,120);
         }
         cleanGotoScene();        
      } else {
         SCENEDIRECTOR->ChangeCharacter(DIRECTOR_CHARACTER_MIDDLEAGE);
         if ((rand()%2) == 0) {
            K_LOG("SCENE_DEBUG_PRELOAD: changement de scene schedulé");
            Loader::IncrIndiceScene();
         }
         //const char *tmp = scenesNames[rand()%scenesNames.size()];
         EObjectContainer::CleanUp(_szCurrentScene);
         SCENEDIRECTOR->getSequencer()->Reset();
         GenNextSceneIndex();
         _szCurrentScene = scenesNames[_nSceneIndex];
         K_LOG("SCENE_DEBUG_PRELOAD: chargement de la scene: %s",_szCurrentScene);
         SCENEDIRECTOR->CreateSceneFromName(_szCurrentScene,_lpScene);
         cleanGotoScene();        
         _lpScene->StartFade(SCENE_SHOWIN);
         //_nSceneIndex = (_nSceneIndex+1) % scenesNames.size();
         EImageBank::PurgeCache();
         ESoundBank::PurgeCache();
         EEmitterBank::PurgeCache();
      }
   } else if (_bTestMiniJeu) {
   }
}

void Scene_Debug_Preload::Close()
{
   EObjectContainer::CleanUp(_szCurrentScene);
   if (_lpScene != NULL) {
      SCENEDIRECTOR->getSequencer()->Reset();
      _lpScene->Close();
      XDELETE(_lpScene);
   }
   if (_lpMiniJeu != NULL)
      XDELETE(_lpMiniJeu);
}

bool Scene_Debug_Preload::ObjectClicked(const char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Debug_Preload::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}