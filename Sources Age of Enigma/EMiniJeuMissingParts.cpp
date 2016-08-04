#include "assert.h"

#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "FontBank.h"

#include "EMiniJeuMissingParts.h"

using EMiniGame::GetRandom;

EMiniJeuMissingParts::EMiniJeuMissingParts(const char *imgName, const char *imgBGName, int xpos, int ypos, const char *imgEnd, int xend, int yend)
{
	SetTextsTrad("MISSINGPARTS");
	SetGameType(MINIJEU_SUBMITGAME);
	SetGameNumber(12,2);
	SetBackGround(EImageBank::getImage(imgBGName));

	_nGameSize = 5;
	_nNumResponse = 2;
	
	_lpImgEnd = new EMiniJeuPart(EImageBank::getImage(imgEnd));
	_lpImgEnd->Hide();
	_lpImgEnd->Move(xend,yend);

	_lpBufferImg = GetKImage(imgName);
	_lpImg = NULL;


	if (xpos < 0 || ypos <0) {
		_posImg = Coord ((int)(_nW - _lpBufferImg->getImageWidth()) /2, (_nH - _lpBufferImg->getImageHeight())/2);
	} else {
		KAssert(xpos >=0 && ypos >= 0);
		_posImg = Coord(xpos - _nX, ypos - _nY);
	}

	_vMasksPath.push_back("ecuts_mask1.png");
	_vMasksPath.push_back("ecuts_mask2.png");
	_vMasksPath.push_back("ecuts_mask3.png");
	_vMasksPath.push_back("ecuts_mask4.png");

	_vMissingPart = std::vector< EMissingPart * >(_nGameSize, (EMissingPart *) NULL);

	_lpGridResponse = NULL;
	Reset();
}

EMiniJeuMissingParts::~EMiniJeuMissingParts(void)
{
	XDELETE(_lpImg);
	EGLL();
   XDELETE(_lpBufferImg);
   EGLR();

	FreeImgCut();

	XDELETE(_lpGridResponse);
	XDELETE(_lpImgEnd);
}

/** 3 steps rendering **/
void EMiniJeuMissingParts::GameCheck ()
{
	if (!Mouse.LeftEvent() || !_lpGridResponse->IsMouseOver())
		return;

	int numSelected = 0;
	for(int i=0; i<_vMissingPart.size(); i++) {
		if (_vMissingPart[i]->IsSelected())
			numSelected++;
	}

	for(int i=0; i<_vMissingPart.size(); i++) {
		if (_vMissingPart[i]->IsMouseOver()) {
			if (_nNumResponse > 1 && numSelected == _nNumResponse && !_vMissingPart[i]->IsSelected()) {
				Mouse.LeftEventClose();
				ESoundBank::getSound("mg_forbid")->playSample();
				return;
			}
			_vMissingPart[i]->Check();
			ESoundBank::getSound("mg_select_deselect")->playSample();
			
		} else if (_nNumResponse < 2) {
			_vMissingPart[i]->Deselect();
		}
	}
}

void EMiniJeuMissingParts::GameLogic ()
{
	for (int i=0; i < _vMissingPart.size(); i++) {
		_vMissingPart[i]->Logic();
	}
}

void EMiniJeuMissingParts::GameDrawSet()
{		
	Coord pos;

	_lpImg->blitAlphaRect(0,0,_lpImg->getWidth(),_lpImg->getHeight(),_posImg.x, _posImg.y);
	_lpGridResponse->DrawBaseGrid(_lpImg,0.2f);

	for (int i=0; i < _vMissingPart.size(); i++) 
		_vMissingPart[i]->Draw();

	_lpImgEnd->Draw();
}

