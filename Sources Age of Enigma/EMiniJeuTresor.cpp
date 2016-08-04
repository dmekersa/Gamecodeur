#include "assert.h"

#include "MyGame.h"
#include "KGraphic.h"
#include "EPointer.h"
#include "global.h"

#include "SoundBank.h"
#include "ImageBank.h"

#include "EMiniJeuTresor.h"

EMiniJeuTresor::EMiniJeuTresor(void)
{
	SetTextsTrad("TREASUREMG");
	SetGameType(MINIJEU_DIRECTGAME);
	SetFeatures(MINIJEU_LOGIC);
	SetGameNumber(2);
	SetBackGround(EImageBank::getImage("ETresorBG","jpg"));

	memset(_cEnigme,0,sizeof(_cEnigme));
	strcpy(_cEnigme,KStr("ENIGME_TREASUREMG"));

   _nFontSize = 60;
   EGLL();
   _lpFont = EFontBank::getFont("acmesa.ttf",_nFontSize);
   EGLR();
   
   EGLL();
   _lpFontEnigme = EFontBank::getFont("renaissance.ttf",32);
   EGLR();
   _lpFontEnigme->setColor(0,0,0,0.8f);
   _lpFontEnigme->setYSpacing(-4);

   //EGLL();
   //_lpFontEnigme = bEFontBank::getFont(KMiscTools::makeFilePath ("BickhamScriptPro-Regular.otf"));
   //EGLR();
   //_lpFontEnigme->setHeightPix (44);
   //_lpFontEnigme->setColor(0,0,0,0.8f);
   //_lpFontEnigme->setYSpacing(-28);

   //_lpFont->setColor(0,0,0,1);
   _cDim.x = 100;
   _cDim.y = 100;


	_vSolution.empty();
	_vSolution.push_back(8);
	_vSolution.push_back(4);
	_vSolution.push_back(6);
	_vSolution.push_back(2);

	_nGameSize = _vSolution.size();

	_vCurrentGame = std::vector < int > (_nGameSize, 0);
	_lpGrid = new Grid(_nGameSize, 1, _cDim.x, _cDim.y,0,78);
	_lpGrid->Move(276,143);
	
	Reset();
}

EMiniJeuTresor::~EMiniJeuTresor(void)
{
	XDELETE(_lpGrid);
	XDELETE(_lpFont);
	XDELETE(_lpFontEnigme);
}

/** 3 steps rendering **/
void EMiniJeuTresor::GameCheck ()
{
	Coord cell;

	if (Mouse.LeftEvent()) {
		if (_lpGrid->IsMouseOver()) {
			cell = _lpGrid->GetSelection(Mouse);
			if (cell.x < 0)
				return

			Mouse.LeftEventClose();
			ESoundBank::getSound("mg_digit")->playSample();
			_vCurrentGame[(int)cell.x]++;
			_vCurrentGame[(int)cell.x] =  _vCurrentGame[(int)cell.x] % 9;
			IsResolved();
		}
	}
}

void EMiniJeuTresor::GameLogic ()
{
}

void EMiniJeuTresor::GameDrawSet ()
{
	Coord cCell;

	_lpFontEnigme->setColor(1,0,0,0.3f);
	_lpFontEnigme->drawMultiline(_cEnigme,243+1, 879+1, 471,TEXTSTYLE_CENTER,3);
	_lpFontEnigme->setColor(0,0,0,0.8f);
	_lpFontEnigme->drawMultiline(_cEnigme,243, 879, 470,TEXTSTYLE_CENTER,3);

	for(int i=0; i<_nGameSize; i++) {
		cCell = _lpGrid->GetCellPos(i,0);
		_lpFont->drawStringCentered(itos(_vCurrentGame[i]+1).c_str(),(long)cCell.x,(long) (cCell.x + _cDim.x),(long)cCell.y);
	}
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuTresor::GameIsResolve()
{
	bool resolve = true;

	for (int i=0; i < _nGameSize; i++) {
		if (_vCurrentGame[i]+1 != _vSolution[i]) {
			resolve = false;
		}
	}
	return resolve;
 }

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuTresor::GameResolve()
{
	for (int i=0; i < _nGameSize; i++) {
		_vCurrentGame[i] = _vSolution[i]-1;
	}
}

void EMiniJeuTresor::GameReset()
{
	for (int i=0; i < _nGameSize; i++) {
		_vCurrentGame[i] = 0;
	}
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuTresor::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  ShowHint(MINIJEU_HINT1);
		  Reset(); 
		  break;
	  case MINIJEU_HINT2:
        ShowHint(MINIJEU_HINT2);
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

// Preload images et sons
void EMiniJeuTresor::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("ETresorBG","jpg",true,true);

   ESoundBank::getSound("mg_digit",false,true);
}
