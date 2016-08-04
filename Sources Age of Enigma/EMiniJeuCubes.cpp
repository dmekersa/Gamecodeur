#include "MyGame.h"
#include "ImageBank.h"
#include "EPointer.h"
#include "FontBank.h"

#include "EMiniJeuCubes.h"

using EMiniGame::GetRandom;

EMiniJeuCubes::EMiniJeuCubes(void)
{
	SetTextsTrad("CUBES");
	SetGameType(MINIJEU_SUBMITGAME);
	SetBackGround(EImageBank::getImage("mg_cubes_bg","jpg"));
	SetGameNumber(14);

	_vCubesPart.push_back(EImageBank::getImage("mg_cube_yellow","png"));
	_vCubesPart.push_back(EImageBank::getImage("mg_cube_blue","png"));
	_vCubesPart.push_back(EImageBank::getImage("mg_cube_green","png"));
	_vCubesPart.push_back(EImageBank::getImage("mg_cube_red","png"));

	_nGameSize = 4;

	memset(_lpQueryText, 0, sizeof(_lpQueryText));
	strcpy(_lpQueryText, KStr("QUERY_CUBES"));
	
	_cVector[0] = Coord(46,17);
	_cVector[1] = Coord(-54,17);
	_cVector[2] = Coord(0,-64);

	_nGameSize = _nCubesSize;

	_cOrigine = Coord(585,350);
	
	_lpFont = EFontBank::getFont("tahomacounter40-0");
	_lpFont->setColor(1,1,1,1);
	_Digit1 = new Digit(_lpFont,0.7f,32);
	_Digit2 = new Digit(_lpFont,0.7f,32);
	XDELETE(_lpFont);	

	_lpFont = EFontBank::getFont("tahoma.ttf",40);
	_lpFont->setColor(1,1,1,1);
   
	Reset();
}

EMiniJeuCubes::~EMiniJeuCubes(void)
{
   XDELETE(_Digit1);
   XDELETE(_Digit2);
   XDELETE(_lpFont);
}

/** 3 steps rendering **/
void EMiniJeuCubes::GameCheck ()
{
	_Digit1->Check();
	_Digit2->Check();
}

void EMiniJeuCubes::GameLogic ()
{
}

void EMiniJeuCubes::GameDrawSet()
{		
	Coord pos;
	KGraphic *tmp;

	Coord posZ, posY, posX;

	tmp = _lpImgCube1;
	posZ = _cFirstDice;
	for (int z=0; z < _vGame.size(); z++) {
		
		posY = posZ;
		for (int y=0; y< _vGame[0].size(); y++) {
			posX = posY;
			for (int x=0; x < _vGame[0][0].size(); x++) {
				if (_vGame[z][y][x])
					tmp->blitAlphaRect(0,0,tmp->getWidth(),tmp->getHeight(),posX.x,posX.y);

				posX.x = posX.x + _cVector[0].x;
				posX.y = posX.y + _cVector[0].y;
			}
			
			posY.x = posY.x + _cVector[1].x;
			posY.y = posY.y + _cVector[1].y;

			tmp == _lpImgCube1? tmp = _lpImgCube2: tmp = _lpImgCube1;
		}

		posZ.y = posZ.y + _cVector[2].y;
	}

	//_lpImgCube1->drawRect(_pBlendBox1.x,_pBlendBox1.y,_pBlendBox2.x,_pBlendBox2.y,0,0,0,0.5);
	_lpFont->drawMultiline(_lpQueryText,_pBlendBox1.x,_pBlendBox2.x,_pBlendBox1.y,TEXTSTYLE_CENTER);
	_Digit1->Draw();
	_Digit2->Draw();
}

