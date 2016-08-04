#include <limits>
#include "KGraphic.h"
#include "KCounter.h"
#include "KPTK.h"
#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"

#include "EGeometry.h"
#include "EMiniJeuHelper.h"

Coord EMiniGame::GetDirVector(int dir) {
		KAssert(dir >= WEST && dir <= NORTH);
		return Coord((dir -1) % 2,(-(dir-2) % 2));
}

int EMiniGame::GetRandom(int Max)
{
	return ((int)(Max* KRandom::getRandomFloat())%Max);
}

Grid::Grid(int _nCol, int _nLine, float _wCell, float _hCell, float _wBorder, float _wInnerGrid, gridtype _type) {
	this->_type  = _type;
	
	this->_nCol  = _nCol;
	this->_nLine = _nLine;

	this->_wCell=_wCell;
	this->_hCell=_hCell;

	this->_wBorder=_wBorder;
	this->_wInnerGrid=_wInnerGrid;

	_lpFont = NULL;
	_nFontSize = 0;

	_xPos=0;
	_yPos=0;

	_dfR=0.8f;
	_dfG=0.8f;
	_dfB=0.8f;
	_bdR=0.95f;
	_bdG=0.95f;
	_bdB=0.95f;
	_igR=0.95f;
	_igG=0.95f;
	_igB=0.95f;
	_fiR=0.7f;
	_fiG=0.7f;
	_fiB=0.7f;

	_bColOdd=false;
	_bLineOdd=false;
	
	// Paramètre pour la disposition en quinconce des pièces
	_fQDelta = 0;
	_nQOdd = 0;
	_nQHOdd = 0;
	_nQVOdd = 0;
	_nQ = 0;
	_nQH = 0;
	_nQV = 0;
	_fQDistSel = 0;

	if (_type != GRID_DEFAULT) {
		KAssert(((_type == GRID_QUINCONCE_H_P || _type == GRID_QUINCONCE_H_I) && _nLine >= 2) 
			|| 	(  _nCol >= 2));

		switch(_type) {
			case GRID_QUINCONCE_H_I:
				_fQDelta = _hCell / 2;  
				_nQHOdd = 1;
				_nQH = 1;
				break;
			case GRID_QUINCONCE_H_P:
				_fQDelta = _hCell / 2;
				_nQH = 1;
				break;
			case GRID_QUINCONCE_V_I:
				_fQDelta = _wCell / 2;
				_nQVOdd = 1;
				_nQV = 1;
				break;
			case GRID_QUINCONCE_V_P:
				_fQDelta = _wCell / 2;  
				_nQV = 1;
				break;
			case GRID_QUINCONCE_I:
				_fQDelta = _wCell / 2;  
				_nQ = 1;
				_fQDistSel = EGeometry::Distance(Coord(0,0),EGeometry::Center(0,0,_wCell,_hCell));
				this->_wInnerGrid=0;
				break;
			case GRID_QUINCONCE_P:
				_fQDelta = _wCell / 2; 
				_nQOdd = 0;
				_nQ = 1;
				_fQDistSel = EGeometry::Distance(Coord(0,0),EGeometry::Center(0,0,_wCell,_hCell));
				this->_wInnerGrid=0;
				break;
			default:
				break;
		}
	}
};

void Grid::FillCell(const char *cStr, int col, int line) const {
	Coord pos = GetCellPos(col, line);
	_lpFont->drawStringCentered(cStr,pos.x,pos.x + _wCell, pos.y + (_hCell - _nFontSize)/2);
}

void Grid::FillCell(KGraphic *renderer, int col, int line, float fBlend) const {
	Coord cellPos = GetCellPos(col,line);

	KAssert(CheckQCell(col,line));

	renderer->drawRect(cellPos.x,cellPos.y,cellPos.x+_wCell,cellPos.y+_hCell,_fiR,_fiG,_fiB,fBlend);
}

void Grid::FillCell(KGraphic *renderer, int col, int line, float r, float v, float b, float fBlend) const {
	Coord cellPos = GetCellPos(col,line);

	KAssert(CheckQCell(col,line));

	renderer->drawRect(cellPos.x,cellPos.y,cellPos.x+_wCell,cellPos.y+_hCell,r,v,b,fBlend);
}

void Grid::FillCell(KGraphic *img, int col, int line, bool scale, float fBlend) const {
	Coord cellPos = GetCellPos(col,line);
	float sc=1;
	float w=_wCell;
	float h=_hCell;

	KAssert(CheckQCell(col,line));

	if (scale) {
		sc = (float)_wCell/(float)img->getWidth();
		w = img->getWidth();
		h = img->getHeight();
		cellPos.x = cellPos.x-(w-_wCell)/2;
		cellPos.y = cellPos.y-(h-_hCell)/2;
	}
	img->blitAlphaRectFx(0,0,w,h,(short)cellPos.x,(short)cellPos.y,0,sc,fBlend);
}


	

struct Coord Grid::GetSelection(float xClick, float yClick) const {
	Coord pos ;
	pos.x = ((xClick-_xPos-_wBorder)/(_wCell+_wInnerGrid));
	pos.y = ((yClick-_yPos-_wBorder)/(_hCell+_wInnerGrid));

