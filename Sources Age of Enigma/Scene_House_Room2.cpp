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
#include "SoundBank.h"
#include "Scene_House_Room2.h"
#include "ESceneDirector.h"
#include "MusicBank.h"
#include "EMiniJeuPoltergeist.h"
#include "ThreadLoader.h"

/* Constructeur */
SceneRoom2::SceneRoom2(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpGhostVoice = EMusicBank::getMusic("ghost_longvoice_middleage");
   _lpGhostVoice->setVolume(0);
   _lpGhostVoice->playStream(true);
}

/* Destructeur */
SceneRoom2::~SceneRoom2()
{
   _lpGhostVoice->stopStream();
}

void SceneRoom2::Init()
{
   SetVisible("house_room2_ghostwin", false);
   // Fragments hors du lit
   SetupItem("house_room2_frag05");
   SetupItem("house_room2_frag06");
   SetupItem("house_room2_frag08");
   
   // Objet laissé au sol par Guillem
   if (TaskResolved("task_middleage_universeresolved")) {
      SetVisible("house_room2_mirrorzone",false);
      SetupItem("room2_flask");
      if (isVisible("room2_flask")) {
         StartEmitter("flake_flask");
      }
   }    

   // Lit découvert ou pas
   if (TaskResolved("task_house_room2_openbed")) {
      SetVisible("house_room2_bedopen", true, true);
      SetupItem("house_room2_frag07");
   }
   else {
      SetVisible("house_room2_bedclosed", true, true);
   }
   
   // Table de nuit
   if (TaskResolved("task_house_room2_openbedside")) {
      SetVisible("house_room2_bedside_open", true, true);
   }
   else {
      SetVisible("house_room2_bedside_closed", true, true);
//      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_ROOM2_ASHLEY_BEDSIDELIGHT"), "", true);
   }
   
   // Fantôme dans le miroir
   if (TaskResolved("task_house_room2_givebible") && !TaskToResolve("task_house_room2_killpoltergeist") && !TaskResolved("task_house_room2_killpoltergeist")) {
//      SetVisible("house_room2_mirrorghost", true);
   }
   
   // On revient de l'univers
   if (getAdditionalName() == "backfrombeyond") {
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_priest_nobible", true);
      int x,y;
      x = 1024/2;
      y = 768/2;
      SetVisible("black", true, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_GUILLEM_EPILOG1"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_GUILLEM_EPILOG2"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_GUILLEM_EPILOG3"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_GUILLEM_EPILOG4"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostgone");
      _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/reveil.ogv","frombeyond");
      SetupItem("room2_flask");
      StartEmitter("flake_flask");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_priest_nobible", false);
      ResolveTask("task_freeghost_done_middleage");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_room2.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_ROOM2", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return;
   }   
   
   if (TaskResolved("task_freeghost_done_middleage")) {
      // Plus rien à faire ici
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED"), "", true);
      return;
   }

   // On n'a pas encore contacté le fantôme
   if (!TaskResolved("task_house_room2_contactghost")) {
      AddTask("task_house_room2_contactghost");
   }
   
   // Attaque du fantôme
   if (_strSceneAdditionalName == "reveal_poltergeist" || TaskToResolve("task_house_room2_killpoltergeist")) {
      EMiniJeuPoltergeist::Preload();
      SetVisible("house_room2_mirrorzone",false);
      ResolveTask("task_house_room2_expelevil");
      StartAnimation("poltergeist_anim");
      ESoundBank::getSound("poltergeistloop",true)->playSample();
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_priest_underattack", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_room2_poltergeist", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_room2_priest_underattack", true);
      ESoundBank::getSound("poltergeist")->playSample();
      int x,y;
      GetObjectPosition("house_room2_priest_underattack", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_PRIEST_HELPPOLTER"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
   }
   
   // Poltergeist tué, cinématique de transition vers l'univers
   if (getAdditionalName()=="killed") {
      Loader::LoadScene("vignette_hands_middleage");
      Loader::LoadScene("middleage_mainscreen");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_priest_nobible", true);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_room2_priest_bible", "house_room2_priest_nobible");
      int x,y;
      GetObjectPosition("house_room2_priest_nobible", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_PRIEST_ICANTRUSTYOU"), "", false, false, _lpGhostVoice);
      // Cinématique
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands_middleage", "middleage", false);
      return;
   }
   
   // Apparition du fantôme
   if (_strSceneAdditionalName == "reveal_ghost") {
      int x,y;
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      ESoundBank::getSound("successisland")->playSample();
      ESoundBank::getSound("ghostappear")->playSample();
      _lpSceneDirector->getSequencer()->Energy(NULL, 928, 460, 92/2, 58/2, 407, 354, 1);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_priest_nobible", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_room2_priest_nobible", true);
      GetObjectPosition("house_room2_priest_nobible", x, y, true, false);

      // Spécial anniv
//      _lpSceneDirector->getSequencer()->PlaySound(NULL, "happybirthday");
//      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, "JOYEUX ANNIVERSAIRE A GUILLEM !!!", "", true);
//      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, "JOYEUX ANNIVERSAIRE A GUILLEM !!!", "", true);
//      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, "JOYEUX ANNIVERSAIRE A GUILLEM !!!", "", true);
//
//      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      // fin anniv
      
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_PRIEST_APPEAR"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ROOM2_ASHLEY_CALMDOWN"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_PRIEST_APPEAR2"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_PRIEST_APPEAR3"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ROOM2_ASHLEY_FOUNDBIBLE"), "", false, true);
      GetObjectPosition("house_room2_isaac_quiet", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_ISAAC_WOOF"), "", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      ResolveTask("task_house_room2_contactghost");
      AddTask("task_house_room2_getbible");
      AddObjective("house","bible");
      AddHint("house","bible","where");

      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_room1.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_ROOM1", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
   }
   
   // Prêtre
   if ((TaskToResolve("task_house_room2_getbible") || TaskToResolve("task_house_room2_givebible")) && _strSceneAdditionalName != "reveal_ghost") {
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_priest_nobible", true);
      SetVisible("house_room2_priest_nobible", true);
   }
   else if (TaskResolved("task_house_room2_givebible") && !TaskToResolve("task_house_room2_killpoltergeist")) {
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_priest_bible", true);
      SetVisible("house_room2_priest_bible", true);
   }
   
   if (TaskResolved("room2_flask")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED") ,"",true);
   }

   setAdditionalName("");   
}

void SceneRoom2::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      _lpSceneDirector->GoToScene("house_room2","backfrombeyond");
   }
   if (KInput::isPressed(K_VK_F7))
   {
      ObjectClicked("house_room2_priest_underattack", 0, 0);
   }
#endif
}

