/*
 *  SceneLivingRoom.cpp
 *  enigma
 *
 *  Created by Rockford on 29/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_House_LivingRoom.h"

#include "ESceneDirector.h"
#include "MusicBank.h"
#include "ThreadLoader.h"

/* Constructeur */
SceneLivingRoom::SceneLivingRoom(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _lpGhostVoice = EMusicBank::getMusic("ghost_longvoice_inca");
   _lpGhostVoice->setVolume(0);
   _lpGhostVoice->playStream(true);
}

/* Destructeur */
SceneLivingRoom::~SceneLivingRoom()
{
   StopAnimation("house_livingroom_ghost_anim");
   delete _lpBgGraphic;
   _lpGhostVoice->stopStream();
}

void SceneLivingRoom::Init()
{
   ResolveTask("task_house_findfirstghost");
      
   SetupItem("envelope_livingroom");
   SetupItem("house_statuette_part_skull");
   SetupItem("house_statuette_part_chest");
   SetupItem("house_statuette_part_legs");
   SetupItem("house_statuette_part_head1");

   SetupItem("livingroom_page[1]");
   SetupItem("livingroom_page[2]");
   SetupItem("livingroom_page[3]");
   SetupItem("livingroom_page[4]");
   SetupItem("livingroom_page[5]");
   SetupItem("livingroom_page[6]");
   
   if (TaskResolved("task_inca_universeresolved")) {
      SetupItem("livingroom_key");
      if (isVisible("livingroom_key")) {
         StartEmitter("flake_key");
      }
   }   

   SetVisible("livingroom_poltergeist",false);
   StopAnimation("livingroom_poltergeistanim");
   
   if (TaskResolved("task_livingroom_openbox") && TestGlobal("livingroom_bookinca_eyestatue")) {
      SetVisible("chessboard", false);
   }
   
   // Statuette
   if (!TaskResolved("task_house_livingroom_repairstatuette")) {
      // On affiche uniquement le livre, on affichera la statuette après avoir posé le livre
      if (TaskResolved("task_house_livingroom_placebook")) {
         SetVisible("livingroom_statuette_ghost", true, true);
      }
   }
   else {
      SetVisible("livingroom_statuette_full", true, true);
      if (!TaskToResolve("task_house_kapangya4effets")) {
         GetObjectImageByName("livingroom_statuette_full")->EnableGlitch(false);
      }
   }
   
   // Livre
   if (!TaskResolved("task_house_livingroom_placebook")) {
      SetVisible("livingroom_book_ghost", true, true);
      if (!TaskResolved("task_living_table_discover")) {
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "flake_statuette", true);
      }
   }
   else if TaskResolved("task_house_livingroom_placebook") {
      SetVisible("livingroom_book_full", true, true);
   }
   
   // Teasing !!
#ifdef BFG_TEASING
   if (TaskResolved("task_japan_universeresolved") && TaskResolved("task_island_universeresolved") && TaskResolved("task_living_discover")) {
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "teasing", "", false);
   }
#endif
   
   // Première visite de la pièce
   if (!TaskResolved("task_living_discover")) {
      int x,y;
      ResolveTask("task_living_discover");
      ResolveObjective("house","contactyumanco");
      SetVisible("livingroom_statuette_ghost", false, true);
      StartAnimation("house_livingroom_ghost_anim");
      StartAnimation("clouds_anim1");
      StartAnimation("clouds_anim2");
      GetObjectPosition("house_livingroom_ghost", x, y, true, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 4000);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostappear");
      _lpSceneDirector->getSequencer()->ShowImage("pirate_sit appear","house_livingroom_ghost",true);
//      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_living_darkcloud1", true, true);
//      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_living_darkcloud2", true, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_STATUETTETALK1"), "", false, false, _lpGhostVoice);
//      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      if (TestGlobal("__mode_adventure__")) {
         //         _lpSceneDirector->getSequencer()->Energy(NULL, 1024/2, 768/2, 1024/2, 768/2, 632, 407, 5);
      }
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostgone");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_livingroom_ghost", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", false);
//      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_living_darkcloud2", false, true);
//      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_living_darkcloud1", false, true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "flake_statuette", true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_WHATWAS"), "", false, true);
      AddObjective("house","living_book");
      AddHint("house","living_book","where");
      //      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow_green", false);
      AddTask("task_house_livingroom_placebook");

      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_living1.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_LIVING1", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();

      // Teasing !!
