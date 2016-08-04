#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "EGeometry.h"

#include "EMinijeuBonzai.h"

EMinijeuBonzai::EMinijeuBonzai(void)
{
	SetTextsTrad("BONZAIMG");
	SetGameType(MINIJEU_DIRECTGAME);
	SetFeatures(MINIJEU_OBSERVATION);
	SetBackGround(EImageBank::getImage("ebonzai_bg","jpg"));
	SetGameNumber(6);

	_lpImgSel = new EMiniJeuPart(EImageBank::getImage("ebonzai_selection.png"),1);
	_lpImgSel->SwitchDrag();
	_lpImgSel->SwitchRotation();

	_vPartSet.push_back(new EBonzaiPart(
		EImageBank::getImage("ebonzai_m1.png"),
		EImageBank::getImage("ebonzai_mr1.png"),
		Coord(458,379),
		Coord(460,505)));
	_vPartSet.push_back(new EBonzaiPart(
		EImageBank::getImage("ebonzai_m2.png"),
		EImageBank::getImage("ebonzai_mr2.png"),
		Coord(591,365),
		Coord(614,473)));
	_vPartSet.push_back(new EBonzaiPart(
		EImageBank::getImage("ebonzai_m3.png"),
		EImageBank::getImage("ebonzai_mr3.png"),
		Coord(517,384),
		Coord(514,521)));
	_vPartSet.push_back(new EBonzaiPart(
		EImageBank::getImage("ebonzai_m4.png"),
		EImageBank::getImage("ebonzai_mr4.png"),
		Coord(475,86),
		Coord(526,594)));
	_vPartSet.push_back(new EBonzaiPart(
		EImageBank::getImage("ebonzai_m5.png"),
		EImageBank::getImage("ebonzai_mr5.png"),
		Coord(372,106),
		Coord(345,463)));
	_vPartSet.push_back(new EBonzaiPart(
		EImageBank::getImage("ebonzai_m6.png"),
		EImageBank::getImage("ebonzai_mr6.png"),
		Coord(630,114),
		Coord(661,516)));
	_vPartSet.push_back(new EBonzaiPart(
		EImageBank::getImage("ebonzai_m7.png"),
		EImageBank::getImage("ebonzai_mr7.png"),
		Coord(342,189),
		Coord(346,503)));

	_nDiffs = _vPartSet.size();
	//_nTry = 4;

	_nFontSize = 40;
   EGLL();
	_lpFont = EFontBank::getFont("acmesa.ttf",_nFontSize);
	EGLR();
	_lpFont->setColor(0,0,0,0.8f);

	_vSelections = std::vector< EMiniJeuPart *>(_vPartSet.size(),(EMiniJeuPart *) NULL);
	
   _pSelArbre.AddPoint(372,121);
   _pSelArbre.AddPoint(522,70);
   _pSelArbre.AddPoint(681,90);
   _pSelArbre.AddPoint(792,172);
   _pSelArbre.AddPoint(774,342);
   _pSelArbre.AddPoint(622,447);
   _pSelArbre.AddPoint(501,448);
   _pSelArbre.AddPoint(445,411);
   _pSelArbre.AddPoint(333,221);

   Reset();
}

EMinijeuBonzai::~EMinijeuBonzai(void)
{
	XDELETE(_lpImgSel);
	XDELETE(_lpFont);

	for (int i=0; i < (int)_vPartSet.size(); i++) {
		XDELETE(_vPartSet[i]);
	}
	for (int i=0; i<(int)_vSelections.size();i++){
		XDELETE(_vSelections[i]);
	}
	_vPartSet.clear();
}

/** 3 steps rendering **/
void EMinijeuBonzai::GameCheck ()
{
	EMiniJeuPart *tmp;

   if (!Mouse.LeftEvent() || !_pSelArbre.IsMouseOver())
		return;

	tmp = new EMiniJeuPart(_lpImgSel);
	tmp->Move(Mouse.x - tmp->Width()/2, Mouse.y - tmp->Height()/2);
	for (int i=0; i<(int)_vSelections.size();i++) {
		if (_vSelections[i] == NULL) {
			_vSelections[i] = tmp;
			break;
		}
	}
	
	tmp->FadOut(500);
	for (int i=0; i<(int)_vPartSet.size(); i++) {
		if (_vPartSet[i] == NULL || ! _vPartSet[i]->IsMouseOver())
			continue;
	
		if (!_vPartSet[i]->IsSecationed()) {
			_vPartSet[i]->ShowSolution(true,true);
			IsResolved();
			ESoundBank::getSound("mg_cut_fall")->playSample();
			return;
		}
	}

	ESoundBank::getSound("mg_cut")->playSample();
	SetGameFailed();
}

void EMinijeuBonzai::GameLogic ()
{
	for (int i=0; i<(int)_vSelections.size();i++) {
		if (_vSelections[i] != NULL && _vSelections[i]->FadOutComplete()) {
			XDELETE(_vSelections[i]);
		}
	}
	
	for (int i=0; i<(int)_vPartSet.size();i++) {
		_vPartSet[i]->Logic();
	}
}

