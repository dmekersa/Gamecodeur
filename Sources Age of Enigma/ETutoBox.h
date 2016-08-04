#ifndef ETUTOBOX_H
#define ETUTOBOX_H

#include "KPTK.h"
#include "KCounter.h"
#include "EMiniJeuHelper.h"

#include "EBox.h"

class ETutoArrow;

class ETutoBox: public EImgExpendableBox {
public:
	ETutoBox(const char *tutoText,float posXArrow=-1, float posYArrow=-1, float dir=0, float clickRayon = 0);
	~ETutoBox();

	void Draw();
	void Check();

	void Hide();
	void Show();
	bool IsVisible();

	bool IsCompleted();

	bool HasArrow();
	void SetArrowDir(float angle);
	float GetArrowDir();
	
	Coord GetArrowPos();
	void MoveArrow(float x, float y);
	void MoveArrow(Coord pos) { MoveArrow(pos.x,pos.y);}

	static ETutoBox* GetCurrentTuto();
	static ETutoBox *_lpCurrentTuto;
private:
	void ComputePos();
	void UpdateAlpha();
	void ToggleCursor();
	void CloseTuto();

	float _fFadDuration;

	KCounter *_fTutoBlend;
	float _fClickRayon;

	bool _bCompleted;
	bool _bWasOver;

	ETextBox *_lpTutoText;
	ETextBox *_lpTextClickVariation;
	ETutoArrow *_lpArrow; 
};

class ETutoArrow {
public:
	ETutoArrow(float x, float y, float dir, float scalemax=1.20f, float scaleduration=500.0f, float fadduration=1000.0);
	~ETutoArrow();

	void Draw();

	void Move(float x, float y);
	void SetDir(float dir);
	void SetArrow(KGraphic *arrow);
	void SetBlend(float blend);

	Coord GetPos();
	Coord GetPosImg();
	float GetDir() {return _fAngle;}
	KGraphic* GetImg() {return _lpArrow;}
	void SetScaleParm(float scale, float duration);
	void SetFadeDuration(float duration);

	void Hide(float duration=-1);
	void Show(float duration=-1);
	bool IsVisible();

	float _x, _y, _fAngle;
protected:
	KCounter *_lpCounterScale;
	float _fScaleDuration;
	float _fScaleMax;

	KCounter *_lpCounterFad;
	float _fFadDuration;

	float _xImg, _yImg, _w, _h;

	KGraphic *_lpArrow;
};
#endif