	int nCol = _nCol;
	int nLine = _nLine;
	
	if (_nQ) {
		Coord p1, p2, cCenter, sel;

		sel = Coord(-1,-1);
		float d=0;
		float dmini=_fQDistSel;

		pos.x = xClick;
		pos.y = yClick;

		for (int i=0; i<_nLine;i++) {
			for (int j=0; j<_nCol;j++) {
				if (!CheckQCell(j,i))
					continue;
				GetZone(j,i,&p1,&p2);
				if (pos.x < p1.x || pos.x >= p2.x || pos.y < p1.y || pos.y >= p2.y)
					continue;
				cCenter = EGeometry::Center(p1,p2);
				d = EGeometry::Distance(cCenter.x,cCenter.y,pos.x,pos.y);
				if (d<dmini) {
					dmini = d;
					sel = Coord(j,i);
				}
			}
		}
		return sel;
	}
	if (_nQH == 1) {
		float yDelta = _fQDelta * (((int)pos.x+_nQHOdd) % 2);
		pos.y = ((yClick - yDelta - _yPos - _wBorder)/(_hCell + _wInnerGrid));
		nLine = nLine - (((int)pos.x+_nQHOdd) % 2);
	} else if (_nQV == 1) {
		float xDelta = _fQDelta * (((int)pos.y+_nQVOdd) % 2);
		pos.x = ((xClick - xDelta - _xPos - _wBorder)/(_wCell + _wInnerGrid));
		nCol = nCol - (((int)pos.y+_nQVOdd) % 2);
	}

	if ( pos.x<0 || pos.y<0 || pos.x>=nCol || pos.y>=nLine) {
		pos.x=-1;
		pos.y=-1;
	}

	pos.x = (int)pos.x;
	pos.y = (int)pos.y;
	return pos;
}

struct Coord Grid::GetMouseSelection() const{
	return GetSelection(Mouse);
}

void Grid::GetZone(int col, int line, struct Coord *cPos1, struct Coord *cPos2) const {
	Coord pos;
	cPos1->x=-1;
	cPos1->y=-1;
	cPos2->x=-1;
	cPos2->y=-1;

	if (col>=_nCol || line >=_nLine || ! CheckQCell(col,line))
		return;

	pos = GetCellPos(col,line);

	cPos1->x=pos.x;
	cPos1->y=pos.y;
	cPos2->x=cPos1->x+_wCell;
	cPos2->y=cPos1->y+_hCell;
}

struct Coord Grid::GetCellPos(int col, int line) const {
	int nCol = _nCol;
	int nLine = _nLine;

	float xDelta = 0;
	float yDelta = 0;

	struct Coord c = Coord(-1,-1);

	if (_nQ) {
		nLine = nLine -  ((col+_nQOdd) % 2);
		if (nLine != _nLine)
				yDelta = _fQDelta;
	} else if (_nQH || _nQV) {
		nLine = nLine - (_nQH * ((col+_nQHOdd) % 2));
		nCol = nCol - (_nQV * ((line+_nQVOdd) % 2));

		if (nLine != _nLine) yDelta = _fQDelta;
		if (nCol != _nCol) xDelta = _fQDelta;
	}

	KAssert(col >= 0 && col < nCol && line >=0 && line < nLine);
	
	c.x=((_wCell+_wInnerGrid)*col);
	c.y=((_hCell+_wInnerGrid)*line);
	if (_nQ) {
		if (!_nQOdd)
			c.x=c.x /2;
	}
	c.x=_xPos+_wBorder+c.x + xDelta;
	c.y=_yPos+_wBorder+c.y + yDelta;
	return c;
}

struct Coord Grid::GetCellCenter(int col, int line) const {
	Coord c = GetCellPos(col,line);
	c.x = c.x + (_wCell/2);
	c.y = c.y + (_hCell/2);
	return c;
}

struct Coord  Grid::GetCellSize() const {
	return Coord(_wCell,_hCell);
}

