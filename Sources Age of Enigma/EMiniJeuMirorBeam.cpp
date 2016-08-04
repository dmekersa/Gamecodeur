#include "assert.h"

#include "MyGame.h"
#include "ImageBank.h"
#include "EPointer.h"
#include "FontBank.h"
#include "SoundBank.h"

#include "EMiniJeuMirorBeam.h"

EMiniJeuMirorBeam *EMiniJeuMirorBeam::IMG = NULL;

EMiniJeuMirorBeam::EMiniJeuMirorBeam(void)
{
	using namespace EMiniGame;

	if (IMG != NULL)
		XDELETE(IMG);
	IMG = this;

	SetTextsTrad("MIRORGRID");
	SetGameType(MINIJEU_DIRECTGAME);
	SetGameNumber(19);
	SetBackGround(EImageBank::getImage("mg_mirorsgrid_bg","jpg"),0,0);

	_lpFont = EFontBank::getFont("NITECLUB.TTF",40);
	_lpFont->setColor(0,0,0,1);

	_lpImgGrid = EImageBank::getImage("mg_mirorsgrid_grid","png");
	_lpSelBg = EImageBank::getImage("mg_mirorsgrid_sel_bg","png");
	_lpImgWrong = EImageBank::getImage("minigame_middleage_wrong","png");
   _lpImgArrow = EImageBank::getImage("mg_mirrorsgrid_arrow","png");

	_nNumPart = std::vector< int >(2,0);	
	_lpImgMiror1 = EImageBank::getImage("mg_mirorsgrid_miror1","png");
	_lpImgMiror2 = EImageBank::getImage("mg_mirorsgrid_miror2","png");
	_vMirors.push_back(new EMirorPart(_lpImgMiror1));
	_vMirors.push_back(new EMirorPart(_lpImgMiror2));
	
	_lpGridSet = NULL;

	_vGame  = std::vector< std::vector< int > >( _nGameSize, std::vector< int >(_nGameSize, 0));

	_lpGridGame = new Grid(_nGameSize,_nGameSize,_lpImgMiror1->getWidth(),_lpImgMiror1->getHeight(),2,2);
	_lpGridGame->Move((_nW - _lpGridGame->width()) /2, (_nH - _lpGridGame->height())/2);
	_lpGridGame->Move(233+71,67+63);

	Coord pos1 = Coord(_lpGridGame->_xPos,_lpGridGame->_yPos);
	Coord pos2 = Coord(pos1.x + _lpGridGame->width(), pos1.y + _lpGridGame->height());

	_vGridPair = std::vector< Grid * >(4, (Grid *)NULL);
	_vGridPair[NORTH] = new Grid(_nGameSize,1,_lpImgMiror1->getWidth(),_lpImgMiror1->getHeight(),2,2);
	_vGridPair[NORTH]->Move(pos1.x, pos1.y - _vGridPair[NORTH]->height());
	_vGridPair[NORTH]->SetFont(_lpFont);
	_vGridPair[EAST] = new Grid(1,_nGameSize,_lpImgMiror1->getWidth(),_lpImgMiror1->getHeight(),2,2);
	_vGridPair[EAST]->Move(pos2.x, pos1.y);
	_vGridPair[EAST]->SetFont(_lpFont);
	_vGridPair[SOUTH] = new Grid(_nGameSize,1,_lpImgMiror1->getWidth(),_lpImgMiror1->getHeight(),2,2);
	_vGridPair[SOUTH]->Move(pos1.x, pos2.y);
	_vGridPair[SOUTH]->SetFont(_lpFont);
	_vGridPair[WEST] = new Grid(1,_nGameSize,_lpImgMiror1->getWidth(),_lpImgMiror1->getHeight(),2,2);
	_vGridPair[WEST]->Move(pos1.x - _vGridPair[WEST]->width(),pos1.y);
	_vGridPair[WEST]->SetFont(_lpFont);

	_vPaths = std::vector< std::vector< Path * > >(4, std::vector< Path * >(_nGameSize,(Path *)NULL));
	for (int i=0; i<_vPaths.size(); i++) {
		for (int j=0; j< _vPaths[0].size(); j++) {
			_vPaths[i][j] = new Path(j,i);
		}
	}

	_lpGrabedPart = NULL;

	GenerateGrid();
	Reset();
}

