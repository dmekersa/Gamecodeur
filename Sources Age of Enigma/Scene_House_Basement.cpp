/*
 *  Scene_House_Basement.cpp
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Basement.h"
#include "ESceneDirector.h"
#include "SoundBank.h"
#include "GlobalBank.h"
#include "EMiniJeuBonzai.h"
#include "MusicBank.h"
#include "ThreadLoader.h"

/* Constructeur */
SceneBasement::SceneBasement(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpTimerGhost = new KCounter();
   _lpGhostVoice = EMusicBank::getMusic("ghost_longvoice_japan");
   _lpGhostVoice->setVolume(0);
   _lpGhostVoice->playStream(true);

   EMinijeuBonzai::Preload();
}

/* Destructeur */
SceneBasement::~SceneBasement()
{
   XDELETE(_lpTimerGhost);
   _lpGhostVoice->stopStream();
   StopAnimation("basement_ghostup_anim");
}

void SceneBasement::Init()
{
   StartAnimation("basement_ghostup_anim");
   SetVisible("basement_ghostup", false, true);
   SetupItem("envelope_basement");
   
   // 1ère visite
   if (!TaskResolved("task_house_basementvisit")) {
      ResolveTask("task_house_basementvisit");
      AddObjective("house","basementlight");
      AddHint("house","basementlight","how");
      RemoveHint("house","helpnatsumi","where");
      AddHint("house","helpnatsumi","what");
      AddHint("house","helpnatsumi","nextpaper");
   }
   
   // Lumière éteinte
   if (!TaskResolved("task_house_basement_light")) {
      AddTask("task_house_basement_light");
      SetVisible("darkness", true);
      // Ampoule non réparée
      if (TaskResolved("task_house_basement_changebulb")) {
         SetVisible("bulboff", true);
      }
      else {
         // Bulbe cassé retiré ?
         if (TaskResolved("task_house_basement_removebulb")) {
            SetVisible("bulbbroken", false);
            SetVisible("nobulb", true);
         }else {
            SetVisible("bulbbroken", true);
         }
      }
   }
   else {
      SetVisible("darkness", false);
      SetVisible("lightning", true);
      SetVisible("bulboff", false, true);
      SetVisible("bulbon", true, true);
   }
   
   if (TaskResolved("task_japan_universeresolved")) {
      SetVisible("basement_ghosthidden", false, true);
      SetupItem("basement_rope");
      if (isVisible("basement_rope")) {
         StartEmitter("flake_rope");
      }
   }         

   // La clé
   SetVisible("basement_screwdriver", TestGlobal("basement_screwdriver") == false);
   // La pince
   SetVisible("basement_grip", TestGlobal("basement_grip") == false);
   // Le bonzaï
   if (TaskResolved("task_house_basement_bonzai")) {
      SetVisible("basement_bonzairesolved", true, true);
   } else {
      SetVisible("basement_bonzai", true, true);
   }
   
   // La grille
   if (TaskResolved("task_house_basement_opengrid")) {
      SetVisible("basement_gridopen", true);
   } else {
      SetVisible("basement_gridclosed", true);
   }
   
   // Le charbon
   if (!TaskResolved("task_house_basement_reversecoal")) {
      SetVisible("basement_coalbucketup", true, true);
   } else {
      SetVisible("basement_coalbucketdown", true, true);
   }
   
   // Le vase
   if (!TaskResolved("task_house_basement_brokevase")) {
      SetVisible("basement_vase", true, true);
   } else {
      SetVisible("basement_vasebroken", true, true);
      // Le papier
      SetVisible("basement_paper", TestGlobal("basement_paper") == false);
   }
   
   // La boite à outils
   if (!TaskResolved("task_house_basement_opentoolbox")) {
      SetVisible("basement_toolboxclosed", true, true);
   } else {
      SetVisible("basement_toolboxopen", true, true);
      // Le marteau
      SetVisible("basement_hammer", TestGlobal("basement_hammer") == false);
   }
   
   // Le fantôme
   if (!TaskResolved("task_house_basement_light")) {
      int x,y;
      GetObjectPosition("basement_ghostup", x, y);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_up", true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostcreep");
      _lpSceneDirector->getSequencer()->Wait(NULL, 2500);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostappear");
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghostup",true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENT_NATSUMI_LETME"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghostup",false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghosthidden",true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_up", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_hidden", true);
   } else {
      if (!TaskResolved("task_japan_universeresolved")) {
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_hidden", true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghosthidden",true);
      }
   }
   
   // Le croquis
   if (TaskResolved("task_house_basement_givepaper") && TaskResolved("task_house_basement_givecharcoal")) {
      SetVisible("basement_sketch", TestGlobal("basement_sketch") == false);
   }
   
   // On revient de l'univers
   if (getAdditionalName() == "backfrombeyond") {
      int x,y;
      x = 1024/2;
      y = 768/2;
      SetVisible("black", true, true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_up", true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("JAPAN_NATSUMI_HOUSEEPILOG1"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("JAPAN_NATSUMI_HOUSEEPILOG2"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("JAPAN_NATSUMI_HOUSEEPILOG3"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostgone");
      _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/reveil.ogv","frombeyond");
      SetupItem("basement_rope");
      StartEmitter("flake_rope");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "black", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_up", false);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      ResolveTask("task_freeghost_done_japan");
      AddObjective("house","freemurray");
      AddHint("house","freemurray","where");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_basement2.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_BASEMENT2", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return;
   }
   
   // Allumage
   if (getAdditionalName() == "light") {
      // On provoque l'apparition de Natsumi
      ObjectClicked("basement_ghosthidden", 0, 0);
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_basement1.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_BASEMENT1", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
   }
   
   if (getAdditionalName() == "completed") {
      BasementGoforUniverse();
      return;
   }   
   
   // On force le passage à l'univers, en cas de plantage/quit prématuré entre la scène et l'univers
   if (TaskResolved("task_japan_prologcompleted") && !TaskResolved("task_japan_universeresolved")) {
      BasementGoforUniverse();
      return;
   }
   
   // On a tout terminé
   if ( TaskResolved("task_japan_universeresolved") && TestGlobal("basement_rope")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED"), "", true);
      return;
   }
}

