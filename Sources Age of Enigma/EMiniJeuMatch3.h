#ifndef EMINIJEUMATCH3_H
#define EMINIJEUMATCH3_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"
#include "KGraphic.h"
#include "ESmartImage.h"

class EMatch3Part;

class EMiniJeuMatch3:
	public EMiniJeuBase
{
public:
	EMiniJeuMatch3(void);
	virtual ~EMiniJeuMatch3(void);

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

	int _nNumCol, _nNumLine;
	int _nWinFloor;

	std::vector< KGraphic * > _vSymbs;
	Grid *_lpGridGame;

	ESmartImage *_lpClouds;
	KGraphic *_lpMatch3bg;
	ESmartImage *_lpTorch1;
	ESmartImage *_lpTorch2;

   float _fFrameTime;
};

class EMatch3Part: public EMiniJeuPart {
public:
	EMatch3Part(KGraphic *imgPart);
	~EMatch3Part(void);

	static void GameInit(Grid *grid, std::vector< KGraphic *> vSymbols);
	static void GameClear();

	static void GameCheck();
	static void GameDrawSet();
	static void GameLogic();
	
   static void RotateGridCW();
	static void RotateGridACW();
	static void CheckMatch3();
   
   static void SwapGridPart(Coord c1, Coord c2);

	void Check();
	void Draw();

	void Select(bool bSelect=true);
	bool IsSelected();

	//static void SetElapsedTime();
	static Grid *_lpGrid;
	static std::vector< KGraphic * > _vSymbols;
	static std::vector< std::vector < EMatch3Part * > > _lpGame;


private:
	friend class EMiniJeuMatch3;

	static int _numpart; // Debug
	static float _fElapsedTime;
	static KGraphic *_lpGraphic;
	static EMatch3Part *_lpSelected;
	static EMatch3Part *_lpSwaped;
	
	static Coord _vDir[4];

	static std::list< EMatch3Part * > _lpMatchList;
	static std::list< KCounter * > _lpMatchCounterList;
	static std::list< KCounter * > _lpPreFallCounterList;
	static std::list< KCounter * > _lpFallCounterList;

	static float _fPrePopDuration, _fPopDuration;
	static float _fPreFallDuration, _fFallDuration;
	static float _fBumpDuration;

	enum FallState{
		STABLE,
		PREFALL,
		FALL,
		PREPOP,
      POP,
		DISABLED
	};

	// Part Function
	void SetGridPos(int col, int line);
	void SetGridPos(Coord pos) {SetGridPos(pos.x,pos.y);}
	
	Coord GetPosOffset();
	EMatch3Part* GetPart(Coord cell);

	bool ShouldFall();
	bool CouldMove(Coord toCell);
	void UpdateSelectedPos();

	void PreFall();
	void Fall();
	void ForcePop();
	void Bump();
   EMatch3Part::FallState GetState();

	bool Match(KGraphic *symb=NULL, Coord dir=Coord(0,0), bool test=false);
	bool HasMatch() {return Match(NULL,Coord(0,0),true);}

	Coord _posGrid;

	// POP gestion
	KCounter *_lpCounterPrePop;
	float _fCurrentPopDuration;

	// Fall gestion
	float _randomPreFallOffset;
	KCounter *_lpCounterPreFall, *_lpCounterFall;
	Coord _preFallOffset, _fallOffset;
	Coord _preFallDir, _fallDir;
	bool _bPop;

	// Bump gestion
	Coord _bumpOffset;
	KCounter *_lpCounterBump;

	// Selection gestion
	KCounter *_lpCounterScale;
	float _fScaleDuration;
	float _fMaxScale;
	Coord _fromPos;
	//float _fSelectedDeltaX, _fSelectedX;
	
	Coord _fSelectedPos;
	float _fSelectedPosDelta;
};
#endif