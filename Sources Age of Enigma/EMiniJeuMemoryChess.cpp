#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "FontBank.h"

#include "EMiniJeuMemoryChess.h"

#define MEMORYPLANT_NARIATION 3

using EMiniGame::GetRandom;

EMiniJeuMemoryChess::EMiniJeuMemoryChess(void) {
	SetTextsTrad("CHESSMEMORY");
	SetGameType(MINIJEU_SUBMITGAME);
	SetBackGround(EImageBank::getImage("mg_chessboard_bg","jpg"));
	SetGameNumber(11);

	_nNumQuery = 1;
	_nCurrentQuery = 0;

	_lpFont = EFontBank::getFont("NITECLUB.TTF",45);
	_lpFontStart = EFontBank::getFont("NITECLUB.TTF",46);
	_lpFontTimeout = EFontBank::getFont("NITECLUB.TTF",60);
	_lpFont->setColor(1,1,1,1);
	_lpFontStart->setColor(1,1,1,1);
	_lpFontTimeout->setColor(1,1,1,1);

	memset(_cTextStart,0,sizeof(_cTextStart));
	memset(_cTextTimeout,0,sizeof(_cTextTimeout));
	memset(_cTextTimeoutSeconds,0,sizeof(_cTextTimeoutSeconds));
	strcat(_cTextStart,KStr("PLANTMEMORY_START"));
	strcat(_cTextTimeout,KStr("PLANTMEMORY_SHOW"));

	_fShowTimeDuration = 30000;
	_bBlockReset = false;
	_bDisplayStart = true;
	_bMaskSet = false;
	_lpShowTime = NULL;
	_qCurrentQuery = NULL;
	_nSecondRemaining = 0;
	_nResponse = -1;

	_vChessboards.push_back(new EMemoryChessboard("mg_chessboard_1",112,163));
	_vChessboards.push_back(new EMemoryChessboard("mg_chessboard_2",375,175));
	_vChessboards.push_back(new EMemoryChessboard("mg_chessboard_3",619,166));

	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY1_CHESSMEMORY"),2));
	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY2_CHESSMEMORY"),0));
	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY3_CHESSMEMORY"),0));
	//_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY4_CHESSMEMORY"),3));
	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY5_CHESSMEMORY"),6,0,EMemoryChessQuery::NUMERIC));
	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY5_CHESSMEMORY"),5,1,EMemoryChessQuery::NUMERIC));
	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY5_CHESSMEMORY"),5,2,EMemoryChessQuery::NUMERIC));
	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY6_CHESSMEMORY"),2,0,EMemoryChessQuery::NUMERIC));
	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY6_CHESSMEMORY"),2,1,EMemoryChessQuery::NUMERIC));
	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY6_CHESSMEMORY"),3,2,EMemoryChessQuery::NUMERIC));
	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY7_CHESSMEMORY"),4,0,EMemoryChessQuery::NUMERIC));
	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY7_CHESSMEMORY"),3,1,EMemoryChessQuery::NUMERIC));
	_vQuerySet.push_back(new EMemoryChessQuery(KStr("QUERY7_CHESSMEMORY"),1,2,EMemoryChessQuery::NUMERIC));
	
	_lpImgStart = new EMiniJeuPart(EImageBank::getImage("mg_memorygrid_start_bouton","png"));
	_lpImgStart->Move((_nW - _lpImgStart->Width()) /2, _vChessboards[1]->Pos().y + _vChessboards[1]->Height() + 50);

	for (int i=0; i<_vQuerySet.size();i++)
		_vQuerySet[i]->SetYPos(_lpImgStart->Pos().y);
	
	Reset();
}

EMiniJeuMemoryChess::~EMiniJeuMemoryChess(void)
{
	for(int i=0;i<_vQuerySet.size();i++)
		XDELETE(_vQuerySet[i]);

	for (int i=0; i<_vChessboards.size(); i++)
		XDELETE(_vChessboards[i]);

	XDELETE(_lpImgStart);
   XDELETE(_lpFont);
   XDELETE(_lpFontStart);
   XDELETE(_lpFontTimeout);
}

