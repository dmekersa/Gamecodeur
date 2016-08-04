#include "KRandom.h"
#include "MyGame.h"
#include "SoundBank.h"
#include "ImageBank.h"
#include "FontBank.h"
#include "EPointer.h"
#include "EGeometry.h"

#include "EMiniJeuConnect.h"

#define PATH_RED 0
#define PATH_BLUE 1
#define PATH_GREEN 2

EMiniJeuConnect::EMiniJeuConnect(void)
{
	SetTextsTrad("CONNECTMG");
	SetGameType(MINIJEU_DIRECTGAME);
	SetFeatures(MINIJEU_LOGIC);
	SetBackGround(EImageBank::getImage("econnect_bg.jpg"));
	SetGameNumber(18);

	_nGameSizeX=9;
	_nGameSizeY=5;

	EConnectPart *_lpBBT = new EConnectPart("BBBB","EConnectBBT.png");
	EConnectPart *_lpRRT = new EConnectPart("RRRR","EConnectRRT.png");
	EConnectPart *_lpRBS = new EConnectPart("RBRB","EConnectBRS.png",false);
	EConnectPart *_lpBRT = new EConnectPart("BBRR","EConnectBRT.png");
    EConnectPart *_lpRRS = new EConnectPart("RRRR","EConnectRRS.png",false);
	EConnectPart *_lpBBS = new EConnectPart("BBBB","EConnectBBS.png",false);

	// Preparation des Grilles
	_lpGridConnect=new Grid(_nGameSizeX,_nGameSizeY,101,101,0,0,GRID_QUINCONCE_P);
	_lpGridConnect->Move((_nW-_lpGridConnect->width())/2,((_nH-_lpGridConnect->height()))/2 - 30);
	_lpGridConnect->SetQDeltaSelection(_lpBBT->Width()/2);

	_lpGame = std::vector< std::vector< EConnectPart * > > (_nGameSizeY,
		std::vector< EConnectPart * > (_nGameSizeX, (EConnectPart *) NULL));

	// Red Path
	_lpGame[0][2] = new EConnectPart(_lpRBS); 	_lpGame[0][2]->SetSol(1); 
	_lpGame[0][3] = new EConnectPart(_lpBRT); 	_lpGame[0][3]->SetSol(2); 
	_lpGame[0][4] = new EConnectPart(_lpBRT); 	_lpGame[0][4]->SetSol(0); 
	_lpGame[0][5] = new EConnectPart(_lpBRT); 	_lpGame[0][5]->SetSol(3); 
	_lpGame[1][4] = new EConnectPart(_lpRBS); 	_lpGame[1][4]->SetSol(0); 
	_lpGame[1][3] = new EConnectPart(_lpRRS); 	_lpGame[1][3]->SetSol(0); 
	_lpGame[2][2] = new EConnectPart(_lpBRT); 	_lpGame[2][2]->SetSol(1); 
	_lpGame[2][3] = new EConnectPart(_lpBRT); 	_lpGame[2][3]->SetSol(2); 
	_lpGame[2][4] = new EConnectPart(_lpRRT); 	_lpGame[2][4]->SetSol(1); 
	_lpGame[1][2] = new EConnectPart(_lpRBS); 	_lpGame[1][2]->SetSol(1); 
	_lpGame[0][1] = new EConnectPart(_lpBRT); 	_lpGame[0][1]->SetSol(0); 
	_lpGame[1][0] = new EConnectPart(_lpBRT); 	_lpGame[1][0]->SetSol(1); 
	_lpGame[1][1] = new EConnectPart(_lpBRT); 	_lpGame[1][1]->SetSol(3); 
	_lpGame[2][0] = new EConnectPart(_lpBRT); 	_lpGame[2][0]->SetSol(1); 
	_lpGame[2][1] = new EConnectPart(_lpRBS); 	_lpGame[2][1]->SetSol(1); 
	_lpGame[3][2] = new EConnectPart(_lpRBS); 	_lpGame[3][2]->SetSol(1); 
	_lpGame[3][4] = new EConnectPart(_lpBRT); 	_lpGame[3][4]->SetSol(0); 
	_lpGame[3][5] = new EConnectPart(_lpRRS); 	_lpGame[3][5]->SetSol(0); 
	_lpGame[4][6] = new EConnectPart(_lpBRT); 	_lpGame[4][6]->SetSol(2); 
	_lpGame[3][7] = new EConnectPart(_lpRBS); 	_lpGame[3][7]->SetSol(0); 
	_lpGame[3][8] = new EConnectPart(_lpBRT); 	_lpGame[3][8]->SetSol(3); 
	_lpGame[2][7] = new EConnectPart(_lpBRT); 	_lpGame[2][7]->SetSol(1); 
	_lpGame[2][8] = new EConnectPart(_lpBRT); 	_lpGame[2][8]->SetSol(3); 
	_lpGame[1][7] = new EConnectPart(_lpBRT); 	_lpGame[1][7]->SetSol(0); 
	_lpGame[2][6] = new EConnectPart(_lpBRT); 	_lpGame[2][6]->SetSol(2); 
	_lpGame[1][5] = new EConnectPart(_lpBRT); 	_lpGame[1][5]->SetSol(0); 
	_lpGame[2][5] = new EConnectPart(_lpRBS); 	_lpGame[2][5]->SetSol(1); 
	_lpGame[3][6] = new EConnectPart(_lpBRT); 	_lpGame[3][6]->SetSol(3); 
	_lpGame[4][4] = new EConnectPart(_lpBRT); 	_lpGame[4][4]->SetSol(2); 
	_lpGame[3][3] = new EConnectPart(_lpRRS); 	_lpGame[3][3]->SetSol(0); 
	_lpGame[4][2] = new EConnectPart(_lpBRT); 	_lpGame[4][2]->SetSol(2); 
	_lpGame[3][1] = new EConnectPart(_lpBRT); 	_lpGame[3][1]->SetSol(0); 
	
	_lpGame[0][6] = new EConnectPart(_lpRBS); 	_lpGame[0][6]->SetSol(1);
	_lpGame[1][6] = new EConnectPart(_lpBBS); 	_lpGame[1][6]->SetSol(0); 
	_lpGame[0][7] = new EConnectPart(_lpBRT); 	_lpGame[0][7]->SetSol(2); 
	_lpGame[1][8] = new EConnectPart(_lpBRT); 	_lpGame[1][8]->SetSol(1); 

	_lpGame[3][0] = new EConnectPart(_lpBBT); 	_lpGame[3][0]->SetSol(1); 
	
	
	float inc=45;
	Coord c;
	for (int i=0; i <(int)_lpGame.size(); i++) {
		for (int j=0; j <(int)_lpGame[0].size(); j++) {
			if (_lpGridConnect->CheckQCell(j,i)) {	
				if(_lpGame[i][j] != NULL) {
					_lpGame[i][j]->SetBaseAngle(inc);
					c = _lpGridConnect->GetCellCenter(j,i);
					_lpGame[i][j]->Move(c.x - _lpBBS->Width()/2, c.y - _lpBBS->Height()/2);
					_lpGame[i][j]->SwitchDrag();

					_lpGame[i][j]->RotateToStep(EMiniGame::GetRandom(4));
				}
			}
		}
	}

	// Initialisation des chemins
	WirePoint::SetRefGrid(_lpGridConnect);
	WirePoint::SetRefGame(&_lpGame);
	EConnectPart *tmp;
	
	// Red wire
	c.x = 2; c.y = 0;
	tmp = _lpGame[(int)c.y][(int)c.x];
	_wire[0] = new WirePoint(PATH_RED,tmp,90+inc, (float)_nX, (float)_nY);
	c.x = 1; c.y = 3;
	tmp = _lpGame[(int)c.y][(int)c.x];
	_wire[1] = new WirePoint(PATH_RED,tmp,180+inc, (float)_nX, (float)_nY);
	
	// Green Wire
	c.x = 6; c.y = 0;
	tmp = _lpGame[(int)c.y][(int)c.x];
	_wire[2] = new WirePoint(PATH_BLUE,tmp,0+inc, (float)_nX, (float)_nY);
	c.x = 7; c.y = 3;
	tmp = _lpGame[(int)c.y][(int)c.x];
	_wire[3] = new WirePoint(PATH_BLUE,tmp,270+inc, (float)_nX, (float)_nY);
	
	_wire[0]->SetEndPoint(_wire[1]);
	_wire[0]->ResolvePath(); _wire[1]->ResolvePath(); 
	_wire[2]->SetEndPoint(_wire[3]);
	_wire[2]->ResolvePath(); _wire[3]->ResolvePath(); 
		
	_lpGrabedPiece= NULL;

	// Préparation de Target pour dessiner le mini jeu dedans...
	_lpImgBgSuccess = EImageBank::getImage("econnect_bg_resolved","jpg");
	_lpImgArena = EImageBank::getImage("econnect_arena","png");
	_lpImgArenaSuccess = EImageBank::getImage("econnect_arena_resolved","png");

	_posArena = Coord(242,85);
	_posArenaSuccess = Coord(225,68);

	_bRedLink = false;
	_bBlueLink = false;

	_lpSuccessTimeout = NULL;

	XDELETE(_lpBBT);
	XDELETE(_lpRRT);
	XDELETE(_lpRBS);
	XDELETE(_lpBRT);
    XDELETE(_lpRRS);
	XDELETE(_lpBBS);

	Reset();
}

