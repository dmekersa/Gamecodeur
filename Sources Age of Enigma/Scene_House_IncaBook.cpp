/*
 *  Scene_House_IncaBook.cpp
 *  enigma
 *
 *  Created by Rockford on 21/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuTemplate.h"
#include "FontBank.h"

#include "Scene_House_IncaBook.h"

#define PAGE(x) _vPages[x]->c_str()
#define SLOT(x) _vSlots[x]->c_str()
#define STAMP(x) _vStamps[x]->c_str()

#define HOUSE_BOOK_CURRENT_PAGE "house_book_page"

/* Constructeur */
Scene_House_IncaBook::Scene_House_IncaBook(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
	_nCurrentPage = atoi(GetStrGlobal(HOUSE_BOOK_CURRENT_PAGE,"0"));
   _lpFont = EFontBank::getFont("tahoma.ttf",12);

   EMiniJeuTemplate::Preload();
}

/* Destructeur */
Scene_House_IncaBook::~Scene_House_IncaBook()
{
   XDELETE(_lpFont);
}

void Scene_House_IncaBook::Init()
{
   if (!TaskResolved("task_house_incabook_discover")) {
      ResolveTask("task_house_incabook_discover");
      AddObjective("house","living_buildbase");
      AddHint("house","living_buildbase","fire");
   }
   
	PageSet(_nCurrentPage);
}

void Scene_House_IncaBook::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_House_IncaBook::Logic()
{
	EScene::Logic();
}

void Scene_House_IncaBook::Draw()
{
	EScene::Draw();
	_lpFont->drawStringFromLeft(itos(_nCurrentPage+1).c_str(),50,50);
}

void Scene_House_IncaBook::Close()
{
}

bool Scene_House_IncaBook::ObjectClicked(const char *szObjectName, float x, float y)
{
	if (strcmp(szObjectName, "livingroom_book_left") == 0 ) {
		PageTurn(true);
	}
	if (strcmp(szObjectName, "livingroom_book_right") == 0 ) {
		PageTurn(false);
	}
	return false;
}

bool Scene_House_IncaBook::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Scene_House_IncaBook::ItemUsed(const char *szItemName, const char *szObjectName)
{
	return false;
}

void Scene_House_IncaBook::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
}

void Scene_House_IncaBook::PageTurn(bool left) {
	if ((left && _nCurrentPage == 0) || (!left && _nCurrentPage == _nPages -1))
		return;

	if (left) {
		PageSet(_nCurrentPage -1);
	} else {
		PageSet(_nCurrentPage +1);
	}
	ESoundBank::getSound("book_pageturn")->playSample();
}

void Scene_House_IncaBook::PageSet(int page) {
	KAssert(page < _nPages && page >= 0);
	
	_nCurrentPage = page;

	// Activation des zone pour tourner les pages
	SetVisible("livingroom_book_left",true);
	SetVisible("livingroom_book_right",true);
	if (_nCurrentPage == 0)
		SetVisible("livingroom_book_left",false);
	if (_nCurrentPage == _nPages -1)
		SetVisible("livingroom_book_right",false);

   // Efface tout
   SetVisible("livingroom_book_page1", false);
   SetVisible("txt_title_left_page1", false);
   SetVisible("txt_subtitle_left_page1", false);

   SetVisible("livingroom_book_page2", false);
   SetVisible("txt_title_left_page2", false);
   SetVisible("txt_subtitle_left_page2", false);
   SetVisible("txt_text_page2", false);

   SetVisible("livingroom_book_page3", false);
   SetVisible("txt_title_left_page3", false);
   SetVisible("txt_subtitle_left_page3", false);
   SetVisible("txt_text_page3", false);

   SetVisible("livingroom_book_page4", false);
   SetVisible("txt_title_left_page4", false);
   SetVisible("txt_subtitle_left_page4", false);
   SetVisible("txt_text_page4", false);

   SetVisible("livingroom_book_page5", false);
   SetVisible("txt_title_left_page5", false);
   SetVisible("txt_subtitle_left_page5", false);
   SetVisible("txt_text_page5", false);

   SetVisible("livingroom_book_page6", false);
   SetVisible("txt_title_left_page6", false);
   SetVisible("txt_subtitle_left_page6", false);
   SetVisible("txt_text_page6", false);
   
   switch (_nCurrentPage) {
      case 0:
         SetVisible("livingroom_book_page1", true);
         SetVisible("txt_subtitle_left_page1", true);
         SetVisible("txt_title_left_page1", true);
         break;
      case 1:
         SetVisible("livingroom_book_page2", true);
         SetVisible("txt_subtitle_left_page2", true);
         SetVisible("txt_title_left_page2", true);
         SetVisible("txt_text_page2", true);
         break;
      case 2:
         SetVisible("livingroom_book_page3", true);
         SetVisible("txt_subtitle_left_page3", true);
         SetVisible("txt_title_left_page3", true);
         SetVisible("txt_text_page3", true);
         break;
      case 3:
         SetVisible("livingroom_book_page4", true);
         SetVisible("txt_subtitle_left_page4", true);
         SetVisible("txt_title_left_page4", true);
         SetVisible("txt_text_page4", true);
         break;
      case 4:
         SetVisible("livingroom_book_page5", true);
         SetVisible("txt_subtitle_left_page5", true);
         SetVisible("txt_title_left_page5", true);
         SetVisible("txt_text_page5", true);
         break;
      case 5:
         SetVisible("livingroom_book_page6", true);
         SetVisible("txt_subtitle_left_page6", true);
         SetVisible("txt_title_left_page6", true);
         SetVisible("txt_text_page6", true);
         break;
      default:
         break;
   }

	// Mémorisation de la page courante
	SetGlobal(HOUSE_BOOK_CURRENT_PAGE,itos(_nCurrentPage).c_str());
	
}
