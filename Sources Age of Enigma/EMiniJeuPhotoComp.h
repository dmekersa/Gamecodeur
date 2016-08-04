#ifndef EMINIJEUPHOTOCOMP_H
#define EMINIJEUPHOTOCOMP_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EPhotoCompPart;

class EMiniJeuPhotoComp:
	public EMiniJeuBase
{
public:
	EMiniJeuPhotoComp(const char *imgName="ejigsaw_book.png", const char *imgBGName="mg_mirorsgrid_bg.png", int xpos=-1, int ypos=-1, const char *imgEnd="mg_missingpart_artefact_f1.png", const char *cartouche="mg_photocomp_cartouche.png", int xend=-1, int yend=1);
	virtual ~EMiniJeuPhotoComp(void);

	void SetVersion(int nVersion);

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
	bool TestSolColision(Coord pos, int w, int h);

	int _nGameSize;
	
	KImage *_lpBufferImg;
	KGraphic *_lpImg;
	KGraphic *_lpImgCartouche;

	Coord _posImg;
	EMiniJeuPart *_lpImgEnd;

	std::vector< const char * > _vMasksPath;
	std::vector< EPhotoCompPart * > _vPhotoComp;
	std::vector< Coord > _vDestPos;

	EPhotoCompPart *_lpSelected;

	Grid *_lpGridResponse;
};

class EPhotoCompPart: public EMiniJeuPart {
public:
	EPhotoCompPart(KGraphic *Img);
	~EPhotoCompPart(void);

	virtual void Check();
	void Draw();

	void UnLock(bool Anim=true);
	void Lock(bool Anim=true);
	void Solve();

	bool IsSelected();
	bool ValidAnswer();
	
	void SetImgInPos(Coord pos) {_posInImg = pos;}
	void SetEndPoint(Coord pos) {if (_bIsLocked){_EndPoint = pos;}}

	Coord _posInImg;

private:
	bool _bError;
	bool _bIsLocked;
	Coord _EndPoint;

	KGraphic *_lpImgPoint;

	static float _fTimeout;
	KCounter *_lpAlphaLock;

	float _fAlphaLock;
	float _fScaleLock;

	KCounter *_lpCounterA;
};
#endif
