#include "assert.h"

#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "FontBank.h"

#include "EMiniJeuPlantAddition.h"

using EMiniGame::GetRandom;

EMiniJeuPlantAddition::EMiniJeuPlantAddition(void)
{
	SetTextsTrad("PLANTADDITION");
	SetGameType(MINIJEU_SUBMITGAME);
	SetGameNumber(21);
	SetBackGround(EImageBank::getImage("mg_plant_addition_bg","jpg"));

	_nXHintTextBox = 6;
	_nYHintTextBox = 465;
	_nWHintTextBox = 192;

	_nGameSize = 3;
	_nNumPart = 4;
	_nStartCheck = 0;

	_lpFontSet = EFontBank::getFont("NITECLUB.TTF",46);
	_lpFont = EFontBank::getFont("NITECLUB.TTF",45);
	_lpFontHint = EFontBank::getFont("acmesa.ttf",30);
	_lpFontSet->setColor(1,1,1,1);
	_lpFont->setColor(0,0,0,1);
	_lpFontHint->setColor(0,0,0,1);

	_lpImgQuerry = EImageBank::getImage("mg_plant_addition_case_distrib","png");
	_lpImgPlus = EImageBank::getImage("mg_plant_addition_plus","png");
	_lpImgMinus = EImageBank::getImage("mg_plant_addition_minus","png");

	_vPartSet.push_back(EImageBank::getImage("mg_plant_addition_aneth","png"));
	_vPartSet.push_back(EImageBank::getImage("mg_plant_addition_cerfeuil","png"));
	_vPartSet.push_back(EImageBank::getImage("mg_plant_addition_fenouil","png"));
	_vPartSet.push_back(EImageBank::getImage("mg_plant_addition_garlic","png"));
	_vPartSet.push_back(EImageBank::getImage("mg_plant_addition_ginkgo","png"));
	_vPartSet.push_back(EImageBank::getImage("mg_plant_addition_millepertuis","png"));
	_vPartSet.push_back(EImageBank::getImage("mg_plant_addition_pavot","png"));
	_vPartSet.push_back(EImageBank::getImage("mg_plant_addition_rue","png"));

	int r;
	for (int i=0; i<_nNumPart; i++) {
		r = GetRandom(_vPartSet.size());
		_vSelPart.push_back( _vPartSet[r]);
		_vPartSet[r] = _vPartSet[_vPartSet.size() -1];
		_vPartSet.pop_back();
m0:		r = GetRandom(8)+1;
		for (int j=0; j<_vPartValue.size(); j++) {
			if (_vPartValue[j] == r)
				goto m0;
		}
		_vPartValue.push_back(r);
	}
	_vResponse = std::vector< int >(_nNumPart,1);

	_lpImgHint1 = _vSelPart[0];
	_lpImgHint2 = _vSelPart[1];

	memset(_cHintText1,0,sizeof(_cHintText1));
	memset(_cHintText2,0,sizeof(_cHintText2));
	strcat(_cHintText1," = ");
	strcat(_cHintText2," = ");
	strcat(_cHintText1,itos(_vPartValue[0]).c_str());
	strcat(_cHintText2,itos(_vPartValue[1]).c_str());
	_fHintScale = 60 / _vPartSet[0]->getWidth();

	_vSet = std::vector< std::vector< int > >(_nGameSize, std::vector< int >(_nGameSize,0));
	_vSet[0][0] = 0;
	_vSet[0][1] = 1;
	_vSet[0][2] = 1;
	_vSet[1][0] = 2;
	_vSet[1][1] = 0;
	_vSet[1][2] = 3;
	_vSet[2][0] = 3;
	_vSet[2][1] = 2;
	_vSet[2][2] = 2;

	_vColSum = std::vector< int >(_nGameSize, 0);
	_vLineSum = std::vector< int >(_nGameSize, 0);
	for (int i=0; i<_nGameSize; i++) {
		for (int j=0; j<_nGameSize; j++) {
			_vColSum[i]+= _vPartValue[_vSet[j][i]];
			_vLineSum[i]+= _vPartValue[_vSet[i][j]];
		}
	}

	_lpGridGame = new Grid(_nGameSize,_nGameSize,_vSelPart[0]->getWidth(),_vSelPart[0]->getHeight(),0,11);
	_lpGridGame->Move((_nW - _lpGridGame->width())/2 +5, 104);

	_lpGridQuerry = new Grid(_nNumPart,1,_lpImgQuerry->getWidth(),_lpImgQuerry->getHeight(),0,11);
	_lpGridQuerry->Move((_nW - _lpGridQuerry->width())/2,_lpGridGame->_yPos + _lpGridGame->height() + 55);

	_cPlusPos = Coord(100,28);
	_cMinusPos = Coord(100,114);

   GameReset();
}

