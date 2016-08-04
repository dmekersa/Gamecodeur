#include "ETutoBox.h"

#include "MyGame.h"
#include "EPointer.h"
#include "ImageBank.h"
#include "FontBank.h"
#include "SoundBank.h"
#include "EGeometry.h"
#include "ESceneDirector.h"

#define ZTUTOWIDTH  1024
#define ZTUTOHEIGHT 606

ETutoBox *ETutoBox::_lpCurrentTuto = NULL;
ETutoBox* ETutoBox::GetCurrentTuto() {
	return _lpCurrentTuto;
}
ETutoBox::ETutoBox(const char *tutoText,float posXArrow, float posYArrow, float dir, float clickRayon):
	EImgExpendableBox(EImageBank::getImage("tuto_img_bg.png"),6,6,6,6) {

    SetWidth(500);

	_fFadDuration = 20;
	_bCompleted = false;
	_bWasOver = false;

	_fTutoBlend = new KCounter;
	_fTutoBlend->startCounter(0,0,0,0,K_COUNTER_LINEAR);
	_fTutoBlend->move(1);

	_lpArrow = NULL;

	_fClickRayon = clickRayon;
	if (posXArrow >=0 && posYArrow>=0) {
		_lpArrow = new ETutoArrow(posXArrow,posYArrow,dir);
		_lpArrow->SetFadeDuration(_fFadDuration);
	} else {
		_fClickRayon = 0;
	}

   EFont *tmp = EFontBank::getFont("jancient22",-19);
	_lpTutoText = new ETextBox(tutoText,tmp,350,-1,2,22);
   XDELETE(tmp);
   _lpTutoText->SetWidth(_lpTutoText->GetWidth()*2);
	_lpTutoText->SetMargin(30,30,15,15);
	_lpTutoText->SetParent(this);
	_lpTutoText->SetHeight(_lpTutoText->GetHeight());

   if (_fClickRayon > 0) {
		_lpTextClickVariation = new ETextBox(KStr("TUTOBOX_CLICK_ZONE"));
	} else {
		_lpTextClickVariation = new ETextBox(KStr("TUTOBOX_CLICK_EVERYWHERE"));
	}
   tmp =  EFontBank::getFont("tahoma.ttf",12);
	_lpTextClickVariation->SetFont(tmp);
   XDELETE(tmp);
   _lpTextClickVariation->SetParent(this);	
	_lpTextClickVariation->SetMargin(4,4,4,4);

	ComputePos();
}
ETutoBox::~ETutoBox() {
	if (_lpCurrentTuto == this)
		_lpCurrentTuto = NULL;
	XDELETE(_lpArrow);
	XDELETE(_fTutoBlend);
}

void ETutoBox::Draw() {
	if (!_fTutoBlend->isCompleted()) {
		_fTutoBlend->move(KGame::getGame()->getElapsedTime());
		UpdateAlpha();
	}
   else {
      UpdateAlpha();
   }

	if (!IsVisible()) {
		if (_lpCurrentTuto == this)
			_lpCurrentTuto = NULL;
		return;
	}

   if (_lpArrow != NULL) {
		_lpArrow->Draw();
#ifdef DEBUG
      if (_fClickRayon > 0 && Mouse.RightState()) {
         Coord pix;
         for (float i=0; i<=360.0; i+=0.5) {
            pix = EGeometry::GetSegmentEnd(_lpArrow->GetPos(),_fClickRayon,i);
            _lpImgBox->plotPixel(pix.x,pix.y,1,0,0,1);
         }
      }
#endif
   }
   	
	EImgExpendableBox::Draw();
	ToggleCursor();
}
void ETutoBox::Check() {
	if (!Mouse.LeftEvent() || 
		(_bCompleted && IsVisible()) || 
		(_fTutoBlend->getCurrentValue() != 1)) {
		Mouse.EventClose();
		return;
	} else if (!IsVisible()) {
		return;
	}

	if (_fClickRayon > 0) {
		if (EGeometry::IsInRange(_lpArrow->GetPos(),_fClickRayon,Mouse)) {
			CloseTuto();
			return;
		}
	} else {
		CloseTuto();
	}
	Mouse.EventClose();
}

