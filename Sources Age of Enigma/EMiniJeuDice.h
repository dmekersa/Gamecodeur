#ifndef EMINIJEUDICE_H
#define EMINIJEUDICE_H

#include <list>
#include "global.h"
#include "KPTK.h"
#include "EMiniJeuHelper.h"
#include "eminijeubase.h"

class EDicePart;

class EMiniJeuDice: public EMiniJeuBase {
public:
	EMiniJeuDice (void);
	virtual ~EMiniJeuDice (void);

	/** 3 steps rendering **/

	/* Pilotage du mini jeu */
	// Choix de la variation (modèle ou sujet)
	virtual void SetVariation (int _nVariation);

   static void Preload();

private:
	void GameCheck ();
	void GameLogic ();
	
	void GameDrawSet ();
	void GameDrawGrab();

	bool GameIsResolve();
	void GameResolve();
	void GameReset();

	void GameSetHint(int nHintLevel);

	EDicePart* GetRandomPart();
	void DrawDices();
	void DrawFirstDice();
	void DrawSecondDice();


	bool _bLeftMouseWasDown, _bRightMouseWasDown;
	int __nVariation;

	EDicePart *_lpGrabedPiece;
	//EFont * _lpFont;
	
	std::vector< Coord > _lpDicePatron;
	std::vector< EDicePart * > _lpDiceSolution;
	std::vector< KGraphic * > _lpDiceSolutionSymbol;
	std::list< EDicePart * > _lpRandomPartPool;

	std::vector< EDicePart * > _lpPartSet;
	std::vector< std::vector< EDicePart * > > _lpGame;

	Grid *_lpGridPartSet;
	Grid *_lpGridDice;

	KGraphic *target;
};

class EDicePart: public EMiniJeuPart {
public:
	EDicePart(const char* ImgPath, int nPos=4);
	EDicePart(const EDicePart* a, bool rotate=false);
};

#endif