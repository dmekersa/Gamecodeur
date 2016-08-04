/*
 *  Scene_House_Room1.cpp
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Bathroom.h"
#include "ESceneDirector.h"
#include "EMiniJeuFlacons.h"
#include "MusicBank.h"
#include "ThreadLoader.h"

/* Constructeur */
SceneBathroom::SceneBathroom(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _lpGhostVoice = EMusicBank::getMusic("ghost_longvoice_egypt");
   _lpGhostVoice->setVolume(0);
   _lpGhostVoice->playStream(true);
   _lpTempo = NULL;

   EMiniJeuFlacons::Preload();
}

/* Destructeur */
SceneBathroom::~SceneBathroom()
{
   _lpGhostVoice->stopStream();
   delete _lpBgGraphic;
   XDELETE(_lpTempo);
}

void SceneBathroom::Init()
{
   SetupItem("house_bathroom_flacon_yellow[1]");
   SetupItem("house_bathroom_flacon_yellow[2]");
   SetupItem("house_bathroom_flacon_red[2]");
   
   // 1ère visite
   if (!TaskResolved("task_house_bathroomvisit")) {
      ResolveTask("task_house_bathroomvisit");
   }
   
   if (TaskResolved("task_egypt_universeresolved")) {
      SetupItem("bathroom_chalk");
      if (isVisible("bathroom_chalk")) {
         StartEmitter("flake_chalk");
      }
   }
   
   // Cabinet qui glitte ou pas
   if (TaskResolved("house_bathroom_flacon_blue[2]") && TaskResolved("house_bathroom_flacon_green[1]")) {
      GetObjectImageByName("house_bathroom_cabinet_zone")->EnableGlitch(false);
   }
      
   // Mini jeu prêt
   if (TaskResolved("task_house_bathroom_disposeflacon") && !TaskResolved("task_house_bathroom_moveflacon")) {
      ESoundBank::getSound("reveal")->playSample();
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_bathroom_blokoban_frame", true);
   }
   // Mini jeu résolu
   if (TaskResolved("task_house_bathroom_moveflacon")) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_bathroom_blokoban_frame_resolved", true);
   }
   // Flacons déjà posés
   if (TestStrGlobal("house_bathroom_square1_zone")) {
      SetVisible(GetStrGlobal("house_bathroom_square1_zone",""), true, true);
      // On efface la zone de dépot
      SetVisible("house_bathroom_square1_zone", false);
   }
   if (TestStrGlobal("house_bathroom_square2_zone")) {
      SetVisible(GetStrGlobal("house_bathroom_square2_zone",""), true, true);
      // On efface la zone de dépot
      SetVisible("house_bathroom_square2_zone", false);
   }
   if (TestStrGlobal("house_bathroom_square3_zone")) {
      SetVisible(GetStrGlobal("house_bathroom_square3_zone",""), true, true);
      // On efface la zone de dépot
      SetVisible("house_bathroom_square3_zone", false);
   }
   if (TestStrGlobal("house_bathroom_square4_zone")) {
      SetVisible(GetStrGlobal("house_bathroom_square4_zone",""), true, true);
      // On efface la zone de dépot
      SetVisible("house_bathroom_square4_zone", false);
   }
   
   // Rentrer en contact avec le fantôme
   if (!TaskResolved("task_house_bathroom_contactghost")) {
      AddTask("task_house_bathroom_contactghost");
      // Fantôme toujours allongé dans la baignoire
      SetVisible("house_bathroom_ghost_bath", true);
   }
   
   // Rideau de douche
   if (!TaskResolved("task_house_bathroom_opencurtain")) {
      // Fermé
      SetVisible("house_bathroom_curtain_closed", true, true);
   }
   
   // Le prologue a été complété (+ou bien le jeu a planté avant vortex)
   if (getAdditionalName() == "completed" || (TaskResolved("task_house_bathroom_contactghost") && getAdditionalName() != "backfrombeyond") && !TaskResolved("task_freeghost_done_egypt")) {
      Loader::LoadScene("vignette_hands_egypt");
      Loader::LoadScene("egypt_land");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
      setAdditionalName("");
      int x,y;
      ResolveTask("task_house_bathroom_contactghost");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_bathroom_ghost_bath", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_ghost", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1500);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostappear");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_bathroom_ghost", true);
      GetObjectPosition("house_bathroom_ghost", x, y, true, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      // Dialogue avant puis des âmes
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BATHROOM_NEFERES_GOD"),"",false,false,_lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BATHROOM_NEFERES_OSIRIS"),"",false,false,_lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BATHROOM_ASHLEY_IMNOT1"),"",false,true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BATHROOM_NEFERES_RITUAL1"),"",false,false,_lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BATHROOM_NEFERES_RITUAL2"),"",false,false,_lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BATHROOM_ASHLEY_IMNOT2"),"",false,true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BATHROOM_ASHLEY_IMNOT3"),"",false,true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BATHROOM_NEFERES_BACKTOMB"),"",false,false,_lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BATHROOM_ASHLEY_ICANHELP"),"",false,true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BATHROOM_NEFERES_OK"),"",false,false,_lpGhostVoice);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      // Cinématique
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands_egypt", "egypt", false);
      ResolveTask("task_house_bathroom_contactghost");
      return;
   }

   // On revient de l'univers
   if (getAdditionalName() == "backfrombeyond") {
      setAdditionalName("");
      int x,y;
      x = 1024/2;
      y = 768/2;
      SetVisible("black", true, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_ghost", true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_NEFERES_EPILOG1"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_NEFERES_EPILOG2"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_NEFERES_EPILOG3"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostgone");
      _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/reveil.ogv","frombeyond");
      SetupItem("bathroom_chalk");
      StartEmitter("flake_chalk");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_pirate_standup", false);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_ghost", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow_gem", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow_gem", false);
      ResolveTask("task_freeghost_done_egypt");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_bathroom2.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_BATHROOM2", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return;
   }
   
   if (TestGlobal("bathroom_chalk")) {
      // Plus rien à faire ici
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED"), "", true);
      return;
   }
}