void SceneRoom2::Logic()
{
	EScene::Logic();
}

void SceneRoom2::Draw()
{
   EScene::Draw();
}

void SceneRoom2::Close()
{
   ESoundBank::StopSample("poltergeistloop");
}

bool SceneRoom2::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Ouvre le lit
   if (strcmp(szObjectName, "house_room2_bedclosed") == 0) {
      ESoundBank::getSound("success")->playSample();
      SetVisible(szObjectName, false);
      SetVisible("house_room2_bedopen", true);
      SetVisible("house_room2_frag07", true);
      ResolveTask("task_house_room2_openbed");
      return true;
   }
   
   // Fiole
   if (strcmp(szObjectName, "room2_flask") == 0) {
      PickupSimple(szObjectName, "inv_room2_flask");
      StopEmitter("flake_flask");
      return true;
   }
   
   // Fragments
   if (strcmp(szObjectName, "house_room2_frag05") == 0) {
      PickupSimple(szObjectName, "inv_room2mirror_frag05");
      return true;
   }
   if (strcmp(szObjectName, "house_room2_frag06") == 0) {
      PickupSimple(szObjectName, "inv_room2mirror_frag06");
      return true;
   }
   if (strcmp(szObjectName, "house_room2_frag07") == 0) {
      PickupSimple(szObjectName, "inv_room2mirror_frag07");
      return true;
   }
   if (strcmp(szObjectName, "house_room2_frag08") == 0) {
      PickupSimple(szObjectName, "inv_room2mirror_frag08");
      return true;
   }
   
   // Table de nuit
   if (strcmp(szObjectName, "house_room2_bedside_closed") == 0) {
      _lpSceneDirector->GoToScene("house_room2_bedside", "", false);
      return true;
   }
   
   // Mirroir
   if (strcmp(szObjectName, "house_room2_mirrorzone") == 0) {
      _lpSceneDirector->GoToScene("house_room2_mirror", "", false);
      return true;
   }
   
   // Prêtre sans bible
   if (strcmp(szObjectName, "house_room2_priest_nobible") == 0) {
      int x,y;
      GetObjectPosition("house_room2_priest_nobible", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_PRIEST_NOBIBLE"), "", true, false, _lpGhostVoice);
      return true;
   }

   // Prêtre avec bible
   if (strcmp(szObjectName, "house_room2_priest_bible") == 0) {
      if (TaskToResolve("task_house_room2_expelevil")) {
         int x,y;
         GetObjectPosition("house_room2_priest_nobible", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk("cannotprey", x, y, KStr("ROOM2_PRIEST_EVIL"), "", true, false, _lpGhostVoice);
         return true;
      }
   }

   // Clic sur le poltergeist
   if (strcmp(szObjectName, "house_room2_priest_underattack") == 0 || strcmp("house_room2_poltergeist", szObjectName) == 0) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_room2_poltergeist", false, true);
      ESoundBank::StopSample("poltergeistloop");
      EMiniJeuPoltergeist *_lpMiniJeu = new EMiniJeuPoltergeist();
      _lpSceneDirector->GoToMiniGame("poltergeist",_lpMiniJeu,"poltergeist");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_poltergeist.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_POLTERGEIST", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return true;
   }
   
   // Isaac
   if (strcmp(szObjectName, "house_room2_isaac_quiet") == 0)
   {
      int x,y;
      GetObjectPosition("house_room2_isaac_quiet", x, y, true, false);
      // Doit chercher la bible ?
      if (TaskToResolve("task_house_room2_getbible")) {
//         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ROOM2_ASHLEY_SEEK"), "", false, false);
//         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_ISAAC_WOOF"), "", false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
//         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
         _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_room2_isaac_quiet", "house_room2_isaac_seek");
         _lpSceneDirector->getSequencer()->Wait(NULL, 3000);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_room2_isaac_seek", "house_room2_isaac_bible");
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         return true;
      }
      else {
         if (!TaskResolved("task_house_room2_openbedside")) {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ROOM2_ISAAC_LIGHTBEDSIDE"), "", true);
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_ISAAC_WOOF"), "", true);
         }
         return true;
      }
   }
   if (strcmp(szObjectName, "house_room2_isaac_bible") == 0)
   {
      ResolveTask("task_house_room2_getbible");
      AddTask("task_house_room2_givebible");
      PickupSimple("house_room2_isaac_bible", "inv_room2_bible");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_room2_isaac_bible", "house_room2_isaac_quiet");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ROOM2_ASHLEY_THANKSISAAC"), "", true, false);
      return true;
   }
   
   return false;
}

