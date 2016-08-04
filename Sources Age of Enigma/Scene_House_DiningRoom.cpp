/*
 *  SceneDiningRoom.cpp
 *  enigma
 *
 *  Created by Rockford on 29/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_House_DiningRoom.h"
#include "ESceneDirector.h"
#include "MyGame.h"
#include "GlobalBank.h"
#include "MusicBank.h"
#include "EMiniJeuTemplate.h"
#include "EMiniJeuTresor.h"
#include "ThreadLoader.h"

/* Constructeur */
SceneDiningRoom::SceneDiningRoom(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _lpGhostVoice = EMusicBank::getMusic("ghost_longvoice_island");
   _lpGhostVoice->setVolume(0);
   _lpGhostVoice->playStream(true);

   EMiniJeuTemplate::Preload();
   EMiniJeuTresor::Preload();
}

/* Destructeur */
SceneDiningRoom::~SceneDiningRoom()
{
   delete _lpBgGraphic;
   _lpGhostVoice->stopStream();
}

void SceneDiningRoom::Init()
{
   ResolveTask("task_house_findfirstghost");
   
#ifdef BFG_TEASING
   _lpSceneDirector->getSequencer()->GoToScene(NULL, "teasing", "", false);
   return;
#endif
   
   // Etats des objets ramassables
   
   SetupItem("envelope_diningroom");
   
   // Le cadre sur le meuble du fond
   if (!TestGlobal("frame_dresser")) {
      SetVisible("frame_dresser", true);
   }
   else {
      SetVisible("frame_dresser", false);
   }
   
   // Cadre au mur inactif si 1er tiroir résolu
   if (TaskResolved("task_house_dining_firstdrawer")) {
      GetObjectImageByName("frame_wall")->EnableGlitch(false);
   }
   
   // Livres inactifs si 2ème tiroir résolu
   if (TaskResolved("task_house_dining_seconddrawer")) {
      SetVisible("hall_books", false);
   }
   
   // Le cadre au mur
   if (TaskResolved("task_house_dining_putframe")) {
      SetVisible("frame_wall", true);
      StopEmitter("flake_framewall");
      if (TaskResolved("task_house_dining_firstdrawer")) {
         GetObjectImageByName("frame_wall")->EnableGlitch(false);
      }
   }
   else {
      SetVisible("frame_wall", false);
      StartEmitter("flake_framewall");
   }
   
   // Le glit de la pendule
   if (TaskResolved("task_house_livingroom_getmast")) {
      SetVisible("hall_books", false);
      SetVisible("hall_clock", false);
   }
   
   // Le vase
   if (TaskResolved("task_house_dining_putvase")) {
      SetVisible("vase_dresser", true);
      SetVisible("vase_trace", false);
      SetVisible("vase_left", false);
      StopEmitter("flake_vasedresser");
   }
   else {
      SetVisible("vase_dresser", false);
      SetVisible("vase_trace", true);
      if (TestGlobal("frame_dresser")) {
         SetVisible("vase_trace", true);
         StartEmitter("flake_vasedresser");
      }
      else {
         SetVisible("vase_trace", false);
         StopEmitter("flake_vasedresser");
      }
   }
   // Les morceaux de vase
   SetupItem("dining_vasepieces[1]");
   SetupItem("dining_vasepieces[2]");
   SetupItem("dining_vasepieces[3]");
   SetupItem("dining_vasepieces[4]");
   SetupItem("dining_vasepieces[5]");
   SetupItem("dining_vasepieces[6]");
   // Le bateau
   if (!TaskResolved("task_house_dining_repairboat")) { SetVisible("boatbroken", true); }
   else {
      if (!TaskResolved("task_house_dining_taketreasure")) {
         SetVisible("boatrepaired", true);
         SetVisible("boat", false);
      }
   }
   
   if (TaskResolved("task_island_universeresolved")) {
      SetupItem("diningroom_hook");
      if (isVisible("diningroom_hook")) {
         StartEmitter("flake_hook");
      }
   }      

   // Le tapis brulé
   SetVisible("firezone", false);
   StopEmitter("fire_carpet");
   if (TaskResolved("task_house_dining_stopfire")) {
      SetVisible("carpet_burn", true);
      if (!TestGlobal("carpet_key")) {
         SetVisible("carpet_key", true);
      }
   }
   else {
      SetVisible("carpet_burn", false);
      SetVisible("carpet_key", false);
      if (TaskToResolve("task_house_dining_stopfire")) {
         StartEmitter("fire_carpet");
         SetVisible("firezone", true);
         int x,y;
         GetObjectPosition("pirate_sit", x, y);
         _lpSceneDirector->getSequencer()->Talk("hemousaillon1", x, y, KStr("DININGROOM_MURRAY_BAILLE"), "", true, false, _lpGhostVoice);
      }
   }

   // Le fauteuil et le fantôme
   if (!TaskResolved("task_house_dining_contactghost")) {
      StopEmitter("flake_chair");
      ResolveTask("task_house_dining_contactghost");
      int x,y;
      GetObjectPosition("pirate_sit", x, y);
      // Affiche le fantome dans son fauteuil
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_pirate_static", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 4000);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostappear");
      _lpSceneDirector->getSequencer()->ShowImage("pirate_sit appear","pirate_sit",true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_HELLO"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_ASHLEY_MYNAME"),"", true, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_AREYOU"), "", true, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_ASHLEY_NOTEXACT"),"", true, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_SHOWME"), "", true, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_ASHLEY_HOW"),"", true, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_HOW"), "", true, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->NarrationMode("ghost appear dialog stop", SEQUENCE_NARRATION_DIALOG, false);
      _lpSceneDirector->getSequencer()->NarrationMode("ghost appear sequence done", SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_ASHLEY_OKLETSTRY"),"", true, false);
      AddObjective("house","tinyboat");
      AddHint("house","tinyboat","how");
      AddTask("task_house_dining_putvase");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_dining1.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_DINING1", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
   } else {
      StopEmitter("flake_chair");
      if (!TaskResolved("task_house_dining_givetreasure")) {
         SetVisible("pirate_sit", true);
         StartEmitter("creeping_pirate_static");
      }
   }

   // La vitrine de droite
   if (!TaskResolved("task_house_dining_openrightwindow")) {
      SetVisible("rightwindow_open",false);
      SetVisible("rightwindow_closed",true);
   }
   else {
      SetVisible("rightwindow_open",true);
      SetVisible("rightwindow_closed",false);
   }

   /*********************/
   /* PROLOGUE COMPLETE */
   /*********************/
   if (getAdditionalName() == "completed") {
      Loader::LoadScene("vignette_hands_island");
      Loader::LoadScene("island_beach");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
      SetVisible("pirate_sit",true,true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_pirate_static", false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"pirate_sit",false);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_pirate_standup", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostappear");
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"pirate_standup",true);         
      int x,y;
      GetObjectPosition("pirate_standup", x, y);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_BRAVO"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_TRUST"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_PRISONER"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_ASHLEY_IMEDIUM"),"", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_HOWCANYOU"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_ASHLEY_GIVEME"),"", false, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands_island", "island", false);
      return;
   }
      
   /************/
   /* EPILOGUE */
   /************/
   if (getAdditionalName() == "backfrombeyond") {
      ResolveObjective("house","freemurray");
      SetVisible("boat", false);
      SetVisible("pirate_sit", false);
      StopEmitter("creeping_pirate_static");
      int x,y;
      x = 1024/2;
      y = 768/2;
      SetVisible("black", true, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_pirate_standup", true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_EPILOG1"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_EPILOG2"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostgone");
      _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/reveil.ogv","frombeyond");
      SetupItem("diningroom_hook");
      StartEmitter("flake_hook");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_pirate_standup", false);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls",true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_shadow",true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "attic_nathanappear");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_diningroom_nathan", true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls",false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      GetObjectPosition("house_diningroom_nathan", x, y, true, false);

      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET3_1"), "HALL_NATHAN_MEET3_1", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET3_2"), "HALL_NATHAN_MEET3_2", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY,
                                             KStr("HALL_ASHLEY_MEET3_3"), "hall_ashley_meet3_3", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET3_4"), "HALL_NATHAN_MEET3_4", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY,
                                             KStr("HALL_ASHLEY_MEET3_5"), "hall_ashley_meet3_5", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET3_6"), "HALL_NATHAN_MEET3_6", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY,
                                             KStr("HALL_ASHLEY_MEET3_7"), "hall_ashley_meet3_7", false, true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_souls",true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HALL_NATHAN_MEET3_8"), "HALL_NATHAN_MEET3_8", false);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostgone");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_diningroom_nathan", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_nathan_shadow",false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      ResolveTask("task_freeghost_done_island");
      AddTask("task_house_basement_opendoor");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_dining2.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_DINING2", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_nathan3.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_NATHAN3", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return;
   }

   // On force le passage à l'univers, en cas de plantage/quit prématuré entre la scène et l'univers
   if (TaskResolved("task_island_prologcompleted") && !TaskResolved("task_island_universeresolved")) {
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands_island", "island", false);
   }
   
   // On a tout terminé
   if ( TaskResolved("task_island_universeresolved") ) {
      SetVisible("boat", false);
      SetVisible("boatrepaired", true);
      GetObjectImageByName("boatrepaired")->EnableGlitch(false);
      StopEmitter("creeping_pirate_static");
      StopEmitter("creeping_pirate_standup");
      SetVisible("pirate_sit", false);
      SetVisible("pirate_standup", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED"), "", true);
      return;
   }
}

