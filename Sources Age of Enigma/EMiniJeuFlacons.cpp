#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "FontBank.h"
#include "EGeometry.h"

#include "EMiniJeuFlacons.h"



using namespace EMiniGame;

EMiniJeuFlacons::EMiniJeuFlacons(void)
{
	SetTextsTrad("FLACONS");
	SetGameType(MINIJEU_DIRECTGAME);
	SetBackGround(EImageBank::getImage("mg_flaconsbg.jpg"));
	SetGameNumber(15);

	_lpImgArrow = EImageBank::getImage("mg_flacons_arrow","png");

	_lpGridGame = NULL;
	_lpSelected = NULL;

	GenerateGame();

	Reset();
}

EMiniJeuFlacons::~EMiniJeuFlacons(void)
{
	for (int i=0; i<_vSlots.size();i++) {
		XDELETE(_vSlots[i]);
	}
	for (int i=0; i< _vFlacons.size(); i++) {
		XDELETE(_vFlacons[i]);
	}
	XDELETE(_lpGridGame);
	_lpSelected = NULL;
}

/** 3 steps rendering **/
void EMiniJeuFlacons::GameCheck ()
{
	Coord pos;

	if (!Mouse.LeftEvent() && !Mouse.LeftReleaseEvent())
		return;

	for (int i=0; i<_vFlacons.size(); i++) {
		if (_vFlacons[i]->IsMoving())
			return;
	}

	for (int i=0; i<_vFlacons.size(); i++) {
		_vFlacons[i]->Check();
	}
}

void EMiniJeuFlacons::GameLogic ()
{
	Coord pos = Coord(-1,-1);
	EFlaconsPart *tmp;
	int index;

	for (index=0 ; index<_vFlacons.size(); index++) {
		tmp= _vFlacons[index];
		if (tmp->IsMoving() && tmp->TransformComplete()) {
			pos = _lpGridGame->GetSelection(tmp->Center());
			pos = pos + GetDirVector(tmp->GetDir());
			if (!ESoundBank::getSound("mg_rotate")->isPlaying()) {
				ESoundBank::getSound("mg_rotate")->playSample();
			}
			break;
		}
	}


	if (pos == Coord(-1,-1))
		return;			// Aucune pièce en déplacement

	if (_lpGridGame->CheckQCell(pos)) {
		bool collide = false;
		for (int i=0; i<_vFlacons.size(); i++) {
			if (index==i)
				continue;

			if (pos == _lpGridGame->GetSelection(_vFlacons[i]->Center())) {
				collide = true;
				break;
			}
		}
		if (!collide) {
			tmp->Move(_lpGridGame->GetCellPos(pos),200);
			return;
		}
	}
	// La pièce ne peux pas allez plus loin
	tmp->EndMove();
	IsResolved();
}

void EMiniJeuFlacons::GameDrawSet()
{	
	_lpGridGame->DrawBaseGrid(_lpImgArrow,0.2);

	for (int i=0; i<_vSlots.size(); i++) {
		_vSlots[i]->Draw();
	}
	for (int i=0; i<_vFlacons.size(); i++) {
		_vFlacons[i]->Draw();
	}
}