void EMiniJeuMissingParts::GameReset() {
	const char *maskPath = _vMasksPath[GetRandom(_vMasksPath.size())];
	KImage *imgMask = GetKImage(maskPath);
	int imgW = imgMask->getImageWidth(), imgH = imgMask->getImageHeight();
	int margin = 10;	
	unsigned long *lpImgPixels = _lpBufferImg->getPixels();
	unsigned long *lpMaskPixels = imgMask->getPixels();

   EGLL();
	FreeImgCut();
	_vMissingPart = std::vector< EMissingPart * >(_nGameSize, (EMissingPart *) NULL);

	// Initialisation de la grille
	XDELETE(_lpGridResponse);
	_lpGridResponse = new Grid(_nGameSize,1,imgMask->getImageWidth(),imgMask->getImageHeight(),2,15);
	_lpGridResponse->Move(_posImg.x + (_lpBufferImg->getImageWidth() - _lpGridResponse->width())/2,_posImg.y + _lpBufferImg->getImageHeight() + 80);

	// Génération des découpages de l'image
	Coord randomPos;
	std::vector< bool > *tmpMask = GenerateAlphaMask(maskPath);
m0:	for (int i=0; i < _nGameSize; i++) {
		randomPos = Coord(
			imgW + GetRandom(_lpBufferImg->getImageWidth() - 2*imgW - margin),
			imgH + GetRandom(_lpBufferImg->getImageHeight() - 2*imgH - margin));
		
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

		_vMissingPart[i] = new EMissingPart(cut);
		_vMissingPart[i]->Move(_lpGridResponse->GetCellPos(i,0));
		_vMissingPart[i]->SetBaseAngle(GetRandom(360));
		_vMissingPart[i]->SetImgInPos(randomPos);
	}

	for (int i=0; i< _nNumResponse; i++) {
		int ntry = 0;
m1:		int nResponse = GetRandom(_vMissingPart.size());
		if (TestSolColision(nResponse, imgW, imgH)) {
			ntry++;
			if (ntry > 10) {
				FreeImgCut();
				goto m0;
			}
			goto m1;
		}
		_vMissingPart[nResponse]->SetIsSol();
	}

	unsigned long *tmpImg;
	tmpImg = (unsigned long *) malloc(_lpBufferImg->getImageWidth() * _lpBufferImg->getImageHeight() * sizeof(unsigned long));
	memcpy(tmpImg,_lpBufferImg->getPixels(),_lpBufferImg->getImageWidth() * _lpBufferImg->getImageHeight() * sizeof(unsigned long));

	for (int i=0; i< _vMissingPart.size(); i++) {
		randomPos = _vMissingPart[i]->_posInImg;
		if (_vMissingPart[i]->IsSol()) {

			// Cut the image
			for (int j=0; j < imgH; j++) {
				for (int k=0; k < imgW; k++) {
					if ((*tmpMask)[j*imgW+k])
						tmpImg[(int)(randomPos.y + j)*_lpBufferImg->getImageWidth()+(int)(randomPos.x + k)] = 0x00000000;
				}
			}
		}
		_vMissingPart[i]->SetImgInPos(Coord(randomPos.x + _posImg.x, randomPos.y + _posImg.y));
	}
	XDELETE(_lpImg);
	_lpImg = KPTK::createKGraphic();
	_lpImg->makePictureFromArray(tmpImg,_lpBufferImg->getImageWidth(),_lpBufferImg->getImageHeight(),true);
	_lpImg->pollArray();

	XDELETE(tmpMask);
	free(tmpImg);
	XDELETE(imgMask);
   EGLR();
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuMissingParts::GameIsResolve()
{
	bool res = true;
	for (int i=0; i<_vMissingPart.size(); i++) {
		if (!_vMissingPart[i]->ValidAnswer()) 
			res = false;
	}
	
	if (res == true)
		_lpImgEnd->FadIn(1000);

	return res;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuMissingParts::GameResolve()
{
	for (int i=0; i<_vMissingPart.size(); i++) {
		_vMissingPart[i]->Deselect();
		if (_vMissingPart[i]->IsSol())
			_vMissingPart[i]->Select();
	}
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuMissingParts::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  ShowHint(MINIJEU_HINT1);
		  _nNumResponse = 1;
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

void EMiniJeuMissingParts::FreeImgCut() {
   EGLL();
	// Liberation des précédent découpages
	for (int i=0; i < _vMissingPart.size(); i++) {
		if (_vMissingPart[i] != NULL) {
			KGraphic *tmpGraphic = _vMissingPart[i]->GetImg();
			XDELETE(tmpGraphic);
			XDELETE(_vMissingPart[i]);
		}
	}
   EGLR();
}

bool EMiniJeuMissingParts::TestSolColision(int sol, int w, int h) {
	Coord pos1, pos2;
	pos1 = _vMissingPart[sol]->_posInImg;
	
	if (_vMissingPart[sol]->IsSol())
		return true;

	for (int i=0; i < _vMissingPart.size(); i++) {
		if (i == sol)
			continue;
		
		pos2 = _vMissingPart[i]->_posInImg;
		if ( pos1.x >= pos2.x - w && pos1.x <= pos2.x + w &&
			pos1.y >= pos2.y - h && pos1.y <= pos2.y + h)
			return true;
	}
	return false;
}

float EMissingPart::_fTimeout = 500;
EMissingPart::EMissingPart(KGraphic *img): EMiniJeuPart(img,1,0,0,0) {
	_lpOverlaySelected = EImageBank::getImage("minijeu_selection.png");;
	
	_bIsSelected = false;
	_bIsSol = false;
	_bError = false;
	
	_lpAlphaSelect = NULL;
	_fAlphaSelect = 0;

	_fScaleSelect = _lpImg->getWidth() / _lpOverlaySelected->getWidth();

	_lpCounterA = NULL;

	SwitchDrag();
}
EMissingPart::~EMissingPart(void) {
	XDELETE(_lpAlphaSelect);
	XDELETE(_lpCounterA);
}
void EMissingPart::Check() {
	if (IsMouseOver() && Mouse.LeftEvent()) {
		if(_bIsSelected) {
			Deselect();
		} else {
			Select();
		}
		Mouse.LeftEventClose();
	}
}
void EMissingPart::Draw() {
	EMiniJeuPart::Draw();
	double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();

	if (_lpAlphaSelect != NULL) {
		_lpAlphaSelect->move(fElapsed);
		_fAlphaSelect = _lpAlphaSelect->getCurrentValue();
		if (_lpAlphaSelect->isCompleted())
			XDELETE(_lpAlphaSelect);
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
	if (_fAlphaSelect > 0) {
		_lpOverlaySelected->blitAlphaRectFx(0,0,_lpOverlaySelected->getWidth(),_lpOverlaySelected->getHeight(),
			(short) (_x + (_w/2) - (_lpOverlaySelected->getWidth()/2)),
			(short) (_y + (_h/2) - (_lpOverlaySelected->getHeight()/2)),
			0,_fScaleSelect,_fAlphaSelect);
	}
}
bool EMissingPart::ValidAnswer() {
	if (IsSol()) {
		Move(_posInImg, 1000);
		_lpCounterA = new KCounter();
		_lpCounterA->startCounter(_fBaseAngle,720,0,1000,K_COUNTER_LINEAR);
			
		if (IsSelected()) {
			Deselect();
			return true;
		}
		_lpImg->setBlitColor(1,0.5,0.5,1);
		_bError = true;
	} else if (IsSelected()) {
		_lpImg->setBlitColor(1,0.5,0.5,1);
		_bError = true;
	}

	return !_bError;
}
void EMissingPart::Deselect(bool Anim){
	if (!IsSelected())
		return;

	_bIsSelected = false;
	if (Anim) {
		XDELETE(_lpAlphaSelect);
		_lpAlphaSelect = new KCounter();
		_lpAlphaSelect->startCounter(_fAlphaSelect,0,0,_fTimeout,K_COUNTER_LINEAR);
	} else {
		_fAlphaSelect = 0;
	}
}
void EMissingPart::Select(bool Anim){
	if (IsSelected())
		return;
	
	_bIsSelected = true;
	if (Anim) {
		XDELETE(_lpAlphaSelect);
		_lpAlphaSelect = new KCounter();
		_lpAlphaSelect->startCounter(_fAlphaSelect,1,0,_fTimeout,K_COUNTER_LINEAR);
	} else {
		_fAlphaSelect = 1;
	}
}
bool EMissingPart::IsSelected() {
	return _bIsSelected;
}

// Preload images et sons
void EMiniJeuMissingParts::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("minijeu_selection.png","",true,true);
   EImageBank::getImage("ecuts_mask1.png","",true,true);
   EImageBank::getImage("ecuts_mask2.png","",true,true);
   EImageBank::getImage("ecuts_mask3.png","",true,true);
   EImageBank::getImage("ecuts_mask4.png","",true,true);
   EImageBank::getImage("mg_missingpart_fresque1.png","",true,true);
   EImageBank::getImage("mg_missingpart_fresque2.png","",true,true);
}
