/*
 *  Scene_House_Firstfloor.cpp
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Firstfloor.h"
#include "ESceneDirector.h"

/* Constructeur */
SceneFirstfloor::SceneFirstfloor(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
SceneFirstfloor::~SceneFirstfloor()
{
   delete _lpBgGraphic;
}

void SceneFirstfloor::Init()
{
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_HOUSE);
   
   SetupItem("envelope_firstfloor");
   
   // Retrouvailles Isaac
   if (!TaskResolved("task_house_firstfloor_isaacreunion")) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_firstfloor_isaac1", true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("FIRSTFLOOR_ASHLEY_ISAACREUNION"), "", false);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_firstfloor_isaac1", "house_firstfloor_isaac2");
      ResolveTask("task_house_firstfloor_isaacreunion");
      // Maintenant on ne peut plus redescendre
      ResolveTask("task_house_hall_groundfloor");
   }
   
   // Il reste une âme à libérer, Nathan vient casser les burnes de Ashley...
   if (!TaskResolved("task_house_meetnathan1stfloor")) {
      bool bMeetNathan = false;
      if (TaskResolved("task_middleage_universeresolved") &&
          !TaskResolved("task_egypt_universeresolved")) {
         bMeetNathan = true;
      }
      else if (TaskResolved("task_egypt_universeresolved") &&
               !TaskResolved("task_middleage_universeresolved")) {
         bMeetNathan = true;
      }
      if (bMeetNathan) {
         int x,y;
         ResolveTask("task_house_meetnathan1stfloor");
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls",true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_shadow",true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "attic_nathanappear");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_firstfloor_nathan", true);
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls",false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         GetObjectPosition("house_firstfloor_nathan", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET4_1"), "HALL_NATHAN_MEET4_1", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY,
                                                KStr("HALL_ASHLEY_MEET4_2"), "hall_ashley_meet4_2", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET4_3"), "HALL_NATHAN_MEET4_3", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY,
                                                KStr("HALL_ASHLEY_MEET4_4"), "hall_ashley_meet4_4", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET4_5"), "HALL_NATHAN_MEET4_5", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET4_6"), "HALL_NATHAN_MEET4_6", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY,
                                                KStr("HALL_ASHLEY_MEET4_7"), "hall_ashley_meet4_7", false, true);
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls",true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET4_8"), "HALL_NATHAN_MEET4_8", false);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostgone");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_firstfloor_nathan", false);
         _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("HALL_NATHAN_MEET4_9"), "HALL_NATHAN_MEET4_9", SEQUENCE_VOICEOVER_RED);
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_shadow",false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, false);
         _lpSceneDirector->getDiaryPtr()->beginCreatePage();
         _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_nathan4.png");
         _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_NATHAN4", FONT_DIARY_1, 0, 0);
         _lpSceneDirector->getDiaryPtr()->endCreatePage();
      }
   }
   
   
#ifdef BFG_TEASING
   _lpSceneDirector->getSequencer()->GoToScene(NULL, "teasing", "", false);
#endif
   
}

void SceneFirstfloor::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void SceneFirstfloor::Logic()
{
	EScene::Logic();
}

void SceneFirstfloor::Draw()
{
   EScene::Draw();
}

void SceneFirstfloor::Close()
{
   
}

bool SceneFirstfloor::ObjectClicked(const char *szObjectName, float x, float y)
{
   if ( strcmp(szObjectName, "gotohall") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk("ashleyhallrepaired", CHARACTER_POSX, CHARACTER_POSY, KStr("FIRSTFLOOR_ASHLEY_NOBACK") ,"",true);
      return true;
   }
   if (strcmp(szObjectName, "house_firstfloor_isaac2") == 0) {
      int x,y;
      GetObjectPosition("house_firstfloor_isaac2", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_ISAAC_WOOF"), "", true);
      ESoundBank::getSound("isaac_bark")->playSample();
      return true;
   }
   if ( strcmp(szObjectName, "envelope_firstfloor") == 0)
   {
      PickupSimple(szObjectName, "ihm_envelope");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage("letterpaper.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("ENVELOPE_FIRSTFLOOR", FONT_DIARY_2, 0, 0);
      _lpSceneDirector->getDiaryPtr()->addImageToPage("logofraternity.png");
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return true;
   }
   
   return false;
}

bool SceneFirstfloor::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