EMiniJeuMirorBeam::~EMiniJeuMirorBeam(void)
{
	XDELETE(_lpGridGame);
   XDELETE(_lpGridSet);
	for (int i=0; i<_vPaths.size(); i++) {
		XDELETE(_vGridPair[i]);
		for (int j=0; j< _vPaths[0].size(); j++) {
			_vPaths[i][j]->_endCell = _vPaths[i][j]->_startCell;
			XDELETE(_vPaths[i][j]);
		}
	}
	for (int i=0; i<_vPartSet.size(); i++) {
		XDELETE(_vPartSet[i]);
	}
	XDELETE(_lpGrabedPart);
	XDELETE(_vMirors[0]);
	XDELETE(_vMirors[1]);
   
	IMG = NULL;
   XDELETE(_lpFont);
}

/** 3 steps rendering **/
void EMiniJeuMirorBeam::GameCheck ()
{
	Coord pos;
	EMirorPart *tmp;
	KSound *snd= ESoundBank::getSound("mg_select_deselect");
	int indexPart;

	if(!Mouse.LeftEvent())
		return;

	// Drop de la pièce courante
	if (_lpGrabedPart != NULL) {
		Mouse.LeftEventClose();

		// Recuperation de l'indice de la piece
		indexPart = 0;
		if (*_lpGrabedPart != *(_vMirors[0]))
			indexPart = 1;

		if (!_lpGridGame->IsMouseOver()) {
			pos = _lpGridSet->GetMouseSelection();
			
			tmp = NULL;
			if (pos.y >= 0) {
				// Swap de pièce
				tmp = _vPartSet[pos.y];
			} else {
				// Récupération du premier emplacement libre sur la grille
				for (int i=0; i<_vPartSet.size(); i++) {
					pos = Coord(0,i);
					if (_vPartSet[i] == NULL)
						break;
				}
			}

			// Finalisation du swap de pièce
			if (tmp != NULL) {
				if (*tmp == *_lpGrabedPart)
					return;
				_nNumPart[indexPart]++;
				
				indexPart = 0;
				if (*tmp != *(_vMirors[0]))
					indexPart = 1;
				tmp->Grab();
				tmp->Move(_lpGrabedPart->Pos(),200);
				_nNumPart[indexPart]--;
				snd = ESoundBank::getSound("mg_swap");
			}

			// Release dans le set du jeux de la pièces
			_lpGrabedPart->Move(_lpGridSet->GetCellPos(pos),200);
			_lpGrabedPart->Release();
			_vPartSet[pos.y] = _lpGrabedPart;
			_lpGrabedPart = tmp;

			Path::_cDisplayMod = Coord(-1,-1);
			Path::ResolvePaths();
			snd->playSample();
			return;
		}

		// Pièce inamovible
		pos = _lpGridGame->GetMouseSelection();
		if (_vGameSet[pos.y][pos.x] > 0) {
			ESoundBank::getSound("mg_forbid")->playSample();
			return;
		}
	
		// Depose de la pièce dans la grille de jeux		
		XDELETE(_lpGrabedPart);

		// Switch de pièces
		if (_vGame[pos.y][pos.x] > 0) {
			snd = ESoundBank::getSound("mg_swap");
			_lpGrabedPart = new EMirorPart(_vMirors[_vGame[pos.y][pos.x] -1]);
			_lpGrabedPart->Grab();
			_lpGrabedPart->Show();
		}
		_vGame[pos.y][pos.x] = indexPart +1;
		
		
		Path::_cDisplayMod = pos;
		Path::ResolvePaths();
		snd->playSample();
		IsResolved();
		return;
	}
	
	// Selection d'une piece dans la grille de jeux
	if (_lpGridGame->IsMouseOver()) {
		Mouse.LeftEventClose();

		pos = _lpGridGame->GetMouseSelection();
		if (_vGame[pos.y][pos.x] > 0) {
			if (_vGameSet[pos.y][pos.x] > 0) {
				ESoundBank::getSound("mg_forbid")->playSample();
				return;
			}
			indexPart = _vGame[pos.y][pos.x] -1;
			_lpGrabedPart = new EMirorPart(_vMirors[indexPart]);
			_vGame[pos.y][pos.x] = 0;
			_lpGrabedPart->Show();
			_lpGrabedPart->Grab();
			_vMirors[indexPart]->FadOut(200);

			Path::_cDisplayMod = pos;
			Path::ResolvePaths();
			snd->playSample();
			return;
		}
	}

	if (_lpGridSet->IsMouseOver()) {
		Mouse.LeftEventClose();

		pos = _lpGridSet->GetMouseSelection();
		_lpGrabedPart = _vPartSet[pos.y];
		
		if (_lpGrabedPart == NULL)
			return;

		_vPartSet[pos.y] = NULL;
		_lpGrabedPart->Grab();

		indexPart = 0;
		if (*_lpGrabedPart != *(_vMirors[0]))
			indexPart = 1;
		_nNumPart[indexPart]--;
		snd->playSample();
		return;
	}
}