void EMiniJeuFlacons::GameReset() {
	for (int i=0; i<_vFlacons.size(); i++) {
		_vFlacons[i]->Reset();
	}
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuFlacons::GameIsResolve()
{
	
	for (int i=0; i<_vFlacons.size(); i++) {
		if (_lpGridGame->GetSelection(_vFlacons[i]->Center()) != _lpGridGame->GetSelection(_vSlots[i]->Center()))
			return false;
	}
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuFlacons::GameResolve()
{
	for (int i=0; i<_vFlacons.size(); i++) {
		_vFlacons[i]->Hide();
		_vFlacons[i]->Move(_lpGridGame->GetCellPos(_lpGridGame->GetSelection(_vSlots[i]->Center())));
		_vFlacons[i]->FadIn(500);
	}
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuFlacons::GameSetHint(int nHintLevel)
{

	switch (nHintLevel) {
		case MINIJEU_HINT1:
			ShowHint(MINIJEU_HINT1);
			Reset(); 
			break;
		case MINIJEU_HINT2:
			ShowHint(MINIJEU_HINT2);
			Reset();
			SetElemPos(_vFlacons[0],2,3);_vFlacons[0]->RegisterPosForReset(); 
			SetElemPos(_vFlacons[3],1,0);_vFlacons[3]->RegisterPosForReset(); 
			SetElemPos(_vFlacons[2],3,1);_vFlacons[2]->RegisterPosForReset(); 
			
			break;
		case MINIJEU_RESOLVED:
			Resolve();
			break;
		default:
			KAssert(false);
			break;
	}
}

void EMiniJeuFlacons::SetElemPos(EMiniJeuPart *elem,int x,int y) {
	elem->Move(_lpGridGame->GetCellPos(x,y));
}

void EMiniJeuFlacons::GenerateGame() {
	_vSlots.clear();
	for (int i=0; i<_vSlots.size();i++) {
		XDELETE(_vSlots[i]);
	}
	_vSlots.push_back(new EMiniJeuPart(EImageBank::getImage("mg_flacons_slot_3","png")));
	_vSlots.push_back(new EMiniJeuPart(EImageBank::getImage("mg_flacons_slot_2","png")));
	_vSlots.push_back(new EMiniJeuPart(EImageBank::getImage("mg_flacons_slot_1","png")));
	_vSlots.push_back(new EMiniJeuPart(EImageBank::getImage("mg_flacons_slot_4","png")));
	for (int i=0; i<_vSlots.size();i++) {
		_vSlots[i]->SwitchDrag();
		_vSlots[i]->SwitchRotation();
	}

	for (int i=0; i<_vFlacons.size();i++) {
		XDELETE(_vFlacons[i]);
	}
	_vFlacons.clear();
	_vFlacons.push_back(new EFlaconsPart(EImageBank::getImage("mg_flacons_3","png"),
		EImageBank::getImage("mg_flacons_shadow_3","png")));
	_vFlacons.push_back(new EFlaconsPart(EImageBank::getImage("mg_flacons_2","png"),
		EImageBank::getImage("mg_flacons_shadow_2","png")));
	_vFlacons.push_back(new EFlaconsPart(EImageBank::getImage("mg_flacons_1","png"),
		EImageBank::getImage("mg_flacons_shadow_1","png")));
	_vFlacons.push_back(new EFlaconsPart(EImageBank::getImage("mg_flacons_4","png"),
		EImageBank::getImage("mg_flacons_shadow_4","png")));
	_nGameSize = 4;

	XDELETE(_lpGridGame);
	_lpGridGame = new Grid(_nGameSize,_nGameSize,_lpImgArrow->getWidth(),_lpImgArrow->getHeight(),0,0);
	_lpGridGame->Move((_nW - _lpGridGame->width()) / 2, (_nH - _lpGridGame->height())/2);
	_lpGridGame->Move(270,103);

	SetElemPos(_vSlots[0],0,1);
	SetElemPos(_vSlots[1],2,0);
	SetElemPos(_vSlots[2],3,2);
	SetElemPos(_vSlots[3],1,3);

	SetElemPos(_vFlacons[0],0,0);_vFlacons[0]->RegisterPosForReset(); 
	SetElemPos(_vFlacons[1],3,0);_vFlacons[1]->RegisterPosForReset();
	SetElemPos(_vFlacons[2],3,3);_vFlacons[2]->RegisterPosForReset();
	SetElemPos(_vFlacons[3],0,3);_vFlacons[3]->RegisterPosForReset();
}

EFlaconsPart::EFlaconsPart(KGraphic *img, KGraphic *shadow): EMiniJeuPart(img,1) {
	_lpImgArrow = EImageBank::getImage("mg_flacons_arrow","png");
	_bSelected = false;
	_dMoveTo = DEADEND;

	_fResetX = _fResetY = 0;
	_lpImgShadow = shadow;
	KAssert(_lpImgShadow != NULL);
	_shadowSize = Coord(_lpImgShadow->getWidth(),_lpImgShadow->getHeight());
	_shadowOffset = Coord((float)(_lpImg->getWidth() - _shadowSize.x)/2, (float)(_lpImg->getHeight() - _shadowSize.y)/2);
	_bSkipDraw = true;
}
EFlaconsPart::~EFlaconsPart(void) {
}

void EFlaconsPart::Reset() {
	_bSelected = false;
	_dMoveTo = DEADEND;
	FinishMove();
	Hide();
	_x = _fResetX;
	_y = _fResetY;
	FadIn(200);
}

void EFlaconsPart::Check() {
	if (!_bSelected && _dMoveTo != DEADEND)
		return;						// En mouvement

	if (!_bSelected) {				
		// Selection de la pièce
		if (Mouse.LeftEvent() && IsMouseOver()) { 
			Mouse.LeftEventClose();
			_bSelected = true;
			ESoundBank::getSound("mg_select_deselect")->playSample();
		}
	} else if (Mouse.LeftReleaseEvent()) {
		// Choix de la direction
		Mouse.LeftEventClose();
		_dMoveTo = GetCurSelDir();
		_bSelected = false;
		ESoundBank::getSound("mg_select_deselect")->playSample();
	}
}
void EFlaconsPart::Draw() {
	EMiniJeuPart::Draw();

	_lpImgShadow->blitAlphaRectFxF(0, 0, _shadowSize.x, _shadowSize.y, _x + _shadowOffset.x, _y + _shadowOffset.y, 0, _fScale, _fBlend );
	_lpImg->blitAlphaRectFxF(0, 0, _w, _h, _x, _y, 0, _fScale, _fBlend );

	if (_bSelected) {
		_dMoveTo = GetCurSelDir();
	}
	DrawSelDir();
}
bool EFlaconsPart::IsMoving() {
	return !_bSelected && _dMoveTo != DEADEND;
}
void EFlaconsPart::EndMove() {
	_dMoveTo = DEADEND;
	ESoundBank::StopSample("mg_rotate");
}
EMiniGame::Direction EFlaconsPart::GetCurSelDir() {
	if (!_bSelected)
		return DEADEND;			// La pièce n'es pas selectionnée

	double dist = EGeometry::Distance(Center(),Mouse);
	if (dist < _w/2)
		return DEADEND;

	Coord vector = EGeometry::Vector(Center(),Mouse);
	if (abs(vector.x) >= abs(vector.y)) {
		if (vector.x >= 0) {
			return EAST;
		} else {
			return WEST;
		}
	} else {
		if (vector.y >= 0) {
			return SOUTH;
		} else {
			return NORTH;
		}
	}
}
void EFlaconsPart::DrawSelDir() {
	if (!_bSelected || _dMoveTo == DEADEND)
		return;

	float angle;
	switch (_dMoveTo) {
			case WEST:
				angle = 180;
				break;
			case SOUTH:
				angle = 270;
				break;
			case EAST:
				angle = 0;
				break;
			case NORTH:
				angle = 90;
				break;
			default:
				return;
	}
	_lpImgArrow->blitAlphaRectFx(0,0,_w,_h,_x,_y,angle,1,1);
}
EMiniGame::Direction EFlaconsPart::GetDir() {
	return _dMoveTo;
}

void EFlaconsPart::RegisterPosForReset() {
	_fResetX = _x;
	_fResetY = _y;
}

// Preload images et sons
void EMiniJeuFlacons::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_flaconsbg.jpg","",true,true);
   EImageBank::getImage("mg_flacons_arrow","png",true,true);
   EImageBank::getImage("mg_flacons_slot_3","png",true,true);
   EImageBank::getImage("mg_flacons_slot_2","png",true,true);
   EImageBank::getImage("mg_flacons_slot_1","png",true,true);
   EImageBank::getImage("mg_flacons_slot_4","png",true,true);
   EImageBank::getImage("mg_flacons_3","png",true,true);
   EImageBank::getImage("mg_flacons_shadow_3","png",true,true);
   EImageBank::getImage("mg_flacons_2","png",true,true);
   EImageBank::getImage("mg_flacons_shadow_2","png",true,true);
   EImageBank::getImage("mg_flacons_1","png",true,true);
   EImageBank::getImage("mg_flacons_shadow_1","png",true,true);
   EImageBank::getImage("mg_flacons_4","png",true,true);
   EImageBank::getImage("mg_flacons_shadow_4","png",true,true);
   EImageBank::getImage("mg_flacons_arrow","png",true,true);

   ESoundBank::getSound("mg_rotate",false,true);
}