void ETutoBox::Hide() {
	_fTutoBlend->startCounter(_fTutoBlend->getCurrentValue(),0,0,300,K_COUNTER_LINEAR);
}
void ETutoBox::Show() {
	if (_lpCurrentTuto != NULL)
		return;
	_lpCurrentTuto = this;
	_fTutoBlend->startCounter(_fTutoBlend->getCurrentValue(),1,0,_fFadDuration,K_COUNTER_LINEAR);
   ESoundBank::getSound("dialogshow")->playSample();
}
bool ETutoBox::IsVisible() {
	return (_fBlend > 0);
}
bool ETutoBox::IsCompleted() {
	return _bCompleted && !IsVisible();
}
void ETutoBox::ComputePos() {
	if (_lpArrow == NULL) {
      // tuto sans fleche au centre de l'ecran
		SetCentered(true);
		SetBoxCenter(ZTUTOWIDTH/2,ZTUTOHEIGHT/2);
		return;
	}

   // Calcul du centre du tuto a partir de l'angle et de la position de la fleche
	float angle = _lpArrow->GetDir() + 180;
	float minDistance = _lpArrow->GetImg()->getWidth();
	Coord posArrow = _lpArrow->GetPos();
	Coord center(GetCenter());
	Coord limitPos = EGeometry::GetSegmentEnd(posArrow,minDistance,angle);

	SetBoxCenter(limitPos);

	_w > _h? minDistance+= _h/2:minDistance+=_w/2;

	while (IsOver(limitPos)) {
		minDistance+=1;
		SetBoxCenter(EGeometry::GetSegmentEnd(posArrow,minDistance,angle));
	}

   // Ajustement de la position de la boite en fonction de la zone d'affichage.
	Coord p1(GetPos());
	Coord p2(p1+GetSize());
	if (p1.x < 0) {
		Move(0,p1.y);
   } else if (p2.x > ZTUTOWIDTH) {
      Move(ZTUTOWIDTH-_w,p1.y);
	}
	p1 = GetPos();
	p2 = p1 + GetSize();
	if (p1.y < 0) {
		Move(p1.x,0);
   } else if (p2.y > ZTUTOHEIGHT) {
      Move(p1.x,ZTUTOHEIGHT-_h);
	}

   // Reajustement de la direction de la fleche en fonction
   // de la position définitive de la boite de tuto
   // _lpArrow->SetDir(EGeometry::Angle(GetCenter(),_lpArrow->GetPos()));
}
void ETutoBox::SetArrowDir(float angle) {
	if (_lpArrow == NULL)
		return;
	_lpArrow->SetDir(angle);
	ComputePos();
}
float ETutoBox::GetArrowDir() {
	if (_lpArrow == NULL)
		return 0;
	return _lpArrow->GetDir();
}
bool ETutoBox::HasArrow() {
	return _lpArrow != NULL;
}
void ETutoBox::MoveArrow(float x, float y) {
	if (_lpArrow == NULL) {
		SetBoxCenter(x,y);
	} else {
		_lpArrow->Move(x,y);
	}
	ComputePos();
}
Coord ETutoBox::GetArrowPos() {
	return _lpArrow->GetPos();
}

void ETutoBox::UpdateAlpha() {
	SetBlend(_fTutoBlend->getCurrentValue());

	_lpTutoText->GetFont()->setBlend(_fBlend);
	_lpTextClickVariation->GetFont()->setBlend(_fBlend);

	if (_lpArrow != NULL)
		_lpArrow->SetBlend(_fBlend);
}
void ETutoBox::ToggleCursor() {
	if (IsOver()) {
		if (_bWasOver)
			return;
		SCENEDIRECTOR->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
		EScene *lpSceneCurrent = SCENEDIRECTOR->GetCurrentScene();
		if (lpSceneCurrent != NULL) {
			lpSceneCurrent->SetMouseOverInfo("", "");
		}
		lpSceneCurrent = SCENEDIRECTOR->GetCurrentVignette();
		if (lpSceneCurrent != NULL) {
			lpSceneCurrent->SetMouseOverInfo("", "");
		}
		_bWasOver = true;
	} else if (_bWasOver) {
		_bWasOver = false;
	}
}
void ETutoBox::CloseTuto() {
	_bCompleted = true;
	Hide();
}