EMiniJeuPlantAddition::~EMiniJeuPlantAddition(void)
{
   XDELETE(_lpGridQuerry);
	XDELETE(_lpGridGame);
   XDELETE(_lpFontSet);
   XDELETE(_lpFont);
   XDELETE(_lpFontHint);
}

/** 3 steps rendering **/
void EMiniJeuPlantAddition::GameCheck ()
{
	Coord pos,relPos;
	int tmp;

	if (!Mouse.LeftEvent())
		return;

	if (_lpGridQuerry->IsMouseOver()) {
		pos = _lpGridQuerry->GetMouseSelection();
		tmp = pos.x;
		if (tmp <  _nStartCheck)
			return;

		pos = _lpGridQuerry->GetCellPos(tmp,0);
		relPos = Coord(Mouse.x -pos.x, Mouse.y -pos.y);
		if (relPos.x >= _cPlusPos.x &&  relPos.x <= (_cPlusPos.x + 32) &&
			relPos.y >= _cPlusPos.y &&  relPos.y <= (_cPlusPos.y + 32)) {
			_vResponse[tmp] = (_vResponse[tmp]+1) %9;
			Mouse.LeftEventClose();
			ESoundBank::getSound("mg_digit")->playSample();
			return;
		}
		if (relPos.x >= _cMinusPos.x &&  relPos.x <= (_cMinusPos.x + 32) &&
			relPos.y >= _cMinusPos.y &&  relPos.y <= (_cMinusPos.y + 32)) {
			_vResponse[tmp] = (_vResponse[tmp]-1+9) %9;
			Mouse.LeftEventClose();
			ESoundBank::getSound("mg_digit")->playSample();
			return;
		}
	}
}

void EMiniJeuPlantAddition::GameLogic ()
{
}

void EMiniJeuPlantAddition::GameDrawSet()
{
	Coord pos,gridBondary;
	KGraphic *tmp;
	
	for (int i=0; i<_vSelPart.size(); i++) {
		pos = _lpGridQuerry->GetCellPos(i,0);
		_lpImgQuerry->blitAlphaRect(0,0,_lpImgQuerry->getWidth(),_lpImgQuerry->getHeight(),pos.x,pos.y);
		_vSelPart[i]->blitAlphaRect(0,0,_lpImgQuerry->getWidth(),_lpImgQuerry->getHeight(),pos.x+16,pos.y+48);
		_lpFont->drawStringFromLeft(itos(_vResponse[i]+1).c_str(),pos.x+110,pos.y+66);
		if (i >= _nStartCheck) {
			_lpImgMinus->blitAlphaRect(0,0,_lpImgMinus->getWidth(),_lpImgMinus->getHeight(),pos.x + _cMinusPos.x,pos.y + _cMinusPos.y);
			_lpImgPlus->blitAlphaRect(0,0,_lpImgPlus->getWidth(),_lpImgPlus->getHeight(),pos.x + _cPlusPos.x,pos.y + _cPlusPos.y);
		}
	}

	_lpGridGame->GetZone(_nGameSize -1, _nGameSize -1,&pos,&gridBondary);
	for (int i=0; i<(int)_vSet.size(); i++) {
		for (int j=0; j<(int)_vSet[1].size(); j++) {
			tmp = _vSelPart[_vSet[i][j]];
			pos = _lpGridGame->GetCellPos(j,i);
			tmp->blitAlphaRect(0,0,tmp->getWidth(),tmp->getHeight(),pos.x,pos.y);
		}
		
		pos = _lpGridGame->GetCellPos(_nGameSize -1,i);
		_lpFontSet->drawStringFromLeft (itos(_vLineSum[i]).c_str(),gridBondary.x +8,pos.y);
		pos = _lpGridGame->GetCellPos(i,_nGameSize -1);
		_lpFontSet->drawStringCentered(itos(_vColSum[i]).c_str(),pos.x,pos.x+_vSelPart[0]->getWidth(),gridBondary.y +5);

	}
}

