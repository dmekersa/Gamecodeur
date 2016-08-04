#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"

#include "EMiniJeuGardening.h"

using EMiniGame::GetRandom;

EMiniJeuGardening::EMiniJeuGardening(void)
{
	SetTextsTrad("GARDENING");
	SetGameType(MINIJEU_SUBMITGAME);
	SetGameNumber(22);
	SetExitOnFail(true);
	SetBackGround(EImageBank::getImage("minigame_middleage_bg","jpg"));

	_lpImgWrong = EImageBank::getImage("minigame_middleage_wrong","png");

	_vPartSet.push_back(new EMiniJeuPart(EImageBank::getImage("minigame_middleage_symb01","png"),1));
	_vPartSet.push_back(new EMiniJeuPart(EImageBank::getImage("minigame_middleage_symb02","png"),1));
	_vPartSet.push_back(new EMiniJeuPart(EImageBank::getImage("minigame_middleage_symb03","png"),1));
	_vPartSet.push_back(new EMiniJeuPart(EImageBank::getImage("minigame_middleage_symb04","png"),1));
	_vPartSet.push_back(new EMiniJeuPart(EImageBank::getImage("minigame_middleage_symb05","png"),1));

	_lpGridGame = new Grid(_nGameSize,_nGameSize,_vPartSet[1]->Width()+17,_vPartSet[1]->Height()+17,0,0);
	_lpGridPartSet = new Grid(1,_nGameSize,_vPartSet[1]->Width(),_vPartSet[1]->Height(),0,17);

	//_lpGridPartSet->Move((_nW - (_lpGridPartSet->width() + _lpGridGame->width() + 40)) /2, (_nH -200 - _lpGridGame->height()) /2);
	//_lpGridGame->Move(_lpGridPartSet->_xPos + _lpGridPartSet->width() + 40, _lpGridPartSet->_yPos);
	_lpGridGame->Move((_nW - _lpGridGame->width()) /2 +30, (_nH -200 - _lpGridGame->height()) /2 +37);
	_lpGridPartSet->Move(_lpGridGame->_xPos - 150, _lpGridGame->_yPos);
	//_lpGridPartSet->Move(16,5);
	//_lpGridGame->Move(140, _lpGridPartSet->_yPos);


	_vGame = std::vector< std::vector< int > >(_nGameSize, std::vector< int >(_nGameSize, -1));
	_vBaseGame = std::vector< std::vector< int > >(_nGameSize, std::vector< int >(_nGameSize, -1));
	_vSolution = std::vector< std::vector< int > >(_nGameSize, std::vector< int >(_nGameSize, -1));

	_vNumPart = std::vector< int >(_nGameSize,0);

	_lpGrabedPart = NULL;
	ResetPartSet();

	_nMinClue = 0;
	_nMaxClue = 7;

	GenerateGrid();
	Reset();
}

EMiniJeuGardening::~EMiniJeuGardening(void)
{
	for (int i=0; i<(int)_vPartSet.size(); i++) {
		XDELETE(_vPartSet[i]);
	}
	XDELETE(_lpGridGame);
	XDELETE(_lpGridPartSet);
}

