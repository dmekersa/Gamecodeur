
/*
 *  Vignette_Diningroom_Book.cpp
 *  enigma
 *
 *  Created by Rockford on 13/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Vignette_Diningroom_Book.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuJigsawBook.h"

/* Constructeur */
Vignette_Diningroom_Book::Vignette_Diningroom_Book(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   EMiniJeuJigsawBook::Preload();
}

/* Destructeur */
Vignette_Diningroom_Book::~Vignette_Diningroom_Book()
{
}

void Vignette_Diningroom_Book::Init()
{
   if (!TaskResolved("task_house_dining_openbook")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGBOOK_ASHLEY_ILLUS"),"", true, false);
      SetVisible("bookbg_broken", true);
      SetVisible("book_jigsawzone", true);
   }
   else {
      SetVisible("bookbg_repaired", true);
      SetVisible("book_jigsawzone", false);
   }
}

void Vignette_Diningroom_Book::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
  if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Vignette_Diningroom_Book::Logic()
{
	EScene::Logic();
}

void Vignette_Diningroom_Book::Draw()
{
   EScene::Draw();
}

void Vignette_Diningroom_Book::Close()
{
}

bool Vignette_Diningroom_Book::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "back") == 0)
   {
      _lpSceneDirector->CloseVignette();
   }
   else if (strcmp(szObjectName, "bookbg_broken") == 0 || strcmp(szObjectName, "bookbg_repaired") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk("stopfire", CHARACTER_POSX, CHARACTER_POSY, KStr("DININGBOOK_ASHLEY_CLUE"), "", false);
   }
   else if (strcmp(szObjectName, "book_jigsawzone") == 0)
   {
      EMiniJeuBase *lpMiniJeu = new EMiniJeuJigsawBook();
      ((EMiniJeuJigsawBook *) lpMiniJeu)->SetVariation(1);
      _lpSceneDirector->GoToMiniGame("book", lpMiniJeu);
      return true;
   }
   return false;
}

bool Vignette_Diningroom_Book::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Vignette_Diningroom_Book::ItemUsed(const char *szItemName, const char *szObjectName)
{
   bool bResult = false;
   return bResult;
}

void Vignette_Diningroom_Book::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   // On peut maintenant retirer l'objet de l'inventaire
   if (strcmp(szGameName, "book")==0 && bIsRevolved) {
      ESoundBank::getSound("success")->playSample();
      _lpSceneDirector->DropItem("inv_dining_page");
      ResolveTask("task_house_dining_openbook");
      // On a maintenant l'info pour ouvrir le 2ème tiroir
      AddTask("task_house_dining_seconddrawer");
      Init();
   }
}