void Grid::DrawBaseGrid( KGraphic *renderer, float fBlend) const {
	float hGrid=height();
	float wGrid=width();

	if (_wBorder)
		renderer->drawRect(_xPos,_yPos,_xPos+wGrid,_yPos+hGrid,_bdR,_bdG,_bdB,fBlend);

	if (_wInnerGrid)
		renderer->drawRect(_xPos+_wBorder,_yPos+_wBorder,_xPos+wGrid-_wBorder,_yPos+hGrid-_wBorder,_igR,_igG,_igB,fBlend);

	for (int i=0; i <_nLine; i++)
		for (int j=0; j<_nCol; j++) {
			if (! CheckQCell(j,i))
				continue;
			FillCell(renderer,j,i,_dfR,_dfG,_dfB,fBlend);

			if (_bColOdd && ((j%2)==0))
				FillCell(renderer,j,i,_dfR-0.1f,_dfG-0.1f,_dfB-0.1f,0.2f);
			if (_bLineOdd && ((i%2)==0))
				FillCell(renderer,j,i,_dfR-0.1f,_dfG-0.1f,_dfB-0.1f,0.2f);
		}
}
void Grid::DrawGrid( KGraphic *renderer, float fBlend) const {
   Coord TL(_xPos,_yPos),BR(_xPos+width(),_yPos+height());

   if (_wBorder) {
      renderer->drawRect(TL.x,TL.y,BR.x,TL.y+_wBorder,_bdR,_bdG,_bdB,fBlend);
      renderer->drawRect(TL.x,TL.y,TL.x+_wBorder,BR.y,_bdR,_bdG,_bdB,fBlend);
      renderer->drawRect(TL.x,BR.y-_wBorder,BR.x,BR.y,_bdR,_bdG,_bdB,fBlend);
      renderer->drawRect(BR.x-_wBorder,TL.y,BR.x,BR.y,_bdR,_bdG,_bdB,fBlend);
   }
  if (_wInnerGrid) {
     int dY, dX;
     for (int i=1;i<_nLine;i++) {
        dY = _wBorder+(i*_hCell)+(i-1)*_wInnerGrid;
        renderer->drawRect(TL.x,TL.y+dY,BR.x,TL.y+dY+_wInnerGrid,_bdR,_bdG,_bdB,fBlend);
     }
     for (int i=1;i<_nCol;i++) {
        dX = _wBorder+(i*_wCell)+(i-1)*_wInnerGrid;
        renderer->drawRect(TL.x+dX,TL.y,TL.x+dX+_wInnerGrid,BR.y,_bdR,_bdG,_bdB,fBlend);
     }
   }
 }
void Grid::Move(float x, float y) {
	_xPos=x; _yPos=y;
}


void Grid::SetDefaultColor( float r, float g, float b) {
	_dfR=r;
	_dfG=g;
	_dfB=b;
}

void Grid::SetBorderColor( float r, float g, float b) {
	_bdR=r;
	_bdG=g;
	_bdB=b;
}

void Grid::SetInnerGridColor( float r, float g, float b) {
	_igR=r;
	_igG=g;
	_igB=b;
}

void Grid::SetFillColor( float r, float g, float b) {
	_fiR=r;
	_fiG=g;
	_fiB=b;
}

void Grid::SetQDeltaSelection(float d) {
	_fQDistSel = d;
}

float Grid::width() const {
   return ((2*_wBorder) + (_nCol-1)*(_wCell + _wInnerGrid)/(1+_nQ) +_wCell);
}

float Grid::height() const {
	return ((2*_wBorder) + (_nLine-1)*(_hCell + _wInnerGrid) +_hCell);
}

int Grid::GetCellCount() const {
	if (_type == GRID_DEFAULT)
		return _nCol*_nLine;

	int ncell = 0;
	for (int i=0; i<_nCol; i++) {
		ncell += GetQLineSize(i);
	}
	return ncell;
}

Coord Grid::GetCellCoord(int cellNum) const {
	Coord pos;

	if (_type == GRID_DEFAULT) {
		pos.y = cellNum % _nCol;
		pos.x = cellNum - (pos.y * _nCol);
	} else {
		pos.x = cellNum;
		pos.y = 0;
		while (pos.x >= GetQColSize(pos.y)) {
			pos.x -= GetQColSize(pos.y);
			pos.y++;
		}
		if (_nQH && pos.y == _nLine-1)
			pos.x = pos.x*2 + _nQHOdd;

	}
	return pos;

}

void Grid::SetLineOdd(bool x) {
	_bLineOdd=x;
	if (_nQH)
		_bLineOdd = false;
}

void Grid::SetColOdd(bool x) {
	_bColOdd=x;
	if (_nQV)
		_bColOdd = false;
}

void Grid::SetCellWidth(float x) {
	_wCell = x;
}

void Grid::SetCellHeight(float y) {
	_hCell = y;
}
void Grid::SetFont(EFont *font) {
   _lpFont = font;
   _nFontSize = _lpFont->getHeightPix();
}

bool Grid::IsOver(float x, float y) const {
	if (x<_xPos || 
		y < _yPos || 
		x > _xPos + width() ||
		y > _yPos + height())
		return false;
	return (GetSelection(x,y).x>=0);
}
bool Grid::IsMouseOver(void) const {
	return IsOver(Mouse);
}

bool Grid::CheckQCell(int col, int line) const {
	return line  < GetQLineSize(col) && col < GetQColSize(line) && line >=0 && col >=0;
}
int  Grid::GetQColSize(int line) const {
	if (_type == GRID_DEFAULT || _nQ) {
		return _nCol;
	} else if (_nQV) {
		return (_nCol - ((line + _nQVOdd) %2));
	} else {
		if (line == _nLine -1)
			return (_nCol+1)/2 - _nQHOdd;
		return _nCol;
	}
}
int Grid::GetQLineSize(int col) const {
	if (_type == GRID_DEFAULT) {
		return _nLine;
	} else if (_nQ || _nQH) {
		return (_nLine - ((col + _nQHOdd) %2));
	} else {
		if (col == _nCol-1) {
			return (_nLine+1)/2 - _nQVOdd;
		}
		return _nLine;
	}
}
/* ___________________________________ */