/** 3 steps rendering **/
void EMiniJeuGardening::GameCheck ()
{
	Coord pos;
	int tmpIndex;

	if (!Mouse.LeftEvent())
		return;

	if (_lpGrabedPart != NULL) {
		Mouse.LeftEventClose();

		tmpIndex = -1;
		pos = _lpGridGame->GetMouseSelection();
		
		if (_lpGridGame->IsMouseOver()) {
			// Au dessus d'une piËce fixe
			if (_vBaseGame[pos.y][pos.x]>=0) {
				ESoundBank::getSound("mg_forbid")->playSample();
				return;
			}

			// Mise a jour de la cellule
			tmpIndex = _vGame[pos.y][pos.x];
			_vGame[pos.y][pos.x] = IndexPart(_lpGrabedPart);

			// skip du swap si les piËces sont identique
			if (tmpIndex >= 0 && tmpIndex == _vGame[pos.y][pos.x] ) {
				Mouse.LeftEventClose();
				return;
			}
		} else {
			_vNumPart[IndexPart(_lpGrabedPart)]++;
			ESoundBank::getSound("mg_select_deselect")->playSample();
		}

		// DÈpose de la piËces
		ResetPartSet();
		_lpGrabedPart = NULL;

		// Gestion du swap de piËces
		if (tmpIndex >= 0) {
			_vPartSet[tmpIndex]->Grab();
			_lpGrabedPart = _vPartSet[tmpIndex];
			_lpGrabedPart->Show();
			ESoundBank::getSound("mg_swap")->playSample();
		} else {
			ESoundBank::getSound("mg_select_deselect")->playSample();
		}

		return;
	}

	// Selection depuis le Set de piËces
	if (_lpGridPartSet->IsMouseOver()) {
		Mouse.LeftEventClose();

		pos = _lpGridPartSet->GetMouseSelection();
		if (_vNumPart[pos.y] == 0) {
			return;
		}
		_lpGrabedPart = _vPartSet[pos.y];
		_lpGrabedPart->Grab();
		_vNumPart[pos.y]--;
		ESoundBank::getSound("mg_select_deselect")->playSample();
	}

	// Selection depuis la grille de jeux
	if (_lpGridGame->IsMouseOver()) {
		Mouse.LeftEventClose();

		pos = _lpGridGame->GetMouseSelection();

		// Selection de piËce fixe ou d'une cellule vide
		if (_vBaseGame[pos.y][pos.x]>=0 || _vGame[pos.y][pos.x]<0) {
			if (_vBaseGame[pos.y][pos.x]>=0) {
				ESoundBank::getSound("mg_forbid")->playSample();
			}
			return;
		}
		
		_lpGrabedPart = _vPartSet[_vGame[pos.y][pos.x]];
		_lpGrabedPart->Grab();
		_lpGrabedPart->Show();
		_vGame[pos.y][pos.x] = -1;
		ESoundBank::getSound("mg_select_deselect")->playSample();
	}
}

void EMiniJeuGardening::GameLogic ()
{
	if (_lpGrabedPart != NULL)
		_lpGrabedPart->Logic();
}

void EMiniJeuGardening::GameDrawSet()
{
	Coord pos;

	//_lpGridPartSet->DrawBaseGrid(_lpImgBg);
	//_lpGridGame->DrawBaseGrid(_lpImgBg);

	for (int i=0; i<(int)_vPartSet.size(); i++) {
		_vPartSet[i]->Draw();
	}

	for (int i=0; i<(int)_vGame.size(); i++) {
		for (int j=0; j<(int)_vGame[1].size(); j++) {
			if (_vGame[i][j] >= 0) {
				pos = _lpGridGame->GetCellPos(j,i);
				//_vPartSet[_vGame[i][j]]->GetImg()->blitAlphaRectFx(0,0,_lpGridGame->GetCellSize().x,_lpGridGame->GetCellSize().y,pos.x,pos.y,0,1,1);
				_lpGridGame->FillCell(_vPartSet[_vGame[i][j]]->GetImg(),j,i,false,1);
				if (_nHintLevel == MINIJEU_HINT2 &&  _vGame[i][j] != _vSolution[i][j])
					_lpGridGame->FillCell(_lpImgWrong,j,i,false,1);
			}
			if (_vBaseGame[i][j] >= 0) {
				_lpGridGame->FillCell(_lpImgWrong,j,i,0,0,0,0.40f);
			}
		}
	}
}