void EMiniJeuCubes::GameReset() {
	_lpImgCube1 = _vCubesPart[GetRandom(_vCubesPart.size())];
	_lpImgCube2 = _lpImgCube1;
	while (_lpImgCube2 == _lpImgCube1)
		_lpImgCube2 = _vCubesPart[GetRandom(_vCubesPart.size())];

	_vGame = std::vector< std::vector< std::vector< int > > >(
		_nGameSize, std::vector< std::vector< int > >(
		_nGameSize, std::vector< int >(
		_nGameSize, 0)));

	_nResponse = 0;

	GenerateSol();

	float fSpacing = 15, _nMaxWidth = 500;
	float _fQueryWidth = _lpFont->getMultilineWidth(_lpQueryText,0,_nMaxWidth- (fSpacing + _Digit1->width()*2),0,TEXTSTYLE_LEFT);
	float _fQueryHeight = _lpFont->getMultilineHeight(_lpQueryText,0,_nMaxWidth- (fSpacing + _Digit1->width()*2),0,TEXTSTYLE_LEFT);

	_pBlendBox1.x = (_nW - (_fQueryWidth + fSpacing + _Digit1->width()*2))/2;
	_pBlendBox2.x = _pBlendBox1.x+_fQueryWidth;
	if (_fQueryHeight > _Digit1->FontHeight()) {
		_pBlendBox1.y = _maxPosCube.y; 
		_Digit1->Move(_pBlendBox2.x + fSpacing,_pBlendBox1.y + (_fQueryHeight - _Digit1->FontHeight())/2);
		_Digit2->Move(_Digit1->x() + _Digit1->width(),_pBlendBox1.y + (_fQueryHeight - _Digit1->FontHeight())/2);
	} else {
		_Digit1->Move(_pBlendBox2.x + fSpacing,_maxPosCube.y);
		_Digit2->Move(_Digit1->X() + _Digit1->width(),_maxPosCube.y);
		_pBlendBox1.y = _Digit1->y(); 
	}
}

// Retourne true si le mini jeu est rŽsolu
bool EMiniJeuCubes::GameIsResolve()
{
	_nResponse = _Digit1->GetVal() * 10 + _Digit2->GetVal();
	return _nResponse == _nNumCube;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuCubes::GameResolve()
{
	_Digit1->SetVal(_nNumCube / 10);
	_Digit2->SetVal(_nNumCube - (int)(_nNumCube / 10)*10);
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuCubes::GameSetHint(int nHintLevel)
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

void EMiniJeuCubes::GenerateSol() {
	int buildStatic[_nCubesSize][_nCubesSize][_nCubesSize];
	int level1 [_nCubesSize][_nCubesSize][_nCubesSize] = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 0, 0,

		1, 1, 1, 0,
		1, 1, 1, 1,
		1, 1, 1, 1,
		0, 1, 0, 0,

		1, 1, 1, 0,
		1, 1, 1, 1,
		1, 0, 0, 0,
		0, 0, 0, 0,

		1, 1, 1, 0,
		0, 1, 1, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	int level2 [_nCubesSize][_nCubesSize][_nCubesSize] = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 0, 0,

		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		0, 1, 0, 0,

		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 0,
		0, 1, 0, 0,

		1, 1, 1, 0,
		1, 1, 1, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};	
	int level3 [_nCubesSize][_nCubesSize][_nCubesSize] = {
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 0, 0,

		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 0, 0,

		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 0,
		0, 0, 0, 0,

		1, 1, 1, 0,
		1, 1, 1, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};	
	if (_nHintLevel <= MINIJEU_NORMAL) {
		memcpy(buildStatic,level1,sizeof(level1));
	} else if(_nHintLevel == MINIJEU_HINT1) {
		memcpy(buildStatic,level2,sizeof(level1));
	} else if (_nHintLevel > MINIJEU_HINT1) {
		memcpy(buildStatic,level3,sizeof(level1));
	}

	_nNumCube = 0;
	for (int z=0; z < _vGame.size(); z++) {
		for (int y=0; y< _vGame[0].size(); y++) {
			for (int x=0; x < _vGame[0][0].size(); x++) {
				_vGame[z][y][x] = buildStatic[z][y][x];
				if (_vGame[z][y][x] != 0)
					_nNumCube++;
			}
		}
	}

	// Calcule des dimension du cube et de la position initial de la piece en 0,0,0
	Coord min, max;
	min.x = _vGame[0].size() * _cVector[1].x;
	min.y = (_vGame.size() - 1) * _cVector[2].y;
	max.x = _vGame[0][0].size() * _cVector[0].x + _lpImgCube1->getWidth();
	max.y = (_vGame[0].size() * _cVector[1].y) + (_vGame[0][0].size() * _cVector[0].y) + _lpImgCube1->getHeight();

	_cFirstDice.x = _cOrigine.x - _cVector[0].x;
	_cFirstDice.y = _cOrigine.y + _cVector[2].y;

	_minPosCube = Coord(_cFirstDice.x + min.x, _cFirstDice.y + min.y );
	_maxPosCube = Coord(_cFirstDice.x + max.x, _cFirstDice.y + max.y);
}

// Preload images et sons
void EMiniJeuCubes::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_cubes_bg","jpg",true,true);
   EImageBank::getImage("mg_cube_yellow","png",true,true);
   EImageBank::getImage("mg_cube_blue","png",true,true);
   EImageBank::getImage("mg_cube_green","png",true,true);
   EImageBank::getImage("mg_cube_red","png",true,true);

}
