#ifndef EMINIJEUSYMBOLMATCH_H
#define EMINIJEUSYMBOLMATCH_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EMiniJeuSymbolMatch:
	public EMiniJeuBase
{
public:
	EMiniJeuSymbolMatch(void);
	virtual ~EMiniJeuSymbolMatch(void);

   static void Preload();

private:
	struct Asso {
		EMiniJeuPart *card;
		EMiniJeuPart *symbol;
	
		Asso(void) {card = NULL; symbol = NULL;}
		~Asso(void) {card = NULL; symbol = NULL;}
	};
	
	/** 3 steps rendering **/
	void GameCheck ();
	void GameLogic ();
	void GameDrawSet();
	void GameDrawGrab();

	bool GameIsResolve();
	void GameResolve();
	void GameReset();
	void GameSetHint(int nHintLevel);
	
	void RandomSet();
	void RandomCard();

	EMiniJeuPart *GetCard(EMiniJeuPart *symbol);
	int GetResponseIndex(EMiniJeuPart *symbol);

	int _nGameSize;
	
	KGraphic *_lpImgHands;

	std::vector< EMiniJeuPart *> _vCard;
	std::vector< EMiniJeuPart *> _vResponse;
	std::vector< EMiniJeuPart *> _vSet;
	
	std::vector< Asso > _vSolution;

	Grid *_lpGridCard;
	Grid *_lpGridResponse;
	Grid *_lpGridSet;

	EMiniJeuPart *_lpGrabedPart;
	EMiniJeuPart *_lpHintPart1;
};
#endif