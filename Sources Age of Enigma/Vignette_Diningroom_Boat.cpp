/*
 *  Vignette_Diningroom_Boat.cpp
 *  enigma
 *
 *  Created by Rockford on 13/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Vignette_Diningroom_Boat.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuTresor.h"

/* Constructeur */
Vignette_Diningroom_Boat::Vignette_Diningroom_Boat(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   EMiniJeuTresor::Preload();
}

/* Destructeur */
Vignette_Diningroom_Boat::~Vignette_Diningroom_Boat()
{
}

void Vignette_Diningroom_Boat::Init()
{
   // Le bateau
   StopEmitter("flake");
   if (!TaskResolved("task_house_dining_repairboat")) {
      SetVisible("boatbrokenbg", true, true);
      SetVisible("mast", true, true);
   }
   else {
      SetVisible("mast", false, true);
      if (!TaskResolved("task_house_dining_openboat")) {
         SetVisible("boatrepairedbg", true, true);
         SetVisible("windows", true, true);
         StartEmitter("flake");
      }
      else {
         if (!TaskResolved("task_house_dining_taketreasure")) {
            SetVisible("boatopenbg", true, true);
            SetVisible("windows", false, true);
            // Le trésor
            SetVisible("treasure", TestGlobal("treasure") == false);
         }
         else {
            SetVisible("boatemptybg", true, true);
            SetVisible("windows", false, true);
            // Le trésor
            SetVisible("treasure", false, true);
         }
      }
   }
   
}

void Vignette_Diningroom_Boat::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      MiniGameDone("code", true);
   }
#endif
}

void Vignette_Diningroom_Boat::Logic()
{
	EScene::Logic();
}

void Vignette_Diningroom_Boat::Draw()
{
   EScene::Draw();
}

void Vignette_Diningroom_Boat::Close()
{
}

bool Vignette_Diningroom_Boat::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "close") == 0)
   {
      _lpSceneDirector->CloseVignette();
   }
   
   if (strcmp(szObjectName, "windows") == 0)
   {
      EMiniJeuBase *lpMiniJeu = new EMiniJeuTresor();
      _lpSceneDirector->GoToMiniGame("code", lpMiniJeu);
   }
   
   // Objets à ramasser
   // trésor
   if ( strcmp(szObjectName, "treasure") == 0 )
   {
      PickupSimple(szObjectName,"inv_dining_treasure");
      SetVisible("boatrepairedbg", false);
      SetVisible("boatemptybg", true);
      AddTask("task_house_dining_givetreasure");
      ResolveTask("task_house_dining_findtreasure");
      ResolveTask("task_house_dining_taketreasure");
   }
   return false;
}

bool Vignette_Diningroom_Boat::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Vignette_Diningroom_Boat::ItemUsed(const char *szItemName, const char *szObjectName)
{
   bool bResult = false;
   if ( strcmp(szItemName, "inv_dining_mast") == 0 ) {
      if (strcmp(szObjectName, "mast") == 0 )
      {
         ESoundBank::getSound("success")->playSample();
         ESoundBank::getSound("openboat")->playSample();
         ResolveTask("task_house_dining_repairboat");
         AddTask("task_house_dining_openboat");
         SetVisible("mast", false, true);
         SetVisible("boatrepairedbg", true);
         SetVisible("windows", true);
         StartEmitter("flake");
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGCLOCK_ASHLEY_BOATREPAIRED"), "", false);
         // On retire le mat de l'inventaire
         _lpSceneDirector->DropItem("inv_dining_mast");
         return true;
      }
   }
   return bResult;
}

void Vignette_Diningroom_Boat::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "code")==0 && bIsRevolved) {
      SetVisible("boatrepairedbg", false);
      SetVisible("boatbrokenbg", false);
      SetVisible("boatopenbg", true);
      SetVisible("treasure", true);
      SetVisible("windows", false);
      ResolveTask("task_house_dining_openboat");
      ResolveObjective("house","repairboat");
      _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
      _lpSceneDirector->getSequencer()->PlaySound(NULL,"openboat");
   }
}