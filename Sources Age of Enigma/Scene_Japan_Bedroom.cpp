/*
 *  Scene_Japan_Bedroom.cpp
 *  enigma
 *
 *  Created by Rockford on 31/08/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Japan_Bedroom.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "SoundBank.h"
#include "MyGame.h"
#include "EMiniJeuStones.h"

/* Constructeur */
Scene_Japan_Bedroom::Scene_Japan_Bedroom(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpCounterVision = new KCounter();
   _bVision = false;
   
   EMiniJeuStones::Preload();
}

/* Destructeur */
Scene_Japan_Bedroom::~Scene_Japan_Bedroom()
{
   XDELETE(_lpCounterVision);
}

void Scene_Japan_Bedroom::Init()
{
   if (!TaskResolved("task_japan_bedroomvisit")) {
      ResolveTask("task_japan_bedroomvisit");
      AddTask("task_japan_tidy");
      ResolveObjective("japan","findbedroom");
      AddObjective("japan","tiny");
      AddHint("japan","tiny","how");
      AddHint("japan","tiny","encens");
      AddHint("japan","tiny","next");
   }
   
   // Torche
   // En bordel
   SetVisible("japan_torch_messy", TestGlobal("japan_torch_messy") == false, true);
   // Rangée
   SetVisible("japan_torchoff_tidy", TaskResolved("task_japan_torch"), true);
   _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "fire_torch", TaskResolved("task_japan_torch"));
   // Zone de dépot
   SetVisible("torchzone", TaskResolved("task_japan_torch") == false, true);
   
   // Bento
   // En bordel
   SetVisible("japan_bento_messy", TestGlobal("japan_bento_messy") == false, true);
   // Rangé
   SetVisible("japan_bento_tidy", TestGlobal("task_japan_bento"), true);

   // Meal
   // En bordel
   SetVisible("japan_meal_messy", TestGlobal("japan_meal_messy") == false, true);
   
   // Bowl 1
   // En bordel
   SetVisible("japan_bowl1_messy", TestGlobal("japan_bowl1_messy") == false, true);
   // Rangé
   SetVisible("japan_bowl1_tidy", TaskResolved("task_japan_bowl1"), true);
   // Bowl 2
   // En bordel
   SetVisible("japan_bowl2_messy", TestGlobal("japan_bowl2_messy") == false, true);
   // Rangé
   SetVisible("japan_bowl2_tidy", TaskResolved("task_japan_bowl2"), true);
   // underTeapot 2
   // En bordel
   SetVisible("japan_underteapot_messy", TestGlobal("japan_underteapot_messy") == false, true);
   // Rangé
   SetVisible("japan_underteapot_tidy", TaskResolved("task_japan_underteapot"), true);
   // Teapot 2
   // En bordel
   SetVisible("japan_teapot_messy", TestGlobal("japan_teapot_messy") == false, true);
   // Rangé
   SetVisible("japan_teapot_tidy", TaskResolved("task_japan_teapot"), true);

   // Pétales rangés
   SetVisible("japan_leaves_tidy", TaskResolved("task_japan_leaves"), true);
   
   // Kimono 1
   // En bordel
   SetVisible("japan_kimono1_messy", TestGlobal("japan_kimono1_messy") == false, true);
   // Rangé
   SetVisible("japan_kimono1_tidy", TaskResolved("task_japan_kimono1"), true);
   // Kimono 2
   // En bordel
   SetVisible("japan_kimono2_messy", TestGlobal("japan_kimono2_messy") == false, true);
   // Rangé
   SetVisible("japan_kimono2_tidy", TaskResolved("task_japan_kimono2"), true);
   // Kimono 3
   // En bordel
   SetVisible("japan_kimono3_messy", TestGlobal("japan_kimono3_messy") == false, true);
   // Rangé
   SetVisible("japan_kimono3_tidy", TaskResolved("task_japan_kimono3"), true);
   
   // Encens
   // stick En bordel
   SetVisible("japan_encensstick_messy", TestGlobal("japan_encensstick_messy") == false, true);
   // holder En bordel
   SetVisible("japan_encensholder_messy", TestGlobal("japan_encensholder_messy") == false, true);
   // Rangé
   SetVisible("japan_encensfull_tidy", TestGlobal("task_japan_encens"), true);
   if (TaskResolved("task_japan_encenslight")) {
      SetVisible("japan_encenslight", true);
      SetSmoothOverlap("japan_encenslight", true);
      StartAnimation("encensanim");
   }
   
   // Table
   // En bordel
   SetVisible("japan_table_messy[1]", TestGlobal("japan_table_messy[1]") == false, true);
   SetVisible("japan_table_messy[2]", TestGlobal("japan_table_messy[2]") == false, true);
   SetVisible("japan_table_messy[3]", TestGlobal("japan_table_messy[3]") == false, true);
   SetVisible("japan_table_messy[4]", TestGlobal("japan_table_messy[4]") == false, true);
   // Rangée
   SetVisible("japan_table_tidy", TaskResolved("task_japan_table"), true);
   // Zone de dépot
   SetVisible("tablezone", TaskResolved("task_japan_table") == false, true);
   
   // Galets
   if (TaskResolved("task_japan_stones")) {
      SetVisible("japan_stones_tidy", true);
      SetVisible("japan_stones_messy", false);
   } else {
      SetVisible("japan_stones_messy", true);
   }

   // Voile
   if (TaskResolved("task_japan_veil")) {
      SetVisible("japan_veil_tidy", true);
      SetSmoothOverlap("japan_veil_tidy", true);
      StartAnimation("veilanim");
   } else {
      SetVisible("japan_veil_messy", true);
   }
   
   // Lit
   SetVisible("japan_bed_tidy", TaskResolved("task_japan_bed"), true);
   SetVisible("japan_bed_messy", TaskResolved("task_japan_bed") == false, true);
   // Paravent
   SetVisible("japan_screen_tidy", TaskResolved("task_japan_screen"), true);
   SetVisible("japan_screen_messy", TaskResolved("task_japan_screen") == false, true);
   // Bonzai
   SetVisible("japan_bonzai_tidy", TaskResolved("task_japan_bonzai"), true);
   SetVisible("japan_bonzai_messy", TaskResolved("task_japan_bonzai") == false, true);
   // Lanterne intérieure
   SetVisible("japan_candlelamp_tidy", TaskResolved("task_japan_candlelamp"), true);
   SetVisible("japan_candlelamplight", TaskResolved("task_japan_candlelamplight"), true);
   
   // Zone du lampion
   if (TaskResolved("task_japan_inspectcandlezone")) {
      SetVisible("candlelampzone", true);
   }
}

