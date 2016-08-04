#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "FontBank.h"
#include "EGeometry.h"
#include "math.h"

#include "EMiniJeuPentagram.h"

EMiniJeuPentagram::EMiniJeuPentagram(void)
{
	SetTextsTrad("PENTAGRAM");
	SetGameType(MINIJEU_SUBMITGAME);
	SetBackGround(EImageBank::getImage("mg_pentagram_bg.jpg"));
	SetGameNumber(23);

	int _queryDigitSpace = 25;
	int _digitMargin = 3;
	_nSol = 30;

	_posPentagram = Coord(388,137);
	_lpImgFinal = new EMiniJeuPart(EImageBank::getImage("mg_pentagram_final","png"));
	_lpImgFinal->Hide();
	_lpImgFinal->Move(_posPentagram);
	
	_lpImgTriangle = new EMiniJeuPart(EImageBank::getImage("mg_pentagram_2","png"));
	_lpImgTriangle->Hide();
	_lpImgTriangle->Move(_posPentagram);
	
	_lpImgReflet = EImageBank::getImage("mg_pentagram_final2","png");
	_posReflet = _posPentagram - Coord((_lpImgReflet->getWidth() - _lpImgFinal->Width())/2, (_lpImgReflet->getHeight() - _lpImgFinal->Height())/2);
	
	_lpImgPentagram = EImageBank::getImage("mg_pentagram_1","png");
	
	memset(_lpQueryText,0,K_MAXPATH*sizeof(char));
	strcpy(_lpQueryText,KStr("PENTAGRAM_QUERY"));

	_lpFont = EFontBank::getFont("tahomacounter40-0");
	_lpFont->setColor(1,1,1,1);
	_digit1 = new Digit(_lpFont,0.7f,32);
	_digit2 = new Digit(_lpFont,0.7f,32);
	XDELETE(_lpFont);

	_lpFont = EFontBank::getFont("tahoma.ttf",40);
	_lpFont->setColor(1,1,1,1);

	_posQuery.x = _lpFont->getStringWidth(_lpQueryText,TEXTSTYLE_LEFT) + _queryDigitSpace + 2*_digit1->width();
	_posQuery.x = (_nW - _posQuery.x) / 2;
	_posQuery.y = _posPentagram.y + _lpImgPentagram->getHeight() + 10 + _digit1->height() - _lpFont->getHeightPix();

	_posDigit.x = _posQuery.x + _lpFont->getStringWidth(_lpQueryText,TEXTSTYLE_LEFT) + _queryDigitSpace;
	_posDigit.y = _posQuery.y;

	_digit1->Move(_posDigit.x,_posDigit.y);
	_digit2->Move(_posDigit.x + _digit1->width() + _digitMargin ,_posDigit.y);

	Reset();
}

EMiniJeuPentagram::~EMiniJeuPentagram(void)
{
	XDELETE(_lpImgFinal);
	XDELETE(_lpImgTriangle);
	XDELETE(_digit1);
	XDELETE(_digit2);
   XDELETE(_lpFont);
}

/** 3 steps rendering **/
void EMiniJeuPentagram::GameCheck ()
{
	if (!(Mouse.LeftEvent() || Mouse.LeftReleaseEvent()))
		return;

	if (_nHintLevel == MINIJEU_HINT1) {
		if (Mouse.LeftEvent() && Mouse >= _lpImgTriangle->Pos() && Mouse <= _lpImgTriangle->Pos()+_lpImgTriangle->Size()) {
			_lpImgTriangle->FadIn(500,true);		
			Mouse.EventClose();
		} else if (Mouse.LeftReleaseEvent()) {
			_lpImgTriangle->FadOut(1500,true);
			Mouse.EventClose();
		}

	}

	_digit1->Check();
	_digit2->Check();

}

void EMiniJeuPentagram::GameLogic()
{
}

void EMiniJeuPentagram::GameDrawSet()
{
	float cBlend = _lpImgFinal->GetAlpha();
	
	if (cBlend != 1)
		_lpImgPentagram->blitAlphaRectFx(0,0,_lpImgPentagram->getWidth(),_lpImgPentagram->getHeight(),_posPentagram.x,_posPentagram.y,0,1,1-cBlend);
	
	if (cBlend == 0) {
		_lpFont->drawStringFromLeft(_lpQueryText,_posQuery.x,_posQuery.y);
		_digit1->Draw();
		_digit2->Draw();

		_lpImgTriangle->Draw();
	}
	
	_lpImgFinal->Draw();
	_lpImgReflet->blitAlphaRectFx(0,0,_lpImgPentagram->getWidth(),_lpImgPentagram->getHeight(),_posReflet.x,_posReflet.y,0,1,cBlend);


	if (cBlend == 0) {
	}
}

void EMiniJeuPentagram::GameReset() {
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuPentagram::GameIsResolve()
{
	if ((_digit1->GetVal() * 10 + _digit2->GetVal()) == _nSol) {
		_lpImgFinal->FadIn(1500);
		return true;
	}
	return false;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuPentagram::GameResolve()
{
	_digit1->SetVal((int)(_nSol / 10));
	_digit2->SetVal(_nSol % 10);
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuPentagram::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
		case MINIJEU_HINT1:
			ShowHint(MINIJEU_HINT1);
			_lpImgTriangle->Show();
			_lpImgTriangle->FadOut(3000,true);
			Reset(); 
			break;
		case MINIJEU_HINT2:
			ShowHint(MINIJEU_HINT2);
			_lpImgTriangle->FadOut(300,true);
			_lpImgPentagram = EImageBank::getImage("mg_pentagram_3","png");
			_nSol = 10;
			Reset();
			break;
		case MINIJEU_RESOLVED:
			Resolve();
			_lpImgTriangle->FadOut(300,true);
			break;
		default:
			KAssert(false);
			break;
	}
}

// Preload images et sons
void EMiniJeuPentagram::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_pentagram_bg.jpg","",true,true);
   EImageBank::getImage("mg_pentagram_final","png",true,true);
   EImageBank::getImage("mg_pentagram_2","png",true,true);
   EImageBank::getImage("mg_pentagram_final2","png",true,true);
   EImageBank::getImage("mg_pentagram_1","png",true,true);
   EImageBank::getImage("mg_pentagram_3","png",true,true);

}