EMiniJeuPart::EMiniJeuPart (KGraphic *lpGraphic, int nNumAngle,  int nStartInd, float fBaseAngle, float fAngleStep, float fAlpha, float w, float h) {
		KAssert(lpGraphic!=NULL && nNumAngle >= 0);
		KAssert((w>0 && h>0) || (lpGraphic->getWidth()>0 && lpGraphic->getHeight()>0));
		KAssert(nNumAngle>0);

		_bSkipDraw = false;

		_bLocalAlloc=false;

		_lpAnimMoveX = new KCounter;
		_lpAnimMoveY = new KCounter;
		_lpAnimFad = new KCounter;
		_lpAnimRotate= new KCounter;

		_bAnimMove = false;
		_bAnimFad = false;
		_bAnimRotate = false;
		

		_lpImg=lpGraphic;
		_x=0; _y=0;
		if (w > 0 && h > 0) {
			_w = w;
			_h = h;
		} else {
			_w=_lpImg->getWidth();
			_h=_lpImg->getHeight();
		}
		_preGrabX=-1; _preGrabY=-1;

		_fRotationDuration=0;

		_bDragable=true;
		_bGrabed=false;


		_bRotation=true;
		_nSide = nNumAngle;
		_nCurrentStep = 0;
		_nRotationStepRemaining =0;
		
		_fBaseAngle = fBaseAngle;
		if (fAngleStep ==0)
			_fAngleStep = 360 / (float)_nSide;
		else
			_fAngleStep = fAngleStep;

		_fMaxBlend = fAlpha;
		_fBlend = _fMaxBlend;

		_fScale = 1.0;
		_dElapsedTime = -1;

		SetOrientation(nStartInd);
		SetBaseAngle(fBaseAngle);
}

EMiniJeuPart::EMiniJeuPart (const char *ImgPath, int nNumAngle,  int nStartInd, float fBaseAngle, float fAngleStep, float fAlpha, float w, float h) {
		KAssert(ImgPath!=NULL && strlen(ImgPath)>0);
	
		KGraphic *tmpImg=NULL;
		tmpImg = EImageBank::getImage(ImgPath);
		
		EMiniJeuPart *tmpPart=new EMiniJeuPart(tmpImg,nNumAngle,nStartInd,fBaseAngle,fAngleStep,fAlpha,w,h);
		*this=tmpPart;
		_bLocalAlloc=true;

		XDELETE(tmpPart);
}

void EMiniJeuPart::operator= (const EMiniJeuPart  a)  {
	_bSkipDraw=a._bSkipDraw;

	_fRotationDuration=a._fRotationDuration;

	_bDragable=a._bDragable;
	_bRotation=a._bRotation;

	_lpImg=a._lpImg;

	_x=a._x; _y=a._y;
	_w=a._w; _h=a._h;
	_preGrabX=a._preGrabX; _preGrabY=a._preGrabY;

	_nSide=a._nSide;
	_nCurrentStep=a._nCurrentStep;
	_nRotationStepRemaining=a._nRotationStepRemaining;
	_fBaseAngle=a._fBaseAngle;
	_fAngleStep=a._fAngleStep;
	_fBlend=a._fBlend;
	_fMaxBlend=a._fMaxBlend;
	_fScale=a._fScale;

	_lpAnimMoveX = new KCounter;
	_lpAnimMoveY = new KCounter;
	_lpAnimFad = new KCounter;
	_lpAnimRotate= new KCounter;

	_bAnimMove = false;
	_bAnimFad = false;
	_bAnimRotate = false;
	_bLocalAlloc=false;
	_bGrabed=false;

	_dElapsedTime = -1;
}

EMiniJeuPart::EMiniJeuPart(const EMiniJeuPart* a) {
	_bAnimMove = false;
	_bAnimFad = false;
	_bAnimRotate = false;

	_bSkipDraw=a->_bSkipDraw;
	_fRotationDuration=a->_fRotationDuration;

	_bDragable=a->_bDragable;
	_bRotation=a->_bRotation;
	_bGrabed=false;
	
	_lpImg=a->_lpImg;
	_bLocalAlloc=false;

	_x=a->_x; _y=a->_y;
	_w=a->_w; _h=a->_h;
	_preGrabX=a->_preGrabX; _preGrabY=a->_preGrabY;

	_nSide=a->_nSide;
	_nCurrentStep=a->_nCurrentStep;
	_nRotationStepRemaining=a->_nRotationStepRemaining;
	_fBaseAngle=a->_fBaseAngle;
	_fAngleStep=a->_fAngleStep;
	_fBlend=a->_fBlend;
	_fMaxBlend=a->_fMaxBlend;
	_fScale=a->_fScale;

	_lpAnimMoveX = new KCounter;
	_lpAnimMoveY = new KCounter;
	_lpAnimFad = new KCounter;
	_lpAnimRotate= new KCounter;

	_dElapsedTime = -1;
}
EMiniJeuPart::~EMiniJeuPart(){
	XDELETE(_lpAnimMoveX);
	XDELETE(_lpAnimMoveY);
	XDELETE(_lpAnimFad);
	XDELETE(_lpAnimRotate);
}
void EMiniJeuPart::Move(float x, float y,float fDuration, bool force, KCounterFunction counterType) {
	if (_bAnimMove && !force)
		return;

	XDELETE(_lpAnimMoveX);
	XDELETE(_lpAnimMoveY);
	_lpAnimMoveX = new KCounter;
	_lpAnimMoveY = new KCounter;
	_lpAnimMoveX->startCounter(_x,x,0,fDuration,counterType);
	_lpAnimMoveY->startCounter(_y,y,0,fDuration,counterType);
	_bAnimMove = true;
	_posDest = Coord(x,y);
}
void EMiniJeuPart::StopMove() {
	if (!_bAnimMove)
		return;

	_lpAnimMoveX->move(std::numeric_limits<double>::max());
	_lpAnimMoveY->move(std::numeric_limits<double>::max());
	_bAnimMove = false;
}
void EMiniJeuPart::FinishMove() {
	if (!_bAnimMove)
		return;
	
	StopMove();
	_x = _lpAnimMoveX->getCurrentValue();
	_y = _lpAnimMoveY->getCurrentValue();
}
bool EMiniJeuPart::operator== (EMiniJeuPart const& a) const {
	return (_lpImg==a._lpImg && (_nCurrentStep+_nSide)%_nSide==(a._nCurrentStep+a._nSide)%a._nSide);
}