#ifdef BFG_TEASING
      if (TaskResolved("task_japan_universeresolved") && TaskResolved("task_island_universeresolved")) {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "teasing", "", false);
      }
#endif
   }
   
   // Yumanco attend les 4 effets !
   if (TaskToResolve("task_house_kapangya4effets")) {
      int x,y;
      ResolveTask("task_living_discover");
      
      if (TaskResolved("task_house_kapangya_effect1") &&
          TaskResolved("task_house_kapangya_effect2") &&
          TaskResolved("task_house_kapangya_effect3") &&
          TaskResolved("task_house_kapangya_effect4")) {
         ResolveTask("task_house_kapangya4effets");
         ResolveObjective("house","living_4powers");
         setAdditionalName("completed");	
      } else {
         StartAnimation("house_livingroom_ghost_anim");
         SetVisible("livingroom_statuette_ghost", false, true);
         GetObjectPosition("house_livingroom_ghost", x, y, true, false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         StartEmitter("creeping_static");
         StartAnimation("house_livingroom_ghost_anim");
         SetVisible("house_livingroom_ghost", true, true);
         AskEffect();
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         if (!TaskResolved("task_house_living_comment4effects")) {
            ResolveTask("task_house_living_comment4effects");
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_OKLETME"), "", false, true);
         }
      }
   }
   
   // Le minijeu chessboard a été fait
   if (TaskResolved("task_livingroom_memorychess")) {
		SetVisible("chessboard",false);
   }
   
   // Pas de glitch sur le piano si on a déjà obtenu la machoire
   if (TaskResolved("task_house_getjaw")) {
      GetObjectImageByName("piano")->EnableGlitch(false);
   }
   
   // Les 4 effets ont été complétés
   if (getAdditionalName() == "completed") {
      Loader::LoadScene("vignette_hands_inca");
      Loader::LoadScene("inca_mainscreen");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      int x,y;
      ResolveTask("task_house_livingroom_effectsdone");
      StopEmitter("creeping_dust1");
      StartAnimation("house_livingroom_ghost_anim");
      SetVisible("house_livingroom_ghost", true, true);
      GetObjectPosition("house_livingroom_ghost", x, y, true, false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2500);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      // Dialogue avant puis des âmes
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_DIAL1"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_DIAL2"), "", false, false, _lpGhostVoice);
      //      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_HELLO"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_NOTGOD"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_DIAL3"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_DIAL4"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_DIAL5"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("LIVINGROOM_ASHLEY_GIVEME"), "", false, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      // Cinématique
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands_inca", "inca", false);
      return;
   }
   
   // On revient de l'au-delà
   if (getAdditionalName() == "backfrombeyond") {
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
      int x,y;
      x = 1024/2;
      y = 768/2;
      SetVisible("black", true, true);
      StopEmitter("creeping_dust1");
      StartEmitter("creeping_static");
      // Dialogue
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_BACK1"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_BACK2"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostgone");
      _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/reveil.ogv","frombeyond");
      SetupItem("livingroom_key");
      StartEmitter("flake_key");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static", false);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      // Pendentif
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "neckless", true, true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "neckless_stone", true, true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "neckless_stone", false, true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "neckless", false, true);
      
      AddObjective("house","helpnatsumi");
      AddHint("house","helpnatsumi","where");

      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_living2.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_LIVING2", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();

      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, false);
      
      return;
   }
   
   // L'inca attend la corde pour nous aider
   if (TaskToResolve("task_house_livingroom_giverope")) {
      int x,y;
      GetObjectPosition("house_livingroom_ghost", x, y, true, false);
      SetVisible("house_livingroom_ghost", true, true);
      StartEmitter("creeping_static");
      StopEmitter("creeping_dust1");
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_GIVEMEROPE"), "", false, false, _lpGhostVoice);
      return;
   }
   
   // On force le passage à l'univers, en cas de plantage/quit prématuré entre la scène et l'univers
   if (TaskResolved("task_house_livingroom_effectsdone") && !TaskResolved("task_inca_universeresolved")) {
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands_inca", "inca", false);
      return;
   }
   if (TaskResolved("task_inca_prologcompleted") && !TaskResolved("task_inca_universeresolved")) {
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands_inca", "inca", false);
      return;
   }
   // On a tout terminé
   if ( TaskResolved("task_inca_universeresolved") && TestGlobal("livingroom_key")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED"), "", true);
      return;
   }
}

