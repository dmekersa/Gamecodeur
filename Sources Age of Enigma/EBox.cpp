#include "EBox.h"

#include "MyGame.h"
#include "EPointer.h"
#include "ImageBank.h"
#include "EGeometry.h"

namespace {
	KGraphic *bg = NULL;
   int refCount = 0;
}
EBox::EBox(float x, float y, float w,float h, float marginL, float marginR, float marginU, float marginD) {
	_nIterIndice = 0;
	_minInnerWidth = 0;
	_minInnerHeight = 0;
	_lpParent = NULL;
	_fMaxBlend = 1;
	_fBlend = 1;
   _x = 0;
   _y = 0;

   _nNumMaximizeChild = 0;

	SetVerticalPolicy(VCENTER);
	SetHorisontalPolicy(HCENTER);
	SetSizePolicy(INHERIT);

	SetCentered(false);

	SetGeometry(x,y,w,h);
	SetMargin(marginL,marginR,marginU,marginD);

	_layoutType = VERTICAL;

	if (bg == NULL)
		bg = KPTK::createKGraphic();
   refCount++;
}
EBox::~EBox() {
	if (_lpParent != NULL)
		_lpParent->RemoveChild(this);

	EBox *tmp;
	while (_vChilds.size()>0) {
		tmp =  _vChilds[0];
		XDELETE(tmp);
	}
   refCount--;
   if (refCount == 0)
      XDELETE(bg);
}

void EBox::Draw() {
#ifdef DEBUG
	if (Mouse.MiddleState() && IsOver()) {
		Coord p1,p2;

		p1 = GetPos();
		p2 = p1 + GetSize();
		bg->drawRect(p1.x,p1.y,p2.x,p2.y,1,1,1,0.2);

		p1 = p1 + GetLeftTopMargin();
		p2 = p2 - GetRightBottomMargin();	
		bg->drawRect(p1.x,p1.y,p2.x,p2.y,1,1,1,0.2);
	}
#endif

	for (int i=0; i<_vChilds.size(); i++) {
		_vChilds[i]->Draw();
	}
}
void EBox::SetCentered(bool centered) {
	_bCentered = centered;
}
void EBox::SetBoxCenter(float x, float y) {
	Move(x - _w/2,y - _h/2);
}
void EBox::SetGeometry(float x, float y, float w, float h) {
	Move(x,y);
	SetSize(w,h);
}
void EBox::Move(float x, float y) {
	float deltax=x-_x, deltay=y-_y;
	MoveRel(deltax,deltay);
}
void EBox::MoveRel(float x, float y) {
	_x += x;
	_y += y;
	_center = GetCenter();
	
	for (int i=0; i<_vChilds.size(); i++) {
		_vChilds[i]->MoveRel(x,y);
	}
}
void EBox::SetSize(float width, float height) {
	_wHint = width;
	_hHint = height;
	
	_w = GetMinWidth();
	_h = GetMinHeight();

	if (IsCentered())
		SetBoxCenter(_center);
	UpdateChild();
}
void EBox::SetWidth(float width) {
	_wHint = width;
	_w = GetMinWidth();
	_h = GetMinHeight();

	if (IsCentered())
		SetBoxCenter(_center);
	UpdateChild();
}
void EBox::SetHeight(float height) {
	_hHint = height;
	_h = GetMinHeight();
	_w = GetMinWidth();

	if (IsCentered())
		SetBoxCenter(_center);
	UpdateChild();
}

Coord EBox::GetPos() {
	return Coord(_x,_y);
}
Coord EBox::GetSize() {
	return Coord(GetWidth(),GetHeight());
}
float EBox::GetWidth() {
	return _w;
}
float EBox::GetHeight() {
	return _h;
}

void EBox::SetMargin(float left, float right, float up, float down) {
	_marginLeft = left;
	_marginRight = right;
	_marginUp = up;
	_marginDown = down;

	SetMinWidth();
	SetMinHeight();
	UpdateGeometry();
	UpdateChild();
}
Coord EBox::GetLeftRightMargin() {
	return Coord(_marginLeft,_marginRight);
}
Coord EBox::GetTopBottomMargin() {
	return Coord(_marginUp,_marginDown);
}
Coord EBox::GetLeftTopMargin() {
	return Coord(_marginLeft,_marginUp);
}
Coord EBox::GetRightBottomMargin() {
	return Coord(_marginRight,_marginDown);
}
float EBox::GetHMargin() {
	return _marginLeft+_marginRight;
}
float EBox::GetVMargin() {
	return _marginUp+_marginDown;
}