bool SceneRoom2::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool SceneRoom2::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Donne la bible au prêtre
   if (strcmp(szObjectName, "house_room2_priest_nobible") == 0) {
      if (strcmp(szItemName, "inv_room2_bible") == 0) {
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         ESoundBank::getSound("success")->playSample();
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_house_room2_givebible");
         ResolveObjective("house","bible");
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_priest_nobible", false);
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_priest_bible", true);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_room2_priest_nobible", "house_room2_priest_bible");
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ROOM2_ASHLEY_HEREBIBLE"), "", true, true);
         int x,y;
         int x2,y2;
         GetObjectPosition("house_room2_priest_nobible", x, y, true, false);
         GetObjectPosition("house_room2_priest_nobible", x2, y2, true, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_PRIEST_EVIL"), "", false, false, _lpGhostVoice);
         _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_room2mirror_frag00", x2, y2, 1);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ROOM2_PRIEST_REPAIR"), "", false, false, _lpGhostVoice);
//         SetVisible("house_room2_mirrorghost", true);
         AddTask("task_house_room2_expelevil");
         AddTask("task_house_room2_repairmirror");
         AddObjective("house","expelroom2");
         AddHint("house","expelroom2","how");
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);

         return true;
      }
   }
   return false;
}

void SceneRoom2::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (bIsRevolved) {
      // Tâche résolue
      ResolveTask("task_house_room2_killpoltergeist");
      ResolveObjective("house","expelroom2");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_priest_underattack", false);
      SetVisible("house_room2_priest_underattack", false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_room2_poltergeist", false);
      ESoundBank::StopSample("poltergeistloop");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_priest_bible", true);
      SetVisible("house_room2_priest_bible", true);
      setAdditionalName("killed");
      Init();
   }
}
