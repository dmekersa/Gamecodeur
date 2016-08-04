/*
 *  Scene_Egypt_Boat.cpp
 *  enigma
 *
 *  Created by Rockford on 05/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Egypt_Boat.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"

/* Constructeur */
Scene_Egypt_Boat::Scene_Egypt_Boat(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_Egypt_Boat::~Scene_Egypt_Boat()
{
   delete _lpBgGraphic;
}

void Scene_Egypt_Boat::Init()
{
   if (TaskResolved("egypt_boat_fig01_zone")) {
      SetVisible("egypt_boat_fig01", true);
      SetVisible("egypt_boat_fig01_zone", false);
   }
   if (TaskResolved("egypt_boat_fig02_zone")) {
      SetVisible("egypt_boat_fig02", true);
      SetVisible("egypt_boat_fig02_zone", false);
   }
   if (TaskResolved("egypt_boat_fig03_zone")) {
      SetVisible("egypt_boat_fig03", true);
      SetVisible("egypt_boat_fig03_zone", false);
   }
   if (TaskResolved("egypt_boat_fig04_zone")) {
      SetVisible("egypt_boat_fig04", true);
      SetVisible("egypt_boat_fig04_zone", false);
   }
   if (TaskResolved("egypt_boat_fig05_zone")) {
      SetVisible("egypt_boat_fig05", true);
      SetVisible("egypt_boat_fig05_zone", false);
   }
   if (TaskResolved("egypt_boat_fig06_zone")) {
      SetVisible("egypt_boat_fig06", true);
      SetVisible("egypt_boat_fig06_zone", false);
   }
   if (TaskResolved("egypt_boat_fig07_zone")) {
      SetVisible("egypt_boat_fig07", true);
      SetVisible("egypt_boat_fig07_zone", false);
   }
   if (TaskResolved("egypt_boat_fig08_zone")) {
      SetVisible("egypt_boat_fig08", true);
      SetVisible("egypt_boat_fig08_zone", false);
   }
   if (TaskResolved("egypt_boat_fig09_zone")) {
      SetVisible("egypt_boat_fig09", true);
      SetVisible("egypt_boat_fig09_zone", false);
   }
   if (TaskResolved("task_egypt_resolveboat")) {
      SetupItem("egypt_boat_key");
   }
   
   if (TaskResolved("task_egypt_resolveboat")) {
      SetupItem("egypt_boat_key");
   }
   else {
      CheckBoatCompleted();
   }

}

void Scene_Egypt_Boat::CheckBoatCompleted()
{
   if (TaskResolved("task_egypt_resolveboat")) {
      return;
   }
   if (TaskResolved("egypt_boat_fig01_zone") &&
       TaskResolved("egypt_boat_fig02_zone") &&
       TaskResolved("egypt_boat_fig03_zone") &&
       TaskResolved("egypt_boat_fig04_zone") &&
       TaskResolved("egypt_boat_fig05_zone") &&
       TaskResolved("egypt_boat_fig06_zone") &&
       TaskResolved("egypt_boat_fig07_zone") &&
       TaskResolved("egypt_boat_fig08_zone") &&
       TaskResolved("egypt_boat_fig09_zone")) {
      _lpSceneDirector->getSequencer()->PlaySound(NULL,"reveal");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_glow",true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_boat_key", true);
      ResolveTask("task_egypt_resolveboat");
      RemoveHint("egypt","boat","how");
      AddTask("task_egypt_opentomb");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_NEFERES_RESOLVEBOAT"), "", false, false);
   }
}

void Scene_Egypt_Boat::Check()
{
   EScene::Check();
   CheckBoatCompleted();

#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_boat_key", true);
   }
#endif
}

void Scene_Egypt_Boat::Logic()
{
	EScene::Logic();
}

void Scene_Egypt_Boat::Draw()
{
   EScene::Draw();
}

void Scene_Egypt_Boat::Close()
{
}

bool Scene_Egypt_Boat::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "egypt_boat_key") == 0) {
      ResolveTask("task_egypt_getboatkey");
      PickupSimple(szObjectName, "inv_egypt_boat_key");
   }
   return false;
}

