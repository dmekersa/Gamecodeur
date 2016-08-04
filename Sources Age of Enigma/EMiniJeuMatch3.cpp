#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "FontBank.h"
#include "EGeometry.h"
#include "math.h"

#include "EMiniJeuMatch3.h"

EMiniJeuMatch3::EMiniJeuMatch3(void)
{
	SetTextsTrad("MATCH3");
	SetGameType(MINIJEU_DIRECTGAME);
	SetGameNumber(25);
	
	_nNumCol  = 5;
	_nNumLine = 11;
	
	_nWinFloor = 2;
	_vSymbs.push_back(EImageBank::getImage("mg_match3_symb01","png"));
	_vSymbs.push_back(EImageBank::getImage("mg_match3_symb02","png"));
	_vSymbs.push_back(EImageBank::getImage("mg_match3_symb03","png"));
	_vSymbs.push_back(EImageBank::getImage("mg_match3_symb04","png"));
	_vSymbs.push_back(EImageBank::getImage("mg_match3_symb05","png"));
	
	_lpGridGame = NULL;
	
	// initialisation des graphiques
	_lpMatch3bg = EImageBank::getImage("mg_match3_bg","png");

	KGraphic *tmpClouds = EImageBank::getImage("mg_match3_clouds","png");
	_lpClouds = new ESmartImage(tmpClouds,(1024-tmpClouds->getHeight())/2,-tmpClouds->getHeight()/2,1,true,false,0);
	_lpClouds->SetRotation(0,360,30000,false,K_COUNTER_LINEAR);
	_lpClouds->SetLoop(true);
	_lpClouds->Rotate(true);

	_lpTorch1 = new ESmartImage("torch_anim",93,211,true,false,0);
	_lpTorch1->StartVideo();
	
	_lpTorch2 = new ESmartImage("torch_anim",831,44,true,false,0);
	_lpTorch2->StartVideo();
   
   _fFrameTime = 0;
	
   Reset();
}

EMiniJeuMatch3::~EMiniJeuMatch3(void)
{
	EMatch3Part::GameClear();
	//for (int i=0; i<_vSymbs.size(); i++) {
	//	_vSymbs[i]=NULL;
	//}
	_vSymbs.clear();

	XDELETE(_lpGridGame);
	XDELETE(_lpClouds);
	XDELETE(_lpTorch1);
	XDELETE(_lpTorch2);
}

/** 3 steps rendering **/
void EMiniJeuMatch3::GameCheck ()
{
	EMatch3Part::GameCheck();

#ifdef DEBUG
	if (Mouse.RightEvent()) {
		EMatch3Part::RotateGridCW();
		Mouse.RightEventClose();
	}
#endif
}

void EMiniJeuMatch3::GameLogic()
{
	EMatch3Part::GameLogic();
}

void EMiniJeuMatch3::GameDrawSet()
{
   _lpClouds->Draw();
   if (!_lpClouds->isRotate())
      _lpClouds->Rotate(true);

   _lpMatch3bg->blitAlphaRectFx(0,0,_lpMatch3bg->getWidth(),_lpMatch3bg->getHeight(),0,0,0,1,1);
	_lpTorch1->Draw();
	_lpTorch2->Draw();

	EMatch3Part::GameDrawSet();
	Coord pos1,pos2;
	pos1 = _lpGridGame->GetCellPos(0,_nNumLine-_nWinFloor);
	pos2 = Coord(pos1.x + _lpGridGame->width() +5,pos1.y);
	pos1.x-=5;


   _fFrameTime += MyGame::getGame()->getKWindow()->getFrameTime();
   float lineAlpha = fabs(sinf(_fFrameTime*0.001*3.14116));
   _vSymbs[0]->drawLine(pos1.x,pos1.y+1,pos2.x,pos2.y+1,1,0,0,lineAlpha);
	_vSymbs[0]->drawLine(pos1.x,pos1.y,pos2.x,pos2.y,1,0,0,lineAlpha);

	IsResolved();
}

