#ifndef EMINIJEUMIRORBEAM_H
#define EMINIJEUMIRORBEAM_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"
#include <list>

class EMirorPart;
class Path;

class EMiniJeuMirorBeam :
	public EMiniJeuBase
{
public:
	EMiniJeuMirorBeam(void);
	virtual ~EMiniJeuMirorBeam(void);

   static void Preload();

private:
	friend class Path;

	/** 3 steps rendering **/
	void GameCheck ();
	void GameLogic ();
	void GameDrawSet ();
	void GameDrawGrab ();

	bool GameIsResolve();
	void GameResolve();
	void GameReset();
	void GameSetHint(int nHintLevel);

	static const int _nGameSize=8;
	
	KGraphic *_lpImgGrid;
	KGraphic *_lpImgMiror1, *_lpImgMiror2;
	KGraphic *_lpImgWrong;
   KGraphic *_lpImgArrow;
	EFont *_lpFont;

	std::vector< EMirorPart * > _vMirors;
	std::vector< int > _nNumPart;
	
	// Game Solution
	std::vector< std::vector< int > > _vGameSet;
	std::vector< std::vector< char * > > _vPair;
	std::vector< std::vector< Path * > > _vPaths;

	// Game Grid
	std::vector< std::vector< int > > _vGame;	
	std::vector< Grid * > _vGridPair;
	Grid *_lpGridGame;

	KGraphic *_lpSelBg;
	std::vector< EMirorPart * > _vPartSet;
	
	Grid *_lpGridSet;
	EMirorPart *_lpGrabedPart;
   Coord _arrowPos;

	// Game Helper
	void GenerateGrid();

	static EMiniJeuMirorBeam *IMG;
};

class Path {
public:
	friend class EMiniJeuMirorBeam;

	Path(int pos, int indGridPair);
	~Path(void);
	
	static void Draw();
	void DrawPaired();
	void DrawPath(bool single=false);
	
	// Helper
	struct CellPath {
		Coord cell;
		KGraphic *imgInd;
	public:
		CellPath() {
			cell = Coord(-1,-1);
			imgInd = NULL;
		}
		CellPath(Coord t, KGraphic *i) {
			cell = t;
			imgInd = i;
		}
	};
	std::list< CellPath > typedef ListPath;

	Coord ResolvePath();
	bool NextStep(Coord *cell, EMiniGame::Direction *to, KGraphic **imgInd);
	bool GoOver(Coord c);

	ListPath _cells;
	Coord _startCell, _endCell;
	bool _bPaired;
	
	static KCounter *_lpDisplayMod;
	static Coord _cDisplayMod;
	static int _nDisplayPath;

	static std::vector< KGraphic * > _vBeamImg;
	static EMiniJeuMirorBeam *IMG;
	static int ninst;
	static void ResolvePaths();
};

class EMirorPart: public EMiniJeuPart {
public:
	EMirorPart(KGraphic *img): EMiniJeuPart(img,1,0) {;}
	EMirorPart(const EMirorPart* a): EMiniJeuPart(a) {;}
	~EMirorPart(void) {;}
};
#endif
