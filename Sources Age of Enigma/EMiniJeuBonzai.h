#ifndef EMINIJEUTREEDIFF_H
#define EMINIJEUTREEDIFF_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"


class EBonzaiPart;

class EMinijeuBonzai :
	public EMiniJeuBase
{
public:
	EMinijeuBonzai(void);
	virtual ~EMinijeuBonzai(void);

   static void Preload();

private:
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();

	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset();
	virtual void GameSetHint(int nHintLevel);
	
	int _nDiffs;
	/*int _nTry;
	int _nCurrentTry;*/

	EFont * _lpFont;
	int _nFontSize;

	EMiniJeuPart *_lpImgSel;

   Polygone _pSelArbre;

	std::vector< EBonzaiPart * > _vPartSet;
	std::vector< EMiniJeuPart *> _vSelections;
};

class EBonzaiPart: public EMiniJeuPart {
public:
	EBonzaiPart(KGraphic *imgUnresolved, KGraphic *imgResolved, Coord UnResolved, Coord Resolved);
	virtual ~EBonzaiPart(void){;}

	virtual void Logic();

	void ShowSolution(bool status=true, bool bAnim=false);
	bool IsSecationed();

private:
	KGraphic *_lpImgUnresolved;
	KGraphic *_lpImgResolved;

	bool _bTransition;
	Coord _cUnresolved;
	Coord _cResolved;
};
#endif