void EMinijeuBonzai::GameDrawSet ()
{
	int nSec = 0;

	if (_nGameStatus <= MINIJEU_RESOLVED) {
		for (int i=0; i<(int)_vPartSet.size(); i++)
			if (_vPartSet[i]->IsSecationed())
				nSec++;
		_lpFont->drawStringFromRight(itos(nSec).c_str(),60,380);
		_lpFont->drawStringFromLeft("/7",60,380);
	}

	for (int i=0; i <(int)_vPartSet.size(); i++) {
		_vPartSet[i]->Draw();
	}

	for (int i=0; i <(int)_vSelections.size(); i++) {
		if (_vSelections[i] != NULL)
			_vSelections[i]->Draw();
	}
}

// Retourne true si le mini jeu est résolu
bool EMinijeuBonzai::GameIsResolve()
{
	Coord pos;
	for (int i=0; i<(int)_vPartSet.size(); i++) {
		if (!_vPartSet[i]->IsSecationed()) {
				return false;
		}
	}
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMinijeuBonzai::GameResolve() {
	Coord tmp;
	for (int i=0; i<_nDiffs; i++) {
		XDELETE(_vSelections[i]);
		_vSelections[i] = new EMiniJeuPart(_lpImgSel);
		tmp = _vPartSet[i]->Center();
		tmp.x = tmp.x - _lpImgSel->Width()/2;
		tmp.y = tmp.y - _lpImgSel->Height()/2;
		_vSelections[i]->Move(tmp);
		_vPartSet[i]->ShowSolution(true,false);
	}
	IsResolved();
	for (int i=0; i<_nDiffs; i++) {
		_vPartSet[i]->ShowSolution(false,false);
		_vPartSet[i]->ShowSolution(true,true);
	}
}

void EMinijeuBonzai::GameReset()
{
	for (int i=0; i<(int)_vSelections.size();i++){
		if (_vSelections[i] != NULL)
			_vSelections[i]->FadOut(500);
	}
	for (int i=0; i<_nDiffs;i++) {
		if (_vPartSet[i]->IsSecationed())
			_vPartSet[i]->ShowSolution(false,true);
	}
	//_nCurrentTry = _nTry;
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMinijeuBonzai::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  ShowHint(MINIJEU_HINT1);
		  _nDiffs -=1;
		  if(!_vPartSet[_nDiffs]->IsSecationed())
			  _vPartSet[_nDiffs]->ShowSolution(true,true);
		  Reset();
		  break;
	  case MINIJEU_HINT2:
		  ShowHint(MINIJEU_HINT2);
		  _nDiffs -=1;
		  if(!_vPartSet[_nDiffs]->IsSecationed())
			_vPartSet[_nDiffs]->ShowSolution(true,true);
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
EBonzaiPart::EBonzaiPart(KGraphic *imgUnresolved, KGraphic *imgResolved, Coord UnResolved, Coord Resolved): EMiniJeuPart(imgUnresolved,1)  {
	_lpImgUnresolved = imgUnresolved;
	_lpImgResolved = imgResolved;

	_cUnresolved = Coord(UnResolved.x - EMiniJeuBase::X(), UnResolved.y - EMiniJeuBase::Y());
	_cResolved = Coord(Resolved.x - EMiniJeuBase::X(), Resolved.y - EMiniJeuBase::Y());

	_bTransition = false;

	SwitchDrag();
	SwitchRotation();
	ShowSolution(false);
}

void EBonzaiPart::ShowSolution(bool status, bool bAnim) {
	if (bAnim) {
		_bTransition = true;
		return;
	}
	if (status) {
		_lpImg = _lpImgResolved;
		Move(_cResolved);
	} else {
		_lpImg = _lpImgUnresolved;
		Move(_cUnresolved);
	}
	_w = _lpImg->getWidth();
	_h = _lpImg->getHeight();
}

bool EBonzaiPart::IsSecationed() {
	if (_bTransition) {
		if (_lpImg == _lpImgResolved) {
			return false;
		}
		return true;
	}
	return _lpImg == _lpImgResolved;
};

void EBonzaiPart::Logic() {
	if (_bTransition && !_bAnimFad) {
		if (FadOutComplete() && _fBlend == 0) {
			_bTransition = false;
			if (_lpImg == _lpImgResolved) {
				ShowSolution(false);
			} else {
				ShowSolution(true);
			}
			FadIn(500);
		} else {
			FadOut(500);
		}
	}
	((EMiniJeuPart)this).Logic();
}

// Preload images et sons
void EMinijeuBonzai::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("ebonzai_bg","jpg",true,true);
   EImageBank::getImage("ebonzai_selection.png","",true,true);
   EImageBank::getImage("ebonzai_m1.png","",true,true);
   EImageBank::getImage("ebonzai_mr1.png","",true,true);
   EImageBank::getImage("ebonzai_m2.png","",true,true);
   EImageBank::getImage("ebonzai_mr2.png","",true,true);
   EImageBank::getImage("ebonzai_m3.png","",true,true);
   EImageBank::getImage("ebonzai_mr3.png","",true,true);
   EImageBank::getImage("ebonzai_m4.png","",true,true);
   EImageBank::getImage("ebonzai_mr4.png","",true,true);
   EImageBank::getImage("ebonzai_m5.png","",true,true);
   EImageBank::getImage("ebonzai_mr5.png","",true,true);
   EImageBank::getImage("ebonzai_m6.png","",true,true);
   EImageBank::getImage("ebonzai_mr6.png","",true,true);
   EImageBank::getImage("ebonzai_m7.png","",true,true);
   EImageBank::getImage("ebonzai_mr7.png","",true,true);

   ESoundBank::getSound("mg_cut_fall",false,true);
   ESoundBank::getSound("mg_cut",false,true);
}
