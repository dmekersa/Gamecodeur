/*
 *  Scene_Egypt_Mazeentry.cpp
 *  enigma
 *
 *  Created by MEKERSA David on 24/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Egypt_Mazeentry.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"

/* Constructeur */
Scene_Egypt_Mazeentry::Scene_Egypt_Mazeentry(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_Egypt_Mazeentry::~Scene_Egypt_Mazeentry()
{
   delete _lpBgGraphic;
}

void Scene_Egypt_Mazeentry::Init()
{
   SetupItem("egypt_symbol_god04h");

   if (!TaskResolved("task_egypt_meetmummy")) {
      AddHint("egypt","laby","how");
      ResolveTask("task_egypt_meetmummy");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL,"ghostappear");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_mummy", true);
      int x,y;
      GetObjectPosition("egypt_mummy", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_MUMMY_RITUAL1"), "", false);      
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_MUMMY_RITUAL2"), "", false);      
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_mummy", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_NEFERES_WHATHAP"), "", true, true);
      AddObjective("egypt","laby");
   }
   
   // Porte du laby
   if (!TaskResolved("task_egypt_openmaze")) {
      SetVisible("egypt_mazedoor", true, true);
      SetVisible("egypt_maze_zone",false);
   }
   else {
      SetVisible("egypt_mazedoor", false, true);
      SetVisible("egypt_maze_zone",true);
   }
   
   // Retour après s'être perdu dans le laby
   if (getAdditionalName() == "lost") {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_NEFERES_LOST"), "", true, false);
   }
}

void Scene_Egypt_Mazeentry::Check()
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

void Scene_Egypt_Mazeentry::Logic()
{
	EScene::Logic();
}

void Scene_Egypt_Mazeentry::Draw()
{
   EScene::Draw();
}

void Scene_Egypt_Mazeentry::Close()
{
}

bool Scene_Egypt_Mazeentry::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "egypt_symbol_god04h") == 0) {
      PickupSimple(szObjectName, "inv_egypt_god04");
   }
   return false;
}

bool Scene_Egypt_Mazeentry::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Egypt_Mazeentry::ItemUsed(const char *szItemName, const char *szObjectName)
{
   
   return false;
}

void Scene_Egypt_Mazeentry::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
