#include "assert.h"

#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"

#include "EMinijeuZenGarden.h"

EMiniJeuZenGarden::EMiniJeuZenGarden(void)
{
	SetTextsTrad("ZENGARDEN");
	SetGameType(MINIJEU_SUBMITGAME);
	SetFeatures(MINIJEU_OBSERVATION);
	SetBackGround(EImageBank::getImage("ezengarden_bg","jpg"));
	SetGameNumber(9);

	_vProposal.push_back(new EZenGardenPart(
		EImageBank::getImage("ezengarden_propositions_1.png"),81,91));
	_vProposal.push_back(new EZenGardenPart(
		EImageBank::getImage("ezengarden_propositions_2.png"),278,90));
	_vProposal.push_back(new EZenGardenPart(
		EImageBank::getImage("ezengarden_propositions_3.png"),477,76));
	_vProposal.push_back(new EZenGardenPart(
		EImageBank::getImage("ezengarden_propositions_4.png"),305,256));
	_vProposal.push_back(new EZenGardenPart(
		EImageBank::getImage("ezengarden_propositions_5.png"),509,254));
	_vProposal.push_back(new EZenGardenPart(
		EImageBank::getImage("ezengarden_propositions_6.png"),722,243));

	_nIndSol = 2;
	Reset();
}

EMiniJeuZenGarden::~EMiniJeuZenGarden(void)
{
	for (int i=0; i<(int)_vProposal.size(); i++) {
		XDELETE(_vProposal[i]);
	}
}

/** 3 steps rendering **/
void EMiniJeuZenGarden::GameCheck ()
{

	if (!Mouse.LeftEvent())
		return;

	bool bIsOver = false;
	for (int i=0; i<(int)_vProposal.size(); i++) {
		if (_vProposal[i]->IsMouseOver())
			bIsOver = true;
	}
	if (!bIsOver)
		return;

	for (int i=0; i < (int)_vProposal.size(); i++) {
		if (_vProposal[i]->IsDisabled()) {
			if (_vProposal[i]->IsMouseOver())
				ESoundBank::getSound("mg_forbid")->playSample();
			continue;
		}
		if (!_vProposal[i]->IsMouseOver()) {
			if ( _vProposal[i]->IsSelected())
				_vProposal[i]->Deselect();
		} else {
			ESoundBank::getSound("mg_select_deselect")->playSample();
		}
		_vProposal[i]->Check();
	}
}

void EMiniJeuZenGarden::GameLogic ()
{
}

