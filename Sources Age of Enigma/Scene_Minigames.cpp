/*
*  Scene_Minigames.cpp
*  enigma
*
*  Created by Rockford on 20/04/10.
*  Copyright 2010 Casual Games France. All rights reserved.
*
*/

#include "EScene.h"
#include "Scene_Minigames.h"
#include "ESceneDirector.h"
#include "MyGame.h"
#include "SoundBank.h"
#include "ImageBank.h"
#include "EPointer.h"

#include "EMiniJeuTemplate.h"
#include "EMiniJeuConnect.h"
#include "EMiniJeuSearchgrid.h"
#include "EMiniJeuTresor.h"
#include "EMiniJeuDice.h"
#include "EMiniJeuPicross.h"
#include "EMiniJeuJigsawBook.h"
#include "EMiniJeuSimonShark.h"
#include "EMiniJeuMikado.h"
#include "EMiniJeuMasterMind.h"
#include "EMiniJeuTest.h"
#include "EMinijeuBonzai.h"
#include "EMiniJeuUnscrew.h"
#include "EMiniJeuZenGarden.h"
#include "EMiniJeuStones.h"
#include "EMiniJeuGardening.h"
#include "EMiniJeuPlantAddition.h"
#include "EMiniJeuMemoryPlant.h"
#include "EMiniJeuPiano.h"
#include "EMiniJeuMirorBeam.h"
#include "EMiniJeuCubes.h"
#include "EMiniJeuMissingParts.h"
#include "EMiniJeuPhotoComp.h"
#include "EMiniJeuSymbolMatch.h"
#include "EMiniJeuFlacons.h"
#include "EMiniJeuPoltergeist.h"
#include "EMiniJeuDemelage.h"
#include "EMiniJeuPentagram.h"
#include "EMiniJeuMatch3.h"
//#include "EMiniJeuVideoDisplay.h"


class BoxGameShot: public EBox {
public:
	BoxGameShot(int gameNumber): EBox(0,0,0,0) {
		_nGameNumber = gameNumber;
		_bTaskIsResolved = false;

		_lpScreenShotBg = EImageBank::getImage("mg_screenshot_frame","png");
		_lpScreenShotOverlay = EImageBank::getImage("mg_screenshot_overlay","png",true);
		_lpScreenShot = EImageBank::getImage((std::string("mg_screenshot_") + itos(_nGameNumber)).c_str(),"png",true);
		if (_lpScreenShot == NULL)
			_lpScreenShot = EImageBank::getImage("mg_default_screenshot","png");


		SetSizePolicy(FREE);
		SetSize(_lpScreenShotBg->getWidth(),_lpScreenShotBg->getHeight());
		SetMargin(0,0,0,0);

		if (GetGlobal((std::string("mg_gameresolved_")+itos(_nGameNumber)).c_str(),0)==1)
			_bTaskIsResolved = true;
	}
   ~BoxGameShot() {;}

	void Draw() {
		EBox::Draw();
		if (IsEnabled()) {
			_lpScreenShot->setBlitColor(1,1,1,1);
		} else {
			_lpScreenShot->setBlitColor(0.5,0.5,0.5,1);
		}
		_lpScreenShot->blitAlphaRectFx(0,0,_lpScreenShot->getWidth(),_lpScreenShot->getHeight(),_x+8,_y+5,0,1,_fBlend);
		if (!IsEnabled())
			_lpScreenShotOverlay->blitAlphaRectFx(0,0,_lpScreenShot->getWidth(),_lpScreenShot->getHeight(),_x+8,_y+5,0,1,_fBlend);
		_lpScreenShotBg->blitAlphaRectFx(0,0,_lpScreenShotBg->getWidth(),_lpScreenShotBg->getHeight(),_x,_y,0,1,_fBlend);
	}

	bool IsClicked() {
		if (IsOver() & Mouse.LeftEvent()) {
			Mouse.LeftEventClose();
			return true;
		}
		return false;
	};

	bool IsEnabled() {
		return _bTaskIsResolved;
	}
	void SetEnabled(bool enabled) {
		_bTaskIsResolved = enabled;
	}
	int GetGameNumber() {
		return _nGameNumber;
	}
private:
	int _nGameNumber;
	bool _bTaskIsResolved;

	KGraphic *_lpScreenShot;
	KGraphic *_lpScreenShotBg;
	KGraphic *_lpScreenShotOverlay;
};

