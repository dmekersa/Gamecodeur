#ifndef EMINIJEUJIGSAWBOOK_H
#define EMINIJEUJIGSAWBOOK_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include <list>
#include "KPTK.h"

class EMiniJeuJigsawBook: public EMiniJeuBase
{
public:
	EMiniJeuJigsawBook(void);
	virtual ~EMiniJeuJigsawBook(void);

   void SetGridParam(float x=-1, float y=-1, float outerSpace=3, float innerSpace=2) {
      _cGridPos = Coord(x,y);
      _fGridOuterSpace=outerSpace;
      _fGridInnerSpace=innerSpace;
   }
	void SetJigsawParam (char *cImageName,int nCol=5, int nLine=5, float cXFixed=-1, float cYFixed=-1, int nHintIncrement=1 );
   void SetVariation(int nVariation);

   static void Preload();

private:
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();

	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset();
	virtual void GameSetHint(int nHintLevel); 

	void SetJigsawParam (char *cImageName,int nCol, int nLine, Coord cPointFixed, int nHintIncrement) {
		SetJigsawParam(cImageName, nCol, nLine, cPointFixed.x, cPointFixed.y, nHintIncrement);
	}
	void ShowImg();

	void DrawImagePart(Coord imgSel, Coord gridCell);

   void FreeGrid();

	// variable spécifique au gameplay courrant
	int _nCol;
	int _nLine;
	int _nHintIncrement;
	char *_cImageName;
	Coord _cPointFixed; // permet de fixé une partie de l'image a la bonne place.

	KGraphic *_lpImg;
	KGraphic *_lpImgOk;
	KGraphic *_lpImgCisor;
	KGraphic *_lpImgShadow;

	float _fPartW;
	float _fPartH;

	std::vector < std::vector < Coord *> > _vCurrentGame;
	std::vector < std::vector < KCounter * > > _vCounter;
	Grid *_lpGrid;

	Coord _cCurrentSel;
	bool _bShowImg;
	KCounter * _lpCounter;
   float _fGridInnerSpace;
   float _fGridOuterSpace;
   Coord _cGridPos;
};
#endif