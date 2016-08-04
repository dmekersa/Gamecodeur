#include <list>

#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "global.h"
#include "KRandom.h"

#include "EMiniJeuJigsawBook.h"

EMiniJeuJigsawBook::EMiniJeuJigsawBook(void)
{
	SetTextsTrad("JIGSAWMG");
	SetGameType(MINIJEU_DIRECTGAME);
	SetFeatures(MINIJEU_OBSERVATION);
	SetGameNumber(1,1);
	SetBackGround(EImageBank::getImage("ejigsaw_bg","jpg"));

	_nCol = 5;
	_nLine = 5;
	_nHintIncrement = 1;
	_lpImgOk = EImageBank::getImage("ejigsaw_ok","png");

   _cImageName = NULL;
	_lpImg = NULL;
	_lpImgCisor = NULL;
	_lpImgShadow = NULL;
	
	_lpGrid = NULL;
	
	_cPointFixed = Coord(-1,-1);
   _cCurrentSel = Coord(-1,-1);

	_fPartW = 0;
	_fPartH = 0;

	_bShowImg = false;
	_lpCounter = NULL;
   SetVariation(0);
}

EMiniJeuJigsawBook::~EMiniJeuJigsawBook(void)
{
	XDELETE(_lpGrid);
   FreeGrid();
	XDELETE(_lpCounter);
}

void EMiniJeuJigsawBook::SetVariation(int nVariation) {
   _nVariation = nVariation;
   if (_nVariation == -1) {
      _nVariation = floor(GetRandom(0,1.99));
   }

   switch (_nVariation) {
		case 0:
         SetBackGround(EImageBank::getImage("mg_jigsaw_box_bg","jpg"));
         _lpImgCisor = NULL;
         _lpImgShadow = NULL;
         SetGridParam(185,174,2,1);
         SetJigsawParam("mg_jigsaw_box_motif.jpg",6,4,-1,-1,1);
         SetGameNumber(1);
			break;
		case 1:
         SetBackGround(EImageBank::getImage("ejigsaw_bg","jpg"));
         _lpImgCisor = EImageBank::getImage("ejigsaw_cisor","png");
         _lpImgShadow = EImageBank::getImage("ejigsaw_dropshadow","png");
         SetGridParam(-1,-1,3,2);
         SetJigsawParam("ejigsaw_book.png", 5, 5,-1,-1, 1);
			SetGameNumber(11);
         break;
		case 2:
         SetBackGround(EImageBank::getImage("ejigsaw_bg","jpg"));
         _lpImgCisor = EImageBank::getImage("ejigsaw_cisor","png");
         _lpImgShadow = EImageBank::getImage("ejigsaw_dropshadow","png");
         SetGridParam(-1,-1,3,2);
         SetJigsawParam("ejigsaw_bonus1.png", 10, 10,10,10, 1);
			SetGameNumber(11);
         break;
		default:
			KAssert(false);
	}
   _lpImg = EImageBank::getImage(_cImageName);

	Reset();
}

void EMiniJeuJigsawBook::SetJigsawParam(char *cImageName,int nCol, int nLine, float cXFixed, float cYFixed, int nHintIncrement ) {
	
	
	// Libère les objet en cas d'utilisation de paramètre de jeux différents du défaut.
	if (_lpGrid != NULL) {
		XDELETE(_lpGrid);
		FreeGrid();
	}
	
	_cImageName = cImageName;
	_nCol = nCol;
	_nLine = nLine;

	_lpImg = EImageBank::getImage(_cImageName);
	
	_fPartW = floorf(_lpImg->getWidth() / _nCol);
	_fPartH = floorf(_lpImg->getHeight() / _nLine);
	
	if (cXFixed >= 0 && cYFixed >= 0) {
		KAssert(cXFixed < _lpImg->getWidth() && cXFixed >= 0);
		KAssert(cYFixed < _lpImg->getHeight() && cYFixed >= 0);
		_cPointFixed.x = floorf (cXFixed / _fPartW);
		_cPointFixed.y = floorf (cYFixed / _fPartH);
	} else {
		_cPointFixed = Coord(-1,-1);
	}

	_lpGrid = new Grid(_nCol, _nLine,_fPartW, _fPartH, _fGridOuterSpace, _fGridInnerSpace);
   if (_cGridPos.x < 0 && _cGridPos.y < 0) {
	   _lpGrid->Move((_nW-_lpGrid->width())/2,((_nH-_lpGrid->height())-10)/2 - 100);
   } else {
      _lpGrid->Move(_cGridPos.x,_cGridPos.y);
   }

	_vCurrentGame = std::vector< std::vector < Coord *> > (_nLine, std::vector< Coord *> (_nCol, (Coord *) NULL));
	_vCounter = std::vector< std::vector < KCounter * > > (_nLine, std::vector< KCounter * > (_nCol, (KCounter *) NULL));
	Reset();
}

