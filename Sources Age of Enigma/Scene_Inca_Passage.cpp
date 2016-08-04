/*
 *  Scene_Inca_Passage.cpp
 *  enigma
 *
 *  Created by Rockford on 04/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Inca_Passage.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuPhotoComp.h"

/* Constructeur */
Scene_Inca_Passage::Scene_Inca_Passage(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _bEffect = false;
   
   EMiniJeuPhotoComp::Preload();
}

/* Destructeur */
Scene_Inca_Passage::~Scene_Inca_Passage()
{
   delete _lpBgGraphic;
}

void Scene_Inca_Passage::Init()
{
   if (!_bEffect) {
      if (!TaskResolved("__say_deathpassage__")) {
         ResolveTask("__say_deathpassage__");
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_DEATHPASSAGE"), "", true, false);
         AddHint("inca","4artefacts","cross");
      }
   }
   SetupItem("inca_cartouche1_ground");
   SetupItem("inca_cartouche2_ground");
   
   // Blocs au plafond ou au sol ?
   if (!_bEffect) {
      if (TaskResolved("task_inca_fillhole")) {
         SetVisible("inca_passage_blocks_ground", true, true);
         SetVisible("inca_final_zone", true, true);
      }
      else {
         AddTask("task_inca_fillhole");
         SetVisible("inca_passage_blocks_ceiling", true, true);
      }
   }
   
   // Artefacts
   if (!TaskResolved("task_inca_repairfresco3")) {
      AddTask("task_inca_repairfresco3");
   }
   if (!TaskResolved("task_inca_repairfresco4")) {
      AddTask("task_inca_repairfresco4");
   }
   
   // artefact découverts
   if (TaskResolved("task_inca_repairfresco3")) {
      SetupItem("inca_artefact3");
      SetVisible("inca_stairsfresco3_zone", false);
      SetVisible("inca_cartouche1_wall", true, true);
   }
   if (TaskResolved("task_inca_repairfresco4")) {
      SetupItem("inca_artefact4");
      SetVisible("inca_stairsfresco4_zone", false);
      SetVisible("inca_cartouche2_wall", true, true);
   }
}

void Scene_Inca_Passage::Check()
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

void Scene_Inca_Passage::Logic()
{
	EScene::Logic();
}

void Scene_Inca_Passage::Draw()
{
   EScene::Draw();
}

void Scene_Inca_Passage::Close()
{
   //   _lpSoundDesign->stopSample();   
}

bool Scene_Inca_Passage::ObjectClicked(const char *szObjectName, float x, float y)
{
   /* Cartouches */
   if (strcmp(szObjectName, "inca_cartouche1_ground") == 0) {
      PickupSimple(szObjectName, "inv_inca_cartouche1");
   }
   if (strcmp(szObjectName, "inca_cartouche2_ground") == 0) {
      if (TaskResolved("task_inca_fillhole")) {
         PickupSimple(szObjectName, "inv_inca_cartouche2");
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_HOLE"), "", true, false);
         AddTask("task_inca_fillhole");
      }
   }
   
   /* Artefacts */
   if (strcmp(szObjectName, "inca_artefact3") == 0) {
      PickupSimple(szObjectName, "inv_inca_artefact3");
      ResolveTask("task_inca_getartefact3");
      TestTaskArtefacts();
   }
   if (strcmp(szObjectName, "inca_artefact4") == 0) {
      PickupSimple(szObjectName, "inv_inca_artefact4");
      ResolveTask("task_inca_getartefact4");
      TestTaskArtefacts();
   }
   return false;
}

