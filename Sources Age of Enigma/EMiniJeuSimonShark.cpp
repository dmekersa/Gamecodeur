#include "assert.h"
#include <list>

#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "global.h"
#include "KRandom.h"
#include "ESceneDirector.h"
#include "EGeometry.h"

#include "EMiniJeuSimonShark.h"

using EMiniGame::GetRandom;

EMiniJeuSimonShark::EMiniJeuSimonShark(void)
{
	SetTextsTrad("SIMONSHARKMG");
	SetGameType(MINIJEU_DIRECTGAME);
	SetFeatures(MINIJEU_MEMORY);
	SetGameNumber(5);

	_bHold = true;

	_nEndStep = 9;
	_nCol = 5;
	_nLine = 3;

	_lpFont = EFontBank::getFont("tahoma.ttf",40);
	_lpFont->setColor(1,1,1,1);
	
	EGLL();
   _lpFontPath = EFontBank::getFont("acmesa.ttf",40);
	EGLR();
	_lpFontPath->setColor(0,0,0,0.4f);


	memset(_cTextStart,0,sizeof(_cTextStart));
	strcat(_cTextStart,KStr("PLANTMEMORY_START"));
	_lpImgStart = new EMiniJeuPart(EImageBank::getImage("mg_memorygrid_start_bouton","png"));
	_lpPirate = new EMiniJeuPart( EImageBank::getImage("mg_simon_pirate","png"));
	_lpPirate->Hide();

	_lpMaille = EImageBank::getImage("mg_simon_cross_inter","png");
	_lpFanion = EImageBank::getImage("esimonshark_fanion","png");
	_lpPiratPath = EImageBank::getImage("mg_simon_path","png");
	_lpRedCross = EImageBank::getImage("mg_simon_cross_final","png");
	
	_lpSeqTimer = NULL;

	_lpImgBg = EImageBank::getImage("mg_simon_bg","jpg");
	
	_fStartDelay = 1500;
	_fStepDelayDelta = 100;
	_fDelay = 1500;

	_nDrawStep = -1;
	_nSequenceStep = -1;
	_nDrawStep = -1;

	_vMaillesPos.push_back(Coord(218,93));
	_vMaillesPos.push_back(Coord(700,92));
	_vMaillesPos.push_back(Coord(909,196));
	_vMaillesPos.push_back(Coord(321,72));
	_vMaillesPos.push_back(Coord(664,202));
	_vMaillesPos.push_back(Coord(446,306));
	_vMaillesPos.push_back(Coord(172,360));
	_vMaillesPos.push_back(Coord(627,400));
	_vMaillesPos.push_back(Coord(213,453));
	_vMaillesPos.push_back(Coord(705,587));
	_vMaillesPos.push_back(Coord(171,274));
	_vMaillesPos.push_back(Coord(915,424));

	Reset();
	_lpImgStart->Move((_nW - _lpImgStart->Width()) /2, _nH - 150 - _lpImgStart->Height());
	_lpImgStart->Show();
}

EMiniJeuSimonShark::~EMiniJeuSimonShark(void)
{
	for(int i = 0; i< (int)_vMailles.size(); i++)
		XDELETE(_vMailles[i]);

	XDELETE(_lpImgStart);
	XDELETE(_lpPirate);
   XDELETE(_lpFontPath);
   XDELETE(_lpSeqTimer);
   XDELETE(_lpFont);
   XDELETE(_lpFontPath);
}

/** 3 steps rendering **/
void EMiniJeuSimonShark::GameCheck ()
{
	EMiniJeuPart *cell = NULL;

	if (_lpImgStart->IsVisible()) {
		if (_lpImgStart->IsMouseOver() && _lpImgStart->TransformComplete()) {
			Mouse.LeftEventClose();
			_lpImgStart->FadOut(200);
         InitSimonSequence(400);
		}
		return;
	}
	
	if (_bPlaySequence)
		return;

	if (Mouse.LeftEvent()) {
		for (int i=0; i<_vMailles.size(); i++) {
			if (_vMailles[i]->IsMouseOver()) {
				cell = _vMailles[i];
				break;
			}
		}
		Mouse.LeftEventClose();

		if (cell == NULL) {
			ESoundBank::getSound("mg_forbid")->playSample();
			return;
		}

		ESoundBank::getSound("mg_put_flag")->playSample();
		if ( cell == _vPath[_nDrawStep+1]) {
			_nDrawStep++;
			if (_nStep == _nDrawStep){
				_nStep++;
				if (IsResolved())
					return;
				InitSimonSequence(1500);
			}
		} /*else if (cell == *(_vPath[(int)cell.x])) {
		  IsResolved();
		  } */else {
			  SetGameFailed();
		}
	}
}

