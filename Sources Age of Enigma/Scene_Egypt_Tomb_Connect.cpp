/*
 *  Scene_Egypt_Tomb_Connect.cpp
 *  enigma
 *
 *  Created by Rockford on 28/12/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Egypt_Tomb_Connect.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "MyGame.h"
#include "EMiniJeuConnect.h"

/* Constructeur */
Scene_Egypt_Tomb_Connect::Scene_Egypt_Tomb_Connect(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _lpAnimDoor = new KCounter();
   _bAnimDoor = false;
   memset(_szZoneBuffer, 0, 256);
   EMiniJeuConnect::Preload();
}

/* Destructeur */
Scene_Egypt_Tomb_Connect::~Scene_Egypt_Tomb_Connect()
{
   XDELETE(_lpAnimDoor);
   XDELETE(_lpBgGraphic);
}

void Scene_Egypt_Tomb_Connect::Init()
{
   if (!TaskResolved("task_egypt_ordergods")) {
      AddTask("task_egypt_ordergods");
      
      if (!TaskResolved("task_egypt_discoverordergods")) {
         ResolveTask("task_egypt_discoverordergods");
         // Positionne quelques dieux
         EGlobalBank::AddStrValue("egypt_tomb_connect_zone01","egypt_tomb_connect_god02");
         
         EGlobalBank::AddStrValue("egypt_tomb_connect_zone03","egypt_tomb_connect_god05");
         
         EGlobalBank::AddStrValue("egypt_tomb_connect_zone04","egypt_tomb_connect_god06");
         
         EGlobalBank::AddStrValue("egypt_tomb_connect_zone07","egypt_tomb_connect_god03");
      }
      
      SetupGods(true);
   }
   else {
      // Efface les portes
      SetVisible("egypt_tomb_connect_doorleft", false, true);
      SetVisible("egypt_tomb_connect_doorright", false, true);
      SetVisible("egypt_tomb_connect_godsdone", true, true);
      SetVisible("egypt_tomb_connect_zonemg", true);
   }
}

void Scene_Egypt_Tomb_Connect::SetupGods(bool bClear)
{
   const char *sz;

   // On rend invisible tous les symboles
   if (bClear) {
      SetVisible("egypt_tomb_connect_god01", false, true);
      SetVisible("egypt_tomb_connect_god02", false, true);
      SetVisible("egypt_tomb_connect_god03", false, true);
      SetVisible("egypt_tomb_connect_god04", false, true);
      SetVisible("egypt_tomb_connect_god05", false, true);
      SetVisible("egypt_tomb_connect_god06", false, true);
      SetVisible("egypt_tomb_connect_god07", false, true);
   }
   
   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone01","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      // On positionne le symbole dans cet emplacement
      SetPosition(sz, 199, 557);
      SetVisible(sz, true);
   }

   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone02","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      // On positionne le symbole dans cet emplacement
      SetPosition(sz, 281, 557);
      SetVisible(sz, true);
   }

   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone03","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      // On positionne le symbole dans cet emplacement
      SetPosition(sz, 363, 557);
      SetVisible(sz, true);
   }

   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone04","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      // On positionne le symbole dans cet emplacement
      SetPosition(sz, 445, 557);
      SetVisible(sz, true);
   }

   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone05","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      // On positionne le symbole dans cet emplacement
      SetPosition(sz, 527, 557);
      SetVisible(sz, true);
   }

   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone06","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      // On positionne le symbole dans cet emplacement
      SetPosition(sz, 609, 557);
      SetVisible(sz, true);
   }

   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone07","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      // On positionne le symbole dans cet emplacement
      SetPosition(sz, 691, 557);
      SetVisible(sz, true);
   }
   
}

