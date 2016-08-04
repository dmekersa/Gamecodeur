#ifndef EMINIJEUPLANTADDITION_H
#define EMINIJEUPLANTADDITION_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EMiniJeuPlantAddition :
	public EMiniJeuBase
{
public:
	EMiniJeuPlantAddition(void);
	virtual ~EMiniJeuPlantAddition(void);

   static void Preload();

private:
	/** 3 steps rendering **/
	void GameCheck ();
	void GameLogic ();
	void GameDrawSet ();

	bool GameIsResolve();
	void GameResolve();
	void GameReset();
	void GameSetHint(int nHintLevel);
	virtual void DrawHint();

	int _nGameSize;
	int _nNumPart;

	EFont *_lpFontSet;
	EFont *_lpFont;
	EFont *_lpFontHint;

	KGraphic *_lpImgQuerry;
	KGraphic *_lpImgPlus, *_lpImgMinus;

	Coord _cPlusPos, _cMinusPos;

	KGraphic *_lpImgHint1;
	KGraphic *_lpImgHint2;

	char _cHintText1[5];
	char _cHintText2[5];
	float _fHintScale;
	
	std::vector< KGraphic * > _vPartSet;
	std::vector< KGraphic * > _vSelPart;
	std::vector< int > _vPartValue;
	std::vector< std::vector< int > > _vSet;

	std::vector< int > _vColSum;
	std::vector< int > _vLineSum;
	
	std::vector< int > _vResponse;
	std::vector< int > _v;

	Grid *_lpGridGame;
	Grid *_lpGridQuerry;

	int _nXHintTextBox, _nYHintTextBox, _nWHintTextBox;

	int _nStartCheck;
};
#endif
