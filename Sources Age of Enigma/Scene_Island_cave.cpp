/*
 *  Scene_Island_cave.cpp
 *  enigma
 *
 *  Created by Rockford on 01/06/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Island_cave.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuMikado.h"

/* Constructeur */
Scene_Island_Cave::Scene_Island_Cave(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   
   EMiniJeuMikado::Preload();
}

/* Destructeur */
Scene_Island_Cave::~Scene_Island_Cave()
{
   delete _lpBgGraphic;
}

void Scene_Island_Cave::Init()
{
   if (!TaskResolved("task_island_removebranches")) {
      SetVisible("island_cave_wood1", true);
      SetVisible("island_cave_wood2", false);
   }
   else {
      SetVisible("island_cave_wood1", false);
      SetVisible("island_cave_wood2", true);
      SetupItem("island_shovelhandle");
   }

   // Est-ce qu'on a tout terminé ici ?
   if (ItemCollected("island_shovelhandle")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED"), "", true);
   }
   
}

void Scene_Island_Cave::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_Island_Cave::Logic()
{
	EScene::Logic();
}

void Scene_Island_Cave::Draw()
{
   EScene::Draw();
}

void Scene_Island_Cave::Close()
{
   
}

bool Scene_Island_Cave::ObjectClicked(const char *szObjectName, float x, float y)
{
   if ( strcmp(szObjectName, "island_cave_wood1") == 0)
   {
      EMiniJeuBase *lpMiniJeu = new EMiniJeuMikado();
      _lpSceneDirector->GoToMiniGame("mikado", lpMiniJeu);
      return true;
   }
   if ( strcmp(szObjectName, "island_shovelhandle") == 0)
   {
      PickupMultiple(szObjectName, "inv_island_shovelhandle",-1);
      return true;
   }
   return false;
}

bool Scene_Island_Cave::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

void Scene_Island_Cave::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "mikado")==0 && bIsRevolved) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"island_cave_wood1", false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"island_cave_wood2", true);
      ResolveTask("task_island_removebranches");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
     _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "island_shovelhandle", true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_CAVE_MURRAY_MGDONE"), "", false);
      
   }
   else {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FAILED"), "", false);
   }

}