void SceneBathroom::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      ResolveTask("task_egypt_universeresolved");
      _lpSceneDirector->GoToScene("bathroom","backfrombeyond");
   }
#endif
}

void SceneBathroom::Logic()
{
	EScene::Logic();
}

void SceneBathroom::Draw()
{
   EScene::Draw();
}

void SceneBathroom::Close()
{
}

bool SceneBathroom::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Morceaux de flacons à ramasser
   if ( strncmp(szObjectName, "house_bathroom_flacon_yellow[", strlen("house_bathroom_flacon_yellow[")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_bath_flacon_yellow_part",2);
   }
   if ( strncmp(szObjectName, "house_bathroom_flacon_red[", strlen("house_bathroom_flacon_red[")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_bath_flacon_red_part",2);
   }
   
   // Flacons déjà posés à ramasser
   if (!isVisible("house_bathroom_blokoban_frame")) {
      // Blue
      if (strcmp(szObjectName, "house_bathroom_flacon_blue_small_left") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_blue_small_right") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_blue_large_left") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_blue_large_right") ==0) {
         PickupSimple(szObjectName, "inv_bath_flacon_blue_full");
      }
      // Red
      if (strcmp(szObjectName, "house_bathroom_flacon_red_small_left") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_red_small_right") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_red_large_left") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_red_large_right") ==0) {
         PickupSimple(szObjectName, "inv_bath_flacon_red_full");
      }
      // Green
      if (strcmp(szObjectName, "house_bathroom_flacon_green_small_left") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_green_small_right") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_green_large_left") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_green_large_right") ==0) {
         PickupSimple(szObjectName, "inv_bath_flacon_green_full");
      }
      // yellow
      if (strcmp(szObjectName, "house_bathroom_flacon_yellow_small_left") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_yellow_small_right") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_yellow_large_left") ==0
          || strcmp(szObjectName, "house_bathroom_flacon_yellow_large_right") ==0) {
         PickupSimple(szObjectName, "inv_bath_flacon_yellow_full");
      }
      // Zones de dépots à remettre visible
      if (strstr(szObjectName, "small_left")) {
         SetVisible("house_bathroom_square1_zone", true);
         SetGlobal("house_bathroom_square1_zone","");
      }
      if (strstr(szObjectName, "small_right")) {
         SetVisible("house_bathroom_square2_zone", true);
         SetGlobal("house_bathroom_square2_zone","");
      }
      if (strstr(szObjectName, "large_left")) {
         SetVisible("house_bathroom_square3_zone", true);
         SetGlobal("house_bathroom_square3_zone","");
      }
      if (strstr(szObjectName, "large_right")) {
         SetVisible("house_bathroom_square4_zone", true);
         SetGlobal("house_bathroom_square4_zone","");
      }
   }
   
   if ( strcmp(szObjectName, "flush") == 0)
   {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "flush");
      return true;
   }   
   
   // Rideau de douche
   if (strcmp(szObjectName, "house_bathroom_curtain_closed") == 0) {
      ResolveTask("task_house_bathroom_opencurtain");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "bathcurtain");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_bathroom_curtain_closed", "house_bathroom_curtain_half");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_bathroom_curtain_half", "house_bathroom_curtain_open");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_bathroom_curtain_open",false);
      AddObjective("house","wakeuppharaoh");
      AddHint("house","wakeuppharaoh","how");
      AddHint("house","wakeuppharaoh","4canope");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_bathroom1.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_BATHROOM1", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
   }

   if (strcmp(szObjectName, "bathroom_chalk") == 0) {
      PickupSimple(szObjectName, "inv_bathroom_chalk");
      StopEmitter("flake_chalk");
      return true;
   }
   
   // Fantôme dans la baignoire
   if (strcmp(szObjectName, "house_bathroom_ghost_bath") == 0) {
      _lpSceneDirector->GoToScene("bathroom_tub","house",false);
      return true;
   }

   // Armoire à pharmacie
   if (strcmp(szObjectName, "house_bathroom_cabinet_zone") == 0) {
      if (!TaskResolved("task_house_bathroom_contactghost")) {
         _lpSceneDirector->GoToScene("bathroom_cabinet","house",false);
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED"), "", true);
      }

      return true;
   }
   
   // Blokoban
   if (strcmp(szObjectName, "house_bathroom_blokoban_frame") == 0) {
      EMiniJeuBase *lpMiniJeu = new EMiniJeuFlacons();
      _lpSceneDirector->GoToMiniGame("blokoban", lpMiniJeu);
   }
   
   return false;
}

