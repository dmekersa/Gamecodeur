/*
 *  Scene_Island_Beach.cpp
 *  enigma
 *
 *  Created by Rockford on 30/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Island_Beach.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuSearchgrid.h"
#include "EMiniJeuSimonShark.h"
#include "MusicBank.h"
#include "ThreadLoader.h"
#include "MyGame.h"

/* Constructeur */
Scene_Island_Beach::Scene_Island_Beach(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _lpCounterWave = new KCounter();
   _lpCounterCrab = new KCounter();

   EMiniJeuSearchgrid::Preload();
   EMiniJeuSimonShark::Preload();
}

/* Destructeur */
Scene_Island_Beach::~Scene_Island_Beach()
{
   SetSmoothOverlap("bg_clouds", true);
   SetSmoothOverlap("bg_clouds", true);
   StopAnimation("bg_clouds_anim");
   delete _lpBgGraphic;
   XDELETE(_lpCounterWave);
   XDELETE(_lpCounterCrab);
}

void Scene_Island_Beach::Init()
{
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_ISLAND);
   
   _lpCounterCrab->startCounter(0, 1, 0, GetRandom(1000, 2000), K_COUNTER_LINEAR);
   SetupItem("island_berry");
   
   StartAnimation("cloudsanim");
   StartAnimation("bganim");
   
   // Inventaire de début d'univers
   if (_lpSceneDirector->getInventory()->getItemFromInventory("inv_island_neckless1") == NULL && !TaskResolved("task_island_opensafe")) {
      EInventoryItem *item = new EInventoryItem("island","inv_island_neckless1",2,0);
      _lpSceneDirector->getInventory()->AddItem(item);
      AddTask("task_island_findsafe");
      AddObjective("island","findchest");
      AddHint("island","findchest","map");
      AddHint("island","findchest","howdig");

      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIALOGBOX_INTRO1"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIALOGBOX_INTRO2"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIALOGBOX_INTRO3"), "", false, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);      
   }
   
   _lpCounterWave->startCounter(0, 1, 0, 3000, K_COUNTER_LINEAR);
   StartAnimation("bg_clouds_anim");
   StartAnimation("plantanimrot");
   
   // Le perroquet a été chassé de la crique, il est donc ici
   if (TaskResolved("task_island_expelparrot")) {
      // Il est sur le cocotier, avec la perle
      if (!TaskResolved("task_island_fallpearl")) {
         SetVisible("parrot_pearl", true);
         SetVisible("parrot_nopearl", false);
      }
      else {
         SetVisible("parrot_pearl", false);
         SetVisible("parrot_nopearl", true);
         if (!TaskResolved("task_island_opensafe")) {
            // Il est sur le cocotier, sans la perle
            SetVisible("parrot_nopearl", true);
         }
         else {
            // Il est sur le coffre
            SetVisible("parrot_nopearl", false);
            SetVisible("parrot_safe", true);
         }
      }
   }
   
   // Le requin
   if (!TaskResolved("task_island_expelshark")) {
      SetVisible("island_shark", true);
      StartAnimation("sharkanim");
   }
   else {
      SetVisible("island_shark", false);
   }
   
   // La perle dans le sable
   if (TaskResolved("task_island_fallpearl") && !TaskResolved("task_island_getpearl")) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "pearl_sand", true);
   }
   
   // La croix
   if (TaskResolved("task_island_findsafe") && !TaskResolved("task_island_digsand")) {
      SetVisible("island_cross", true);
      SetVisible("island_sand",false);
   }
   
   // Coffre ouvert ou fermé
   if (TaskResolved("task_island_opensafe")) {
      SetVisible("safe_close", false);
      SetVisible("safe_open", true);
      SetVisible("island_sand",false);
   }
   else {
      if (TaskResolved("task_island_digsand")) {
         SetVisible("safe_close", true);
         SetVisible("island_sand",false);
      }
      else {
         SetVisible("safe_close", false);
      }
      SetVisible("safe_open", false);
   }
   
   if (TaskResolved("task_island_revealqueen")) {
      SetVisible("island_queen", true);
      SetVisible("island_queen_stone", true);
   }
   
   if (getAdditionalName() == "completed") {
      int x,y;
      GetObjectPosition("island_queen", x, y, true, false);
      ResolveTask("task_island_revealqueen");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "island_queen", true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ISLAND_QUEEN_EPILOG1"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_MURRAY_EPILOG1"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ISLAND_QUEEN_EPILOG2"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_MURRAY_EPILOG2"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ISLAND_QUEEN_EPILOG3"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ISLAND_QUEEN_EPILOG4"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ISLAND_QUEEN_EPILOG5"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ISLAND_QUEEN_EPILOG6"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_MURRAY_EPILOG3"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ISLAND_QUEEN_EPILOG7"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_MURRAY_EPILOG4"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ISLAND_QUEEN_EPILOG8"), "", false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      ReturnToHouse();
   }
}