/** 3 steps rendering **/
void EMiniJeuMemoryChess::GameCheck ()
{
	if (_bDisplayStart) {
		if (!Mouse.LeftEvent() || !_lpImgStart->IsMouseOver())
			return;

		ESoundBank::getSound("mg_apparition")->playSample();
		_bDisplayStart = false;
		for (int i=0; i<_vChessboards.size(); i++)
			_vChessboards[i]->FadIn(500);
		Mouse.LeftEventClose();
	
		if (_lpShowTime != NULL)
			XDELETE(_lpShowTime);
		_lpShowTime = new KCounter();
		_lpShowTime->startCounter(-3,_fShowTimeDuration/1000,0,(double)_fShowTimeDuration+3000,K_COUNTER_LINEAR);
		return;
	}

	if (!_bMaskSet && Mouse.LeftEvent()) {
		_lpShowTime->move(_fShowTimeDuration);
		Mouse.LeftEventClose();
		return;
	}

	if (Mouse.LeftEvent() && _qCurrentQuery != NULL) {
		if (_qCurrentQuery->_nType == EMemoryChessQuery::SELECT) {
			for (int i=0; i<_vChessboards.size(); i++) {
				if (_vChessboards[i]->IsMouseOver()) {
					_qCurrentQuery->IsValidAnswer(i);
					Mouse.LeftEventClose();
					_vChessboards[i]->Select();
					ESoundBank::getSound("mg_select_deselect")->playSample();
					continue;
				}
				_vChessboards[i]->DeSelect();
			}
		} else {
			_qCurrentQuery->Check();
		}
	}
}

void EMiniJeuMemoryChess::GameLogic ()
{
	if (_bDisplayStart)
		return;

	for (int i=0; i<_vChessboards.size(); i++)
		_vChessboards[i]->Logic();
}

void EMiniJeuMemoryChess::GameDrawSet()
{
	Coord pos;

	for (int i=0; i<_vChessboards.size(); i++)
			_vChessboards[i]->Draw();

	if (_bDisplayStart) {
		pos = _lpImgStart->Pos();
		pos.y = pos.y + (100 - 46)/2;
		_lpImgStart->Draw();
		_lpFontStart->drawStringCentered(_cTextStart,pos.x +44,pos.x + _lpImgStart->Width() -30,pos.y);
		return;
	}
	
	if (!_lpShowTime->isCompleted()) {
		double fElapsed = _lpMyGame->getKWindow()->getFrameTime();
		
		_lpShowTime->move(fElapsed);
		
		if (_nSecondRemaining != (int)((_fShowTimeDuration/1000) - _lpShowTime->getCurrentValue())) {
			_nSecondRemaining = (_fShowTimeDuration/1000) - _lpShowTime->getCurrentValue();
			memset(_cTextTimeoutSeconds,0,sizeof(_cTextTimeoutSeconds));
			strcat(_cTextTimeoutSeconds,(itos(_nSecondRemaining)+" Secondes").c_str());
			if (_nSecondRemaining <=(int)(_fShowTimeDuration /1000))
				ESoundBank::getSound("mg_counter_tic")->playSample();
		}

		if (_nSecondRemaining > _fShowTimeDuration/1000)
			return;

		pos = _lpImgStart->Pos();
		
		_lpFont->drawStringCentered(_cTextTimeout,pos.x + 44,pos.x + _lpImgStart->Width() -30,pos.y);
		_lpFontTimeout->drawStringCentered(_cTextTimeoutSeconds,pos.x +44,pos.x + _lpImgStart->Width() -30,pos.y + 55);

		return;
	} else if (!_bMaskSet) {
		for (int i=0; i<_vChessboards.size(); i++)
			_vChessboards[i]->FadOut(500);
		ESoundBank::getSound("mg_end_counter")->playSample();
		ESoundBank::getSound("mg_disparition")->playSample();
		SetQuery();
		_bMaskSet = true;
	}

	if(_qCurrentQuery != NULL)
		_qCurrentQuery->Draw();
}

