/*
 *  Scene_House_Kitchen.cpp
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Egypt_Tomb.h"
#include "ESceneDirector.h"
#include "ThreadLoader.h"

/* Constructeur */
Scene_Egypt_Tomb::Scene_Egypt_Tomb(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_Egypt_Tomb::~Scene_Egypt_Tomb()
{
   delete _lpBgGraphic;
}

void Scene_Egypt_Tomb::Init()
{
   SetupItem("egypt_symbol_god01h");
   
   // Arrivée et étonnement de NEFERE, rencontre avec HERES sa mère
   if (!TaskResolved("task_egypt_entertomb")) {
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      ResolveTask("task_egypt_entertomb");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_NEFERES_ARRIVETOMB"), "", false, true);
      ResolveTask("task_egypt_meetheres");
      // Début de la cinématique !
      int x,y;
      GetObjectPosition("egypt_tomb_neres", x, y, true, false);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_tomb_neres", true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_NEFERES_MEETHERES1"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_HERES_MEETHERES1"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_HERES_MEETHERES2"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_NEFERES_MEETHERES2"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_HERES_MEETHERES3"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_HERES_MEETHERES4"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_HERES_MEETHERES5"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_NEFERES_MEETHERES3"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_HERES_MEETHERES6"), "", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      AddTask("task_egypt_purifying");
      AddObjective("egypt","ritual");
      AddHint("egypt","ritual","how");
      AddHint("egypt","ritual","order");
      return;
   }
   
   if (TaskResolved("task_egypt_meetheres")) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_tomb_neres", true);
   }
   
   // Fini !
   if (getAdditionalName() == "completed") {
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_scab1",false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_scab2",false);
      // Animation pour symboliser la purification
      ResolveTask("task_egypt_universeresolved");
      //_lpSceneDirector->getMap()->setUniversVisited("UniverseEgypt");
      ResolveObjective("egypt","ritual");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      _lpSceneDirector->getSequencer()->PlaySound(NULL,"inca_eclipse");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_tomb_statue2", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_tomb_glow", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_NEFERES_FINAL1"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_NEFERES_FINAL2"), "", false, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      // Retour
      Loader::LoadScene("bathroom");
      _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "blank", true, true);
      _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/soul_tunnel_r.ogv","tunnel_r");
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"bathroom","backfrombeyond", false);
   }
}

void Scene_Egypt_Tomb::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   
   if (KInput::isPressed(K_VK_F6))
   {
      _lpSceneDirector->GoToScene("egypt_tomb", "completed", false);
   }
#endif
}

void Scene_Egypt_Tomb::Logic()
{
	EScene::Logic();
}

void Scene_Egypt_Tomb::Draw()
{
   EScene::Draw();
}

void Scene_Egypt_Tomb::Close()
{
   StopEmitter("creeping_glow");
}

bool Scene_Egypt_Tomb::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "egypt_symbol_god01h") == 0) {
      PickupSimple(szObjectName, "inv_egypt_god01");
   }
   
   if (strcmp(szObjectName, "egypt_tomb_neres") == 0)
   {
      int x,y;
      GetObjectPosition("egypt_tomb_neres", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_HERES_MEETHERES6"), "", true);
      return true;
   }
   return false;
}

bool Scene_Egypt_Tomb::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

void Scene_Egypt_Tomb::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "") == 0 && bIsRevolved) {
      ESoundBank::getSound("success")->playSample();
      ResolveTask("");
   }
   
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
