#include "assert.h"

#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "FontBank.h"

#include "EMiniJeuPhotoComp.h"

using EMiniGame::GetRandom;

EMiniJeuPhotoComp::EMiniJeuPhotoComp(const char *imgName, const char *imgBGName, int xpos, int ypos, const char *imgEnd, const char *cartouche, int xend, int yend)
{
	SetTextsTrad("PHOTOCOMP");
	SetGameType(MINIJEU_SUBMITGAME);
	SetGameNumber(13,2);
	SetBackGround(EImageBank::getImage(imgBGName));
	
	_nGameSize = 5;

	_lpBufferImg = GetKImage(imgName);
	_lpImg = EImageBank::getImage(imgName);

	_lpImgEnd = new EMiniJeuPart(EImageBank::getImage(imgEnd));
	_lpImgEnd->Hide();
	_lpImgEnd->Move(xend,yend);

	if (xpos < 0 || ypos <0) {
		_posImg = Coord ((int)(_nW - _lpBufferImg->getImageWidth()) /2, (_nH - _lpBufferImg->getImageHeight())/2);
	} else {
		KAssert(xpos >=0 && ypos >= 0);
		_posImg = Coord(xpos - _nX,ypos - _nY);
	}

	_vMasksPath.push_back("ecuts_mask1.png");
	_vMasksPath.push_back("ecuts_mask2.png");
	_vMasksPath.push_back("ecuts_mask3.png");
	_vMasksPath.push_back("ecuts_mask4.png");

	_lpImgCartouche = EImageBank::getImage(cartouche,"png");
	_vPhotoComp = std::vector< EPhotoCompPart * >(_nGameSize, (EPhotoCompPart *) NULL);
	
	_lpSelected = NULL;

	_lpGridResponse = NULL;
	Reset();
}

EMiniJeuPhotoComp::~EMiniJeuPhotoComp(void)
{
	EGLL();
   XDELETE(_lpBufferImg);
   EGLR();

	FreeImgCut();

	XDELETE(_lpGridResponse);
   XDELETE(_lpImgEnd);
}

void EMiniJeuPhotoComp::SetVersion(int nVersion) {
}

/** 3 steps rendering **/
void EMiniJeuPhotoComp::GameCheck ()
{
	if (!Mouse.LeftEvent())
		return;

	if (_lpSelected != NULL) {
		if (Mouse.x < _posImg.x || Mouse.x > _posImg.x + _lpImg->getWidth() ||
			Mouse.y < _posImg.y || Mouse.y > _posImg.y + _lpImg->getHeight()) {
			_lpSelected->UnLock();
		}
		ESoundBank::getSound("mg_select_deselect")->playSample();
		_lpSelected = NULL;
	}

	if (!Mouse.LeftEvent() || !_lpGridResponse->IsMouseOver())
		return;

	for(int i=0; i<_vPhotoComp.size(); i++) {
		if (_vPhotoComp[i]->IsMouseOver()) {
			_lpSelected = _vPhotoComp[i];
			_lpSelected->Check();
			ESoundBank::getSound("mg_select_deselect")->playSample();
			return;
		}
	}
}

void EMiniJeuPhotoComp::GameLogic ()
{
	for (int i=0; i < _vPhotoComp.size(); i++) {
		_vPhotoComp[i]->Logic();
	}
}

void EMiniJeuPhotoComp::GameDrawSet()
{		
	Coord pos;

	_lpImg->blitAlphaRect(0,0,_lpImg->getWidth(),_lpImg->getHeight(),_posImg.x, _posImg.y);
	//_lpGridResponse->DrawBaseGrid(_lpImg);
	_lpImgCartouche->blitAlphaRectFx(0,0,_lpImgCartouche->getWidth(),_lpImgCartouche->getHeight(),251,435,0,1,1);
	if (_lpSelected != NULL)
		_lpSelected->SetEndPoint(Coord(Mouse.x, Mouse.y));

	for (int i=0; i < _vPhotoComp.size(); i++) 
		_vPhotoComp[i]->Draw();
	
	_lpImgEnd->Draw();
}