void EMiniJeuSimonShark::GameLogic ()
{
	if (_lpImgStart->IsVisible())
		return;

	LogicSimonSequence();
}

void EMiniJeuSimonShark::GameDrawSet ()
{
	Coord cell;
	_lpImgBg->blitAlphaRectF(0,0,_lpImgBg->getWidth(),_lpImgBg->getHeight(),0,0);

	for (int i=0; i<_vMailles.size(); i++) {
		_vMailles[i]->Draw();
	}

	_lpImgStart->Draw();
	if (_lpImgStart->IsVisible()) {
		cell = _lpImgStart->Pos();
		cell.y = cell.y + (100 - 46)/2;
		_lpImgStart->Draw();
		_lpFont->setColor(1,1,1,_lpImgStart->GetAlpha());
		_lpFont->drawStringCentered(_cTextStart,cell.x +44,cell.x + _lpImgStart->Width() -30,cell.y);
		return;
	}

	_lpFontPath->drawStringFromRight(itos(_nStep).c_str(),60,60);
	_lpFontPath->drawStringFromLeft((std::string("/")+itos(_nEndStep)).c_str(),60,60);

	Coord pcell,pathc;
	float pathangle,distance;
	if (_nDrawStep >= 0) {
		for (int i=0; i<= _nDrawStep; i++) {
			cell = _vPath[i]->Center();
			if (i>0) {
				pathangle = EGeometry::Angle(pcell,cell);
				distance = EGeometry::Distance(pcell,cell);
				if (i == _nDrawStep && _lpPirate->IsVisible()) {
					pathangle = EGeometry::Angle(pcell,_lpPirate->Pos() + Coord(_lpPirate->Width()/2,_lpPirate->Height()));
					distance = EGeometry::Distance(pcell,_lpPirate->Pos() + Coord(_lpPirate->Width()/2,_lpPirate->Height()));
				}
				pathc = EGeometry::GetSegmentEnd(pcell,distance/2,pathangle);
				pathc.y -= _lpPiratPath->getHeight()/2;
				pathc.x -= distance/2;
				_lpPiratPath->blitAlphaRectFx(0,0,distance,_lpPiratPath->getHeight(),pathc.x,pathc.y,pathangle,1,0.3f);
			}
			pcell = cell;
			cell = cell - Coord(_lpFanion->getWidth()/2,_lpFanion->getHeight()/2);
			if (_bPlaySequence) {
				if  ((i == _nDrawStep) && ( _nDrawStep == _nSequenceStep)) {
					continue;
				}
			}
			if ( _nHintLevel >= MINIJEU_PREAMBUL || _nDrawStep != _nSequenceStep  )
				_lpFanion->blitAlphaRect(0,0,_lpFanion->getWidth(),_lpFanion->getHeight(),cell.x,cell.y);
		}
	}

	_lpPirate->Draw();

	if (_lpSeqTimer != NULL) {
		double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
		_lpSeqTimer->move(fElapsed);
	}
}

