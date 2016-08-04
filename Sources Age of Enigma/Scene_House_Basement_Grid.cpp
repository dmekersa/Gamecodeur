/*
 *  Scene_House_Basement_Grid.cpp
 *  enigma
 *
 *  Created by Rockford on 24/08/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Basement_Grid.h"
#include "ESceneDirector.h"
#include "SoundBank.h"
#include "GlobalBank.h"
#include "EMiniJeuUnscrew.h"

/* Constructeur */
Scene_House_Basement_Grid::Scene_House_Basement_Grid(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   EMiniJeuUnscrew::Preload();
}

/* Destructeur */
Scene_House_Basement_Grid::~Scene_House_Basement_Grid()
{
}

void Scene_House_Basement_Grid::Init()
{
   // 1ère visite de la grille
   if (!TaskResolved("task_house_basement_visitgrid")) {
      ResolveTask("task_house_basement_visitgrid");
      AddObjective("house","basementgrid");
      AddHint("house","basementgrid","how");
   }
   
   // Grille ouverte ?
   if (TaskResolved("task_house_basement_opengrid")) {
      SetVisible("basement_grid_gridclosed", false, true);
      SetVisible("basement_grid_gridopen", true, true);
      // La vis
      SetVisible("basement_grid_bolt", TestGlobal("basement_grid_bolt") == false, true);
   } else {
      SetVisible("basement_grid_gridclosed", true, true);
      SetVisible("basement_grid_gridopen", false, true);
   }

   // Le charbon
   if (TaskResolved("task_house_basement_reversecoal")) {
      SetVisible("basement_grid_coal", true, true);
      // Le fusain
      SetVisible("basement_grid_charcoal", TestGlobal("basement_grid_charcoal") == false, true);
   }
}

void Scene_House_Basement_Grid::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_House_Basement_Grid::Logic()
{
	EScene::Logic();
}

void Scene_House_Basement_Grid::Draw()
{
   EScene::Draw();
//   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
}

void Scene_House_Basement_Grid::Close()
{
   
}

bool Scene_House_Basement_Grid::ObjectClicked(const char *szObjectName, float x, float y)
{
   if ( strcmp(szObjectName, "basement_grid_bolt") == 0)
   {
      PickupMultiple(szObjectName, "inv_house_basement_bolt",-1);
      return true;
   }
   if ( strcmp(szObjectName, "basement_grid_charcoal") == 0)
   {
      PickupSimple(szObjectName, "inv_house_basement_grid_charcoal");
      return true;
   }
   return false;
}

bool Scene_House_Basement_Grid::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_House_Basement_Grid::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur utilise la clé sur la grille
   if ( strcmp(szItemName, "inv_house_basement_screwdriver") == 0 ) {
      if (strcmp(szObjectName, "basement_grid_gridclosed") == 0)
      {
         EMiniJeuBase *lpMiniJeu = new EMiniJeuUnscrew();
         _lpSceneDirector->GoToMiniGame("grid", lpMiniJeu);
         return true;
      }
   }
   return false;
}

void Scene_House_Basement_Grid::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "grid")==0 && bIsRevolved) {
      ResolveObjective("house","basementgrid");
      ResolveTask("task_house_basement_opengrid");
      ESoundBank::getSound("reveal")->playSample();
      // On peut maintenant retirer l'objet de l'inventaire
      _lpSceneDirector->DropItem("inv_house_basement_screwdriver");
      Init();
   }
}
