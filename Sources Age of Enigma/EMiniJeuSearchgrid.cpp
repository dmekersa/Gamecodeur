#include "assert.h"

#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"

#include "EPointer.h"
#include "global.h"
#include "KRandom.h"

#include "EMiniJeuSearchgrid.h"

EMiniJeuSearchgrid::EMiniJeuSearchgrid(void)
{
	SetTextsTrad("DIGITMG");
	SetGameType(MINIJEU_DIRECTGAME);
	SetFeatures(MINIJEU_LOGIC);
	SetGameNumber(3);
	SetBackGround(EImageBank::getImage("mg_searchgrid_bg","jpg"));

	_nGameSize = 9;
	_nDefaultTry = 6;

	_nFontSize = 30;
    _nFontHintDistanceSize = 15;

	EGLL();
   _lpFont = EFontBank::getFont("acmesa.ttf",_nFontSize);
	EGLR();
	_lpFont->setColor(0,0,0,1);

   EGLL();
	_lpFontHintDistance = EFontBank::getFont("acmesa.ttf",_nFontHintDistanceSize);
	EGLR();
	_lpFontHintDistance->setColor(0,0,0,1);

	_lpImgDigged1 = EImageBank::getImage("ESearchgrid_digged1.png");
	_lpImgDigged2 = EImageBank::getImage("ESearchgrid_digged2.png");
	_lpImgPearl = EImageBank::getImage("ESearchgrid_pearl.png");
   _lpImgPearlWrong = EImageBank::getImage("ESearchgrid_pearl_wrong.png");
	_lpImgBg = EImageBank::getImage("ESearchgrid_grid.png");
	_lpImgCartouche = EImageBank::getImage("ESearchgrid_cartouche.png");

	_vCurrentGame = std::vector < std::vector < int > > (_nGameSize, std::vector < int > (_nGameSize,0));
	_lpGrid = new Grid(_nGameSize,_nGameSize,_lpImgDigged1->getWidth(),_lpImgDigged1->getHeight(),4,2);
	_lpGrid->Move((_nW-_lpGrid->width())/2,((_nH-_lpGrid->height())-10)/2);
	_lpGrid->Move(250,32);

	_lpCounter = NULL;
	Reset();
}

EMiniJeuSearchgrid::~EMiniJeuSearchgrid(void)
{
	XDELETE(_lpGrid);
	XDELETE(_lpFont);
	XDELETE(_lpFontHintDistance);
	XDELETE(_lpCounter);
}

/** 3 steps rendering **/
void EMiniJeuSearchgrid::GameCheck ()
{
	Coord cell;

	if (Mouse.LeftEvent()) {
		if (_lpGrid->IsMouseOver()) {
			cell = _lpGrid->GetSelection(Mouse);
			if (cell.x < 0) {
				return;
			} else if (_vCurrentGame[(int)cell.y][(int)cell.x] < 0) {
				return;
			}
			 
			Mouse.LeftEventClose();
			_nTry--;
			_sTry = itos(_nTry);

			if (cell == PearlPos) {
				_vCurrentGame[(int)PearlPos.y][(int)PearlPos.x] = 1;
				IsResolved();
				ESoundBank::getSound("mg_dig")->playSample();
				ESoundBank::getSound("mg_good")->playSample();
				return;
			}
			if (_vCurrentGame[(int)cell.y][(int)cell.x] > 0) {
				_sDistance = itos(_vCurrentGame[(int)cell.y][(int)cell.x]);
				_vCurrentGame[(int)cell.y][(int)cell.x] = -_vCurrentGame[(int)cell.y][(int)cell.x];
				ESoundBank::getSound("mg_dig")->playSample();
			}
			if (_nTry == 0) {
				SetGameFailed();
            _fTimeFromFail = 0;
				_lpCounter = new KCounter();
				_lpCounter->startCounter(0,1,0,500,K_COUNTER_LINEAR);
				_vCurrentGame[(int)PearlPos.y][(int)PearlPos.x]=1;
				return;
			}
		}
	}
}

void EMiniJeuSearchgrid::GameLogic ()
{
}