void EMiniJeuPart::Logic() {
	// Gestion deplacement de la pièce
	if (IsGrab()) {
		Move(Mouse.x - Width()/2, Mouse.y - Height()/2);
	}
}

Coord EMiniJeuPart::GetPosOffset() {
	return Coord(0,0);
}
Coord EMiniJeuPart::GetPos() {
	return GetPosOffset() + Coord(_x,_y);
}
void EMiniJeuPart::Check() {
	if (IsMouseOver() && Mouse.Event()) {
		//Right Click
		if (_bRotation && Mouse.RightEvent()) { 
			if (RotationComplete())
				RotateCW();
			Mouse.RightEventClose(); 
		} 
		//Left Click
		if (Mouse.LeftEvent()) {
			if (IsGrab()) {
				Release();
			} else {
				Grab();
			}
			Mouse.LeftEventClose(); 
		}
	}
}

void EMiniJeuPart::Draw() {
	double dElapsedTime = 0;
	
	if (_dElapsedTime < 0) {
		// Recuperation automatique du temps écoulé
		dElapsedTime = MyGame::getGame()->getElapsedTime();
	} else {
		// temps écoulé fourni
		dElapsedTime = _dElapsedTime;
		_dElapsedTime = 0;
	}
	float rAngle = 0;

	if (_bAnimRotate) {
		_lpAnimRotate->move(dElapsedTime);
		 rAngle = _lpAnimRotate->getCurrentValue();
		 if (_lpAnimRotate->isCompleted())
			 _bAnimRotate = false;
	}
	if (_bAnimFad) {
		_lpAnimFad->move(dElapsedTime);
		_fBlend = _lpAnimFad->getCurrentValue();
		if (_lpAnimFad->isCompleted())
			 _bAnimFad = false;
	}
	if (_bAnimMove) {
		_lpAnimMoveX->move(dElapsedTime);
		_lpAnimMoveY->move(dElapsedTime);
		_x = _lpAnimMoveX->getCurrentValue();
		_y = _lpAnimMoveY->getCurrentValue();
		if (_lpAnimMoveX->isCompleted()) {
			_bAnimMove = false;
		}
	}
	Coord posOffset = GetPosOffset();
	if (!_bSkipDraw)
		_lpImg->blitAlphaRectFxF(0, 0, _w, _h, _x + posOffset.x, _y + posOffset.y, GetAngle()+rAngle, _fScale, _fBlend );
	
	// Reprise de la rotation 
	if(_nRotationStepRemaining !=0 && ! _bAnimRotate) {
		if (_nRotationStepRemaining > 0)
			EMiniJeuPart::RotateCW(_nRotationStepRemaining);
		if (_nRotationStepRemaining <0) 
			EMiniJeuPart::RotateACW(-_nRotationStepRemaining);
	}
}
void EMiniJeuPart::SetOrientation(int nIndOrientation) {
	if ( _nSide == 0 )
		return;
	_nCurrentStep=(nIndOrientation+_nSide)%_nSide;
}

void EMiniJeuPart::RotateCW(bool bAnim) {
	if (_nSide) {
		float oldAngle = GetAngle();
		_nCurrentStep--;
		
		if (!_bAnimRotate && bAnim) {
			_lpAnimRotate->startCounter(oldAngle-GetAngle(),0,0,_fRotationDuration,K_COUNTER_LINEAR);
			_bAnimRotate = true;
		}

		_nCurrentStep = _nCurrentStep % _nSide;
	}
}

void EMiniJeuPart::RotateACW(bool bAnim) {
	if (_nSide) {
		float oldAngle = GetAngle();
		_nCurrentStep++;
		_nCurrentStep = _nCurrentStep % _nSide;
		if (!_bAnimRotate && bAnim) {
			if (oldAngle < GetAngle()) {
				_lpAnimRotate->startCounter(oldAngle-GetAngle(),0,0,_fRotationDuration,K_COUNTER_LINEAR);
			} else {
				_lpAnimRotate->startCounter(oldAngle,oldAngle + _fAngleStep,0,_fRotationDuration,K_COUNTER_LINEAR);
			} 
			_bAnimRotate = true;
		}
	}
}

