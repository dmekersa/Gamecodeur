/*
 *  Scene_Inca_Mainscreen.cpp
 *  enigma
 *
 *  Created by MEKERSA David on 04/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Inca_Mainscreen.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "MusicBank.h"
#include "MyGame.h"
#include "EMiniJeuCubes.h"

/* Constructeur */
Scene_Inca_Mainscreen::Scene_Inca_Mainscreen(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   
   _bAnimDoor = false;
   _lpAnimDoor = new KCounter();
   
   EMiniJeuCubes::Preload();
}

/* Destructeur */
Scene_Inca_Mainscreen::~Scene_Inca_Mainscreen()
{
   StopAnimation("bg2_anim");
   XDELETE(_lpBgGraphic);
   XDELETE(_lpAnimDoor);
}

void Scene_Inca_Mainscreen::StartEffect(const char *szEffect)
{
   StartAnimation("bg2_anim");
   if (strcmp(szEffect, "raise_door") == 0) {
      _bAnimDoor = true;
      int x, y;
      GetObjectPosition("inca_main_door", x, y, false, false);
      ESoundBank::getSound("rockdoor")->playSample();
      _lpAnimDoor->startCounter(0.0f, -210.0f, 0, 10000, K_COUNTER_EASEOUT);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
   }
}

void Scene_Inca_Mainscreen::Init()
{
   StartAnimation("bg2_anim");
   StartAnimation("clouds_anim");

   if (!TaskResolved("inca_ashleytalkintro")) {
      ResolveTask("inca_ashleytalkintro");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_DIALOGBOX_INTRO1"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_DIALOGBOX_INTRO2"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_DIALOGBOX_INTRO3"), "", false, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      AddTask("task_inca_findspear");
      AddObjective("inca","spear");
      AddHint("inca","spear","where");
   }
   
   // Ajout de la statuette à l'inventaire si elle n'y est pas déjà
   if (_lpSceneDirector->getInventory()->getItemFromInventory("inv_inca_statuette") == NULL) {
      _lpSceneDirector->getInventory()->AddItem("inca","inv_inca_statuette");
   }
   
   // Morceaux de lance
   SetupItem("inca_spear[1]");
   SetupItem("inca_spear[2]");
   SetupItem("inca_spear[3]");
   SetupItem("inca_spear[4]");   
   SetupItem("inca_spear[5]");
   if (TaskResolved("task_inca_opencube")) {
      SetupItem("inca_spear[6]");
   }
   
   // Bloc de pierre
   if (TaskResolved("task_inca_opencube")) {
      SetVisible("inca_main_cubesclosed", false, true);
      SetVisible("inca_main_cubesopen", true, true);
   }
   else {
      SetVisible("inca_main_cubesclosed", true, true);
      SetVisible("inca_main_cubesopen", false, true);
   }
   
   // Le garde
   // Il n'a pas encore sa lance
   if (!TaskResolved("task_inca_givespear")) {
      SetVisible("inca_main_guard_nospear", true);
   }
   // Il a eu sa lance
   else {
      SetVisible("inca_main_guard_spear", true);
   }
   
   // La porte est ouverte
   if (TaskResolved("task_inca_opendoor")) {
      SetVisible("inca_main_door", false, false);
   }
}

void Scene_Inca_Mainscreen::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      _lpSceneDirector->GoToScene("inca_final");
   }
#endif
}

void Scene_Inca_Mainscreen::Logic()
{
	EScene::Logic();
}

void Scene_Inca_Mainscreen::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   if (_bAnimDoor) {
      _lpAnimDoor->move(fElapsed);
      // Déplace la porte
      RestorePosition("inca_main_door");
      MovePosition("inca_main_door", GetRandom(-1.0, 1.0), _lpAnimDoor->getCurrentValue());
      if (_lpAnimDoor->isCompleted()) {
         _bAnimDoor = false;
         ResolveTask("task_inca_opendoor");
         AddObjective("inca","4artefacts");
         AddHint("inca","4artefacts","where");
         AddHint("inca","4artefacts","light");
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      }
   }
   
   EScene::Draw();
}

void Scene_Inca_Mainscreen::Close()
{
}