void EBox::SetLayoutType(LayoutType layoutType) {
   if (_layoutType == layoutType)
      return;
   
	_layoutType = layoutType;
	UpdateUI();
}
void EBox::SetHorisontalPolicy(HorisontalPolicy policy) {
	if (policy == _hPolicy)
		return;

	_hPolicy = policy;

	UpdateChild();
}
void EBox::SetVerticalPolicy(VerticalPolicy policy) {
	if (policy == _vPolicy)
		return;

	_vPolicy = policy;
	UpdateChild();
}
void EBox::SetSizePolicy(SizePolicy policy) {
	if (policy == _sizePolicy)
		return;

   if (_lpParent != NULL) {
      if (policy == MAXIMIZE) {
         _lpParent->_nNumMaximizeChild++;
      } else if (_sizePolicy == MAXIMIZE) {
         _lpParent->_nNumMaximizeChild--;
      }
   }

	_sizePolicy = policy;
}
void EBox::InheritSize() {
	if (_lpParent->_layoutType == VERTICAL) {
		SetWidth(_lpParent->GetInnerWidth());
	} else {
		SetHeight(_lpParent->GetInnerHeight());
	}
}
void EBox::AddChild(EBox *child) {
	if (IsChild(child) || child == NULL || child == this)
		return;

	_vChilds.push_back(child);
	child->SetParent(this);

   if (child->_sizePolicy == MAXIMIZE)
      _nNumMaximizeChild++;

	UpdateGeometry();
	UpdateChild();
}
void EBox::RemoveChild(EBox *child) {
	if (!IsChild(child) || child == NULL || child == this)
		return;

   if (child->_sizePolicy == MAXIMIZE)
      _nNumMaximizeChild--;

	EBox *tmp;
	for (int i=0; i < _vChilds.size(); i++) {
		if (_vChilds[i] == child) {
			tmp = _vChilds[i];
			for (int j=i+1; j < _vChilds.size(); j++) {
				_vChilds[j-1] = _vChilds[j];
			}
			_vChilds.pop_back();
			if (tmp->GetParent() == this)
				tmp->SetParent(NULL);
			break;
		}
	}

	UpdateGeometry();
	UpdateChild();
};
void EBox::SetParent(EBox *parent) {
	EBox *tmp;

	if (parent == this)
		return;

	if (_lpParent != NULL) {
		// Séparation du précédent père
		if (_lpParent != parent) {
			tmp = _lpParent;
			_lpParent = NULL;
			tmp->RemoveChild(this);
		} else {
			_lpParent->AddChild(this);
			return;	
		}
	}
	if (parent == NULL)
		return;

	_lpParent = parent;
	if (_sizePolicy == INHERIT)
		InheritSize();
	_lpParent->AddChild(this);	
}
bool EBox::IsChild(EBox *box) {
	for (int i=0; i<_vChilds.size(); i++) {
		if (box == _vChilds[i])
			return true;
	}
	return false;
}
int EBox::GetNumbChilds() {
	return _vChilds.size();
}
EBox* EBox::GetChild(int nChild) {
	if (nChild >= _vChilds.size() || nChild < 0)
		return NULL;
	return _vChilds[nChild];
}
void EBox::IterInitialize() {
	_nIterIndice = 0;
}
EBox* EBox::IterNextChild() {
	EBox *tmp = GetChild(_nIterIndice);
	if ( tmp != NULL)
		_nIterIndice++;
	return tmp;	
}

