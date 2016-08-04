/*
 *  SceneCorridor.cpp
 *  enigma
 *
 *  Created by Rockford on 29/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "SceneCorridor.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"

/* Constructeur */
SceneCorridor::SceneCorridor(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
SceneCorridor::~SceneCorridor()
{
   delete _lpBgGraphic;
}

void SceneCorridor::Init()
{
   if (!TaskResolved("task_house_corridorvisit")) {
      ResolveTask("task_house_corridorvisit");
   }
   
   // Tiroir
   if (!TaskResolved("task_house_corridordrawer")) {
      SetVisible("house_corridor_drawer_closed", true, true);
   }
   else {
      SetVisible("house_corridor_drawer_open", true, true);
      SetupItem("house_zippo_empty");
   }
   
   // Tableau
   SetupItem("house_cordon");
   if (!TaskResolved("task_house_fallpainting")) {
      SetVisible("house_corridor_painting", true, true);
   }
   else {
      SetVisible("house_corridor_painting_down", true, true);
   }

   // Autres objets
   SetupItem("house_statuette_part_head2");
   SetupItem("envelope_corridor");
}

void SceneCorridor::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void SceneCorridor::Logic()
{
	EScene::Logic();
}

void SceneCorridor::Draw()
{
   EScene::Draw();
}

void SceneCorridor::Close()
{
   
}

bool SceneCorridor::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Tableau au mur
   if (strcmp(szObjectName, "house_corridor_painting") == 0) {
      ResolveTask("task_house_fallpainting");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "drawer_close");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_corridor_painting", "house_corridor_painting_down");
//      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_cordon", true);
   }

   // Tiroir
   if (strcmp(szObjectName, "house_corridor_drawer_closed") == 0) {
      ResolveTask("task_house_corridordrawer");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "drawer_open");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_corridor_drawer_closed", "house_corridor_drawer_open");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_zippo_empty", true);
   }
   
   // Cordon
   if (strcmp(szObjectName, "house_cordon") == 0) {
      PickupMultiple(szObjectName, "inv_house_cordon",-1);
   }

   // Briquet
   if (strcmp(szObjectName, "house_zippo_empty") == 0) {
      PickupMultiple(szObjectName, "inv_house_zippo_empty",-1);
   }
   
   // Morceau de tete
   if (strcmp(szObjectName, "house_statuette_part_head2") == 0) {
      PickupSimple("house_statuette_part_head2", "inv_house_statuette_part_head2");
   }
   
   if ( strcmp(szObjectName, "envelope_corridor") == 0)
   {
      PickupSimple(szObjectName, "ihm_envelope");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage("letterpaper.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("ENVELOPE_CORRIDOR", FONT_DIARY_2, 0, 0);
      _lpSceneDirector->getDiaryPtr()->addImageToPage("logofraternity.png");
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return true;
   }
   
   // Porte de la cave
   if (strcmp(szObjectName, "stairsdown") == 0) {
      // Si on a ouvert la porte, on ne va plus au trou de serrure mais directement dans les escaliers
      if (TaskResolved("task_house_basement_opendoor")) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "opendoor");
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "stairsdown");
         return true;
      }
   }
   return false;
}

bool SceneCorridor::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool SceneCorridor::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if ( strcmp(szItemName, "inv_basementkey") == 0 && strcmp(szObjectName, "stairsdown") == 0 ) {
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "keyhole");
      return true;
   }
   return false;
}