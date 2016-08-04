#ifndef EMINIJEUSTONES_H
#define EMINIJEUSTONES_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"


class EStonePart;

class EMiniJeuStones: public EMiniJeuBase
{
public:
	EMiniJeuStones(void);
	virtual ~EMiniJeuStones(void);

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

	std::vector< EStonePart * > _vProposal;
};

class EStonePart: public EMiniJeuPart {
public:
	EStonePart(KGraphic *img, int x, int y);
	virtual ~EStonePart(void);

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