void EMiniJeuPlantAddition::GameReset() {
	for (int i =0; i<_vResponse.size(); i++) {
		if (i>=_nStartCheck) {
			_vResponse[i] = 0;
		} else {
			_vResponse[i] = _vPartValue[i]-1;
		}
	}
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuPlantAddition::GameIsResolve()
{
	for (int i =_nStartCheck; i<_vResponse.size(); i++)
		if (_vResponse[i]+1 != _vPartValue[i])
			return false;

	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuPlantAddition::GameResolve()
{
	for (int i =0; i<_vResponse.size(); i++)
		_vResponse[i] = _vPartValue[i]-1;
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuPlantAddition::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  //SetDisplayHintBox(true);
		  _nStartCheck++;
		  ShowHint(MINIJEU_HINT1);
		  Reset(); 
		  break;
	  case MINIJEU_HINT2:
		  _nStartCheck++;
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

void EMiniJeuPlantAddition::DrawHint() {
	float hintImgSize = _lpImgHint1->getWidth() * _fHintScale;
	float y = (float)_nYHintTextBox;
	float x = _nXHintTextBox + hintImgSize;
	

	if (!_nHintLevel > MINIJEU_NORMAL)
		return;

//	EMiniJeuBase::DrawHint();

	_lpImgHint1->blitAlphaRectFx(0,0,hintImgSize,hintImgSize,(short)_nXHintTextBox,(short)y,0,_fHintScale,1);
	y = y + (hintImgSize - (30 / _fHintScale))/2;
	_lpFontHint->drawStringFromLeft(_cHintText1,x,y);
	
	if (_nHintLevel == MINIJEU_HINT1)
		return;
	y = (float)_nYHintTextBox + hintImgSize +10;
	_lpImgHint2->blitAlphaRectFx(0,0,hintImgSize,hintImgSize,(short)_nXHintTextBox,(short)y,0,_fHintScale,1);
	y = y + (hintImgSize - (30 / _fHintScale))/2;
	_lpFontHint->drawStringFromLeft(_cHintText2,x,y);
}


// Preload images et sons
void EMiniJeuPlantAddition::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_plant_addition_bg","jpg",true,true);
   EImageBank::getImage("mg_plant_addition_case_distrib","png",true,true);
   EImageBank::getImage("mg_plant_addition_plus","png",true,true);
   EImageBank::getImage("mg_plant_addition_minus","png",true,true);
   EImageBank::getImage("mg_plant_addition_aneth","png",true,true);
   EImageBank::getImage("mg_plant_addition_cerfeuil","png",true,true);
   EImageBank::getImage("mg_plant_addition_fenouil","png",true,true);
   EImageBank::getImage("mg_plant_addition_garlic","png",true,true);
   EImageBank::getImage("mg_plant_addition_ginkgo","png",true,true);
   EImageBank::getImage("mg_plant_addition_millepertuis","png",true,true);
   EImageBank::getImage("mg_plant_addition_pavot","png",true,true);
   EImageBank::getImage("mg_plant_addition_rue","png",true,true);

   ESoundBank::getSound("mg_digit",false,true);
}