void EMiniJeuMirorBeam::GameLogic ()
{
	if (_lpGrabedPart != NULL) {
		_lpGrabedPart->Logic();
	}
}

void EMiniJeuMirorBeam::GameDrawSet()
{		
	KGraphic *tmpImg;
	Coord pos;

	_lpImgGrid->blitAlphaRectF(0,0,_lpImgGrid->getWidth(), _lpImgGrid->getHeight(),233,67);
	//_lpGridGame->DrawBaseGrid(_lpImgGrid,0.5f);

   // Dessin du distributeur de pièces et des pièces
   _lpImgArrow->blitAlphaRectF(0,0,_lpImgArrow->getWidth(),_lpImgArrow->getHeight(),_arrowPos.x,_arrowPos.y);
	_lpGridSet->DrawBaseGrid(_lpImgMiror1);
	_lpSelBg->blitAlphaRect(0,0,_lpSelBg->getWidth(),_lpGridSet->height() + 3, _lpGridSet->_xPos - 5, _lpGridSet->_yPos - 3);
	for (int i=0; i < _vPartSet.size(); i++) {
		if (_vPartSet[i] != NULL) _vPartSet[i]->Draw();
	}

	Path::Draw();

	// Affichage des pair en bord de grille de jeux
	for (int i=0; i<_vGridPair.size(); i++) {
		// Affichage du chemin sur survol d'un chiffre
		for (int j=0; j<_vPair[i].size(); j++) {
			if (i%2 > 0) {
				_vGridPair[i]->FillCell(_vPair[i][j],j,0);
			} else {
				_vGridPair[i]->FillCell(_vPair[i][j],0,j);
			}
		}
	}
	
	// Affichage du plateau de jeux
	for (int i=0; i<_vGame.size(); i++) {
		for (int j=0; j<_vGame[0].size(); j++) {
			if (_vGame[i][j] == 0)
				continue;

			tmpImg = _lpImgMiror1;
			if (_vGame[i][j] == 2)
				tmpImg = _lpImgMiror2;

			pos = _lpGridGame->GetCellPos(j,i);
			if (_vGameSet[i][j] > 0) {
				tmpImg->setBlitColor(0.6,0.6,0.6,1);
				tmpImg->blitAlphaRect(0,0,tmpImg->getWidth(),tmpImg->getHeight(),pos.x,pos.y);
				tmpImg->setBlitColor(1,1,1,1);
			} else {
				tmpImg->blitAlphaRect(0,0,tmpImg->getWidth(),tmpImg->getHeight(),pos.x,pos.y);
			}
		}
	}
	if (_nGameStatus > MINIJEU_NORMAL) {
		for (int i=0; i<_vGame.size(); i++) {
			for (int j=0; j<_vGame[0].size(); j++) {
				if (_vGame[i][j] > 0 && _vGame[i][j] != abs(_vGameSet[i][j]))
					_lpGridGame->FillCell(_lpImgWrong,j,i,true,1);
			}
		}
	}
}

