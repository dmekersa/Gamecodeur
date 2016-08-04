#include "KPTK.h"
#include "KGraphic.h"
#include "KRandom.h"
#include "MyGame.h"
#include "SoundBank.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"

#include "EMiniJeuDice.h"

EMiniJeuDice::EMiniJeuDice(void)
{
	SetTextsTrad("DICE");
	SetGameType(MINIJEU_SUBMITGAME);
	SetFeatures(MINIJEU_LOGIC);
	SetBackGround(EImageBank::getImage("edice_bg.jpg"));
	SetGameNumber(17);
	
	// initialisation du masque/patron pour le jeux
	_lpDicePatron.push_back(Coord(1,0));
	_lpDicePatron.push_back(Coord(1,1));
	_lpDicePatron.push_back(Coord(1,2));
	_lpDicePatron.push_back(Coord(2,3));
	_lpDicePatron.push_back(Coord(0,1));
	_lpDicePatron.push_back(Coord(2,2));
	
	// initialisation de la solution
	_lpDiceSolution.push_back(new EDicePart("edice1.png"));
	_lpDiceSolution.push_back(new EDicePart("edice2.png",1));
	_lpDiceSolution.push_back(new EDicePart("edice3.png"));
	_lpDiceSolution.push_back(new EDicePart("edice4.png"));
	_lpDiceSolution.push_back(new EDicePart("edice5.png"));
	_lpDiceSolution.push_back(new EDicePart("edice6.png"));
	_lpDiceSolution[1]->SwitchRotation();
	
	_lpDiceSolutionSymbol.push_back(EImageBank::getImage("edice_symbol1.png"));
	_lpDiceSolutionSymbol.push_back(EImageBank::getImage("edice_symbol2.png"));
	_lpDiceSolutionSymbol.push_back(EImageBank::getImage("edice_symbol3.png"));
	_lpDiceSolutionSymbol.push_back(EImageBank::getImage("edice_symbol4.png"));
	_lpDiceSolutionSymbol.push_back(EImageBank::getImage("edice_symbol5.png"));
	_lpDiceSolutionSymbol.push_back(EImageBank::getImage("edice_symbol6.png"));
	
	float imgSize=(_lpDiceSolution[0]->Width());

	//for (int i=0;i<6;i++) {
	//	// Laisse de coter la piËce de rÈfÈrence
	//	if (i==2) continue;
	//	// Peuple la liste servant a la gÈnÈration alÈatoire du set de piËce initial
	//	_lpRandomPartPool.push_back(new EDicePart(_lpDiceSolution[i]));
	//}

	_lpRandomPartPool.push_back(new EDicePart(_lpDiceSolution[1]));
	_lpRandomPartPool.push_back(new EDicePart(_lpDiceSolution[4]));
	_lpRandomPartPool.push_back(new EDicePart(_lpDiceSolution[3]));
	_lpRandomPartPool.push_back(new EDicePart(_lpDiceSolution[0]));
	_lpRandomPartPool.push_back(new EDicePart(_lpDiceSolution[5]));

	// Initialisation et positionnement des grilles
	//_lpGridDice=new Grid(3,4,imgSize,imgSize,3.0f,2.0f);
	//_lpGridDice->Move((_nW-_lpGridDice->width())/2,((_nH-_lpGridDice->height())-10)/2);
	_lpGridDice=new Grid(3,4,imgSize,imgSize,0,0);
	_lpGridDice->Move(383,98);
	_lpGame = std::vector< std::vector< EDicePart * > > (4,
		std::vector< EDicePart * > (3, (EDicePart *) NULL));

	//_lpGridPartSet=new Grid(5,1,imgSize,imgSize,3.0f,2.0f);
	//_lpGridPartSet->Move((_nW-_lpGridPartSet->width())/2,_lpGridDice->_yPos+_lpGridDice->height()+30);
	
	_lpGridPartSet=new Grid(5,1,imgSize,imgSize,0,4);
	_lpGridPartSet->Move(285,518);

	// Peuplement du set de piËce initial
	_lpPartSet = std::vector< EDicePart * > (5, (EDicePart *) NULL);
	for (int i=0;i<5;i++) {
		_lpPartSet[i]= GetRandomPart();	
		_lpPartSet[i]->Move(_lpGridPartSet->GetCellPos(i,0));
	}

	// Positionnement de la piËce de rÈfÈrence
	_lpGame[2][1]=new EDicePart(_lpDiceSolution[2]);
	_lpGame[2][1]->SwitchDrag();
	_lpGame[2][1]->SwitchRotation();
	_lpGame[2][1]->Move(_lpGridDice->GetCellPos(1,2));

	
	target = KPTK::createKGraphic();
	_lpGrabedPiece= NULL;
}

