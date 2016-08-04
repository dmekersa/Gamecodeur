#ifndef EMINIJEUHELPER_H
#define EMINIJEUHELPER_H

#include <list>
#include "global.h"
#include "KGraphic.h"
#include "KPTK.h"
#include "KCounter.h"
#include "FontBank.h"
#include "EFont.h"

// Generic graph tupple, A utiliser a la place de std::pair?
struct Coord {
	float x;
	float y;

	Coord() {x=0; y=0;} // Garantie une initialisation consistante sur tous les compilateurs.
	Coord(float _x, float _y) {x=_x; y=_y;}
	Coord(int _x, int _y) {x=(float)_x; y=(float)_y;}

	bool operator== (struct Coord const& a) const {
		return this->x == a.x && this->y == a.y;
	}
	bool operator!= (struct Coord const& a) const {
		return ! (*this == a);
	}
	bool operator> (struct Coord const& a) const {
		return this->x > a.x && this->y > a.y;
	}
	bool operator< (struct Coord const& a) const {
		return this->x < a.x && this->y < a.y;
	}
	bool operator>= (struct Coord const& a) const {
		return *this>a || *this==a;
	}
	bool operator<= (struct Coord const& a) const {
		return *this<a || *this==a;
	}
	Coord operator- (struct Coord const& a) const {
		return Coord(this->x - a.x, this->y - a.y);
	}
	Coord operator+ (struct Coord const& a) const {
		return Coord(this->x + a.x, this->y + a.y);
	}
	Coord operator* (int const& a) const {
		return Coord(this->x * (float)a, this->y * (float)a);
	}
	Coord operator* (float const& a) const {
		return Coord(this->x * a, this->y * a);
	}

	void operator+= (struct Coord const& a) {
		this->x += a.x;
		this->y += a.y;
	}
	void operator-= (struct Coord const& a) {
		this->x -= a.x;
		this->y -= a.y;
	}
};

namespace EMiniGame {
	typedef enum {
		DEADEND = -1,
		WEST = 0,
		SOUTH,
		EAST,
		NORTH
	} Direction;

	Coord GetDirVector(int dir);

	int GetRandom(int Max);
};

/* ___________________________________ */

typedef enum {
		GRID_DEFAULT = 0,
		GRID_QUINCONCE_H_P,		// Grille en quinconce horizontal la première col etant la plus grande
		GRID_QUINCONCE_H_I,		
		GRID_QUINCONCE_V_P,		// Grille en quinconce vertical la première ligne etant la plus grande
		GRID_QUINCONCE_V_I,
		GRID_QUINCONCE_P,		// Grille en quinconce
		GRID_QUINCONCE_I,		
} gridtype;

// Grid Draw Helper
class Grid {
public:

	Grid(int _nCol=1, int _nLine=1, float _wCell=10, float _hCell=10, float _wBorder=0, float _wInnerGrid=0, gridtype _type=GRID_DEFAULT);

	void DrawBaseGrid(KGraphic *renderer, float fBlend=1) const;
   void DrawGrid(KGraphic *renderer, float fBlend=1) const;
	void FillCell(KGraphic *renderer, int col, int line, float fBlend=1) const;
	void FillCell(KGraphic *renderer, int col, int line, float r, float v, float b, float fBlend=1) const;
	void FillCell(KGraphic *img, int col, int line, bool scale, float fBlend=1) const;
	
	void FillCell(const char *cStr, int col, int line) const;
	
	struct Coord GetSelection(float xClick, float yClick) const;
	struct Coord GetSelection(struct Coord cClick) const {return GetSelection(cClick.x,cClick.y);}
	struct Coord GetMouseSelection() const;
	
	void GetZone(int col, int line, struct Coord *cPos1, struct Coord *cPos2) const;
	struct Coord GetCellPos(int col, int line) const;
	struct Coord GetCellPos(struct Coord c) const {return GetCellPos((int)c.x,(int)c.y);}
	struct Coord GetCellPos(int numCell) const {return GetCellPos(GetCellCoord(numCell));}
	struct Coord GetCellCenter(int col, int line) const;
	struct Coord GetCellCenter(struct Coord c) const {return GetCellCenter((int)c.x,(int)c.y);}
	struct Coord GetCellCenter(int numCell) const {return GetCellCenter(GetCellCoord(numCell));}
	struct Coord GetCellSize() const;

	void Move(float x, float y);
	void Move(Coord c) {Move(c.x,c.y);}

	void SetDefaultColor( float r, float g, float b);
	void SetBorderColor( float r, float g, float b);
	void SetInnerGridColor( float r, float g, float b);
	void SetFillColor( float r, float g, float b);
	void SetQDeltaSelection(float d);

	void SetLineOdd(bool x);
	void SetColOdd(bool x);

