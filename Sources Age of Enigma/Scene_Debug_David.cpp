/*
 *  Scene_Debug_David.cpp
 *  enigma
 *
 *  Created by Rockford on 14/06/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Debug_David.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EWatchDog.h"
#include "ThreadLoader.h"

char __scn_array[90][256] = {
   "house_out_car",  
   "house_out",
   "hell_vale",
   "parvis",
   "bathroom_tub",
   "bathroom",
   "bathroom_cabinet",
   "house_room2",
   "house_livingroom_table",
   "hall",
   "vignette_island_safe",
   "vignette_diningroom_clock",
   "vignette_diningroom_boat",
   "middleage_stairs",
   "middleage_mainscreen",
   "middleage_jail",
   "middleage_cloister",
   "kitchen",
   "japan_mainscreen",
   "japan_bedroom",
   "island_dive",
   "island_creek",
   "island_cave",
   "island_beach",
   "inca_stairs",
   "inca_passage",
   "inca_mainscreen",
   "house_room2_mirror",
   "house_livingroom",
   "house_livingroom_piano",
   "house_livingroom_box",
   "house_hall",
   "house_diningroom",
   "hell_styx",
   "hell_mainscreen",
   "firstfloor",
   "egypt_tomb",
   "egypt_tomb_connect",
   "egypt_mazeentry",
   "egypt_maze",
   "egypt_boat",
   "corridor",
   "closet",
   "basement",
   "basement_grid",
   "house_attic",
   "vignette_inca_statuette",
   "keyhole",
   "japan_garden",
   "inca_final",
   "house_room2_bedside",
   "house_atticdoor",
   "house_attic_pentacle",
   "hell_door",
   "loading",
   "vignette_hands_middleage",
   "vignette_hands_island",
   "vignette_hands_egypt",
   "vignette_hands_japan",
   "vignette_hands_inca",
   "menu",
   "mode",
   "vignette_hands",
   "inca_autel",
   "pirate",
   "minigames",
   "teasing",
   "minijeu",
   "poltergeist",
   "vortex",
   "vignette_spirit",
   "vignette_diningroom_lib",
   "vignette_cinematic",
   "title",
   "title_splashscreen",
   "halldoor",
   "egypt_land",
   "egypt_keyroom",
   "egypt_boatroom",
   "house_incabook",
   "vignette_diningroom_picture",
   "stairsdown",
   "house_livingroom_statuette",
   "egypt_keyroom_zoom",
   "island_falaise",
   "vignette_diningroom_book",
   "egypt_shop",
   "room1",
   "vortexisland",
   "test"
};

/* Constructeur */
Scene_Debug_David::Scene_Debug_David(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   EWatchDog::Disable();
   _lpCounter = new KCounter;
   _lpCounter->startCounter(0,1,0,2000,K_COUNTER_LINEAR);
}

/* Destructeur */
Scene_Debug_David::~Scene_Debug_David()
{
   delete _lpBgGraphic;
}

void Scene_Debug_David::Init()
{
}

void Scene_Debug_David::Callback(const char *szParam)
{
   int i; 
   
   if (strcmp(szParam, "pickup") == 0) {
      EInventoryItem *item = new EInventoryItem("hell","inv_basementkey",1,0);
      _lpSceneDirector->getInventory()->AddItem(item);
      return;
   }
   i = GetRandom(1, 12);
   char szName[256];
   memset(szName, 0, 256);
   sprintf(szName, "japan_table_messy[%d]", i);
   
   // Table déjà complète?
   if (_lpSceneDirector->getInventory()->getItemFromInventory("inv_japan_table")) {
      EInventoryItem *item = new EInventoryItem("house","inv_bath_flacon_blue_full",1,0);
      _lpSceneDirector->getInventory()->AddItem(item);
      _lpSceneDirector->DropItem("inv_japan_table");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_table_messy[1]", true,true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_table_messy[2]", true,true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_table_messy[3]", true,true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_table_messy[4]", true,true);
   }
   
   if (i <= 4) {
      PickupMultiple(szName, "inv_japan_tablepieces", 4);
   }
}

void Scene_Debug_David::Check()
{
   EScene::Check();
   if (_lpSceneDirector->getInventory()->getCount("hell")> 20) {
      _lpSceneDirector->DropItem("inv_japan_table");
      _lpSceneDirector->DropItem("inv_basementkey");
   }

   if (KInput::isPressed(K_VK_F5))
   {
      for (int ii=1; ii<=10; ii++) {
         EInventoryItem *item = new EInventoryItem("house","inv_bath_flacon_blue_full",1,0);
         _lpSceneDirector->getInventory()->AddItem(item);
      }
      for (int i=1; i<=2000; i++) {
         _lpSceneDirector->getSequencer()->Wait(NULL, 5);
         _lpSceneDirector->EndTravellingItem();
         _lpSceneDirector->getSequencer()->Callback(NULL, "");
         int i2;
         i2 = GetRandom(1, 12);
         if (i2>6 || _lpSceneDirector->getInventory()->getCount("hell")> 20 || _lpSceneDirector->getInventory()->getCount("house")> 20 ) {
            _lpSceneDirector->DropItem("inv_basementkey");
         }
         if (i2>9) {
            _lpSceneDirector->getSequencer()->Callback(NULL, "pickup");
         }
      }
   }
   
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   _lpCounter->move(fElapsed);
   if (_lpCounter->isCompleted()) {
      Loader::IncrIndiceScene();
      // Lance un preload aléatoire
      int i = GetRandom(0, 89);
      K_LOG("********************** MOULINETTE : Preload de %s",__scn_array[i]);
      Loader::LoadScene(__scn_array[i]);
      _lpCounter->startCounter(0,1,0,GetRandom(10, 50),K_COUNTER_LINEAR);
   }
}

void Scene_Debug_David::Logic()
{
	EScene::Logic();
}

void Scene_Debug_David::Draw()
{
   EScene::Draw();
}

void Scene_Debug_David::Close()
{
}

bool Scene_Debug_David::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "livingroom_key") == 0)
   {
      PickupSimple(szObjectName, "inv_basementkey");
      return true;
   }
   return false;
}

bool Scene_Debug_David::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}