void SceneDiningRoom::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      if (!_lpSceneDirector->getInventory()->getItemFromInventory("inv_dining_treasure")) {
         _lpSceneDirector->getInventory()->AddItem("house","inv_dining_treasure");
      }
   }
   if (KInput::isPressed(K_VK_F7))
   {
      setAdditionalName("backfrombeyond");
      Init();
   }
#endif
}

void SceneDiningRoom::Logic()
{
	EScene::Logic();
}

void SceneDiningRoom::Draw()
{
   EScene::Draw();
}

void SceneDiningRoom::Close()
{
   
}

bool SceneDiningRoom::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Clic sur le fauteuil : première apparition de Murray
//   if (strcmp(szObjectName, "armchair") == 0 && !TaskResolved("task_house_dining_contactghost")) {
   if (strcmp(szObjectName, "chairol") == 0 && !TaskResolved("task_house_dining_contactghost")) {
   }
   
   // Cadre posé sur le meuble du fond
   if (strcmp(szObjectName, "frame_dresser") == 0)
   {
      PickupSimple(szObjectName,"inv_house_framedining");
      StartEmitter("flake_vasedresser");
      SetVisible("vase_trace", true);
      AddTask("task_house_dining_putframe");
   }

   /* Morceaux de l'interrupteur */
   if ( strncmp(szObjectName, "dining_vasepieces", strlen("dining_vasepieces")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_vasepieces",6);
   }
   
   if ( strcmp(szObjectName, "envelope_diningroom") == 0)
   {
      PickupSimple(szObjectName, "ihm_envelope");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage("letterpaper.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("ENVELOPE_DININGROOM", FONT_DIARY_2, 0, 0);
      _lpSceneDirector->getDiaryPtr()->addImageToPage("logofraternity.png");
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return true;
   }   
   
   // Clé sous le tapis
   if (strcmp(szObjectName, "carpet_key") == 0)
   {
      PickupSimple(szObjectName, "inv_diningroom_rightwindowkey");
   }

   // Grapin
   if (strcmp(szObjectName, "diningroom_hook") == 0)
   {
      PickupMultiple(szObjectName, "inv_house_hook",-1);
      StopEmitter("flake_hook");
   }
   
   // Vitrine fermée
   if (strcmp(szObjectName, "rightwindow_closed") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk("stopfire", CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_INFO_CLOSED"), "", true);
   }

   // Feu
   if (strcmp(szObjectName, "firezone") == 0)
   {
      if (!TaskResolved("task_house_kitchen_fill_pan")) {
         _lpSceneDirector->getSequencer()->Talk("stopfire", CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_ASHLEY_NEEDWATER"), "", true);
      }
      else {
         _lpSceneDirector->getSequencer()->Talk("stopfire", CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_ASHLEY_USEPAN"), "", true);
      }

   }
   
   // Pendule
   if (strcmp(szObjectName, "hall_clock") == 0)
   {
      _lpSceneDirector->GoToVignette("vignette_diningroom_clock");
   }

   // Livres
   if (strcmp(szObjectName, "hall_books") == 0)
   {
      if (!TaskResolved("task_house_dining_openbook")) {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_diningroom_lib", "", false);
      }
      else {
         _lpSceneDirector->GoToVignette("vignette_diningroom_book", "", true);
      }
   }
   
   // Bateau 
   if (strcmp(szObjectName, "boatbroken") == 0)
   {
      if (!TestGlobal("dialogboatdining")) {
         SetGlobal("dialogboatdining","1");
      }
      _lpSceneDirector->getSequencer()->GoToScene("openboat", "vignette_diningroom_boat", "", true);
   }
   if (strcmp(szObjectName, "boatrepaired") == 0)
   {
      _lpSceneDirector->getSequencer()->GoToScene("openboat", "vignette_diningroom_boat", "", true);
   }

   // Vase de gauche
   if (strcmp(szObjectName, "vase_left") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk("infoboat", CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_ASHLEY_VASEBOAT"), "", true);
   }
   
   // Trace de tableau
   if (strcmp(szObjectName, "frame_trace") == 0)
   {
      _lpSceneDirector->getSequencer()->VoiceOver("frame_trace", KStr("DININGROOM_VOICEOVER_MISSING"), "");
   }
   
   // Tableau remis en place
   if (strcmp(szObjectName, "frame_wall") == 0)
   {
      _lpSceneDirector->GoToVignette("vignette_diningroom_picture");
   }
   
   // Trace de vase
   if (strcmp(szObjectName, "vase_trace") == 0)
   {
      _lpSceneDirector->getSequencer()->VoiceOver("miss", KStr("DININGROOM_VOICEOVER_MISSING"), "");
   }
   
   if (strcmp(szObjectName, "pirate_sit") == 0) {
      int x,y;
      GetObjectPosition("pirate_sit", x, y);
      // En attente de tout ranger
      if (TaskToResolve("task_house_dining_putvase"))
      {
         _lpSceneDirector->getSequencer()->Talk("task todo", x, y, KStr("DININGROOM_MURRAY_ORDER"), "", true, false, _lpGhostVoice);
      }
      else if (TaskToResolve("task_house_dining_stopfire")) {
         _lpSceneDirector->getSequencer()->Talk("task todo", x, y, KStr("DININGROOM_MURRAY_DANGERFIRE"), "", true, false, _lpGhostVoice);
      }
      else if (TaskToResolve("task_house_dining_openrightwindow")) {
         _lpSceneDirector->getSequencer()->Talk("task todo", x, y, KStr("DININGROOM_MURRAY_KEY"), "", true, false, _lpGhostVoice);
      }
      else if (TaskToResolve("task_house_dining_openleftwindow")) {
         _lpSceneDirector->getSequencer()->Talk("task todo", x, y, KStr("DININGROOM_MURRAY_NOTFINISH"), "", true, false, _lpGhostVoice);
      }
      else if (TaskToResolve("task_house_dining_firstdrawer")) {
         _lpSceneDirector->getSequencer()->Talk("task todo", x, y, KStr("DININGROOM_MURRAY_CLOCK"), "", true, false, _lpGhostVoice);
      }
      else if (TaskToResolve("task_house_dining_seconddrawer")) {
         _lpSceneDirector->getSequencer()->Talk("task todo", x, y, KStr("DININGROOM_MURRAY_2DRAWER"), "", true, false, _lpGhostVoice);
      }
      else if (TaskToResolve("task_house_dining_openboat")) {
         _lpSceneDirector->getSequencer()->Talk("task todo", x, y, KStr("DININGROOM_MURRAY_WHERETRE"), "", true, false, _lpGhostVoice);
      }
      else if (TaskResolved("task_house_dining_openboat")) {
         _lpSceneDirector->getSequencer()->Talk("infoboat2", x, y, KStr("DININGROOM_MURRAY_GIVEME"), "", true, false, _lpGhostVoice);
      }
      else {
         _lpSceneDirector->getSequencer()->Talk("task todo", x, y, KStr("DININGROOM_MURRAY_PROGRESS"), "", true, false, _lpGhostVoice);
      }

   }

   return false;
}

bool SceneDiningRoom::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool SceneDiningRoom::ItemUsed(const char *szItemName, const char *szObjectName)
{
   bool bTaskOrder = false;
   bool bResult = false;
   // Le joueur utilise la clé sur la vitrine de droite
   if ( strcmp(szItemName, "inv_diningroom_rightwindowkey") == 0 ) {
      if (strcmp(szObjectName, "rightwindow_closed") == 0 )
      {
         /* Ouvre la vitrine */
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"key_open");
         _lpSceneDirector->getSequencer()->ShowImage(NULL,"rightwindow_open",true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL,"rightwindow_closed",false);
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         // On peut maintenant retirer l'objet de l'inventaire
         _lpSceneDirector->DropItem("inv_diningroom_rightwindowkey");
         // Tâche résolue
         int x,y;
         GetObjectPosition("pirate_sit", x, y);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_NEAR"), "", true, false, _lpGhostVoice);
         AddTask("task_house_dining_findtreasure");
         ResolveTask("task_house_dining_openrightwindow");
         AddTask("task_house_dining_firstdrawer");
         RemoveHint("house","treasure","key");
         AddHint("house","treasure","firstdrawer");
         bResult = true;
      }
   }
   // Le joueur utilise le mat sur le bateau
   if ( strcmp(szItemName, "inv_dining_mast") == 0 ) {
      if (strcmp(szObjectName, "boatbroken") == 0 )
      {
         _lpSceneDirector->GoToVignette("vignette_diningroom_boat");
      }
   }
   // Le joueur utilise le trésor sur Murray
   if ( strcmp(szItemName, "inv_dining_treasure") == 0 ) {
      if (strcmp(szObjectName, "pirate_sit") == 0 )
      {
         // On peut maintenant retirer l'objet de l'inventaire
         ESoundBank::getSound("success")->playSample();
         _lpSceneDirector->DropItem("inv_dining_treasure");
         ResolveTask("task_house_dining_givetreasure");
         ResolveObjective("house","treasure");
         // Prologue terminé !!!
         ResolveTask("task_island_prologcompleted");
         setAdditionalName("completed");
         Init();
         bResult = true;
      }
   }
   // Le joueur utilise le cadre
   if ( strcmp(szItemName, "inv_house_framedining") == 0 && strcmp(szObjectName, "frame_trace") == 0 ) {
      /* Pose le cadre au mur */
      if (!TestGlobal("task_house_dining_putframe"))
      {
         ESoundBank::getSound("success")->playSample();
         SetVisible("frame_wall", true);
         // On peut maintenant retirer l'objet de l'inventaire
         _lpSceneDirector->DropItem("inv_house_framedining");
         // Tâche résolue
         ResolveTask("task_house_dining_putframe");
         StopEmitter("flake_framewall");
         bResult = true;
         _lpSceneDirector->GoToVignette("vignette_diningroom_picture");
      }
   }
   // Le joueur utilise le vase
   // Gêné par le cadre
   if ( strcmp(szItemName, "inv_dining_vase") == 0)
   {
      if (!TestGlobal("task_house_dining_putvase") && isVisible("frame_dresser"))
      {
         _lpSceneDirector->getSequencer()->VoiceOver("frame to remove", KStr("DININGROOM_VOICEOVER_MISSING"), "");
         bResult = true;
      }
      if ( strcmp(szObjectName, "vase_trace") == 0 ) {
         /* Pose le vase sur le meuble */
         if (!TestGlobal("task_house_dining_putvase") && !isVisible("frame_dresser"))
         {
            ESoundBank::getSound("success")->playSample();
            SetVisible("vase_dresser", true);
            SetVisible("vase_trace", false);
            SetVisible("vase_left", false);
            StopEmitter("flake_vasedresser");
            // On peut maintenant retirer l'objet de l'inventaire
            _lpSceneDirector->DropItem("inv_dining_vase");
            // Tâche résolue
            ResolveTask("task_house_dining_putvase");
            ResolveObjective("house","tinyboat");
            bTaskOrder = true;
            bResult = true;
         }
      }
   }
   // Tache de rangement terminée ?
   if (TaskResolved("task_house_dining_putvase") && bTaskOrder)
   {
      int x,y;
      GetObjectPosition("pirate_sit", x, y);
      _lpSceneDirector->getSequencer()->NarrationMode("startdial", SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_WELLDONE"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_SERIOUS"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"pirate_sit_throw",true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"pirate_sit",false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"cigar",true);
      _lpSceneDirector->getSequencer()->Animation(NULL,"cigar_animrot",true);
      _lpSceneDirector->getSequencer()->Animation(NULL,"cigar_animp2p",true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"cigar",false);
      _lpSceneDirector->getSequencer()->Animation(NULL,"cigar_animrot",false);
      _lpSceneDirector->getSequencer()->Animation(NULL,"cigar_animp2p",false);
      _lpSceneDirector->getSequencer()->PlaySound(NULL,"carpet_burn");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"fire_carpet",true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_ASHLEY_CRAZY"),"", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("DININGROOM_MURRAY_CIGAR"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"pirate_sit",true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"pirate_sit_throw",false);
      _lpSceneDirector->getSequencer()->NarrationMode("stopdial", SEQUENCE_NARRATION_DIALOG, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGROOM_ASHLEY_NEEDWATER"), "", false);
      AddObjective("house","treasure");
      AddHint("house","treasure","key");
      SetVisible("firezone", true);
      AddTask("task_house_dining_stopfire");
      bResult = true;
   }
   // Le joueur utilise la casserole
   if ( strcmp(szItemName, "inv_panfull") == 0 && strcmp(szObjectName, "firezone") == 0 ) {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "firestop");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
      StopEmitter("fire_carpet");
      SetVisible("firezone", false);
      SetVisible("carpet_burn", true);
      SetVisible("carpet_key", true);
      // On peut maintenant retirer l'objet de l'inventaire
      _lpSceneDirector->DropItem("inv_panfull");
      // Tâche résolue
      int x,y;
      GetObjectPosition("pirate_sit", x, y);
      _lpSceneDirector->getSequencer()->VoiceOver("frame to remove", KStr("DININGROOM_VOICEOVER_CARPET"), "");
      ResolveTask("task_house_dining_stopfire");
      AddTask("task_house_dining_openrightwindow");
      bResult = true;
   }
   return bResult;
}

void SceneDiningRoom::MiniGameDone(const char *szGameName, bool bIsRevolved)
{

}
