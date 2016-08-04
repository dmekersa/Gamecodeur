#ifndef EMINIJEUMEMORYPLANT_H
#define EMINIJEUMEMORYPLANT_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EMemoryPlantPart;
class EMemoryPlantQuery;

class EMiniJeuMemoryPlant :
	public EMiniJeuBase
{
public:
	EMiniJeuMemoryPlant(void);
	virtual ~EMiniJeuMemoryPlant(void);

	void SetVariation(int nVariation);

   static void Preload();

private:
	friend class EMemoryPlantQuery;
	friend class QueryVariante0;
	friend class QueryVariante1;
	friend class QueryVariante2;

	/** 3 steps rendering **/
	void GameCheck ();
	void GameLogic ();
	void GameDrawSet ();

	bool GameIsResolve();
	void GameResolve();
	void GameReset();
	void GameSetHint(int nHintLevel);

	int _nXGameSize,_nYGameSize;
	float _fShowTimeDuration;

	EFont *_lpFont;
	EFont *_lpFontStart;
	EFont *_lpFontTimeout;

	char _cTextStart[254];
	char _cTextTimeout[254];
	char _cTextTimeoutSeconds[254];
	int _nSecondRemaining;

	EMiniJeuPart *_lpImgStart;

	bool _bDisplayStart;
	KCounter *_lpShowTime;

	std::vector< EMemoryPlantPart * > _vPartSet;
	std::vector< std::vector< EMemoryPlantPart * > > _vGame;

	Grid *_lpGridGame;

	EMemoryPlantQuery *_lpQueryVariant;
};


// ------------------------------------------ //
class EMemoryPlantPart: public EMiniJeuPart {
public:
	EMemoryPlantPart (KGraphic *lpGraphic, KGraphic *Color): EMiniJeuPart(lpGraphic,1)  {_Color = Color;}
	EMemoryPlantPart (const EMemoryPlantPart* a): EMiniJeuPart(a) {_Color = a->_Color;}

	KGraphic *_Color;
};

// ------------------------------------------ //
class EMemoryPlantQuery {
public:
	EMemoryPlantQuery(EMiniJeuMemoryPlant *game, const char *cQuery);
	virtual ~EMemoryPlantQuery(void);

	virtual void InitQuery() = 0;

	virtual void Draw() = 0;
	virtual void Check() = 0;
	virtual bool IsResolved() = 0;
	virtual void Resolve() = 0;
	virtual void SetHint() = 0;

protected:
	void DrawQuery();

	EMiniJeuMemoryPlant *MG;
	KGraphic *_lpImgQuery;
	KGraphic *_lpImgError;

	EFont *_lpFont;

	char _cTextQuery[256];
	char _cTextHint1[256];
	char _cTextHint2[256];

	Coord _cTextQueryCentered;
	float _fYTextQuerry, _fQueryHeight;

	Coord _pBlendBox1, _pBlendBox2;
};


// ------------------------------------------ //
class QueryVariante0: public EMemoryPlantQuery {
public:
	QueryVariante0(EMiniJeuMemoryPlant *game);
	virtual ~QueryVariante0(void);

	void InitQuery();

	void Draw();
	void Check();
	bool IsResolved();
	void Resolve();
	void SetHint();

private:
	std::vector< std::vector< bool > > _vSelect;
	std::vector< std::vector< KGraphic * > > _vHint;
	
	KGraphic *_lpImgPlant;
	KGraphic *_lpImgBoxPlant;
	Coord posPlant, posBox;

	KGraphic *_lpHintPlant1;
	KGraphic *_lpHintPlant2;
};
class QueryVariante1: public EMemoryPlantQuery {
public:
	QueryVariante1(EMiniJeuMemoryPlant *game);
	virtual ~QueryVariante1(void);

	void InitQuery();

	void Draw();
	void Check();
	bool IsResolved();
	void Resolve();
	void SetHint();

private:
	std::vector< std::vector< bool > > _vSelect;
	std::vector< std::vector< KGraphic * > > _vHint;
	
	KGraphic *_lpImgColor;
	Coord posColor;

	KGraphic *_lpHintPlant1;
	KGraphic *_lpHintPlant2;
};
class QueryVariante2: public EMemoryPlantQuery {
public:
	QueryVariante2(EMiniJeuMemoryPlant *game);
	virtual ~QueryVariante2(void);

	void InitQuery();

	void Draw();
	void Check();
	bool IsResolved();
	void Resolve();
	void SetHint();

private:
	std::vector< KGraphic * > _vColor;
	Grid *_lpGridColor;

	float _fScaleColorBox;
	KGraphic *_lpColorResponse;
	Coord posQuery;
};
#endif
