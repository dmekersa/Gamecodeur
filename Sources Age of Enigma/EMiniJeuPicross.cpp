#include <iostream>
#include <sstream>

#include "EMiniJeuHelper.h"
#include "EMiniJeuPicross.h"
#include "ImageBank.h"
#include "assert.h"
#include "MyGame.h"
#include "KGraphic.h"
#include "global.h"
#include "EPointer.h"

// Sweethome
bool _tbMapPicross[10][10]= {
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,1,0,0,0,0,
	0,0,0,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,0,
	0,1,0,0,1,1,0,0,1,0,
	0,1,0,0,1,1,0,0,1,0,
	0,1,1,1,1,1,0,0,1,0,
	0,1,1,1,1,1,0,0,1,0,
};


EMiniJeuPicross::EMiniJeuPicross(void)
{
	SetTitle("Picross");
	SetRule("Click on SUBMIT when you want our aprouval\n\nClick on HELP to lower the difficulty.");
	SetGameType(MINIJEU_SUBMITGAME);
	SetFeatures(MINIJEU_LOGIC);

	_lpGraphicToken = KPTK::createKGraphic();
	_lpGraphicToken = EImageBank::getImage("gallet.png");

   EGLL();
	_lpHintFont = EFontBank::getFont("acmesa.ttf",20);
	EGLR();
	_lpHintFont->setColor(0,0,0,1);
	//_lpHintFont->setBlitColor(0,0,0,1);

	_nGameStatus = MINIJEU_NORMAL;
	_nHintCol=0;

	_nCol=10;
	_nLine=10;

	// Préparation de Target pour dessiner le mini jeu dedans...
	_lpGraphicTarget = KPTK::createKGraphic();
	_lpGraphicTarget->setTextureQuality(true);

	_nWidthCell=_lpGraphicToken->getWidth();

	// Initialisation tableau de Jeu
	_vCurrentGame=std::vector< std::vector< bool > > (_nLine,std::vector<bool> (_nCol,false));

	// Positionnement la grille principale
	_lpGrid = new Grid(_nCol,_nLine,_nWidthCell,_nWidthCell,2,2);
	_lpGrid->Move(_nW-_lpGrid->width(),	_nH-_lpGrid->height());

	// initialisation des hint de ligne.
	_vHintLine = std::vector<std::string> (_nLine,"");
	_nMaxSizeText = 0;

	_lpHintFont->setYSpacing(0.1f);
	for (int seq=0, size=0, i=0; i<_nLine; i++) {
		for (int j=0; j<_nCol; j++) {
			if (_tbMapPicross[i][j]) {
				seq++;
			}
			else if (seq>0) {
				_vHintLine[i]=_vHintLine[i]+" - "+itos(seq);
				seq=0;
			}
		}
		if (seq>0)
			_vHintLine[i] = _vHintLine[i]+" - "+itos(seq);
		if (_vHintLine[i].length()>0) {
			_vHintLine[i] = _vHintLine[i].substr(2);
			size = (int)_lpHintFont->getStringWidth(_vHintLine[i].c_str());
			if (size>_nMaxSizeText)
				_nMaxSizeText = (float)size;
		}
		seq=0;
	}

	// Hints ligne.
	_nMaxSizeText = _nMaxSizeText + 2*_nTextMargin;
	_lpHintLineGrid = new Grid(1, _nLine, _nMaxSizeText, _nWidthCell, 2, 2);
	_lpHintLineGrid->Move(_nW-_lpGrid->width()-_nMaxSizeText-2,_lpGrid->_yPos);
	_lpHintLineGrid->SetLineOdd(true);

	// Initialisation des Hints de lignes.
	_vHintCol = std::vector<std::string> (_nCol,"");
	_nMaxSizeText = 0;

	for (int seq=0, size=0, i=0; i<_nCol; i++) {
		for (int j=0; j<_nLine; j++) {
			if (_tbMapPicross[j][i])
				seq++;
			else if (seq>0) {
				_vHintCol[i]+=itos(seq)+"\n";
				seq=0;
			}
		}
		if (seq>0)
			_vHintCol[i] += itos(seq)+"\n";
		if (_vHintCol[i].length()>0) {
			_vHintCol[i] = _vHintCol[i].substr(0,_vHintCol[i].length()-1);
			size = (int)_lpHintFont->getMultilineHeight(_vHintCol[i].c_str(),0,(long)_nWidthCell,_nW,TEXTSTYLE_CENTER);
			if (size>_nMaxSizeText)
				_nMaxSizeText = (float)size;
		}
		seq=0;
	}

	// Hints colonne.
	_nMaxSizeText = _nMaxSizeText + 2*_nTextMargin;
	_lpHintColGrid = new Grid(_nCol,1,_nWidthCell,_nMaxSizeText,2,2);
	_lpHintColGrid->Move(_lpGrid->_xPos,_nH-_lpGrid->height()-_nMaxSizeText-2);
	_lpHintColGrid->SetColOdd(true);


	// Positionnement la zone de Hint
	_lpHintGrid = new Grid(_nCol,_nLine,15,15,0,0);
	_lpHintGrid->Move(
		(_lpGrid->_xPos-_lpHintGrid->width())/2,
		(_lpGrid->_yPos-_lpHintGrid->height())/2);
	_lpHintGrid->SetDefaultColor(0.8f,0.9f,0.9f);
	_lpHintGrid->SetFillColor(0.1f,0.1f,0.1f);
}