// Retourne true si le mini jeu est rŽsolu
bool EMiniJeuSimonShark::GameIsResolve()
{
	bool res = false;

	if (_nStep == _nEndStep)
		res = true;

	return res;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuSimonShark::GameResolve()
{
	_nStep = _nEndStep;
}

void EMiniJeuSimonShark::GameReset()
{
	_lpImgStart->FadIn(200);

	XDELETE(_lpSeqTimer);
	for(int i = 0; i<(int)_vMailles.size(); i++)
		XDELETE(_vMailles[i]);
	_vMailles = std::vector< EMiniJeuPart * > (_vMaillesPos.size(),(EMiniJeuPart *) NULL);
	
	EMiniJeuPart *tmpMaille;
	for (int i=0; i<_vMailles.size(); i++) {
		tmpMaille = new EMiniJeuPart(_lpMaille);
		//	tmpMaille = new EMiniJeuPart(_lpRedCross);
		tmpMaille->SetMaxBlend(0.6f);
		tmpMaille->MoveCenterTo(_vMaillesPos[i]);
		_vMailles[i] = tmpMaille;
	}
	
	_vPath = std::vector< EMiniJeuPart * > (_nEndStep,(EMiniJeuPart *) NULL);
	RandomSequence();

	_nStep = 1;
	_nDrawStep = -1;
	_nSequenceStep = -1;
	
	if (_nGameStatus < MINIJEU_NORMAL)
		return;
//	InitSimonSequence(_fStartDelay);
	_fStartDelay = 700;

}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuSimonShark::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
  		  ShowHint(MINIJEU_HINT1);
		  _nEndStep = _nEndStep - 2;
		  Reset();
		  break;
	  case MINIJEU_HINT2:
		  ShowHint(MINIJEU_HINT2);
		  _nEndStep = _nEndStep - 2;
		  Reset();
		  break;
	  case MINIJEU_RESOLVED:
		  Resolve();
		  break;
	  default:
		  KAssert(false);
		  break;
	}
}
void EMiniJeuSimonShark::InitSimonSequence(float fStartDelay) {
	_nSequenceStep = -1;
	_bPlaySequence = true;

   XDELETE(_lpSeqTimer);
	if (fStartDelay != 0) {
		_lpSeqTimer = new KCounter();
		_lpSeqTimer->startCounter(0,1,0,fStartDelay,K_COUNTER_LINEAR);
	}
}
void EMiniJeuSimonShark::LogicSimonSequence() {
	if (! _bPlaySequence)
		return;

	Coord cell;
	float delay;

	if (_lpSeqTimer == NULL || (_lpSeqTimer->isCompleted() && _lpPirate->MoveComplete())) {	
		XDELETE(_lpSeqTimer);
		if (_nDrawStep != _nSequenceStep) 
			_nSequenceStep = -2;
		if (_nSequenceStep == _nStep) {
			ESoundBank::getSound("mg_dig")->playSample();
			_lpPirate->FadOut(500);
			_bPlaySequence = false;
			_nSequenceStep = -1;
			_nDrawStep = -1;
			return;
		}

		_nSequenceStep++;
		_nDrawStep = _nSequenceStep;

		delay = _fDelay - (_nStep * _fStepDelayDelta);

		_lpSeqTimer = new KCounter();
		_lpSeqTimer->startCounter(0,1,0,delay,K_COUNTER_LINEAR);

		if (_nSequenceStep < 0)
			return;

		cell = _vPath[_nSequenceStep]->Center();
		cell.x = cell.x - (_lpPirate->Width()/2);
		cell.y = cell.y - _lpPirate->Height();

		if (_nSequenceStep == 0) {
			_lpPirate->Move(cell.x,cell.y);
			_lpPirate->FadIn();
			ESoundBank::getSound("mg_dig")->playSample();
		} else {
			ESoundBank::getSound("mg_put_flag")->playSample();
			_lpPirate->Move(cell.x,cell.y,500,false,K_COUNTER_LINEAR);
		} 
	}
}
void EMiniJeuSimonShark::RandomSequence() {
	
	EMiniJeuPart *cell;
	for (int i=0; i < _vPath.size(); i++) {
ref:	int cellref = GetRandom(_vMailles.size());
		cell = _vMailles[cellref];
		for (int j=0; j < _vPath.size(); j++) {
			if (_vPath[j] == NULL)
				break;
			if (_vPath[j] == cell)
				goto ref;
		}

		if (i == _vPath.size()-1) {
			_vMailles[cellref] = new EMiniJeuPart(_lpRedCross);
			_vMailles[cellref]->MoveCenterTo(cell->Center());
			_vMailles[cellref]->SetMaxBlend(0.6f);
			XDELETE(cell);
			cell = _vMailles[cellref];
		}
		_vPath[i]=cell;
	}
}

//void EMiniJeuSimonShark::GameDrawPreambul() {
//	if (_bHold && !  SCENEDIRECTOR->isDialogboxActive()) {
//		SCENEDIRECTOR->OpenDialogbox(KStr("MINIGAME_HOLDSTART"));
//		_bHold = false;
//	}
//	EImageBank::getImage("minijeu_success","png")->drawRect(0,0,1024,768,0,0,0,0.3f);
//
//}
//void EMiniJeuSimonShark::GameCheckPreambul() {
//	if (!_bHold && !SCENEDIRECTOR->isDialogboxActive()) {
//		SCENEDIRECTOR->CloseDialogbox();
//		_nGameStatus = MINIJEU_NORMAL;
//		Reset();
//	}
//	Mouse.EventClose();
//}

// Preload images et sons
void EMiniJeuSimonShark::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_memorygrid_start_bouton","png",true,true);
   EImageBank::getImage("mg_simon_pirate","png",true,true);
   EImageBank::getImage("mg_simon_cross_inter","png",true,true);
   EImageBank::getImage("esimonshark_fanion","png",true,true);
   EImageBank::getImage("mg_simon_path","png",true,true);
   EImageBank::getImage("mg_simon_cross_final","png",true,true);
   EImageBank::getImage("mg_simon_bg","jpg",true,true);
   EImageBank::getImage("minijeu_success","png",true,true);

   ESoundBank::getSound("mg_put_flag",false,true);
   ESoundBank::getSound("mg_dig",false,true);
}
