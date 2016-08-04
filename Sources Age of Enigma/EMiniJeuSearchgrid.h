#ifndef EMINIJEUSEARCHGRID_H
#define EMINIJEUSEARCHGRID_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"

#include "KPTK.h"

class FooToken;

class EMiniJeuSearchgrid: public EMiniJeuBase
{
public:
	EMiniJeuSearchgrid(void);
	virtual ~EMiniJeuSearchgrid(void);

   static void Preload();

private:
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();

	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset();
	virtual void GameSetHint(int nHintLevel); 

	Coord GetRandomPos();

	int _nGameSize;
	int _nDefaultTry;

	KGraphic *_lpImgDigged1;
	KGraphic *_lpImgDigged2;
	KGraphic *_lpImgPearl;
   KGraphic *_lpImgPearlWrong;
	KGraphic *_lpImgBg;
	KGraphic *_lpImgCartouche;
	EFont *_lpFont;
	EFont *_lpFontHintDistance;
	
	int _nFontSize;
    int _nFontHintDistanceSize;

	std::vector < std::vector < int > > _vCurrentGame;
	Grid *_lpGrid;

	int _nTry;

	std::string _sTry;
	std::string _sDistance;

	Coord PearlPos;
	KCounter *_lpCounter;
   float _fTimeFromFail;
};
#endif