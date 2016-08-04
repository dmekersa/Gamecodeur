/*
 *  Scene_House_Attic_Pentacle.cpp
 *  enigma
 *
 *  Created by Rockford on 04/01/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Attic_Pentacle.h"
#include "ESceneDirector.h"
#include "EMiniJeuDemelage.h"

/* Constructeur */
Scene_House_Attic_Pentacle::Scene_House_Attic_Pentacle(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   EMiniJeuDemelage::Preload();
}

/* Destructeur */
Scene_House_Attic_Pentacle::~Scene_House_Attic_Pentacle()
{
   delete _lpBgGraphic;
}

void Scene_House_Attic_Pentacle::Init()
{
   // Pentacle visible au sol
   if (TaskResolved("task_house_attic_untangle")) {
      SetVisible("house_attic_pentacle_hexa", true, true);
   }
   
}

void Scene_House_Attic_Pentacle::Check()
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

void Scene_House_Attic_Pentacle::Logic()
{
	EScene::Logic();
}

void Scene_House_Attic_Pentacle::Draw()
{
   EScene::Draw();
}

void Scene_House_Attic_Pentacle::Close()
{
}

bool Scene_House_Attic_Pentacle::ObjectClicked(const char *szObjectName, float x, float y)
{

   return false;
}

bool Scene_House_Attic_Pentacle::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_House_Attic_Pentacle::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (strcmp(szItemName, "inv_hexagramme_final") == 0) {
      if (strcmp(szObjectName, "house_attic_pentacle_zone") == 0) {
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_attic_pentacle_hexa", true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         EMiniJeuBase *lpMiniJeu = new EMiniJeuDemelage();
         _lpSceneDirector->getSequencer()->MiniGame(NULL, "untangle", lpMiniJeu);
         return true;
      }
   }
   return false;
}

void Scene_House_Attic_Pentacle::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "untangle") == 0 && bIsRevolved) {
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      ResolveTask("task_house_attic_untangle");
      ResolveObjective("house","atticritual");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "inca_eclipse");
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_attic", "gotohell", false);
      return;
   }
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_attic_pentacle_hexa", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FAILED"), "", true, false);
      return;
   }
}
