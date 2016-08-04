/*
 *  Scene_Hell_Styx.cpp
 *  enigma
 *
 *  Created by Rockford on 05/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Hell_Styx.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "ThreadLoader.h"

/* Constructeur */
Scene_Hell_Styx::Scene_Hell_Styx(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   Loader::LoadScene("hell_door");
}

/* Destructeur */
Scene_Hell_Styx::~Scene_Hell_Styx()
{
   delete _lpBgGraphic;
}

void Scene_Hell_Styx::Init()
{
   StartAnimation("clouds_anim");
   
   SetupItem("hell_stone");
   SetupItem("hell_sand");
   SetupItem("hell_sling_rope");
   SetupItem("hell_item_clover");
   SetupItem("hell_item_cane[3]");
   
   StartAnimation("charon_animp2p");

   if (!TaskResolved("talk_hell_meetcharon")) {
      ResolveTask("talk_hell_meetcharon");
      AddObjective("hell","styx");
      AddHint("hell","styx","how");
   }
       
}

void Scene_Hell_Styx::Check()
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

void Scene_Hell_Styx::Logic()
{
	EScene::Logic();
}

void Scene_Hell_Styx::Draw()
{
   EScene::Draw();
}

void Scene_Hell_Styx::Close()
{
}

bool Scene_Hell_Styx::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "hell_stone") == 0)
   {
      PickupMultiple("hell_stone", "inv_hell_stone",-1);
      return true;
   }
   if (strcmp(szObjectName, "hell_sand") == 0)
   {
      PickupMultiple("hell_sand", "inv_hell_sand",-1);
      return true;
   }
   if (strcmp(szObjectName, "hell_sling_rope") == 0)
   {
      PickupMultiple("hell_sling_rope", "inv_hell_sling_rope",-1);
      return true;
   }
   if (strcmp(szObjectName, "hell_item_clover") == 0)
   {
      PickupSimple("hell_item_clover", "inv_hell_item_clover");
      return true;
   }
   if ( strncmp(szObjectName, "hell_item_cane[", strlen("hell_item_cane[")) == 0 )
   {
      PickupMultiple(szObjectName, "inv_hell_item_cane_part", 3);
      return true;
   }
   
   if (strcmp(szObjectName, "hell_charon") == 0) {
      int x,y;
      GetObjectPosition("hell_charon", x, y, true, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_CHARON"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_CHARON_PAY"), "", true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HELL_ASHLEY_CHARON2"), "", false, false);
      return true;
   }
      
   return false;
}

bool Scene_Hell_Styx::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Hell_Styx::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (strcmp(szObjectName, "hell_charon") == 0) {
      int x,y;
      GetObjectPosition("hell_charon", x, y, true, false);
      // Le joueur donne la pièce
      if (strcmp(szItemName, "inv_hell_coin") == 0) {
         // On traverse !
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         ResolveObjective("hell","styx");
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_CHARON_COME"), "", false);
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "video_player", "styx", false);
         return true;
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HELL_CHARON_NO"), "", true);
      }

   }
   return false;
}

void Scene_Hell_Styx::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