void EMiniJeuSearchgrid::GameDrawSet ()
{
	Coord pos;
	float _fAlphaPearl = 0;

	_lpFont->drawStringFromRight(_sTry.c_str(), _lpGrid->_xPos + _lpGrid->width()/2 -10,_lpGrid->_yPos + _lpGrid->height() + 45);
	_lpFont->drawStringFromLeft(("/"+itos(_nDefaultTry)).c_str(), _lpGrid->_xPos + _lpGrid->width()/2 - 10,_lpGrid->_yPos + _lpGrid->height() + 45);

	if (_lpCounter != NULL) { 
		if (!_lpCounter->isCompleted())
			_lpCounter->move(MyGame::getGame()->getKWindow()->getFrameTime());
		_fAlphaPearl = _lpCounter->getCurrentValue();

   }

	for(int i=0; i<_nGameSize; i++) {
		for (int j=0; j<_nGameSize; j++) {
         if (PearlPos.x == j && PearlPos.y == i && _vCurrentGame[i][j] == 1) {
				_lpGrid->FillCell(_lpImgPearl,j,i,false,_fAlphaPearl);
            if (_fTimeFromFail >=0) {
               _fTimeFromFail += MyGame::getGame()->getKWindow()->getFrameTime();
               float _fBlitColor = sinf(_fTimeFromFail*0.0025*3.14116);      
               _lpGrid->FillCell(_lpImgPearlWrong,j,i,false,_fBlitColor*_fAlphaPearl);
            }
         }
			if (_vCurrentGame[i][j] < 0) {
				pos = _lpGrid->GetCellPos(j,i);
				if ( (i % 2) + (j % 2) == 0)
					_lpGrid->FillCell(_lpImgDigged1,j,i,false);
				else 
					_lpGrid->FillCell(_lpImgDigged2,j,i,false);
				_lpFontHintDistance->drawStringFromRight(itos(-(_vCurrentGame[i][j])).c_str(),
					(long) (pos.x-2+_lpImgDigged1->getWidth()),
					(long)pos.y);
			}
		}
	}
}

// Retourne true si le mini jeu est rÈsolu
bool EMiniJeuSearchgrid::GameIsResolve()
{
	if (_vCurrentGame[(int)PearlPos.y][(int)PearlPos.x] == 1) {
		_lpCounter = new KCounter();
		_lpCounter->startCounter(0,1,0,GetHoldEndTimeout(),K_COUNTER_EASEOUT);
		return true;
	}

	return false;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuSearchgrid::GameResolve()
{
	_vCurrentGame[(int)PearlPos.y][(int)PearlPos.x] = 1;
}

void EMiniJeuSearchgrid::GameReset()
{
	Coord Cell;
	PearlPos = GetRandomPos();
	_nTry = _nDefaultTry;
	_sTry = itos(_nTry);
	_sDistance = "";

	for(int i=0; i<_nGameSize; i++) {
		for (int j=0; j<_nGameSize; j++) {
			_vCurrentGame[i][j] = 0;
		}
	}

	for(int i=1; i<=_nGameSize; i++) {
		for (int j = (int)PearlPos.y - i; j <= (int)PearlPos.y + i; j++) {
			for (int k = (int)PearlPos.x -i ; k <= (int)PearlPos.x +i; k++) {
				if (j < 0 || k < 0 || j >= _nGameSize || k >= _nGameSize || (
					_vCurrentGame[j][k] != 0))
					continue;
				_vCurrentGame[j][k]=i;
			}
		}
	}

	_vCurrentGame[(int)PearlPos.y][(int)PearlPos.x] = 0;

   XDELETE(_lpCounter);
   _fTimeFromFail = -1;
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuSearchgrid::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
  		  ShowHint(MINIJEU_HINT1);
		  _nDefaultTry = _nDefaultTry +2;
		  Reset();
		  break;
	  case MINIJEU_HINT2:
		  ShowHint(MINIJEU_HINT2);
		  _nDefaultTry = _nDefaultTry +2;
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

Coord EMiniJeuSearchgrid::GetRandomPos() {
	return Coord((int)(_nGameSize*KRandom::getRandomFloat())%_nGameSize,
		(int)(_nGameSize*KRandom::getRandomFloat())%_nGameSize);
}

// Preload images et sons
void EMiniJeuSearchgrid::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_searchgrid_bg","jpg",true,true);
   EImageBank::getImage("ESearchgrid_digged1.png","",true,true);
   EImageBank::getImage("ESearchgrid_digged2.png","",true,true);
   EImageBank::getImage("ESearchgrid_pearl.png","",true,true);
   EImageBank::getImage("ESearchgrid_grid.png","",true,true);
   EImageBank::getImage("ESearchgrid_cartouche.png","",true,true);
   EImageBank::getImage("ESearchgrid_pearl_wrong.png","",true,true);

   ESoundBank::getSound("mg_dig",false,true);
   ESoundBank::getSound("mg_good",false,true);
}