void Scene_Japan_Bedroom::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      if (isVisible("japan_encensholder_messy")) {
         PickupMultiple("japan_table_messy[1]", "inv_japan_tablepieces", 4);
         PickupMultiple("japan_encensholder_messy", "inv_japan_encensholder",-1);
      }
   }
   if (KInput::isPressed(K_VK_F6))
   {
      ResolveTask("task_japan_tidy");
      ResolveTask("task_japan_universeresolved");
      
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_tidyvision", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 3000);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"japan_mainscreen", "completed", false);
   }
#endif

   // Teste si tout est fini
   if (TaskResolved("task_japan_screen") &&
       TaskResolved("task_japan_bed") &&
       TaskResolved("task_japan_stones") &&
       TaskResolved("task_japan_torch") &&
       TaskResolved("task_japan_table") &&
       TaskResolved("task_japan_teapot") &&
       TaskResolved("task_japan_underteapot") &&
       TaskResolved("task_japan_bonzai") &&
       TaskResolved("task_japan_candlelamp") &&
       TaskResolved("task_japan_candlelamplight") &&
       TaskResolved("task_japan_encens") &&
       TaskResolved("task_japan_encenslight") &&
       TaskResolved("task_japan_bento") &&
       TaskResolved("task_japan_veil") &&
       TaskResolved("task_japan_leaves")
       && !TaskResolved("task_japan_tidy"))
   {
      ResolveTask("task_japan_tidy");
      ResolveObjective("japan","tiny");

      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_tidyvision", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 3000);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
      _lpSceneDirector->getSequencer()->Wait(NULL, 100);
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"japan_mainscreen", "completed", false);
      
   }
}

void Scene_Japan_Bedroom::Logic()
{
	EScene::Logic();
}

void Scene_Japan_Bedroom::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   EScene::Draw();
   if (_bVision) {
      _lpCounterVision->move(fElapsed);
      if (_lpCounterVision->isCompleted()) {
         _bVision = false;
         SetVisible("japan_tidyvision", false);
      }
   }
}

