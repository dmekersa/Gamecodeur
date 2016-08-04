#ifndef EMINIJEUMIKADO_H
#define EMINIJEUMIKADO_H

#include <list>

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EMikadoPart;

class EMiniJeuMikado: public EMiniJeuBase {
public:
	EMiniJeuMikado(void);
	virtual ~EMiniJeuMikado(void);

   static void Preload();

private:
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();

	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset();
	virtual void GameReset2();
	virtual void GameSetHint(int nHintLevel);

	EMikadoPart* GetRandomPart();
	Coord GetRandomPos();

	bool Intersect(EMikadoPart* A, EMikadoPart* B);
	Coord LineIntersection(EMikadoPart* A, EMikadoPart* B);

	int _nGameSize;

	std::vector < EMikadoPart * > _vSolution;
	// Submit Game
	// std::list< EMikadoPart * > _vCurrentGame;
	float _fPartWidth;
	//KGraphic *_test;

	// Couronne de placement
	Coord _fCc;		// Centre de la couronne
	float _fCi;		// Rayon interne de la couronne
	float _fCe;		// Rayon externe de la couronne
};

class EMikadoPart {
public: 
	EMikadoPart(float x=0, float y=0, double dAngle=0);
	virtual ~EMikadoPart(void);

	void Check();
	void Draw();

	void SetAngle(double dAngle);
	void Move(float x, float y);
	void Move(Coord c) {Move(c.x,c.y);}

	float GetAngle() {return (float)_dAngle;}   
	Coord GetPoint1() {return a;}
	Coord GetPoint2() {return b;}
	
	void Remove() {_bRemoved = true;}

	bool IsSelected () {return _bSelected;}
	bool IsClicked();
	bool IsRemoved() {return _bRemoved;}

	void Select() {_bSelected = true;}
	void DeSelect() { _bSelected = false;}

	float GetWidth() {return _lpPart->getWidth();}
	Coord a, b, c;

private:
	Coord p1, p2, p3, p4;

	double _dAngle, _dCoefDir, _dOrdOrig, _dPCoefDir, _dPOrdOrig;
	double _dPartLength, _dDistance;

	bool _bSelected;
	bool _bRemoved;

	KGraphic* _lpPart;
	KGraphic* _lpPartSelected;
};
#endif