void EMiniJeuMatch3::GameReset() {
	// Cleanup
	XDELETE(_lpGridGame);

	// Reset
	_lpGridGame = new Grid(_nNumCol, _nNumLine, _vSymbs[0]->getWidth(),_vSymbs[0]->getHeight(),2,1);
	_lpGridGame->Move((_nW - _lpGridGame->width())/2,3);
	
	EMatch3Part::GameInit(_lpGridGame,_vSymbs);
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuMatch3::GameIsResolve()
{
	for (int i=0; i<EMatch3Part::_lpGame.size() - _nWinFloor; i++) {
		for (int j=0; j<EMatch3Part::_lpGame[0].size(); j++) {
			if (EMatch3Part::_lpGame[i][j] != NULL)
				return false;
		}
	}
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuMatch3::GameResolve()
{
	for (int i=0; i < EMatch3Part::_lpGame.size() - _nWinFloor; i++) {
		for (int j=0; j < EMatch3Part::_lpGame[0].size(); j++) {
			if (EMatch3Part::_lpGame[i][j] != NULL)
				EMatch3Part::_lpGame[i][j]->ForcePop();
		}
	}
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuMatch3::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
		case MINIJEU_HINT1:
			_nWinFloor = 3;
			ShowHint(nHintLevel);
			Reset();
			break;
		case MINIJEU_HINT2:
			_nWinFloor = 4;
			ShowHint(nHintLevel);
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

 /* --------------------- General Match3 logic ---------------------------- */

//float EMatch3Part::_fElapsedTime = 0;
int EMatch3Part::_numpart = 0;
EMatch3Part *EMatch3Part::_lpSelected = NULL;
EMatch3Part *EMatch3Part::_lpSwaped = NULL;
KGraphic *EMatch3Part::_lpGraphic  = NULL;
std::vector< KGraphic * > EMatch3Part::_vSymbols;
Grid *EMatch3Part::_lpGrid = NULL;
std::vector< std::vector < EMatch3Part * > > EMatch3Part::_lpGame;

std::list< EMatch3Part * > EMatch3Part::_lpMatchList;
std::list< KCounter * > EMatch3Part::_lpMatchCounterList;
std::list< KCounter * > EMatch3Part::_lpPreFallCounterList;
std::list< KCounter * > EMatch3Part::_lpFallCounterList;

float EMatch3Part::_fPrePopDuration = 250, EMatch3Part::_fPopDuration = 1500;
float EMatch3Part::_fPreFallDuration = 250, EMatch3Part::_fFallDuration = 100;
//float EMatch3Part::_fPrePopDuration = 250, EMatch3Part::_fPopDuration = 1500;
//float EMatch3Part::_fPreFallDuration = 1000, EMatch3Part::_fFallDuration = 1500;
float EMatch3Part::_fBumpDuration = 130;

Coord EMatch3Part::_vDir[4] = { EMiniGame::GetDirVector(EMiniGame::WEST),
	EMiniGame::GetDirVector(EMiniGame::SOUTH),
	EMiniGame::GetDirVector(EMiniGame::EAST),
	EMiniGame::GetDirVector(EMiniGame::NORTH)};

void EMatch3Part::GameInit(Grid *grid, std::vector< KGraphic *> vSymbols) {
	// Clean des variables statics
	//_fElapsedTime = 0;
	GameClear();

	_lpSelected = NULL;
	_lpSwaped = NULL;
	_lpGrid = grid;
	_lpGraphic = KPTK::createKGraphic();

	// Initialisation des vecteur de directions
	_vDir[0] = EMiniGame::GetDirVector(EMiniGame::WEST);
	_vDir[1] = EMiniGame::GetDirVector(EMiniGame::SOUTH);
	_vDir[2] = EMiniGame::GetDirVector(EMiniGame::EAST);
	_vDir[3] = EMiniGame::GetDirVector(EMiniGame::NORTH);

	for (int i=0; i<vSymbols.size(); i++) {
		_vSymbols.push_back(vSymbols[i]);
	}

	_lpGame = std::vector< std::vector< EMatch3Part * > >(_lpGrid->_nLine, std::vector< EMatch3Part * > (_lpGrid->_nCol, (EMatch3Part *) NULL));

	// peuplement de la grille de jeu courante
	EMatch3Part *tmp;
	for (int i=0; i<_lpGame.size(); i++) {
		for (int j=0; j<_lpGame[0].size(); j++) {

			/*int t = EMiniGame::GetRandom(_vSymbols.size()+1);
			if (t == _vSymbols.size()) {
				_lpGame[i][j] = NULL;
				continue;
			}*/
w0:			tmp = new EMatch3Part(_vSymbols[EMiniGame::GetRandom(_vSymbols.size())]);
			while (tmp->HasMatch()) {
				XDELETE(tmp);
				goto w0;
			}
			_lpGame[i][j] = tmp;
			tmp->SetGridPos(j,i);
			if (tmp->HasMatch()) {
				_lpGame[i][j] = NULL;
				XDELETE(tmp);
				goto w0;
			}
		}
	}
}
void EMatch3Part::GameClear() {
	// Reinitialisation des variables statics
	XDELETE(_lpGraphic);
	_lpGrid = NULL; // External Ref
	_lpSelected = NULL;
	_lpSwaped = NULL;
	
	// Re Initialisation des vecteur de directions
	_vDir[0] = EMiniGame::GetDirVector(EMiniGame::WEST);
	_vDir[1] = EMiniGame::GetDirVector(EMiniGame::SOUTH);
	_vDir[2] = EMiniGame::GetDirVector(EMiniGame::EAST);
	_vDir[3] = EMiniGame::GetDirVector(EMiniGame::NORTH);

	// Abandon des symbol ref
	_vSymbols.clear();

	// Nettoyage de la grille de jeux
	for (int i=0; i<_lpGame.size(); i++) {
		for (int j=0; j<_lpGame[0].size(); j++) {
			XDELETE(_lpGame[i][j]);
		}
	}
	_lpGame.clear();
	_lpMatchList.clear();

	// nettoyage des ref counter generals
	std::list< KCounter * >::iterator it;
	for (it = _lpMatchCounterList.begin(); it != _lpMatchCounterList.end(); it++) {
		XDELETE((*it));
	}
	_lpMatchCounterList.clear();
	for (it = _lpPreFallCounterList.begin(); it != _lpPreFallCounterList.end(); it++) {
		XDELETE((*it));
	}
	_lpPreFallCounterList.clear();
	for (it = _lpFallCounterList.begin(); it != _lpFallCounterList.end(); it++) {
		XDELETE((*it));
	}
	_lpFallCounterList.clear();
}
void EMatch3Part::GameCheck() {
	EMatch3Part *tmp = NULL;
	Coord pos;

	if (Mouse.LeftEvent() && _lpGrid->IsMouseOver()) {
		// clic droit au dessu de la grille	
		pos = _lpGrid->GetMouseSelection();
		tmp = _lpGame[pos.y][pos.x];
	} else if (Mouse.LeftReleaseEvent() && _lpSelected != NULL) {
		// Relachement de la pièce précédemment selectionné
		tmp = _lpSelected;
	} else {
		return;
	}

	if (tmp != NULL) {
		tmp->Check();
	}
}

void EMatch3Part::GameLogic() {
}

void EMatch3Part::GameDrawSet() {
	int numPart = 0;
	double dElapsedTime = MyGame::getGame()->getElapsedTime();
	EMatch3Part *tmp;
	//_lpGrid->DrawBaseGrid(_lpGraphic);
	std::list< KCounter * >::iterator it;

	// Avance les compteurs des match3 et des prefall
	for (it = _lpMatchCounterList.begin(); it != _lpMatchCounterList.end(); it++) {
		(*it)->move(dElapsedTime);
	}
	for (it = _lpPreFallCounterList.begin(); it != _lpPreFallCounterList.end(); it++) {
		(*it)->move(dElapsedTime);
	}
	for (it = _lpFallCounterList.begin(); it != _lpFallCounterList.end(); it++) {
		(*it)->move(dElapsedTime);
	}

	// Initialisation du elapsedtime et affichage des pièces static
	for (int i=0; i<_lpGame.size(); i++) {
		for (int j=0; j<_lpGame[0].size(); j++) {
			tmp = _lpGame[i][j];
			if (tmp != NULL) {
				numPart++;
				tmp->SetElapsedTime(dElapsedTime);
				if (tmp != _lpSelected && tmp->_lpCounterScale->isCompleted())
					tmp->Draw();
			}
		}
	}


	// Affichage des pièces en transformations
	for (int i=0; i<_lpGame.size(); i++) {
		for (int j=0; j<_lpGame[0].size(); j++) {
			tmp = _lpGame[i][j];
			if (tmp != NULL && tmp != _lpSelected && !tmp->_lpCounterScale->isCompleted())
				tmp->Draw();
		}
	}

	// Dessin de la pièce selectionné par dessus les autres
	if (_lpSelected != NULL)
		_lpSelected->Draw();

	// Recherche les nouveaux match3
	CheckMatch3();

	// Nettoyage des compteurs des match3 et prefall
	KCounter *tmpCounter;
	for (it = _lpMatchCounterList.begin(); it != _lpMatchCounterList.end();) {
		tmpCounter = (*it);
		it++;
		if (tmpCounter->isCompleted()) {
			_lpMatchCounterList.remove(tmpCounter);
			XDELETE(tmpCounter);
			//ESoundBank::getSound("laser")->playSample();
		}
	}
	for (it = _lpPreFallCounterList.begin(); it != _lpPreFallCounterList.end();) {
		tmpCounter = (*it);
		it++;
		if (tmpCounter->isCompleted()) {
			_lpPreFallCounterList.remove(tmpCounter);
         for (int i=0; i<_lpGame.size(); i++)
            for (int j=0; j<_lpGame[0].size(); j++)
               if (_lpGame[i][j] != NULL)
                  KAssert(_lpGame[i][j]->_lpCounterPreFall != tmpCounter);
			XDELETE(tmpCounter);
		}
	}
	for (it = _lpFallCounterList.begin(); it != _lpFallCounterList.end();) {
		tmpCounter = (*it);
		it++;
		if (tmpCounter->isCompleted()) {
			_lpFallCounterList.remove(tmpCounter);
         for (int i=0; i<_lpGame.size(); i++)
            for (int j=0; j<_lpGame[0].size(); j++)
               if (_lpGame[i][j] != NULL)
                  KAssert(_lpGame[i][j]->_lpCounterFall != tmpCounter);
         XDELETE(tmpCounter);
         ESoundBank::getSound("mg_match3_fall")->playSample();
		}
	}

	// Nettoyage des pièce detruite
	for (int i=0; i<_lpGame.size(); i++) {
		for (int j=0; j<_lpGame[0].size(); j++) {
			tmp = _lpGame[i][j];
			if (tmp != NULL && tmp->GetState() == POP && !tmp->IsVisible()) {
				ESoundBank::getSound("mg_match3_explode")->playSample();
				_lpGame[i][j] = NULL;
				XDELETE(tmp);
			}
		}
	}

	// Debug affichage du nombre de piece au draw
	//KTextFace *font = EFontBank::getFont("tahoma.ttf",30);
	//font->drawStringFromLeft(itos(numPart).c_str(),10,10);
}

void EMatch3Part::SwapGridPart(Coord c1, Coord c2) {
	EMatch3Part *tmp = _lpGame[c2.y][c2.x];
	_lpGame[c2.y][c2.x] = _lpGame[c1.y][c1.x];
	_lpGame[c1.y][c1.x] = tmp;
	if (tmp != NULL) {
		tmp->SetGridPos(c1);
	}
	if (_lpGame[c2.y][c2.x] != NULL) {
		_lpGame[c2.y][c2.x]->SetGridPos(c2);
	}
}

void EMatch3Part::RotateGridCW() {
	Coord tmp = _vDir[3];
	for (int i=3; i>0; i--) {
		_vDir[i] = _vDir[i-1];
	}
	_vDir[0] = tmp;

	if (_lpSelected != NULL) {
		_lpSelected->Select(false);
	}

	for (int i=0; i < _lpGame.size(); i++) {
		for (int j=0; j<_lpGame[0].size(); j++) {
			if (_lpGame[i][j] != NULL) {
				_lpGame[i][j]->RotateCW(true);
			}
		}
	}
}

void EMatch3Part::RotateGridACW() {
	Coord tmp = _vDir[0];
	for (int i=0; i<3; i++) {
		_vDir[i] = _vDir[i+1];
	}
	_vDir[3] = tmp;

	if (_lpSelected != NULL) {
		_lpSelected->Select(false);
	}

	for (int i=0; i < _lpGame.size(); i++) {
		for (int j=0; j<_lpGame[0].size(); j++) {
			if (_lpGame[i][j] != NULL) {
				_lpGame[i][j]->RotateACW(true);
			}
		}
	}
}

void EMatch3Part::CheckMatch3() {
	_lpMatchList.clear();
	for (int i=0; i < _lpGame.size(); i++) {
		for (int j=0; j<_lpGame[0].size(); j++) {
			if (_lpGame[i][j] != NULL) {
				_lpGame[i][j]->Match();
			}
		}
	}
}
/* ------------------ Specific Game Part ------------------------------ */
EMatch3Part::EMatch3Part(KGraphic *imgPart): EMiniJeuPart(imgPart,4,0,0,90) {
	_posGrid = Coord(-1,-1);
	_fromPos = Coord(-1,-1);
	_bumpOffset = Coord(0,0);

	_lpCounterScale = new KCounter();
	_lpCounterScale->startCounter(1,1,0,0,K_COUNTER_LINEAR);
	_lpCounterScale->move(1);

	// Gestion de la selection
	_fScaleDuration = 150;
	_fMaxScale = 1.20;
	_fSelectedPos = Coord(0,0);
	_fSelectedPosDelta =0;
	
	// Gestion de la chute
	_preFallOffset = Coord(0,0);
	_fallOffset = Coord(0,0);
	_preFallDir = Coord(0,0);
	_fallDir = Coord(0,0);
    _lpCounterPreFall = NULL;
	_lpCounterFall = NULL;
	_randomPreFallOffset = 0;
	_bPop = false;

	// Gestion du bump
	_lpCounterBump = NULL;

	// gestion du match
	_lpCounterPrePop = NULL;
	_fCurrentPopDuration = 0;

	SetRotationDuration(500);
}
EMatch3Part::~EMatch3Part(void) {
	XDELETE(_lpCounterBump);
	XDELETE(_lpCounterScale);
	_lpCounterFall = NULL;
	_lpCounterPreFall = NULL;
	_lpCounterPrePop = NULL;
}
Coord EMatch3Part::GetPosOffset() {
	return _preFallOffset + _fallOffset + _bumpOffset; 
}
EMatch3Part* EMatch3Part::GetPart(Coord cell) {
	if(_lpGrid->CheckQCell(cell)) {
		return _lpGame[cell.y][cell.x];
	}
	return NULL;
}
bool EMatch3Part::ShouldFall() {
	Coord tmp = _posGrid  + _vDir[EMiniGame::SOUTH];
	if (!_lpGrid->CheckQCell(tmp))
		return false;
	EMatch3Part *tmpPart = _lpGame[tmp.y][tmp.x];
	if (tmpPart == NULL) {
		return true;
	} 
	return false;
}
void EMatch3Part::Check() {
	if (Mouse.LeftEvent()) {//&& IsMouseOver()) {
		// impossible de selectionné une piece en chute libre
		if (GetState() > STABLE) {
			ESoundBank::getSound("mg_forbid")->playSample();
			Mouse.LeftEventClose();
			return;
		}
		Select(true);
		ESoundBank::getSound("mg_select_deselect")->playSample();
		Mouse.LeftEventClose();
	} else if (Mouse.LeftReleaseEvent() && IsSelected()) {
		Select(false);
		ESoundBank::getSound("mg_select_deselect")->playSample();
		Mouse.LeftEventClose();
	}
}
void EMatch3Part::Draw() {
	Coord tmp;

	/*if (_posGrid == Coord(0,0)) {
		int titi=0;
	}*/
	if (!_lpCounterScale->isCompleted()) {
		_lpCounterScale->move(_dElapsedTime);
		_fScale = _lpCounterScale->getCurrentValue();
	}
	if (IsSelected()) {
		UpdateSelectedPos();
	} else if (_lpSwaped == this) {
		
	}
	PreFall();
	Bump();
	EMiniJeuPart::Draw();

	if (_lpCounterPrePop != NULL && _lpCounterPrePop->isCompleted()) {
		_lpCounterPrePop=NULL;
      _bPop = true;
		FadOut(_fCurrentPopDuration);
	}
}
void EMatch3Part::ForcePop() {
	_lpCounterPrePop = NULL;
   _bPop = true;
	FadOut(_fPopDuration + GetRandom(-1000,500));
}
void EMatch3Part::Bump() {
	if (_lpCounterBump == NULL)
		return;

	if (!_lpCounterBump->isCompleted() && GetState() == STABLE) {
		_lpCounterBump->move(_dElapsedTime);
		_bumpOffset = _fallDir * -((float)sin(_lpCounterBump->getCurrentValue())) * 3;
		return;
	}
	XDELETE(_lpCounterBump);
	_bumpOffset = Coord(0,0);
}

EMatch3Part::FallState EMatch3Part::GetState() {
   if (_bPop) return POP;
   if (_fMaxBlend > _fBlend) return DISABLED;
   if (_lpCounterPrePop != NULL) return PREPOP;
   if (_lpCounterFall != NULL) return FALL;
   if (_lpCounterPreFall != NULL) return PREFALL;
   return STABLE;
}

void EMatch3Part::Select(bool bSelect) {
	Coord pos;

	if (bSelect ) {
		KAssert(_lpSelected == NULL);
		double dTimer = _fScaleDuration * ((_fMaxScale - _lpCounterScale->getCurrentValue()) / (_fMaxScale - 1.0));
		_lpCounterScale->startCounter(_lpCounterScale->getCurrentValue(),_fMaxScale,0,dTimer,K_COUNTER_LINEAR);
		_lpSelected = this;
		_fSelectedPos = Mouse;
		_fromPos = _posGrid;
	} else {
		KAssert(_lpSelected == this);
		double dTimer = _fScaleDuration * ((_lpCounterScale->getCurrentValue() - 1.0) / (_fMaxScale - 1.0));
		_lpCounterScale->startCounter(_lpCounterScale->getCurrentValue(),1.0,0,dTimer,K_COUNTER_LINEAR);
		
		Coord swapcurpos;
		if (_lpSwaped != NULL) {
			swapcurpos = Coord(_lpSwaped->_x,_lpSwaped->_y);
		}
		if (abs(_fSelectedPosDelta)>(_w+_lpGrid->GetInnerGrid())/2) {
			Coord curpos = Coord(_x,_y);
			SwapGridPart(_posGrid,_lpGrid->GetSelection(this->Center()));
			this->Move(curpos);
		}
		if (_lpSwaped != NULL) { // Repositionement du swap
			_lpSwaped->Move(swapcurpos);
			_lpSwaped->Move(_lpGrid->GetCellPos(_lpSwaped->_posGrid),200);
		}
		Move(_lpGrid->GetCellPos(_posGrid),_fScaleDuration);
		_fromPos = Coord(-1,-1);

		_lpSwaped = NULL;
		_lpSelected = NULL;
	}
}
bool EMatch3Part::IsSelected() {
	return this == _lpSelected;
}

void EMatch3Part::SetGridPos(int col, int line) {
	_posGrid.x = col;
	_posGrid.y = line;
	Move(_lpGrid->GetCellPos(_posGrid));
}
void EMatch3Part::PreFall() {
	EMatch3Part *tmpPart;
   
	if (_lpCounterPreFall != NULL) {
		// Changement d'orientation du plateau
		if (_preFallDir != _vDir[EMiniGame::EAST]) {
			_lpCounterPreFall =  NULL;	
			_preFallDir = Coord(0,0);
			_fallDir= Coord(0,0);
			_preFallOffset = _preFallDir;
			return;
		}
		// Decompte de prefall
		if (!_lpCounterPreFall->isCompleted()) {
			_preFallOffset = _preFallDir * (float)sin(_lpCounterPreFall->getCurrentValue() + _randomPreFallOffset);
			return;
		}
		
      KAssert(_fallDir != Coord(0,0));
		// Gestion de la fin de prefall
		tmpPart = GetPart(_posGrid + _fallDir);
		if (tmpPart != NULL && _lpCounterPreFall == tmpPart->_lpCounterPreFall) {
			// Fais partie d'une pile de prefall 
			// ne rien faire,  attendre l'appel sur le bas de la  pile
			return;
		}
		
		// Bas de la pile prefall lance la chute pour la pile
		Fall();
		
		////netoyage de la pile de prefall
		tmpPart = this;
      KCounter *refCounter = this->_lpCounterPreFall;
		while (tmpPart != NULL && tmpPart->_lpCounterPreFall == refCounter) {
			tmpPart->_lpCounterPreFall =  NULL;	
			tmpPart->_preFallDir = Coord(0,0);
			tmpPart->_preFallOffset = Coord(0,0);
			tmpPart = GetPart(tmpPart->_posGrid - _fallDir);
		}
	} else if (GetState() == STABLE && ShouldFall()) {
		// Debut de prefall
		_preFallDir = _vDir[EMiniGame::EAST];
		_fallDir = _vDir[EMiniGame::SOUTH];
		_lpPreFallCounterList.push_back(new KCounter);
		_lpCounterPreFall = *(_lpPreFallCounterList.rbegin());
		_lpCounterPreFall->startCounter(0,3*3.14159265358979323846,0,_fPreFallDuration,K_COUNTER_LINEAR);
      _randomPreFallOffset = 0;

		// initiation du prefall pour toutes les pièces supérieures et n'etant pas en chute
		tmpPart = this;
		while (tmpPart != NULL && tmpPart->GetState() < FALL) {
			if (tmpPart->GetState() == PREFALL && tmpPart->_preFallDir != _preFallDir)
				break;
			tmpPart->_randomPreFallOffset = _randomPreFallOffset;
			_randomPreFallOffset+=GetRandom(0,3.14159265358979323846);
			tmpPart->_preFallDir = _preFallDir;
			tmpPart->_fallDir = _fallDir;
			tmpPart->_lpCounterPreFall = _lpCounterPreFall;

			tmpPart = GetPart(tmpPart->_posGrid - _fallDir);
		}
	} else if (GetState() == FALL) {
		Fall();
	}
}
void EMatch3Part::Fall() {
	EMatch3Part *tmpPart;
	Coord tmpPos;

	if (_lpCounterFall != NULL) {
		if (!_lpCounterFall->isCompleted()) {
			_fallOffset = _fallDir * -(_lpCounterFall->getCurrentValue());
			return;
		}
		// Suite d'une chute mais changement de sens du plateau
		if (_fallDir != Coord(0,0) && _fallDir != _vDir[EMiniGame::SOUTH]) {
			_fallOffset = Coord(0,0);
			_lpCounterFall = NULL;
			return;
		}
		// Appartien a une pile de chute
		// Gestion de la fin de prefall
		tmpPart = GetPart(_posGrid + _fallDir);
		if (tmpPart != NULL && _lpCounterFall == tmpPart->_lpCounterFall) {
			// Fais partie d'une pile de chute 
			// ne rien faire,  attendre l'appel sur le bas de la  pile
			return;
		}
		if (ShouldFall()) {
			//Redemarage du compteur
			_fallOffset = _lpGrid->GetCellPos(_posGrid) - _lpGrid->GetCellPos(_posGrid + _fallDir);
			_lpCounterFall->startCounter(abs(_fallOffset.x)+abs(_fallOffset.y),0,0,_fFallDuration,K_COUNTER_LINEAR);
			_fallOffset = _fallDir * -(_lpCounterFall->getCurrentValue()) ;
			
			tmpPart = GetPart(_posGrid - _fallDir);
			SwapGridPart(_posGrid,_posGrid + _fallDir);
			
			// Enchainement de chute
			while (tmpPart != NULL && _lpCounterFall == tmpPart->_lpCounterFall) {
				tmpPart->_fallOffset = _fallOffset;

				tmpPos = tmpPart->_posGrid;
				tmpPart = GetPart(tmpPart->_posGrid - _fallDir);
				SwapGridPart(tmpPos,tmpPos + _fallDir);
			}
		} else {
			// Nettoyage de la pile de chute
			KCounter *refCounter = _lpCounterFall;
			tmpPart = this;
			while (tmpPart != NULL && tmpPart->_lpCounterFall == refCounter ) {
				tmpPart->_lpCounterFall = NULL;
				tmpPart->_fallOffset = Coord(0,0);
				
				// Si sommet de la pile petit saut
				if (GetPart(tmpPart->_posGrid - _fallDir) == NULL) {
					tmpPart->_lpCounterBump = new KCounter;
					tmpPart->_lpCounterBump->startCounter(0,3.14159265358979323846,0,_fBumpDuration,K_COUNTER_EASEIN);
					break;
				}
				tmpPart = GetPart(tmpPart->_posGrid - _fallDir);
			}
		}
	} else if ( GetState() == PREFALL) {
		if (!ShouldFall() ||
			(GetState() == FALL && _fallDir != _vDir[EMiniGame::SOUTH])) {
			Move(_lpGrid->GetCellPos(_posGrid));
			return;
		}
		
		_fallDir = _vDir[EMiniGame::SOUTH];

		_lpFallCounterList.push_back(new KCounter);
		_lpCounterFall = *(_lpFallCounterList.rbegin());
      _fallOffset = _lpGrid->GetCellPos(_posGrid) - _lpGrid->GetCellPos(_posGrid + _fallDir);
		_lpCounterFall->startCounter(abs(_fallOffset.x)+abs(_fallOffset.y),0,0,_fFallDuration*3,K_COUNTER_EASEIN);
		
		_fallOffset = _fallDir * -(_lpCounterFall->getCurrentValue()) ;
		
		tmpPart = GetPart(_posGrid - _fallDir);
		SwapGridPart(_posGrid,_posGrid + _fallDir);
		// Enchainement de chute
		while (tmpPart != NULL && _lpCounterPreFall == tmpPart->_lpCounterPreFall) {
			tmpPart->_lpCounterFall = _lpCounterFall;
			tmpPart->_fallOffset = _fallOffset;
			tmpPart->_fallDir = _fallDir;
			
			tmpPos = tmpPart->_posGrid;
			tmpPart = GetPart(tmpPart->_posGrid - _fallDir);
			SwapGridPart(tmpPos,tmpPos + _fallDir);
		}

      ESoundBank::getSound("mg_match3_startfall")->playSample();
	}
}

bool EMatch3Part::CouldMove(Coord toCell) {
	EMatch3Part *tmp = NULL;

	// Mouvement impossible si la pièce doit tomber ou es en limite de jeux
	if (ShouldFall() )
		return false;

	if (!_lpGrid->CheckQCell(toCell))
		return false;

	if (HasMatch())
		return false;

	// Mouvement impossible si cellule cible es occupé par une pièce en chute
	tmp = GetPart(toCell);
	if (tmp != NULL) {
		if (tmp->GetState() >= FALL )
			return false;
	}

	// Mouvement impossible si la cellule en dessous de la cellule cible es occupé par une pièce en chute
	tmp = GetPart(toCell + _vDir[EMiniGame::SOUTH]);
	if (tmp != NULL) {
		if (tmp->GetState() == FALL)
			return false;
	}
	return true;
}

void EMatch3Part::UpdateSelectedPos() {
	Coord tmpPosDelta = Mouse - _fSelectedPos;
	_fSelectedPosDelta = tmpPosDelta.x * _vDir[EMiniGame::EAST].x + tmpPosDelta.y * _vDir[EMiniGame::EAST].y ;
	float tmpDelta = _fSelectedPosDelta;
	
	EMatch3Part *nextSwap = NULL;

	EMiniGame::Direction dir = EMiniGame::EAST;
	if (tmpDelta < 0)
		dir = EMiniGame::WEST;
	int DirDeltaS = (EMiniGame::GetDirVector(dir).x + EMiniGame::GetDirVector(dir).y);

	Coord nextPos = _posGrid;
	// Gestion du deplacement, ratrapage de déplacement, swap et marquage swap prévisionel
	while (tmpDelta*EMiniGame::GetDirVector(dir).x + tmpDelta*EMiniGame::GetDirVector(dir).y > 0) {
		// Récupération position future et marquage swap prévisionel
		nextPos = _posGrid + _vDir[dir];

		// Mouvement impossible si la pièce doit tomber et ne retourne pas a sa position initial
		if (!CouldMove(nextPos)) {
			_fSelectedPosDelta = 0;
			nextSwap = NULL;
			break;
		}

		nextSwap = _lpGame[nextPos.y][nextPos.x];
		// du swap si le deplacement es supérieur a la largeur de la piece
		if (abs(tmpDelta) > _w+_lpGrid->GetInnerGrid()) {
			_fSelectedPos = _fSelectedPos + (_lpGrid->GetCellPos(nextPos) - _lpGrid->GetCellPos(_posGrid));
			_fromPos = _posGrid;
			SwapGridPart(_posGrid,nextPos);
		}

		_fSelectedPosDelta = tmpDelta;
		tmpDelta = tmpDelta - (_w+_lpGrid->GetInnerGrid())*DirDeltaS;
	}
	

	// Decalage synchrone a la pièce selectionné pour le prochain swap
	if (nextSwap != NULL) {
		Coord swapPos = _vDir[EMiniGame::EAST] * _fSelectedPosDelta;
		swapPos = _lpGrid->GetCellPos(nextSwap->_posGrid) - swapPos;
		nextSwap->_x = swapPos.x;
		nextSwap->_y = swapPos.y;
	}
	if (_lpSwaped != NULL && nextSwap != _lpSwaped) {
		// Repositionnement de l'ancien swap
		_lpSwaped->SetGridPos(_lpSwaped->_posGrid);
	}
	_lpSwaped = nextSwap;
	
	Coord partPos = _lpGrid->GetCellPos(_posGrid) + ((_vDir[EMiniGame::EAST])*_fSelectedPosDelta);
	_x = partPos.x;
	_y = partPos.y;
	
	if (ShouldFall() || HasMatch()) {
		Select(false);
	}
}

KCounter *_lpOldestCounter = NULL;
bool EMatch3Part::Match(KGraphic *symb,Coord dir,bool test) {
   EMatch3Part *tmpPart;

   if (GetState() != STABLE && GetState() != PREPOP)
      return false;

   if (symb==NULL) {
      if (_lpCounterPrePop != NULL)
         return false;

      _lpOldestCounter = NULL;

      // Debut du test de match a partir de la piece courante
      _lpMatchList.clear();

      // Vérification des voisins
      int listSize;
      // Test en largeur puis hauteur
      for (int i = 0; i<2; i++) {
         listSize = _lpMatchList.size();
         tmpPart = GetPart(_posGrid + _vDir[i]);
         if (tmpPart != NULL)
            tmpPart->Match(_lpImg,_vDir[i],test);
         tmpPart = GetPart(_posGrid + _vDir[i]*-1);
         if (tmpPart != NULL)
            tmpPart->Match(_lpImg,_vDir[i]*-1,test);

         if (!(_lpMatchList.size() - listSize >= 2)) {
            for (; _lpMatchList.size() > listSize; _lpMatchList.pop_back()) {
            }
         }
      }
      if (_lpMatchList.size() < 2) {
         _lpMatchList.clear();
         return false;
      }

      if (test == true) {
         _lpMatchList.clear();
         return true;
      }

      
      if (_lpOldestCounter == NULL) {
         _lpMatchCounterList.push_back(new KCounter);
         _lpCounterPrePop = *(_lpMatchCounterList.rbegin());
         _lpCounterPrePop->startCounter(0,1,0,_fPrePopDuration,K_COUNTER_LINEAR);
      } else {
         _lpCounterPrePop = _lpOldestCounter;
      }

      std::list< EMatch3Part * >::iterator it;
      float incr=1000;
      _fCurrentPopDuration = _fPopDuration-incr;
      for (it = _lpMatchList.begin(); it != _lpMatchList.end(); it++) {
         incr = incr/2;
         (*it)->_fCurrentPopDuration = _fCurrentPopDuration - incr; 
         (*it)->_lpCounterPrePop = _lpCounterPrePop;
      }

      _lpMatchList.clear();
      return true;
   } else {
      // Piece pas de la meme couleur
      if (symb != _lpImg) 
         return false;

      // Test si la pièce a déjà été visité
      if (_lpMatchList.size() > 0) {
         std::list< EMatch3Part * >::iterator it;
         for (it = _lpMatchList.begin(); it != _lpMatchList.end(); it++) {
            if ((*it) == this)
               return false;
         }
      }

      if (_lpOldestCounter == NULL)
         _lpOldestCounter = _lpCounterPrePop;
      else if (_lpCounterPrePop != NULL && 
               _lpCounterPrePop->getCurrentValue() > _lpOldestCounter->getCurrentValue())
         _lpOldestCounter = _lpCounterPrePop;

      // Continue le test dans la direction d'origine
      _lpMatchList.push_back(this);
      tmpPart =  GetPart(_posGrid + dir);
      if (tmpPart != NULL)
         tmpPart->Match(symb,dir,test);
      return true;

   }
}


// Preload images et sons
void EMiniJeuMatch3::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_match3_symb01","png",true,true);
   EImageBank::getImage("mg_match3_symb02","png",true,true);
   EImageBank::getImage("mg_match3_symb03","png",true,true);
   EImageBank::getImage("mg_match3_symb04","png",true,true);
   EImageBank::getImage("mg_match3_symb05","png",true,true);
   EImageBank::getImage("mg_match3_bg","png",true,true);
   EImageBank::getImage("mg_match3_clouds","png",true,true);

   ESoundBank::getSound("mg_match3_explode",false,true);
   ESoundBank::getSound("mg_match3_fall",false,true);
}
