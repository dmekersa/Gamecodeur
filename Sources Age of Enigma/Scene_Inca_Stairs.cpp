/*
 *  Scene_Inca_Stairs.cpp
 *  enigma
 *
 *  Created by Rockford on 04/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Inca_Stairs.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuMissingParts.h"

/* Constructeur */
Scene_Inca_Stairs::Scene_Inca_Stairs(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _bLight = false;
   
   EMiniJeuMissingParts::Preload();
}

/* Destructeur */
Scene_Inca_Stairs::~Scene_Inca_Stairs()
{
   delete _lpBgGraphic;
}

void Scene_Inca_Stairs::Init()
{
   //   _lpSoundDesign = EMusicBank::getMusic("middleage_sd_outside");
   //   _lpSoundDesign->playStream();
   
   SetupItem("inca_fresco1_piece[1]");
   SetupItem("inca_fresco1_piece[2]");
   SetupItem("inca_fresco1_piece[3]");
   SetupItem("inca_fresco1_piece[4]");   
   SetupItem("inca_fresco1_piece[5]");   
   SetupItem("inca_fresco2_piece[1]");
   SetupItem("inca_fresco2_piece[2]");
   SetupItem("inca_fresco2_piece[3]");
   SetupItem("inca_fresco2_piece[4]");   
   SetupItem("inca_fresco2_piece[5]");
   
   // 1ère entrée
   if (!TaskResolved("task_inca_entertemple")) {
      ResolveTask("task_inca_entertemple");
      ResolveObjective("inca","enter");
   }
   
   // Retour après vignette statuette
   if (_bLight) {
      ESoundBank::getSound("inca_light")->playSample();
      SetVisible("inca_stairs_darkness", true,true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "inca_stairs_light", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "inca_stairs_darkness", false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "inca_stairs_light", false);
      ResolveTask("task_inca_stairslight");
      RemoveHint("inca","4artefacts","light");
      AddTask("task_inca_findfresco1");
      AddTask("task_inca_findfresco2");
      _bLight = false;
   }
   else if (!TaskResolved("task_inca_stairslight")) {
      AddTask("task_inca_stairslight");
      SetVisible("inca_stairs_darkness", true);
   }
   else {
      SetVisible("inca_stairs_darkness", false);
   }
      
   // artefact découverts
   if (TaskResolved("task_inca_repairfresco1")) {
      SetupItem("inca_artefact1");
      SetVisible("inca_stairsfresco1_zone", false, true);
   }
   if (TaskResolved("task_inca_repairfresco2")) {
      SetupItem("inca_artefact2");
      SetVisible("inca_stairsfresco2_zone", false, true);
   }
}

void Scene_Inca_Stairs::Check()
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

void Scene_Inca_Stairs::Logic()
{
	EScene::Logic();
}

void Scene_Inca_Stairs::Draw()
{
   EScene::Draw();
}

void Scene_Inca_Stairs::Close()
{
   //   _lpSoundDesign->stopSample();   
}