bool EBox::IsOver() {
	return Mouse >= GetPos() && Mouse <= GetPos()+GetSize();
}
bool EBox::IsOver(Coord pos) {
	return pos >= GetPos() && pos <= GetPos()+GetSize();	
}
void EBox::UpdateUI() {
	UpdateGeometry();
	UpdateChild();
}
void EBox::UpdateGeometry() {
	_w = GetMinWidth();
	_h = GetMinHeight();
}
void EBox::UpdateChild() {
	// Repositionnement des box filles
	Coord innersize = Coord(GetInnerWidth(),GetInnerHeight());
	Coord startpos = GetPos() + GetLeftTopMargin();
	Coord pos = startpos;
	
	// preparation pour la mise en forme gestion LAYOUT + LMAMYOUTDIRPOLICY
   if (_nNumMaximizeChild == 0) {
      if (_layoutType == VERTICAL) {
         if (_vPolicy == VCENTER) {
            pos.y += (innersize.y - _fChildMinHeight)/2;
         } else if (_vPolicy == DOWN) {
            pos.y = pos.y +innersize.y - _fChildMinHeight;
         }
      }
      else {
         if (_hPolicy == HCENTER) {
            pos.x += (innersize.x - _fChildMinWidth)/2;
         } else if (_hPolicy == RIGHT) {
            pos.x = pos.x + innersize.x - _fChildMinWidth;
         }
      }
   }

	// mise a jour des position des enfants dans le sens du layout
	float childWidth, childHeight;
	EBox *child;
	for (int i=0; i<_vChilds.size(); i++) {
		child = _vChilds[i];
		childWidth = child->GetWidth();
		childHeight = child->GetHeight();
		if (_layoutType == VERTICAL) {
			if (_hPolicy == HCENTER) {
				pos.x = startpos.x + (innersize.x - childWidth)/2;
			} else if (_hPolicy == RIGHT) {
				pos.x = startpos.x + (innersize.x - childWidth);
			}
		} else {
			if (_vPolicy == VCENTER) {
				pos.y = startpos.y + (innersize.y - childHeight)/2;
			} else if (_vPolicy == DOWN) {
				pos.y = startpos.y + (innersize.y - childHeight);
			}
		}
		child->Move(pos);
		if (_layoutType == VERTICAL) {
			pos.y += childHeight;
		} else {
			pos.x += childWidth;
		}
	}
}
Coord EBox::GetMinSize() {
	float width=GetMinWidth(), height=GetMinHeight();
	return Coord(width,height);
}
float EBox::GetMinWidth() {
	float minWidth=0;
	EBox *child;
	for (int i=0; i<_vChilds.size(); i++) {
		child = _vChilds[i];

      if (child->_sizePolicy == MAXIMIZE)
         child->SetSize(0,child->GetHeight());

 		if (_layoutType == VERTICAL) {
			if (child->GetMinWidth() > minWidth)
				minWidth = child->GetMinWidth();
		} else {
			minWidth += child->GetMinWidth();
		}
	}
	_fChildMinWidth = minWidth;

   if (minWidth < _minInnerWidth) {
      // Gestion du filler
      if (_nNumMaximizeChild > 0) {
         for (int i=0; i<_vChilds.size(); i++) {
            child = _vChilds[i];
            
            if (child->_sizePolicy == MAXIMIZE) {
               if (_layoutType == VERTICAL) {
                  child->SetSize(_minInnerWidth,child->_h);
               } else if (_layoutType == HORIZONTAL) {
                  child->SetSize((_minInnerWidth - (minWidth - child->GetWidth()))/_nNumMaximizeChild,0);
               }
            }
         }
      }
		minWidth = _minInnerWidth;
   }

	minWidth += GetHMargin();
	if (minWidth < _wHint)
		minWidth = _wHint;
	return minWidth;
}
float EBox::GetMinHeight() {
	float minHeight=0;
	EBox *child;
	for (int i=0; i<_vChilds.size(); i++) {
		child = _vChilds[i];

      if (child->_sizePolicy == MAXIMIZE)
         child->SetSize(child->GetWidth(),0);
 
		if (_layoutType == VERTICAL) {
			minHeight += child->GetMinHeight();
		} else {
			if (child->GetMinHeight() > minHeight)
				minHeight = child->GetMinHeight();
		}
	}
	_fChildMinHeight = minHeight;
	
   if (minHeight < _minInnerHeight) {
      // Gestion du filler
      if (_nNumMaximizeChild > 0) {
         for (int i=0; i<_vChilds.size(); i++) {
            child = _vChilds[i];
            if (child->_sizePolicy == MAXIMIZE) {
               if (_layoutType == VERTICAL) {
                  child->SetSize(child->_w,(_minInnerHeight - (minHeight - child->GetHeight()))/_nNumMaximizeChild);                  
               } else if (_layoutType == HORIZONTAL) {
                  child->SetSize(0,_minInnerHeight);
               }
            }
         }
      }
      minHeight = _minInnerHeight;
   }
	minHeight += GetVMargin();

	if (minHeight < _hHint)
		minHeight = _hHint;
	return minHeight;
}
void EBox::SetMinWidth() {
	_minInnerWidth = ComputeMinInnerWidth();
}
void EBox::SetMinHeight() {
	_minInnerHeight = ComputeMinInnerHeight();
}


void EBox::SetBlend(float ratio) {
	if (ratio < 0) {
		_fBlend = 0;
		if (_lpParent != NULL)
			_fBlend = _fMaxBlend * _lpParent->_fBlend;
	} else {
		_fBlend = _fMaxBlend * ratio;
	}

	for (int i=0; i<_vChilds.size(); i++) {
		_vChilds[i]->SetBlend();
	}
}
float EBox::GetBlend() {
	return _fBlend;
}