void SceneBasement::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
   }
   if (KInput::isPressed(K_VK_F6))
   {
      ResolveObjective("house","helpnatsumi");
      _lpSceneDirector->GoToScene("basement","backfrombeyond");
   }
#endif
}

void SceneBasement::Logic()
{
	EScene::Logic();
}

void SceneBasement::Draw()
{
   EScene::Draw();

   if (!TaskResolved("task_japan_universeresolved")) {
      // Timer du fantome (position debout)
      if (isVisible("basement_ghostup") && !_lpTimerGhost->isCompleted()) {
         double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
         _lpTimerGhost->move(fElapsed);
         if (_lpTimerGhost->isCompleted()) {
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghostup",false);
            _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_up", false);
            _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_hidden", true);
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghosthidden",true);
         }
      }
   }
}

void SceneBasement::Close()
{
   
}

bool SceneBasement::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Interrupteur
   if ( strcmp(szObjectName, "switch") == 0)
   {
      if (!TaskResolved("task_house_basement_changebulb")) {
         _lpSceneDirector->getSequencer()->VoiceOver(szObjectName, KStr("HOUSE_BASEMENT_ASHLEY_BULBBROK"),"");
      } else {
         if (TaskResolved("task_house_basement_light")) {
            _lpSceneDirector->getSequencer()->VoiceOver(szObjectName, KStr("HOUSE_BASEMENT_ASHLEY_LIGHT"),"");
         } else {
            ESoundBank::getSound("reveal")->playSample();
            ResolveObjective("house","basementlight");
            ResolveTask("task_house_basement_light");
            AddTask("task_house_basement_natsumineed");
            setAdditionalName("light");
            Init();
         }
      }

      return true;
   }
   if ( strcmp(szObjectName, "bulbbroken") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_CUT"), "", true);         
      return true;
   }
   if ( strcmp(szObjectName, "nobulb") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_MISSBULB"), "", true);         
      return true;
   }
   if ( strcmp(szObjectName, "basement_rope") == 0)
   {
      PickupMultiple(szObjectName, "inv_house_rope",-1);
      StopEmitter("flake_rope");
      return true;
   }
   if ( strcmp(szObjectName, "basement_screwdriver") == 0)
   {
      PickupSimple(szObjectName, "inv_house_basement_screwdriver");
      return true;
   }
   if ( strcmp(szObjectName, "basement_grip") == 0)
   {
      PickupMultiple(szObjectName, "inv_house_basement_gripbroken",-1);
//      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_WRISTBROK"), "", true);         
      return true;
   }
   if ( strcmp(szObjectName, "basement_hammer") == 0)
   {
      PickupSimple(szObjectName, "inv_house_basement_hammer");
      return true;
   }
   if ( strcmp(szObjectName, "basement_paper") == 0)
   {
      PickupSimple(szObjectName, "inv_house_basement_paper");
      return true;
   }
   if ( strcmp(szObjectName, "basement_sketch") == 0)
   {
      PickupSimple(szObjectName, "inv_house_basement_sketch");
      _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_DRAWING"), "", true);         
      return true;
   }
   if ( strcmp(szObjectName, "basement_bonzai") == 0)
   {
      if (TaskResolved("task_house_basement_givepaper") && TaskResolved("task_house_basement_givecharcoal")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_INEED2"), "", true);         
         return true;
      }
   }
   
   if ( strcmp(szObjectName, "envelope_basement") == 0)
   {
      PickupSimple(szObjectName, "ihm_envelope");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage("letterpaper.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("ENVELOPE_BASEMENT", FONT_DIARY_2, 0, 0);
      _lpSceneDirector->getDiaryPtr()->addImageToPage("logofraternity.png");
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return true;
   }
   
   // Seau de charbon
   if ( strcmp(szObjectName, "basement_coalbucketup") == 0)
   {
      ResolveTask("task_house_basement_reversecoal");
      ESoundBank::getSound("success")->playSample();
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_coalbucketup",false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_coalbucketdown",true);
      _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_OUPSFALL"), "", true);         
   }

   // boite à outils
   if ( strcmp(szObjectName, "basement_toolboxclosed") == 0)
   {
      ResolveTask("task_house_basement_opentoolbox");
      ESoundBank::getSound("success")->playSample();
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_toolboxopen",true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_toolboxclosed",false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_hammer",true);
   }
   
   // Grille fermée
   if ( strcmp(szObjectName, "basement_gridclosed") == 0 || strcmp(szObjectName, "basement_gridopen") == 0)
   {
      _lpSceneDirector->GoToScene("basement_grid","",false);
      return true;
   }   
   
   // Fantome
   if ( strcmp(szObjectName, "basement_ghosthidden") == 0)
   {
      // 1ère communication
      if (!TaskResolved("task_house_meetnatsumi")) {
         ResolveTask("task_house_meetnatsumi");
      }
      
      int x,y;
      GetObjectPosition("basement_ghostup", x, y);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostappear");
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghosthidden",false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_hidden", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_up", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghostup",true);
      // On a donné les 2 objets attendus
      if (TaskResolved("task_house_basement_givepaper") && TaskResolved("task_house_basement_givecharcoal")) {
         _lpSceneDirector->getSequencer()->Talk("canu", x, y, KStr("HOUSE_BASEMENT_NATSUMI_CANYOUBONZ"), "", false, false, _lpGhostVoice);
         _lpTimerGhost->startCounter(0.0f, 1.0f, 0, 10000, K_COUNTER_LINEAR);
      }
      else {
         if (!TaskToResolve("task_house_basement_givepaper")) {
            AddTask("task_house_basement_givepaper");
         }
         if (!TaskToResolve("task_house_basement_givecharcoal")) {
            AddTask("task_house_basement_givecharcoal");
         }
         _lpSceneDirector->getSequencer()->Talk("doyouhave", x, y, KStr("HOUSE_BASEMENT_NATSUMI_DOYOUHAVE"), "", true, false, _lpGhostVoice);
         _lpTimerGhost->startCounter(0.0f, 1.0f, 0, 10000, K_COUNTER_LINEAR);
      }
      return true;
   }   
   if ( strcmp(szObjectName, "basement_ghostup") == 0)
   {
      int x,y;
      GetObjectPosition("basement_ghostup", x, y);
      _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("HOUSE_BASEMENT_NATSUMI_DOYOUHAVE2"), "", true, false, _lpGhostVoice);
      return true;
   }
   