void EMiniJeuPart::RotateCW(int nStep) {
	KAssert(nStep>=0);
	if (_bAnimRotate)
		return;
	_nRotationStepRemaining = nStep -1;
	RotateCW(true);
}

void EMiniJeuPart::RotateACW(int nStep) {
	KAssert(nStep>=0);
	if (_bAnimRotate)
		return;
	_nRotationStepRemaining =-(nStep -1);
	RotateACW(true);
}
void EMiniJeuPart::RotateToStep(int nStep) {
	KAssert(nStep < _nSide);
	int numStep = _nCurrentStep - nStep;
	if (numStep > 0)
		RotateACW(numStep);
	else if (numStep < 0)
		RotateCW(-numStep);
};
bool EMiniJeuPart::IsOver(struct Coord c) const {
	if (_fBlend == 0)
		return false;
	float w=_w*_fScale,h=_h*_fScale;
	Coord tmp = Coord(_x + (_w-w)/2, _y + (_h-h)/2);
	return (c.x>=tmp.x && c.x<tmp.x+w && c.y>= tmp.y&& c.y<tmp.y+h);
}

bool EMiniJeuPart::IsMouseOver() const {
	return IsOver(Mouse);
}
bool EMiniJeuPart::IsVisible() const {
	return _fBlend > 0;
}
void EMiniJeuPart::Show() {
	if (_bAnimFad) {
		XDELETE(_lpAnimFad);
		_bAnimFad = false;
		_lpAnimFad = new KCounter();
	}
	_fBlend = _fMaxBlend;
}
void EMiniJeuPart::Hide() {
	if (_bAnimFad) {
		XDELETE(_lpAnimFad);
		_bAnimFad = false;
		_lpAnimFad = new KCounter();
	}
	_fBlend = 0;
}

bool EMiniJeuPart::FadIn(double fDuration, bool force) {
	double duration = fDuration;
	if (_bAnimFad) {
		if (!force)
			return false;
		StopFad();
		duration = duration * (_fMaxBlend - _fBlend)/_fMaxBlend;
	}

	_lpAnimFad->startCounter(_fBlend,_fMaxBlend,0,fDuration, K_COUNTER_LINEAR);
	_bAnimFad = true;
   return _bAnimFad;
}
bool EMiniJeuPart::FadOut(double fDuration, bool force) {
	double duration = fDuration;
	if (_bAnimFad) {
		if (!force)
			return false;
		StopFad();
		duration = duration * _fBlend/_fMaxBlend;
	}
	
	_lpAnimFad->startCounter(_fBlend,0,0,duration, K_COUNTER_LINEAR);
	_bAnimFad = true;
   return _bAnimFad;
}
void EMiniJeuPart::StopFad() {
	if (!_bAnimFad)
		return;
	
	_bAnimFad = false;
	_fBlend = _lpAnimFad->getCurrentValue();
	_lpAnimFad->startCounter(_fBlend,_fBlend,0,0,K_COUNTER_LINEAR);
	_lpAnimFad->move(1);
}
bool EMiniJeuPart::TransformComplete() const {
	return RotationComplete()&& FadComplete() && MoveComplete();
}

float EMiniJeuPart::GetAngle() const {
	return _fBaseAngle+(_fAngleStep*_nCurrentStep);
}

Coord EMiniJeuPart::Center() const {
	if (IsGrab())
		return Coord(Mouse.x,Mouse.y);
	return Coord(_x + _w/2, _y +_h/2);
}

/* ___________________________________ */

Digit::Digit(EFont *lpFont, float scale, float digitsize) {
	_lpFont = new EFont(*lpFont);

	_lpImgPlus = EImageBank::getImage("mg_plant_addition_plus","png");
	_lpImgMoins = EImageBank::getImage("mg_plant_addition_minus","png");

	_nMargin = 5;
	_fScale = scale;

	_nVal = 0;
	_nCharMaxWidth = 0;
	_nCurrentDigitWidth = 0;

	_nDigitSize = _lpFont->getHeightPix();
	if (digitsize > 0)
		_nDigitSize = digitsize;
	
	Move(0,0);
	Reset();
}
Digit::~Digit(void) {
	XDELETE(_lpFont);
}