void EMiniJeuZenGarden::GameDrawSet()
{
	for (int i=0; i<(int)_vProposal.size();i++) {
		_vProposal[i]->Draw();
	}
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuZenGarden::GameIsResolve()
{
	Coord pos;
	bool res = false;

	if (_vProposal[_nIndSol]->IsSelected()) 
		res = true;
	
	return res;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuZenGarden::GameResolve() {
	for(int i=0; i< (int)_vProposal.size(); i++) {
		if (i == _nIndSol)
			continue;
		_vProposal[i]->Deselect();
	}
	_vProposal[_nIndSol]->Select();

	IsResolved();
}

void EMiniJeuZenGarden::GameReset()
{
	for (int i=0; i<(int)_vProposal.size();i++) {
		_vProposal[i]->Deselect();
	}
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuZenGarden::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
  		  ShowHint(MINIJEU_HINT1);
		  _vProposal[4]->Disable();
		  Reset();
		  break;
	  case MINIJEU_HINT2:
  		  ShowHint(MINIJEU_HINT2);
		  _vProposal[1]->Disable();
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

int EZenGardenPart::_nX = 0;
int EZenGardenPart::_nY = 0;
float EZenGardenPart::_fTimeout = 500;

EZenGardenPart::EZenGardenPart(KGraphic *img, int x, int y): EMiniJeuPart(img,1,0,0,0) {
	_lpOverlaySelected = EImageBank::getImage("minijeu_selection.png");

	_bIsSelected = false;
	_bIsDisabled = false;

	_lpAlphaSelect = NULL;
	_lpAlphaDisable = NULL;

	_fAlphaSelect = 0;
	_fAlphaDisable = 0;

	Move((float)_nX+x,(float)_nY+y);
	SwitchDrag();
	SwitchRotation();

}
EZenGardenPart::~EZenGardenPart(void) {
	XDELETE(_lpAlphaDisable);
	XDELETE(_lpAlphaSelect);
}
void EZenGardenPart::Check() {
	if (_bIsDisabled)
		return;
	if (IsMouseOver() && Mouse.LeftEvent()) {
		if(_bIsSelected) {
			Deselect();
		} else {
			Select();
		}
		Mouse.LeftEventClose();
	}
}
void EZenGardenPart::Draw() {
	EMiniJeuPart::Draw();
	double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();

	if (_lpAlphaDisable != NULL) {
		_lpAlphaDisable->move(fElapsed);
		_fAlphaDisable = _lpAlphaDisable->getCurrentValue();
		if (_lpAlphaDisable->isCompleted())
			XDELETE(_lpAlphaDisable);
	}
	if (_fAlphaDisable > 0) {
		_lpImg->setBlitColor(0.5,0.5,0.5,1);
		_lpImg->blitAlphaRectFx(0,0,_w,_h,(short)_x,(short)_y,0,1,_fAlphaDisable);
		_lpImg->setBlitColor(1,1,1,1);
	}

	if (_lpAlphaSelect != NULL) {
		_lpAlphaSelect->move(fElapsed);
		_fAlphaSelect = _lpAlphaSelect->getCurrentValue();
		if (_lpAlphaSelect->isCompleted())
			XDELETE(_lpAlphaSelect);
	}
	if (_fAlphaSelect > 0) {
		_lpOverlaySelected->blitAlphaRectFx(0,0,_lpOverlaySelected->getWidth(),_lpOverlaySelected->getHeight(),
			(short) (_x + (_w/2) - (_lpOverlaySelected->getWidth()/2)),
			(short) (_y + (_h/2) - (_lpOverlaySelected->getHeight()/2)),
			0,1,_fAlphaSelect);
	}
}
void EZenGardenPart::Disable(bool Anim) {
	if (IsDisabled())
		return;

	_bIsDisabled = true;
	if (Anim) {
		XDELETE(_lpAlphaDisable);
		_lpAlphaDisable = new KCounter();
		_lpAlphaDisable->startCounter(_fAlphaDisable,1,0,_fTimeout,K_COUNTER_LINEAR);
	} else {
		_fAlphaDisable = 1;
	}
	Deselect(Anim);
}
void EZenGardenPart::Deselect(bool Anim){
	if (!IsSelected())
		return;

	_bIsSelected = false;
	if (Anim) {
		XDELETE(_lpAlphaSelect);
		_lpAlphaSelect = new KCounter();
		_lpAlphaSelect->startCounter(_fAlphaSelect,0,0,_fTimeout,K_COUNTER_LINEAR);
	} else {
		_fAlphaSelect = 0;
	}
}
void EZenGardenPart::Select(bool Anim){
	if (IsSelected())
		return;
	
	_bIsSelected = true;
	if (Anim) {
		XDELETE(_lpAlphaSelect);
		_lpAlphaSelect = new KCounter();
		_lpAlphaSelect->startCounter(_fAlphaSelect,1,0,_fTimeout,K_COUNTER_LINEAR);
	} else {
		_fAlphaSelect = 1;
	}
}
void EZenGardenPart::SetRefPos(Coord pos) {
	_nX = (int)pos.x;
	_nY = (int)pos.y;
}

bool EZenGardenPart::IsDisabled() {
	return _bIsDisabled;
}
bool EZenGardenPart::IsSelected() {
	return _bIsSelected;
}

// Preload images et sons
void EMiniJeuZenGarden::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("ezengarden_bg","jpg",true,true);
   EImageBank::getImage("ezengarden_propositions_1.png","",true,true);
   EImageBank::getImage("ezengarden_propositions_2.png","",true,true);
   EImageBank::getImage("ezengarden_propositions_3.png","",true,true);
   EImageBank::getImage("ezengarden_propositions_4.png","",true,true);
   EImageBank::getImage("ezengarden_propositions_5.png","",true,true);
   EImageBank::getImage("ezengarden_propositions_6.png","",true,true);
   EImageBank::getImage("minijeu_selection.png","",true,true);
}
