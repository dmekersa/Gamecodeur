#include "assert.h"

#include "FontBank.h"
#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "global.h"
#include "KRandom.h"

#include "EMiniJeuMasterMind.h"

using EMiniGame::GetRandom;

EMiniJeuMasterMind::EMiniJeuMasterMind(void)
{
	SetTextsTrad("MASTERMINDMG");
	SetGameType(MINIJEU_DIRECTGAME);
	SetFeatures(MINIJEU_LOGIC);
	SetGameNumber(10);
	SetBackGround(EImageBank::getImage("emastermind_bg","jpg"));

	_lpFont = EFontBank::getFont("tahoma.ttf",14);
	_lpFont->setColor(0,0,0,1);

	_lpFontCB = EFontBank::getFont("OldNewspaperTypes.ttf",18);
	_lpFontCB->setColor(1, 1, 1, .9f);
	_bColorBlindMode = false;

	memset(_cCBMessage, 0, 256);
	memset(_cNCBMessage, 0, 256);
	memset(_cHintText1,0,256);
	memset(_cHintText2,0,256);

	strcpy(_cCBMessage, KStr("COLORBLIND_MASTERMINDMG"));
	strcpy(_cNCBMessage, KStr("COLORBLIND_MASTERMINDMG_DISABLE"));

	strcpy(_cHintText1, KStr("RULES1_MASTERMINDMG"));
	strcpy(_cHintText2, KStr("RULES2_MASTERMINDMG"));

	_nXHintTextBox = 20;
	_nYHintTextBox = 305;
	_nWHintTextBox = 220;

	_nCol = 4;
	_nRow = 8;
	_nColor = 8;
	_nSetCol = _nCol;
	_gridHintType = GRID_DEFAULT;

	_lpImgBgFront = EImageBank::getImage("emastermind_bgfront","png");
	_lpImgHintQuerry = EImageBank::getImage("emastermind_querry","png");
	_lpImgIntero = EImageBank::getImage("emastermind_intero","png");
	_lpImgHintbg = EImageBank::getImage("emastermind_hintbg","png");
	_lpImgHint1 = EImageBank::getImage("emastermind_hint1","png");
	_lpImgHint2 = EImageBank::getImage("emastermind_hint2","png");
	_lpImgHintTextBg = EImageBank::getImage("emastermind_hinttextbg","png");
	_fHintScale = 20 /_lpImgHintbg->getWidth();

	_lpSOndes = new ESmartImage("ondes",0,0,true,false,0,false,24);
	_fOndesDelay = -1;

	_vGame.clear();
	_vPart.clear();
	_vHint.clear();
	_vSolution.clear();

	// Chargement du set de pièce
	_vPart.push_back(new EMasterMindPart(EImageBank::getImage("emastermind_part1.png"),
		EImageBank::getImage("emastermind_part1cb.png")));
	_vPart.push_back(new EMasterMindPart(EImageBank::getImage("emastermind_part2.png"),
		EImageBank::getImage("emastermind_part2cb.png")));
	_vPart.push_back(new EMasterMindPart(EImageBank::getImage("emastermind_part3.png"),
		EImageBank::getImage("emastermind_part3cb.png")));
	_vPart.push_back(new EMasterMindPart(EImageBank::getImage("emastermind_part4.png"),
		EImageBank::getImage("emastermind_part4cb.png")));
	_vPart.push_back(new EMasterMindPart(EImageBank::getImage("emastermind_part5.png"),
		EImageBank::getImage("emastermind_part5cb.png")));
	_vPart.push_back(new EMasterMindPart(EImageBank::getImage("emastermind_part6.png"),
		EImageBank::getImage("emastermind_part6cb.png")));
	_vPart.push_back(new EMasterMindPart(EImageBank::getImage("emastermind_part7.png"),
		EImageBank::getImage("emastermind_part7cb.png")));
	_vPart.push_back(new EMasterMindPart(EImageBank::getImage("emastermind_part8.png"),
		EImageBank::getImage("emastermind_part8cb.png")));
	
	for (int i=0; i<(int)_vPart.size();i++) {
		_vPart[i]->SwitchDrag();
	}

	_vGame = std::vector < std::vector < EMasterMindPart * > > (_nRow, std::vector < EMasterMindPart * > (_nCol,(EMasterMindPart *) NULL));
	_vSolution = std::vector < EMasterMindPart * > (_nCol, (EMasterMindPart *) NULL);
	_vHint = std::vector< std::vector< int > >(_nRow,(std::vector< int >(_nCol,0)));

	/*_lpGrid = new Grid(_nGameSize,_nGameSize,_lpImgDigged1->getWidth(),_lpImgDigged1->getHeight(),4,2);
	_lpGridGame->Move((_nW-_lpGridGame->width())/2,((_nH-_lpGridGame->height())-10)/2);*/

	_GrabedPart = NULL;
	_lpGridGame = NULL;
	_lpGridHint = NULL;
	_lpGridSolution = NULL;
	_lpGridSet = NULL;

	_fImgSize = _vPart[0]->Width();

	Reset();
}