void Digit::Check() {
	if (!Mouse.LeftEvent())
		return;

	// Click sur le chiffre
	if (Mouse.x >= _posNumber.x && 
		Mouse.x < _posNumber.x+_nCharMaxWidth && 
		Mouse.y >= _posNumber.y+(_lpFont->getHeightPix()-_nDigitSize) && 
		Mouse.y <= _posNumber.y + _lpFont->getHeightPix()) {
		Incr();
		ESoundBank::getSound("mg_digit")->playSample();
		Mouse.LeftEventClose();
	}

	int x = _posNumber.x + (_nCharMaxWidth - _nImgWidth)/2;
	// Click sur le plus
	if (Mouse.x >= x && 
		Mouse.x < x + _nImgWidth && 
		Mouse.y >= _posNumber.y+(_lpFont->getHeightPix()-_nDigitSize) - _nMargin - _nImgHeight && 
		Mouse.y <= _posNumber.y+(_lpFont->getHeightPix()-_nDigitSize) - _nMargin) {
		Incr();
		Mouse.LeftEventClose();
		ESoundBank::getSound("mg_digit")->playSample();
	}

	// Click sur le moins
	if (Mouse.x >= x && 
		Mouse.x < x + _nImgWidth && 
		Mouse.y >= _posNumber.y + _lpFont->getHeightPix() + _nMargin && 
		Mouse.y <= _posNumber.y + _lpFont->getHeightPix() + _nMargin + _nImgHeight ) {
		Decr();
		Mouse.LeftEventClose();
		ESoundBank::getSound("mg_digit")->playSample();
	}
}
int Digit::GetDigitWidth(int digit) {
	return _lpFont->getCharWidth(48+digit)-_lpFont->getCharXOffset(48+digit);
}
void Digit::Reset() {
	_nCharMaxWidth = 0;
	for (int i=0; i < 10; i++) {
		int width = GetDigitWidth(i);
		if (width > _nCharMaxWidth) 
			_nCharMaxWidth = width;
	}
	
	//if (_lpImgPlus->getWidth() > _nCharMaxWidth)
	//	_fScale = _nCharMaxWidth / _lpImgPlus->getWidth();

	_nImgHeight = _lpImgPlus->getHeight() * _fScale;
	_nImgWidth = _lpImgPlus->getWidth() * _fScale;
	SetVal(_nVal);

	Move(_posNumber);
}

void Digit::SetVal(int x) {
	KAssert(_nVal >= 0 && _nVal < 10);
	_nVal = x;
	_nCurrentDigitWidth = GetDigitWidth(_nVal);
}

void Digit::SetMargin(int x) {
	_nMargin = x;
}

void Digit::SetImgScale(float x) {
	_fScale = x;
	Reset();
}

int Digit::GetVal() {
	return _nVal;
}

void Digit::Move(int x, int y) {
	Coord tmp = Coord(
		(_lpImgPlus->getWidth() - _nImgWidth)/2,
		(_lpImgPlus->getHeight() - _nImgHeight)/2);
	
	_posNumber = Coord(x,y);
	_posPlus = Coord((float)(x + (_nCharMaxWidth - _nImgWidth)/2) - tmp.x, (float)(y +(_lpFont->getHeightPix()-_nDigitSize) - _nImgHeight - _nMargin - tmp.y));
	_posMoins = Coord((float)(x + (_nCharMaxWidth - _nImgWidth)/2) - tmp.x, (float)(y + _lpFont->getHeightPix() + _nMargin - tmp.y));
}

void Digit::Move(Coord pos) {
	Move(pos.x,pos.y);
}

void Digit::MovePlus(int x, int y) {
	if ((_nCharMaxWidth - _nImgWidth) >= 0) {
		Move(x,y +(_lpFont->getHeightPix()-_nDigitSize)+_nImgHeight+_nMargin);
	} else {
		Move(x+(_nCharMaxWidth - _nImgWidth)/2,y +(_lpFont->getHeightPix()-_nDigitSize)+_nImgHeight+_nMargin);
	}
}
void Digit::MovePlus(Coord pos) {
	MovePlus(pos.x,pos.y);
}

void Digit::Draw() {
	_lpImgPlus->blitAlphaRectFx(0,0,_lpImgPlus->getWidth(),_lpImgPlus->getHeight(),(short)_posPlus.x,(short)_posPlus.y,0,_fScale,1);
	_lpFont->drawStringFromLeftF(itos(_nVal).c_str(),_posNumber.x + (_nCharMaxWidth-_nCurrentDigitWidth-1)/2,(long)_posNumber.y);
	_lpImgMoins->blitAlphaRectFx(0,0,_lpImgMoins->getWidth(),_lpImgMoins->getHeight(),(short)_posMoins.x,(short)_posMoins.y,0,_fScale,1);
#ifdef DEBUGSTUDIO
	_lpImgMoins->drawLine(_posNumber.x,_posNumber.y +(_lpFont->getHeightPix()-_nDigitSize),_posNumber.x+_nCharMaxWidth,_posNumber.y +(_lpFont->getHeightPix()-_nDigitSize),1,1,0,1);
	_lpImgMoins->drawLine(_posNumber.x+_nCharMaxWidth,_posNumber.y+(_lpFont->getHeightPix()-_nDigitSize),_posNumber.x+_nCharMaxWidth,_posNumber.y+(_lpFont->getHeightPix()-_nDigitSize)+_nDigitSize,1,1,0,1);
	_lpImgMoins->drawLine(_posNumber.x+_nCharMaxWidth,_posNumber.y+(_lpFont->getHeightPix()-_nDigitSize)+_nDigitSize,_posNumber.x,_posNumber.y+(_lpFont->getHeightPix()-_nDigitSize)+_nDigitSize,1,1,0,1);
	_lpImgMoins->drawLine(_posNumber.x,_posNumber.y+(_lpFont->getHeightPix()-_nDigitSize),_posNumber.x,_posNumber.y+(_lpFont->getHeightPix()-_nDigitSize)+_nDigitSize,1,1,0,1);
#endif
}