void Scene_Egypt_Tomb_Connect::CheckGods()
{
   if (strcmp(EGlobalBank::getStrValue("egypt_tomb_connect_zone01",""),"egypt_tomb_connect_god01") == 0
       && strcmp(EGlobalBank::getStrValue("egypt_tomb_connect_zone02",""),"egypt_tomb_connect_god02") == 0
       && strcmp(EGlobalBank::getStrValue("egypt_tomb_connect_zone03",""),"egypt_tomb_connect_god03") == 0
       && strcmp(EGlobalBank::getStrValue("egypt_tomb_connect_zone04",""),"egypt_tomb_connect_god04") == 0
       && strcmp(EGlobalBank::getStrValue("egypt_tomb_connect_zone05",""),"egypt_tomb_connect_god05") == 0
       && strcmp(EGlobalBank::getStrValue("egypt_tomb_connect_zone06",""),"egypt_tomb_connect_god06") == 0
       && strcmp(EGlobalBank::getStrValue("egypt_tomb_connect_zone07",""),"egypt_tomb_connect_god07") == 0
       ) {
      SetVisible("egypt_tomb_connect_godsdone", true);
      RemoveHint("egypt","ritual","order");
      ResolveTask("task_egypt_ordergods");
      AddTask("task_egypt_purifying");
      StartEffect();
   }
   //egypt_tomb_connect_godsdone
}

void Scene_Egypt_Tomb_Connect::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
   }
#endif
}

void Scene_Egypt_Tomb_Connect::Logic()
{
	EScene::Logic();
}

void Scene_Egypt_Tomb_Connect::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   if (_bAnimDoor) {
      _lpAnimDoor->move(fElapsed);
      // Déplace les porte
      RestorePosition("egypt_tomb_connect_doorleft");
      RestorePosition("egypt_tomb_connect_doorright");
      MovePosition("egypt_tomb_connect_doorleft", 0.0f-_lpAnimDoor->getCurrentValue(), GetRandom(-1.0, 1.0));
      MovePosition("egypt_tomb_connect_doorright", _lpAnimDoor->getCurrentValue(), GetRandom(-1.0, 1.0));
      if (_lpAnimDoor->isCompleted()) {
         _bAnimDoor = false;
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"reveal");
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_NEFERES_RESOLVECONNECT"), "", false, true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt_tomb_connect_zonemg", true);
      }
   }

   EScene::Draw();
}

void Scene_Egypt_Tomb_Connect::Close()
{
}

void Scene_Egypt_Tomb_Connect::StartEffect()
{
   _bAnimDoor = true;
   ESoundBank::getSound("rockdoor")->playSample();
   _lpAnimDoor->startCounter(0.0f, 75.0f, 0, 10000, K_COUNTER_EASEOUT);
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
}

bool Scene_Egypt_Tomb_Connect::ObjectClicked(const char *szObjectName, float x, float y)
{
   const char *sz;
   
   // Mini jeu connect
   if (strcmp(szObjectName, "egypt_tomb_connect_zonemg") == 0) {
      EMiniJeuBase *lpMiniJeu = new EMiniJeuConnect();
      _lpSceneDirector->getSequencer()->MiniGame(NULL, "connect", lpMiniJeu);
      return true;
   }
   
   // Retrait des symboles
   if (!TaskResolved("task_egypt_ordergods")) {
      if ( strcmp(szObjectName, "egypt_tomb_connect_god01") == 0 )
      {
         PickupSimple(szObjectName, "inv_egypt_god01");         
         // L'emplacement ne contient plus de symbole
         sz = getZoneContaining(szObjectName);
         EGlobalBank::AddStrValue(sz, "", false);
      }
      if ( strcmp(szObjectName, "egypt_tomb_connect_god02") == 0 )
      {
         PickupSimple(szObjectName, "inv_egypt_god02");
         // L'emplacement ne contient plus de symbole
         sz = getZoneContaining(szObjectName);
         EGlobalBank::AddStrValue(sz, "", false);
      }
      if ( strcmp(szObjectName, "egypt_tomb_connect_god03") == 0 )
      {
         PickupSimple(szObjectName, "inv_egypt_god03");
         // L'emplacement ne contient plus de symbole
         sz = getZoneContaining(szObjectName);
         EGlobalBank::AddStrValue(sz, "", false);
      }
      if ( strcmp(szObjectName, "egypt_tomb_connect_god04") == 0 )
      {
         PickupSimple(szObjectName, "inv_egypt_god04");
         // L'emplacement ne contient plus de symbole
         sz = getZoneContaining(szObjectName);
         EGlobalBank::AddStrValue(sz, "", false);
      }
      if ( strcmp(szObjectName, "egypt_tomb_connect_god05") == 0 )
      {
         PickupSimple(szObjectName, "inv_egypt_god05");
         // L'emplacement ne contient plus de symbole
         sz = getZoneContaining(szObjectName);
         EGlobalBank::AddStrValue(sz, "", false);
      }
      if ( strcmp(szObjectName, "egypt_tomb_connect_god06") == 0 )
      {
         PickupSimple(szObjectName, "inv_egypt_god06");
         // L'emplacement ne contient plus de symbole
         sz = getZoneContaining(szObjectName);
         EGlobalBank::AddStrValue(sz, "", false);
      }
      if ( strcmp(szObjectName, "egypt_tomb_connect_god07") == 0 )
      {
         PickupSimple(szObjectName, "inv_egypt_god07");
         // L'emplacement ne contient plus de symbole
         sz = getZoneContaining(szObjectName);
         EGlobalBank::AddStrValue(sz, "", false);
      }
   }
   return false;
}