EMiniJeuDice::~EMiniJeuDice(void)
{
	XDELETE(_lpGridDice);
	for (int i=0;i<4;i++) {
		for (int j=0;j<3;j++) {
			if (_lpGame[i][j])
				_lpGame[i][j]->GetImg()->setBlitColor(1,1,1,1);
			XDELETE(_lpGame[i][j]);
		}
	}

	XDELETE(_lpGridPartSet);
	for (int i=0;i<5;i++) {
		XDELETE(_lpPartSet[i]);
	}
	
	if (_lpGrabedPiece)
		XDELETE(_lpGrabedPiece);

	for (int i=0;i<6;i++)
		XDELETE(_lpDiceSolution[i]);

	XDELETE(target);
}

/** 3 steps rendering **/
void EMiniJeuDice::GameCheck ()
{
	EDicePart* tmpPart = NULL;
	struct Coord grid;
	std::list< Coord >::iterator itDice;

	if (!(Mouse.LeftEvent() || Mouse.RightEvent()))
		return;

	if (_lpGrabedPiece != NULL) {
		_lpGrabedPiece->Check();
		
		if (!_lpGrabedPiece->IsGrab()) {
			// Piece relachÈ
			if (!(_lpGridDice->IsMouseOver())) {
					// la piece n'es au dessu d'aucune zone, ne rien faire
					_lpGrabedPiece->ResetPos();
					if (_lpGridPartSet->IsOver(_lpGrabedPiece->Pos())) {
						_lpPartSet[(int)_lpGridPartSet->GetSelection(_lpGrabedPiece->Pos()).x] = _lpGrabedPiece;
						_lpGrabedPiece = NULL;
						ESoundBank::getSound("mg_select_deselect")->playSample();
					} else {
						_lpGrabedPiece->Grab();
					}
					return;
			}

			if (_lpGridPartSet->IsOver(_lpGrabedPiece->Pos())) {
				// piece au dessus du set
				grid = _lpGridPartSet->GetSelection(_lpGrabedPiece->Pos());
				_lpGrabedPiece->Move(_lpGridPartSet->GetCellPos((int)grid.x,(int)grid.y));
				_lpPartSet[(int)grid.x]=_lpGrabedPiece;
				_lpGrabedPiece = NULL;
				ESoundBank::getSound("mg_select_deselect")->playSample();
				return;
			}

			if (_lpGridDice->IsMouseOver()) { //_lpGrabedPiece->Pos())) {
				grid=_lpGridDice->GetSelection(Mouse);
				for (int i=0;i<=6;i++) {
					if (i==6) {
						_lpGrabedPiece->Grab(false);
						return;
					} else if (grid == _lpDicePatron[i]) {
						break;
					}
				}

				KAssert(grid.x >=0 && grid.y >=0 && grid.x<3 && grid.y <4);
				if (_lpGame[(int)grid.y][(int)grid.x] != NULL) {
					tmpPart=_lpGame[(int)grid.y][(int)grid.x];
					if (!tmpPart->IsDragable()) {
						ESoundBank::getSound("mg_forbid")->playSample();
						Mouse.LeftEventClose();
						_lpGrabedPiece->Grab();
						return;
					}
					_lpGame[(int)grid.y][(int)grid.x] = _lpGrabedPiece;

					grid=tmpPart->Pos();
					tmpPart->Move(_lpGrabedPiece->Pos());
					if (_lpGridPartSet->IsOver(tmpPart->Pos()))
						_lpPartSet[(int)_lpGridPartSet->GetSelection(tmpPart->Pos()).x] = tmpPart;
					_lpGrabedPiece->Move(grid);
					_lpGrabedPiece = tmpPart;

					_lpGrabedPiece->Grab();
					ESoundBank::getSound("mg_swap")->playSample();
					return;
				}
				_lpGrabedPiece->Move(_lpGridDice->GetCellPos((int)grid.x,(int)grid.y));
				_lpGame[(int)grid.y][(int)grid.x]=_lpGrabedPiece;
				ESoundBank::getSound("mg_select_deselect")->playSample();
			} 
			_lpGrabedPiece=NULL;
		}
		return;
	}

	grid=_lpGridDice->GetSelection(Mouse.x,Mouse.y);
	if (grid.x>=0 && Mouse.Event()) {
		
		tmpPart=_lpGame[(int)grid.y][(int)grid.x];
		if (tmpPart==NULL)
			return;
		bool curState = tmpPart->IsGrab();
		tmpPart->Check();
		if (tmpPart->IsGrab()) {
			_lpGame[(int)grid.y][(int)grid.x]=NULL;
			_lpGrabedPiece=tmpPart;
			ESoundBank::getSound("mg_select_deselect")->playSample();
			return;
		} 
		if (tmpPart->IsGrab() == curState)
			ESoundBank::getSound("mg_forbid")->playSample();
	}

	grid=_lpGridPartSet->GetSelection(Mouse.x,Mouse.y);
	if (grid.x>=0 && Mouse.Event()) {
		tmpPart=_lpPartSet[(int)grid.x];
		if (tmpPart==NULL)
			return;
		tmpPart->Check();
		if (tmpPart->IsGrab()) {
			_lpPartSet[(int)grid.x]=NULL;
			_lpGrabedPiece=tmpPart;
		}
		ESoundBank::getSound("mg_select_deselect")->playSample();
	}
}