EImgBox::EImgBox(KGraphic *imgBox, float margin, float x, float y):
	EBox(x,y,imgBox->getWidth(),imgBox->getHeight(),margin,margin,margin,margin) {
		SetImg(imgBox);
}
void EImgBox::SetImg(KGraphic *imgBox) {
   _lpImgBox = imgBox;
   _fImgWidth = _lpImgBox->getWidth();
   _fImgHeight = _lpImgBox->getHeight();
}
EImgBox::~EImgBox() {
	_lpImgBox = NULL;
}
void EImgBox::Draw() {
	_lpImgBox->blitAlphaRectFx(0,0,_fImgWidth,_fImgHeight,_center.x - (_fImgWidth/2),_center.y - (_fImgHeight/2),0,1,_fBlend);

	EBox::Draw();
}
float EImgBox::ComputeMinInnerWidth() {
	return _fImgWidth - GetHMargin();
}
float EImgBox::ComputeMinInnerHeight() {
	return _fImgHeight - GetVMargin();
}


EImgExpendableBox::EImgExpendableBox(KGraphic *imgBox,  float wLUCorner, float hLUCorner, float wRDCorner, float hRDCorner, float margin, float x, float y):
	EBox(x,y,imgBox->getWidth(),imgBox->getHeight(),margin,margin,margin,margin) {
		_lpImgBox = imgBox;
		_fImgWidth = _lpImgBox->getWidth();
		_fImgHeight = _lpImgBox->getHeight();

		_wLUCorner = wLUCorner;
		_wRDCorner = wRDCorner;
		_wCenter   = _fImgWidth -(_wLUCorner+_wRDCorner);

		_hLUCorner = hLUCorner;
		_hRDCorner = hRDCorner;
		_hCenter   = _fImgHeight -(_hLUCorner+_hRDCorner);

		SetMargin(_wLUCorner,_wRDCorner,_hLUCorner,_hRDCorner);
}
EImgExpendableBox::~EImgExpendableBox() {
	_lpImgBox = NULL;
}
void EImgExpendableBox::Draw() {
	Coord dst(_x,_y),
		pos(0,0),
		chunk(_wLUCorner + _wCenter, _hLUCorner + _hCenter),
		toDraw(_w - _wRDCorner, _h - _hRDCorner); 

	while (toDraw.y >= 0) {
		if (toDraw.y == 0) {
			pos.y += _hCenter;
			chunk.y = _hRDCorner;
		} else if (toDraw.y < chunk.y) {
			chunk.y = toDraw.y;
		}

		while(toDraw.x >= 0) {
			// Toute la partie gauche de la boite a été déssiné
			if (toDraw.x == 0) {
				pos.x += _wCenter;
				chunk.x = _wRDCorner;
			} else if (toDraw.x < chunk.x) {// Finalistion du bourage horizontal
				chunk.x = toDraw.x;
			}

			_lpImgBox->blitAlphaRectFx(pos.x,pos.y,pos.x+chunk.x,pos.y+chunk.y,dst.x,dst.y,0,1,_fBlend); // LU Corner
			
			// Next horizontal chunk
			dst.x+=chunk.x;
			toDraw.x-=chunk.x;

			// Draw de la partie gauche fait, ajustement du chunk
			if (pos.x == 0) { 
				pos.x   += _wLUCorner;
				chunk.x -= _wLUCorner;
			}
		}

		// Next vertical chunk
		dst.y += chunk.y;
		toDraw.y -= chunk.y;
		
		if (pos.y == 0) { // Draw de la partie up de la boite fait, ajustement du chunk
			pos.y   += _hLUCorner;
			chunk.y -= _hLUCorner;
		}

		// Restart draw from left
		pos.x = 0;
		chunk.x = _wLUCorner + _wCenter;
		toDraw.x = _w - _wRDCorner;
		dst.x = _x;
	}

	EBox::Draw();
}