bool Scene_Egypt_Boat::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Egypt_Boat::ItemUsed(const char *szItemName, const char *szObjectName)
{
   bool bBadPlace = false;
   if (strcmp(szItemName, "inv_egypt_fig01") == 0) {
      if (strcmp(szObjectName, "egypt_boat_fig01_zone") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         SetVisible("egypt_boat_fig01", true);
         SetVisible("egypt_boat_fig01_zone", false);
         ResolveTask(szObjectName);
         _lpSceneDirector->DropItem(szItemName);
         return true;
      }
      else {
         bBadPlace = true;
      }
   }
   if (strcmp(szItemName, "inv_egypt_fig02") == 0) {
      if (strcmp(szObjectName, "egypt_boat_fig02_zone") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         SetVisible("egypt_boat_fig02", true);
         SetVisible("egypt_boat_fig02_zone", false);
         ResolveTask(szObjectName);
         _lpSceneDirector->DropItem(szItemName);
         return true;
      }
      else {
         bBadPlace = true;
      }
   }
   if (strcmp(szItemName, "inv_egypt_fig03") == 0) {
      if (strcmp(szObjectName, "egypt_boat_fig03_zone") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         SetVisible("egypt_boat_fig03", true);
         SetVisible("egypt_boat_fig03_zone", false);
         ResolveTask(szObjectName);
         _lpSceneDirector->DropItem(szItemName);
         return true;
      }
      else {
         bBadPlace = true;
      }
   }
   if (strcmp(szItemName, "inv_egypt_fig04") == 0) {
      if (strcmp(szObjectName, "egypt_boat_fig04_zone") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         SetVisible("egypt_boat_fig04", true);
         SetVisible("egypt_boat_fig04_zone", false);
         ResolveTask(szObjectName);
         _lpSceneDirector->DropItem(szItemName);
         return true;
      }
      else {
         bBadPlace = true;
      }
   }
   if (strcmp(szItemName, "inv_egypt_fig05") == 0) {
      if (strcmp(szObjectName, "egypt_boat_fig05_zone") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         SetVisible("egypt_boat_fig05", true);
         SetVisible("egypt_boat_fig05_zone", false);
         ResolveTask(szObjectName);
         _lpSceneDirector->DropItem(szItemName);
         return true;
      }
      else {
         bBadPlace = true;
      }
   }
   if (strcmp(szItemName, "inv_egypt_fig06") == 0) {
      if (strcmp(szObjectName, "egypt_boat_fig06_zone") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         SetVisible("egypt_boat_fig06", true);
         SetVisible("egypt_boat_fig06_zone", false);
         ResolveTask(szObjectName);
         _lpSceneDirector->DropItem(szItemName);
         return true;
      }
      else {
         bBadPlace = true;
      }
   }
   if (strcmp(szItemName, "inv_egypt_fig07") == 0) {
      if (strcmp(szObjectName, "egypt_boat_fig07_zone") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         SetVisible("egypt_boat_fig07", true);
         SetVisible("egypt_boat_fig07_zone", false);
         ResolveTask(szObjectName);
         _lpSceneDirector->DropItem(szItemName);
         return true;
      }
      else {
         bBadPlace = true;
      }
   }
   if (strcmp(szItemName, "inv_egypt_fig08") == 0) {
      if (strcmp(szObjectName, "egypt_boat_fig08_zone") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         SetVisible("egypt_boat_fig08", true);
         SetVisible("egypt_boat_fig08_zone", false);
         ResolveTask(szObjectName);
         _lpSceneDirector->DropItem(szItemName);
         return true;
      }
      else {
         bBadPlace = true;
      }
   }
   if (strcmp(szItemName, "inv_egypt_fig09") == 0) {
      if (strcmp(szObjectName, "egypt_boat_fig09_zone") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         SetVisible("egypt_boat_fig09", true);
         SetVisible("egypt_boat_fig09_zone", false);
         ResolveTask(szObjectName);
         _lpSceneDirector->DropItem(szItemName);
         return true;
      }
      else {
         bBadPlace = true;
      }
   }
   
   // Mal placé
   if (bBadPlace) {
      _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("EGYPT_NEFERES_BADPLACE"), "");
      return true;
   }
   
   return false;
}

void Scene_Egypt_Boat::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