void Scene_Japan_Bedroom::Close()
{
   
}

bool Scene_Japan_Bedroom::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Paravent messy
   if ( strcmp(szObjectName, "japan_screen_messy") == 0)
   {
      if (TestGlobal("japan_kimono3_messy")) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         SetVisible("japan_screen_messy", false);
         SetVisible("japan_screen_tidy", true);
         ResolveTask("task_japan_screen");
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_ORDERFIRST"),"", true, false);
      }
      return true;
   }
   // Lit messy
   if ( strcmp(szObjectName, "japan_bed_messy") == 0)
   {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      SetVisible("japan_bed_messy", false);
      SetVisible("japan_bed_tidy", true);
      ResolveTask("task_japan_bed");
      return true;
   }
   // Bonzai messy
   if ( strcmp(szObjectName, "japan_bonzai_messy") == 0)
   {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
      SetVisible("japan_bonzai_messy", false);
      SetVisible("japan_bonzai_tidy", true);
      ResolveTask("task_japan_bonzai");
      return true;
   }
   // Voile messy
   if ( strcmp(szObjectName, "japan_veil_messy") == 0)
   {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
      SetVisible("japan_veil_messy", false);
      SetVisible("japan_veil_tidy", true);
      SetSmoothOverlap("japan_veil_tidy", true);
      StartAnimation("veilanim");
      ResolveTask("task_japan_veil");
      return true;
   }
   
   // Lit = vision
   if ( strcmp(szObjectName, "japan_bed_tidy") == 0)
   {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      _lpCounterVision->startCounter(0.0f, 1.0f, 0, 5000, K_COUNTER_LINEAR);
      _bVision = true;
      SetVisible("japan_tidyvision", true);
      return true;
   }
   
   // Torche
   if ( strcmp(szObjectName, "japan_torch_messy") == 0)
   {
      PickupSimple(szObjectName, "inv_japan_torchoff");
      AddObjective("japan","torch");
      AddHint("japan","torch","how");
      return true;
   }
   // Bento
   if ( strcmp(szObjectName, "japan_bento_messy") == 0)
   {
      PickupMultiple(szObjectName, "inv_japan_bentoempty",-1);
      return true;
   }
   // Meal
   if ( strcmp(szObjectName, "japan_meal_messy") == 0)
   {
      PickupMultiple(szObjectName, "inv_japan_meal",-1);
      return true;
   }
   // Bowl 1
   if ( strcmp(szObjectName, "japan_bowl1_messy") == 0)
   {
      PickupSimple(szObjectName, "inv_japan_bowl1");
      return true;
   }
   // Bowl 2
   if ( strcmp(szObjectName, "japan_bowl2_messy") == 0)
   {
      PickupSimple(szObjectName, "inv_japan_bowl2");
      return true;
   }
   // underTeapot
   if ( strcmp(szObjectName, "japan_underteapot_messy") == 0)
   {
      PickupSimple(szObjectName, "inv_japan_underteapot");
      return true;
   }
   // Teapot
   if ( strcmp(szObjectName, "japan_teapot_messy") == 0)
   {
      PickupSimple(szObjectName, "inv_japan_teapot");
      SetVisible("candlelampzone", true);
      if (!TaskResolved("task_japan_inspectcandlezone")) {
         ResolveTask("task_japan_inspectcandlezone");
         AddObjective("japan","candlelamp");
         AddHint("japan","candlelamp","where");
         AddHint("japan","candlelamp","lamps");      
      }
      return true;
   }
   // Kimono 1
   if ( strcmp(szObjectName, "japan_kimono1_messy") == 0)
   {
      PickupSimple(szObjectName, "inv_japan_kimono1");
      return true;
   }
   // Kimono 2
   if ( strcmp(szObjectName, "japan_kimono2_messy") == 0)
   {
      PickupSimple(szObjectName, "inv_japan_kimono2");
      return true;
   }
   // Kimono 3
   if ( strcmp(szObjectName, "japan_kimono3_messy") == 0)
   {
      PickupSimple(szObjectName, "inv_japan_kimono3");
      return true;
   }
   // Encens / stick
   if ( strcmp(szObjectName, "japan_encensstick_messy") == 0)
   {
      PickupMultiple(szObjectName, "inv_japan_encensstick",-1);
      return true;
   }
   // Encens / holder
   if ( strcmp(szObjectName, "japan_encensholder_messy") == 0)
   {
      PickupMultiple(szObjectName, "inv_japan_encensholder",-1);
      return true;
   }
   // Zone du lampion
   if ( strcmp(szObjectName, "candlelampzone") == 0) {
   }
   
   /* Morceaux de la table */
   if ( strncmp(szObjectName, "japan_table_messy", strlen("japan_table_messy")) == 0 )
   {
      PickupMultiple(szObjectName, "inv_japan_tablepieces", 4);
      return true;
   }

   // Galets
   if (strcmp(szObjectName,"japan_stones_messy") == 0) {
      if (!TaskResolved("task_japan_stones")) {
         // Mini jeu du jardin
         EMiniJeuBase *lpMiniJeu = new EMiniJeuStones();
         _lpSceneDirector->GoToMiniGame("stones", lpMiniJeu);
         return true;
      }
   }
   
   return false;
}