bool Scene_Egypt_Tomb_Connect::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Egypt_Tomb_Connect::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if ( strncmp(szItemName, "inv_egypt_god", strlen("inv_egypt_god")) == 0 )
   {
      if ( strncmp(szObjectName, "egypt_tomb_connect_zone", strlen("egypt_tomb_connect_zone")) == 0 )
      {
         // Constitue le nom de l'image à mettre dans la zone
         // C'est egypt_tomb_connect_godXX où XX est le numéro du dieu
         char szImageName[256];
         memset(szImageName, 0, 256);
         strcpy(szImageName, "egypt_tomb_connect_god");
         // Numéro du dieu (2 dernier caractère de l'objet de l'inventaire)
         char szGodNumber[256];
         memset(szGodNumber, 0, 256);
         strncpy(szGodNumber, szItemName+strlen("inv_egypt_god"), 2);
         // On assemble le tout
         strcat(szImageName, szGodNumber);
         // Enfin, on inscrit le bordel dans le machin
         EGlobalBank::AddStrValue(szObjectName,szImageName,false);
         SetupGods(false);
         // On enlève le symbole de l'inventaire
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "mg_good");
         // On teste si tout est en place
         CheckGods();
         return true;
      }
   }
   
   return false;
}

void Scene_Egypt_Tomb_Connect::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (bIsRevolved) {
      ResolveTask("task_egypt_purifying");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_LOCKMOUSE, true);
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "egypt_tomb", "completed", false);
   }
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}

char *Scene_Egypt_Tomb_Connect::getZoneContaining(const char *szSymbolName)
{
   const char *sz;
    
   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone01","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      if (strcmp(sz, szSymbolName) == 0) {
         strcpy(_szZoneBuffer, "egypt_tomb_connect_zone01");
         return _szZoneBuffer;
      }
   }
   
   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone02","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      if (strcmp(sz, szSymbolName) == 0) {
         strcpy(_szZoneBuffer, "egypt_tomb_connect_zone02");
         return _szZoneBuffer;
      }
   }
   
   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone03","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      if (strcmp(sz, szSymbolName) == 0) {
         strcpy(_szZoneBuffer, "egypt_tomb_connect_zone03");
         return _szZoneBuffer;
      }
   }
   
   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone04","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      if (strcmp(sz, szSymbolName) == 0) {
         strcpy(_szZoneBuffer, "egypt_tomb_connect_zone04");
         return _szZoneBuffer;
      }
   }
   
   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone05","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      if (strcmp(sz, szSymbolName) == 0) {
         strcpy(_szZoneBuffer, "egypt_tomb_connect_zone05");
         return _szZoneBuffer;
      }
   }
   
   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone06","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      if (strcmp(sz, szSymbolName) == 0) {
         strcpy(_szZoneBuffer, "egypt_tomb_connect_zone06");
         return _szZoneBuffer;
      }
   }
   
   sz = EGlobalBank::getStrValue("egypt_tomb_connect_zone07","");
   // Il y a bien un symbole dans cet emplacement
   if (strlen(sz) == strlen("egypt_tomb_connect_god00")) {
      if (strcmp(sz, szSymbolName) == 0) {
         strcpy(_szZoneBuffer, "egypt_tomb_connect_zone07");
         return _szZoneBuffer;
      }
   }
   return NULL;
}