bool Scene_Inca_Stairs::ObjectClicked(const char *szObjectName, float x, float y)
{
   /* Morceaux de fresque 1 */
   if ( strncmp(szObjectName, "inca_fresco1_piece", strlen("inca_fresco1_piece")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_inca_fresco1broken",5);
   }
   /* Morceaux de fresque 2 */
   if ( strncmp(szObjectName, "inca_fresco2_piece", strlen("inca_fresco2_piece")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_inca_fresco2broken",5);
   }
   
   /* Artefacts */
   if (strcmp(szObjectName, "inca_artefact1") == 0) {
      PickupSimple(szObjectName, "inv_inca_artefact1");
      ResolveTask("task_inca_getartefact1");
      TestTaskArtefacts();
   }
   if (strcmp(szObjectName, "inca_artefact2") == 0) {
      PickupSimple(szObjectName, "inv_inca_artefact2");
      ResolveTask("task_inca_getartefact2");
      TestTaskArtefacts();
   }
   
   return false;
}

bool Scene_Inca_Stairs::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Inca_Stairs::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur utilise la statuette
   if (strcmp(szItemName, "inv_inca_statuette") == 0) {
      if (strcmp(szObjectName, "inca_stairs_darkness") == 0) {
         // Ouverture de la vignette
         ESoundBank::getSound("success")->playSample();
         _lpSceneDirector->GoToVignette("vignette_inca_statuette", szObjectName, true);
         return true;
      }
   }
   
   // Le joueur utilise un effet de la statuette
   if (strcmp(szItemName, "statuette_effect_light") == 0) {
      if (strcmp(szObjectName, "inca_stairs_darkness") == 0) {
         // Lancement de l'effet
         _bLight = true;
         return true;
      }
   }

   char *effectprefix = "statuette_effect_";
   if (strncmp(szItemName, effectprefix,strlen(effectprefix)) == 0) {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_MAIN_YUMANCO_STATUETTE_NOEFFECT"), "", true, false);
		 return true;
   }
   
   // Le joueur recompose la fresque1
   if (strcmp(szObjectName, "inca_stairsfresco1_zone") == 0) {
      if (strcmp(szItemName, "inv_inca_fresco1repaired") == 0) {
         // Mini jeu
         EMiniJeuBase *lpMiniJeu = new EMiniJeuMissingParts("mg_missingpart_fresque1.png","mg_missingpart_bg_f1.jpg",79,129,"mg_missingpart_artefact_f1.png",492,267);
         // TODO: lpMiniJeu->SetVariant();
         _lpSceneDirector->GoToMiniGame("fresco1", lpMiniJeu);
      }
      else if (strcmp(szItemName, "inv_inca_fresco1broken") == 0) {
         // Incomplet
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_INCOMPLETE"), "", true, false);
      }
      else {
         // Ca ne va pas ici
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_NOTHERE"), "", true, false);
      }
      return true;
   }

   // Le joueur recompose la fresque2
   if (strcmp(szObjectName, "inca_stairsfresco2_zone") == 0) {
      if (strcmp(szItemName, "inv_inca_fresco2repaired") == 0) {
         // Mini jeu
         EMiniJeuBase *lpMiniJeu = new EMiniJeuMissingParts("mg_missingpart_fresque2.png","mg_missingpart_bg_f2.jpg",79,129,"mg_missingpart_artefact_f2.png",462,265);
         // TODO: lpMiniJeu->SetVariant();
         _lpSceneDirector->GoToMiniGame("fresco2", lpMiniJeu);
      }
      else if (strcmp(szItemName, "inv_inca_fresco2broken") == 0) {
         // Incomplet
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_INCOMPLETE"), "", true, false);
      }
      else {
         // Ca ne va pas ici
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_NOTHERE"), "", true, false);
      }
      return true;
   }
   
   return false;
}

void Scene_Inca_Stairs::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "fresco1") == 0 && bIsRevolved) {
      // Artefact 1
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow1", true);
      ESoundBank::getSound("reveal")->playSample();
      _lpSceneDirector->DropItem("inv_inca_fresco1repaired");
      ResolveTask("task_inca_repairfresco1");
      SetVisible("inca_artefact1", true);
      SetVisible("inca_stairsfresco1_zone", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_ARTEFACT_APPEAR"), "", true, false);
   }
   if (strcmp(szGameName, "fresco2") == 0 && bIsRevolved) {
      // Artefact 2
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow2", true);
      ESoundBank::getSound("reveal")->playSample();
      _lpSceneDirector->DropItem("inv_inca_fresco2repaired");
      ResolveTask("task_inca_repairfresco2");
      SetVisible("inca_artefact2", true);
      SetVisible("inca_stairsfresco2_zone", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_ARTEFACT_APPEAR"), "", true, false);
   }
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}

// Attention : copier le même code dans Scene_Inca_Passage
void Scene_Inca_Stairs::TestTaskArtefacts()
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