	void SetCellWidth(float x);
	void SetCellHeight(float y);
	void SetFont(EFont *font);

	bool IsOver(float x, float y) const;
	bool IsOver(struct Coord c) const {return IsOver(c.x,c.y);}
	bool IsMouseOver(void) const;

	bool CheckQCell(int col, int line) const;
	bool CheckQCell(Coord c) const {return CheckQCell((int)c.x,(int)c.y);}
	int GetQColSize(int line) const;
	int GetQLineSize(int col) const;

	float width() const;
	float height() const;
	float GetBorder() const {return _wBorder;}
	float GetInnerGrid() const {return _wInnerGrid;}

	int GetCellCount() const;
	Coord GetCellCoord(int cellNum) const;

	float _xPos, _yPos;
	float _wCell, _hCell;
	int _nCol, _nLine;

private:
	gridtype _type;

	
	float _wBorder, _wInnerGrid;

	float _dfR, _dfG, _dfB;
	float _bdR, _bdG, _bdB;
	float _igR, _igG, _igB;
	float _fiR, _fiG, _fiB;

	bool _bLineOdd, _bColOdd;

	// Paramètre pour la disposition en quinconce des pièces
	float _fQDelta;
	int   _nQOdd;
	int   _nQHOdd;
	int   _nQVOdd;
	int   _nQ;
	int   _nQH;
	int   _nQV;
	float _fQDistSel;

	EFont *_lpFont;
	int _nFontSize;
};

/* ___________________________________ */

class EMiniJeuPart {
public:
	EMiniJeuPart (KGraphic *lpGraphic, int nNumAngle=4,  int nStartInd=0, float fBaseAngle=0, float AngleStep=0, float fAlpha=1, float w=0, float h=0);
	EMiniJeuPart (const char * ImgPath, int nNumAngle=4,  int nStartInd=0, float fBaseAngle=0, float AngleStep=0, float fAlpha=1, float w=0, float h=0);
	EMiniJeuPart(const EMiniJeuPart* a);
	
	virtual ~EMiniJeuPart(void);
	
	virtual void operator= (EMiniJeuPart const a);
	virtual bool operator== (EMiniJeuPart const& a) const;
	virtual bool operator== (EMiniJeuPart* const a) const {return (*this==*a);}
	virtual bool operator!= (EMiniJeuPart const& a) const {return !(*this==a);}
	virtual bool operator!= (EMiniJeuPart* const& a) const {return !(*this==*a);}

	virtual bool IsCopy() const {return _bLocalAlloc;}

	virtual void Logic();
	virtual void Check();
	virtual void Draw();

	void SetElapsedTime(double dElapsedTime) {_dElapsedTime = dElapsedTime;};

	void Move(float x, float y) {_x=x;_y=y;}
	void Move(struct Coord c) {Move(c.x,c.y);}

	void MoveCenterTo(float x, float y) {_x=x-(_w*_fScale/2);_y=y-(_h*_fScale/2);}
	void MoveCenterTo(struct Coord c) {MoveCenterTo(c.x,c.y);}

	void Move(float x, float y,float fDuration, bool force=false, KCounterFunction counterType=K_COUNTER_LINEAR);
	void Move(struct Coord c,float fDuration, bool force=false, KCounterFunction counterType=K_COUNTER_LINEAR) {Move(c.x,c.y,fDuration, force, counterType);}

	void MoveCenterTo(float x, float y,float fDuration, bool force=false, KCounterFunction counterType=K_COUNTER_LINEAR) {Move(x-(_w*_fScale/2),y-(_h*_fScale/2),fDuration, force, counterType);}
	void MoveCenterTo(struct Coord c,float fDuration, bool force=false, KCounterFunction counterType=K_COUNTER_LINEAR) {MoveCenterTo(c.x,c.y,fDuration, force, counterType);}

	void StopMove();
	void FinishMove();
	
	// Gestion angle de base et orientation de la pièce
	void SetBaseAngle(float fDeg) {_fBaseAngle=fDeg;}
	void SetAngleStep(float fDeg) {_fAngleStep=fDeg;}
	void SetOrientation(int nIndOrientation);
	void SetRotationDuration(float fDuration) {_fRotationDuration=fDuration;}
	void SetMaxBlend(float fMaxBlend) {_fMaxBlend = fMaxBlend; if (_fBlend >0) _fBlend = _fMaxBlend;}

	virtual Coord GetPosOffset();
	int GetCurrentStep() const {return _nCurrentStep;}
	int GetAbsoluteOrientation() const {return (_nCurrentStep+_nSide)%_nSide;}
	float GetAngle() const;
	KGraphic * GetImg() const {return _lpImg;}
	Coord Center() const;
	Coord GetPos(); // Offset included
	float GetAlpha() {return _fBlend;}
	float GetMaxAlpha() {return _fMaxBlend;}
	