void EMiniJeuDice::GameLogic ()
{
	// Deplacement de la piece selectionnÈe
	if (_lpGrabedPiece && _lpGrabedPiece->IsGrab())
		_lpGrabedPiece->Logic();
}

void EMiniJeuDice::GameDrawSet ()
{
	EMiniJeuPart* cpiece=NULL;
	Coord posCell;
	
//	_lpGridDice->DrawBaseGrid(target,0.2);
//	_lpGridPartSet->DrawBaseGrid(target,0.2);

	for (int i=0;i<6;i++) {
		posCell = _lpDicePatron[i];
		cpiece=_lpGame[(int)posCell.y][(int)posCell.x];
		if (cpiece != NULL && cpiece->TransformComplete())
			cpiece->Draw();
	}

	for (int i=0;i<5;i++) {
		cpiece=_lpPartSet[i];
		if (cpiece != NULL && cpiece->TransformComplete())
			cpiece->Draw();
	}

	DrawDices();
}
void EMiniJeuDice::GameDrawGrab() {
	EMiniJeuPart* cpiece=NULL;
	Coord posCell;

	for (int i=0;i<6;i++) {
		posCell = _lpDicePatron[i];
		cpiece=_lpGame[(int)posCell.y][(int)posCell.x];
		if (cpiece != NULL && !cpiece->TransformComplete())
			cpiece->Draw();
	}

	for (int i=0;i<5;i++) {
		cpiece=_lpPartSet[i];
		if (cpiece != NULL && !cpiece->TransformComplete())
			cpiece->Draw();
	}
	if (_lpGrabedPiece) _lpGrabedPiece->Draw();
}

