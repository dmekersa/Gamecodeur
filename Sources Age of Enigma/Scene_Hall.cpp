/*
 *  SceneHall.cpp
 *  enigma
 *
 *  Created by Rockford on 28/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Hall.h"
#include "ESceneDirector.h"
#include "MyGame.h"
#include "GlobalBank.h"
#include "SoundBank.h"
#include "EMiniJeuConnect.h"

SceneHall::SceneHall(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _bForceExit = false;
   _bEscape = false;
   _bNeedIHM = true;
   
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_HOUSE);
   
   _lpProgress = new KCounter ();
   _lpProgress->startCounter(0, 80, 0, 20000, K_COUNTER_EASEIN);
   EMiniJeuConnect::Preload();
}

SceneHall::~SceneHall()
{
   XDELETE(_lpProgress);
}

void SceneHall::Init()
{
   _bNeedIHM = true;
   SetVisible("halloff", true);
   SetVisible("nathan_shadow", true);
   SetVisible("hallon", false);
 
   SetupItem("hall_switchpieces[1]");
   SetupItem("hall_switchpieces[2]");
   SetupItem("hall_switchpieces[3]");
   SetupItem("house_score");
   
   // Plante
   if (!TaskResolved("task_house_getbrushwood")) {
      SetVisible("house_hall_plant", true, true);
   }
   else {
      SetVisible("house_hall_plant_done", true, true);
   }

   /* Le hall est encore éteint */
   if (!TaskResolved("task_house_hall_repair_switch"))
   {
      if (!TaskResolved("task_house_hall_ahsleyintrocomment"))
      {
         int x, y;
         GetObjectPosition("nathan_shadow", x, y, true, false);
         _lpSceneDirector->getSequencer()->Wait(NULL, 500);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_WHOSTHERE"), "nathan_hall_welcome", false);
//         _lpSceneDirector->getSequencer()->VoiceOver("welcome", KStr("HALL_NATHAN_WHOSTHERE"), "nathan_hall_welcome", SEQUENCE_VOICEOVER_RED);
         ResolveTask("task_house_hall_ahsleyintrocomment");
         AddObjective("house","hall_light");
         AddHint("house","hall_light","how");
      }
      AddTask("task_house_hall_repair_switch");
      StartEmitter("creeping_sparks");
      SetVisible("switchrepaired", false);
      SetVisible("switchbroken", true);
   }
   else {
      SetVisible("halloff", false);
      SetVisible("hallon", true);
      SetVisible("darkness", false);
      SetVisible("switchrepaired_on", true);
      SetVisible("switchbroken", false);
      Escape();
   }
   
   // Le hall est détruit
   if (TaskResolved("task_house_hall_escape_nathan"))
   {
      SetVisible("halldestroyed", true);
      StartEmitter("creeping_dust");
   }
   
   // Isaac
   if (TaskResolved("task_house_hall_escape_nathan") && !TaskResolved("task_house_hall_groundfloor") && !_bEscape) {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "hall_dog_cry");
      SetVisible("hall_isaac_waiting", true, true);
   }
   
   // Aide pour trouver la cuisine
   if (TaskToResolve("task_house_dining_stopfire") && !TaskResolved("task_house_kitchen_fill_pan")) {
//      _lpSceneDirector->getSequencer()->Talk("kitchen", CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_NEEDWATER"), "", false);
   }
   
   // Porte de la salle à manger ouverte ou fermée ?
   if (ObjectiveResolved("house","helpnatsumi")) {
      SetVisible("diningroom", false);
      SetVisible("house_hall_diningdooropen", true);
   }
   else {
      SetVisible("diningroom", true);
      SetVisible("house_hall_diningdooropen", false);
   }
}

void SceneHall::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _bForceExit = true;
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      ResolveTask("task_house_hall_repair_switch");
      ResolveTask("task_house_hall_escape_nathan");
      ResolveTask("task_house_hall_light");
      ResolveTask("task_gethexagramme");
      _lpSceneDirector->SetHexagramme(true);
      Init();
   }
   if (KInput::isPressed(K_VK_F7))
   {
      SetVisible("diningroom", false);
      SetVisible("house_hall_diningdooropen", true);
   }
#endif
}

void SceneHall::Logic()
{
	EScene::Logic();
}

void SceneHall::Draw()
{
   EScene::Draw();
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   _lpProgress->move(fElapsed);
}

void SceneHall::Close()
{
   
}

bool SceneHall::AskForClose()
{
   return true;
}

