#ifndef EMINIJEUCUBES_H
#define EMINIJEUCUBES_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class ECubePart;

class EMiniJeuCubes :
	public EMiniJeuBase
{
public:
	EMiniJeuCubes(void);
	virtual ~EMiniJeuCubes(void);

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

	void GenerateSol();

	static const int _nCubesSize = 4;
	int _nGameSize;
	
	EFont *_lpFont;
	char _lpQueryText[256];

	std::vector< KGraphic * > _vCubesPart;
	KGraphic *_lpImgCube1;
	KGraphic *_lpImgCube2;

	Coord _cOrigine;
	Coord _cFirstDice;
	Coord _minPosCube, _maxPosCube;
	Coord _cVector[3];

	int _nNumCube;
	int _nResponse;

	std::vector< std::vector< std::vector< int > > > _vGame;
	
	Digit *_Digit1;
	Digit *_Digit2;

	Coord _pBlendBox1, _pBlendBox2;
};

class ECubePart: public EMiniJeuPart {
public:
	ECubePart(KGraphic *img): EMiniJeuPart(img,1,0) {;}
	ECubePart(const ECubePart* a): EMiniJeuPart(a) {;}
	~ECubePart(void) {;}
};
#endif