EMiniJeuConnect::~EMiniJeuConnect(void)
{
	XDELETE(_lpGrabedPiece);
   XDELETE(_lpSuccessTimeout);
	
	for (int i=0;i<(int)_lpGame.size();i++) {
		for (int j=0;j<(int)_lpGame[0].size();j++) {
			XDELETE(_lpGame[i][j]);
		}
	}
	XDELETE(_lpGridConnect);
	
	if (_lpGrabedPiece)
		XDELETE(_lpGrabedPiece);

	for (int i=0;i<_nWires*2;i++)
		XDELETE(_wire[i]);
}

/** 3 steps rendering **/
void EMiniJeuConnect::GameCheck ()
{
	EConnectPart* tmpPart = NULL;
	struct Coord grid;

	if (Mouse.LeftEvent() && _lpGridConnect->IsMouseOver()) {
		grid = _lpGridConnect->GetMouseSelection();
		if (grid.x >=0 && (_lpGame[(int)grid.y][(int)grid.x] != NULL && !_lpGame[(int)grid.y][(int)grid.x]->Fixed())) {
			Mouse.LeftEventClose();
			tmpPart = _lpGame[(int)grid.y][(int)grid.x];
			
			if (!tmpPart->Fixed()) {
				tmpPart->RotateCW(true);
				ESoundBank::getSound("mg_rotate")->playSample();
				IsResolved();
			}
		}
	}
}

