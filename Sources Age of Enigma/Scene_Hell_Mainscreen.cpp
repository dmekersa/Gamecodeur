/*
 *  Scene_Hell_Mainscreen.cpp
 *  enigma
 *
 *  Created by Rockford on 05/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Hell_Mainscreen.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"

/* Constructeur */
Scene_Hell_Mainscreen::Scene_Hell_Mainscreen(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_Hell_Mainscreen::~Scene_Hell_Mainscreen()
{
   delete _lpBgGraphic;
}

void Scene_Hell_Mainscreen::Init()
{
   // On arrive c'est l'horreur !
   if (!TaskResolved("task_hell_arrival")) {
      ResolveTask("task_hell_arrival");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_ARRIVAL"), "", false);
   }
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);

   // Les objets à collecter
   SetupItem("hell_item_beetle");
   SetupItem("hell_item_hourglass");
   SetupItem("hell_knife");
   SetupItem("hell_item_crow");
   SetupItem("hell_item_cane[1]");

   // Le corbeau
   if (TaskResolved("task_hell_falllemon")) {
      SetupItem("hell_lemon");
   }
   else {
      SetVisible("hell_item_lemon_hangon", true, true);
   }

   // Seth
   if (TaskResolved("task_hell_meetseth")) {
      SetVisible("hell_mainscreen_seth", true, true);
   }
   else {
      ResolveTask("task_hell_meetseth");
      SetVisible("hell_mainscreen_seth", true, true);
      // Début de la cinématique !
      int x,y;
      GetObjectPosition("hell_mainscreen_seth", x, y, true, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SETH_MEET1"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_MEETSETH1"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SETH_MEET2"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SETH_HELP"), "", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      AddTask("task_hell_foundrichard");
      AddObjective("hell","escape");
      AddHint("hell","escape","how");
      AddHint("hell","escape","richard");
      AddHint("hell","escape","soul");
   }
   
#ifdef BFG_TEASING
   _lpSceneDirector->getSequencer()->GoToScene(NULL, "teasing", "", false);
#endif
}

void Scene_Hell_Mainscreen::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      if (_lpSceneDirector->getInventory()->getItemFromInventory("inv_hell_coin") == NULL) {
         _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_hell_coin", 500, 500, 1);
      }
   }
#endif
}

void Scene_Hell_Mainscreen::Logic()
{
	EScene::Logic();
}

void Scene_Hell_Mainscreen::Draw()
{
   EScene::Draw();
}

void Scene_Hell_Mainscreen::Close()
{
}

bool Scene_Hell_Mainscreen::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "hell_mainscreen_seth") == 0)
   {
      int x,y;
      GetObjectPosition("hell_mainscreen_seth", x, y, true, false);
      if (!TaskResolved("task_hell_foundrichard")) {
         _lpSceneDirector->getSequencer()->Talk("seth", x, y,  KStr("HELL_SETH_HELP"), "", true);
      }
      else {
         if (!TaskResolved("task_hell_savesoul_richard")) {
            RemoveHint("hell","escape","richard");
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SETH_THANKS1"), "", true);
            ResolveTask("task_hell_reassureseth");
            ResolveTask("task_hell_savesoul_richard");
            GetObjectPosition("hell_mainscreen_seth", x, y, true, true);
            _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_hell_coin", x, y, 1);
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SETH_THANKS2"), "", true);
            AddTask("task_hell_leave");
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_SETH_THANKS2"), "", true);
         }
      }
      return true;
   }
   
   // Objets à collecter
   if (strcmp(szObjectName, "hell_item_crow") == 0)
   {
      PickupSimple("hell_item_crow", "inv_hell_item_crow");
      return true;
   }
   if (strcmp(szObjectName, "hell_item_beetle") == 0)
   {
      PickupSimple("hell_item_beetle", "inv_hell_item_beetle");
      return true;
   }
   if (strcmp(szObjectName, "hell_lemon") == 0)
   {
      PickupSimple("hell_lemon", "inv_hell_lemon");
      return true;
   }
   if (strcmp(szObjectName, "hell_item_hourglass") == 0)
   {
      PickupMultiple("hell_item_hourglass", "inv_hell_item_hourglass_empty",-1);
      return true;
   }
   if (strcmp(szObjectName, "hell_knife") == 0)
   {
      PickupSimple("hell_knife", "inv_hell_knife");
      return true;
   }
   if ( strncmp(szObjectName, "hell_item_cane[", strlen("hell_item_cane[")) == 0 )
   {
      PickupMultiple(szObjectName, "inv_hell_item_cane_part", 3);
   }
   
   return false;
}

bool Scene_Hell_Mainscreen::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Hell_Mainscreen::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur jette la pierre sur le corbeau
   if (strcmp(szItemName, "inv_hell_sling_armed") == 0) {
      if (strcmp(szObjectName, "hell_item_lemon_hangon") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "hell_item_lemon_hangon", "hell_item_lemon_falling");
         _lpSceneDirector->getSequencer()->Animation(NULL, "lemon_animp2p", true);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "hell_item_lemon_falling", "hell_lemon");
         ResolveTask("task_hell_falllemon");
         return true;
      }
   }
   
   return false;
}

void Scene_Hell_Mainscreen::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