EMiniJeuMasterMind::~EMiniJeuMasterMind(void)
{
	for (int i=0;i<(int)_vGame.size();i++){
		for (int j=0;j<(int)_vGame[0].size();j++) {
			XDELETE(_vGame[i][j]);
		}
	}
	_vGame.clear();

	for (int i=0;i<(int)_vSolution.size();i++) {
		XDELETE(_vSolution[i]);
	}
	_vSolution.clear();

	for (int i=0;i<(int)_vPart.size();i++) {
		XDELETE(_vPart[i]);
	}
	_vPart.clear();
	
	XDELETE(_GrabedPart);

	XDELETE(_lpGridGame);
	XDELETE(_lpGridHint);
	XDELETE(_lpGridSet);
	XDELETE(_lpGridSolution);

	XDELETE(_lpSOndes);

   XDELETE(_lpFont);
   XDELETE(_lpFontCB);
}

/** 3 steps rendering **/
void EMiniJeuMasterMind::GameCheck ()
{
	Coord cell;
	EMasterMindPart *tmp = NULL;

	if (! Mouse.LeftEvent())
		return;

	if (_GrabedPart != NULL) {
		if (!_lpGridGame->IsMouseOver() || _lpGridGame->GetMouseSelection().y != _nCurrentRow) {
			/*if (!_lpGridSet->IsMouseOver() && !_lpGridGame->IsMouseOver() && !_lpGridHint->IsMouseOver())
				Mouse.LeftEventClose();*/
			for (int i=0; i<(int)_vPart.size(); i++) {
				if (_vPart[i] == NULL)
					continue;
				if (*(_vPart[i]) == *_GrabedPart) {
					_vPart[i]->UnHide(0);
					cell = _vPart[i]->Pos();
					_vPart[i]->Move(_GrabedPart->Pos());
					_vPart[i]->Move(cell,200);
				}
			}
			XDELETE(_GrabedPart);
			Mouse.LeftEventClose();
		} else if (_lpGridGame->GetMouseSelection().y == _nCurrentRow) {
			tmp = NULL;
			cell = _lpGridGame->GetMouseSelection();
			if (_vGame[(int)cell.y][(int)cell.x] != NULL) {
				tmp = _vGame[(int)cell.y][(int)cell.x];
				tmp->Grab();
			}
			_GrabedPart->Check();
			_GrabedPart->Move(_lpGridGame->GetCellPos((int)cell.x,(int)cell.y));
			_vGame[(int)cell.y][(int)cell.x] = _GrabedPart;
			_GrabedPart = tmp;
			if (tmp != NULL) {
				ESoundBank::getSound("mg_swap")->playSample();
			} else {
				ESoundBank::getSound("mg_select_deselect")->playSample();
			}
			IsResolved();
		}
	}

	if (! Mouse.LeftEvent() || _GrabedPart != NULL)
		return;

	// gestion de la selection de pièce
	if (_lpGridSet->IsMouseOver()) {
		cell = _lpGridSet->GetSelection(Mouse);
		tmp = _vPart[(int)(cell.x + cell.y*_lpGridSet->_nCol)];
		_GrabedPart = new EMasterMindPart(tmp);
		_GrabedPart->SwitchDrag();
		tmp->Hide();
		_GrabedPart->Check();
		ESoundBank::getSound("mg_select_deselect")->playSample();
	}
	if (_lpGridGame->IsMouseOver()) {
		cell = _lpGridGame->GetMouseSelection();
		if (cell.y == _nCurrentRow && _vGame[(int)cell.y][(int)cell.x] != NULL) {
			_GrabedPart = _vGame[(int)cell.y][(int)cell.x];
			_GrabedPart->Check();
			_vGame[(int)cell.y][(int)cell.x] = NULL;
			ESoundBank::getSound("mg_select_deselect")->playSample();
		}
	}

	if (Mouse.x >=0 && Mouse.x<= 300 && Mouse.y > 20 && Mouse.y < 40) {
		_bColorBlindMode = ! _bColorBlindMode;
		ESoundBank::getSound("mg_select_deselect")->playSample();
		for (int i=0; i<(int)_vPart.size(); i++) {
			if (_vPart[i] == NULL)
				continue;
			_vPart[i]->ColorBlind();
		}
		Reset();
	}
}

