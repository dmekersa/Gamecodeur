/*
 *  Scene_House_Out_Car.cpp
 *  enigma
 *
 *  Created by Rockford on 23/03/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Out_Car.h"
#include "ESceneDirector.h"
#include "EMiniJeuPentagram.h"
#include "MusicBank.h"

/* Constructeur */
Scene_House_Out_Car::Scene_House_Out_Car(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
  
   EMiniJeuPentagram::Preload();
}

/* Destructeur */
Scene_House_Out_Car::~Scene_House_Out_Car()
{
   delete _lpBgGraphic;
}

void Scene_House_Out_Car::Init()
{   
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE, true);
   if (!TaskResolved("task_house_opencarbox") && !TaskToResolve("task_house_opencarbox")) {
      AddTask("task_house_opencarbox");
   }
   
   if (TaskResolved("task_house_opencarbox")) {
      SetVisible("car_box_open", true, true);
      SetVisible("car_box_closed", false, true);
      SetupItem("car_box_map");
      SetupItem("car_box_key");
      if (isVisible("car_map") && TestGlobal("__tutorial__")) {
         _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("HOUSE_OUT_TUTO_TAKEMAP"), 765, 500, -90, 200);
      }
   }
   else {
      SetVisible("car_box_open", false, true);
      SetVisible("car_box_closed", true, true);
      if (_lpSceneDirector->getInventory()->getItemFromInventory("inv_house_carkey") && TestGlobal("__tutorial__")) {
         _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("HOUSE_OUT_TUTO_CARKEYOK"), 346, 690, -90, 40);
         _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("HOUSE_OUT_TUTO_USECARKEY"), 613, 353, -45, 200);
      }
   }

   
   SetupItem("car_key");
   
   if (isVisible("car_key") && TestGlobal("__tutorial__")) {
      _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("HOUSE_OUT_TUTO_CARKEY"), 505, 300, -45, 200);         
   }
   
   if ( ObjectiveResolved("house","getmap") ) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED"), "", true);
      return;
   }
}

void Scene_House_Out_Car::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_House_Out_Car::Logic()
{
	EScene::Logic();
}

void Scene_House_Out_Car::Draw()
{
   EScene::Draw();
}

void Scene_House_Out_Car::Close()
{
}

bool Scene_House_Out_Car::ObjectClicked(const char *szObjectName, float x, float y)
{
   if ( strcmp(szObjectName, "car_key") == 0)
   {
      PickupSimple("car_key", "inv_house_carkey");
      if (TestGlobal("__tutorial__")) {
         _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("HOUSE_OUT_TUTO_CARKEYOK"), 346, 690, -90, 40);
         _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("HOUSE_OUT_TUTO_USECARKEY"), 613, 353, -45, 200);
      }
      return true;
   }   
   if ( strcmp(szObjectName, "car_box_map") == 0)
   {
      ResolveTask("task_out_map");
      PickupSimple(szObjectName, "inv_map");
      if (TaskResolved("task_out_map") && TaskResolved("task_out_key")) {
         ResolveObjective("house","getmap");
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_out", "gotohouse");
      }
      return true;
   }   
   if ( strcmp(szObjectName, "car_box_key") == 0)
   {
      ResolveTask("task_out_key");
      PickupSimple(szObjectName, "inv_key");
      if (TaskResolved("task_out_map") && TaskResolved("task_out_key")) {
         ResolveObjective("house","getmap");
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_out", "gotohouse");
      }
      return true;
   }   
   if ( strcmp(szObjectName, "horn") == 0)
   {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "horn");
      return true;
   }   
   
   return false;
}

bool Scene_House_Out_Car::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_House_Out_Car::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (strcmp(szItemName, "inv_house_carkey") == 0) {
      if (strcmp(szObjectName, "car_box_closed") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_house_opencarbox");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "carboxclose");
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "car_box_closed", "car_box_open");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "car_box_map", true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "car_box_key", true);
//         _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("HOUSE_OUT_TUTO_TAKEMAP"), 765, 502, -90, 200);
         return true;
      }
   }
   return false;
}

void Scene_House_Out_Car::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (bIsRevolved) {
      return;
   }
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_ASHLEY_GASP"), "", true, false);
      return;
   }
}