	virtual void RotateCW(bool bAnim=true);	// Rotation sens horaire
	virtual void RotateACW(bool bAnim=true);	// Rotation sens antihoraire
	virtual void RotateCW(int nStep);	// Rotation animé implicite
	virtual void RotateACW(int nStep);	// Rotation animé implicite
	virtual void RotateToStep(int nStep); // Rotation animé implicite

	// Gestion de la capture de la pièce
	virtual void Release() {_bGrabed=false; if (_x<0 || _y<0) ResetPos();} // A completer si necessaire avec un check de position (ex: zone de jeux)
	virtual void Grab(bool setPrecedentPos=true) {
		if (_bDragable) {
			_bGrabed = true;
			if (_bAnimMove)
				StopMove();
			if (setPrecedentPos) {
				_preGrabX=_x;
				_preGrabY=_y;
			}
		}
	}
	virtual void SwitchDrag() {if (_bDragable&&_bGrabed) Release(); _bDragable=!_bDragable;} 
	virtual void SwitchRotation() {_bRotation=!_bRotation;} 
	virtual void ResetPos() {_x=_preGrabX;_y=_preGrabY;}

	bool IsOver(struct Coord c) const;
	bool IsMouseOver() const;
	bool IsGrab() const {return _bGrabed;}
	bool IsDragable() const {return _bDragable;}
	bool IsVisible() const;

	float Width() const {return _w;}
	float Height() const {return _h;}
	struct Coord Size() const {return Coord(_w,_h);}
	struct Coord Pos() const {struct Coord c;c.x=_x;c.y=_y;return c;}
	struct Coord GetDest() const {if (!MoveComplete()) return _posDest; return Pos();}

	void Show();
	void Hide();

	bool FadIn(double fDuration=0, bool force=false); 
	bool FadOut(double fDuration=0, bool force=false); // A implementer
	void StopFad();
	bool FadInComplete() const {return ! _bAnimFad && _fBlend != 0;} // A implementer
	bool FadOutComplete() const {return ! _bAnimFad && _fBlend == 0;} // A implementer
	bool FadComplete() const {return ! _bAnimFad;}
	bool RotationComplete () const {return ! _bAnimRotate;} // A implementer
	bool MoveComplete() const {return ! _bAnimMove;}
	bool TransformComplete() const;

protected:
	bool _bSkipDraw;

	double _dElapsedTime;
	float _fRotationDuration;

	bool _bDragable, _bRotation;
	bool _bGrabed;
	
	bool _bAnimMove;
	KCounter *_lpAnimMoveX;
	KCounter *_lpAnimMoveY;

	KCounter *_lpAnimFad;
	bool _bAnimFad;

	KCounter *_lpAnimRotate;
	bool _bAnimRotate;

	KGraphic * _lpImg;
	bool _bLocalAlloc;

	float _x, _y;
	float _w, _h;
	float _preGrabX, _preGrabY;

	int _nSide, _nCurrentStep, _nRotationStepRemaining;
	float _fBaseAngle, _fAngleStep, _cAngle;
	float _fBlend, _fMaxBlend;
	float _fScale;
	Coord _posDest;
};

class Digit {
public:
	Digit(EFont *lpFont, float scale=1, float digitsize=0);
	~Digit(void);
	
	void Check();
	void Draw();
	void Reset();
	
	void SetVal(int x);
	void SetMargin(int x);
	void SetImgScale(float x);
	int GetVal();

	void Move(int x, int y);
	void Move(Coord pos);

	void MovePlus(int x, int y);
	void MovePlus(Coord pos);

	float width();
	float height();
	float X();
	float Y();
	float x();
	float y();
	float FontHeight();
private:
	void Incr();
	void Decr();

	int GetDigitWidth(int digit);

	EFont *_lpFont;

	KGraphic *_lpImgPlus, *_lpImgMoins;
	Coord _posPlus, _posMoins, _posNumber;

	float _fScale;
	int _nImgWidth, _nImgHeight;
	int _nMargin, _nCharMaxWidth, _nVal;
	int _nCurrentDigitWidth,_nDigitSize;
};

KImage *GetKImage(const char* szImageName, const char *szExt = "png");
std::vector< bool >* GenerateAlphaMask(const char* szImageName, const char *szExt = "png");

/* ___________________________________ */

class Polygone {
public:
   Polygone();
   ~Polygone();

   void Draw();

   void AddPoint(float x, float y);
   void ClearPoints();

   bool IsOver(float x, float y);
   bool IsOver(Coord pos);
   bool IsMouseOver();

private:
   KGraphic *target;
   std::list< Coord > _lpListPoint;
};
#endif