void EMiniJeuGardening::GameDrawGrab() {
	if (_lpGrabedPart != NULL)
		_lpGrabedPart->Draw();
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuGardening::GameIsResolve()
{
	// Check si le tableau es complet
	for (int i=0; i<(int)_vGame.size(); i++) {
		for (int j=0; j<(int)_vGame[1].size(); j++) {
			if (_vGame[i][j] < 0 )
				return false;
			if (!CheckCellValue(j,i))
				return false;
		}
	}
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuGardening::GameResolve()
{
	Solve();
	for (int i=0; i<(int)_vNumPart.size(); i++) {
		_vNumPart[i]=0;
		_vPartSet[i]->FadOut(500);
	}
}

void EMiniJeuGardening::GameReset()
{
	_vNumPart = std::vector< int >(_nGameSize,_nGameSize);

	for (int i=0; i<(int)_vNumPart.size(); i++)
		if (_vNumPart[i] > 0)
		   _vPartSet[i]->FadIn(500);

	for (int i=0; i<(int)_vGame.size(); i++) {
		for (int j=0; j<(int)_vGame[1].size(); j++) {
			_vGame[i][j] = _vBaseGame[i][j];
			if (_vGame[i][j]>=0)
				_vNumPart[_vGame[i][j]]--;
		}
	}

	_lpGrabedPart = NULL;
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuGardening::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  _nMinClue = 9;
		  _nMaxClue = 10;
		  ShowHint(MINIJEU_HINT1);
		  GenerateGrid();
		  Reset(); 
		  break;
	  case MINIJEU_HINT2:
		  ShowHint(MINIJEU_HINT2);
		  GenerateGrid();
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

//void EMiniJeuGardening::GameInit() {
//	//_vBaseGame[0][1] = 0;
//	//_vBaseGame[0][2] = 1;
//	//_vBaseGame[0][4] = 2;
//	//_vBaseGame[3][0] = 4;
//	//_vBaseGame[3][4] = 3;
//	//_vBaseGame[4][1] = 4;
//	////
//	//_vBaseGame[1][3] = 3;
//	//_vBaseGame[2][3] = 2;
//}

bool EMiniJeuGardening::CheckCellValue(int col, int line, int value){
	for (int i=0; i<_nGameSize; i++) {
		if (_vGame[line][i] == value)
			return false;
		if (_vGame[i][col] == value)
			return false;
	}
	return true;
}

bool EMiniJeuGardening::CheckCellValue(int col, int line){
	int tmpVal = _vGame[line][col];
	bool res;

	_vGame[line][col] = -1;
	res = CheckCellValue(col, line, tmpVal);
	_vGame[line][col] = tmpVal;

	return res;
}

void EMiniJeuGardening::ResetPartSet() {
	for (int i=0; i<(int)_vPartSet.size(); i++) {
		_vPartSet[i]->Move(_lpGridPartSet->GetCellPos(0,i));
		if (_vPartSet[i] == _lpGrabedPart) {
			_vPartSet[i]->Release();
			_vPartSet[i]->Hide();
			if (_vNumPart[i] >0) {
				_vPartSet[i]->FadIn(500);
			}
		}
	}
}

int EMiniJeuGardening::IndexPart(EMiniJeuPart *part) {
	for (int i=0; i<(int)_vPartSet.size(); i++) {
		if (_vPartSet[i] == part) 
			return i;
	}
	return -1;
}

void EMiniJeuGardening::LockCell(int col, int line, int value, bool lock) {
	if (value < 0)
		return;
	_vColLock[col][value] = lock;
	_vLineLock[line][value] = lock;
}

bool EMiniJeuGardening::CheckLockCell(int col, int line, int value) {
	return _vLineLock[line][value] || _vColLock[col][value];
}

void EMiniJeuGardening::Solve() {
	Coord pos;

	int nCurrentCell = 0;
	_bBackTrack = false;

	int nVal = _vPartSet.size();
	int val;

	_vColLock = std::vector< std::vector< bool > >( _nGameSize, std::vector< bool >( _vPartSet.size(), false)); 
	_vLineLock = std::vector< std::vector< bool > >( _nGameSize, std::vector< bool >( _vPartSet.size(), false));

	// Check si le tableau es complet
	for (int i=0; i<(int)_vGame.size(); i++) {
		for (int j=0; j<(int)_vGame[1].size(); j++) {
				_vGame[i][j] = _vBaseGame[i][j];
				LockCell(j,i,_vGame[i][j]);
		}
	}

	while ( nCurrentCell < _nGameSize * _nGameSize && nCurrentCell >=0) {
		pos = Coord (nCurrentCell % _nGameSize, (int)(nCurrentCell / _nGameSize));

		if (_vBaseGame[pos.y][pos.x] >= 0) {
			if (_bBackTrack) {
				nCurrentCell--;
			} else {
				nCurrentCell++;
			}
			continue;
		}

		val = _vGame[pos.y][pos.x];
		UnLockCell(pos.x,pos.y,val);

		val++;
		while (val < nVal && CheckLockCell(pos.x,pos.y,val))
			val++;

		if (val >= nVal) {
			_bBackTrack = true;
			_vGame[pos.y][pos.x] = -1;
			nCurrentCell--;
		} else {
			_vGame[pos.y][pos.x] = val;
			LockCell(pos.x,pos.y,_vGame[pos.y][pos.x]);
			nCurrentCell++;
			_bBackTrack = false;
		}
	}
	for (int i=0; i<(int)_vGame.size(); i++) {
		for (int j=0; j<(int)_vGame[1].size(); j++) {
				_vSolution[i][j] = _vGame[i][j];
		}
	}
}
void EMiniJeuGardening::GenerateGrid() {
	Coord pos;
	int r,v, clue, nVal, nSol;
	_vBaseGame = std::vector< std::vector< int > >(_nGameSize, std::vector< int >(_nGameSize, -1));
	nVal = _vPartSet.size();

m0:	clue = -1;
	_vBaseGame = std::vector< std::vector< int > >(_nGameSize, std::vector< int >(_nGameSize, -1));
m1:	clue++;
	nSol = TestGrid(1);
	if (nSol == 0 )
		goto m0;
	if (nSol == 1) {
		if (clue > _nMaxClue)
			goto m0;
		Solve();
		while (clue < _nMinClue) {
			r = GetRandom(_nGameSize * _nGameSize);
			pos = Coord(r%_nGameSize,r/_nGameSize);
			if (_vBaseGame[pos.y][pos.x] < 0) {
				_vBaseGame[pos.y][pos.x] = _vGame[pos.y][pos.x];
				clue++;
			}
		} 
		return;
	}

m2:	r = GetRandom(_nGameSize * _nGameSize);
	pos = Coord(r%_nGameSize,r/_nGameSize);
	if (_vBaseGame[pos.y][pos.x] > 0)
		goto m2;
	v = GetRandom(nVal);
	if (CheckLockCell(pos.x,pos.y,v))
		goto m2;
	_vBaseGame[pos.y][pos.x] = v;
	_vGame[pos.y][pos.x] = v;
	LockCell(pos.x,pos.y,v);
	goto m1;
}
int EMiniJeuGardening::TestGrid(int nSolMax) {
	Coord pos;

	int nSol =0;
	int nCurrentCell = 0;
	_bBackTrack = false;

	int nVal = _vPartSet.size();
	int val;

	_vColLock = std::vector< std::vector< bool > >( _nGameSize, std::vector< bool >( _vPartSet.size(), false)); 
	_vLineLock = std::vector< std::vector< bool > >( _nGameSize, std::vector< bool >( _vPartSet.size(), false));

	// Check si le tableau es complet
	for (int i=0; i<(int)_vGame.size(); i++) {
		for (int j=0; j<(int)_vGame[1].size(); j++) {
				_vGame[i][j] = _vBaseGame[i][j];
				LockCell(j,i,_vGame[i][j]);
		}
	}

	while ( nCurrentCell >=0) {
		if (nCurrentCell == _nGameSize*_nGameSize) {
			_bBackTrack = true;
			nCurrentCell--;
			nSol++;
			if (nSol > nSolMax)
				goto t0;
			continue;
		}
		pos = Coord (nCurrentCell % _nGameSize, (int)(nCurrentCell / _nGameSize));

		if (_vBaseGame[pos.y][pos.x] >= 0) {
			if (_bBackTrack) {
				nCurrentCell--;
			} else {
				nCurrentCell++;
			}
			continue;
		}

		val = _vGame[pos.y][pos.x];
		UnLockCell(pos.x,pos.y,val);

		val++;
		while (val < nVal && CheckLockCell(pos.x,pos.y,val))
			val++;

		if (val >= nVal) {
			_bBackTrack = true;
			_vGame[pos.y][pos.x] = -1;
			nCurrentCell--;
		} else {
			_vGame[pos.y][pos.x] = val;
			LockCell(pos.x,pos.y,_vGame[pos.y][pos.x]);
			nCurrentCell++;
			_bBackTrack = false;
		}
	}

t0:	_vColLock = std::vector< std::vector< bool > >( _nGameSize, std::vector< bool >( _vPartSet.size(), false)); 
	_vLineLock = std::vector< std::vector< bool > >( _nGameSize, std::vector< bool >( _vPartSet.size(), false));

	// Check si le tableau es complet
	for (int i=0; i<(int)_vGame.size(); i++) {
		for (int j=0; j<(int)_vGame[1].size(); j++) {
				_vGame[i][j] = _vBaseGame[i][j];
				LockCell(j,i,_vGame[i][j]);
		}
	}
	return nSol;
}

// Preload images et sons
void EMiniJeuGardening::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("minigame_middleage_bg","jpg",true,true);
   EImageBank::getImage("minigame_middleage_wrong","png",true,true);
   EImageBank::getImage("minigame_middleage_symb01","png",true,true);
   EImageBank::getImage("minigame_middleage_symb02","png",true,true);
   EImageBank::getImage("minigame_middleage_symb03","png",true,true);
   EImageBank::getImage("minigame_middleage_symb04","png",true,true);
   EImageBank::getImage("minigame_middleage_symb05","png",true,true);
}
