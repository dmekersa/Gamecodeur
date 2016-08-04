/*
 *  Scene_House_Atticdoor.cpp
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Atticdoor.h"
#include "ESceneDirector.h"
#include "EMiniJeuPentagram.h"

/* Constructeur */
SceneAtticDoor::SceneAtticDoor(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   EMiniJeuPentagram::Preload();
}

/* Destructeur */
SceneAtticDoor::~SceneAtticDoor()
{
   delete _lpBgGraphic;
}

void SceneAtticDoor::Init()
{
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_HOUSE);
   
   SetupItem("envelope_attic");
   
   SetVisible("house_room2_isaac_quiet", true, true);
   
   // 1ère visite
   if (!TaskResolved("task_house_atticdoorvisit")) {
      ResolveTask("task_house_atticdoorvisit");
      AddObjective("house","atticdoor");
      AddHint("house","atticdoor","how");
      AddHint("house","atticdoor","pentagram");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_atticdoor.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_ATTICDOOR", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
   }
   
   // Etat de la porte
   if (TaskResolved("task_house_openatticdoor")) {
      SetVisible("house_atticdoor_door_open", true, true);
   }
   else if (TaskResolved("task_house_activeatticdoor")) {
      SetVisible("house_atticdoor_door_on", true, true);
      // Pentacle la tête en bas !
      SetVisible("house_atticdoor_pentacle_on", true, false);
   }
   else {
      SetVisible("house_atticdoor_door_off", true, true);
   }
}

void SceneAtticDoor::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      if (!TaskResolved("__atticdoortest__")) {
         ResolveTask("__atticdoortest__");
         _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_bathroom_chalk", 500, 500, 1);
         _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_room2_flask", 500, 500, 1);
      }
   }
   if (KInput::isPressed(K_VK_F7)) {
      if (!TaskResolved("__rotatetest__")) {
         ResolveTask("__rotatetest__");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_atticdoor_pentacle_on", true);
         _lpSceneDirector->getSequencer()->Animation(NULL, "pentacle_anim", true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 3000);
         _lpSceneDirector->getSequencer()->Animation(NULL, "pentacle_anim", false);
         //      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_atticdoor_pentacle_on", false);
      }
   }
#endif
}

void SceneAtticDoor::Logic()
{
	EScene::Logic();
}

void SceneAtticDoor::Draw()
{
   EScene::Draw();
}

void SceneAtticDoor::Close()
{
   
}

bool SceneAtticDoor::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "house_atticdoor_door_off") == 0) {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "doorclosed");
      return true;
   }
   
   if ( strcmp(szObjectName, "envelope_attic") == 0)
   {
      PickupSimple(szObjectName, "ihm_envelope");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage("letterpaper.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("ENVELOPE_ATTIC", FONT_DIARY_2, 0, 0);
      _lpSceneDirector->getDiaryPtr()->addImageToPage("logofraternity.png");
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return true;
   }
   
   if ( strcmp(szObjectName, "house_room2_isaac_quiet") == 0)
   {
      int x,y;
      GetObjectPosition("house_room2_isaac_quiet", x, y, true, false);
      ESoundBank::getSound("isaac_bark")->playSample();
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_ISAAC_WOOF"), "", true);
   }
   
   return false;
}

bool SceneAtticDoor::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool SceneAtticDoor::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (strcmp(szItemName, "inv_bathroom_chalk") == 0) {
      if (strcmp(szObjectName, "house_atticdoor_door_off") == 0) {
         EMiniJeuBase *lpMiniJeu = new EMiniJeuPentagram();
         _lpSceneDirector->getSequencer()->MiniGame(NULL, "penta", lpMiniJeu);
         return true;
      }
   }

   if (strcmp(szItemName, "inv_room2_flask") == 0) {
      if (strcmp(szObjectName, "house_atticdoor_door_on") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"inca_eclipse");
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_house_openatticdoor");
         ResolveObjective("house","atticdoor");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_atticdoor_pentacle_goat", true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 1500);
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_atticdoor_pentacle_goat", false);
         _lpSceneDirector->getSequencer()->Animation(NULL, "pentacle_anim", true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 3000);
         _lpSceneDirector->getSequencer()->Animation(NULL, "pentacle_anim", false);
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", false);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_atticdoor_pentacle_on", false);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "opendoor");      
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_atticdoor_door_off", false);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_atticdoor_door_draw", false);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_atticdoor_door_on" , "house_atticdoor_door_open");
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         return true;
      }
   }
   return false;
}

void SceneAtticDoor::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (bIsRevolved) {
      _lpSceneDirector->DropItem("inv_bathroom_chalk");
      ResolveTask("task_house_activeatticdoor");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL,"carpet_burn");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_atticdoor_door_draw", true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_atticdoor_pentacle_on", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_atticdoor_door_on", true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      return;
   }
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
