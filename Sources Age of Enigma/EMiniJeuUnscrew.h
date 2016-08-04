#ifndef EMINIJEUUNSCREW_H
#define EMINIJEUUNSCREW_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include <list>

#include "KPTK.h"

class EUnScrewPart;

class EMiniJeuUnscrew: public EMiniJeuBase
{
public:
	EMiniJeuUnscrew(void);
	virtual ~EMiniJeuUnscrew(void);

   static void Preload();

private:
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();

	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset();
	virtual void GameSetHint(int nHintLevel);

	KGraphic *_lpImgBg;
	KGraphic *_lpImgSideScrew;
	Coord _cPosBg;

	std::vector < Coord > _vPosSideScrew;
	std::vector < EUnScrewPart * > _vGame;
	Grid *_lpGridHint;
};



class EUnScrewPart: public EMiniJeuPart {
public:
	EUnScrewPart();
	~EUnScrewPart();

	void Screw(int step, bool direct=true);
	void SetSuccessor(EUnScrewPart *, int step);
	void ResetSuccessor();

	void DrawCounter();
	void DrawHelpers();

	bool CouldRotate(int step);
	
private:
	struct {
		EUnScrewPart *next;
		int step;
	} typedef ScrewNext;
	void DrawHelper(int level);

	EFont *_lpFont;
	EFont *_lpFontHint;

	bool ScrewSuccessor(int step);

	std::list< ScrewNext > _lSuccessor;
};
#endif