void EMiniJeuConnect::GameLogic ()
{
}

void EMiniJeuConnect::GameDrawSet ()
{
	KGraphic *tmp;
	EConnectPart* cpiece=NULL;

	if (_lpSuccessTimeout != NULL) {
		_lpSuccessTimeout->move(MyGame::getGame()->getElapsedTime());
		_lpImgBgSuccess->blitAlphaRectFx(0,0,_lpImgBgSuccess->getWidth(), _lpImgBgSuccess->getHeight(),0,0,0,1,_lpSuccessTimeout->getCurrentValue());
	}
	
	for (int i=0;i<(int)_lpGame.size();i++) {
		for (int j=0;j<(int)_lpGame[0].size();j++) {
			if (! _lpGridConnect->CheckQCell(j,i))
				continue;
			cpiece=_lpGame[i][j];
			if (cpiece)	{
				if (cpiece->Fixed()) {
					tmp = cpiece->GetImg();
					tmp->setBlitColor(1,0.5,1,1);
					cpiece->Draw();
					tmp->setBlitColor(1,1,1,1);
				} else {
					cpiece->Draw();
				}
			}
		}
	}

	_lpImgArena->blitAlphaRectF(0,0,_lpImgArena->getWidth(),_lpImgArena->getHeight(),_posArena.x,_posArena.y);
	if (_lpSuccessTimeout != NULL) {
		_lpImgArenaSuccess->blitAlphaRectFx(0,0,_lpImgArenaSuccess->getWidth(),_lpImgArenaSuccess->getHeight(),_posArenaSuccess.x,_posArenaSuccess.y,0,1,_lpSuccessTimeout->getCurrentValue());
	}
	DrawSpark();
	
	IsResolved();
}

