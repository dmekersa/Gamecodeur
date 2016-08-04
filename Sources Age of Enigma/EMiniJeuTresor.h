#ifndef EMINIJEUTRESOR_H
#define EMINIJEUTRESOR_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"

#include "KPTK.h"

class FooToken;

class EMiniJeuTresor: public EMiniJeuBase
{
public:
	EMiniJeuTresor(void);
	virtual ~EMiniJeuTresor(void);

   static void Preload();

private:
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();

	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset();
	virtual void GameSetHint(int nHintLevel); 

	int _nGameSize;

	EFont *_lpFont;
	EFont *_lpFontEnigme;

	int _nFontSize;
	Coord _cDim;

	std::vector < int > _vCurrentGame;
	std::vector < int > _vSolution;
	Grid *_lpGrid;

	char _cEnigme[1024];
};

#endif