ETextBox::ETextBox(const char *text, EFont *font, float width, float minheight,int textStyle, int nFontHeight):
EBox(0,0,width,minheight) {
	_cText = NULL;
	_lpFont = NULL;

	_nTextStyle = textStyle;
	_fTextHeight = 0;
	_fMinTextWidth = 0;
   _nFontSize = 0;
   _nOutlineThickness = 0;

   _textColor[0] = _textColor[1] = _textColor[2] = _textColor[3] = -1;
   _textOutlineColor[0] = _textOutlineColor[1] = _textOutlineColor[2] =_textOutlineColor[3] = -1;
   
   // Contournement runtime erreur R6025 VS2008
   if (font != NULL) { 
      _lpFont = new EFont(*font);
      _nFontSize = _lpFont->getHeightPix();
   }
   
   SetText(text);
}
ETextBox::~ETextBox() {
	if (_cText != NULL)
		free(_cText);
	_cText = NULL;
   XDELETE(_lpFont);
}
void ETextBox::Draw() {
	if (_lpFont == NULL || _nFontSize<=0)
		return;
	EBox::Draw();

	if (_fTextHeight > 0 && _fTextWidth >= _fMinTextWidth) {
      _lpFont->setBlend(_fBlend);
      if (_textColor[0] >=0) {
         DrawOutlineText();
         _lpFont->setColor(_textColor[0], _textColor[1], _textColor[2], _textColor[3]); 
      }
      _lpFont->drawMultiline(_cText,_x+_marginLeft,_x+_w-_marginRight,_y+_marginUp,_nTextStyle,0,_nFontSize);
	}
}
void ETextBox::DrawOutlineText() {
   if (!_nOutlineThickness > 0)
      return;

   _lpFont->setColor (_textOutlineColor[0], _textOutlineColor[1], _textOutlineColor[2], _textOutlineColor[3]*_fBlend);
   
   float fX, fY;
   for (fY = -_nOutlineThickness; fY <= _nOutlineThickness; fY++) {
      for (fX = -_nOutlineThickness; fX <= _nOutlineThickness; fX++) {
         if (fX < 0 || fY < 0 || fX > 0 || fY > 0)
            _lpFont->drawMultiline(_cText,_x+_marginLeft + fX,_x+_w-_marginRight,_y+_marginUp + fY,_nTextStyle,0,_nFontSize);
      }
   }
}

void ETextBox::SetText(const char *text) {
	int l = strlen(text) +1;

	if (_cText != NULL) {
		free(_cText);
		_cText = NULL;
	}

	_cText = (char *) malloc(l*sizeof(char));
	KAssert(_cText != NULL);
	memset(_cText,0,l*sizeof(char));

	strcpy(_cText,text);

	_fMinTextWidth = 0;
	SetMinWidth();
	UpdateGeometry();
}
void ETextBox::SetFont(EFont *font, int nFontHeight) {
   XDELETE(_lpFont);
   if (font != NULL)
   	_lpFont = new EFont(*font);
	
	_fMinTextWidth = 0;
   _nFontSize = 0;
   
   if (_lpFont != NULL) {
      _nFontSize = _lpFont->getHeightPix();
   }

	SetMinWidth();
	UpdateGeometry();
}
void ETextBox::SetTextColor(float r, float g, float b, float a) {
   _textColor[0]= r; _textColor[1] = g; _textColor[2] = b; _textColor[3] = a; 
}
void ETextBox::SetTextOutlineThickness(int Thickness) {
   if (_textColor[0] < 0)
      _nOutlineThickness = 0;
   _nOutlineThickness = Thickness;
}
void ETextBox::SetTextOutlineColor (float r, float g, float b, float a) {
   _textOutlineColor[0] = r; _textOutlineColor[1] = g; _textOutlineColor[2] = b; _textOutlineColor[3] = a; 
}

const char* ETextBox::GetText() {
	return _cText;
}
EFont* ETextBox::GetFont() {
   if (_nFontSize<=0)
      return NULL;
	return _lpFont;
}
float ETextBox::ComputeMinInnerWidth() {
	if (_fMinTextWidth > 0)
		return _fMinTextWidth;

	_fMinTextWidth = 0;
	if (_nFontSize>0 && _cText != NULL && strcmp(_cText,"") != 0) {
		float textWidth;
		while ((textWidth = _lpFont->getMultilineWidth(_cText,0,_fMinTextWidth,0,_nTextStyle,0,_nFontSize)) > _fMinTextWidth)
			_fMinTextWidth ++;
	}
	return _fMinTextWidth;
}
float ETextBox::GetMinHeight() {
	if (_nFontSize<=0 || _cText == NULL || strcmp(_cText,"") == 0) {
		_fTextHeight = 0;
	} else {
		_fTextHeight = _lpFont->getMultilineHeight(_cText,0,GetInnerWidth(),0,_nTextStyle,0,_nFontSize);
		_fTextWidth  = _lpFont->getMultilineWidth(_cText,0,GetInnerWidth(),0,_nTextStyle,0,_nFontSize);
	}
	_minInnerHeight = _fTextHeight;
	
	return EBox::GetMinHeight();
}