void EMiniJeuPhotoComp::GameReset() {
	const char *maskPath = _vMasksPath[GetRandom(_vMasksPath.size())];
	
   EGLL();
	KImage *imgMask = GetKImage(maskPath);
	int imgW = imgMask->getImageWidth(), imgH = imgMask->getImageHeight();
	int margin = 10;	
	
	unsigned long *lpImgPixels = _lpBufferImg->getPixels();
	unsigned long *lpMaskPixels = imgMask->getPixels();

	_lpSelected = NULL;

	FreeImgCut();
	_vPhotoComp = std::vector< EPhotoCompPart * >(_nGameSize, (EPhotoCompPart *) NULL);
	
	// Initialisation de la grille
	XDELETE(_lpGridResponse);
	_lpGridResponse = new Grid(_nGameSize,1,imgMask->getImageWidth(),imgMask->getImageHeight(),2,15);
	_lpGridResponse->Move(348 + ( 739 - 348 - _lpGridResponse->width())/2,465);
	//_lpGridResponse->Move(_posImg.x + (_lpBufferImg->getImageWidth() - _lpGridResponse->width())/2,_posImg.y + _lpBufferImg->getImageHeight() + 10);

	// Génération des découpages de l'image
	Coord randomPos;
	std::vector< bool > *tmpMask = GenerateAlphaMask(maskPath);
	for (int i=0; i < _vPhotoComp.size(); i++) {
m0:		randomPos = Coord(
			imgW + GetRandom(_lpBufferImg->getImageWidth() - 2*imgW - margin),
			imgH + GetRandom(_lpBufferImg->getImageHeight() - 2*imgH - margin));
		
		if (TestSolColision(randomPos,imgW,imgH))
			goto m0;

		// copie des élément de l'image recouvert par le masque
		for (int j=0; j < imgH; j++) {
			for (int k=0; k < imgW; k++) {
				if ((*tmpMask)[j*imgW+k]) {
					lpMaskPixels[j*imgW+k] = lpImgPixels[(int)(randomPos.y + j)*_lpBufferImg->getImageWidth()+(int)(randomPos.x + k)];
					continue;
				}
				lpMaskPixels[j*imgW+k] = 0;
			}
		}
		
		// Creation de l'image découpé a partir du masque
		KGraphic *cut = KPTK::createKGraphic();
		cut->makePictureFromArray(lpMaskPixels,imgW,imgH,true);
		cut->pollArray();
      EGLR();
		_vPhotoComp[i] = new EPhotoCompPart(cut);
		EGLL();
      _vPhotoComp[i]->Move(_lpGridResponse->GetCellPos(i,0));
		_vPhotoComp[i]->SetBaseAngle(GetRandom(360));
		_vPhotoComp[i]->SetImgInPos(randomPos);
	}

	for (int i=0; i < _vPhotoComp.size(); i++) {
		randomPos = _vPhotoComp[i]->_posInImg;
		_vPhotoComp[i]->SetImgInPos(Coord(randomPos.x + _posImg.x, randomPos.y + _posImg.y));
	}
	XDELETE(tmpMask);
	XDELETE(imgMask);
   EGLR();
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuPhotoComp::GameIsResolve()
{
	bool res = true;
	for (int i=0; i<_vPhotoComp.size(); i++) {
		if (!_vPhotoComp[i]->ValidAnswer())
			res = false;
	}

	if (res == true)
		_lpImgEnd->FadIn(1000);

	return res;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuPhotoComp::GameResolve()
{
	for (int i=0; i<_vPhotoComp.size(); i++) {
		_vPhotoComp[i]->Solve();
	}
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuPhotoComp::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  ShowHint(MINIJEU_HINT1);
		  _nGameSize = 4;
		  Reset(); 
		  break;
	  case MINIJEU_HINT2:
		  ShowHint(MINIJEU_HINT2);
		  _nGameSize = 3;
		  Reset();
		  break;
	  case MINIJEU_RESOLVED:
		  Resolve();
		  break;
	  default:
		  KAssert(false);
		  break;
	}
}

void EMiniJeuPhotoComp::FreeImgCut() {
	// Liberation des précédent découpages
	EGLL();
   for (int i=0; i < _vPhotoComp.size(); i++) {
		if (_vPhotoComp[i] != NULL) {
			KGraphic *tmpGraphic = _vPhotoComp[i]->GetImg();
			XDELETE(tmpGraphic);
			XDELETE(_vPhotoComp[i]);
		}
	}
   EGLR();
}

bool EMiniJeuPhotoComp::TestSolColision(Coord pos, int w, int h) {
	Coord pos2;
	
	for (int i=0; i < _vPhotoComp.size(); i++) {
		if (_vPhotoComp[i] == NULL)
			break;
		
		pos2 = _vPhotoComp[i]->_posInImg;
		if ( pos.x >= pos2.x - w && pos.x <= pos2.x + w &&
			pos.y >= pos2.y - h && pos.y <= pos2.y + h)
			return true;
	}
	return false;
}

float EPhotoCompPart::_fTimeout = 1000;
EPhotoCompPart::EPhotoCompPart(KGraphic *img): EMiniJeuPart(img,1,0,0,0) {
	_lpImgPoint = EImageBank::getImage("ephotocomp_point.png");;
	
	_bError = false;
	_bIsLocked = false;
	
	_lpAlphaLock = NULL;
	_fAlphaLock = 0;

	_fScaleLock = _lpImg->getWidth() / _lpImgPoint->getWidth();

	_lpCounterA = NULL;

	_EndPoint = Coord(-1,-1);
	SwitchDrag();
}
EPhotoCompPart::~EPhotoCompPart(void) {
	XDELETE(_lpAlphaLock);
	XDELETE(_lpCounterA);
}
void EPhotoCompPart::Check() {
	if (IsMouseOver() && Mouse.LeftEvent()) {
		Lock();
		Mouse.LeftEventClose();
	}
}
void EPhotoCompPart::Draw() {
	EMiniJeuPart::Draw();
	double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();

	if (_lpAlphaLock != NULL) {
		_lpAlphaLock->move(fElapsed);
		_fAlphaLock = _lpAlphaLock->getCurrentValue();
		if (_lpAlphaLock->isCompleted()) {
			XDELETE(_lpAlphaLock);
			if (!_bIsLocked)
				_EndPoint = Coord(-1,-1);
		}
	}
	if (_lpCounterA != NULL) {
		_lpCounterA->move(fElapsed);
		SetBaseAngle(_lpCounterA->getCurrentValue());
		if (_lpCounterA->isCompleted()) {
			XDELETE(_lpCounterA);
			if (!_bError)
				_lpImg->setBlitColor(0.4,1,0.4,1);
		}
	}
	
	if (_bError) {
		_lpImg->blitAlphaRectFx(0,0,_lpImg->getWidth(),_lpImg->getHeight(),_posInImg.x,_posInImg.y,0,1,0.5);
	}

	if (_fAlphaLock > 0 && _EndPoint.x >=0 && _EndPoint.y >= 0) {
		Coord c = Center();
		
		_lpImgPoint->blitAlphaRectFx(0,0,_lpImgPoint->getWidth(),_lpImgPoint->getHeight(),
			(short) (c.x - (_lpImgPoint->getWidth()/2)),
			(short) (c.y - (_lpImgPoint->getHeight()/2)),
			0,_fScaleLock,_fAlphaLock);
		_lpImgPoint->drawLine(c.x,c.y,_EndPoint.x,_EndPoint.y,1,0,0,_fAlphaLock);
		_lpImgPoint->blitAlphaRectFx(0,0,_lpImgPoint->getWidth(),_lpImgPoint->getHeight(),
			(short) (_EndPoint.x - (_lpImgPoint->getWidth()/2)),
			(short) (_EndPoint.y - (_lpImgPoint->getHeight()/2)),
			0,_fScaleLock,_fAlphaLock);
	}
}

void EPhotoCompPart::UnLock(bool Anim){
	if (!IsSelected())
		return;

	_bIsLocked = false;
	if (Anim) {
		XDELETE(_lpAlphaLock);
		_lpAlphaLock = new KCounter();
		_lpAlphaLock->startCounter(_fAlphaLock,0,0,_fTimeout,K_COUNTER_LINEAR);
	} else {
		_fAlphaLock = 0;
	}
}
void EPhotoCompPart::Lock(bool Anim){
	if (IsSelected())
		return;
	
	_bIsLocked = true;
	if (Anim) {
		XDELETE(_lpAlphaLock);
		_lpAlphaLock = new KCounter();
		_lpAlphaLock->startCounter(_fAlphaLock,1,0,_fTimeout,K_COUNTER_LINEAR);
	} else {
		_fAlphaLock = 1;
	}
}
bool EPhotoCompPart::IsSelected() {
	return _bIsLocked;
}
bool EPhotoCompPart::ValidAnswer() {
	if (_EndPoint.x >= _posInImg.x && _EndPoint.x <= _posInImg.x + _w
		&& _EndPoint.y >= _posInImg.y && _EndPoint.y <= _posInImg.y + _h) {
		UnLock();
		_lpCounterA = new KCounter();
		_lpCounterA->startCounter(_fBaseAngle,720,0,1000,K_COUNTER_LINEAR);
		Move(_posInImg,1000);
		return true;
	}

	if (!_bIsLocked) {
		Lock();
		_EndPoint = Coord(_posInImg.x + _w/2, _posInImg.y + _h/2);
	}
	_lpImg->setBlitColor(1,0.5,0.5,1);
	_bError = true;
	return false;
}
void EPhotoCompPart::Solve() {
	Lock(false);
	_EndPoint = Coord(_posInImg.x + (_w/2), _posInImg.y + (_h/2));
}

// Preload images et sons
void EMiniJeuPhotoComp::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_photocomp_cartouche","png",true,true);
   EImageBank::getImage("ephotocomp_point.png","",true,true);
   EImageBank::getImage("ecuts_mask1.png","",true,true);
   EImageBank::getImage("ecuts_mask2.png","",true,true);
   EImageBank::getImage("ecuts_mask3.png","",true,true);
   EImageBank::getImage("ecuts_mask4.png","",true,true);
   EImageBank::getImage("mg_photocomp_fresque1.png","",true,true);
   EImageBank::getImage("mg_photocomp_fresque1.png","",true,true);
}
