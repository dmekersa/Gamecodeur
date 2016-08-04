/*
 *  Scene_Inca_Autel.cpp
 *  enigma
 *
 *  Created by MEKERSA David on 17/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Inca_Autel.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuTemplate.h"

/* Constructeur */
Scene_Inca_Autel::Scene_Inca_Autel(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   EMiniJeuTemplate::Preload();
}

/* Destructeur */
Scene_Inca_Autel::~Scene_Inca_Autel()
{
}

void Scene_Inca_Autel::Init()
{
   // Artefact déjà posés
   if (TaskResolved("task_inca_placeartefact1")) {
      SetVisible("inca_autel_artefact1", true, true);
      SetVisible("inca_autel_artefact3zone", false, true);
   }
   if (TaskResolved("task_inca_placeartefact2")) {
      SetVisible("inca_autel_artefact2", true, true);
      SetVisible("inca_autel_artefact4zone", false, true);
   }
   if (TaskResolved("task_inca_placeartefact3")) {
      SetVisible("inca_autel_artefact3", true, true);
      SetVisible("inca_autel_artefact2zone", false, true);
   }
   if (TaskResolved("task_inca_placeartefact4")) {
      SetVisible("inca_autel_artefact4", true, true);
      SetVisible("inca_autel_artefact1zone", false, true);
   }
   if (TaskResolved("task_inca_placekapangya")) {
      SetVisible("inca_autel_statuette", true, true);
      SetVisible("inca_autel_statuettezone", false, true);
   }

   TestAutel();
}

void Scene_Inca_Autel::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_Inca_Autel::Logic()
{
	EScene::Logic();
}

void Scene_Inca_Autel::Draw()
{
   EScene::Draw();
}

void Scene_Inca_Autel::Close()
{
}

bool Scene_Inca_Autel::ObjectClicked(const char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Inca_Autel::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Scene_Inca_Autel::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (strcmp(szItemName, "inv_inca_artefact1") == 0) {
      if (strcmp(szObjectName, "inca_autel_artefact3zone") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         SetVisible("inca_autel_artefact1", true);
         SetVisible("inca_autel_artefact3zone", false);
         ResolveTask("task_inca_placeartefact1");
         TestAutel();
      }
      else {
         // Ca ne va pas ici
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_NOTHERE"), "", true, false);
      }
      return true;
   }
   if (strcmp(szItemName, "inv_inca_artefact2") == 0) {
      if (strcmp(szObjectName, "inca_autel_artefact4zone") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         SetVisible("inca_autel_artefact2", true);
         SetVisible("inca_autel_artefact4zone", false);
         ResolveTask("task_inca_placeartefact2");
         TestAutel();
      }
      else {
         // Ca ne va pas ici
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_NOTHERE"), "", true, false);
      }
      return true;
   }
   if (strcmp(szItemName, "inv_inca_artefact3") == 0) {
      if (strcmp(szObjectName, "inca_autel_artefact2zone") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         SetVisible("inca_autel_artefact3", true);
         SetVisible("inca_autel_artefact2zone", false);
         ResolveTask("task_inca_placeartefact3");
         TestAutel();
      }
      else {
         // Ca ne va pas ici
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_NOTHERE"), "", true, false);
      }
      return true;
   }
   if (strcmp(szItemName, "inv_inca_artefact4") == 0) {
      if (strcmp(szObjectName, "inca_autel_artefact1zone") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         SetVisible("inca_autel_artefact4", true);
         SetVisible("inca_autel_artefact1zone", false);
         ResolveTask("task_inca_placeartefact4");
         TestAutel();
      }
      else {
         // Ca ne va pas ici
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_NOTHERE"), "", true, false);
      }
      return true;
   }
   if (strcmp(szItemName, "inv_inca_statuette") == 0) {
      if (strcmp(szObjectName, "inca_autel_statuettezone") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         ESoundBank::getSound("success")->playSample();
         SetVisible("inca_autel_statuette", true);
         SetVisible("inca_autel_statuettezone", false);
         ResolveTask("task_inca_placekapangya");
         TestAutel();
      }
      else {
         // Ca ne va pas ici
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_NOTHERE"), "", true, true);
      }
      return true;
   }
   return false;
}

void Scene_Inca_Autel::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
}

// Le rituel est il complet ?
void Scene_Inca_Autel::TestAutel()
{
   if (TaskResolved("task_inca_placeartefact1") &&
       TaskResolved("task_inca_placeartefact2") &&
       TaskResolved("task_inca_placeartefact3") &&
       TaskResolved("task_inca_placeartefact4") &&
       TaskResolved("task_inca_placekapangya")
       ) {
      ESoundBank::getSound("reveal")->playSample();
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"inca_final", "done", false);
   }
}