ETutoArrow::ETutoArrow(float x, float y, float dir, float scalemax, float scaleduration, float fadduration) {	
	_lpCounterFad = new KCounter();
	_lpCounterFad->startCounter(0,0,0,0,K_COUNTER_LINEAR);
	_lpCounterFad->move(1);
	_lpCounterScale =  new KCounter();
	_lpCounterScale->startCounter(1,1,0,0,K_COUNTER_LINEAR);
	_lpCounterScale->move(1);

	_fAngle = dir;
	Move(x,y);
	SetArrow(EImageBank::getImage("hint_arrow","png"));
	SetScaleParm(scalemax,scaleduration);
	SetFadeDuration(fadduration);
	
}
ETutoArrow::~ETutoArrow() {
	XDELETE(_lpCounterFad);
	XDELETE(_lpCounterScale);
}

void ETutoArrow::Draw() {
	double fElapsedTime = MyGame::getGame()->getElapsedTime();
	if (!_lpCounterFad->isCompleted()) {
		_lpCounterFad->move(fElapsedTime);
	}
	if (!_lpCounterScale->isCompleted()) {
		_lpCounterScale->move(fElapsedTime);
	} else {
		float scaleDst = _fScaleMax;
		if (_lpCounterScale->getCurrentValue() > 1) {
			scaleDst = 1;
		}
		_lpCounterScale->startCounter(_lpCounterScale->getCurrentValue(),scaleDst,0,_fScaleDuration,K_COUNTER_EASEIN);
	}

	_lpArrow->blitAlphaRectFx(0,0,_w,_h,_xImg,_yImg,_fAngle,_lpCounterScale->getCurrentValue(),_lpCounterFad->getCurrentValue());
}
void  ETutoArrow::Move(float x, float y) {
	_x = x;
	_y = y;
	_xImg = _x - _w/2;
	_yImg = _y - _h/2;
}
void  ETutoArrow::SetDir(float dir) {
	_fAngle = dir;
}
void ETutoArrow::SetBlend(float blend) {
	_lpCounterFad->startCounter(blend,blend,0,0,K_COUNTER_LINEAR);
	_lpCounterFad->move(1);
}
Coord ETutoArrow::GetPos() {
	return Coord(_x,_y);
}
Coord ETutoArrow::GetPosImg() {
	return Coord(_xImg,_yImg);
}

void ETutoArrow::SetArrow(KGraphic *arrow) {
	_lpArrow = arrow;
	_w = _lpArrow->getWidth();
	_h = _lpArrow->getHeight();
	Move(_x,_y);
}
void  ETutoArrow::SetScaleParm(float scale, float duration) {
	_fScaleMax = scale; 
	_fScaleDuration = duration;
}

void  ETutoArrow::SetFadeDuration(float duration) {
	_fFadDuration = duration;
}

void  ETutoArrow::Hide(float duration) {
	if (_lpCounterFad->getCurrentValue() != 0) {
		float d = _fFadDuration;
		if (duration >= 0)
			d = duration;

		_lpCounterFad->startCounter(_lpCounterFad->getCurrentValue(),0,0,d,K_COUNTER_LINEAR);
	}
}
void  ETutoArrow::Show(float duration) {
	if (_lpCounterFad->getCurrentValue() != 1) {
		float d = _fFadDuration;
		if (duration >= 0)
			d = duration;
		_lpCounterFad->startCounter(_lpCounterFad->getCurrentValue(),1,0,d,K_COUNTER_LINEAR);
   }
}
bool  ETutoArrow::IsVisible() {
	return _lpCounterFad->getCurrentValue() > 0;
}