/*
 *  Vignette_Island_Safe.cpp
 *  enigma
 *
 *  Created by Rockford on 18/08/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Vignette_Island_Safe.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuTresor.h"

/* Constructeur */
Vignette_Island_Safe::Vignette_Island_Safe(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   EMiniJeuTresor::Preload();
}

/* Destructeur */
Vignette_Island_Safe::~Vignette_Island_Safe()
{
}

void Vignette_Island_Safe::Init()
{
   SetupItem("island_safe_neckless[2]");
   _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_BEACH_NECKLESS_INFO"), "", false, true);
}

void Vignette_Island_Safe::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Vignette_Island_Safe::Logic()
{
	EScene::Logic();
}

void Vignette_Island_Safe::Draw()
{
   EScene::Draw();
}

void Vignette_Island_Safe::Close()
{
}

bool Vignette_Island_Safe::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "close") == 0)
   {
      _lpSceneDirector->CloseVignette();
   }
   
   // Le pendentif
   if (strcmp(szObjectName, "island_safe_neckless[2]") == 0)
   {
      ResolveObjective("island","findchest");
      PickupMultiple(szObjectName,"inv_island_neckless1",2);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "successisland");     
   }
   
   return false;
}

bool Vignette_Island_Safe::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Vignette_Island_Safe::ItemUsed(const char *szItemName, const char *szObjectName)
{
   return false;
}

void Vignette_Island_Safe::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
}