bool Scene_Japan_Bedroom::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Japan_Bedroom::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Torche allumée à poser
   if (strcmp(szItemName, "inv_japan_torchon") == 0) {
      if (strcmp(szObjectName, "torchzone") == 0) {
         if (TaskResolved("task_japan_candlelamplight") && TaskResolved("task_japan_encenslight")) {
            ResolveTask("task_japan_torch");
            SetVisible("torchzone", false);
            _lpSceneDirector->DropItem("inv_japan_torchon");
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"japan_torchoff_tidy", true);
            _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "fire_torch", true);
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_STILLNEED"),"", true, false);
         }
         return true;
      }
   }
   // Torche allumée sur lanterne
   if (strcmp(szItemName, "inv_japan_torchon") == 0) {
      if (strcmp(szObjectName, "japan_candlelamp_tidy") == 0) {
         if (!TaskResolved("task_japan_candlelamplight")) {
            ResolveTask("task_japan_candlelamplight");
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"japan_candlelamplight", true);
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_DEJAFAIT"),"", true, false);
         }
         return true;
      }
   }
   // Torche allumée sur encens
   if (strcmp(szItemName, "inv_japan_torchon") == 0) {
      if (strcmp(szObjectName, "japan_encensfull_tidy") == 0 || strcmp(szObjectName, "japan_bento_tidy") == 0 || strcmp(szObjectName, "japan_table_tidy") == 0) {
         if (!TaskResolved("task_japan_encenslight")) {
            RemoveHint("japan","tiny","encens");
            ResolveTask("task_japan_encenslight");
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"japan_encenslight", true);
            SetSmoothOverlap("japan_encenslight", true);
            StartAnimation("encensanim");
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_DEJAFAIT"),"", true, false);
         }
         return true;
      }
   }
   
   // Table
   if (strcmp(szItemName, "inv_japan_table") == 0) {
      if (strcmp(szObjectName, "tablezone") == 0) {
         if (TestGlobal("japan_bowl1_messy")) {
            ResolveTask("task_japan_table");
            SetVisible("tablezone", false);
            _lpSceneDirector->DropItem("inv_japan_table");
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"japan_table_tidy", true);
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_ORDERFIRST"),"", true, false);
         }
         return true;
      }
   }
   // Bowl 1
   if (strcmp(szItemName, "inv_japan_bowl1") == 0) {
      if (strcmp(szObjectName, "japan_table_tidy") == 0 || strcmp(szObjectName, "japan_underteapot_tidy") == 0) {
         ResolveTask("task_japan_bowl1");
         _lpSceneDirector->DropItem("inv_japan_bowl1");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_bowl1_tidy", true);
         return true;
      }
   }
   // Bowl 2
   if (strcmp(szItemName, "inv_japan_bowl2") == 0) {
      if (strcmp(szObjectName, "japan_table_tidy") == 0 || strcmp(szObjectName, "japan_underteapot_tidy") == 0) {
         ResolveTask("task_japan_bowl2");
         _lpSceneDirector->DropItem("inv_japan_bowl2");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_bowl2_tidy", true);
         return true;
      }
   }
   // Bento
   if (strcmp(szItemName, "inv_japan_bentofull") == 0) {
      if (strcmp(szObjectName, "japan_table_tidy") == 0 || strcmp(szObjectName, "japan_underteapot_tidy") == 0) {
         ResolveTask("task_japan_bento");
         _lpSceneDirector->DropItem("inv_japan_bentofull");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_bento_tidy", true);
         return true;
      }
   }
   // Underteapot
   if (strcmp(szItemName, "inv_japan_underteapot") == 0) {
      if (strcmp(szObjectName, "japan_table_tidy") == 0) {
         ResolveTask("task_japan_underteapot");
         _lpSceneDirector->DropItem("inv_japan_underteapot");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_underteapot_tidy", true);
         return true;
      }
   }
   // Teapot
   if (strcmp(szItemName, "inv_japan_teapot") == 0) {
      if (TaskResolved("task_japan_underteapot")) {
         if (strcmp(szObjectName, "japan_underteapot_tidy") == 0 || strcmp(szObjectName, "japan_bento_tidy") == 0) {
            ResolveTask("task_japan_teapot");
            _lpSceneDirector->DropItem("inv_japan_teapot");
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_teapot_tidy", true);
            return true;
         }
      }
   }
   // Lanterne intérieure
   if (strcmp(szItemName, "inv_japan_candlelamp") == 0) {
      if (strcmp(szObjectName, "candlelampzone") == 0) {
         ResolveObjective("japan","candlelamp");
         ResolveTask("task_japan_candlelamp");
         _lpSceneDirector->DropItem("inv_japan_candlelamp");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_candlelamp_tidy", true);
         return true;
      }
   }
   // Encens
   if (strcmp(szItemName, "inv_japan_encensfull") == 0) {
      if (strcmp(szObjectName, "encenszone") == 0 || strcmp(szObjectName, "japan_table_tidy") == 0) {
         if (TestGlobal("japan_underteapot_messy") && TestGlobal("japan_table_messy[3]")) {
            ResolveTask("task_japan_encens");
            SetVisible("encenszone", false);
            _lpSceneDirector->DropItem("inv_japan_encensfull");
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"japan_encensfull_tidy", true);
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_NOROOM"),"", true, false);
         }
         return true;
      }
   }
   // Petales
   if (strcmp(szItemName, "inv_japan_leaves") == 0) {
      if (strcmp(szObjectName, "japan_bed_tidy") == 0) {
         ResolveTask("task_japan_leaves");
         _lpSceneDirector->DropItem("inv_japan_leaves");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_leaves_tidy", true);
         return true;
      }
   }
   
   // Torche éteinte
   if (strcmp(szItemName, "inv_japan_torchoff") == 0) {
      if (strcmp(szObjectName, "torchzone") == 0) {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_LIGHTFIRST"),"", true, false);
         return true;
      }
   }
   // Kimono part 1
   if (strcmp(szItemName, "inv_japan_kimono1") == 0) {
      if (strcmp(szObjectName, "japan_screen_tidy") == 0) {
         ResolveTask("task_japan_kimono1");
         _lpSceneDirector->DropItem("inv_japan_kimono1");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_kimono1_tidy", true);
         return true;
      }
   }
   // Kimono part 2
   if (strcmp(szItemName, "inv_japan_kimono2") == 0) {
      if (strcmp(szObjectName, "japan_kimono1_tidy") == 0) {
         ResolveTask("task_japan_kimono2");
         _lpSceneDirector->DropItem("inv_japan_kimono2");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_kimono2_tidy", true);
         return true;
      }
   }
   // Kimono part 3
   if (strcmp(szItemName, "inv_japan_kimono3") == 0) {
      if (strcmp(szObjectName, "japan_kimono2_tidy") == 0) {
         ResolveTask("task_japan_kimono3");
         _lpSceneDirector->DropItem("inv_japan_kimono3");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan_kimono3_tidy", true);
         return true;
      }
   }
   return false;
}

void Scene_Japan_Bedroom::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   // Les pierres ont été remises en ordre sur la table de nuit
   if (strcmp(szGameName, "stones")==0 && bIsRevolved) {
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      ESoundBank::getSound("success")->playSample();
      SetVisible("japan_stones_messy", false);
      ResolveTask("task_japan_stones");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("JAPAN_NATSUMI_BEDROOM_MGDONE"),"", true, false);
      Init();
   }   
}