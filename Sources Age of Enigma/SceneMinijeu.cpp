/*
*  SceneMiniJeu.cpp
*
*  Created by Rockford
*  Copyright 2010 Casual Games France. All rights reserved.
*
*/

#include "EScene.h"
#include "SceneMiniJeu.h"
#include "ESceneDirector.h"
#include "MyGame.h"

#include "EPointer.h"
#include "EMiniJeuBase.h"
#include "EMiniJeuHintDlg.h"


SceneMiniJeu::SceneMiniJeu(ESceneDirector *lpSceneDirector, EMiniJeuBase *lpMiniJeu) : EScene(lpSceneDirector)
{
	_lpBgGraphic = NULL;
	_lpMiniJeu = NULL;
	_bResolved = false;

	_lpBgGraphic = KPTK::createKGraphic();

	_lpMiniJeu = lpMiniJeu;
	_lpHintDialog = new EMiniJeuHintDlg(_lpMiniJeu);
}

SceneMiniJeu::~SceneMiniJeu()
{
	XDELETE(_lpHintDialog);
	XDELETE(_lpBgGraphic);
	XDELETE(_lpMiniJeu);
}

void SceneMiniJeu::Init()
{
}

void SceneMiniJeu::Check()
{
	if (_State != SCENE_STABLE)
		return;

	CheckTuto();

#ifdef SCENE_SHORTCUT
	if (KInput::isPressed(K_VK_P))
	{
		std::string tmp = "pictures/mg_screenshot_";
		tmp = tmp + itos(_lpMiniJeu->GetGameNumber())+".png";
		SCENEDIRECTOR->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_NONE);
		SCENEDIRECTOR->Draw();
		MyGame::getGame()->getKWindow()->saveBackBuffer(tmp.c_str(),K_IMAGE_PNG,171,128);
		SCENEDIRECTOR->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
	}

   // Retour au menu !
	if (KInput::isPressed(K_VK_F6))
	{
		_lpSceneDirector->GoToScene("menu");
	}
#endif

	if (((EMiniJeuHintDlg *)_lpHintDialog)->IsVisible()) {
		((EMiniJeuHintDlg *)_lpHintDialog)->Check();
	}

	// Impossible d'implementer une gestion d'evenement complete sans toucher a EScene.cpp
	EScene::Check();

	// Traitement des "fils" a effectuer en premier
	_lpMiniJeu->Check();
}

void SceneMiniJeu::Logic()
{
	EScene::Logic();
	_lpMiniJeu->Logic();
	
	if (_lpMiniJeu->End() && _State == SCENE_STABLE) {
		_lpSceneDirector->CloseCurrentMiniGame();
	}
}

void SceneMiniJeu::Draw()
{
	EScene::Draw();
	_lpMiniJeu->Draw();
	_lpHintDialog->Draw();
	// Titre
	/* Fadein/out : affichage d'un rectangle de masquage */
	switch (_State) {
	  case SCENE_SHOWOUT:
		  _lpBgGraphic->drawRect(-1, -1, 1024+1, 768+1, 0, 0, 0, _lpAnimOut->getCurrentValue());
		  break;
	  case SCENE_SHOWIN:
		  _lpBgGraphic->drawRect(-1, -1, 1024+1, 768+1, 0, 0, 0, _lpAnimIn->getCurrentValue());
		  break;
	  default:
		  break;
	}
}

void SceneMiniJeu::Close()
{
}
bool SceneMiniJeu::HasEnded() {
	return _lpMiniJeu->End();
}
bool SceneMiniJeu::isResolved()
{
	return _lpMiniJeu->End() && _lpMiniJeu->GameStatus() == MINIJEU_RESOLVED;
}

bool SceneMiniJeu::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (_lpMiniJeu->IsShowingEnd()) {
      ESoundBank::getSound("mg_forbid")->playSample();
      Mouse.EventClose();
      return false;
   }
   
   if (strcmp(szObjectName, "helpzone") == 0 && _State == SCENE_STABLE)
   {
      ((EMiniJeuHintDlg *)_lpHintDialog)->Show();
   } else if (strcmp(szObjectName, "cancelzone") == 0) {
      if (Mouse.LeftEvent()) {
         ESoundBank::getSound("mg_select_deselect")->playSample();
         _lpSceneDirector->CloseCurrentMiniGame();
      } 
      Mouse.EventClose();
   } else if (strcmp(szObjectName, "resetzone") == 0) {
      ESoundBank::getSound("mg_select_deselect")->playSample();
      _lpMiniJeu->Reset();
      Mouse.EventClose();
   } else if (strcmp(szObjectName, "skipzone") == 0) {
      ESoundBank::getSound("mg_select_deselect")->playSample();
      _lpMiniJeu->ActivateHint(MINIJEU_RESOLVED);
      Mouse.EventClose();
   } else if (strcmp(szObjectName, "hint1zone") == 0) {
      _lpMiniJeu->ShowHint(MINIJEU_HINT1);
      Mouse.EventClose();
   } else if (strcmp(szObjectName, "hint2zone") == 0) {
      _lpMiniJeu->ShowHint(MINIJEU_HINT2);
      Mouse.EventClose();
   } else if (_lpMiniJeu->GameType() == MINIJEU_SUBMITGAME) {
      if (strcmp(szObjectName, "submitzone") == 0) {
         _lpMiniJeu->IsResolved();
      }
      Mouse.EventClose();
   } 
   return false;
}

bool SceneMiniJeu::ObjectOver(char *szObjectName, float x, float y)
{
	return false;
}

void SceneMiniJeu::_myUIMessageHandler (KUIMessage *lpUIMessage) {

}

void SceneMiniJeu::CheckTuto() {
	if (_lpMiniJeu->HasSimpleUI())
		return;

	if (!TaskResolved("minijeu_tuto_help")) {
		SCENEDIRECTOR->getSequencer()->Tutobox(NULL,KStr("MINIJEU_TUTO_HELP"),913,699,-45,0);
		ResolveTask("minijeu_tuto_help");
	}
	if (!TaskResolved("minijeu_tuto_submit")) {
		if (_lpMiniJeu->GameType() == MINIJEU_SUBMITGAME &&  !TaskResolved("minijeu_tuto_submit")) {
//			SCENEDIRECTOR->getSequencer()->Tutobox(NULL,KStr("MINIJEU_TUTO_SUBMIT"),500,649,-30,0);
			ResolveTask("minijeu_tuto_submit");
		}
	}
	if (!TaskResolved("minijeu_tuto_clue")) {
		if (_lpMiniJeu->HintLevel() > MINIJEU_NORMAL && _lpMiniJeu->HintLevel() < MINIJEU_RESOLVED && strlen(_lpMiniJeu->HintText(MINIJEU_HINT1)) > 0) {
//			SCENEDIRECTOR->getSequencer()->Tutobox(NULL,KStr("MINIJEU_TUTO_CLUE"),875,650,-45,0);
			ResolveTask("minijeu_tuto_clue");
		}
	}
}