void EMiniJeuDice::DrawDices() {
	ResetWorldView();
	DrawFirstDice();
	DrawSecondDice();
	SetRelWorldView();
}
void EMiniJeuDice::DrawFirstDice() {
	KGraphic *part1,*part2, *part3;
	part1 = _lpDiceSolutionSymbol[0];
	part2 = _lpDiceSolutionSymbol[3];
	part3 = _lpDiceSolutionSymbol[5];

	float x=76,y=56;
	const MyGame *lpMyGame=(MyGame *)MyGame::getGame();
	//lpMyGame->_lpKWindow->setWorldView(x+_nX,y-_nY,0,1,false);
	lpMyGame->_lpKWindow->setWorldView(x,-y,0,1,false);

	//Coord pos1(0,36), pos2(86,50), pos3(86,140), pos4(0,125), pos5(44,0), pos6(128,13), pos7(128,100);
	Coord pos1(1,58), pos2(121,81), pos3(121,201), pos4(1,179), pos5(63,1), pos6(181,23), pos7(181,143);

	float w=part1->getWidth(), h=part1->getHeight();
	part1->blitArbitraryQuad(
		0,0, w,0, w,h,0,h,
		pos1.x,pos1.y, pos2.x,pos2.y, pos3.x,pos3.y, pos4.x,pos4.y
		);
	part2->blitArbitraryQuad(
		0,h, 0,0, w,0, w,h, 
		pos5.x,pos5.y, pos6.x,pos6.y, pos2.x,pos2.y, pos1.x,pos1.y
		);
	part3->blitArbitraryQuad(
		0,h, w,h, w,0, 0,0,
		pos2.x,pos2.y, pos6.x,pos6.y, pos7.x,pos7.y, pos3.x,pos3.y
		);

	/*part1->blitArbitraryQuad(
		0,0, w,0, w,h,0,h,
		0,-10, w,0, w,h, 0,h-10
		);
	part2->blitArbitraryQuad(
		0,h, 0,0, w,0, w,h, 
		0+35,-40, w+35,-40+10, w,0, 0,-10
		);
	part3->blitArbitraryQuad(
		0,h, w,h, w,0, 0,0,
		w,0, w+35,-40+10, w+35,h-40+10, w,h
		);*/

	lpMyGame->_lpKWindow->setWorldView((float)_nX,(float)-_nY,0,1,false);
}
void EMiniJeuDice::DrawSecondDice(){
		KGraphic *part1,*part2, *part3;
	part1 = _lpDiceSolutionSymbol[4];
	part2 = _lpDiceSolutionSymbol[2];
	part3 = _lpDiceSolutionSymbol[3];
	
	float x=76,y=271;
	const MyGame *lpMyGame=(MyGame *)MyGame::getGame();
	lpMyGame->_lpKWindow->setWorldView(x,-y,0,1,false);

	Coord pos1(1,58), pos2(121,81), pos3(121,201), pos4(1,179), pos5(63,1), pos6(181,23), pos7(181,143);

	float w=part1->getWidth(), h=part1->getHeight();
	part1->blitArbitraryQuad(
		w,h, 0,h, 0,0, w,0, 
		pos1.x,pos1.y, pos2.x,pos2.y, pos3.x,pos3.y, pos4.x,pos4.y
		);
	part2->blitArbitraryQuad(
		w,0, w,h, 0,h, 0,0, 
		pos5.x,pos5.y, pos6.x,pos6.y, pos2.x,pos2.y, pos1.x,pos1.y
		);
	part3->blitArbitraryQuad(
		0,h, 0,0, w,0, w,h,
		pos2.x,pos2.y, pos6.x,pos6.y, pos7.x,pos7.y, pos3.x,pos3.y
		);
	/*part1->blitArbitraryQuad(
		w,h, 0,h, 0,0, w,0, 
		0,-10, w,0, w,h, 0,h-10
		);
	part2->blitArbitraryQuad(
		w,0, w,h, 0,h, 0,0, 
		0+35,-40, w+35,-40+10, w,0, 0,-10
		);
	part3->blitArbitraryQuad(
		0,h, 0,0, w,0, w,h,
		w,0, w+35,-40+10, w+35,h-40+10, w,h
		);*/

	lpMyGame->_lpKWindow->setWorldView((float)_nX,(float)-_nY,0,1,false);
}

/* Pilotage du mini jeu */

