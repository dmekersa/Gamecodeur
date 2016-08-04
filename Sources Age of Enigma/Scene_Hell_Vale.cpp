/*
 *  Scene_Hell_Vale.cpp
 *  enigma
 *
 *  Created by Rockford on 05/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Hell_Vale.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"

/* Constructeur */
Scene_Hell_Vale::Scene_Hell_Vale(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_Hell_Vale::~Scene_Hell_Vale()
{
   delete _lpBgGraphic;
}

void Scene_Hell_Vale::Init()
{
   StartAnimation("smoke_for_hell_anim");
   StartAnimation("clouds_anim");
   
   // Objets à ramasser
   SetupItem("hell_item_heart");
   SetupItem("hell_item_eyeball");
   SetupItem("hell_item_butterfly");
   SetupItem("hell_item_cane[2]");
   SetupItem("hell_sling_stick");
   
   // Quelle est l'âme visible ?
   if (TaskResolved("task_hell_meetsoul_1") && !TaskResolved("task_hell_savesoul_1")) {
      SetVisible("hell_vale_soul_1", true);
   }
   else if (TaskResolved("task_hell_meetsoul_2") && !TaskResolved("task_hell_savesoul_2")) {
      SetVisible("hell_vale_soul_2", true);
   }
   else if (TaskResolved("task_hell_meetsoul_3") && !TaskResolved("task_hell_savesoul_3")) {
      SetVisible("hell_vale_soul_3", true);
   }
   else if (TaskResolved("task_hell_meetsoul_richard")) {
      SetVisible("hell_vale_soul_richard", true);
   }
}

void Scene_Hell_Vale::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
   }
#endif
}

void Scene_Hell_Vale::Logic()
{
	EScene::Logic();
}

void Scene_Hell_Vale::Draw()
{
   EScene::Draw();
}

void Scene_Hell_Vale::Close()
{
   //   _lpSoundDesign->stopSample();
}

bool Scene_Hell_Vale::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Le joueur pioche dans les âmes
   if (strcmp(szObjectName, "hell_vale_hands_zone") == 0) {
      // Il y a déjà une âme
      if (isVisible("hell_vale_soul_1")
          || isVisible("hell_vale_soul_2")
          || isVisible("hell_vale_soul_3")
          ) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_ENOUGHSOUL"), "", true, false);
         return true;
      }
      else if (isVisible("hell_vale_soul_richard"))
      {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_ENDSOUL"), "", true, false);
         return true;
      }
      // On pioche une âme
      else if (TaskResolved("task_hell_foundrichard")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_ENDSOUL"), "", true, false);
      }
      else if (TaskResolved("task_hell_savesoul_3")) {
         RemoveHint("hell","escape","soul");
         ResolveTask("task_hell_meetsoul_richard");
         AddTask("task_hell_reassureseth");
         StartAnimation("soul4anim");
         StartAnimation("soul4rot");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "hell_vale_soul_richard", true);
         SoulRequest(4);
      }
      else if (TaskResolved("task_hell_savesoul_2")) {
         ResolveTask("task_hell_meetsoul_3");
         AddTask("task_hell_savesoul_3");
         StartAnimation("soul3anim");
         StartAnimation("soul3rot");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "hell_vale_soul_3", true);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_NEWSOUL"), "", false, true);
         SoulRequest(3);
      }
      else if (TaskResolved("task_hell_savesoul_1")) {
         ResolveTask("task_hell_meetsoul_2");
         AddTask("task_hell_savesoul_2");
         StartAnimation("soul2anim");
         StartAnimation("soul3rot");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "hell_vale_soul_2", true);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_NEWSOUL"), "", false, true);
         SoulRequest(2);
      }
      else {
         ResolveTask("task_hell_meetsoul_1");
         AddTask("task_hell_savesoul_1");
         StartAnimation("soul1anim");
         StartAnimation("soul1rot");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "hell_vale_soul_1", true);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_NEWSOUL"), "", false, true);
         SoulRequest(1);
      }
   }
   
   // Clic sur une âme : rappel
   if (strcmp(szObjectName, "hell_vale_soul_1") == 0) {
      SoulRequest(1);
   }
   else if (strcmp(szObjectName, "hell_vale_soul_2") == 0) {
      SoulRequest(2);
   }
   else if (strcmp(szObjectName, "hell_vale_soul_3") == 0) {
      SoulRequest(3);
   }
   else if (strcmp(szObjectName, "hell_vale_soul_richard") == 0) {
      _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_ENDSOUL"), "", true, false);
   }
   
   // Objets à ramasser
   if (strcmp(szObjectName, "hell_item_heart") == 0)
   {
      PickupSimple(szObjectName, "inv_hell_item_heart");
      return true;
   }
   if (strcmp(szObjectName, "hell_item_eyeball") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_EYEBALL"), "", true, false);
      return true;
   }
   if (strcmp(szObjectName, "hell_item_butterfly") == 0)
   {
      PickupSimple(szObjectName, "inv_hell_item_butterfly");
      return true;
   }
   if (strcmp(szObjectName, "hell_sling_stick") == 0)
   {
      PickupMultiple(szObjectName, "inv_hell_sling_stick",-1);
      return true;
   }
   if ( strncmp(szObjectName, "hell_item_cane[", strlen("hell_item_cane[")) == 0 )
   {
      PickupMultiple(szObjectName, "inv_hell_item_cane_part", 3);
   }
   
   return false;
}

