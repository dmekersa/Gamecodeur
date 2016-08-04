#ifndef EMINIJEUFLACONS_H
#define EMINIJEUFLACONS_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EFlaconsPart;

class EMiniJeuFlacons:
	public EMiniJeuBase
{
public:
	EMiniJeuFlacons(void);
	virtual ~EMiniJeuFlacons(void);

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
	
	void SetElemPos(EMiniJeuPart *elem,int x,int y);
	void GenerateGame();

	int _nGameSize;
	
	KGraphic *_lpImgArrow;

	std::vector< EMiniJeuPart * > _vSlots;
	std::vector< EFlaconsPart * > _vFlacons;

	Grid *_lpGridGame;

	EFlaconsPart *_lpSelected;
};

class EFlaconsPart: public EMiniJeuPart {
public:
	EFlaconsPart(KGraphic *img, KGraphic *shadow);
	~EFlaconsPart(void);

	void Check();
	void Draw();

	void Reset();

	bool IsMoving();
	void EndMove();

	EMiniGame::Direction GetDir();
	
	void RegisterPosForReset();

private:
	void DrawSelDir();
	EMiniGame::Direction GetCurSelDir();

	KGraphic *_lpImgArrow;
	bool _bSelected; 
	EMiniGame::Direction _dMoveTo;

	Coord _shadowOffset, _shadowSize;
	KGraphic *_lpImgShadow;

	float _fResetX, _fResetY;
};
#endif