void EMiniJeuMasterMind::GameLogic()
{
	if (_GrabedPart != NULL)
		_GrabedPart->Logic();
}

void EMiniJeuMasterMind::GameDrawSet()
{
	_lpSOndes->Draw();
	_lpImgBgFront->blitAlphaRect(0,0,_lpImgBgFront->getWidth(),_lpImgBgFront->getHeight(),0,0);
	Coord pos,size;
			
	if (_fOndesDelay <=0) {
		pos = Coord(626,0);
		size = Coord((1024-pos.x)*GetRandom(0,1),545*GetRandom(0,1));
		pos = pos + size;

		_lpSOndes->Move(pos.x,pos.y);
		_lpSOndes->StartVideo();
		_fOndesDelay = GetRandom(200,1500);
	} else if (_lpSOndes->IsVideoEndReached()) {
		_fOndesDelay -= MyGame::getGame()->getElapsedTime();
	}
		
	// Chaine color blind a affiché en cas de modification adapté le GameCheck
	if (_bColorBlindMode) {
      _lpFontCB->drawStringFromLeft(_cNCBMessage,0, 20);
	} else  {
		_lpFontCB->drawStringFromLeft(_cCBMessage,0, 20);
	}

	DrawHint();

	for(int i=0;i<(int)_vPart.size();i++) {
		if (_vPart[i] != NULL && _vPart[i]->TransformComplete())
			_vPart[i]->Draw();
	}

	float hintImgSize = _lpImgHintbg->getWidth();
	for(int i=0; i<_nRow; i++) {
		if (i == _nCurrentRow && _nGameStatus < MINIJEU_RESOLVED) {
			pos = _lpGridGame->GetCellPos(0,i);
			_lpImgIntero->blitAlphaRect(0,0,_lpImgIntero->getWidth(),_lpImgIntero->getHeight(),(short)pos.x,(short)pos.y);
		}
		for (int j=0; j<_nCol; j++) {
			pos = _lpGridHint->GetCellPos(1,i);
			pos.y = pos.y + (hintImgSize-(hintImgSize*_fHintScale));
			pos.x = pos.x + (int)(j%2)*hintImgSize*_fHintScale;
			pos.y = pos.y + ((int)j/2)*hintImgSize*_fHintScale;
			if (_vHint[i][j] == 1) {
				_lpImgHint1->blitAlphaRectFx(0,0,hintImgSize,hintImgSize,pos.x,pos.y,0,_fHintScale,1);
			} else if (_vHint[i][j] == 2) {
				_lpImgHint2->blitAlphaRectFx(0,0,hintImgSize,hintImgSize,pos.x,pos.y,0,_fHintScale,1);
			} else {
				_lpImgHintbg->blitAlphaRectFx(0,0,hintImgSize,hintImgSize,pos.x,pos.y,0,_fHintScale,1);
			}
			if (_vGame[i][j] != NULL)
				_vGame[i][j]->Draw();
			
			//if (i > _nCurrentRow)
			//	_lpGridGame->FillCell(_lpImgIntero,j,i,0.2f);
		}
	}

	if (_nGameStatus == MINIJEU_RESOLVED)
		for(int i=0;i<(int)_vSolution.size();i++)
			_vSolution[i]->Draw();
		
	if (_nGameStatus < MINIJEU_RESOLVED)
		_lpGridHint->FillCell(_lpImgHintQuerry,0,_nCurrentRow,false);
}