// Retourne true si le mini jeu est rÈsolu
bool EMiniJeuConnect::GameIsResolve()
{
	if (_bRedLink != (_wire[0]->ResolvePath() || _wire[1]->ResolvePath())) {
		_bRedLink= ! _bRedLink;
		
		if (_bRedLink) {
			//MyGame::showFileError ("Red Link OK");
			//sound good
			ESoundBank::getSound("mg_good")->playSample();
		} else {
			//MyGame::showFileError ("Broken Red Link");
			ESoundBank::getSound("mg_disparition")->playSample();
		}
	}
	if (_bBlueLink != (_wire[2]->ResolvePath() || _wire[3]->ResolvePath())) {
			_bBlueLink = ! _bBlueLink;
			if (_bBlueLink) {
				//MyGame::showFileError ("Green Link OK");
				//sound good
				ESoundBank::getSound("mg_good")->playSample();
			} else {
				//MyGame::showFileError ("Broken Green Link ");
				ESoundBank::getSound("mg_disparition")->playSample();
			}
	}
	if (_bRedLink && _bBlueLink) { //&&_bBlueLink) {
		for (int i=0;i<_nWires*2;i++)
			_wire[i]->StopSpark();
		SetBackGround(_lpImgBgSuccess);
		_lpSuccessTimeout = new KCounter();
		_lpSuccessTimeout->startCounter(0,1,0,500,K_COUNTER_LINEAR);
		return true;
	}
	return false;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuConnect::GameResolve()
{
	for (int j=0; j<_nGameSizeY; j++) {
		for (int i= 0; i< _nGameSizeX; i++) {
			if (_lpGridConnect->CheckQCell(i,j) && _lpGame[j][i] != NULL)
				_lpGame[j][i]->FixSol();
		}
	}
	IsResolved();
}

// Active un niveau d'indices (_nGameStatus = 1, 2 ou 3)
void EMiniJeuConnect::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	case MINIJEU_HINT1:
		ShowHint(MINIJEU_HINT1);
		for (int i= 0; i< _nGameSizeX; i++) {
			if (_lpGridConnect->CheckQCell(i,0) && _lpGame[0][i] != NULL)
				_lpGame[0][i]->FixSol();
		}
		Reset();
		break;
	case MINIJEU_HINT2:
		ShowHint(MINIJEU_HINT2);
		for (int j=1; j<3; j++) {
			for (int i= 0; i< _nGameSizeX; i++) {
				if (_lpGridConnect->CheckQCell(i,j) && _lpGame[j][i] != NULL)
					_lpGame[j][i]->FixSol();
			}
		}
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

void EMiniJeuConnect::DrawSpark() {
	if (_nGameStatus == MINIJEU_RESOLVED)
		return;

	for (int i=0; i<_nWires*2; i++)
		_wire[i]->DrawSpark();
}

void EMiniJeuConnect::GameReset() {
	for (int i=0; i <(int)_lpGame.size(); i++) {
		for (int j=0; j <(int)_lpGame[0].size(); j++) {
			if (_lpGridConnect->CheckQCell(j,i) && _lpGame[i][j] != NULL)
				_lpGame[i][j]->RotateToStep(EMiniGame::GetRandom(4));
		}
	}
}

/* ___________________________________ */

EConnectPart::EConnectPart(char *color, const char* ImgPath, bool turn, int nPos): EMiniJeuPart(ImgPath,nPos) {
	SetRotationDuration(0.5);
	if (nPos!=4)
		SetAngleStep(90);
	_bStraight=true;

	memcpy(_Color,color,sizeof(char [4]));
	_Color[4]='\0';
	for (char *c=_Color; c<&_Color[4]; c++)
		KAssert(*c=='R' || *c=='B' || *c=='G');

	// Utilisation de Coord en tant que tuple Sortie, Couleur
	if (turn) {
		_Link[0].x=1;
		_Link[1].x=0;
		_Link[2].x=3;
		_Link[3].x=2;
		_bStraight=true;
	} else {
		_Link[0].x=2; 
		_Link[1].x=3;
		_Link[2].x=0;
		_Link[3].x=1;
	}
	for (int i=0; i< nPos; i++) {
		switch(_Color[i]) {
			case 'R':
				_Link[i].y = PATH_RED;
				break;
			case 'B':
				_Link[i].y = PATH_BLUE;
				break;
			case 'G':
				_Link[i].y = PATH_GREEN;
				break;
			default:
				KAssert(false);
		}
	}
	SetRotationDuration(300);
	_nSolAngle = 0;
	_bSolved = false;
}

EConnectPart::EConnectPart(const EConnectPart* a, bool rotate): EMiniJeuPart(a) {
	
	memcpy(_Color,a->_Color,sizeof(char [5]));
	memcpy(_Link,a->_Link,sizeof(Coord [4]));
	_bStraight=a->_bStraight;
	
	SetRotationDuration(300);
	if (rotate)
		RotateCW(false);
	_nSolAngle = 0;
	_bSolved = false;
}

void EConnectPart::RotateCW(bool bAnim) {
	/*if (_bSolved)
		return;*/

	Coord tmp = _Link[0];
	for (int i=0;i<_nSide-1;i++) {
		_Link[i] = _Link[i+1];
		}
	_Link[_nSide-1] = tmp;
	
	for (int i=0;i<_nSide ;i++) {
		_Link[i].x = (float)((int)(_Link[i].x - 1 + _nSide) % _nSide);
	}
	EMiniJeuPart::RotateCW(bAnim);
};
void EConnectPart::RotateACW(bool bAnim) {
	/*if (_bSolved)
		return;*/

	Coord tmp = _Link[_nSide-1];
	for (int i=_nSide-1;i>0;i--) {
		_Link[i] = _Link[i-1];
		}
	_Link[0] = tmp;
	
	for (int i=0;i<_nSide ;i++) {
		_Link[i].x = (float)((int)(_Link[i].x + 1 + _nSide) % _nSide);
	}
	EMiniJeuPart::RotateACW(bAnim);
};
void EConnectPart::RotateToStep(int nstep) {
	if (_bSolved)
		return;

	int tmp = (nstep - 4) % 4;
	if (_nCurrentStep <= 0) {
		if (_nCurrentStep < tmp) {
			EMiniJeuPart::RotateACW(tmp-_nCurrentStep);
		} else if (_nCurrentStep > tmp) {
			EMiniJeuPart::RotateCW(_nCurrentStep-tmp);
		}
	} else {
		tmp = (tmp +4)%4;
		if (_nCurrentStep < tmp) {
			EMiniJeuPart::RotateACW(tmp-_nCurrentStep);
		} else if (_nCurrentStep > tmp) {
			EMiniJeuPart::RotateCW(_nCurrentStep-tmp);
		}
	}
	//while (_nCurrentStep != tmp)
	//	RotateCW(false);
}
bool EConnectPart::Connect(int color, int to) {
	int ind = (int)((to-_fBaseAngle + 180)/_fAngleStep) %_nSide;  // Angle d'arrivÈ 
	
	if (_Link[ind].y != color)
		return false;
	return true;
}

bool EConnectPart::GetNext(int color, int *to, Coord *c) {
	int ind = (int)((*to+180-_fBaseAngle)/_fAngleStep) % _nSide;  // Angle d'arrivÈ 

	if (_Link[ind].y!=color)
		return false;

	// le retour es un angle de sortie de la piËce plus la couleur du fil
	*to = (int)(_Link[ind].x * _fAngleStep) + (int)_fBaseAngle;
	*c = Center();

	return true;
}

void EConnectPart::SetSol(int n) {
	_nSolAngle = (n - 4)%4;
}
void EConnectPart::FixSol() {
	SetRotationDuration(1500);
	RotateToStep(_nSolAngle);
	_bSolved = true;
}
/* ___________________________________ */

WirePoint::WirePoint(int c, EConnectPart *i, float sAngle, float nX, float nY) {
	KAssert(_lpGrid != NULL && _lpGame != NULL);
	_cColor=c;
	_lpImg=i;

	_cStart= _lpImg->Center();
	_cEnd=_lpImg->Center();

	_sDir=(float)(((int)sAngle + 360) %360);
	_eDir=_sDir;

	_bWired=false;

	_nX = nX;
	_nY = nY;

   if (c == PATH_BLUE)
      _lpSpark=EImageBank::getImage("ConnectSparkBlue");
   if (c == PATH_RED)
      _lpSpark=EImageBank::getImage("ConnectSparkRed");
   if (c == PATH_GREEN)
      _lpSpark=EImageBank::getImage("ConnectSparkGreen");

   /* Effet de particules */
//   pPyroEffect pyroe = PyroEffect_Find("spark_mini.pyro");
//   if (pyroe == NULL) {
//		Pyro_LoadDirectoryEffect("spark_mini.pyro",false);
//		pyroe = PyroEffect_Find("spark_mini.pyro");
//   }
//   _lpEmitter = PyroEffect_Add(pyroe, 0, 0, 1);   
}

WirePoint::~WirePoint(void) {
//      _lpEmitter->stopEmitting = true;
}

const Grid *WirePoint::_lpGrid;
void WirePoint::SetRefGrid(Grid *g) {
	_lpGrid = g;
}

const std::vector< std::vector< EConnectPart * > > *WirePoint::_lpGame;
void WirePoint::SetRefGame(std::vector< std::vector< EConnectPart * > > *g) {
	_lpGame = g;
}

bool WirePoint::ResolvePath (void) {
	int dir=(int)(_sDir-180 + 360)%360;
	Coord c=_lpGrid->GetSelection(_cStart);
	Coord oldc=_cStart;
	Coord nextCell;

	EConnectPart* tmpPart=_lpImg;

	KAssert(_lpEnd != NULL);

	_cEnd=_cStart; _eDir=_sDir;
	_bWired=false; _lpEnd->_bWired=false;

	while (tmpPart->GetNext(_cColor,&dir,&c)) {
		_eDir=(float)dir;
		// Calcul de la position du centre de la piËce suivante
		nextCell = EGeometry::GetSegmentEnd(c,_lpImg->Width(),_eDir);
		nextCell = _lpGrid->GetSelection(nextCell);


		if (oldc == _lpEnd->_cStart && _eDir == _lpEnd->_sDir) {
			_bWired=true;			_lpEnd->_bWired=true;
			_eDir=_lpEnd->_sDir;	_lpEnd->_eDir=_sDir;
			_lpEnd->_cEnd=_cStart;	_cEnd=_lpEnd->_cStart;
			break;
		}	

		// VÈrification de l'existance de la piËce suivante
		if (!_lpGrid->CheckQCell(nextCell) || (*_lpGame)[(int)nextCell.y][(int)nextCell.x] == NULL ){
			// Test si la "sortie de jeu correspond a l'extrÈmitÈ du fil
			break;

		}
		if ( ! (*_lpGame)[(int)nextCell.y][(int)nextCell.x]->Connect(_cColor,(int)_eDir)) {
			_cEnd=oldc;
			break;
		}
		tmpPart=(*_lpGame)[(int)nextCell.y][(int)nextCell.x];

		_cEnd= tmpPart->Center();
		oldc=_cEnd;
		c = nextCell;
	}
	return _bWired;
}

void WirePoint::DrawSpark() {
//	Coord c, sSpark;
//	float dx,dy;
//
//	Coord delta;
//
//	int col= _lpGrid->GetQColSize((int)_lpGrid->GetSelection(_cEnd).y);
//	int line= _lpGrid->GetQLineSize((int)_lpGrid->GetSelection(_cEnd).x);
//
//	if (_cEnd == _lpEnd->_cStart && _eDir == _lpEnd->_sDir) { //_cEnd.x != _cStart.x || _cEnd.y != _cStart.y) {
////		_lpEmitter->stopEmitting = true;
//		return;
//	}
//	
//	sSpark = Coord (_lpSpark->getWidth(), _lpSpark->getHeight());
//	c = _lpGrid->GetSelection(_cEnd);
//	c = (*_lpGame)[(int)c.y][(int)c.x]->Center();
//	delta = EGeometry::GetSegmentEnd(_lpImg->Width()/2,_eDir);
//	c = EGeometry::GetSegmentEnd(c,_lpImg->Width()/2,_eDir);
//   
////   _lpEmitter->stopEmitting = false;
////   _lpEmitter->x = c.x;
////   _lpEmitter->y = c.y;
//
//   dx=_lpSpark->getWidth();
//   dy=_lpSpark->getHeight();
//   _lpSpark->blitAlphaRect(0,0,dx,dy,c.x -(dx/2),c.y -(dy/2));
}


// Preload images et sons
void EMiniJeuConnect::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("econnect_bg.jpg","",true,true);
   EImageBank::getImage("econnect_bg_resolved","jpg",true,true);
   EImageBank::getImage("econnect_arena","png",true,true);
   EImageBank::getImage("econnect_arena_resolved","png",true,true);
   EImageBank::getImage("ConnectSparkBlue","",true,true);
   EImageBank::getImage("ConnectSparkRed","",true,true);
   EImageBank::getImage("ConnectSparkGreen","",true,true);

   ESoundBank::getSound("mg_rotate",false,true);
   ESoundBank::getSound("mg_good",false,true);
   ESoundBank::getSound("mg_disparition",false,true);
   ESoundBank::getSound("mg_good",false,true);
   ESoundBank::getSound("mg_disparition",false,true);
}
