/*
 *  Scene_Japan_Garden.cpp
 *  enigma
 *
 *  Created by Rockford on 31/08/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Japan_Garden.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuZenGarden.h"

/* Constructeur */
Scene_Japan_Garden::Scene_Japan_Garden(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   
   EMiniJeuZenGarden::Preload();
}

/* Destructeur */
Scene_Japan_Garden::~Scene_Japan_Garden()
{
   delete _lpBgGraphic;
}

void Scene_Japan_Garden::Init()
{
   // Scintillement du sable
   if (!TaskResolved("task_japan_garden")) {
      StartEmitter("flake_garden");
   } else {
      StopEmitter("flake_garden");
      SetVisible("japan_gardenzone", false);
   }
   
   // Feu allumé
   if (TaskResolved("task_japan_garden"))
   {
      StartEmitter("fire_lamp");
   }
   else {
      StopEmitter("fire_lamp");
   }
}

void Scene_Japan_Garden::Check()
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

void Scene_Japan_Garden::Logic()
{
	EScene::Logic();
}

void Scene_Japan_Garden::Draw()
{
   EScene::Draw();
}

void Scene_Japan_Garden::Close()
{
   
}

bool Scene_Japan_Garden::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Clic dans le jardin
   if (strcmp(szObjectName,"japan_gardenzone") == 0) {
      if (!TaskResolved("task_japan_garden")) {
         // Mini jeu du jardin
         EMiniJeuBase *lpMiniJeu = new EMiniJeuZenGarden();
         _lpSceneDirector->GoToMiniGame("garden", lpMiniJeu);
      }
   }

   // Clic sur la porte fermée
   if (strcmp(szObjectName,"japan_bedroomzone") == 0) {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "store");
      SetVisible("japan_bedroomzone", false);
      SetVisible("dooropen", true);
   }
   
   return false;
}

bool Scene_Japan_Garden::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Japan_Garden::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur utilise la torche sur la lampe allumée
   if ( strcmp(szItemName, "inv_japan_torchoff") == 0 ) {
      if (strcmp(szObjectName, "lamp_zone") == 0)
      {
         if (TaskResolved("task_japan_garden")) {
            ResolveObjective("japan","torch");
            ESoundBank::getSound("carpet_burn")->playSample(); // TODO
            ESoundBank::getSound("success")->playSample();
            _lpSceneDirector->DropItem("inv_japan_torchoff");
            _lpSceneDirector->getInventory()->AddItem("japan","inv_japan_torchon");
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_NEEDFIRE"),"", true, false);
         }

         return true;
      }
   }
   
   return false;
}

void Scene_Japan_Garden::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "garden")==0 && bIsRevolved) {
      // Fait apparaitre la flamme
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      ESoundBank::getSound("reveal")->playSample();
      ResolveTask("task_japan_garden");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_FLAME"),"", true, false);
      Init();
   }   
}