float Digit::width() {
	if (_nCharMaxWidth >= _lpImgPlus->getWidth() * _fScale)
		return (float)_nCharMaxWidth;
	else
		return _lpImgPlus->getWidth() * _fScale;
}

float Digit::height() {
	return (float)(2 * (_nMargin + _nImgHeight*_fScale) + _lpFont->getHeightPix());
}

float Digit::X() {
	float deltaX = 0;

	if (_lpImgPlus->getWidth() * _fScale > _nCharMaxWidth) {
		deltaX = (_lpImgPlus->getWidth() * _fScale - _nCharMaxWidth)/2;
	}
	return _posNumber.x - deltaX;
}
float Digit::Y() {
	return ((_lpImgPlus->getHeight() * _fScale) - _lpImgPlus->getHeight())/2 + _posPlus.y;
}
float Digit::x() {
   return _posNumber.x;
}
float Digit::y() {
	return _posNumber.y;
}
float Digit::FontHeight() {
	return _nDigitSize;
}
void Digit::Incr() {
	SetVal((_nVal + 1) %10);
}
void Digit::Decr() {
	SetVal((_nVal + 9) %10);
}

KImage *GetKImage(const char* szImageName, const char *szExt) {
   char buff[K_MAXPATH];
   char szImageNameExt[1024];
	memset(szImageNameExt, 0, 1024);
	strcpy(szImageNameExt, "pictures/");
	strcat(szImageNameExt, szImageName);
	// Si l'extension n'est pas dans le nom
	if (strstr(szImageNameExt, ".")==NULL) {
		strcat(szImageNameExt, ".");
		strcat(szImageNameExt, szExt);
	}

	// Recuperation d'un buffer pointant sur le fichier image compréssé
	unsigned char *lpCmpData = NULL;
	size_t nCmpDataSize = 0;
   EGLL();
	KResource::loadResource (KMiscTools::makeFilePath(szImageNameExt), lpCmpData, nCmpDataSize);
   
	// Décodage de du buffer
	KImage *lpBufImg = new KImage();
	lpBufImg->decodeImage(KImage::detectFormat(SafeMakeFilePath(szImageNameExt,buff)), lpCmpData, nCmpDataSize);
   EGLR();

	free(lpCmpData);
	return lpBufImg;
}

std::vector< bool >* GenerateAlphaMask(const char* szImageName, const char *szExt) {
	KImage *lpIMask = GetKImage(szImageName, szExt);

	// Recupere la référence du buffer image décompréssé
	unsigned long* tmpPixels;
	tmpPixels = lpIMask->getPixels();

	// Initialisation du mask
	std::vector< bool > *_lpArrayMask = new std::vector< bool >(lpIMask->getImageWidth() * lpIMask->getImageHeight(), false);
	int i=0;
	while (i < _lpArrayMask->size()) {
		if(tmpPixels[i] & 0xff000000)
			(*_lpArrayMask)[i] = true;
		i++;
	}

	// Liberation des buffer temporaire
	XDELETE(lpIMask);

	return _lpArrayMask;
}


/* ___________________________________ */


Polygone::Polygone() {
   target = NULL;
}
Polygone::~Polygone() {
   XDELETE(target);
}

void Polygone::Draw() {
   if (_lpListPoint.size() < 3)
      return;
   if (target == NULL)
      target = KPTK::createKGraphic();

   std::list< Coord >::iterator first = _lpListPoint.begin(), next;
   next=first;

   float r=1,g=0;
   if (IsMouseOver()) {
      r=0;
      g=1;
   }
   for (;first != _lpListPoint.end();first++) {
      next++;
      if (next == _lpListPoint.end())
         next = _lpListPoint.begin();
      target->drawLine((*first).x,(*first).y,(*next).x,(*next).y,r,g,0,1);
   }
}
void Polygone::AddPoint(float x, float y) {
   _lpListPoint.push_back(Coord(x,y));
}
void Polygone::ClearPoints() {
   _lpListPoint.clear();
}

bool Polygone::IsOver(float x, float y) {
   if (_lpListPoint.size() < 3)
      return false;

   Coord endsegtest = EGeometry::GetSegmentEnd(Coord(x,y),1024,90);

   std::list< Coord >::iterator first = _lpListPoint.begin(), next;
   next=first;

   int nIntersect = 0;
   for (;first != _lpListPoint.end();first++) {
      next++;
      if (next == _lpListPoint.end())
         next = _lpListPoint.begin();
      if (EGeometry::IsSegmentIntersect((*first),(*next),Coord(x,y),endsegtest,true))
         nIntersect++;
   }
   if (nIntersect%2 != 0)
      return true;
   return false;
}
bool Polygone::IsOver(Coord pos) {return IsOver(pos.x,pos.y);}
bool Polygone::IsMouseOver() {return IsOver(Mouse.x,Mouse.y);}