bool SceneBathroom::ObjectOver(char *szObjectName, float x, float y)
{
   if ( strcmp(szObjectName, "pills") == 0 )
   {
      _lpSceneDirector->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_LOOK);
   }
   return false;
}

bool SceneBathroom::ItemUsed(const char *szItemName, const char *szObjectName)
{
   char szFlacon[256];
   memset(szFlacon, 0, 256);
   
   // Dépose du flacon yellow
   if (strcmp(szItemName, "inv_bath_flacon_yellow_full") == 0) {
      // En haut à gauche
      if (strcmp(szObjectName, "house_bathroom_square1_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_yellow_small_left");
      }
      else if (strcmp(szObjectName, "house_bathroom_square2_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_yellow_small_right");
      }
      else if (strcmp(szObjectName, "house_bathroom_square3_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_yellow_large_left");
      }
      else if (strcmp(szObjectName, "house_bathroom_square4_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_yellow_large_right");
      }
   }
   if (strcmp(szItemName, "inv_bath_flacon_red_full") == 0) {
      // En haut à gauche
      if (strcmp(szObjectName, "house_bathroom_square1_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_red_small_left");
      }
      else if (strcmp(szObjectName, "house_bathroom_square2_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_red_small_right");
      }
      else if (strcmp(szObjectName, "house_bathroom_square3_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_red_large_left");
      }
      else if (strcmp(szObjectName, "house_bathroom_square4_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_red_large_right");
      }
   }
   if (strcmp(szItemName, "inv_bath_flacon_blue_full") == 0) {
      // En haut à gauche
      if (strcmp(szObjectName, "house_bathroom_square1_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_blue_small_left");
      }
      else if (strcmp(szObjectName, "house_bathroom_square2_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_blue_small_right");
      }
      else if (strcmp(szObjectName, "house_bathroom_square3_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_blue_large_left");
      }
      else if (strcmp(szObjectName, "house_bathroom_square4_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_blue_large_right");
      }
   }
   if (strcmp(szItemName, "inv_bath_flacon_green_full") == 0) {
      // En haut à gauche
      if (strcmp(szObjectName, "house_bathroom_square1_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_green_small_left");
      }
      else if (strcmp(szObjectName, "house_bathroom_square2_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_green_small_right");
      }
      else if (strcmp(szObjectName, "house_bathroom_square3_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_green_large_left");
      }
      else if (strcmp(szObjectName, "house_bathroom_square4_zone") == 0) {
         strcpy(szFlacon, "house_bathroom_flacon_green_large_right");
      }
   }
   // Traitement du flacon si il y a bien un flacon à déposer
   if (strlen(szFlacon)>0) {
      ESoundBank::getSound("success")->playSample();
      SetVisible(szFlacon, true);
      _lpSceneDirector->DropItem(szItemName);
      // On note quel flacon se trouve posé sur cette zone
      SetGlobal(szObjectName,szFlacon);
      // On efface la zone de dépot
      SetVisible(szObjectName, false);
      CheckBlokoban();
      return true;         
   }
   return false;
}