EMiniJeuPicross::~EMiniJeuPicross(void)
{
	XDELETE(_lpGraphicTarget);
	XDELETE(_lpGrid);
	XDELETE(_lpHintGrid);
	XDELETE(_lpHintColGrid);
	XDELETE(_lpHintLineGrid);
   XDELETE(_lpHintFont);
}

/** 3 steps rendering **/
void EMiniJeuPicross::Check ()
{
	// Si le bouton est pressé

	if (Mouse.LeftEvent())
	{
		if (_nGameStatus<MINIJEU_RESOLVED) {
			struct Coord c = _lpGrid->GetSelection(Mouse.x,Mouse.y);
			
			if (!(c.x<0 || c.y<0)) {
				_vCurrentGame[(int)c.y][(int)c.x]= ! _vCurrentGame[(int)c.y][(int)c.x];
				Mouse.LeftEventClose();
			}
		}
	}
}

void EMiniJeuPicross::Logic ()
{
}

void EMiniJeuPicross::Draw ()
{
	const MyGame *lpMyGame=(MyGame *)MyGame::getGame();
	struct Coord pos1, pos2;

	lpMyGame->_lpKWindow->setWorldView((float)_nX,(float)0-_nY,0,1,false);

//	_lpFont->drawStringFromLeft("Exemple de mini jeu (hors target)",0,0);

	// Affichage des ÈlÈments de bases
	_lpGrid->DrawBaseGrid(_lpGraphicTarget,1);
	_lpHintColGrid->DrawBaseGrid(_lpGraphicTarget,1);
	_lpHintLineGrid->DrawBaseGrid(_lpGraphicTarget,1);
	_lpHintGrid->DrawBaseGrid(_lpGraphicTarget,1);


	// Affichage zone de hint
	for (int i=0; i<_nHintCol; i++) {
		for (int j=0; j<_nLine; j++) {
			if (_tbMapPicross[j][i]) {
				_lpHintGrid->FillCell(_lpGraphicTarget,i,j,0.5f,0.5f,0.5f);
			} else {
				_lpHintGrid->FillCell(_lpGraphicTarget,i,j,0.8f,0.5f,0.5f);
			}
		}
	}

	for (int i=0; i<_nLine; i++) {
		// Affichage des hints de lignes.
		_lpHintLineGrid->GetZone(0,i,&pos1,&pos2);
		_lpHintFont->drawStringFromRight(_vHintLine[i].c_str(),(long)pos2.x-_nTextMargin,(long)pos1.y);

		for (int strH=0, j=0; j<_nCol; j++) {
			// Affichage des hints de colonnes.
			if (i==0) {
				strH=_lpHintFont->getMultilineHeight(_vHintCol[j].c_str(),(long)pos1.x,(long)pos2.x,(long)pos1.y,TEXTSTYLE_CENTER);
				_lpHintColGrid->GetZone(j,0,&pos1,&pos2);
				_lpHintFont->drawMultiline(_vHintCol[j].c_str(),(long)pos1.x,(long)pos2.x,(long)pos2.y-strH-_nTextMargin,TEXTSTYLE_CENTER);
			}
			
			if (_nGameStatus==MINIJEU_RESOLVED && _tbMapPicross[i][j])
					_lpGrid->FillCell(_lpGraphicTarget,j,i);
			
			if (_vCurrentGame[i][j]) {
				if (_nGameStatus==MINIJEU_RESOLVED) {
					if  (! _tbMapPicross[i][j])
						_lpGrid->FillCell(_lpGraphicTarget,j,i,0.8f,0.4f,0.4f,1);
				} else {
					_lpHintGrid->FillCell(_lpGraphicToken,j,i,true);
				}
				_lpGrid->FillCell(_lpGraphicToken,j,i,false);
			}
		}
	}

	lpMyGame->_lpKWindow->setWorldView((float)_nX,(float)0-_nY,0,1,false);
   DrawEnd();
}


// Retourne true si le mini jeu est résolu
bool EMiniJeuPicross::IsResolved()
{
	if (_nGameStatus == MINIJEU_RESOLVED)
		return true;

	for (int i=0;i<_nCol;i++)
		for (int j=0;j<_nCol;j++)
			if (_tbMapPicross[i][j] !=  _vCurrentGame[i][j])
				return false;

	Resolve();
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuPicross::Resolve()
{
	_nGameStatus = MINIJEU_RESOLVED;
	_nHintCol = _nCol;

	//for (int i=0;i<_nLine;i++) {
	//	for (int j=0;j<_nCol;j++) {
	//		if (_tbMapPicross[i][j])
	//			_vCurrentGame[i][j]= true;
	//	}
	//}
}

// Active un niveau d'indices (_nGameStatus = 1, 2 ou 3)
void EMiniJeuPicross::ActivateHint(int nHintLevel)
{
	if (nHintLevel <= _nHintLevel || _nGameStatus >= MINIJEU_RESOLVED)
		return;

	switch (nHintLevel) {
	case MINIJEU_HINT1:
		_nHintLevel = MINIJEU_HINT1;
		_nHintCol=(_nCol*(_nGameStatus+1)/4)+1;
		break;
	case MINIJEU_HINT2:
		_nHintLevel = MINIJEU_HINT2;
		_nHintCol=(_nCol*(_nGameStatus+1)/4)+1;
		break;
	case MINIJEU_RESOLVED:
		Resolve();
		_nHintCol=(_nCol*(_nGameStatus+1)/4)+1;
		_nHintLevel = MINIJEU_HINT3;
		break;
	default:
		KAssert(false);
		break;
	}
}



// Preload images et sons
void EMiniJeuPicross::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("gallet.png","",true,true);
}
