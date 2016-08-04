/*
 *  Vignette_Diningroom_Lib.cpp
 *  enigma
 *
 *  Created by Rockford on 14/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Vignette_Diningroom_Lib.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuJigsawBook.h"

/* Constructeur */
Vignette_Diningroom_Lib::Vignette_Diningroom_Lib(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   EMiniJeuJigsawBook::Preload();
}

/* Destructeur */
Vignette_Diningroom_Lib::~Vignette_Diningroom_Lib()
{
}

void Vignette_Diningroom_Lib::Init()
{
   if (!TaskResolved("task_house_dining_openbook")) {
      StartEmitter("flake");
   }
   else {
      _lpSceneDirector->GoToScene("house_diningroom","");
   }
}

void Vignette_Diningroom_Lib::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Vignette_Diningroom_Lib::Logic()
{
	EScene::Logic();
}

void Vignette_Diningroom_Lib::Draw()
{
   EScene::Draw();
}

void Vignette_Diningroom_Lib::Close()
{
}

bool Vignette_Diningroom_Lib::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "close") == 0)
   {
      _lpSceneDirector->CloseVignette();
      return true;
   }
   if (strcmp(szObjectName, "pages") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk("pages", CHARACTER_POSX, CHARACTER_POSY, KStr("DININGLIB_ASHLEY_MISSPART"), "", false);
//      _lpSceneDirector->getSequencer()->GoToScene(NULL, "vignette_diningroom_book", "", true);
      return true;
   }
   return false;
}

bool Vignette_Diningroom_Lib::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Vignette_Diningroom_Lib::ItemUsed(const char *szItemName, const char *szObjectName)
{
   bool bResult = false;
   // Le joueur utilise la page sur les livres
   if ( strcmp(szItemName, "inv_dining_page") == 0 ) {
      if ( strcmp(szObjectName, "pages") == 0 ) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"book_pageturn");
         _lpSceneDirector->GoToVignette("vignette_diningroom_book", "", true);
         bResult = true;
      }
   }
   return bResult;
}

void Vignette_Diningroom_Lib::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
}