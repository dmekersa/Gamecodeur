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
#include "Scene_House_Attic.h"
#include "ESceneDirector.h"
#include "EMiniJeuDemelage.h"
#include "MusicBank.h"

/* Constructeur */
SceneAttic::SceneAttic(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   EMiniJeuDemelage::Preload();
}

/* Destructeur */
SceneAttic::~SceneAttic()
{
   delete _lpBgGraphic;
}

void SceneAttic::Init()
{
   // On revient de l'au-delà
   if (getAdditionalName() == "backfrombeyond") {
      int x,y;
      GetObjectPosition("house_attic_nathan_panic", x, y, true, false);
      SetVisible("house_attic_nathan_panic", true, true);
      SetVisible("black5_", true,true);
      EMusicBank::getMusic("attic_sd")->setVolume(70);
      EMusicBank::getMusic("attic_sd")->playStream();
      ESoundBank::getSound("ghost_energy",true)->playSample();
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_shadow_panic",true);
      _lpSceneDirector->getSequencer()->Wait(NULL,1000);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "black5_", "black4_");
      _lpSceneDirector->getSequencer()->Wait(NULL,500);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "black4_", "black3_");
      _lpSceneDirector->getSequencer()->Wait(NULL,500);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "black3_", "black2_");
      _lpSceneDirector->getSequencer()->Wait(NULL,500);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "black2_", "black1_");
      _lpSceneDirector->getSequencer()->Wait(NULL,500);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black1_", false);

      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_END_NATHAN1"), "ATTIC_END_NATHAN1", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ATTIC_END_ASHLEY1"), "attic_end_ashley1", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ATTIC_END_ASHLEY2"), "attic_end_ashley2", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ATTIC_END_ASHLEY3"), "attic_end_ashley3", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ATTIC_END_ASHLEY5"), "attic_end_ashley5", false, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->EarthQuake(NULL,true, "hall_earthquake");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_shadow_die1",true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_shadow_die2",true);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_attic_nathan_panic", "house_attic_nathan_die");
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_END_NATHAN2"), "ATTIC_END_NATHAN2", false);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostgone");      
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_attic_nathan_die", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_shadow_die1",false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_shadow_die2",false);
      _lpSceneDirector->getSequencer()->EarthQuake(NULL,false, "hall_earthquake");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black1", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black2", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black3", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black4", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black5", true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      // Cinématique de fin
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"video_player", "theend", false);
      return;
   }
   
   if (!ObjectiveResolved("house","atticritual")) {
      SetVisible("house_attic_pentacle_zone", true);
   }
   else {
      if (TaskResolved("task_house_attic_firstvisit") && getAdditionalName() == "") {
         setAdditionalName("gotohell");
         Init();
      }
   }
   
   // Première visite dans le grenier
   if (TaskResolved("task_house_attic_firstvisit")) {
   }
   else {
      // Le médaillon passe de l'IHM à notre inventaire
      EGlobalBank::RemoveValue("task_gethexagramme");
      _lpSceneDirector->ShowIHM(true);
      _lpSceneDirector->getSequencer()->PickupItem(NULL,"inv_hexagramme_final", 945, 680, 1);
      _lpSceneDirector->OpenDialogbox(KStr("ATTIC_PENDANT"));
      ResolveTask("task_house_attic_firstvisit");
      AddTask("task_house_attic_untangle");
      AddObjective("house","atticritual");
      AddHint("house","atticritual","how");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_attic.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_ATTIC", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
   }
   
   // Prologue aux enfers
   if (getAdditionalName() == "gotohell") {
      int x,y;
      _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE);
      EMusicBank::getMusic("attic_sd")->setVolume(70);
      EMusicBank::getMusic("attic_sd")->playStream();
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1500);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "attic_pentacle");      
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_attic_pentaclelight", true);
      _lpSceneDirector->getSequencer()->Wait(NULL,1500);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls",true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_shadow_start",true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "attic_nathanappear");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_attic_nathan", true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls",false);
      GetObjectPosition("house_attic_nathan", x, y, true, false);
      _lpSceneDirector->getSequencer()->Wait(NULL,1500);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_NATHAN1"), "ATTIC_NATHAN1", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ATTIC_ASHLEY1"), "attic_ashley1", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_NATHAN2"), "ATTIC_NATHAN2", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_NATHAN3"), "ATTIC_NATHAN3", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_NATHAN4"), "ATTIC_NATHAN4", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ATTIC_ASHLEY2"), "attic_ashley2", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_NATHAN5"), "ATTIC_NATHAN5", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_NATHAN6"), "ATTIC_NATHAN6", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_shadow_start",false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_shadow_anger",true);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_attic_nathan", "house_attic_nathan_anger");
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_NATHAN7"), "ATTIC_NATHAN7", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ATTIC_ASHLEY4"), "attic_ashley4", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_NATHAN8"), "ATTIC_NATHAN8", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ATTIC_ASHLEY5"), "attic_ashley5", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_NATHAN9"), "ATTIC_NATHAN9", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_shadow_anger",false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_shadow_attack",true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls",true);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_attic_nathan_anger", "house_attic_nathan_attack");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_fragments", true);
      _lpSceneDirector->getSequencer()->EarthQuake(NULL,true, "hall_earthquake");
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ATTIC_NATHAN10"), "ATTIC_NATHAN10", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black1", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black2", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black3", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black4", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black5", true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_shadow_attack",false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->EarthQuake(NULL,false, "hall_earthquake");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_fragments", false);
      _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/dark_tunnel.ogv", "tunnel");
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"hell_mainscreen","");
   }
}

void SceneAttic::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"hell_mainscreen","arrival");
   }
   if (KInput::isPressed(K_VK_F7))
   {
      setAdditionalName("backfrombeyond");
      Init();
   }
#endif   
}

void SceneAttic::Logic()
{
	EScene::Logic();
}

void SceneAttic::Draw()
{
   EScene::Draw();
}

void SceneAttic::Close()
{
   EMusicBank::getMusic("attic_sd")->stopStream();
   ESoundBank::StopSample("ghost_energy");
}

bool SceneAttic::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "house_attic_pentacle_zone") == 0) {
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_attic_pentacle", "", false);
      return true;
   }

   return false;
}

bool SceneAttic::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool SceneAttic::ItemUsed(const char *szItemName, const char *szObjectName)
{
   return false;
}

void SceneAttic::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
}