bool SceneHall::ObjectClicked(const char *szObjectName, float x, float y)
{
//#ifdef BFG_TEASING
//   if (strcmp(szObjectName, "livingroom") == 0)
//   {
//      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, "This door is closed.", "", true);
//      return true;
//   }
//#endif
   
   if ( strcmp(szObjectName, "corridor") == 0 && !TaskResolved("task_house_hall_light"))
   {
      _lpSceneDirector->getSequencer()->VoiceOver("ashleyhall1", KStr("HALL_ASHLEY_DARK"), "");
      /* On prend la main, on ne laisse pas passer */
      return true;
   }
   else if ( strcmp(szObjectName, "livingroom") == 0 && !TaskResolved("task_house_hall_light"))
   {
      _lpSceneDirector->getSequencer()->VoiceOver("ashleyhall1", KStr("HALL_ASHLEY_DARK"), "");
      /* On prend la main, on ne laisse pas passer */
      return true;
   }
   else if ( strcmp(szObjectName, "diningroom") == 0 && !TaskResolved("task_house_hall_light"))
   {
      _lpSceneDirector->getSequencer()->VoiceOver("ashleyhall1", KStr("HALL_ASHLEY_DARK"), "");
      /* On prend la main, on ne laisse pas passer */
      return true;
   }
   else if ( strcmp(szObjectName, "diningroom") == 0 && !TaskResolved("task_inca_universeresolved"))
   {
      _lpSceneDirector->getSequencer()->Talk("ashleyhall1", CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_NOTNOWDINING"), "", true);
      /* On prend la main, on ne laisse pas passer */
      return true;
   }
   else if ( strcmp(szObjectName, "parvis") == 0 && !TaskResolved("task_house_hall_light"))
   {
      _lpSceneDirector->getSequencer()->VoiceOver("ashleyhall1", KStr("HALL_ASHLEY_DARK"), "");
      /* On prend la main, on ne laisse pas passer */
      return true;
   }
   else if ( strcmp(szObjectName, "parvis") == 0 && TaskResolved("task_house_hall_escape_nathan"))
   {
      _lpSceneDirector->getSequencer()->Talk("ashleystuck", CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_STUCK"),"", true);
      /* On prend la main, on ne laisse pas passer */
      return true;
   }
   else if ( strcmp(szObjectName, "upstairs") == 0 && !TaskResolved("task_universe_done_japan"))
   {
      _lpSceneDirector->getSequencer()->VoiceOver("ashleystuck", KStr("HALL_ASHLEY_STAIRSBROKEN"), "");
      /* On prend la main, on ne laisse pas passer */
      return true;
   }
   else if ( strcmp(szObjectName, "switch") == 0 || strcmp(szObjectName, "switchbroken") == 0 )
   {
      /* Interrupteur cassé */
      if (!TestGlobal("hallswitchrepaired"))
      {
         _lpSceneDirector->getSequencer()->VoiceOver("ashleyhallswitchbroke", KStr("HALL_ASHLEY_SWITCHBROKEN") ,"");
      }
   }
   else if ( strcmp(szObjectName, "switchrepaired") == 0 )
   {
      /* Allume l'interrupteur */
      if (!TaskResolved("task_house_hall_light"))
      {
         _bNeedIHM = false;
         ResolveObjective("house","hall_light");
         ResolveTask("task_house_hall_light");
         AddTask("task_house_findfirstghost");
         SetVisible("darkness", false);
         SetVisible("switchrepaired", false);
         SetVisible("switchrepaired_on", true);
         ESoundBank::getSound("switch")->playSample();
         ESoundBank::getSound("reveal")->playSample();
         _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan_shadow", "nathan_light");
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "halloff", "hallon");
         Escape();
      }
   }
   /* Essaye d'éteindre */
   else if ( strcmp(szObjectName, "switchrepaired_on") == 0 )
   {
      _lpSceneDirector->getSequencer()->Talk("ashleyhallturnoff", CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_SWITCHOFF") ,"", true);
   }
   /* Morceaux de l'interrupteur                      */
   else if ( strcmp(szObjectName, "hall_switchpieces[1]") == 0 || strcmp(szObjectName, "hall_switchpieces[2]") == 0 || strcmp(szObjectName, "hall_switchpieces[3]") == 0 )
   {
      PickupMultiple(szObjectName,"inv_switchpieces",3);
   }
   /* Peintures au mur */
   else if ( strcmp(szObjectName, "painting1") == 0 )
   {
      _lpSceneDirector->getSequencer()->Talk("ashleyhallpainting1", CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_ARTCRITIC"),"", true);
   }
   /* Tente de cliquer dans l'obscurité */
   else if ( strcmp(szObjectName, "darkness") == 0 )
   {
      _lpSceneDirector->getSequencer()->VoiceOver("ashleyhall1", KStr("HALL_ASHLEY_DARK"), "");
   }
   // La plante
   else if (strcmp(szObjectName, "house_hall_plant") == 0) {
      _lpSceneDirector->getSequencer()->VoiceOver("ashleyhall1", KStr("HALL_ASHLEY_PLANT"),"");
   }
   // La partition
   else if (strcmp(szObjectName, "house_score") == 0) {
      PickupSimple("house_score", "inv_house_score");
   }
   
   return false;
}

bool SceneHall::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool SceneHall::ItemUsed(const char *szItemName, const char *szObjectName)
{
   int x,y;
   
   // Le joueur coupe des brindilles avec le couteau
   if (strcmp(szItemName, "inv_house_knife") == 0) {
      if (strcmp(szObjectName, "house_hall_plant") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         GetObjectPosition("house_hall_plant", x, y, true, true);
         _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_house_brushwood", x, y, -1);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_hall_plant", "house_hall_plant_done");
         ResolveTask("task_house_getbrushwood");
         return true;
      }
   }
   
   // Le joueur utilise les morceaux de pièces de puzzle
   if ( strcmp(szItemName, "inv_switchrepaired") == 0 &&
       ( strcmp(szObjectName, "switch") == 0 || strcmp(szObjectName, "switchbroken") == 0) ) {
      /* Répare l'interrupteur */
      if (!TestGlobal("hallswitchrepaired"))
      {
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
         _lpSceneDirector->getSequencer()->VoiceOver("ashleyhallrepaired", KStr("HALL_ASHLEY_SWITCHREPAIRED") ,"");
         EGlobalBank::AddStrValue("hallswitchrepaired", "1");
         ESoundBank::getSound("success")->playSample();
         StopEmitter("creeping_sparks");
         SetVisible("switchbroken", false);
         SetVisible("switchrepaired", true);
         // On peut maintenant retirer l'objet de l'inventaire
         _lpSceneDirector->DropItem("inv_switchrepaired");
         // Tâche résolue
         ResolveTask("task_house_hall_repair_switch");
         AddTask("task_house_hall_light");
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", false);
         return true;
      }
   }
   return false;
}

void SceneHall::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "connect")==0 && bIsRevolved) {
//      _lpSceneDirector->getSequencer()->Talk("ashleyhallrepaired", CHARACTER_POSX, CHARACTER_POSY, KStr("HALL_ASHLEY_SWITCHREPAIRED") ,"",true);
//      EGlobalBank::AddStrValue("hallswitchrepaired", "1");
//      ESoundBank::getSound("success")->playSample();
//      StopEmitter("sparks");
//      AddEmitter("bling");
//      SetVisible("switchbroken", false);
//      SetVisible("switchrepaired", true);
//      // On peut maintenant retirer l'objet de l'inventaire
//      _lpSceneDirector->DropItem("inv_switchrepaired");
//      // Tâche résolue
//      ResolveTask("task_house_hall_repair_switch");
   }
}