bool Scene_Inca_Mainscreen::ObjectClicked(const char *szObjectName, float x, float y)
{
   /* Morceaux de lance */
   if ( strncmp(szObjectName, "inca_spear", strlen("inca_spear")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_inca_spearbroken",6);
   }
   
   /* Garde sans sa lance */
   if (strcmp(szObjectName, "inca_main_guard_nospear") == 0) {
      int x,y;
      GetObjectPosition("inca_main_guard_nospear", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("INCA_GUARD_NEEDSPEAR"), "", true);
   }
   /* Garde avec sa lance */
   if (strcmp(szObjectName, "inca_main_guard_spear") == 0) {
      int x,y;
      GetObjectPosition("inca_main_guard_spear", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("INCA_GUARD_RITUAL6"), "", true);
   }
   
   /* Mini jeu cubes */
   if (strcmp(szObjectName, "inca_main_cubesclosed") == 0) {
      if (!TaskResolved("task_inca_opencube")) {
         EMiniJeuBase *lpMiniJeu = new EMiniJeuCubes();
         _lpSceneDirector->GoToMiniGame("cubes", lpMiniJeu);
         return true;
      }
   }
   
   return false;
}

bool Scene_Inca_Mainscreen::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Inca_Mainscreen::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur donne la lance réparée au garde
   if (strcmp(szItemName, "inv_inca_spearrepaired") == 0) {
      if (strcmp(szObjectName, "inca_main_guard_nospear") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_inca_givespear");
         int x,y;
         GetObjectPosition("inca_main_guard_spear", x, y, true, false);
         // Le garde reçoit la lance
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "inca_main_guard_nospear", "inca_main_guard_spear");
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_GUARD_RITUAL1"), "", false);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "inca_main_guard_spear", "inca_main_guard_prey");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_GUARD_RITUAL2"), "", false);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "inca_main_guard_prey", "inca_main_guard_spear");
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_GUARD_RITUAL3"), "", false);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_GUARD_RITUAL4"), "", false);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_GUARD_RITUAL5"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_GUARD_RITUAL6"), "", false);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_HOWENTER"), "", false, true);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_GUARD_RITUAL7"), "", false);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_GUARD_RITUAL8"), "", false);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         AddTask("task_inca_opendoor");
         AddTask("task_inca_getartefacts");
         ResolveObjective("inca","spear");
         AddObjective("inca","enter");
         AddHint("inca","enter","how");
         return true;
      }
   }

   // Le joueur donne la lance réparée au garde
   if (strcmp(szItemName, "inv_inca_spearbroken") == 0) {
      if (strcmp(szObjectName, "inca_main_guard_nospear") == 0) {
         int x,y;
         GetObjectPosition("inca_main_guard_spear", x, y, true, false);
         // Le garde n'en veut pas
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_GUARD_SPEARBROKEN"), "", false);
         return true;
      }
   }

   // Le joueur utilise la statuette
   if (strcmp(szItemName, "inv_inca_statuette") == 0) {
      if (strcmp(szObjectName, "inca_main_door") == 0) {
         if (!TaskResolved("task_inca_givespear")) {
            int x,y;
            GetObjectPosition("inca_main_guard_nospear", x, y, true, false);
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_GUARD_REFUSE"), "", true);
         }
         else {
            // Ouverture de la vignette
            ESoundBank::getSound("success")->playSample();
            _lpSceneDirector->GoToVignette("vignette_inca_statuette", szObjectName, true);
         }
         return true;
      }
   }
   
   // Le joueur utilise un effet de la statuette
   if (strcmp(szItemName, "statuette_effect_raise") == 0) {
      if (strcmp(szObjectName, "inca_main_door") == 0) {
         // Lancement de l'effet
         StartEffect("raise_door");
         return true;
      }
   }

   char *effectprefix = "statuette_effect_";
   if (strncmp(szItemName, effectprefix,strlen(effectprefix)) == 0) {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_MAIN_YUMANCO_STATUETTE_NOEFFECT"), "", true, false);
		 return true;
   }
   return false;
}

void Scene_Inca_Mainscreen::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "cubes") == 0 && bIsRevolved) {
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      ResolveTask("task_inca_opencube");
      ESoundBank::getSound("reveal")->playSample();
      SetupItem("inca_spear[6]");
      SetVisible("inca_main_cubesclosed", false);
      SetVisible("inca_main_cubesopen", true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_MAIN_YUMANCO_MGDONE"), "", true, false);
   }
   
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
