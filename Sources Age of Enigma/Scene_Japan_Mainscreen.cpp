/*
 *  Scene_Japan_Mainscreen.cpp
 *  enigma
 *
 *  Created by Rockford on 30/08/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Japan_Mainscreen.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuMasterMind.h"
#include "MyGame.h"
#include "ThreadLoader.h"

/* Constructeur */
Scene_Japan_Mainscreen::Scene_Japan_Mainscreen(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
}

/* Destructeur */
Scene_Japan_Mainscreen::~Scene_Japan_Mainscreen()
{
}

void Scene_Japan_Mainscreen::Init()
{   
   // Inventaire de début d'univers
   if (!TestGlobal("japan_ashleytalkintro")) {
      SetGlobal("japan_ashleytalkintro","1");

   _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_DIALOGBOX_INTRO1"), "", false, true);
   _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_DIALOGBOX_INTRO2"), "", false, true);
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      
      AddObjective("japan","findbedroom");
      AddHint("japan","findbedroom","where");
   }
   
   // Lampes
   if (TaskResolved("task_japan_lamp1")) {
      StartEmitter("fire_lamp1");
   } else {
      StopEmitter("fire_lamp1");
   }
   if (TaskResolved("task_japan_lamp2")) {
      StartEmitter("fire_lamp2");
   } else {
      StopEmitter("fire_lamp2");
   }
   if (TaskResolved("task_japan_lamp3")) {
      StartEmitter("fire_lamp3");
   } else {
      StopEmitter("fire_lamp3");
   }   
   
   if (!TaskResolved("task_japan_revealbank"))
   {
      AddTask("task_japan_revealbank");
   }
   
   // Banc
   // On a allumé correctement les 3 lampes et donc activé le banc
   if (TaskResolved("task_japan_revealbank")) {
      if (TaskResolved("task_japan_resolvebank")) {
         StopEmitter("bank_flake");
         SetVisible("bankzone", false);
         // Affichage de la lampe sur le banc
         SetVisible("japan_mainscreen_candlelamp", TestGlobal("japan_mainscreen_candlelamp") == false);
      } else {
         StartEmitter("bank_flake");
         SetVisible("bankzone", true);
         EMiniJeuMasterMind::Preload();
      }
   }
   // Pétales
   SetVisible("japan_leaves_messy", TestGlobal("japan_leaves_messy") == false);
   
   if (TaskResolved("task_japan_revealatsuhito")) {
      SetVisible("japan_atsuhito", true);
   }   
   
   if (getAdditionalName() == "completed") {
      int x,y;
      GetObjectPosition("japan_atsuhito", x, y, true, false);
      ResolveTask("task_japan_revealatsuhito");
      Loader::LoadScene("basement");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_atsuhito", true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("JAPAN_ATSUHITO_EPILOG1"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("JAPAN_ATSUHITO_EPILOG2"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("JAPAN_ATSUHITO_EPILOG3"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("JAPAN_ATSUHITO_EPILOG4"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_EPILOG1"),"", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("JAPAN_ATSUHITO_EPILOG5"), "", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      ReturnToHouse();
   }
   
}

void Scene_Japan_Mainscreen::Check()
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

void Scene_Japan_Mainscreen::Logic()
{
	EScene::Logic();
}

void Scene_Japan_Mainscreen::Draw()
{

   EScene::Draw();
   
}

void Scene_Japan_Mainscreen::Close()
{
   
}

bool Scene_Japan_Mainscreen::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Pierre de sagesse
   if (strcmp(szObjectName, "japan_atsuhito") == 0)
   {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
      ResolveTask("task_japan_getstone");
      ReturnToHouse();
   }
   
   // Pétales au sol
   if ( strcmp(szObjectName, "japan_leaves_messy") == 0)
   {
      PickupSimple(szObjectName, "inv_japan_leaves");
      SetVisible("japan_leaves_messy", false);
      return true;
   }
   
   // Lampion sur le banc
   if ( strcmp(szObjectName, "japan_mainscreen_candlelamp") == 0)
   {
      PickupSimple(szObjectName, "inv_japan_candlelamp");
      RemoveHint("japan","candlelamp","where");
      return true;
   }
   
   // Le banc
   if (strcmp(szObjectName,"bankzone") == 0) {
      if (TaskResolved("task_japan_revealbank")) {
         // Mini jeu du banc
         EMiniJeuBase *lpMiniJeu = new EMiniJeuMasterMind();
         _lpSceneDirector->GoToMiniGame("bank", lpMiniJeu);
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_MAINSCREEN_BANK"),"", true, false);
      }
   }
   return false;
}

