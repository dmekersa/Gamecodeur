#ifndef EMINIJEUGARDENING_H
#define EMINIJEUGARDENING_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EMiniJeuGardening :
	public EMiniJeuBase
{
public:
	EMiniJeuGardening(void);
	virtual ~EMiniJeuGardening(void);

   static void Preload();

private:
	/** 3 steps rendering **/
	void GameCheck ();
	void GameLogic ();
	void GameDrawSet ();
	void GameDrawGrab();

	bool GameIsResolve();
	void GameResolve();
	void GameReset();
	void GameSetHint(int nHintLevel);

	KGraphic *_lpImgWrong;
	EMiniJeuPart *_lpGrabedPart;

	static const int _nGameSize = 5;

	std::vector< EMiniJeuPart * > _vPartSet;
	std::vector< std::vector< int > > _vBaseGame;
	std::vector< std::vector< int > > _vGame;
	std::vector< std::vector< int > > _vSolution;
	std::vector< int > _vNumPart;

	Grid *_lpGridPartSet;
	Grid *_lpGridGame;

	//void GameInit();

	void ResetPartSet();
	int IndexPart(EMiniJeuPart *part);

	bool CheckCellValue(int col, int line, int value);
	bool CheckCellValue(int col, int line);

	//-------------------------------
	bool _bBackTrack;
	int _nMinClue, _nMaxClue;

	std::vector< std::vector< bool > > _vLineLock;
	std::vector< std::vector< bool > > _vColLock;

	void LockCell(int col, int line, int value, bool lock=true);
	void UnLockCell(int col, int line, int value) {LockCell(col,line,value,false);}
	bool CheckLockCell(int col, int line, int value);

	void Solve();

	void GenerateGrid();
	int TestGrid(int nSolMax);
};
#endif