// Choix de la variation (modèle ou sujet)
void EMiniJeuDice::SetVariation(int nVariation)
{
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuDice::GameIsResolve()
{
	bool bSuccess = true;

	Coord posCell;
	for (int i=0;i<6;i++) {
		posCell=_lpDicePatron[i];
		if (! (_lpGame[(int)posCell.y][(int)posCell.x] != NULL && 
			*_lpGame[(int)posCell.y][(int)posCell.x] == *_lpDiceSolution[i])) {
			bSuccess = false;
		}
	}
	return bSuccess;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuDice::GameResolve()
{
	EDicePart *part;
	Coord pos, pos2;

	Reset();
	for (int i=0; i<6; i++) {
		part = _lpDiceSolution[i];
		for (int j=0; j<5; j++) {
			if (_lpPartSet[j] != NULL && part->GetImg() == _lpPartSet[j]->GetImg()) {
				pos2 = _lpPartSet[j]->Pos();
				XDELETE( _lpPartSet[j]);
				part =  new EDicePart(part);
				pos = _lpDicePatron[i];
				_lpGame[(int)pos.y][(int)pos.x] = part;
				part->Move(pos2);
				part->Move(_lpGridDice->GetCellPos((int)pos.x,(int)pos.y),700);
				part->SwitchDrag();
				part->SwitchRotation();
			}
		}
	}
}

// Active un niveau d'indices (_nGameStatus = 1, 2 ou 3)
void EMiniJeuDice::GameSetHint(int nHintLevel)
{
	EDicePart *part;
	Coord pos, pos2;

	switch (nHintLevel) {
	case MINIJEU_HINT1:
		Reset();
		part = _lpDiceSolution[3];
		for (int i=0; i<5; i++) {
			if (_lpPartSet[i] != NULL && part->GetImg() == _lpPartSet[i]->GetImg()) {
				pos2 = _lpPartSet[i]->Pos();
				XDELETE( _lpPartSet[i]);
				part =  new EDicePart(part);
				pos = _lpDicePatron[3];
				_lpGame[(int)pos.y][(int)pos.x] = part;
				part->Move(pos2);
				part->Move(_lpGridDice->GetCellPos((int)pos.x,(int)pos.y),700);
				part->SwitchDrag();
				part->SwitchRotation();
				part->GetImg()->setBlitColor(0.7,0.7,0.7,1);
			}
		}
		ShowHint(MINIJEU_HINT1);
		break;
	case MINIJEU_HINT2:
		Reset();
		part = _lpDiceSolution[1];
		for (int i=0; i<5; i++) {
			if (_lpPartSet[i] != NULL && part->GetImg() == _lpPartSet[i]->GetImg()) {
				pos2 = _lpPartSet[i]->Pos();
				XDELETE( _lpPartSet[i]);
				part =  new EDicePart(part);
				pos = _lpDicePatron[1];
				_lpGame[(int)pos.y][(int)pos.x] = part;
				part->Move(pos2);
				part->Move(_lpGridDice->GetCellPos((int)pos.x,(int)pos.y),700);
				part->SwitchDrag();
				part->SwitchRotation();
				part->GetImg()->setBlitColor(0.7,0.7,0.7,1);
			}
		}
		ShowHint(MINIJEU_HINT2);
		break;
	case MINIJEU_RESOLVED:
		Resolve();
		_nHintLevel = MINIJEU_HINT3;
		break;
	default:
		KAssert(false);
		break;
	}
}
void EMiniJeuDice::GameReset() {
	EDicePart *part = NULL;
	for (int i=0;i<4;i++){
		for (int j=0;j<3;j++) {
			part = _lpGame[i][j];
			if (part != NULL && part->IsDragable()) {
				for (int k=0;k<5;k++) {
					if (_lpPartSet[k] == NULL) {
						_lpPartSet[k] = part;
						_lpGame[i][j] = NULL;
						part->Move(_lpGridPartSet->GetCellPos(k,0),700);
						break;
					}
				}
			}
		}
	}
}

EDicePart* EMiniJeuDice::GetRandomPart() {
	KAssert(_lpRandomPartPool.size() > 0);

	EDicePart *tmpPart = NULL;
	//int t =_lpRandomPartPool.size();
	//t=(int)(t*KRandom::getRandomFloat())%t; // Modulo pour le cas exceptionel x==1
	int t=0;

	int i=0;
	int rotate=0;

	std::list< EDicePart *>::iterator it;
	for( it=_lpRandomPartPool.begin(); it != _lpRandomPartPool.end(); it++) {
		if (i==t) {
			//rotate = (int)(4*KRandom::getRandomFloat());
			tmpPart = *it;
			tmpPart->SetOrientation(rotate);
			_lpRandomPartPool.remove(*it);
			break;
		}
		i++;
	}
	return tmpPart;
}

/* ___________________________________ */


EDicePart::EDicePart(const char* ImgPath, int nPos): EMiniJeuPart(ImgPath,nPos) {
	SetRotationDuration(300);
	SetAngleStep(90);
}

EDicePart::EDicePart(const EDicePart* a, bool rotate): EMiniJeuPart(a) {
	SetRotationDuration(300);
	if (rotate) RotateCW(false);
}

	

// Preload images et sons
void EMiniJeuDice::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("edice_bg.jpg","",true,true);
   EImageBank::getImage("edice_symbol1.png","",true,true);
   EImageBank::getImage("edice_symbol2.png","",true,true);
   EImageBank::getImage("edice_symbol3.png","",true,true);
   EImageBank::getImage("edice_symbol4.png","",true,true);
   EImageBank::getImage("edice_symbol5.png","",true,true);
   EImageBank::getImage("edice_symbol6.png","",true,true);
}