void Scene_Island_Beach::ReturnToHouse()
{
   ResolveTask("task_island_universeresolved");
   //_lpSceneDirector->getMap()->setUniversVisited("UniverseIsland");
   // Retour
   Loader::LoadScene("house_diningroom");
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE);
   _lpSceneDirector->getSequencer()->ShowImage(NULL, "blank", true, true);
   _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/soul_tunnel_r.ogv","tunnel_r");
   _lpSceneDirector->getSequencer()->GoToScene(NULL,"house_diningroom","backfrombeyond", false);
}

void Scene_Island_Beach::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      ResolveTask("task_island_opensafe");
      Init();
   }
   if (KInput::isPressed(K_VK_F7))
   {
      setAdditionalName("completed");
      Init();
   }
#endif
}

void Scene_Island_Beach::Logic()
{
	EScene::Logic();
}

void Scene_Island_Beach::Draw()
{
   EScene::Draw();
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   _lpCounterWave->move(fElapsed);
   _lpCounterCrab->move(fElapsed);
   if (_lpCounterWave->isCompleted()) {
//      if (isVisible("bg_waves1")) {
//         SetVisible("bg_waves1", false);
//         SetVisible("bg_waves2", true);
//      }
//      else {
//         SetVisible("bg_waves1", true);
//         SetVisible("bg_waves2", false);
//      }
      
      _lpCounterWave->startCounter(0, 1, 0, 3000, K_COUNTER_LINEAR);
   }
   if (_lpCounterCrab->isCompleted()) {
      SetupItem("island_crab");
      if (isVisible("island_crab")) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "crabwalk");
         StartAnimation("island_crab_anim");
         _lpCounterCrab->startCounter(0, 1, 0, GetRandom(5000, 15000), K_COUNTER_LINEAR);
      }
   }
}

void Scene_Island_Beach::Close()
{
   
}