void EMiniJeuMemoryChess::GameReset() {
	if (_bBlockReset) {
		ESoundBank::getSound("mg_forbid")->playSample();
		return;
	}

	if (_lpShowTime != NULL)
		XDELETE(_lpShowTime);

	SetMultiSubmit();
	_bDisplayStart = true;
	_bMaskSet = false;
	_nCurrentQuery = 0;
	_nResponse = 0;
	
	if (_nHintLevel == MINIJEU_NORMAL) {
		int r;
		_vQuery = std::vector< EMemoryChessQuery * >(_nNumQuery,(EMemoryChessQuery *) NULL);
		for(int i=0;i<_vQuery.size();i++) {
m0:		r = GetRandom(_vQuerySet.size());
			for(int j=0;j<_vQuery.size();j++)
				if (_vQuery[j] == _vQuerySet[r])
					goto m0;
			_vQuery[i] = _vQuerySet[r];
			_vQuery[i]->Reset();
		}
	}
	for (int i=0; i<_vChessboards.size();i++) {
		_vChessboards[i]->Reset();
	}

	if (_nHintLevel > MINIJEU_NORMAL)
		_bBlockReset = true;
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuMemoryChess::GameIsResolve()
{
	_nCurrentQuery++;
	SetQuery();

	if(_nCurrentQuery < _nNumQuery)
		return false;

	SetLastSubmit();
	for (int i=0; i<_vChessboards.size(); i++) {
			_vChessboards[i]->FadIn(1000);
			_vChessboards[i]->DeSelect();
	}
	for(int i=0;i<_vQuery.size();i++) {
		if (!_vQuery[i]->ValidAnswer()) {
			_nGameStatus = MINIJEU_FAILED;
			return false;
		}
	}
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuMemoryChess::GameResolve()
{
	_bDisplayStart = false;
	XDELETE(_lpShowTime);
	_lpShowTime = new KCounter;
	_lpShowTime->startCounter(0,_fShowTimeDuration,0,0,K_COUNTER_LINEAR);
	_lpShowTime->move(150);
	SetLastSubmit();
	_vQuery.clear();
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuMemoryChess::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  ShowHint(MINIJEU_HINT1);
		  _fShowTimeDuration = 5000;
		  Reset();
		  break;
	  case MINIJEU_HINT2:
		  ShowHint(MINIJEU_HINT2);
		  _bBlockReset = false;
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

void EMiniJeuMemoryChess::SetQuery() {
	for (int i=0; i<_vChessboards.size();i++) {
		_vChessboards[i]->_bShowQueryMask = false;
		_vChessboards[i]->_bShowQueryMask = false;
	}
	
	_qCurrentQuery = NULL;

	if (_nCurrentQuery >= _nNumQuery)
		return;

	for (int i=0; i<_vChessboards.size();i++) {
			_vChessboards[i]->DeSelect();
	}
	_qCurrentQuery = _vQuery[_nCurrentQuery];
	if (_qCurrentQuery->_nQueryMark == -1) {
		for (int i=0; i<_vChessboards.size();i++)
			_vChessboards[i]->_bShowQueryMask = true;
	} else {
		_vChessboards[_qCurrentQuery->_nQueryMark]->_bShowQueryMask = true;
		_vChessboards[_qCurrentQuery->_nQueryMark]->Select();
	}
}

/* ----------------------------------------------------- */

EMemoryChessboard::EMemoryChessboard(char *imgName, int x, int y): EMiniJeuPart(EImageBank::getImage(imgName,"png"),1) {
	std::string tmp = imgName;

	// Inversion mask et image standard (gestion du clique sur fadin fadout)
	_lpImgMask = _lpImg;
	_lpImg = EImageBank::getImage((tmp+"_mask").c_str(),"png");
	_lpImgMaskQuery = EImageBank::getImage((tmp+"_mask_query").c_str(),"png");
	_lpImgSelect = EImageBank::getImage((tmp+"_select").c_str(),"png");

	std::vector< bool >* tmpMask = GenerateAlphaMask((tmp + "_mask.png").c_str());
	_lpArrayMask = (*tmpMask);
	XDELETE(tmpMask);

	// Fin de l'initialisation de l'echiquier
	_fAlphaMask = 0;
	_fAlphaSelect = 0;
	_bShowQueryMask = false;
	_bSelect = false;

	_lpSelectFad = NULL;

	_x = x;
	_y = y;
}
EMemoryChessboard::~EMemoryChessboard(void) {
}
	
void EMemoryChessboard::Draw() {
	_lpImgMask->blitAlphaRect(0,0,_lpImgMask->getWidth(),_lpImgMask->getHeight(),_x,_y);
	EMiniJeuPart::Draw();
	
	if (_lpSelectFad != NULL) {
		double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
		_lpSelectFad->move(fElapsed);
		_fAlphaSelect = _lpSelectFad->getCurrentValue();
		if (_lpSelectFad->isCompleted()) {
			XDELETE(_lpSelectFad);
		}
	}
	_lpImgSelect->blitAlphaRectFx(0,0,_lpImgSelect->getWidth(),_lpImgSelect->getHeight(),_x,_y,0,1,_fAlphaSelect);

	if (_bShowQueryMask)
		_lpImgMaskQuery->blitAlphaRectFx(0,0,_lpImgMaskQuery->getWidth(),_lpImgMaskQuery->getHeight(),_x,_y,0,1,1-_fAlphaMask);

}
bool EMemoryChessboard::IsMouseOver(){
	if (!EMiniJeuPart::IsMouseOver())
		return false;

	int xrel = Mouse.x-_x;
	int yrel = Mouse.y-_y;

	int pos = _lpArrayMask[(int)(yrel * _lpImgMask->getWidth() + xrel)];
	return pos;
}

void EMemoryChessboard::Logic(){
	EMiniJeuPart::Logic();
	_fAlphaMask = 1 - _fBlend;
}

void EMemoryChessboard::FadIn(double fDuration) {
	EMiniJeuPart::FadOut(fDuration);
}

void EMemoryChessboard::FadOut(double fDuration) {
	EMiniJeuPart::FadIn(fDuration);
}

void EMemoryChessboard::Select(bool fad) {
	float fAlphaStart = _fAlphaSelect;
	if (_bSelect)
	 return;

	if (_lpSelectFad != NULL) {
		fAlphaStart = _lpSelectFad->getCurrentValue();
		XDELETE(_lpSelectFad);
	}

	_bSelect = true;
	if (fad) {
		_lpSelectFad = new KCounter();
		_lpSelectFad->startCounter(fAlphaStart,1,0,200,K_COUNTER_LINEAR);
	} else {
		_fAlphaSelect = 1;
	}
}
void EMemoryChessboard::DeSelect(bool fad) {
	float fAlphaStart = _fAlphaSelect;
	if (!_bSelect && _lpSelectFad == NULL)
	 return;

	if (_lpSelectFad != NULL) {
		fAlphaStart = _lpSelectFad->getCurrentValue();
		XDELETE(_lpSelectFad);
	}

	_bSelect = false;
	if (fad) {
		_lpSelectFad = new KCounter();
		_lpSelectFad->startCounter(fAlphaStart,0,0,200,K_COUNTER_LINEAR);
	} else {
		_fAlphaSelect = 0;
	}
}
void EMemoryChessboard::Reset() {
	_fAlphaMask = 0;
	_fAlphaSelect = 0;
	_bShowQueryMask = false;
	_bSelect = false;
	
	XDELETE(_lpSelectFad);
}

/* ----------------------------------------------------- */

EMemoryChessQuery::EMemoryChessQuery(const char *cQuery, int nAnswer, int queryMark, int nType) {
	_nType = nType;
	_nQueryMark = queryMark;

	memset(_cQuery,0,sizeof(_cQuery));
	strcpy(_cQuery,cQuery);

	_nFontSize = 32;
	_lpFontQuery = EFontBank::getFont("tahoma.ttf",_nFontSize);
	_lpFontQuery->setColor(0,0,0,1);

	_nAnswer = nAnswer;
	_nResponse = 0;
	_y = 0;
	
	_lpImg = KPTK::createKGraphic();
	_fQueryZoneSize = 700;
	_Digit = NULL;

	_lpQueryBox = new EImgExpendableBox(EImageBank::getImage("tuto_img_bg.png"),6,6,6,6);
	_lpQueryBox->Move(0,0);
	_lpQueryBox->SetWidth(_fQueryZoneSize);
	_lpQueryBox->SetHeight(10);

	ETextBox *textBox = new ETextBox(cQuery,_lpFontQuery,_fQueryZoneSize);
	textBox->SetMargin(2,2,2,2);
	
	if (nType == NUMERIC) {
		
		_Digit = new Digit(_lpFontQuery);
		_fQueryZoneSize -= _Digit->width() + 10;

		EBox *digitBox = new EBox(0,0,_Digit->width(),_Digit->height());
		digitBox->SetSizePolicy(EBox::FREE);
		textBox->SetWidth(_fQueryZoneSize-digitBox->GetWidth());

		EBox *tmpBox = new EBox(0,0,_fQueryZoneSize,0);
		tmpBox->SetLayoutType();
		tmpBox->AddChild(textBox);
		tmpBox->AddChild(digitBox);

		_lpQueryBox->AddChild(tmpBox);
	} else {
		_lpQueryBox->AddChild(textBox);
	}
	
//	textbox->SetHeight(textbox->GetHeight());


	SetYPos(0);
}
EMemoryChessQuery::~EMemoryChessQuery(void) {
	XDELETE(_Digit);
	XDELETE(_lpImg);
	XDELETE(_lpQueryBox);
   XDELETE(_lpFontQuery);
}

void EMemoryChessQuery::Check() {
	if(_nType != NUMERIC)
		return;
	
	_Digit->Check();
	IsValidAnswer(_Digit->GetVal());
}
void EMemoryChessQuery::Reset() {
	_bGoodAnswer = false;
}
bool EMemoryChessQuery::IsValidAnswer(int res) {
	_nResponse = res;

	_bGoodAnswer = false;
	if (_nResponse == _nAnswer)
		_bGoodAnswer = true;
	return _bGoodAnswer;
}
void EMemoryChessQuery::Draw() {

	_lpQueryBox->Draw();

	//_lpImg->drawRect(_pBlendBox1.x,_pBlendBox1.y,_pBlendBox2.x,_pBlendBox2.y,0,0,0,0.5);
	//_lpFontQuery->drawMultiline(_cQuery,_pBlendBox1.x + 15,_pBlendBox1.x + 15 + _fQueryTextWidth,_y,TEXTSTYLE_CENTER);

	if (_nType == NUMERIC) {
		_Digit->Draw();
	}
}

void EMemoryChessQuery::SetYPos(int y) {
	_y = y;

	_fQueryTextWidth = _lpFontQuery->getMultilineWidth(_cQuery,0,_fQueryZoneSize,_y,TEXTSTYLE_CENTER);
   //float fQueryTextHeight = _lpFontQuery->getMultilineHeight(_cQuery,0,_fQueryZoneSize,_y,TEXTSTYLE_CENTER);
	
	//if (_Digit != NULL) {
	//	_pBlendBox1.x = (1024 - _fQueryTextWidth - 10 - _Digit->width()) / 2 - 15;
	//	_pBlendBox1.y = y - 7;
	//	_pBlendBox2.x = _pBlendBox1.x + 15 + _fQueryTextWidth + 10 + _Digit->width() + 15;
	//	if (fQueryTextHeight > _Digit->height()) {
	//		_Digit->Move(_pBlendBox2.x - _Digit->width() - 15,y + (fQueryTextHeight - _nFontSize) /2);
	//		_pBlendBox2.y = _pBlendBox1.y + fQueryTextHeight + 2*7;
	//	} else {
	//		_Digit->Move(_pBlendBox2.x - _Digit->width() - 15,y + (_Digit->height() - _nFontSize) /2);
	//		_pBlendBox2.y = _Digit->Y() + _Digit->height() + 7;
	//		_y = _y + (_Digit->height() - fQueryTextHeight)/2;
	//	}

	//	EBox *tmp = _lpQueryBox->GetChild(0)->GetChild(1);
	//	_Digit->MovePlus(tmp->GetPos());	
	//
	//} else {
	//	_pBlendBox1.x = (1024 - _fQueryTextWidth) / 2 - 15;
	//	_pBlendBox1.y = y - 7;
	//	_pBlendBox2.x = _pBlendBox1.x + _fQueryTextWidth + 2*15;
	//	_pBlendBox2.y = _pBlendBox1.y + fQueryTextHeight + 2*7;
	//}

	_lpQueryBox->Move((1024 - _lpQueryBox->GetWidth())/2, y - 7);
	if (_nType == NUMERIC )
		_Digit->MovePlus(_lpQueryBox->GetChild(0)->GetChild(1)->GetPos());
}

// Preload images et sons
void EMiniJeuMemoryChess::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_chessboard_bg","jpg",true,true);
   EImageBank::getImage("mg_memorygrid_start_bouton","png",true,true);
   EImageBank::getImage("tuto_img_bg.png","",true,true);

   ESoundBank::getSound("mg_apparition",false,true);
   ESoundBank::getSound("mg_counter_tic",false,true);
   ESoundBank::getSound("mg_end_counter",false,true);
   ESoundBank::getSound("mg_disparition",false,true);
}
