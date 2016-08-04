#ifndef EMINIJEUTEMPLATE_H
#define EMINIJEUTEMPLATE_H

#include "EMiniJeuBase.h"

#include "ESmartImage.h"
#include "KPTK.h"

class FooToken;

class EMiniJeuTemplate :
	public EMiniJeuBase
{
public:
	EMiniJeuTemplate(void);
	virtual ~EMiniJeuTemplate(void);

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

	KGraphic *_lpGraphicToken;
	ESmartImage *_lpsimgToken;
	FooToken *_lpToken;
};

class FooToken
{
public:
	FooToken(float x, float y, int w, int h)
	{
		_x = x;
		_y = y;
		_w = w;
		_h = h;
		_bHold = false;
	}
	float _x;
	float _y;
	int _w;
	int _h;
	bool _bHold;
};

#endif