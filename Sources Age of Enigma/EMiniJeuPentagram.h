#ifndef EMINIJEUPENTAGRAM_H
#define EMINIJEUPENTAGRAM_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EMiniJeuPentagram:
	public EMiniJeuBase
{
public:
	EMiniJeuPentagram(void);
	virtual ~EMiniJeuPentagram(void);

   static void Preload();

private:

	/** 3 steps rendering **/
	void GameCheck ();
	void GameLogic ();
	void GameDrawSet();

	bool GameIsResolve();
	void GameResolve();
	void GameReset();
	void GameSetHint(int nHintLevel);
	
	Coord _posPentagram;
	Coord _posReflet;
	EMiniJeuPart *_lpImgFinal;
	EMiniJeuPart *_lpImgTriangle;

	KGraphic *_lpImgReflet;
	KGraphic *_lpImgPentagram;
	

	char _lpQueryText[K_MAXPATH];
	Coord _posQuery;
	Coord _posDigit;

	Digit *_digit1, *_digit2;

	int _nSol;
	EFont *_lpFont;
};
#endif