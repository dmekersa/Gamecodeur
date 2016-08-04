/*
 *  Scene_Egypt_Boat.cpp
 *  enigma
 *
 *  Created by Rockford on 05/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Egypt_Boatroom.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "MyGame.h"

/* Constructeur */
Scene_Egypt_Boatroom::Scene_Egypt_Boatroom(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _lpAnimDoor = new KCounter();
   _bAnimDoor = false;
}

/* Destructeur */
Scene_Egypt_Boatroom::~Scene_Egypt_Boatroom()
{
   XDELETE(_lpAnimDoor);
   delete _lpBgGraphic;
}

void Scene_Egypt_Boatroom::Init()
{
   if (!ObjectiveResolved("egypt","laby")) {
      ResolveObjective("egypt","laby");
      AddObjective("egypt","boat");
      AddHint("egypt","boat","how");
      AddHint("egypt","boat","keyboat");
   }
   
   ResolveTask("task_egypt_resolvemaze");
   if (!TaskResolved("task_egypt_resolveboat")) {
      AddTask("task_egypt_resolveboat");
   }
   else {
      if (TaskResolved("task_egypt_getboatkey")) {
         SetVisible("egypt_boat_zone", false, true);
      }
   }
   
   if (TaskResolved("task_egypt_opentomb")) {
      SetVisible("egypt_boatroom_door", false, true);
      SetVisible("egypt_boatroom_key", true, true);
      SetVisible("egypt_boatroom_keyzone", false);
   }
}

void Scene_Egypt_Boatroom::Check()
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

void Scene_Egypt_Boatroom::Logic()
{
	EScene::Logic();
}

void Scene_Egypt_Boatroom::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   if (_bAnimDoor) {
      _lpAnimDoor->move(fElapsed);
      // Déplace la porte
      RestorePosition("egypt_boatroom_door");
      MovePosition("egypt_boatroom_door", _lpAnimDoor->getCurrentValue(), GetRandom(-1.0, 1.0));
      if (_lpAnimDoor->isCompleted()) {
         _bAnimDoor = false;
         ResolveTask("task_egypt_opentomb");
         SetVisible("egypt_boatroom_key", true);
         SetVisible("egypt_boatroom_keyzone", false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, false);
      }
   }
   
   EScene::Draw();
}

void Scene_Egypt_Boatroom::Close()
{
}

void Scene_Egypt_Boatroom::Callback(const char *szParam)
{
   SetVisible("back2", false);
   SetVisible("egypt_boatroomkey_bg", false);
   SetVisible("egypt_boatroomkey_key_zone", false);
   SetVisible("egypt_boatroomkey_key", false);
   StartEffect();
}

void Scene_Egypt_Boatroom::StartEffect()
{
   _bAnimDoor = true;
   int x, y;
   SetVisible("egypt_boatroom_door", true, true);
   GetObjectPosition("egypt_boatroom_door", x, y, false, false);
   ESoundBank::getSound("rockdoor")->playSample();
   _lpAnimDoor->startCounter(0.0f, -82.0f, 0, 10000, K_COUNTER_EASEOUT);
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
}

bool Scene_Egypt_Boatroom::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "egypt_boatroom_keyzone") == 0) {
      SetVisible("back2", true);
      SetVisible("egypt_boatroomkey_bg", true);
      SetVisible("egypt_boatroomkey_key_zone", true);
   }
   if (strcmp(szObjectName, "egypt_boatroomkey_bg") == 0 || strcmp(szObjectName, "back2") == 0) {
      SetVisible("back2", false);
      SetVisible("egypt_boatroomkey_bg", false);
      SetVisible("egypt_boatroomkey_key_zone", false);
   }
   return false;
}

bool Scene_Egypt_Boatroom::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Egypt_Boatroom::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (strcmp(szItemName, "inv_egypt_boat_key") == 0) {
      if (strcmp(szObjectName, "egypt_boatroomkey_key_zone") == 0) {
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
         SetVisible("back2", false);
         SetVisible("egypt_boatroom_key", true);
         _lpSceneDirector->DropItem("inv_egypt_boat_key");
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"reveal");
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_boatroomkey_key", true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", false);
         ResolveObjective("egypt","boat");
         _lpSceneDirector->getSequencer()->Callback(NULL,"");
         return true;
      }
   }
   
   return false;
}

void Scene_Egypt_Boatroom::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
