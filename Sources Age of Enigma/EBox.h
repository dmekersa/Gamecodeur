#ifndef EBOX_H
#define EBOX_H

#include "KPTK.h"
#include "EMiniJeuHelper.h"
#include "EFont.h"

class EBox {
public:
	enum LayoutType {VERTICAL,HORIZONTAL};
	enum HorisontalPolicy {LEFT,HCENTER, RIGHT};
	enum VerticalPolicy {UP,VCENTER, DOWN};
	enum SizePolicy {INHERIT, FREE, MAXIMIZE}; //,FIXED,PREFEXPAND,EXPAND} typedef SizePolicy;

	EBox(float x, float y, float w,float h, float marginL=0, float marginR=0, float marginU=0, float marginD=0);
	virtual ~EBox();
	
	virtual void Draw();

	void SetCentered(bool centered);
	bool IsCentered() {return _bCentered;}
	void SetBoxCenter(float x, float y);
	void SetBoxCenter(Coord newcenter) { SetBoxCenter(newcenter.x,newcenter.y);}
	Coord GetCenter() {return Coord(_x + _w/2,_y + _h/2);}
	
	void SetGeometry(float x, float y, float w, float h);
	void Move(float x, float y);
	void Move(Coord p) { Move(p.x,p.y);}
	void MoveRel(float x, float y);
	void MoveRel(Coord delta) { MoveRel(delta.x,delta.y);}

	void SetSize(float width, float height);
	void SetSize(Coord size) {SetSize(size.x,size.y);}
	void SetWidth(float width);
	void SetHeight(float height);
	
	Coord GetPos();
	Coord GetSize();
	virtual float GetWidth();
	virtual float GetHeight();
	float GetInnerWidth() {return GetWidth() - GetHMargin();}
	float GetInnerHeight(){return GetHeight() - GetVMargin();}
	Coord GetInnerSize() {return Coord(GetInnerWidth(),GetInnerHeight());}
	void SetMargin(float left, float right, float up, float down);

	Coord GetLeftRightMargin();
	Coord GetTopBottomMargin();
	Coord GetLeftTopMargin();
	Coord GetRightBottomMargin();
	float GetHMargin();
	float GetVMargin();

	void SetLayoutType(LayoutType layoutType=HORIZONTAL);
	void SetHorisontalPolicy(HorisontalPolicy policy);
	void SetVerticalPolicy(VerticalPolicy policy);
	void SetSizePolicy(SizePolicy policy);

	virtual void AddChild(EBox *child);
	virtual void RemoveChild(EBox *child);
	virtual void SetParent(EBox *parent);
	virtual EBox* GetParent() { return _lpParent;}
	void InheritSize();
	bool IsChild(EBox *box);
	
	int GetNumbChilds();
	EBox* GetChild(int nChild);
	void IterInitialize();
	EBox* IterNextChild();

	bool IsOver();
	bool IsOver(Coord pos);

	virtual void SetBlend(float ratio=-1);
	float GetBlend();

	void UpdateUI();
protected:
	Coord GetMinSize();
	void SetMinSize();
	virtual float ComputeMinInnerWidth() { return 0;}
	virtual float ComputeMinInnerHeight() { return 0;}
	virtual void SetMinWidth();
	virtual void SetMinHeight();
	virtual float GetMinWidth();
	virtual float GetMinHeight();

	void UpdateGeometry();
	void UpdateChild();

	float _x,_y,_w,_h;
	float _wHint, _hHint;
	float _minInnerWidth, _minInnerHeight;
	float _fChildMinWidth, _fChildMinHeight;
	float _marginLeft, _marginRight, _marginUp, _marginDown;


	LayoutType _layoutType;
	std::vector< EBox * > _vChilds;
	HorisontalPolicy _hPolicy;
	VerticalPolicy _vPolicy;
	SizePolicy _sizePolicy;

	bool _bCentered;
	Coord _center;

	EBox *_lpParent;

	// blend param
	float _fMaxBlend;
	float _fBlend;
   
   int _nNumMaximizeChild;
	int _nIterIndice;
};

class EImgBox: public EBox {
public:
	EImgBox(KGraphic *imgBox, float margin = 2, float x=0, float y=0);
	virtual ~EImgBox();

   void SetImg(KGraphic *imgBox);

	void Draw();
	
private:
	float ComputeMinInnerWidth();
	float ComputeMinInnerHeight();

	KGraphic *_lpImgBox;
	float _fImgWidth, _fImgHeight;
};

class EImgExpendableBox: public EBox {
public:
	EImgExpendableBox(KGraphic *imgBox, float wLUCorner, float hLUCorner, float wRDCorner, float hRDCorner, float margin = 2, float x=0, float y=0);
	virtual ~EImgExpendableBox();

	void Draw();
	
protected:
	KGraphic *_lpImgBox;

private:
	float ComputeMinInnerWidth() { return _wLUCorner + _wRDCorner;}
	float ComputeMinInnerHeight() { return _hLUCorner + _hRDCorner;}
	

	float _wLUCorner, _wCenter, _wRDCorner;
	float _hLUCorner, _hCenter, _hRDCorner;

	float _fImgWidth, _fImgHeight;
};


class ETextBox: public EBox {
public:
	ETextBox(const char *text="", EFont *font=NULL, float width=350, float minheight=-1,int textStyle=TEXTSTYLE_CENTER, int nFontHeight=-1);
	virtual ~ETextBox();

	void Draw();

	void SetText(const char *text);
	void SetFont(EFont *font, int nFontHeight=-1);
   void SetTextColor(float r, float g, float b, float a);
   void SetTextOutlineThickness(int Thickness);
   void SetTextOutlineColor (float r, float g, float b, float a);
	
   const char* GetText();

	EFont* GetFont();
protected:
   void DrawOutlineText();

	float GetMinHeight();

	float ComputeMinInnerWidth();

	EFont *_lpFont;
   int _nFontSize;
	char *_cText;
	int _nTextStyle;
   int _nOutlineThickness;

	float _fTextHeight, _fTextWidth, _fMinTextWidth;
   float _textColor[4];
   float _textOutlineColor[4];
};
#endif