bool Scene_Japan_Mainscreen::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Japan_Mainscreen::ItemUsed(const char *szItemName, const char *szObjectName)
{
   int nLastLamp = 0;
   // Torche sur lampe 1
   if ( strcmp(szItemName, "inv_japan_torchon") == 0) {
      if (strcmp(szObjectName, "japan_mainscreen_lamp1_zone") == 0 && !TaskResolved("task_japan_lamp1") )
      {
         ESoundBank::getSound("carpet_burn")->playSample(); // TODO
         ESoundBank::getSound("success")->playSample();
         StartEmitter("fire_lamp1");
         ResolveTask("task_japan_lamp1");
         nLastLamp = 1;
      }
   }
   // Torche sur lampe 2
   if ( strcmp(szItemName, "inv_japan_torchon") == 0 ) {
      if (strcmp(szObjectName, "japan_mainscreen_lamp2") == 0 && !TaskResolved("task_japan_lamp2") )
      {
         ESoundBank::getSound("carpet_burn")->playSample(); // TODO
         ESoundBank::getSound("success")->playSample();
         StartEmitter("fire_lamp2");
         ResolveTask("task_japan_lamp2");
         nLastLamp = 2;
      }
   }
   // Torche sur lampe 3
   if ( strcmp(szItemName, "inv_japan_torchon") == 0 ) {
      if (strcmp(szObjectName, "japan_mainscreen_lamp3_zone") == 0 && !TaskResolved("task_japan_lamp3") )
      {
         ESoundBank::getSound("carpet_burn")->playSample(); // TODO
         ESoundBank::getSound("success")->playSample();
         StartEmitter("fire_lamp3");
         ResolveTask("task_japan_lamp3");
         nLastLamp = 3;
      }
   }
   // Vérifie que les lampes ont été allumées dans l'ordre
   if (nLastLamp == 3) {
      if (TaskResolved("task_japan_lamp1") && TaskResolved("task_japan_lamp2")) {
         // Réussi !!
         ESoundBank::getSound("reveal")->playSample();
         // Active le mini jeu du banc
         ResolveTask("task_japan_revealbank");
         RemoveHint("japan","candlelamp","lamps");
         Init();
         return true;
      } else {
         // Son
         ESoundBank::getSound("ghostgone")->playSample();
         SetGlobal("task_japan_lamp1", "0");
         SetGlobal("task_japan_lamp2", "0");
         SetGlobal("task_japan_lamp3", "0");
         Init();
         return true;
      }
   }
   // Pas bon !
   if (nLastLamp == 2) {
      if (!TaskResolved("task_japan_lamp1"))
      {
         // Annulé
         SetGlobal("task_japan_lamp2", "0");
         return true;
      }
      else if (TaskResolved("task_japan_lamp1") && TaskResolved("task_japan_lamp3")) {
         // Son
         ESoundBank::getSound("ghostgone")->playSample();
         SetGlobal("task_japan_lamp1", "0");
         SetGlobal("task_japan_lamp2", "0");
         SetGlobal("task_japan_lamp3", "0");
         Init();
         return true;
      }
   }
   if (nLastLamp == 1) {
      if (TaskResolved("task_japan_lamp2"))
      {
         // Annulé
         SetGlobal("task_japan_lamp1", "0");
         return true;
      }
      if (TaskResolved("task_japan_lamp2") && TaskResolved("task_japan_lamp3")) {
         // Son
         ESoundBank::getSound("ghostgone")->playSample();
         SetGlobal("task_japan_lamp1", "0");
         SetGlobal("task_japan_lamp2", "0");
         SetGlobal("task_japan_lamp3", "0");
         Init();
         return true;
      }
   }
   
   if (nLastLamp == 0) {
      return false;
   } else {
      return true;
   }

}

void Scene_Japan_Mainscreen::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "bank")==0 && bIsRevolved) {
      // Fait apparaitre le lampion
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      ESoundBank::getSound("reveal")->playSample();
      ResolveTask("task_japan_resolvebank");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_MAINSCREEN_MGDONE"),"", true, false);
      Init();
   }   
}

void Scene_Japan_Mainscreen::ReturnToHouse()
{
   ResolveTask("task_japan_universeresolved");
  // _lpSceneDirector->getMap()->setUniversVisited("UniverseJapan");
   // Retour
   Loader::LoadScene("basement");
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE);
   _lpSceneDirector->getSequencer()->ShowImage(NULL, "blank", true, true);
   _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/soul_tunnel_r.ogv","tunnel_r");
   _lpSceneDirector->getSequencer()->GoToScene(NULL,"basement","backfrombeyond", false);
}
