#ifndef EMINIJEUMISSINGPARTS_H
#define EMINIJEUMISSINGPARTS_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EMissingPart;

class EMiniJeuMissingParts :
	public EMiniJeuBase
{
public:
	EMiniJeuMissingParts(const char *imgName="ejigsaw_book.png", const char *imgBGName="mg_mirorsgrid_bg.png", int xpos=-1, int ypos=-1, const char *imgEnd="mg_missingpart_part_f1.png", int xend=-1, int yend=1);
	virtual ~EMiniJeuMissingParts(void);

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
	
	void FreeImgCut();
	bool TestSolColision(int sol, int w, int h);

	int _nGameSize;
	int _nNumResponse;

	KImage *_lpBufferImg;
	KGraphic *_lpImg;
	Coord _posImg;
	EMiniJeuPart *_lpImgEnd;

	std::vector< const char * > _vMasksPath;
	std::vector< EMissingPart * > _vMissingPart;
	
	Grid *_lpGridResponse;
};

class EMissingPart: public EMiniJeuPart {
public:
	EMissingPart(KGraphic *Img);
	~EMissingPart(void);

	virtual void Check();
	void Draw();

	void Deselect(bool Anim=true);
	void Select(bool Anim=true);

	bool IsSelected();
	bool IsSol() { return _bIsSol;}
	bool ValidAnswer();

	void SetImgInPos(Coord pos) {_posInImg = pos;}
	void SetIsSol(bool sol = true) {_bIsSol = sol;}

	Coord _posInImg;

private:
	bool _bIsSelected;
	bool _bIsSol;
	bool _bError;

	KGraphic *_lpOverlaySelected;

	static float _fTimeout;
	KCounter *_lpAlphaSelect;

	float _fAlphaSelect;
	float _fScaleSelect;

	KCounter *_lpCounterA;
};
#endif
