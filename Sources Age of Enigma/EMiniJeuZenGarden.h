#ifndef EMINIJEUZENGARDEN_H
#define EMINIJEUZENGARDEN_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EZenGardenPart;

class EMiniJeuZenGarden: public EMiniJeuBase
{
public:
	EMiniJeuZenGarden(void);
	virtual ~EMiniJeuZenGarden(void);

   static void Preload();

private:
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();

	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset();
	virtual void GameSetHint(int nHintLevel);
	
	int _nIndSol;

	std::vector< EZenGardenPart * > _vProposal;
};

class EZenGardenPart: public EMiniJeuPart {
public:
	EZenGardenPart(KGraphic *img, int x, int y);
	virtual ~EZenGardenPart(void);

	virtual void Check();
	void Draw();

	void Disable(bool Anim=true);
	void Deselect(bool Anim=true);
	void Select(bool Anim=true);

	bool IsDisabled();
	bool IsSelected();

	static void SetRefPos(Coord pos);
private:
	bool _bIsSelected;
	bool _bIsDisabled;

	KGraphic *_lpOverlaySelected;
	
	static int _nX;
	static int _nY;

	static float _fTimeout;
	KCounter *_lpAlphaSelect;
	KCounter *_lpAlphaDisable;

	float _fAlphaSelect;
	float _fAlphaDisable;
};
#endif