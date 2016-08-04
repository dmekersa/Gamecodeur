#include "assert.h"

#include "MyGame.h"
#include "KGraphic.h"
#include "EPointer.h"
#include "global.h"
#include "ImageBank.h"
#include "SoundBank.h"

#include "EMiniJeuUnscrew.h"

EMiniJeuUnscrew::EMiniJeuUnscrew(void)
{
	SetTextsTrad("UNSCREWMG");
	SetGameType(MINIJEU_DIRECTGAME);
	SetFeatures(MINIJEU_LOGIC);
	SetGameNumber(7);
	SetBackGround(EImageBank::getImage("eunscrew_bg","jpg"));

	// Placement du background
	_lpImgBg = EImageBank::getImage("eunscrew_grid","png");
	_lpImgSideScrew = EImageBank::getImage("eunscrew_sidescrew.png");
	_cPosBg = Coord(241,180);

	// Positionnement des vis sur la grille
	_vPosSideScrew = std::vector< Coord >(4, Coord(0,0));
	_vPosSideScrew[0] = Coord(_cPosBg.x+72, _cPosBg.y +6);
	_vPosSideScrew[1] = Coord(_cPosBg.x +456, _cPosBg.y +6);
	_vPosSideScrew[2] = Coord(_cPosBg.x +14, _cPosBg.y +168);
	_vPosSideScrew[3] = Coord(_cPosBg.x +521, _cPosBg.y +176);
	
	// DÈfinition du set de vis
	_vGame = std::vector < EUnScrewPart * > (4, (EUnScrewPart *) NULL);
	for (int i=0; i<(int)_vGame.size(); i++) {
		_vGame[i] = new EUnScrewPart();
	}

	_lpGridHint = new Grid(2, 2,  _vGame[0]->Width(), _vGame[0]->Width(),3,3);
	_lpGridHint->Move((_nW - _lpGridHint->width())/2,_cPosBg.y +_lpImgBg->getHeight() +20);

	for (int i=0; i<(int)_vGame.size(); i++) {
		_vGame[i]->Move(_lpGridHint->GetCellPos(i%2,i/2));
	}

	//_vGame[0]->SetSuccessor(_vGame[1],2);
	//_vGame[0]->SetSuccessor(_vGame[2],-3);
	//_vGame[0]->SetSuccessor(_vGame[3],1);

	//_vGame[1]->SetSuccessor(_vGame[0],2);
	//_vGame[1]->SetSuccessor(_vGame[2],-1);
	//_vGame[1]->SetSuccessor(_vGame[3],-1);
	//
	//_vGame[2]->SetSuccessor(_vGame[0],-1);
	//_vGame[2]->SetSuccessor(_vGame[1],2);
	//_vGame[2]->SetSuccessor(_vGame[3],-1);

	//_vGame[3]->SetSuccessor(_vGame[0],-2);
	//_vGame[3]->SetSuccessor(_vGame[1],2);
	//_vGame[3]->SetSuccessor(_vGame[2],2);

	// Solution du HINT2
	_vGame[0]->SetSuccessor(_vGame[1],-1);
	_vGame[1]->SetSuccessor(_vGame[2],-1);
	_vGame[2]->SetSuccessor(_vGame[3],-1);

	Reset();
}

EMiniJeuUnscrew::~EMiniJeuUnscrew(void)
{
	XDELETE(_lpGridHint);
   for (int i=0; i<(int)_vGame.size(); i++) {
		XDELETE(_vGame[i]);
	}
}

/** 3 steps rendering **/
void EMiniJeuUnscrew::GameCheck ()
{
	EUnScrewPart *tmp;
	if (! (Mouse.LeftEvent() || Mouse.RightEvent()))
		return;

	// Une rotation es dÈj‡ en cour
	for (int i=0; i<(int)_vGame.size(); i++) {
		if (!_vGame[i]->RotationComplete())
			return;
	}

	for (int i=0; i<(int)_vGame.size(); i++) {
		tmp = _vGame[i];

		float ScrewHeight = (int) (((_lpImgSideScrew->getHeight() -12)/9)*_vGame[i]->GetAbsoluteOrientation());
		Coord cCell = _vPosSideScrew[i];
		cCell.y=cCell.y - ScrewHeight;
		ScrewHeight = ScrewHeight + 12;

		if (Mouse.x >= cCell.x && Mouse.x <= cCell.x + _lpImgSideScrew->getWidth() &&
			Mouse.y >= cCell.y && Mouse.y <= cCell.y + ScrewHeight) {
			if (Mouse.LeftEvent())
				tmp->Screw(-1);
			if(Mouse.RightEvent())
				tmp->Screw(1);
			Mouse.EventClose();
			IsResolved();
	
		}
	}
}

void EMiniJeuUnscrew::GameLogic()
{
}

