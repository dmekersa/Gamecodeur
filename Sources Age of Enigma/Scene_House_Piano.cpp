/*
 *  Scene_House_Piano.cpp
 *  enigma
 *
 *  Created by Rockford on 15/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Piano.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuPiano.h"

/* Constructeur */
Scene_House_Piano::Scene_House_Piano(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _nSequencePuzzle = 0;
   EMiniJeuPiano::Preload();
}

/* Destructeur */
Scene_House_Piano::~Scene_House_Piano()
{
}

void Scene_House_Piano::Init()
{
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE);

   if (!TaskResolved("task_piano_firstvisit")) {
      ResolveTask("task_piano_firstvisit");
   }
   
   if (TaskResolved("task_house_livingroom_scorepiano")) {
      SetVisible("house_piano_pianoscore", true, true);
      SetVisible("pianoscore_zone", false);
   }
   
   if (TaskResolved("task_house_livingroom_playpiano")) {
      SetupItem("house_statuette_part_jaw");
   }
   else {
      AddTask("task_house_livingroom_playpiano");
   }
   
   if (TaskResolved("task_house_getjaw")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED"), "", true);
   }
}

void Scene_House_Piano::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_House_Piano::Logic()
{
	EScene::Logic();
}

void Scene_House_Piano::Draw()
{
   EScene::Draw();
}

void Scene_House_Piano::Close()
{
}

bool Scene_House_Piano::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "house_statuette_part_jaw") == 0) {
      PickupSimple("house_statuette_part_jaw", "inv_house_statuette_part_jaw");
      ResolveTask("task_house_getjaw");
      return true;
   }
   
   if (strcmp(szObjectName, "piano_key1") == 0
       || strcmp(szObjectName, "piano_key2") == 0
       || strcmp(szObjectName, "piano_key3") == 0
       || strcmp(szObjectName, "piano_key4") == 0
       || strcmp(szObjectName, "house_piano_pianoscore") == 0
       )
   {
      if (!TaskResolved("task_house_livingroom_playpiano") && TaskResolved("task_house_livingroom_scorepiano")) {
         EMiniJeuPiano *_lpMiniJeu = new EMiniJeuPiano();
         _lpSceneDirector->getSequencer()->MiniGame(NULL, "livingroom_piano", _lpMiniJeu);
      }
      else {
         if (strcmp(szObjectName, "piano_key1") == 0)
         {
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_1");
         }
         else if (strcmp(szObjectName, "piano_key2") == 0)
         {
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_2_1");
         }
         else if (strcmp(szObjectName, "piano_key3") == 0)
         {
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_3");
         }
         else if (strcmp(szObjectName, "piano_key4") == 0)
         {
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_4_1");
         }
      }
   }

//   if (strcmp(szObjectName, "piano_key1") == 0)
//   {
//      if (_nSequencePuzzle == 0) {
//         _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_1");
//         _nSequencePuzzle++;
//      }
//      else {
//         _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_1");
//         _nSequencePuzzle = 1;
//      }
//   }
//   else if (strcmp(szObjectName, "piano_key2") == 0) {
//      if (_nSequencePuzzle == 3 || _nSequencePuzzle == 5) {
//         _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_2_2");
//         _nSequencePuzzle++;
//      }
//      else {
//         _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_2_1");
//         _nSequencePuzzle = 0;
//      }
//   }
//   else if (strcmp(szObjectName, "piano_key3") == 0) {
//      if (_nSequencePuzzle == 2) {
//         _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_3");
//         _nSequencePuzzle++;
//      }
//      else {
//         _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_3");
//         _nSequencePuzzle = 0;
//      }
//   }
//   else if (strcmp(szObjectName, "piano_key4") == 0) {
//      if (_nSequencePuzzle == 1 || _nSequencePuzzle == 4) {
//         _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_4_2");
//         _nSequencePuzzle++;
//      }
//      else {
//         _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_4_1");
//         _nSequencePuzzle = 0;
//      }
//   }
   
//   // Séquence complète ?
//   if (!TaskResolved("task_house_livingroom_playpiano") && _nSequencePuzzle == 6) {
//      _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
//      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
//      _lpSceneDirector->getSequencer()->Wait(NULL, 1500);
//      _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_full");
//      _lpSceneDirector->getSequencer()->Wait(NULL, 6000);
//      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
//      ResolveTask("task_house_livingroom_playpiano");
//      RemoveHint("house","living_buildhead","jaw");
//      RemoveHint("house","living_buildhead","piano");
//      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_statuette_part_jaw", true);
//      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
//      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
//   }
   
   return false;
}

bool Scene_House_Piano::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Scene_House_Piano::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (strcmp(szItemName, "inv_house_score") == 0) {
      if (strcmp(szObjectName, "pianoscore_zone") == 0) {
         SetVisible("pianoscore_zone", false);
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_piano_pianoscore", true);
         ResolveTask("task_house_livingroom_scorepiano");
//         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_PIANO_SCORE_INFO"),"", true, false);
         EMiniJeuPiano *_lpMiniJeu = new EMiniJeuPiano();
         _lpSceneDirector->getSequencer()->MiniGame(NULL, "livingroom_piano", _lpMiniJeu);
//         MiniGameDone("livingroom_piano", true);
         return true;
      }
   }
   
   return false;
}

void Scene_House_Piano::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FAILED"), "", true);
      return;
   }

   if (strcmp(szGameName, "livingroom_piano")==0 && bIsRevolved) {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1500);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "pianocode_full");
      _lpSceneDirector->getSequencer()->Wait(NULL, 6000);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      ResolveTask("task_house_livingroom_playpiano");
      RemoveHint("house","living_buildhead","jaw");
      RemoveHint("house","living_buildhead","piano");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_statuette_part_jaw", true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
   }
}