void EMiniJeuMasterMind::GameDrawGrab(){
	for(int i=0;i<(int)_vPart.size();i++) {
		if (_vPart[i] != NULL && !_vPart[i]->TransformComplete())
			_vPart[i]->Draw();
	}

	if (_GrabedPart != NULL)
		_GrabedPart->Draw();
}

// Retourne true si le mini jeu est rŽsolu
bool EMiniJeuMasterMind::GameIsResolve()
{
	// Test si la ligne de réponse es complète
	for (int i=0; i<(int)_vSolution.size();i++) {
		if (_vGame[_nCurrentRow][i] == NULL)
			return false;
	}

	int hint=0;
	std::vector< EMasterMindPart * > tmpSolution = std::vector< EMasterMindPart * >(_vSolution.size(),(EMasterMindPart *) NULL);
	std::vector< EMasterMindPart * > tmpSubmit = std::vector< EMasterMindPart * >(_vSolution.size(),(EMasterMindPart *) NULL);

	// Detection des match exact position couleur
	for (int i=0; i<(int)_vSolution.size();i++) {
		tmpSolution[i] = _vSolution[i];
		tmpSubmit[i] = _vGame[_nCurrentRow][i];
	
		if ((*tmpSolution[i]) == (*tmpSubmit[i])) {
			_vHint[_nCurrentRow][hint] = 1;
			hint++;
			tmpSolution[i] = NULL;
			tmpSubmit[i] = NULL;
			continue;
		}
	}
	// Detection des match couleur uniquement
	for (int i=0; i<(int)tmpSolution.size();i++) {
		if (tmpSubmit[i] == NULL)
			continue;
		for (int j=0;j<(int)tmpSolution.size();j++) {
			if (tmpSolution[j] != NULL && (*tmpSolution[j]) == (*tmpSubmit[i])) {
				_vHint[_nCurrentRow][hint] = 2;
				hint++;
				tmpSolution[j] = NULL;
				tmpSubmit[i] = NULL;
				break;
			}
		}
	}

	// Test si la solution proposé es bonne
	bool res = true;
	for (int i=0; i<(int)tmpSolution.size();i++) {
		_vGame[_nCurrentRow][i]->SwitchDrag();
		if (_vHint[_nCurrentRow][i] != 1)
			res = false;
	}

	// retablissement du set de pièces
	if (!res) {
		for (int i=0; i<(int)_vPart.size(); i++) {
			if (_vPart[i] == NULL)
				break;
			_vPart[i]->UnHide();
		}
	}

	_nCurrentRow--;
	if (!res && _nCurrentRow < 0) {
		SetGameFailed();
	} else {
		ESoundBank::getSound("mg_apparition")->playSample();
	}

	return res;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuMasterMind::GameResolve()
{
	for (int i=0;i<(int)_vSolution.size();i++) {
		XDELETE(_vGame[_nCurrentRow][i]);
		_vGame[_nCurrentRow][i] = new EMasterMindPart(_vSolution[i]);
		_vGame[_nCurrentRow][i]->Move(_lpGridGame->GetCellPos(i,_nCurrentRow));
	}
	IsResolved();
}

void EMiniJeuMasterMind::GameReset()
{
	// Clear
	for (int i=0;i<(int)_vGame.size();i++){
		for (int j=0;j<(int)_vGame[0].size();j++) {
			XDELETE(_vGame[i][j]);
		}
	}

	for (int i=0;i<(int)_vSolution.size();i++) {
		XDELETE(_vSolution[i]);
	}

	XDELETE(_GrabedPart);

	XDELETE(_lpGridGame);
	XDELETE(_lpGridHint);
	XDELETE(_lpGridSet);
	XDELETE(_lpGridSolution);

	_vGame.clear();
	_vSolution.clear();
	_vHint.clear();

	float igs = 1;
	float bgs = 3;

	// Definition et positionnement des grilles
	_lpGridHint		= new Grid(2,_nRow,_fImgSize,_fImgSize,bgs,igs);
	_lpGridGame		= new Grid(_nCol,_nRow,_fImgSize,_fImgSize,bgs,igs);
	_lpGridSet		= new Grid(_nSetCol,2,_fImgSize,_fImgSize,bgs,igs,_gridHintType);
	_lpGridSolution = new Grid(_nCol,1,_fImgSize,_fImgSize,bgs,igs);
	
	float posX = (_nW - (_lpGridGame->width() + _lpGridHint->width() + 5))/2 + 70;

	_lpGridSolution->Move(posX,45);
	_lpGridGame->Move(_lpGridSolution->_xPos,_lpGridSolution->_yPos + _lpGridSolution->height() + 5);
	_lpGridHint->Move(_lpGridSolution->_xPos + _lpGridGame->width() + 5,_lpGridGame->_yPos);
	_lpGridSet->Move(_lpGridSolution->_xPos + (_lpGridGame->width() - _lpGridSet->width())/2, _lpGridGame->_yPos + _lpGridGame->height() + 5);

	// Definition du set de jeu
	_vGame = std::vector < std::vector < EMasterMindPart * > > (_nRow, std::vector < EMasterMindPart * > (_nCol,(EMasterMindPart *) NULL));
	_vSolution = std::vector < EMasterMindPart * > (_nCol, (EMasterMindPart *) NULL);
	_vHint = std::vector< std::vector< int > >(_nRow,(std::vector< int >(_nCol,0)));

	int numpart=0;
	Coord c;
	for (int i=0; i<(int)_vPart.size(); i++) {
		if (i>=_nColor) {
			XDELETE(_vPart[i]);
			continue;
		}
		c = Coord(i%_nSetCol,(int) (i/_nSetCol));
		if (!_lpGridSet->CheckQCell(c)) {
			XDELETE(_vPart[i]);
			continue;
		}
		_vPart[i]->Move(_lpGridSet->GetCellPos(c));
		_vPart[i]->UnHide();
		numpart++;
	}

	EMasterMindPart *tmp = NULL;
	for(int i=0; i<(int)_vSolution.size(); i++) {
		tmp = NULL;
		while (tmp == NULL) {
			tmp = new EMasterMindPart(_vPart[GetRandom(numpart)]);
			for (int j=0; j<(int)_vSolution.size(); j++) {
				if (_vSolution[j] == NULL)
					break;
				if (*tmp == *(_vSolution[j])) {
					XDELETE(tmp);
					break;
				}
			}
		}
		tmp->Show();
		tmp->Move(_lpGridSolution->GetCellPos(i,0));
		_vSolution[i] = tmp;
	}

	_nCurrentRow = _nRow-1;
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuMasterMind::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
   		  ShowHint(MINIJEU_HINT1);
		  _nCol = 4;
		  _nColor = 7;
		  _gridHintType = GRID_QUINCONCE_V_P;
		  Reset();
		  break;
	  case MINIJEU_HINT2:
   		  ShowHint(MINIJEU_HINT2);
		  _nCol = 4;
		  _nColor = 5;
		  _nSetCol = 3;
		  _gridHintType = GRID_QUINCONCE_V_P;
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

void EMiniJeuMasterMind::DrawHint() {
	float y = (float)_nYHintTextBox;
	float hintImgSize = _lpImgHint1->getWidth();

	_lpImgHintTextBg->blitAlphaRectF(0,0,_lpImgHintTextBg->getWidth(), _lpImgHintTextBg->getHeight(),0,_nYHintTextBox);

	y = y + 70;

	_lpImgHint1->blitAlphaRect(0,0,hintImgSize,hintImgSize,(short)_nXHintTextBox,(short)y);
	_lpFont->setBlend(1);
	_lpFont->drawMultiline(_cHintText1,_nXHintTextBox + _lpImgHint1->getWidth() +5 ,(long)_nXHintTextBox + _nWHintTextBox,(long)y,TEXTSTYLE_LEFT);
	
	y = y+30+_lpFont->getMultilineHeight(_cHintText1,_nXHintTextBox,(long)_nXHintTextBox + _nWHintTextBox,(long)y,TEXTSTYLE_LEFT);
	_lpImgHint2->blitAlphaRect(0,0,hintImgSize,hintImgSize,(short)_nXHintTextBox,(short)y);
	_lpFont->drawMultiline(_cHintText2,_nXHintTextBox + _lpImgHint1->getWidth() +5,_nXHintTextBox + _nWHintTextBox,y,TEXTSTYLE_LEFT);
}

EMasterMindPart::EMasterMindPart (KGraphic *lpGraphic,KGraphic *lpGraphicColorBlind): EMiniJeuPart(lpGraphic, 1,1,0,0) {
	_lpImgColorBlind = lpGraphicColorBlind;
	SwitchRotation();
	_bHide = false;
}
EMasterMindPart::EMasterMindPart(const EMasterMindPart* a): EMiniJeuPart(a) {
	_bHide = a->_bHide;	
}

void EMasterMindPart::ColorBlind() {
	KGraphic *tmp;
	if (_lpImgColorBlind != NULL)
		tmp = _lpImg;
		_lpImg = _lpImgColorBlind;
		_lpImgColorBlind = tmp;
}

void EMasterMindPart::Hide(int timeout) {
	if (_bHide)
		return;
	FadOut(timeout);
	_bHide = true;
}
void EMasterMindPart::UnHide(int timeout) {
	if (!_bHide)
		return;
	FadIn(timeout);
	_bHide = false;
}


// Preload images et sons
void EMiniJeuMasterMind::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("emastermind_bg","jpg",true,true);
   EImageBank::getImage("emastermind_bgfront","png",true,true);
   EImageBank::getImage("emastermind_querry","png",true,true);
   EImageBank::getImage("emastermind_intero","png",true,true);
   EImageBank::getImage("emastermind_hintbg","png",true,true);
   EImageBank::getImage("emastermind_hint1","png",true,true);
   EImageBank::getImage("emastermind_hint2","png",true,true);
   EImageBank::getImage("emastermind_hinttextbg","png",true,true);
   EImageBank::getImage("emastermind_part1.png","",true,true);
   EImageBank::getImage("emastermind_part1cb.png","",true,true);
   EImageBank::getImage("emastermind_part2.png","",true,true);
   EImageBank::getImage("emastermind_part2cb.png","",true,true);
   EImageBank::getImage("emastermind_part3.png","",true,true);
   EImageBank::getImage("emastermind_part3cb.png","",true,true);
   EImageBank::getImage("emastermind_part4.png","",true,true);
   EImageBank::getImage("emastermind_part4cb.png","",true,true);
   EImageBank::getImage("emastermind_part5.png","",true,true);
   EImageBank::getImage("emastermind_part5cb.png","",true,true);
   EImageBank::getImage("emastermind_part6.png","",true,true);
   EImageBank::getImage("emastermind_part6cb.png","",true,true);
   EImageBank::getImage("emastermind_part7.png","",true,true);
   EImageBank::getImage("emastermind_part7cb.png","",true,true);
   EImageBank::getImage("emastermind_part8.png","",true,true);
   EImageBank::getImage("emastermind_part8cb.png","",true,true);

   ESoundBank::getSound("mg_apparition",false,true);
}
