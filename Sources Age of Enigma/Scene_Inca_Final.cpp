/*
 *  Scene_Inca_Final.cpp
 *  enigma
 *
 *  Created by Rockford on 04/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Inca_Final.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "MusicBank.h"
#include "ThreadLoader.h"

/* Constructeur */
Scene_Inca_Final::Scene_Inca_Final(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _bEffect = false;
}

/* Destructeur */
Scene_Inca_Final::~Scene_Inca_Final()
{
   delete _lpBgGraphic;
}

void Scene_Inca_Final::Init()
{
   // La statuette et les artefacts sur l'autel
   if (TaskResolved("task_inca_placekapangya")) {
      SetVisible("inca_final_statuette", true, true);
   }
   
   // On a finit ??
   if (getAdditionalName() == "done") {
      int x,y;
      GetObjectPosition("inca_final_priest_ready", x, y, true, false);
      ResolveTask("task_inca_universeresolved");
   //   _lpSceneDirector->getMap()->setUniversVisited("UniverseInca");
      ResolveObjective("inca","sacrifice");
      SetVisible("inca_final_statuette", true, true);
      SetVisible("inca_final_inty_up", true, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      // Prêtre prêt à frapper
      SetVisible("inca_final_priest_ready", true, true);
      _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "incafinal");
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_DIE1"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_DIE2"), "", false);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "incabreak");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "inca_final_priest_ready", "inca_final_priest_killing");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "inca_final_statuette", "inca_final_statuettebroken");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "inca_final_priest_killing", "inca_final_priest_calm");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "vortex");
//      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      // Retour
      Loader::LoadScene("house_livingroom");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "blank", true, true);
      _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/soul_tunnel_r.ogv","tunnel_r");
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"house_livingroom","backfrombeyond", false);
      return;
   }
   
   // L'éclipse
   if (!_bEffect && TaskResolved("task_inca_godsign")) {
      SetVisible("inca_final_eclipse", true, true);
      SetVisible("inca_final_skyzone", false, true);
   }
   
   if (!_bEffect) {
      // Le prêtre
      if (TaskToResolve("task_inca_godsign")) {
         // Prêtre calme
         SetVisible("inca_final_priest_ready", true, true);
         // Inty allongée
         SetVisible("inca_final_inty_lie", true, true);
         // Pas de zoom possible sur l'autel
         SetVisible("inca_final_autelzone", false, true);
      }
      else if (TaskToResolve("task_inca_sacrificekapangya")) {
         // Prêtre calme
         SetVisible("inca_final_priest_calm", true, true);
         // Inty debout
         SetVisible("inca_final_inty_up", true, true);
      }
      else {
         // Prêtre prêt à frapper
         SetVisible("inca_final_priest_ready", true, true);
         // Inty allongée
         SetVisible("inca_final_inty_lie", true, true);
         // Début de la cinématique !
         int x,y;
         GetObjectPosition("inca_final_priest_ready", x, y, true, false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_READY"), "", false);
         _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_WHOISIT"), "", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_INTY"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_IDIDNT"), "", false, true);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "inca_final_priest_ready", "inca_final_priest_calm");
         GetObjectPosition("inca_final_priest_calm", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_YOUDID1"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_YOUDID2"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_IMSAD1"), "", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_IMSAD2"), "", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_NO1"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_NO2"), "", false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "inca_final_priest_calm", "inca_final_priest_ready");
         SetVisible("inca_final_autelzone", false, true);
         AddTask("task_inca_godsign");
         AddObjective("inca","sacrifice");
         AddHint("inca","sacrifice","how");
         AddHint("inca","sacrifice","god");
      }
   }
      
   // Tout est posé ? Scène finale !
}

void Scene_Inca_Final::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"inca_final", "done", false);
   }
#endif
}

void Scene_Inca_Final::Logic()
{
	EScene::Logic();
}

void Scene_Inca_Final::Draw()
{
   EScene::Draw();
}

void Scene_Inca_Final::Close()
{
}