SceneMinigames::SceneMinigames(ESceneDirector *lpSceneDirector): EScene(lpSceneDirector) {
	_lpMiniJeu = NULL;
	memset(_cMenuText,0,sizeof(_cMenuText));
	strcpy(_cMenuText,KStr("UI_MENU"));

	_lpFontTitle = EFontBank::getFont("OldNewspaperTypes.ttf",38);
	_lpFontSubTitle = EFontBank::getFont("OldNewspaperTypes.ttf",14);
	_lpFontTitle->setColor(1,1,1,1);
	_lpFontSubTitle->setColor(1,1,1,1);
	_lpFontMenu = EFontBank::getFont("NITECLUB.TTF",32);
	_lpFontMenu->setColor(0, 0, 0, .9f);

	_lpScreenShotBox = new EBox(0,0,0,0);

	_lpTitleBox = new ETextBox(KStr("MOSAIC_TITLE"), _lpFontTitle);
	((ETextBox *)_lpTitleBox)->SetMargin(0,0,0,0);
	((ETextBox *)_lpTitleBox)->SetSizePolicy(EBox::FREE);
	((ETextBox *)_lpTitleBox)->SetWidth(650);
	((ETextBox *)_lpTitleBox)->Move((1024-650)/2,5);

	_lpSubTitleBox = new ETextBox(KStr("MOSAIC_SUB_TITLE"), _lpFontSubTitle,1024);
	((ETextBox *)_lpSubTitleBox)->SetMargin(0,0,0,0);
	((ETextBox *)_lpSubTitleBox)->SetSizePolicy(EBox::FREE);
	((ETextBox *)_lpSubTitleBox)->SetWidth(650);
	((ETextBox *)_lpSubTitleBox)->Move((1024-650)/2,50);

	EBox *tmpBox;
	EBox *tmpGame;
	for (int i=0; i<5; i++) {
		tmpBox = new EBox(0,0,0,0);
		tmpBox->SetLayoutType(EBox::HORIZONTAL);
		for (int j=0; j<5; j++) {
			tmpGame = new BoxGameShot(5*i + j +1);
			tmpBox->AddChild(tmpGame);
         if (((BoxGameShot *)tmpGame)->IsEnabled()) {
            LauchGame(((BoxGameShot *)tmpGame)->GetGameNumber(),true);
         }
		}
		_lpScreenShotBox->AddChild(tmpBox);
	}
	Coord tmp = _lpScreenShotBox->GetSize();
	_lpScreenShotBox->UpdateUI();
	_lpScreenShotBox->SetBoxCenter(1024/2,768/2 + 35);
}
SceneMinigames::~SceneMinigames() {
	XDELETE(_lpScreenShotBox);
	XDELETE(_lpTitleBox);
	XDELETE(_lpSubTitleBox);
   XDELETE(_lpFontTitle);
   XDELETE(_lpFontMenu);
   XDELETE(_lpFontSubTitle);
}
void SceneMinigames::Init()
{
	// Crée 40 zones
	/*int x,y,i;
	i = 1;
	y = 20;
	for (int l=1; l<=5; l++) {
		x = 20;
		for (int c=1; c<=8; c++) {
			AddZone(itos(i).c_str(), x, y, 50, 40, "arrow_walk", true);
			x+=40;
			i++;
		}
		y += 50;
	}*/
}

void SceneMinigames::Check()
{
	EBox *tmp1;
	BoxGameShot *tmp2;
	EScene::Check();

#ifdef SCENE_SHORTCUT
	if (KInput::isPressed(K_VK_F5))
	{
		_lpSceneDirector->GoToScene("menu");
	}

#ifndef BFG_TEASING
	if (KInput::isPressed(K_VK_E))
	{
		_lpScreenShotBox->IterInitialize();
		while ((tmp1 = _lpScreenShotBox->IterNextChild()) != NULL) {
			tmp1->IterInitialize();
			while ((tmp2 =(BoxGameShot *) tmp1->IterNextChild()) != NULL) {
				tmp2->SetEnabled(true);
			}
		}
	}
#endif

#endif

	_lpScreenShotBox->IterInitialize();
	while ((tmp1 = _lpScreenShotBox->IterNextChild()) != NULL) {
		tmp1->IterInitialize();
		while ((tmp2 =(BoxGameShot *) tmp1->IterNextChild()) != NULL) {
			if (tmp2->IsEnabled() && tmp2->IsClicked()) {
				LauchGame(tmp2->GetGameNumber());
				break;
			}
		}
	}
}