/** 3 steps rendering **/
void EMiniJeuJigsawBook::GameCheck ()
{
	Coord cell;
	KSound *snd = ESoundBank::getSound("mg_select_deselect");

	if (Mouse.LeftEvent()) {
		if (_lpGrid->IsMouseOver()) {
			cell = _lpGrid->GetSelection(Mouse);
			if (cell.x < 0) {
				ESoundBank::getSound("mg_forbid")->playSample();
				return;
			} 
			Mouse.LeftEventClose();

			// la cellule cliquer correspond a une partie d'image bien placer
			if (cell == *(_vCurrentGame[(int)cell.y][(int)cell.x])) {
				ESoundBank::getSound("mg_forbid")->playSample();
				return;
			}

			if (_cCurrentSel.x < 0) {				// Aucune selection 
				_cCurrentSel = cell;
			} else if (_cCurrentSel == cell) {			// Deselectionne la cellule courante
				_cCurrentSel = Coord(-1,-1);
			} else {										// Swap deux parties d'image
				Coord *temp = _vCurrentGame[(int)cell.y][(int)cell.x];
				_vCurrentGame[(int)cell.y][(int)cell.x] = _vCurrentGame[(int)_cCurrentSel.y][(int)_cCurrentSel.x];
				_vCurrentGame[(int)_cCurrentSel.y][(int)_cCurrentSel.x] = temp;
				_cCurrentSel = Coord(-1,-1);
				IsResolved();
				snd = ESoundBank::getSound("mg_swap");
			}
			snd->playSample();
		}
	}
}

void EMiniJeuJigsawBook::GameLogic ()
{
}

void EMiniJeuJigsawBook::GameDrawSet ()
{
	if (_nGameStatus != MINIJEU_RESOLVED) {
		if (_lpImgShadow != NULL)
         _lpImgShadow->blitAlphaRectF(0,0,_lpImgShadow->getWidth(),_lpImgShadow->getHeight(),_lpGrid->_xPos - 6, _lpGrid->_yPos);
		for(int i=0; i<_nLine; i++) {
			for (int j=0; j<_nCol; j++) {
				DrawImagePart(*(_vCurrentGame[i][j]),Coord(j,i));
			}
		}
		_lpGrid->DrawGrid(_lpImg);
	}
   if (_lpImgCisor != NULL)
   	_lpImgCisor->blitAlphaRect(0,0,_lpImgCisor->getWidth(),_lpImgCisor->getHeight(),-40,0);

   ShowImg();
}

// Retourne true si le mini jeu est rŽsolu
bool EMiniJeuJigsawBook::GameIsResolve()
{	
	// Test de résolution
	for(int i=0; i<_nLine; i++) {
		for (int j=0; j<_nCol; j++) {
			if ( Coord(j,i) != *(_vCurrentGame[i][j]))
				return false;
		}
	}

	_bShowImg = true;
	_lpCounter = new KCounter();
	_lpCounter->startCounter(0,1,0,1500,K_COUNTER_LINEAR);

	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuJigsawBook::GameResolve()
{
   FreeGrid();
	for(int i=0; i<_nLine; i++) {
		for (int j=0; j<_nCol; j++) {
			_vCurrentGame[i][j] = new Coord(j,i);
		}
	}
	IsResolved();
}

void EMiniJeuJigsawBook::GameReset()
{
	std::list< Coord * > gameSet;

   FreeGrid();

	// Generation de toute les coordonnées des parties de l'image
	for(int i=0; i<_nLine; i++) {
		for (int j=0; j<_nCol; j++) {
			if (Coord(j,i) == _cPointFixed) {	// permet de laisser une partie de l'image a ca place
				_vCurrentGame[i][j] = new Coord(j,i);	
				_vCounter[i][j] = new KCounter();
				_vCounter[i][j]->startCounter(0,1,0,200,K_COUNTER_LINEAR);
				continue;
			}
			gameSet.push_back(new Coord((float)j,(float)i));
		}
	}

	// Placement aléatoire des partie de l'image
	for(int i=0; i<_nLine; i++) {
		for (int j=0; j<_nCol; j++) {
			
			if (_vCurrentGame[i][j] != NULL)	// permet de laisser une partie de l'image a ca place
				continue;

			bool bDiff = false;
			std::list< Coord * >::iterator it;
			int r;
			while (bDiff == false) {
				// Traitement du cas particulier: dernière rpos corespond a la dernière place dans la grille
				if (gameSet.size() == 2) {
					it = gameSet.begin();
					it++;
					if (**it == Coord (j+1,i)) 
						break;
				}
				r = EMiniGame::GetRandom(gameSet.size());
				for (it = gameSet.begin(); it != gameSet.end() && r != 0; it++,r--) {}
				if (*(*(it)) != Coord(j,i))
					bDiff = true;
			}
			
			_vCurrentGame[i][j]= *it;
			gameSet.remove(_vCurrentGame[i][j]);
		}
	}

	_cCurrentSel = Coord(-1,-1);
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuJigsawBook::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  ShowHint(MINIJEU_HINT1);
		  _nCol = _nCol - _nHintIncrement;
		  _nLine = _nLine - _nHintIncrement;
		  SetJigsawParam(_cImageName,_nCol, _nLine, _cPointFixed,_nHintIncrement);
		  break;
	  case MINIJEU_HINT2:
		  ShowHint(MINIJEU_HINT2);
		  _nCol = _nCol - _nHintIncrement;
		  _nLine = _nLine - _nHintIncrement;
		  SetJigsawParam(_cImageName,_nCol, _nLine, _cPointFixed,_nHintIncrement);
		  break;
	  case MINIJEU_RESOLVED:
		  Resolve();
		  break;
	  default:
		  KAssert(false);
		  break;
	}
}