void SceneHall::Escape()
{
   int x, y;
   GetObjectPosition("nathan_shadow", x, y, true, false);
   _bEscape = true;
   // Evasion de nathan
   ResolveTask("task_house_hall_escape_nathan");
   _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("NATHAN1_SPEECH_2"), "nathan_hall_iwaswaiting", false);
   _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("NATHAN1_SPEECH_3"), "ashley_hall_butwhoare", false, true);
   _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("NATHAN1_SPEECH_4"), "nathan_hall_imnathan", false);
   _lpSceneDirector->getSequencer()->PickupItem(NULL,"inv_hexagramme", 1024/2, 768/2, 1);
   _lpSceneDirector->getSequencer()->ShowImage(NULL, "neckless", true, false);
   _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("NATHAN1_SPEECH_5"), "nathan_hall_thefrat", false);
   _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("NATHAN1_SPEECH_6"), "ashley_hall_butidont", false, true);
   _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("NATHAN1_SPEECH_7"), "nathan_hall_youalready", false);
   _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("NATHAN1_SPEECH_8"), "nathan_hall_animportant", false);
   _lpSceneDirector->getSequencer()->EarthQuake(NULL,true, "hall_earthquake");
   _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_fragments", true);
   _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_plaster", true);
   _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_nathan_shadow", true);
   _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("NATHAN1_SPEECH_9"), "nathan_hall_tothevery", false);
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
   _lpSceneDirector->getSequencer()->ShowImage(NULL, "neckless", false, false);
   _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
   _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_dust", true);
   _lpSceneDirector->getSequencer()->EarthQuake("earthquakehall",true,"hall_earthquake");
   _lpSceneDirector->getSequencer()->PlaySound(NULL, "hall_dog_barkshort");
   _lpSceneDirector->getSequencer()->PlaySound(NULL, "hall_dog_barklong");
   _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan4", "nathan5");
   _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan5", "nathan6");
   _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan6", "nathan7");
   _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan7", "nathan8");
   _lpSceneDirector->getSequencer()->SwitchImage(NULL, "nathan8", "nathan9");
   _lpSceneDirector->getSequencer()->GoToScene(NULL, "halldoor", "", false);
}