bool Scene_Inca_Passage::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Inca_Passage::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur utilise la statuette
   if (strcmp(szItemName, "inv_inca_statuette") == 0) {
      if (strcmp(szObjectName, "inca_passage_blocks_ceiling") == 0) {
         // Ouverture de la vignette
         ESoundBank::getSound("success")->playSample();
         _lpSceneDirector->GoToVignette("vignette_inca_statuette", szObjectName, true);
         return true;
      }
   }
   
   // Le joueur utilise un effet de la statuette
   if (strcmp(szItemName, "statuette_effect_earthquake") == 0) {
      if (strcmp(szObjectName, "inca_passage_blocks_ceiling") == 0) {
         // Lancement de l'effet
         _bEffect = true;
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "rockfall");
         _lpSceneDirector->getSequencer()->EarthQuake(NULL, true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 4000);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "inca_passage_blocks_ceiling", "inca_passage_blocks_ground");
         _lpSceneDirector->getSequencer()->EarthQuake(NULL, false);
         ResolveTask("task_inca_fillhole");
         RemoveHint("inca","4artefacts","cross");
         SetVisible("inca_final_zone", true);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         return true;
      }
   }

   char *effectprefix = "statuette_effect_";
   if (strncmp(szItemName, effectprefix,strlen(effectprefix)) == 0) {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_MAIN_YUMANCO_STATUETTE_NOEFFECT"), "", true, false);
		 return true;
   }

   // Le joueur recompose la fresque3
   if (strcmp(szObjectName, "inca_stairsfresco3_zone") == 0) {
      if (strcmp(szItemName, "inv_inca_cartouche1") == 0) {
         ESoundBank::getSound("success")->playSample();
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "inca_cartouche1_wall", true, true);
         // Mini jeu
         EMiniJeuBase *lpMiniJeu = new EMiniJeuPhotoComp("mg_photocomp_fresque1.png","mg_photocomp_bg_f1.jpg",79,129,"mg_photocomp_artefact_f1.png","mg_photocomp_cartouche_f1.png",458,225);
         _lpSceneDirector->getSequencer()->MiniGame(NULL, "fresco3", lpMiniJeu);
      }
      else {
         // Ca ne va pas ici
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_NOTHERE"), "", true, false);
      }
      return true;
   }
   
   // Le joueur recompose la fresque4
   if (strcmp(szObjectName, "inca_stairsfresco4_zone") == 0) {
      if (strcmp(szItemName, "inv_inca_cartouche2") == 0) {
         if (TaskResolved("task_inca_fillhole")) {
            ESoundBank::getSound("success")->playSample();
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "inca_cartouche1_wall", true, true);
            // Mini jeu
            EMiniJeuBase *lpMiniJeu = new EMiniJeuPhotoComp("mg_photocomp_fresque2.png","mg_photocomp_bg_f2.jpg",79,129,"mg_photocomp_artefact_f2.png","mg_photocomp_cartouche_f2.png",462,235);
            _lpSceneDirector->getSequencer()->MiniGame(NULL, "fresco4", lpMiniJeu);
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_HOLE"), "", true, false);
         }
      }
      else {
         // Ca ne va pas ici
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_NOTHERE"), "", true, false);
      }
      return true;
   }
   
   return false;
}

void Scene_Inca_Passage::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "fresco3") == 0 && bIsRevolved) {
      // Artefact 1
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow1", true);
      ESoundBank::getSound("reveal")->playSample();
      _lpSceneDirector->DropItem("inv_inca_cartouche1");
      ResolveTask("task_inca_repairfresco3");
      SetVisible("inca_cartouche1_wall", true);
      SetVisible("inca_artefact3", true);
      SetVisible("inca_stairsfresco3_zone", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_ARTEFACT_APPEAR"), "", true, false);
   }

   if (strcmp(szGameName, "fresco4") == 0 && bIsRevolved) {
      // Artefact 2
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow2", true);
      ESoundBank::getSound("reveal")->playSample();
      _lpSceneDirector->DropItem("inv_inca_cartouche2");
      ResolveTask("task_inca_repairfresco4");
      SetVisible("inca_cartouche2_wall", true);
      SetVisible("inca_artefact4", true);
      SetVisible("inca_stairsfresco4_zone", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_ARTEFACT_APPEAR"), "", true, false);
   }

   if (!bIsRevolved) {
      
      if (strcmp(szGameName, "fresco3") == 0)
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "inca_cartouche1_wall", false, true);
      if (strcmp(szGameName, "fresco4") == 0)
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "inca_cartouche2_wall", false, true);

      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      Init();
      return;
   }
}

// Attention : copier le même code dans Scene_Inca_Stairs
void Scene_Inca_Passage::TestTaskArtefacts()
{
   if (TaskResolved("task_inca_getartefact1") &&
       TaskResolved("task_inca_getartefact2") &&
       TaskResolved("task_inca_getartefact3") &&
       TaskResolved("task_inca_getartefact4")
       ) {
      ResolveTask("task_inca_getartefacts");
      ResolveObjective("inca","4artefacts");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_GOTALL"), "", true, false);
   }
}
