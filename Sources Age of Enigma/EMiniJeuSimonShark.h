#ifndef EMINIJEUSIMONSHARK_H
#define EMINIJEUSIMONSHARK_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "ESmartImage.h"
#include <list>
#include "KPTK.h"

class EMiniJeuSimonShark: public EMiniJeuBase
{
public:
	EMiniJeuSimonShark(void);
	virtual ~EMiniJeuSimonShark(void);

   static void Preload();

private:
	void GameCheck ();
	void GameLogic ();
	void GameDrawSet ();
	void GameReset();
	
	bool GameIsResolve();
	void GameResolve();

	void GameSetHint(int nHintLevel);

	void RandomSequence();
	void InitSimonSequence(float fStartDelay = 0);
	void LogicSimonSequence();

	// variable spécifique au gameplay courrant
	int _nCol;
	int _nLine;

	EFont *_lpFont;
	EFont *_lpFontPath;
	char _cTextStart[254];

	KGraphic *_lpImgBg;
	EMiniJeuPart *_lpImgStart;
	EMiniJeuPart *_lpPirate;
	
	KGraphic *_lpMaille;
	KGraphic *_lpFanion;
	KGraphic *_lpPiratPath;
	KGraphic *_lpRedCross;
	
	int _nDrawStep;

	// Gestion du display de la sequence shark
	KCounter *_lpSeqTimer;
	int _nStep, _nEndStep;
	int _nSequenceStep;
	bool _bPlaySequence;
	bool _bHold;
	float _fDelay, _fStepDelayDelta, _fStartDelay;

	std::vector< Coord > _vMaillesPos;
	std::vector< EMiniJeuPart * > _vMailles;
	std::vector< EMiniJeuPart * > _vPath;
};
#endif