void SceneMinigames::Logic()
{
	EScene::Logic();
}

void SceneMinigames::Draw()
{
	EScene::Draw();

	_lpScreenShotBox->SetBlend(getFade());
	_lpTitleBox->SetBlend(getFade());
	_lpSubTitleBox->SetBlend(getFade());

	_lpFontTitle->setColor(1,1,1,1);
	_lpFontSubTitle->setColor(1,1,1,1);
	_lpTitleBox->Draw();
	_lpSubTitleBox->Draw();
	
	_lpScreenShotBox->Draw();

	_lpFontMenu->drawStringCentered(_cMenuText,0,249,30);

	//// Dessine un menu de 40 cases
	//int x,y,i;
	//i = 1;
	//y = 20;
	//for (int l=1; l<=5; l++) {
	//	x = 20;
	//	for (int c=1; c<=8; c++) {
	//		_lpFont->drawStringCentered(itos(i).c_str(), x, x+40, y);
	//		x+=40;
	//		i++;
	//	}
	//	y += 50;
	//}
}

void SceneMinigames::Close()
{
}

bool SceneMinigames::ObjectClicked(const char *szObjectName, float x, float y)
{
	if (strcmp(szObjectName, "menu_zone") == 0) {
		SCENEDIRECTOR->GoToScene("menu");
	}
	return false;
}

bool SceneMinigames::ObjectOver(char *szObjectName, float x, float y)
{
	return false;
}

void SceneMinigames::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MENU);
}