bool Scene_Island_Beach::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Objets à ramasser
   if (strcmp(szObjectName, "island_crab") == 0) {
      // On vérifie que le crabe est bien en train de passer
      if (GetObjectImageByName("island_crab")->GetSmartImage()->isVideoPlaying()) {
         PickupSimple(szObjectName, "inv_island_crab");
      }
      else {
         _lpSceneDirector->getSequencer()->Talk("sand", CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_MURRAY_MISSED"), "", true);
      }

      return true;
   }
   if (strcmp(szObjectName, "island_berry") == 0) {
      PickupMultiple(szObjectName, "inv_island_berry", -1);
//      PickupSimple(szObjectName, "inv_island_berry");
      return true;
   }
   
   // Les trous dans le sable
   if (strcmp(szObjectName, "island_sand") == 0) {
      _lpSceneDirector->getSequencer()->Talk("sand", CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_MURRAY_SAND"), "", true);
      return true;
   }
   
   // La reine
   if (strcmp(szObjectName, "island_queen") == 0)
   {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
      ResolveTask("task_island_getstone");
      ReturnToHouse();
      return true;
   }
   
   // Coffre fermé
   if (strcmp(szObjectName, "safe_close") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk("clic", CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_MURRAY_MYSAFE"), "", true);
      return true;
   }
   
   // Coffre ouvert
   if (strcmp(szObjectName, "safe_open") == 0)
   {
      _lpSceneDirector->GoToVignette("vignette_island_safe", "");
      return true;
   }   

   if (strcmp(szObjectName, "dive") == 0)
   {
      ESoundBank::getSound("divestart")->playSample();
      _lpSceneDirector->GoToScene("island_dive");
      return true;
   }

   if (strcmp(szObjectName, "parrot_pearl") == 0 && !TaskResolved("task_island_fallpearl"))
   {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_PARROT_PEARL"), "", true);
      return true;
   }

   if (strcmp(szObjectName, "parrot_nopearl") == 0)
   {
      int x,y;
      GetObjectPosition(szObjectName, x, y);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("ISLAND_BEACH_PARROT_NOPEARL"), "", false);
      return true;
   }
   
   if (strcmp(szObjectName, "pearl_sand") == 0)
   {
      EMiniJeuBase *lpMiniJeu = new EMiniJeuSearchgrid();
      _lpSceneDirector->GoToMiniGame("digit", lpMiniJeu);
   }
   
   return false;
}

bool Scene_Island_Beach::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Island_Beach::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur utilise la clé dans le coffre
   if ( strcmp(szItemName, "inv_island_key") == 0 ) {
      if (strcmp(szObjectName, "safe_close") == 0 )
      {
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "opensafe");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "safe_close", "safe_open");
         ResolveTask("task_island_opensafe");
         // On retire la clé de l'inventaire
         _lpSceneDirector->DropItem("inv_island_key");
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_MURRAY_IWASWAITING"), "", true);
         return true;
      }
   }
   
   // On essaye de creuser
   if ( strcmp(szObjectName, "island_sand") == 0 ) {
      // On a déjà trouvé le coffre
      if (TaskResolved("task_island_findsafe")) {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_MURRAY_CROSSOK"), "", true);
      }
      // On utilise la carte pour trouver le coffre
      else if (strcmp(szItemName, "inv_island_map") == 0)
      {
         EMiniJeuBase *lpMiniJeu = new EMiniJeuSimonShark();
         _lpSceneDirector->GoToMiniGame("sharksimon", lpMiniJeu);
         return true;
      }
      else if (strcmp(szItemName, "inv_island_shovelfull") == 0 || strcmp(szItemName, "inv_island_shovelstrong") == 0)
      {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_MURRAY_SAND"), "", true);
         return true;
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_MURRAY_NEEDMAP"), "", true);
         return true;
      }
   }

   // On essaye de creuser avec une pelle pas solide !
   if ( strcmp(szObjectName, "island_cross") == 0 ) {
      if (strcmp(szItemName, "inv_island_shovelfull") == 0)
      {
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "shovelbreak");
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getInventory()->AddItem("island","inv_island_shovelbroken");
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_MURRAY_BROKE"), "", true);
         return true;
      }
      else if (strcmp(szItemName, "inv_island_shovelstrong") == 0) {
         ResolveTask("task_island_digsand");
         AddTask("task_island_opensafe");
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "island_cross", "safe_close");
         return true;
      }
      else if (strcmp(szItemName, "inv_island_shovelhead") == 0) {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_MURRAY_HANDLE"), "", true);
         return true;
      }
   }
   
   // On dégomme le perroquet
   if ( strcmp(szObjectName, "parrot_pearl") == 0 ) {
      if (strcmp(szItemName, "inv_island_blowpipearmed") == 0 )
      {
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "blowpipe");
         int x,y;
         GetObjectPosition(szObjectName, x, y);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "parrot_nopearl", true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "parrot_pearl", false);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "falling_pearl", true);
         _lpSceneDirector->getSequencer()->Animation(NULL, "pearl_animp2p", true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "pearl_sand", true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "falling_pearl", false);
         ResolveTask("task_island_fallpearl");
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         return true;
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_MURRAY_NOBERRY"), "", true);
         return true;
      }
   }
         
   return false;
}

void Scene_Island_Beach::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "digit")==0 && bIsRevolved) {
      int x,y;
      ESoundBank::getSound("success")->playSample();
      GetObjectPosition("pearl_sand", x, y, true, true);
      _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_island_pearl", (float)x, (float)y, 1);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "pearl_sand", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_MURRAY_MINE"), "", true);
      ResolveTask("task_island_getpearl");
      ResolveObjective("island","pearl");
   }
   else if (strcmp(szGameName, "sharksimon")==0 && bIsRevolved) {
      _lpSceneDirector->DropItem("inv_island_map");
      ESoundBank::getSound("success")->playSample();
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow", true);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "island_cross", true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_CROSS_INFO"), "", true);
      ResolveTask("task_island_findsafe");
      AddTask("task_island_digsand");
      SetVisible("island_sand",false);
   }
}