void EMiniJeuJigsawBook::DrawImagePart(Coord imgSel, Coord gridCell) {
	Coord imgSelPos = Coord(imgSel.x * _fPartW, imgSel.y * _fPartH);
	Coord cellPos = _lpGrid->GetCellPos(gridCell);
	
	float fElapsed = (float) MyGame::getGame()->getKWindow()->getFrameTime();

	_lpImg->blitAlphaRectF(imgSelPos.x,imgSelPos.y,imgSelPos.x + _fPartW+1, imgSelPos.y + _fPartH+1, cellPos.x, cellPos.y);
	
	if (imgSel == gridCell) {
		float fAlphaFinal = 0.40f;
		float fAlpha = fAlphaFinal;
		float fAlphaOk = 0;
		float fScaleOk = 1;

		// Initialisation timeout et lancement d'un son si la pièce viens d'etre bien placer
		if (_vCounter[(int)gridCell.y][(int)gridCell.x] == NULL) {
			ESoundBank::getSound("mg_good")->playSample();
			_vCounter[(int)gridCell.y][(int)gridCell.x] = new KCounter();
			_vCounter[(int)gridCell.y][(int)gridCell.x]->startCounter(0,fAlphaFinal,0,1000,K_COUNTER_LINEAR);
		}

		// Deplacement du compteur et calcul du alpha pour la pièce grisé et le signe ok
		if (!_vCounter[(int)gridCell.y][(int)gridCell.x]->isCompleted()) {
			_vCounter[(int)gridCell.y][(int)gridCell.x]->move(fElapsed);
			fAlpha = _vCounter[(int)gridCell.y][(int)gridCell.x]->getCurrentValue();
			fAlphaOk = 1 - fabsf((1/(fAlphaFinal/2) * (fAlpha-(fAlphaFinal/2))));
			fScaleOk = (_fPartW/2)/_lpImgOk->getWidth();
		}

		// Affichage de l'overlay pour les pièces bien positionné et pour l'indicateur de positionnement correct 
		_lpImg->drawRect(cellPos.x, cellPos.y,cellPos.x + _fPartW, cellPos.y+_fPartH,0,0,0,fAlpha);

		if (fAlphaOk != 0) {
			_lpImgOk->blitAlphaRectFx(0,0,
				_lpImgOk->getWidth(),_lpImgOk->getHeight(),
				(short) (cellPos.x + (_fPartW/2) - (_lpImgOk->getWidth()/2)),
				(short) (cellPos.y + (_fPartH/2) - (_lpImgOk->getHeight()/2)),
				0,fScaleOk,fAlphaOk);
		}
	}

	if (gridCell == _cCurrentSel) {
		_lpImg->drawRect(cellPos.x, cellPos.y,cellPos.x + _fPartW, cellPos.y+_fPartH,1,0.2f,0.2f,0.3f);
	}
}

void EMiniJeuJigsawBook::FreeGrid() {
   //clean des jeux precedant
   for (int i=0; i < (int)_vCurrentGame.size(); i++) {
		for (int j=0; j < (int)_vCurrentGame[0].size(); j++) {
			XDELETE(_vCurrentGame[i][j]);
			XDELETE(_vCounter[i][j]);
		}
	}
}

void EMiniJeuJigsawBook::ShowImg() {
	if (!_bShowImg)
		return;

	if (!_lpCounter->isCompleted())
		_lpCounter->move(MyGame::getGame()->getKWindow()->getFrameTime());

   if (_nVariation == 0) {
      _lpImg->blitAlphaRectFx(0,0, 
         _lpImg->getWidth(),_lpImg->getHeight(),_lpGrid->_xPos,_lpGrid->_yPos,0,1,_lpCounter->getCurrentValue());
      return;
   }
   
   float x, y;

	x=(_nW - _lpImg->getWidth()) /2;
	y=(_nH - _lpImg->getHeight() -10)/2;
	x= _lpGrid->_xPos + (x - _lpGrid->_xPos) * _lpCounter->getCurrentValue();
	y= _lpGrid->_yPos + (y - _lpGrid->_yPos) * _lpCounter->getCurrentValue();

	_lpImg->blitAlphaRectFx(0,0, 
			_lpImg->getWidth(),_lpImg->getHeight(),x,y,0,1+(float)(_lpCounter->getCurrentValue()*0.2),_lpCounter->getCurrentValue());
}

// Preload images et sons
void EMiniJeuJigsawBook::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("ejigsaw_bg","jpg",true,true);
   EImageBank::getImage("ejigsaw_ok","png",true,true);
   EImageBank::getImage("ejigsaw_cisor","png",true,true);
   EImageBank::getImage("ejigsaw_dropshadow","png",true,true);

   ESoundBank::getSound("mg_good",false,true);
}