void SceneLivingRoom::AskEffect()
{
   int x,y;
   GetObjectPosition("house_livingroom_ghost", x, y, true, false);
   if (!TaskResolved("task_house_kapangya_effect1"))
   {
      if (!TaskResolved("task_obj4effect")) {
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_ASKEFFECT0"), "", false, false, _lpGhostVoice);
      }
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_ASKEFFECT1"), "", false, false, _lpGhostVoice);
   }
   else if (TaskResolved("task_house_kapangya_effect1") && !TaskResolved("task_house_kapangya_effect2")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_ASKEFFECT2"), "", false, false, _lpGhostVoice);
   }
   else if (TaskResolved("task_house_kapangya_effect2") && !TaskResolved("task_house_kapangya_effect3")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_ASKEFFECT3"), "", false, false, _lpGhostVoice);
   }
   else if (TaskResolved("task_house_kapangya_effect3") && !TaskResolved("task_house_kapangya_effect4")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_ASKEFFECT4"), "", false, false, _lpGhostVoice);
   }
   
   if (!TaskResolved("task_obj4effect")) {
      ResolveTask("task_obj4effect");
      AddObjective("house","living_4powers");
      AddHint("house","living_4powers","how");
      AddHint("house","living_4powers","next");
   }
}

void SceneLivingRoom::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      ResolveTask("task_inca_prologcompleted");
      setAdditionalName("completed");
      Init();
   }
   if (KInput::isPressed(K_VK_F7))
   {
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_hall.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage(KStr("DIARY_LIVING2"), FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
   }
   if (KInput::isPressed(K_VK_0))
   {
      ResolveTask("task_inca_prologcompleted");
      setAdditionalName("backfrombeyond");
      Init();
   }
   
   if (KInput::isPressed(K_VK_F7)) {
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_livingroom_ghost", true);
   }
#endif
}

void SceneLivingRoom::Logic()
{
	EScene::Logic();
}

void SceneLivingRoom::Draw()
{
   EScene::Draw();
}

void SceneLivingRoom::Close()
{
//   ESoundBank::getSound("poltergeistloop",true)->stopSample();
}