void Scene_Hell_Vale::SoulCheck(int nSoul)
{
   int x,y;
   bool bDone = true;
   switch (nSoul) {
      case 1:
         GetObjectPosition("hell_vale_soul_1", x, y, true, false);
         // Demande les objets non encore distribués
         if (!TaskResolved("task_hell_soul1_give1")) {
            bDone = false;
         }
         if (!TaskResolved("task_hell_soul1_give2")) {
            bDone = false;
         }
         if (!TaskResolved("task_hell_soul1_give3")) {
            bDone = false;
         }
         if (!bDone) {
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SOUL_MORE"), "", true);
         }
         else {
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SOUL_DONE"), "", false);
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "hell_vale_soul_1", false);
            ResolveTask("task_hell_savesoul_1");
         }
         return;
         break;
      case 2:
         GetObjectPosition("hell_vale_soul_2", x, y, true, false);
         // Demande les objets non encore distribués
         if (!TaskResolved("task_hell_soul2_give1")) {
            bDone = false;
         }
         if (!TaskResolved("task_hell_soul2_give2")) {
            bDone = false;
         }
         if (!TaskResolved("task_hell_soul2_give3")) {
            bDone = false;
         }
         if (!bDone) {
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SOUL_MORE"), "", true);
         }
         else {
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SOUL_DONE"), "", false);
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "hell_vale_soul_2", false);
            ResolveTask("task_hell_savesoul_2");
         }
         return;
         break;
      case 3:
         GetObjectPosition("hell_vale_soul_3", x, y, true, false);
         // Demande les objets non encore distribués
         if (!TaskResolved("task_hell_soul3_give1")) {
            bDone = false;
         }
         if (!TaskResolved("task_hell_soul3_give2")) {
            bDone = false;
         }
         if (!TaskResolved("task_hell_soul3_give3")) {
            bDone = false;
         }
         if (!bDone) {
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SOUL_MORE"), "", true);
         }
         else {
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SOUL_DONE"), "", false);
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "hell_vale_soul_3", false);
            ResolveTask("task_hell_savesoul_3");
         }
         return;
         break;
      default:
         break;
   }
}

void Scene_Hell_Vale::SoulRequest(int nSoul)
{
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
   int x,y;
   switch (nSoul) {
      case 1:
         GetObjectPosition("hell_vale_soul_1", x, y, true, false);
         y = 330; // En dur car comme l'âme est animée, on n'a pas sa position finale à ce stade
         _lpSceneDirector->getSequencer()->Talk("HELL_SOUL_IWOULDLIKE", x, y, KStr("HELL_SOUL_IWOULDLIKE"), "", false);
         // Demande les objets non encore distribués
         if (!TaskResolved("task_hell_soul1_give1")) {
            _lpSceneDirector->getSequencer()->Talk("HELL_SOUL1_GIVE1", x, y, KStr("HELL_SOUL1_GIVE1"), "", false);
         }
         if (!TaskResolved("task_hell_soul1_give2")) {
            _lpSceneDirector->getSequencer()->Talk("HELL_SOUL1_GIVE2", x, y, KStr("HELL_SOUL1_GIVE2"), "", false);
         }
         if (!TaskResolved("task_hell_soul1_give3")) {
            _lpSceneDirector->getSequencer()->Talk("HELL_SOUL1_GIVE3", x, y, KStr("HELL_SOUL1_GIVE3"), "", false);
         }
         break;
      case 2:
         GetObjectPosition("hell_vale_soul_2", x, y, true, false);
         y = 330; // En dur car comme l'âme est animée, on n'a pas sa position finale à ce stade
         _lpSceneDirector->getSequencer()->Talk("HELL_SOUL_IWOULDLIKE", x, y, KStr("HELL_SOUL_IWOULDLIKE"), "", false);
         // Demande les objets non encore distribués
         if (!TaskResolved("task_hell_soul2_give1")) {
            _lpSceneDirector->getSequencer()->Talk("HELL_SOUL2_GIVE1", x, y, KStr("HELL_SOUL2_GIVE1"), "", false);
         }
         if (!TaskResolved("task_hell_soul2_give2")) {
            _lpSceneDirector->getSequencer()->Talk("HELL_SOUL2_GIVE2", x, y, KStr("HELL_SOUL2_GIVE2"), "", false);
         }
         if (!TaskResolved("task_hell_soul2_give3")) {
            _lpSceneDirector->getSequencer()->Talk("HELL_SOUL2_GIVE3", x, y, KStr("HELL_SOUL2_GIVE3"), "", false);
         }
         break;
      case 3:
         GetObjectPosition("hell_vale_soul_3", x, y, true, false);
         y = 330; // En dur car comme l'âme est animée, on n'a pas sa position finale à ce stade
         _lpSceneDirector->getSequencer()->Talk("HELL_SOUL_IWOULDLIKE", x, y, KStr("HELL_SOUL_IWOULDLIKE"), "", false);
         // Demande les objets non encore distribués
         if (!TaskResolved("task_hell_soul3_give1")) {
            _lpSceneDirector->getSequencer()->Talk("HELL_SOUL3_GIVE1", x, y, KStr("HELL_SOUL3_GIVE1"), "", false);
         }
         if (!TaskResolved("task_hell_soul3_give2")) {
            _lpSceneDirector->getSequencer()->Talk("HELL_SOUL3_GIVE2", x, y, KStr("HELL_SOUL3_GIVE2"), "", false);
         }
         if (!TaskResolved("task_hell_soul3_give3")) {
            _lpSceneDirector->getSequencer()->Talk("HELL_SOUL3_GIVE3", x, y, KStr("HELL_SOUL3_GIVE3"), "", false);
         }
         break;
      case 4:
         GetObjectPosition("hell_vale_soul_richard", x, y, true, false);
         y = 330; // En dur car comme l'âme est animée, on n'a pas sa position finale à ce stade
         _lpSceneDirector->getSequencer()->Talk("HELL_SOUL4_IMOK", x, y, KStr("HELL_SOUL4_IMOK"), "", false);
         ResolveTask("task_hell_foundrichard");
         break;
      default:
         break;
   }
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
}

