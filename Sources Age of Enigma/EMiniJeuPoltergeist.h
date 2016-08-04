#ifndef EMINIJEUPOLTERGEIST_H
#define EMINIJEUPOLTERGEIST_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "ESmartImage.h"
#include "KVideo.h"
#include "CreepingSmoke.h"

#include "KPTK.h"

class EPoltergeist;

class EMiniJeuPoltergeist:
	public EMiniJeuBase
{
public:
	EMiniJeuPoltergeist(void);
	virtual ~EMiniJeuPoltergeist(void);

   static void Preload();

private:
	/** 3 steps rendering **/
	void GameCheck ();
	void GameLogic ();
	void GameDrawSet();

	bool GameIsResolve();
	void GameResolve();
	void GameReset();
	void GameSetHint(int nHintLevel);

	void ShowImg();
	
	int _nGameSize;

	ESmartImage *_lpImgAshley;
	ESmartImage *_lpVideoPol;

	KGraphic *_lpImgLifeBox;
	KGraphic *_lpImgLife;
	Coord _posLifeBox;

	KGraphic *_lpSceneBG;
	KGraphic *_lpBigBadPol;

	int _nMaxDegats;
	// Display de la force du coup
	//int _nMaxDegats, _nDegats, _nStartDegats;
	//KCounter *_lpCounterDegats;

	EPoltergeist *_lpPoltergeist;
	CreepingSmokeEmitter *_lpEmitterHit;
	
	KCounter *_lpCounter;
	KCounter *_lpCounterDisplayPol;

	EFont *_lpFont;
	char _cHintPoltergeist[256];

	KGraphic *_lpImgBg;
	bool _bShowImg;
};

class EPoltergeist: public EMiniJeuPart {
public:
	EPoltergeist(ESmartImage *sPol);
	~EPoltergeist(void);

	void Logic();
	void Draw();
	void DrawBack();
	void DrawFront();

	void HitPol(int nPV);
	float PV();

	bool IsDead() {return _lpCounterPV->getCurrentValue() <= 0;}
	void KillPol();
	void SetDrawParm();

	KVideo *GetFinalVideo();

private:
	void RandomZoneMove();
	void RandomMove();
	int GetCurrentZone();

	std::string _sImageNameBase;
	int _nXforbid, _nYforbid, _nWforbid, _nHforbid;
	
	static const int _nNumZoneX = 2;
	static const int _nNumZoneY = 2;
	static const int _nImagePol = _nNumZoneX * _nNumZoneY;
	
   KGraphic *_lpHalo;
	ESmartImage* _lpImgPol;
	CreepingSmokeEmitter *_lpEmitterTail;
	
	float _fBaseBlend, _fDeltaBlend;
	int _nMaxPV, _nPV, _prevPV;
	KCounter *_lpCounterPV;
	bool _bHit;

	float _fHealTime;
	KCounter *_lpCounterHeal;

	int _nCurrentZone, _nOldZone;
	bool _bFromBehind, _bSwitchZone,_bFlip;
	float _fAngle;
};
#endif