void EMiniJeuMirorBeam::GameDrawGrab() {
	if (_lpGrabedPart != NULL) {
		_lpGrabedPart->Draw();
	}
}
void EMiniJeuMirorBeam::GameReset() {
	// Netoyage du plateau
	XDELETE(_lpGrabedPart);
	for (int i=0; i<_vPartSet.size(); i++) {
		XDELETE(_vPartSet[i]);
	}
	_vPartSet.clear();

	// Initialisation
	_nNumPart[0] = 0;
	_nNumPart[1] = 0;
	for (int i=0; i<_vGameSet.size(); i++) {
		for (int j=0; j<_vGameSet[0].size(); j++) {
			_vGame[i][j] = 0;
			if(	_vGameSet[i][j] > 0) {
				_vGame[i][j] = _vGameSet[i][j];
			} else if (_vGameSet[i][j] == -1) {
				_nNumPart[0]++;
			} else if (_vGameSet[i][j] == -2) {
				_nNumPart[1]++;
			}
		}
	}

	// Creation du set de pièce restante
	XDELETE(_lpGridSet);
	_lpGridSet = new Grid(1,_nNumPart[0] + _nNumPart[1],_lpImgMiror1->getWidth(),_lpImgMiror1->getHeight(),2,2);
	_lpGridSet->Move(60,_lpGridGame->_yPos + (_lpGridGame->height() - _lpGridSet->height())/2 +60);

	EMirorPart *tmp;
	int pos = 0;
	for (int i=0; i < 2; i++) {
		for (int j=0; j < _nNumPart[i]; j++) {
			tmp = new EMirorPart(_vMirors[i]);
			_vPartSet.push_back(tmp);
			tmp->Move(_lpGridSet->GetCellPos(0,pos));
			tmp->Hide();
			tmp->FadIn(200);
			pos++;
		}
	}
	Path::ResolvePaths();

   _arrowPos.x = _lpGridSet->_xPos + _lpGridSet->width() -5;
   _arrowPos.y = _lpGridSet->_yPos + (_lpGridSet->height() - _lpImgArrow->getHeight()) /2;
}