bool Scene_Inca_Final::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Le prête prêt à sacrifier Inty
   if (strcmp(szObjectName, "inca_final_priest_ready") == 0)
   {
      int x,y;
      GetObjectPosition("inca_final_priest_ready", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_NO2"), "", true);
   }
   // Le prête prêt à sacrifier la statuette
   if (strcmp(szObjectName, "inca_final_priest_calm") == 0)
   {
      int x,y;
      GetObjectPosition("inca_final_priest_calm", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_WAITKAP"), "", true);
   }
   // Inty prête prêt à sacrifier la statuette
   if (strcmp(szObjectName, "inca_final_inty_up") == 0)
   {
      int x,y;
      GetObjectPosition("inca_final_inty_up", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_INTY_WAITKAP"), "", true);
   }
   
   // Autel
   if (strcmp(szObjectName, "inca_final_autelzone") == 0)
   {
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"inca_autel", "");
      return true;
   }
   
   // Back
   if (strcmp(szObjectName, "inca_backtohouse_zone") == 0)
   {
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"house_livingroom", "backfrombeyond");
      return true;
   }
   return false;
}

bool Scene_Inca_Final::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Inca_Final::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur utilise la statuette
   if (strcmp(szItemName, "inv_inca_statuette") == 0 && !TaskResolved("task_inca_godsign")) {
      if (strcmp(szObjectName, "inca_final_skyzone") == 0 || strcmp(szObjectName, "inca_final_priest_ready") == 0) {
         // Ouverture de la vignette
         ESoundBank::getSound("success")->playSample();
         _lpSceneDirector->GoToVignette("vignette_inca_statuette", szObjectName, true);
         return true;
      }
   }
   
   // Le joueur utilise quelque chose sur l'autel
   if (strcmp(szObjectName, "inca_final_autelzone") == 0 || strcmp(szObjectName, "inca_final_priest_calm") == 0) {
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"inca_autel", "");
   }
    
   // Le joueur utilise un effet de la statuette
   if (strcmp(szItemName, "statuette_effect_dark") == 0) {
      if (strcmp(szObjectName, "inca_final_skyzone") == 0 || strcmp(szObjectName, "inca_final_priest_ready") == 0) {
         int x,y;
         // Lancement de l'effet
         _bEffect = true;
         SetVisible("inca_final_skyzone", false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 4000);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "inca_final_eclipse", true);
         ESoundBank::getSound("inca_eclipse")->playSample();
         ResolveTask("task_inca_godsign");
         AddTask("task_inca_sacrificekapangya");
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "inca_final_priest_ready", "inca_final_priest_calm");
         GetObjectPosition("inca_final_priest_calm", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_STOP1"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_PRIEST_STOP2"), "", false);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "inca_final_inty_lie", "inca_final_inty_up");
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         GetObjectPosition("inca_final_inty_up", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_INTY"), "", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_INTY_YOUKILLME"), "", false);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_CRY"), "", false, true);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_INTY_YOUCRY"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_INTY_YOURSOUL1"), "", false);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_INTY_YOURSOUL2"), "", false);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_YUMANCO_HOWSOUL"), "", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_INTY_YOUCAN1"), "", false);
         if (TestGlobal("__mode_adventure__")) _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("INCA_INTY_YOUCAN2"), "", false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
         SetVisible("inca_final_autelzone", true, true);
         return true;
      }
   }

   char *effectprefix = "statuette_effect_";
   if (strncmp(szItemName, effectprefix,strlen(effectprefix)) == 0) {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_MAIN_YUMANCO_STATUETTE_NOEFFECT"), "", true, false);
		 return true;
   }

   // Le joueur utilise quelque chose sur le prêtre, on l'aide à comprendre...
   if (strcmp(szObjectName, "inca_final_priest_ready") == 0) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INCA_FINAL_YUMANCO_CANNOT"), "", false, false);
      return true;
   }
   
   return false;
}

void Scene_Inca_Final::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
