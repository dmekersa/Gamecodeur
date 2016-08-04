
#ifndef EMINIJEUCONNECT_H
#define EMINIJEUCONNECT_H

#include <list>
#include "global.h"
#include "KPTK.h"
#include "EMiniJeuHelper.h"
#include "eminijeubase.h"

class EConnectPart;
class WirePoint;

class EMiniJeuConnect: public EMiniJeuBase {
public:
	EMiniJeuConnect (void);
	virtual ~EMiniJeuConnect (void);

   static void Preload();

private:
	/** 3 steps rendering **/
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();

	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset();
	virtual void GameSetHint(int nHintLevel); 


	void DrawSpark();

	int _nGameSizeX;
	int _nGameSizeY;
	static const int _nGameSetSize=1;
	static const int _nWires = 2;

	KGraphic * _lpImgBgSuccess;
	KGraphic * _lpImgArenaSuccess;
	KGraphic * _lpImgArena;
	EConnectPart *_lpGrabedPiece;
	
	Coord _posArena;
	Coord _posArenaSuccess;

	std::vector< EConnectPart * > _lpPartSet;
	std::vector< std::vector< EConnectPart * > > _lpGame;
	Grid *_lpGridConnect;

	WirePoint* _wire[_nWires*2];

	KCounter *_lpSuccessTimeout;
	bool _bRedLink, _bBlueLink;
};

class EConnectPart: public EMiniJeuPart {
public:
	EConnectPart(char *color, const char* ImgPath, bool turn=true, int nPos=4);
	EConnectPart(const EConnectPart* a, bool rotate=false);

	bool Connect(int color, int to);
	bool GetNext(int color, int *to, Coord *c);
	void RotateCW(bool bAnim);
	void RotateACW(bool bAnim);
	void RotateToStep(int nStep);

	void SetSol(int n);
	void FixSol();
	bool Fixed() {return _bSolved;}

private:
	char _Color[5];
	Coord _Link[4];
	bool _bStraight;
	int _nSolAngle;
	bool _bSolved;
};

class WirePoint
{
public:
	WirePoint(int c, EConnectPart *i, float sAngle, float nX=0, float nY=0);
	virtual ~WirePoint(void);

	static void SetRefGrid(Grid *g);
	static void SetRefGame(std::vector< std::vector< EConnectPart * > > *g);
	void SetEndPoint(WirePoint *e) {_lpEnd=e; e->_lpEnd = this;}
	bool ResolvePath (void);
	
	bool Wired (void) const {
		return _bWired;
	}

	void DrawSpark();
	void StopSpark() {
//		_lpEmitter->stopEmitting = true;
//		_lpEnd->_lpEmitter->stopEmitting = true;
	}

private:
	float _nX, _nY;	
	
	int _cColor;
	
	EConnectPart *_lpImg;
	KGraphic *_lpSpark;
//	pPyroEmitter _lpEmitter;
	
	Coord _cStart;
	Coord _cEnd;
	float _sDir;
	float _eDir;
	
	bool _bWired;
	WirePoint *_lpEnd;
	
	static const std::vector< std::vector< EConnectPart * > > *_lpGame ;
	static const Grid *_lpGrid;
};

#endif
