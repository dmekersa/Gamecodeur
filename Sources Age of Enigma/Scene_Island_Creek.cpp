/*
 *  Scene_Island_Creek.cpp
 *  enigma
 *
 *  Created by Rockford on 21/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Island_Creek.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"

/* Constructeur */
Scene_Island_Creek::Scene_Island_Creek(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_Island_Creek::~Scene_Island_Creek()
{
   delete _lpBgGraphic;
}

void Scene_Island_Creek::Init()
{
   StartAnimation("bganim");

   SetSmoothOverlap("parrotfly", true);
   // Le perroquet a été chassé
   if (!TaskResolved("task_island_expelparrot")) {
      SetVisible("parrot", true);
   }
   
   // La cavité a été découverte ?
   if (TaskResolved("task_island_removebranches")) {
      SetVisible("wood", false);
   }
   
   // Objets à ramasser
   SetupItem("island_rope");
   if (TaskResolved("task_island_breakhead")) {
      SetupItem("island_blowpipe");
      SetVisible("island_creek_head1", false, true);
      SetVisible("island_creek_head2", true, true);
   }
   else {
      SetVisible("island_creek_head1", true, true);
      SetVisible("island_creek_head2", false, true);
   }

   if (TaskResolved("task_island_crabhole")) {
      SetVisible("island_creek_crab2", true);
      SetupItem("island_map");
      SetVisible("island_creek_crabhole", false);
   }
   else {
      AddTask("task_island_crabhole");
   }
   
   // Est-ce qu'on a tout terminé ici ?
   if (ItemCollected("island_blowpipe") &&
       ItemCollected("island_shovelhandle") &&
       ItemCollected("island_map") &&
       TaskResolved("task_island_expelparrot")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED"), "", true);
   }

}

void Scene_Island_Creek::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_Island_Creek::Logic()
{
	EScene::Logic();
}

void Scene_Island_Creek::Draw()
{
   EScene::Draw();
}

void Scene_Island_Creek::Close()
{
   
}

bool Scene_Island_Creek::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Cordelette
   if (strcmp(szObjectName, "island_rope") == 0) {
      PickupMultiple(szObjectName, "inv_island_rope",-1);
      // La tête tombe !
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "skullfall");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "island_creek_head1", "island_creek_head2");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "island_blowpipe", true);
      ResolveTask("task_island_breakhead");
      return true;
   }
   
   if (strcmp(szObjectName, "island_blowpipe") == 0) {
      PickupMultiple(szObjectName, "inv_island_blowpipe",-1);
      return true;
   }
      
   if (strcmp(szObjectName, "island_map") == 0) {
      PickupSimple(szObjectName, "inv_island_map");
      RemoveHint("island","findchest","map");
      AddObjective("island","shovel");
      AddHint("island","shovel","how");
      return true;
   }
   
   // Volcan
   if ( strcmp(szObjectName, "volcano") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk("volcano", CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_CREEK_MURRAY_VOLCANO"), "", true);
      return true;
   }
   
   // Débris de bénitiers
   if ( strcmp(szObjectName, "sand") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk("sand", CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_CREEK_MURRAY_SAND"), "", true);
      return true;
   }
   
   // Perroquet
   if ( strcmp(szObjectName, "parrot") == 0)
   {
      int x,y;
      GetObjectPosition(szObjectName, x, y);
//      _lpSceneDirector->getSequencer()->Talk("coco", x, y, KStr("ISLAND_CREEK_PARROT_PEARL"), "", false);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "parrotfly");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "parrot", false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "parrotfly", true);
      _lpSceneDirector->getSequencer()->Animation(NULL,"parrotanimframe",true);
      _lpSceneDirector->getSequencer()->Animation(NULL,"parrotanimp2p",true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_CREEK_MURRAY_WHERE"), "", false);
      ResolveTask("task_island_expelparrot");
      AddTask("task_island_getpearl");
      AddObjective("island","pearl");
      AddHint("island","pearl","where");
      return true;
   }
   return false;
}

bool Scene_Island_Creek::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Island_Creek::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (strcmp(szItemName, "inv_island_crab") == 0) {
      if (strcmp(szObjectName, "island_creek_crabhole") == 0) {
         ResolveTask("task_island_crabhole");
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "island_creek_crab1", true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "brokenstick");
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "island_creek_crab1", "island_creek_crab2");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "island_map", true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "island_creek_crabhole", false);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "island_creek_crab2", "island_creek_crab1");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "island_creek_crab1", false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         return true;
      }
   }
   return false;
}
