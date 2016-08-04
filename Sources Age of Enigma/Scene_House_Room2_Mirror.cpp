/*
 *  Scene_House_Room2_Mirror.cpp
 *  enigma
 *
 *  Created by Rockford on 22/10/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "SoundBank.h"
#include "Scene_House_Room2_Mirror.h"
#include "ESceneDirector.h"
#include "EMiniJeuJigsawBook.h"
#include "EMiniJeuMirorBeam.h"

/* Constructeur */
Scene_House_Room2_Mirror::Scene_House_Room2_Mirror(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   EMiniJeuJigsawBook::Preload();
   EMiniJeuMirorBeam::Preload();
}

/* Destructeur */
Scene_House_Room2_Mirror::~Scene_House_Room2_Mirror()
{
}

void Scene_House_Room2_Mirror::Init()
{
   SetupItem("house_room2_frag01");
   SetupItem("house_room2_frag02");
   SetupItem("house_room2_frag03");
   SetupItem("house_room2_frag04");

   // Miroir réparé
   SetVisible("house_room2_repaired_frag00", TestGlobal("task_house_room2_fragment00") == true);
   SetVisible("house_room2_repaired_frag01", TestGlobal("task_house_room2_fragment01") == true);
   SetVisible("house_room2_repaired_frag02", TestGlobal("task_house_room2_fragment02") == true);
   SetVisible("house_room2_repaired_frag03", TestGlobal("task_house_room2_fragment03") == true);
   SetVisible("house_room2_repaired_frag04", TestGlobal("task_house_room2_fragment04") == true);
   SetVisible("house_room2_repaired_frag05", TestGlobal("task_house_room2_fragment05") == true);
   SetVisible("house_room2_repaired_frag06", TestGlobal("task_house_room2_fragment06") == true);
   SetVisible("house_room2_repaired_frag07", TestGlobal("task_house_room2_fragment07") == true);
   SetVisible("house_room2_repaired_frag08", TestGlobal("task_house_room2_fragment08") == true);
   
   // 1ère visite
   if (!TaskResolved("task_house_room2_mirrorvisit")) {
      ResolveTask("task_house_room2_mirrorvisit");
      AddObjective("house","repairmirror");
      AddHint("house","repairmirror","where");
      AddHint("house","repairmirror","how");
   }
   
   CheckMirrorFull();

   // Il faut chasser le polt
   if (TaskToResolve("task_house_room2_expelevil")) {
      SetVisible("house_room2mirror_ghost", true);
      ESoundBank::getSound("poltergeistgone")->playSample();
//      _lpSceneDirector->getSequencer()->Talk("demoninmirror", CHARACTER_POSX, CHARACTER_POSY, KStr("ROOM2_ASHLEY_DEMONINMIRROR"), "", true);
   }
   else {
      SetVisible("house_room2mirror_ghost", false);
   }
   
   // Le miroir est réparé
   if (TaskResolved("task_house_room2_repairmirror")) {
      SetVisible("house_room2mirror_dropzone_00", false, true);
      SetVisible("house_room2mirror_dropzone_01", false, true);
      SetVisible("house_room2mirror_dropzone_02", false, true);
      SetVisible("house_room2mirror_dropzone_03", false, true);
      SetVisible("house_room2mirror_dropzone_04", false, true);
      SetVisible("house_room2mirror_dropzone_05", false, true);
      SetVisible("house_room2mirror_dropzone_06", false, true);
      SetVisible("house_room2mirror_dropzone_07", false, true);
      SetVisible("house_room2mirror_dropzone_08", false, true);
      if (TaskToResolve("task_house_room2_freepoltergeist")) {
         ESoundBank::getSound("poltergeist")->playSample();
         SetVisible("house_room2mirror_ghost", true);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ROOM2_ASHLEY_DEMONINMIRROR2"), "", true);
      }
   }
}

void Scene_House_Room2_Mirror::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_House_Room2_Mirror::Logic()
{
	EScene::Logic();
}

void Scene_House_Room2_Mirror::Draw()
{
   EScene::Draw();
}