void EMiniJeuUnscrew::GameDrawSet()
{
	Coord cCell;
	int ScrewHeight;
	
	//_lpGridHint->DrawBaseGrid(_lpImgSideScrew);

	for (int i=0; i <(int)_vGame.size(); i++) {
		// Affichage des tÍte de vis
		//_vGame[i]->Draw();

		// Affichage des vis sur la grille
		ScrewHeight = (int) (((_lpImgSideScrew->getHeight() -12)/9)*_vGame[i]->GetAbsoluteOrientation());
		cCell = _vPosSideScrew[i];
		cCell.y=cCell.y - ScrewHeight;
		ScrewHeight = ScrewHeight + 12;
		_lpImgSideScrew->blitAlphaRect(0,0,_lpImgSideScrew->getWidth(),(float)ScrewHeight,(short)cCell.x,(short)cCell.y);

		//// Affichage des compteurs
		//if (_nHintLevel >= MINIJEU_HINT1)
		//	_vGame[i]->DrawCounter();
		//// Affichage du compteur helper
		//if (_nHintLevel >= MINIJEU_HINT2 && _vGame[i]->IsOver(Coord(Mouse.x-_nX,Mouse.y -_nY)))
		//	_vGame[i]->DrawHelpers();
	}
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuUnscrew::GameIsResolve()
{
	for (int i=0; i <(int)_vGame.size(); i++) {
		if (_vGame[i]->GetAbsoluteOrientation() != 9) {
			return false;
		}
	}
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuUnscrew::GameResolve()
{
	for (int i=0; i<(int)_vGame.size(); i++) {
		_vGame[i]->SetOrientation(9);
	}
	IsResolved();
}

void EMiniJeuUnscrew::GameReset()
{
	for (int i=0; i<(int)_vGame.size(); i++) {
		_vGame[i]->SetOrientation(4);
	}
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuUnscrew::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  ShowHint(MINIJEU_HINT1);
		  Reset(); 
		  break;
	  case MINIJEU_HINT2:
		  ShowHint(MINIJEU_HINT2);
	/*	  for (int i=0; i<(int)_vGame.size(); i++) {
			  _vGame[i]->ResetSuccessor();
		  }
		  _vGame[0]->SetSuccessor(_vGame[1],-1);
		  _vGame[0]->SetSuccessor(_vGame[2],-1);

		  _vGame[1]->SetSuccessor(_vGame[0],1);
		  _vGame[1]->SetSuccessor(_vGame[2],1);
		  
		  _vGame[2]->SetSuccessor(_vGame[1],-1);
		  _vGame[2]->SetSuccessor(_vGame[3],1);
		  
		  _vGame[3]->SetSuccessor(_vGame[2],1);
	*/	  Reset();
		  break;
	  case MINIJEU_HINT3:
		  Reset();
		  break;
	  case MINIJEU_RESOLVED:
		  Resolve();
	  default:
		  break;
	}
}

EUnScrewPart::EUnScrewPart(): EMiniJeuPart(EImageBank::getImage("eunscrew_screwmini.png"),10,0,0,90) {
	SwitchDrag();
	SetRotationDuration(400);

	EGLL();
   _lpFont = EFontBank::getFont("acmesa.ttf",12);
	EGLR();
	_lpFont->setColor(0,0,0,1);
	EGLL();
   _lpFontHint = EFontBank::getFont("acmesa.ttf",12);
	EGLR();
	_lpFontHint->setColor(1,0,0,1);
}

EUnScrewPart::~EUnScrewPart() {
	XDELETE(_lpFont);
	XDELETE(_lpFontHint);
}
bool EUnScrewPart::CouldRotate(int step) {
	int nextStep = GetAbsoluteOrientation() + step;
	if (nextStep <0 || nextStep >= _nSide)
		return false;
	return true;
}
void EUnScrewPart::Screw(int step, bool direct) {
	if (CouldRotate(step)) {
      if (direct && ! ScrewSuccessor(step)) {
         ESoundBank::getSound("mg_forbid")->playSample();
		   return;
      }
      
      ESoundBank::getSound("mg_bruit_de_vis")->playSample();

		if (step > 0)
			while (step > 0) {
				step--;
				RotateACW(false);
			}
		else {
			while (step < 0) {
				step++;
				RotateCW(false);
			}
		}
	} else {
		ESoundBank::getSound("mg_forbid")->playSample();
	}
}

void EUnScrewPart::SetSuccessor(EUnScrewPart *next, int step) {
	ScrewNext tmp = {next,step};
	_lSuccessor.push_back(tmp);
}


bool EUnScrewPart::ScrewSuccessor(int step) {
	std::list< ScrewNext >::iterator it;
	for (it = _lSuccessor.begin();it!=_lSuccessor.end();it++
		) {	if(! (*it).next->CouldRotate(step * (*it).step))
			return false;
	}
	for (it = _lSuccessor.begin();it!=_lSuccessor.end();it++) {
		(*it).next->Screw(step * (*it).step,false);
	}
	return true;
}

void EUnScrewPart::ResetSuccessor() {
	_lSuccessor.clear();
}

void EUnScrewPart::DrawHelpers() {
	std::list< ScrewNext >::iterator it;
	for (it = _lSuccessor.begin();it!=_lSuccessor.end();it++) {
		(*it).next->DrawHelper((*it).step);
	}
}
void EUnScrewPart::DrawCounter() {
	_lpFont->drawStringCentered(itos(_nSide-GetAbsoluteOrientation()).c_str(),(long)_x,(long)(_x + 14),(long)_y);
}
void EUnScrewPart::DrawHelper(int level) {
	_lpFontHint->drawStringFromRight((itos(level)).c_str(),(long)(_x+_w),(long)_y);
}

// Preload images et sons
void EMiniJeuUnscrew::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("eunscrew_bg","jpg",true,true);
   EImageBank::getImage("eunscrew_grid","png",true,true);
   EImageBank::getImage("eunscrew_sidescrew.png","",true,true);
   EImageBank::getImage("eunscrew_screwmini.png","",true,true);

   ESoundBank::getSound("mg_bruit_de_vis",false,true);
}