// Teste si les flacons sont conformes au rituel !
void SceneBathroom::CheckBlokoban()
{
   if (_lpTempo) {
      return;
   }
   if (TestStrGlobal("house_bathroom_square1_zone")
       && TestStrGlobal("house_bathroom_square2_zone")
       && TestStrGlobal("house_bathroom_square3_zone")
       && TestStrGlobal("house_bathroom_square4_zone")) {
      // Vérifie les 4 flacons
      if (strcmp(GetStrGlobal("house_bathroom_square1_zone",""), "house_bathroom_flacon_blue_small_left") == 0
          && strcmp(GetStrGlobal("house_bathroom_square2_zone",""), "house_bathroom_flacon_red_small_right") == 0
          && strcmp(GetStrGlobal("house_bathroom_square3_zone",""), "house_bathroom_flacon_green_large_left") == 0
          && strcmp(GetStrGlobal("house_bathroom_square4_zone",""), "house_bathroom_flacon_yellow_large_right") == 0
          ) {
         ESoundBank::getSound("reveal")->playSample();
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_bathroom_blokoban_frame", true);
         ResolveTask("task_house_bathroom_disposeflacon");
         
         // Vide les zones de dépot
         SetGlobal("house_bathroom_square1_zone","");
         SetGlobal("house_bathroom_square2_zone","");
         SetGlobal("house_bathroom_square3_zone","");
         SetGlobal("house_bathroom_square4_zone","");
         return;
      }
   }
}

void SceneBathroom::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "blokoban") == 0 && bIsRevolved) {
      ESoundBank::getSound("success")->playSample();
      ResolveTask("task_house_bathroom_moveflacon");
      ResolveObjective("house","wakeuppharaoh");
      setAdditionalName("completed");
      Init();
   }
   
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