void Scene_House_Room2_Mirror::Close()
{
   
}

bool Scene_House_Room2_Mirror::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Fragments
   if (strcmp(szObjectName, "house_room2_frag01") == 0) {
      PickupSimple(szObjectName, "inv_room2mirror_frag01");
      return true;
   }
   if (strcmp(szObjectName, "house_room2_frag02") == 0) {
      PickupSimple(szObjectName, "inv_room2mirror_frag02");
      return true;
   }
   if (strcmp(szObjectName, "house_room2_frag03") == 0) {
      PickupSimple(szObjectName, "inv_room2mirror_frag03");
      return true;
   }
   if (strcmp(szObjectName, "house_room2_frag04") == 0) {
      PickupSimple(szObjectName, "inv_room2mirror_frag04");
      return true;
   }
   
   // Fantôme enfermé
   if (strcmp(szObjectName, "house_room2mirror_ghost") == 0) {
      if (TaskResolved("task_house_room2_repairmirror")) {
         EMiniJeuBase *lpMiniJeu = new EMiniJeuMirorBeam();
         _lpSceneDirector->GoToMiniGame("mirror_mirrors", lpMiniJeu);
        return true;
      }
      else {
         _lpSceneDirector->getSequencer()->Talk("demoninmirror", CHARACTER_POSX, CHARACTER_POSY, KStr("ROOM2_ASHLEY_DEMONINMIRROR"), "", true);
      }
   }
   return false;
}

bool Scene_House_Room2_Mirror::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_House_Room2_Mirror::ItemUsed(const char *szItemName, const char *szObjectName)
{
   bool bResult = false;
   if (strcmp(szObjectName, "house_room2mirror_dropzone_02") == 0) {
      if (strcmp(szItemName, "inv_room2mirror_frag02") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_house_room2_fragment02");
         SetVisible("house_room2_repaired_frag02", true);
         CheckMirrorFull();
         bResult = true;
      }
   }
   if (strcmp(szObjectName, "house_room2mirror_dropzone_03") == 0 || strcmp(szObjectName, "house_room2mirror_dropzone_05") == 0) {
      if (strcmp(szItemName, "inv_room2mirror_frag03") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_house_room2_fragment03");
         SetVisible("house_room2_repaired_frag03", true);
         CheckMirrorFull();
         bResult = true;
      }
      if (strcmp(szItemName, "inv_room2mirror_frag05") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_house_room2_fragment05");
         SetVisible("house_room2_repaired_frag05", true);
         CheckMirrorFull();
         bResult = true;
      }
   }
   if (strcmp(szObjectName, "house_room2mirror_dropzone_06") == 0) {
      if (strcmp(szItemName, "inv_room2mirror_frag06") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_house_room2_fragment06");
         SetVisible("house_room2_repaired_frag06", true);
         CheckMirrorFull();
         bResult = true;
      }
   }
   if (strcmp(szObjectName, "house_room2mirror_dropzone_00") == 0) {
      if (strcmp(szItemName, "inv_room2mirror_frag00") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_house_room2_fragment00");
         SetVisible("house_room2_repaired_frag00", true);
         CheckMirrorFull();
         bResult = true;
      }
   }
   if (strcmp(szObjectName, "house_room2mirror_dropzone_04") == 0) {
      if (strcmp(szItemName, "inv_room2mirror_frag04") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_house_room2_fragment04");
         SetVisible("house_room2_repaired_frag04", true);
         CheckMirrorFull();
         bResult = true;
      }
   }
   if (strcmp(szObjectName, "house_room2mirror_dropzone_08") == 0) {
      if (strcmp(szItemName, "inv_room2mirror_frag08") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_house_room2_fragment08");
         SetVisible("house_room2_repaired_frag08", true);
         CheckMirrorFull();
         bResult = true;
      }
   }
   if (strcmp(szObjectName, "house_room2mirror_dropzone_07") == 0 || strcmp(szObjectName, "house_room2mirror_dropzone_01") == 0) {
      if (strcmp(szItemName, "inv_room2mirror_frag07") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_house_room2_fragment07");
         SetVisible("house_room2_repaired_frag07", true);
         CheckMirrorFull();
         bResult = true;
      }
      if (strcmp(szItemName, "inv_room2mirror_frag01") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_house_room2_fragment01");
         SetVisible("house_room2_repaired_frag01", true);
         CheckMirrorFull();
         bResult = true;
      }
   }
   return bResult;
}