// Retourne true si le mini jeu est rŽsolu
bool EMiniJeuMirorBeam::GameIsResolve()
{
	for (int i=0; i<_vGame.size(); i++) {
		for (int j=0; j<_vGame[0].size(); j++) {
			if (abs(_vGameSet[i][j]) != _vGame[i][j])
				return false;
		}
	}
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuMirorBeam::GameResolve()
{
	for (int i=0; i<_vGame.size(); i++) {
		for (int j=0; j<_vGame[0].size(); j++) {
			 _vGame[i][j] = abs(_vGameSet[i][j]);
		}
	}
	_nNumPart[0]=0;
	_nNumPart[1]=0;
	_vMirors[0]->FadOut(200);
	_vMirors[1]->FadOut(200);
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuMirorBeam::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		_vGameSet[2][2] = abs(_vGameSet[2][2]);
		  ShowHint(MINIJEU_HINT1);
		  Reset(); 
		  break;
	  case MINIJEU_HINT2:
		  _vGameSet[2][4] = abs(_vGameSet[2][4]);
		  _vGameSet[1][4] = abs(_vGameSet[1][4]);
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

void EMiniJeuMirorBeam::GenerateGrid() {
	// Initialisation de la solution et du set initial
	int initSol[_nGameSize][_nGameSize] = {
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 2, 0,-1, 0, 1, 0,
		 0, 0,-2, 0,-1, 0, 0, 0,
		-2, 0, 0,-1, 0, 1, 0, 0,
		 0, 0, 1, 0, 0, 0, 1, 0,
		 0, 0, 0,1, 0, 0, 0,-1,
		 0, 0, 0, 0,-2, 0, 0, 0,
		 1, 0, 0, 0,1, 0, 0, 0
	};
	_vGameSet = std::vector< std::vector< int > >( _nGameSize, std::vector< int >(_nGameSize, 0));
	for (int i=0; i<_vGameSet.size(); i++) {
		for (int j=0; j<_vGameSet[0].size(); j++) {
			_vGameSet[i][j] = initSol[i][j];
		}
	}

	// initialisation des tableaux de pairs
	char* initPair[4][_nGameSize] = {
		 "7", "5","16","15","16", "4","14","15",
		"14", "2", "8", "6","13","10","11","12",
		 "7", "8", "9","10","11","12", "9","13",
		 "1", "2", "3", "1", "3", "4", "5", "6"
	};
	_vPair = std::vector< std::vector< char* > >(4, std::vector< char* >(_nGameSize,""));
	for (int i=0; i<_vPair.size(); i++)
		for (int j=0; j<_vPair[0].size(); j++)
			_vPair[i][j] = initPair[i][j];
};

std::vector< KGraphic * > Path::_vBeamImg;
EMiniJeuMirorBeam *Path::IMG = NULL;
int Path::ninst = 0;

KCounter *Path::_lpDisplayMod;
Coord Path::_cDisplayMod;
int Path::_nDisplayPath = 0;
Path::Path(int pos, int indGridPair) {
	KAssert(EMiniJeuMirorBeam::IMG != NULL);

	if (IMG == NULL) {
		_vBeamImg.clear();
		_vBeamImg.push_back(EImageBank::getImage("mg_mirorsgrid_T1","png"));
		_vBeamImg.push_back(EImageBank::getImage("mg_mirorsgrid_T2","png"));
		_vBeamImg.push_back(EImageBank::getImage("mg_mirorsgrid_T3","png"));
		_vBeamImg.push_back(EImageBank::getImage("mg_mirorsgrid_T4","png"));
		_vBeamImg.push_back(EImageBank::getImage("mg_mirorsgrid_S1","png"));
		_vBeamImg.push_back(EImageBank::getImage("mg_mirorsgrid_S2","png"));

		IMG = EMiniJeuMirorBeam::IMG;
	}

	_bPaired = false;

	_startCell.x = pos;
	_startCell.y = indGridPair;
	_endCell = Coord(-1,-1);
	
	_lpDisplayMod = NULL;

	ninst++;
}
Path::~Path(void) {
	ninst--;
	if (ninst == 0) {
		IMG = NULL;
		_vBeamImg.clear();
		XDELETE(_lpDisplayMod);
	}
}

Coord Path::ResolvePath() {
	KGraphic *imgInd;

	// Determiniation pos initial et direction initial
	EMiniGame::Direction curDir = (EMiniGame::Direction) ((int)(_startCell.y+2)%4);
	Coord curCell = EMiniGame::GetDirVector(curDir);
	curCell = Coord(
		curCell.x==0?_startCell.x:0 + curCell.x==-1?IMG->_nGameSize:-1,
		curCell.y==0?_startCell.x:0 + curCell.y==-1?IMG->_nGameSize:-1
		);
	
	_cells.clear();
	
	// Découverte du chemin
	while(NextStep(&curCell,&curDir,&imgInd)) {
		_cells.push_back(CellPath(curCell,imgInd));
	};
	
	// Determination et renvoi de l'indice de grille et de la pos
	curCell.x<0|| curCell.x>=IMG->_nGameSize?curCell.x=curCell.y:curCell.x=curCell.x;
	curCell.y = (int)curDir;
	_endCell = curCell;

   if (*(IMG->_vPair[_startCell.y][_startCell.x]) != *(IMG->_vPair[_endCell.y][_endCell.x])) {
		_bPaired = false;
   } else {
   	_bPaired = true;
   }

	if (_cDisplayMod.x >= 0) {
		XDELETE(_lpDisplayMod);
		_lpDisplayMod = new KCounter();
		_lpDisplayMod->startCounter(0,1,0,2000,K_COUNTER_LINEAR);
	}
	return curCell;
}

bool Path::NextStep(Coord *cell, EMiniGame::Direction *to, KGraphic **imgInd) {
	using namespace EMiniGame;

	Coord t = EMiniGame::GetDirVector(*to);
	*cell += t;		// Saut a la cellule suivante
	EMiniGame::Direction from = (EMiniGame::Direction)((*to+2)%4);
	
	if (cell->x<0 || cell->x>=IMG->_vGame[0].size() || cell->y<0 || cell->y>=IMG->_vGame.size()) {
			return false; // Fin du chemin
	}

	if(IMG->_vGame[cell->y][cell->x] == 1) {
		*to = (Direction)((from + 4 + (from%2==0?-1:1)) % 4);
		(int)(((*to + 1) % 4)/2) == 0?*imgInd = _vBeamImg[0]:*imgInd = _vBeamImg[1];
	} else if (IMG->_vGame[cell->y][cell->x] == 2) {
		*to = (Direction)(from - (from%2==0?-1:1));
		(int)(*to/2) == 0?*imgInd = _vBeamImg[2]:*imgInd = _vBeamImg[3];	
	} else {
		(int)(*to)%2 == 0?*imgInd = _vBeamImg[4]:*imgInd = _vBeamImg[5];
	}

	return true;
}

void Path::DrawPaired() {
	Coord pos = Coord((int)_startCell.y%2 == 0?0:_startCell.x,(int)_startCell.y%2==0?_startCell.x:0);
	if (!_bPaired) {
		IMG->_vGridPair[_startCell.y]->FillCell(IMG->_lpImgMiror1,pos.x,pos.y,1,0,0,0.3);
		IMG->_vGridPair[_endCell.y]->FillCell(IMG->_lpImgMiror1,(int)_endCell.y%2==0?0:_endCell.x,(int)_endCell.y%2==0?_endCell.x:0,1,0,0,0.3);
	} else {
		IMG->_vGridPair[_startCell.y]->FillCell(IMG->_lpImgMiror1,pos.x,pos.y,0,1,0,0.3);
		IMG->_vGridPair[_endCell.y]->FillCell(IMG->_lpImgMiror1,(int)_endCell.y%2==0?0:_endCell.x,(int)_endCell.y%2==0?_endCell.x:0,0,1,0,0.3);
	}
}
void Path::DrawPath(bool single) {
	if (!single && (_endCell.x < _startCell.x || (_endCell.x == _startCell.x && _endCell.y < _startCell.y)))
		return;
	Coord pos;

	ListPath::iterator it = _cells.begin();
	for (it; it != _cells.end();it++) {
		pos = (*it).cell;
		if (_nDisplayPath > 0)
			(*it).imgInd->setBlitColor(1,1,0,1);
		IMG->_lpGridGame->FillCell((*it).imgInd,pos.x,pos.y,false,1);
		if (_nDisplayPath > 0)
			(*it).imgInd->setBlitColor(1,1,1,1);
	};

	_nDisplayPath++;
}
void Path::ResolvePaths() {
	KAssert(IMG != NULL);

	for (int i=0; i<IMG->_vPaths.size(); i++) {
		for (int j=0; j<IMG->_vPaths[0].size(); j++) {
			IMG->_vPaths[i][j]->ResolvePath();
		}
	}
}
bool Path::GoOver(Coord c) {
	if (c.x < 0)
		return false;

	ListPath::iterator it = _cells.begin();
	for (it;it != _cells.end();it++) {
		if (it->cell == c)
			return true;
	}
	return false;
};
void Path::Draw() {
	KAssert(IMG != NULL);
	_nDisplayPath = 0;
	Coord pos;

	for (int i=0; i<IMG->_vPaths.size(); i++) {
		for (int j=0; j<IMG->_vPaths[0].size(); j++) {
			IMG->_vPaths[i][j]->DrawPaired();
		}
	}

	if (_lpDisplayMod != NULL) {
		double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();

		_lpDisplayMod->move(fElapsed);

		for (int i=0; i<IMG->_vPaths.size(); i++) {
			for (int j=0; j<IMG->_vPaths[0].size(); j++) {
				if (IMG->_vPaths[i][j]->GoOver(_cDisplayMod))
					IMG->_vPaths[i][j]->DrawPath();
			}
		}

		if (_lpDisplayMod->isCompleted()) {
			XDELETE(_lpDisplayMod);
			_cDisplayMod = Coord(-1,-1);
		}
	} else {		
		if (IMG->_lpGrabedPart != NULL) 
			return;
		pos = Coord(-1,-1);

		for (int i=0; i<IMG->_vPaths.size(); i++) {
			pos = IMG->_vGridPair[i]->GetMouseSelection();
			if (pos.y > 0)
				pos.x = pos.y;
			if (pos.x >=0) {
				pos.y = i;
				break;
			}
		}
		if (pos.x >= 0) {
			IMG->_vPaths[pos.y][pos.x]->DrawPath(true);
		}
	}
}

// Preload images et sons
void EMiniJeuMirorBeam::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_mirorsgrid_bg","jpg",true,true);
   EImageBank::getImage("mg_mirorsgrid_grid","png",true,true);
   EImageBank::getImage("mg_mirorsgrid_sel_bg","png",true,true);
   EImageBank::getImage("minigame_middleage_wrong","png",true,true);
   EImageBank::getImage("mg_mirorsgrid_miror1","png",true,true);
   EImageBank::getImage("mg_mirorsgrid_miror2","png",true,true);
   EImageBank::getImage("mg_mirorsgrid_T1","png",true,true);
   EImageBank::getImage("mg_mirorsgrid_T2","png",true,true);
   EImageBank::getImage("mg_mirorsgrid_T3","png",true,true);
   EImageBank::getImage("mg_mirorsgrid_T4","png",true,true);
   EImageBank::getImage("mg_mirorsgrid_S1","png",true,true);
   EImageBank::getImage("mg_mirorsgrid_S2","png",true,true);
}
