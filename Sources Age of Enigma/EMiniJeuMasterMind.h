#ifndef EMINIJEUMASTERMIND_H
#define EMINIJEUMASTERMIND_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "ESmartImage.h"
#include "KPTK.h"

class EMasterMindPart;

class EMiniJeuMasterMind: public EMiniJeuBase
{
public:
	EMiniJeuMasterMind(void);
	virtual ~EMiniJeuMasterMind(void);

   static void Preload();

private:
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();
	virtual void GameDrawGrab();

	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset();
	virtual void GameSetHint(int nHintLevel);

	virtual void DrawHint();

	float _fImgSize;

	EFont *_lpFontCB;
	EFont *_lpFont;
	bool _bColorBlindMode;
	char _cCBMessage[256];
	char _cNCBMessage[256];
	
	// Indication de gameplay
	char _cHintText1[256];
	char _cHintText2[256];

	int _nCol, _nRow;
	int _nCurrentRow;
	int _nColor;
	int _nSetCol;

	gridtype _gridHintType;

	KGraphic *_lpImgBgFront;
	KGraphic *_lpImgHintQuerry;
	KGraphic *_lpImgIntero;
	KGraphic *_lpImgHintbg;
	KGraphic *_lpImgHint1;
	KGraphic *_lpImgHint2;
	KGraphic *_lpImgHintTextBg;

	std::vector<EMasterMindPart *> _vPart;
	std::vector < EMasterMindPart * > _vSolution;
	std::vector < std::vector < EMasterMindPart * > > _vGame;
	std::vector < std::vector < int > > _vHint;

	Grid *_lpGridGame;
	Grid *_lpGridHint;
	Grid *_lpGridSolution;
	Grid *_lpGridSet;

	int _nXHintTextBox, _nYHintTextBox, _nWHintTextBox;
	int _nDeltaXHintBG, _nDeltaYHintBG;
	double _fHintScale;

	ESmartImage *_lpSOndes;
	float _fOndesDelay;

	EMasterMindPart *_GrabedPart;
};

class EMasterMindPart: public EMiniJeuPart {
public:
	EMasterMindPart (KGraphic *lpGraphic,KGraphic *lpGraphicColorBlind = NULL);
	EMasterMindPart(const EMasterMindPart* a);

	void ColorBlind();
	virtual ~EMasterMindPart(void) {;}

	void Hide(int timeout=200);
	void UnHide(int timeout=200);
private:
	bool _bHide;
	KGraphic *_lpImgColorBlind;
};
#endif