void Scene_House_Room2_Mirror::CheckMirrorFull()
{
   // Zone complétée
   if (TaskResolved("task_house_room2_fragment00")) {
      SetVisible("house_room2mirror_dropzone_00", false);
   }
   if (TaskResolved("task_house_room2_fragment01")) {
      SetVisible("house_room2mirror_dropzone_01", false);
   }
   if (TaskResolved("task_house_room2_fragment02")) {
      SetVisible("house_room2mirror_dropzone_02", false);
   }
   if (TaskResolved("task_house_room2_fragment03")) {
      SetVisible("house_room2mirror_dropzone_03", false);
   }
   if (TaskResolved("task_house_room2_fragment04")) {
      SetVisible("house_room2mirror_dropzone_04", false);
   }
   if (TaskResolved("task_house_room2_fragment05")) {
      SetVisible("house_room2mirror_dropzone_05", false);
   }
   if (TaskResolved("task_house_room2_fragment06")) {
      SetVisible("house_room2mirror_dropzone_06", false);
   }
   if (TaskResolved("task_house_room2_fragment07")) {
      SetVisible("house_room2mirror_dropzone_07", false);
   }
   if (TaskResolved("task_house_room2_fragment08")) {
      SetVisible("house_room2mirror_dropzone_08", false);
   }
   
   // Teste si le miroir est complet
   if (TaskToResolve("task_house_room2_repairmirror")) {
      if (TaskResolved("task_house_room2_fragment00")
          && TaskResolved("task_house_room2_fragment01")
          && TaskResolved("task_house_room2_fragment02")
          && TaskResolved("task_house_room2_fragment03")
          && TaskResolved("task_house_room2_fragment04")
          && TaskResolved("task_house_room2_fragment05")
          && TaskResolved("task_house_room2_fragment06")
          && TaskResolved("task_house_room2_fragment07")
          && TaskResolved("task_house_room2_fragment08")
          ) {
         SetVisible("house_room2mirror_dropzone_00", false, true);
         SetVisible("house_room2mirror_dropzone_01", false, true);
         SetVisible("house_room2mirror_dropzone_02", false, true);
         SetVisible("house_room2mirror_dropzone_03", false, true);
         SetVisible("house_room2mirror_dropzone_04", false, true);
         SetVisible("house_room2mirror_dropzone_05", false, true);
         SetVisible("house_room2mirror_dropzone_06", false, true);
         SetVisible("house_room2mirror_dropzone_07", false, true);
         SetVisible("house_room2mirror_dropzone_08", false, true);
         ResolveTask("task_house_room2_repairmirror");
         ResolveObjective("house","repairmirror");
         AddTask("task_house_room2_freepoltergeist");
         // Le moiroir réparé on aperçoit le fantôme dedans
         ESoundBank::getSound("success")->playSample();
         ESoundBank::getSound("poltergeist")->playSample();
         SetVisible("house_room2mirror_ghost", true);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ROOM2_ASHLEY_DEMONINMIRROR2"), "", true);
      }
   }
}

void Scene_House_Room2_Mirror::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FAILED"), "", true);
      return;
   }
   
   // Polter libéré ?
   if (strcmp(szGameName, "mirror_mirrors") == 0 && bIsRevolved) {
      ResolveTask("task_house_room2_freepoltergeist");
      AddTask("task_house_room2_killpoltergeist");
      // Le poltergeist attaque le vioque !
      ESoundBank::getSound("success")->playSample();
      ESoundBank::getSound("poltergeist")->playSample();
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_room2mirror_ghost", false, true);
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_room2", "reveal_poltergeist");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
   }
}