bool Scene_Hell_Vale::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Hell_Vale::ItemUsed(const char *szItemName, const char *szObjectName)
{
   int x,y;

   // Le joueur extrait l'oeil avec le couteau
   if (strcmp(szItemName, "inv_hell_knife") == 0) {
      if (strcmp(szObjectName, "hell_item_eyeball") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         PickupSimple("hell_item_eyeball", "inv_hell_item_eyeball");
         return true;
      }
   }
   // Le joueur utilise le citron sur la fumée
   if (strcmp(szItemName, "inv_hell_lemon") == 0) {
      if (strcmp(szObjectName, "hell_smoke") == 0) {
         ESoundBank::getSound("success")->playSample();
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getInventory()->AddItem("hell", "inv_hell_item_fruit");
         return true;
      }
   }
   
   // Distribution des cadeaux !
   
   // Distribution à l'âme n°1
   if (strcmp(szObjectName, "hell_vale_soul_1") == 0) {
      GetObjectPosition("hell_vale_soul_1", x, y, true, false);
      // Le coeur
      if (strcmp(szItemName, "inv_hell_item_heart") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_hell_soul1_give1");
         SoulCheck(1);
         return true;
      }
      // Le trefle
      else if (strcmp(szItemName, "inv_hell_item_clover") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_hell_soul1_give2");
         SoulCheck(1);
         return true;
      }
      // Le scarabée
      else if (strcmp(szItemName, "inv_hell_item_beetle") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_hell_soul1_give3");
         SoulCheck(1);
         return true;
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("HELL_SOUL_NOTHKS"), "", false);
         return true;
      }

   }

   // Distribution à l'âme n°2
   if (strcmp(szObjectName, "hell_vale_soul_2") == 0) {
      GetObjectPosition("hell_vale_soul_2", x, y, true, false);
      // L'oeil
      if (strcmp(szItemName, "inv_hell_item_eyeball") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_hell_soul2_give1");
         SoulCheck(2);
         return true;
      }
      // Le corbeau
      else if (strcmp(szItemName, "inv_hell_item_crow") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_hell_soul2_give2");
         SoulCheck(2);
         return true;
      }
      // Le sablier plein
      else if (strcmp(szItemName, "inv_hell_item_hourglass_full") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_hell_soul2_give3");
         SoulCheck(2);
         return true;
      }
      // Le sablier vide
      else if (strcmp(szItemName, "inv_hell_item_hourglass_empty") == 0) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("HELL_SOUL2_EMPTY"), "", false);
         return true;
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("HELL_SOUL_NOTHKS"), "", false);
         return true;
      }
   }
   
   // Distribution à l'âme n°3
   if (strcmp(szObjectName, "hell_vale_soul_3") == 0) {
      GetObjectPosition("hell_vale_soul_3", x, y, true, false);
      // L'aile de papillon
      if (strcmp(szItemName, "inv_hell_item_butterfly") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_hell_soul3_give1");
         SoulCheck(3);
         return true;
      }
      // Le fruit pourri
      else if (strcmp(szItemName, "inv_hell_item_fruit") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_hell_soul3_give2");
         SoulCheck(3);
         return true;
      }
      // Le fruit non pourri
      else if (strcmp(szItemName, "inv_hell_lemon") == 0) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("HELL_SOUL3_ROTEN"), "", false);
         return true;
      }
      // La canne
      else if (strcmp(szItemName, "inv_hell_item_cane_full") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_hell_soul3_give3");
         SoulCheck(3);
         return true;
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("HELL_SOUL_NOTHKS"), "", false);
         return true;
      }
   }

   return false;
}

void Scene_Hell_Vale::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