bool SceneLivingRoom::ObjectClicked(const char *szObjectName, float x, float y)
{
   if ( strncmp(szObjectName, "livingroom_page[", strlen("livingroom_page[")) == 0 )
   {
      PickupMultiple(szObjectName, "inv_livingroom_page", 6);
   }   
   
   if (strcmp(szObjectName, "livingroom_key") == 0)
   {
      StopEmitter("flake_key");
      PickupSimple(szObjectName, "inv_basementkey");
      return true;
   }
   if ( strcmp(szObjectName, "envelope_livingroom") == 0)
   {
      PickupSimple(szObjectName, "ihm_envelope");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage("letterpaper.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("ENVELOPE_LIVINGROOM", FONT_DIARY_2, 0, 0);
      _lpSceneDirector->getDiaryPtr()->addImageToPage("logofraternity.png");
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return true;
   }
   if (strcmp(szObjectName, "house_statuette_part_skull") == 0)
   {
      PickupSimple("house_statuette_part_skull", "inv_house_statuette_part_skull");
      return true;
   }
   if (strcmp(szObjectName, "house_statuette_part_chest") == 0)
   {
      PickupSimple("house_statuette_part_chest", "inv_house_statuette_part_chest");
      return true;
   }
   if (strcmp(szObjectName, "house_statuette_part_legs") == 0)
   {
      PickupSimple("house_statuette_part_legs", "inv_house_statuette_part_legs");
      return true;
   }
   if (strcmp(szObjectName, "house_statuette_part_head1") == 0)
   {
      PickupSimple("house_statuette_part_head1", "inv_house_statuette_part_head1");
      return true;
   }
   if (strcmp(szObjectName, "chessboard") == 0)
   {
      _lpSceneDirector->GoToScene("house_livingroom_box","",false);
      return true;
   }
//   if (strcmp(szObjectName, "livingroom_poltergeist") == 0)
//   {
//      ESoundBank::getSound("poltergeistloop",true)->stopSample();
//      EMiniJeuPoltergeist *_lpMiniJeu = new EMiniJeuPoltergeist();
//      _lpSceneDirector->GoToMiniGame("livingroom_poltergeist",_lpMiniJeu,"poltergeist");
//      SetVisible("livingroom_poltergeist",false);
//      return true;
//   }
   if (strcmp(szObjectName, "livingroom_statuette_ghost") == 0 || strcmp(szObjectName, "livingroom_statuette_full") == 0 || strcmp(szObjectName, "livingroom_book_full") == 0 || strcmp(szObjectName, "livingroom_book_ghost") == 0)
   {
      _lpSceneDirector->setZoomin(true);
      _lpSceneDirector->GoToScene("house_livingroom_table", "", false);
      return true;
   }
   if (strcmp(szObjectName, "piano") == 0)
   {
	   if (!TaskResolved("task_house_livingroom_play")) {
         //ESoundBank::getSound("poltergeistloop",true);
         //_lpSceneDirector->getSequencer()->PlaySound(NULL, "poltergeistloop", true); 
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "poltergeist_amadeus"); 
         StartAnimation("livingroom_poltergeistanim");
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "livingroom_poltergeist", true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 8000);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         //_lpSceneDirector->getSequencer()->PlaySound(NULL, "poltergeistloop"); 
         //_lpSceneDirector->getSequencer()->MiniGame(NULL, "livingroom_poltergeist", _lpMiniJeu);
         _lpSceneDirector->getSequencer()->ShowImage(NULL,"livingroom_poltergeist", false);
         _lpSceneDirector->getSequencer()->Wait(NULL, 400);
         _lpSceneDirector->getSequencer()->GoToScene(NULL,"house_livingroom_piano","",false);
         ResolveTask("task_house_livingroom_play");
         return true;
      }
      else {
         _lpSceneDirector->setZoomin(true);
         _lpSceneDirector->GoToScene("house_livingroom_piano", "", true);
//         return true;
      }
   }
   
   if (strcmp(szObjectName, "house_livingroom_ghost") == 0) {
      if (TaskToResolve("task_house_kapangya4effets")) {
         AskEffect();
         return true;
      }
      else if (TaskToResolve("task_house_livingroom_giverope")) {
         int x,y;
         GetObjectPosition("house_livingroom_ghost", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_LIVINGROOM_YUMANCO_GIVEMEROPE"), "", false, false, _lpGhostVoice);
         return true;
      }
   }
   return false;
}

bool SceneLivingRoom::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool SceneLivingRoom::ItemUsed(const char *szItemName, const char *szObjectName)
{
   return false;
}

void SceneLivingRoom::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "livingroom_poltergeist")==0 && bIsRevolved) {
//      // Tâche résolue
//      ResolveTask("task_house_livingroom_killpoltergeist");
//      StopAnimation("livingroom_poltergeistanim");
//      SetVisible("livingroom_poltergeist",false);
//      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_KILLPOL"),"", true, false);
   }
}