#ifdef BFG_TEASING
   if ( strcmp(szObjectName, "gotostairs") == 0 && TaskResolved("task_japan_universeresolved"))
   {
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "teasing", "", false);
      return true;
   }
#endif
   
   return false;
}

bool SceneBasement::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool SceneBasement::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur donne quelque chose au fantôme debout
   if (strcmp(szObjectName, "basement_ghostup") == 0)
   {
      int x,y;
      bool bGiven = false;
      GetObjectPosition("basement_ghostup", x, y);
      if (strcmp(szItemName, "inv_house_basement_paper") == 0) {
         ResolveTask("task_house_basement_givepaper");
         RemoveHint("house","helpnatsumi","nextpaper");
         // On peut maintenant retirer l'objet de l'inventaire
         _lpSceneDirector->DropItem("inv_house_basement_paper");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENT_NATSUMI_THANKYOU"), "", true, false, _lpGhostVoice);
         if (!TaskResolved("task_house_basement_givecharcoal")) {
            AddHint("house","helpnatsumi","nextcharcoal");
         }
         bGiven = true;
      }
      else
      if (strcmp(szItemName, "inv_house_basement_grid_charcoal") == 0) {
         ResolveTask("task_house_basement_givecharcoal");
         RemoveHint("house","helpnatsumi","nextcharcoal");
         // On peut maintenant retirer l'objet de l'inventaire
         _lpSceneDirector->DropItem("inv_house_basement_grid_charcoal");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENT_NATSUMI_THANKYOU"), "", true, false, _lpGhostVoice);
         bGiven = true;
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENT_NATSUMI_IDONTNEED"), "", true, false, _lpGhostVoice);
      }
      
      // On teste si on a donné les 2 objets attendus par le fantôme
      if (bGiven) {
         if (TaskResolved("task_house_basement_givepaper") && TaskResolved("task_house_basement_givecharcoal")) {
            _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
            _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_up", false);
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghostup",false);
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghostdrawing1",true);
            _lpSceneDirector->getSequencer()->SwitchImage(NULL, "basement_ghostdrawing1", "basement_ghostdrawing2");
            _lpSceneDirector->getSequencer()->SwitchImage(NULL, "basement_ghostdrawing2", "basement_ghostdrawing1");
            _lpSceneDirector->getSequencer()->SwitchImage(NULL, "basement_ghostdrawing1", "basement_ghostdrawing2");
            _lpSceneDirector->getSequencer()->SwitchImage(NULL, "basement_ghostdrawing2", "basement_ghostdrawing1");
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_SHEDRAW"), "", false, true);
            _lpSceneDirector->getSequencer()->SwitchImage(NULL, "basement_ghostdrawing1", "basement_ghostdrawing2");
            _lpSceneDirector->getSequencer()->SwitchImage(NULL, "basement_ghostdrawing2", "basement_ghostdrawing1");
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghostdrawing1",false);
            _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_hidden", true);
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghosthidden",true);
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_sketch",true);
            _lpSceneDirector->getSequencer()->Animation(NULL, "sketchanimp2p", true);
            _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_SHEDRAW2"), "", true);
            ResolveTask("task_house_basement_natsumineed");
            AddTask("task_house_basement_bonzai");
            RemoveHint("house","helpnatsumi","what");
            AddObjective("house","bonzai");
            AddHint("house","bonzai","how");
            AddHint("house","bonzai","grip");
         }
      }
      return true;
   }
   // Le joueur donne quelque chose au fantôme caché
   if (strcmp(szObjectName, "basement_ghosthidden") == 0)
   {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "ghostappear");
      int x,y;
      GetObjectPosition("basement_ghostup", x, y);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghosthidden",false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_hidden", false);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_up", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghostup",true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENT_NATSUMI_DOYOUWANT2GIVE"), "", false, false, _lpGhostVoice);
      _lpTimerGhost->startCounter(0.0f, 1.0f, 0, 10000, K_COUNTER_LINEAR);
      return true;
   }
   // Le joueur utilise l'ampoule sur le bulbe cassé
   if ( strcmp(szItemName, "inv_house_basement_bulb") == 0 ) {
      if (strcmp(szObjectName, "bulbbroken") == 0)
      {
         _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("HOUSE_BASEMENT_ASHLEY_IMPOBULB"),"");
         return true;
      }
   }
   // Le joueur utilise le torchon sur le bulbe cassé
   if ( strcmp(szItemName, "inv_house_basement_rag") == 0 ) {
      if (strcmp(szObjectName, "bulbbroken") == 0)
      {
         ESoundBank::getSound("success")->playSample();
         // On peut maintenant retirer l'objet de l'inventaire
         _lpSceneDirector->DropItem("inv_house_basement_rag");
         // On enlève la douille cassée
         SetVisible("bulbbroken", false);
         SetVisible("nobulb", true);
         // Tache résolue : enlever le bulbe brisé
         ResolveTask("task_house_basement_removebulb");
         return true;
      }
   }
   // Le joueur utilise l'ampoule neuve sur la douille vide
   if ( strcmp(szItemName, "inv_house_basement_bulb") == 0 ) {
      if (strcmp(szObjectName, "nobulb") == 0)
      {
         ESoundBank::getSound("success")->playSample();
         // On peut maintenant retirer l'objet de l'inventaire
         _lpSceneDirector->DropItem("inv_house_basement_bulb");
         // On met l'ampoule
         SetVisible("bulboff", true);
         SetVisible("nobulb", false);
         // Tache résolue : enlever le bulbe brisé
         ResolveTask("task_house_basement_changebulb");
         return true;
      }
   }
   // Le joueur utilise le marteau sur le vase
   if ( strcmp(szItemName, "inv_house_basement_hammer") == 0 ) {
      if (strcmp(szObjectName, "basement_vase") == 0)
      {
         ESoundBank::getSound("success")->playSample();
         // On peut maintenant retirer l'objet de l'inventaire
         _lpSceneDirector->DropItem("inv_house_basement_hammer");
         SetVisible("basement_vase", false);
         SetVisible("basement_vasebroken", true);
         SetVisible("basement_paper", true);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_VASEBROKEN_INFO"), "", true);
         // Tache résolue
         ResolveTask("task_house_basement_brokevase");
         return true;
      }
   }
   
   // Le joueur utilise la pince cassée sur le bonzai
   if ( strcmp(szItemName, "inv_house_basement_gripbroken") == 0 ) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_CANNOTWRIST"), "", true);
   }
   // Le joueur utilise la pince sur le bonzai
   if ( strcmp(szItemName, "inv_house_basement_grip") == 0 ) {
      // On doit avoir le modèle pour faire le mini jeu !
      if (_lpSceneDirector->getInventory()->getItemFromInventory("inv_house_basement_sketch")) {
         if (strcmp(szObjectName, "basement_bonzai") == 0)
         {
            EMiniJeuBase *lpMiniJeu = new EMinijeuBonzai();
            _lpSceneDirector->GoToMiniGame("bonzai", lpMiniJeu);
            return true;
         }
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_INEEDMODEL"), "", true);
         return true;
      }
   }
   return false;
}