void SceneMinigames::LauchGame(int nGameNumber, bool bPreloadOnly) {
	if (nGameNumber == 1)
	{
		// Jig Saw
      if (bPreloadOnly) {
         EMiniJeuJigsawBook::Preload();
         return;
      }
		_lpMiniJeu = new EMiniJeuJigsawBook();
      _lpSceneDirector->GoToMiniGame("jigsaw book", _lpMiniJeu);
      _lpMiniJeu->SetExitOnFail(false);
      ((EMiniJeuJigsawBook *)_lpMiniJeu)->SetVariation(0);
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
	}
	if (nGameNumber == 2)
	{
      if (bPreloadOnly) {
         EMiniJeuTresor::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuTresor();
		_lpSceneDirector->GoToMiniGame("tresor", _lpMiniJeu);
      _lpMiniJeu->SetExitOnFail(false);
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
	}
	if (nGameNumber == 3)
	{
      if (bPreloadOnly) {
         EMiniJeuSearchgrid::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuSearchgrid();
      _lpSceneDirector->GoToMiniGame("searchgrid", _lpMiniJeu);
      _lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGISLAND);
   }
	if (nGameNumber == 4)
	{
		// Mikado
		if (bPreloadOnly) {
         EMiniJeuMikado::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuMikado();
		_lpSceneDirector->GoToMiniGame("mikado", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGISLAND);
	}
	if (nGameNumber == 5)
	{
		// Shark Simon
		if (bPreloadOnly) {
         EMiniJeuSimonShark::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuSimonShark();
		_lpSceneDirector->GoToMiniGame("Simon Shark", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGISLAND);
	}
	if (nGameNumber == 6)
	{
		if (bPreloadOnly) {
         EMinijeuBonzai::Preload();
         return;
      }
      _lpMiniJeu = new EMinijeuBonzai();
		_lpSceneDirector->GoToMiniGame("EMinijeuBonzai", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
	}
	if (nGameNumber == 7)
	{
		if (bPreloadOnly) {
         EMiniJeuUnscrew::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuUnscrew();
		_lpSceneDirector->GoToMiniGame("EMiniJeuUnscrew", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
	}
	if (nGameNumber == 8)
	{
		if (bPreloadOnly) {
         EMiniJeuStones::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuStones();
		_lpSceneDirector->GoToMiniGame("EMinijeuStone", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGJAPAN);
	}
	if (nGameNumber == 9)
	{
		if (bPreloadOnly) {
         EMiniJeuZenGarden::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuZenGarden();
		_lpSceneDirector->GoToMiniGame("EMiniJeuZenGarden", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGJAPAN);
	}
	if (nGameNumber == 10)
	{
		if (bPreloadOnly) {
         EMiniJeuMasterMind::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuMasterMind();
		_lpSceneDirector->GoToMiniGame("MasterMind", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGJAPAN);
	}
	if (nGameNumber == 11)
	{
		// Jig Saw
      if (bPreloadOnly) {
         EMiniJeuJigsawBook::Preload();
         return;
      }
		_lpMiniJeu = new EMiniJeuJigsawBook();
      _lpSceneDirector->GoToMiniGame("jigsaw book", _lpMiniJeu);
      _lpMiniJeu->SetExitOnFail(false);
      ((EMiniJeuJigsawBook *)_lpMiniJeu)->SetVariation(1);
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
	}
	if (nGameNumber == 12)
	{
		if (bPreloadOnly) {
         EMiniJeuMissingParts::Preload();
         return;
      }
      if ((int)GetRandom(0,2)%2 == 0)
         _lpMiniJeu = new EMiniJeuMissingParts("mg_missingpart_fresque1.png","mg_missingpart_bg_f1.jpg",79,129,"mg_missingpart_artefact_f1.png",492,267);
		else
         _lpMiniJeu = new EMiniJeuMissingParts("mg_missingpart_fresque2.png","mg_missingpart_bg_f2.jpg",79,129,"mg_missingpart_artefact_f2.png",462,265);
		
      _lpSceneDirector->GoToMiniGame("EMiniJeuCuts", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGINCA);
	}
	if (nGameNumber == 13)
	{
		if (bPreloadOnly) {
         EMiniJeuPhotoComp::Preload();
         return;
      }
      if ((int)GetRandom(0,2)%2 == 0)
         _lpMiniJeu = new EMiniJeuPhotoComp("mg_photocomp_fresque1.png","mg_photocomp_bg_f1.jpg",79,129,"mg_photocomp_artefact_f1.png","mg_photocomp_cartouche_f1.png",458,225);
		else
         _lpMiniJeu = new EMiniJeuPhotoComp("mg_photocomp_fresque2.png","mg_photocomp_bg_f2.jpg",79,129,"mg_photocomp_artefact_f2.png","mg_photocomp_cartouche_f2.png",462,235);
		
      _lpSceneDirector->GoToMiniGame("EMiniJeuPhotoComp", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGINCA);
	}
	if (nGameNumber == 14)
	{
		if (bPreloadOnly) {
         EMiniJeuCubes::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuCubes();
		_lpSceneDirector->GoToMiniGame("EMiniJeuCubes", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGINCA);
	}
	if (nGameNumber == 15)
	{
		if (bPreloadOnly) {
         EMiniJeuFlacons::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuFlacons();
		_lpSceneDirector->GoToMiniGame("EMiniJeuFlacons", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
	}	
	if (nGameNumber == 16)
	{
		if (bPreloadOnly) {
         EMiniJeuSymbolMatch::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuSymbolMatch();
		_lpSceneDirector->GoToMiniGame("EMiniJeuSymbolMatch", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGEGYPT);
	}
	if (nGameNumber == 17)
	{
		if (bPreloadOnly) {
         EMiniJeuDice::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuDice();
		_lpSceneDirector->GoToMiniGame("dice", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGEGYPT);
	}
	if (nGameNumber == 18)
	{
		if (bPreloadOnly) {
         EMiniJeuConnect::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuConnect();
		_lpSceneDirector->GoToMiniGame("connect", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGEGYPT);
	}
	if (nGameNumber == 19)
	{
		if (bPreloadOnly) {
         EMiniJeuMirorBeam::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuMirorBeam();
		_lpSceneDirector->GoToMiniGame("EMiniJeuMirorBeam", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
	}
	if (nGameNumber == 20)
	{
		if (bPreloadOnly) {
         EMiniJeuMemoryPlant::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuMemoryPlant();
		((EMiniJeuMemoryPlant *)_lpMiniJeu)->SetGameNumber(20);
		_lpSceneDirector->GoToMiniGame("EMiniJeuMemoryPlant", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGMIDDLEAGE);
	}
	if (nGameNumber == 21)
	{
		if (bPreloadOnly) {
         EMiniJeuPlantAddition::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuPlantAddition();
		_lpSceneDirector->GoToMiniGame("EMiniJeuPlantAddition", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGMIDDLEAGE);
	}
	if (nGameNumber == 22)
	{
		if (bPreloadOnly) {
         EMiniJeuGardening::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuGardening();
		_lpSceneDirector->GoToMiniGame("EMiniJeuGardening", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGMIDDLEAGE);
	}
	if (nGameNumber == 23)
	{
		if (bPreloadOnly) {
         EMiniJeuPentagram::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuPentagram();
		_lpSceneDirector->GoToMiniGame("EMiniJeuPentagram", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
	}
	if (nGameNumber == 24)
	{
		if (bPreloadOnly) {
         EMiniJeuDemelage::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuDemelage();
		_lpSceneDirector->GoToMiniGame("EMiniJeuDemelage", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
	}
	if (nGameNumber == 25)
	{
		if (bPreloadOnly) {
         EMiniJeuMatch3::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuMatch3();
		_lpSceneDirector->GoToMiniGame("EMiniJeuMatch3", _lpMiniJeu);
		_lpMiniJeu->SetExitOnFail(false);
      SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHELL);
	}
	if (nGameNumber == 30)
	{
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
		if (bPreloadOnly) {
         EMiniJeuMemoryPlant::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuMemoryPlant();
		((EMiniJeuMemoryPlant *)_lpMiniJeu)->SetVariation(0);
		((EMiniJeuMemoryPlant *)_lpMiniJeu)->SetGameNumber(30);
		_lpSceneDirector->GoToMiniGame("EMiniJeuMemoryPlant", _lpMiniJeu);
	}
	if (nGameNumber == 31)
	{
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
		if (bPreloadOnly) {
         EMiniJeuMemoryPlant::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuMemoryPlant();
		((EMiniJeuMemoryPlant *)_lpMiniJeu)->SetVariation(1);
		((EMiniJeuMemoryPlant *)_lpMiniJeu)->SetGameNumber(31);
		_lpSceneDirector->GoToMiniGame("EMiniJeuMemoryPlant", _lpMiniJeu);
	}
	if (nGameNumber == 32)
	{
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
		if (bPreloadOnly) {
         EMiniJeuMemoryPlant::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuMemoryPlant();
		((EMiniJeuMemoryPlant *)_lpMiniJeu)->SetVariation(2);
		((EMiniJeuMemoryPlant *)_lpMiniJeu)->SetGameNumber(32);
		_lpSceneDirector->GoToMiniGame("EMiniJeuMemoryPlant", _lpMiniJeu);
	}
	if (nGameNumber == 37)
	{
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
		if (bPreloadOnly) {
         EMiniJeuPoltergeist::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuPoltergeist();
		_lpSceneDirector->GoToMiniGame("EMiniJeuPoltergeist", _lpMiniJeu, "poltergeist");
	}
	if (nGameNumber == 38)
	{
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
		if (bPreloadOnly) {
         EMiniJeuTemplate::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuTemplate();
		_lpSceneDirector->GoToMiniGame("Minijeu Template", _lpMiniJeu);
	}
	if (nGameNumber == 39)
	{
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
		if (bPreloadOnly) {
         EMiniJeuTest::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuTest();
		_lpSceneDirector->GoToMiniGame("Test", _lpMiniJeu);
	}
	if (nGameNumber == 36)
	{
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
		if (bPreloadOnly) {
         EMiniJeuMissingParts::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuMissingParts("mg_missingpart_fresque2.png","mg_missingpart_bg_f2.jpg",79,129,"mg_missingpart_artefact_f2.png",462,265);
		_lpSceneDirector->GoToMiniGame("EMiniJeuCuts", _lpMiniJeu);
	}
	if (nGameNumber == 35)
	{
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
		if (bPreloadOnly) {
         EMiniJeuPhotoComp::Preload();
         return;
      }
      _lpMiniJeu = new EMiniJeuPhotoComp("mg_photocomp_fresque2.png","mg_photocomp_bg_f2.jpg",79,129,"mg_photocomp_artefact_f2.png","mg_photocomp_cartouche_f2.png",462,235);
		_lpSceneDirector->GoToMiniGame("EMiniJeuPhotoComp", _lpMiniJeu);
	}
	if (nGameNumber == 40)
	{
		SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
		//		_lpMiniJeu = new EMiniJeuVideoDisplay();
		//		_lpSceneDirector->GoToMiniGame("EMiniJeuVideoDisplay", _lpMiniJeu);
	}
}