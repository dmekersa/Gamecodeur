/*
 *  SceneHall.cpp
 *  enigma
 *
 *  Created by Rockford on 28/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_House_Hall.h"
#include "ESceneDirector.h"
#include "MyGame.h"
#include "GlobalBank.h"
#include "SoundBank.h"
#include "EMiniJeuConnect.h"

Scene_House_Hall::Scene_House_Hall(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   EMiniJeuConnect::Preload();
}

Scene_House_Hall::~Scene_House_Hall()
{
}

void Scene_House_Hall::Callback(const char *szParam)
{
   ResolveTask("task_gethexagramme");
   _lpSceneDirector->ShowIHM(true);
}

void Scene_House_Hall::Init()
{
   SetupItem("house_score");

   _lpSceneDirector->getMap()->renameZone("hall", "house_hall");
   _lpSceneDirector->getMap()->setVisited("house_hall");
      
   if (getAdditionalName() == "start") {
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
      int x, y;
      GetObjectPosition("nathan_shadow", x, y, true, false);
      ResolveTask("task_house_openliving");
      SetVisible("nathan_shadow",true, true);
      StartEmitter("creeping_nathan_shadow");
      _lpSceneDirector->getSequencer()->EarthQuake("earthquakehall",false,"hall_earthquake");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_fragments", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_plaster", false);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "hall_earthquakeout");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan9", "nathan14");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan14", "nathan15");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan15", "hall_isaac_waiting");

//      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
//      _lpSceneDirector->getSequencer()->PickupItem(NULL,"inv_hexagramme", 1024/2, 768/2, 1);
//      _lpSceneDirector->getSequencer()->ShowImage(NULL, "neckless", true, false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_shadow",false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_show",true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"nathan_show",true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"nathan_shadow",false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_STARTHERE"), "nathan_hall_starthere", false);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "opendoor");
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"house_hall_livingdooropen", true, true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 500);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls1",true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostgone");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "nathan_show", false, false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_show",false);
      _lpSceneDirector->getSequencer()->Callback(NULL,"inv_hexagramme");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);

      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_hall.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_HALL1", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_HALL2", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_medaillon.png");
      _lpSceneDirector->getDiaryPtr()->endCreatePage();

      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, false);

      AddObjective("house","contactyumanco");
      AddHint("house","contactyumanco","where");
   }
   
   // Plante
   if (!TaskResolved("task_house_getbrushwood")) {
      SetVisible("house_hall_plant", true, true);
   }
   else {
      SetVisible("house_hall_plant_done", true, true);
   }
   
   // Isaac
   if (!TaskResolved("task_house_hall_groundfloor") && getAdditionalName()=="") {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "hall_dog_cry");
      SetVisible("hall_isaac_waiting", true, true);
   }

   // Porte du salon
   if (TaskResolved("task_house_openliving") && getAdditionalName() == "") {
      SetVisible("house_hall_livingdooropen", true, true);
   }
   
   // Porte de la salle à manger ouverte ou fermée ?
   if (ObjectiveResolved("house","helpnatsumi")) {
      SetVisible("diningroom", false);
      SetVisible("house_hall_diningdooropen", true, true);
   }
   else {
      SetVisible("diningroom", true);
      SetVisible("house_hall_diningdooropen", false);
   }

   // Etage résolu
   if (TaskResolved("task_house_hall_groundfloor")) {
      ResolveGroundFloor();
   }
   
   // Premier retour dans le hall après avoir sauvé l'inca
   if (TaskResolved("task_inca_universeresolved") && !TaskResolved("task_meetnathan2")) {
      int x, y;
      GetObjectPosition("nathan_corridor", x, y, true, false);
      ResolveTask("task_meetnathan2");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls2",true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_corridor",true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "attic_nathanappear");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "nathan_corridor", true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls2",false);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1500);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET2_1"), "HALL_NATHAN_MEET2_1", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_MEET2"),"hall_ashley_meet2",false,true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls2",true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET2_3"), "HALL_NATHAN_MEET2_3", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls2",true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostgone");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_corridor",false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "nathan_corridor", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_hall.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_NATHAN2", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
   }
   else {
      if (!TaskResolved("hall_tuto_guide") && getAdditionalName() != "start") {
         ResolveTask("hall_tuto_guide");
         if (TestGlobal("__tutorial__")) {
            _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("HOUSE_OUT_TUTO_TASK"), 70, 740, -90, 200);
         }
      }
   }
}

void Scene_House_Hall::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      ResetTask("task_meetnathan2");
      ResolveTask("task_inca_universeresolved");
      Init();
   }
   if (KInput::isPressed(K_VK_F6))
   {
      SetVisible("house_hall_livingdooropen", true, true);
   }
   if (KInput::isPressed(K_VK_F7))
   {
      SetVisible("diningroom", false);
      SetVisible("house_hall_diningdooropen", true);
   }
   if (KInput::isPressed(K_VK_F8))
   {
      ResolveGroundFloor();
   }
#endif
}

void Scene_House_Hall::Logic()
{
	EScene::Logic();
}

void Scene_House_Hall::Draw()
{
   EScene::Draw();
}

void Scene_House_Hall::Close()
{
   
}

bool Scene_House_Hall::AskForClose()
{
   return true;
}

bool Scene_House_Hall::ObjectClicked(const char *szObjectName, float x, float y)
{
   /* Peintures au mur */
   if ( strcmp(szObjectName, "painting1") == 0 )
   {
      _lpSceneDirector->getSequencer()->Talk("ashleyhallpainting1", CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_ARTCRITIC"),"", true);
   }
   // La plante
   else if (strcmp(szObjectName, "house_hall_plant") == 0) {
      _lpSceneDirector->getSequencer()->VoiceOver("ashleyhall1", KStr("HALL_ASHLEY_PLANT"),"");
   }
   // La partition
   else if (strcmp(szObjectName, "house_score") == 0) {
      PickupSimple("house_score", "inv_house_score");
   }
   else if (strcmp(szObjectName, "upstairs") == 0)
   {
      _lpSceneDirector->getSequencer()->VoiceOver("ashleystuck", KStr("HALL_ASHLEY_STAIRSBROKEN"), "");
      /* On prend la main, on ne laisse pas passer */
      return true;
   }
   else if ( strcmp(szObjectName, "diningroom") == 0)
   {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "doorclosed");
      _lpSceneDirector->getSequencer()->VoiceOver("doorclosed", KStr("GLOBAL_DOORCLOSED"), "");
      return true;
   }
   else if ( strcmp(szObjectName, "corridor") == 0)
   {
      if (TestGlobal("__mode_casual__")) {
         if (!TaskResolved("task_living_discover")) {
            _lpSceneDirector->getSequencer()->Talk("ashley", CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_LIVINGFIRST"),"", true);
            return true;
         }
         else {
            return false;
         }
      }
   }
   else if ( strcmp(szObjectName, "parvis") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk("ashleystuck", CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_STUCK"),"", true);
      /* On prend la main, on ne laisse pas passer */
      return true;
   }
   
   return false;
}

bool Scene_House_Hall::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Scene_House_Hall::ItemUsed(const char *szItemName, const char *szObjectName)
{
   int x,y;
   
   // Le joueur coupe des brindilles avec le couteau
   if (strcmp(szItemName, "inv_house_knife") == 0) {
      if (strcmp(szObjectName, "house_hall_plant") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "mg_cut");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->DropItem(szItemName);
         GetObjectPosition("house_hall_plant", x, y, true, true);
         _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_house_brushwood", x, y, -1);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_hall_plant", "house_hall_plant_done");
         ResolveTask("task_house_getbrushwood");
         return true;
      }
   }
   
   // Le joueur utilise L'échelle
   if ( strcmp(szItemName, "inv_house_ropehook") == 0 &&
       ( strcmp(szObjectName, "hall_isaac_waiting") == 0) ) {
      ResolveTask("task_house_useropeladder");
      ESoundBank::getSound("success")->playSample();
      ResolveGroundFloor();
      // On n'a plus besoin de la corde
      _lpSceneDirector->DropItem(szItemName);
      return true;
   }

   // Le joueur utilise la corde ou le grappin seul
   if ( (strcmp(szItemName, "inv_house_rope") == 0 || strcmp(szItemName, "inv_house_hook") == 0) &&
       ( strcmp(szObjectName, "hall_isaac_waiting") == 0) ) {
      _lpSceneDirector->getSequencer()->Talk("incomplete", CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_INCOMPLETEROPE") ,"",true);
      return true;
   }
   return false;
}

void Scene_House_Hall::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
}

void Scene_House_Hall::ResolveGroundFloor()
{
   ResolveTask("task_house_hall_groundfloor");
   _lpSceneDirector->getSequencer()->ShowImage(NULL, "hall_rope", true);
   _lpSceneDirector->getSequencer()->Talk("ashleyhallrepaired", CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_GROUNDFLOORRESOLVED") ,"",true);
}