void SceneBasement::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "bonzai")==0 && bIsRevolved) {
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "basement_bonzai", "basement_bonzairesolved");
      ResolveTask("task_house_basement_bonzai");
      ResolveTask("task_japan_prologcompleted");
      ResolveObjective("house","bonzai");
      ResolveObjective("house","helpnatsumi");
      ESoundBank::getSound("success")->playSample();
      // On peut maintenant retirer l'objet de l'inventaire
      _lpSceneDirector->DropItem("inv_house_basement_grip");
      _lpSceneDirector->DropItem("inv_house_basement_sketch");
      // Lance l'épilogue de la cave
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      setAdditionalName("completed");
      Init();
   }
}

void SceneBasement::BasementGoforUniverse()
{
   int x,y;
   Loader::LoadScene("vignette_hands_japan");
   Loader::LoadScene("japan_mainscreen");
   GetObjectPosition("basement_ghostup", x, y);
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
   _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghosthidden",false);
   _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_hidden", false);
   _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_static_up", true);
   _lpSceneDirector->getSequencer()->ShowImage(NULL,"basement_ghostup",true);
   _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENT_NATSUMI_WOMANYOUCAN"), "", false, false, _lpGhostVoice);
   _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENT_NATSUMI_CANYOUIWILL"), "", false, false, _lpGhostVoice);
   _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_REALLYSO"), "", false, true);
   _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENT_NATSUMI_ITISATSU"), "", false, false, _lpGhostVoice);
   _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENT_NATSUMI_HELPME"), "", false, false, _lpGhostVoice);
   _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENT_ASHLEY_GIVEME"),"", true, true);
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
   _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_hands_japan", "japan", false);      
}
