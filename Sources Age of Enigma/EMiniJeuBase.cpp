#include "global.h"

#include "KCounter.h"
#include "SoundBank.h"
#include "GlobalBank.h"
#include "ImageBank.h"
#include "FontBank.h"
#include "EPointer.h"
#include "MyGame.h"
#include "ESceneDirector.h"
#include "EBox.h"

#include "EMiniJeuBase.h"

#define T_EXIT          _textTab[0]
#define T_SUBMIT        _textTab[1]
#define T_RESET         _textTab[2]
#define T_SKIP          _textTab[3]
#define T_HELP          _textTab[4]
#define T_TITLE_PREFIX  _textTab[5]
#define T_SUCCES        _textTab[6]
#define T_FAIL          _textTab[7]
#define T_TITLE         _textTab[9]
#define T_RULE          _textTab[10]
#define T_HINT1         _textTab[11]
#define T_HINT2         _textTab[12]
#define T_QUERY1        _textTab[13]
#define T_QUERY2        _textTab[14]
#define T_LAST          15
#define T_ _textTab[0]

#define I_VMASK         _imgTab[0]
#define I_VICTORY       _imgTab[1]
#define I_FMASK         _imgTab[2]
#define I_FAIL          _imgTab[3]
#define I_VICTORY_BG    _imgTab[4]
#define I_IHM           _imgTab[5]
#define I_IHM_SUBMIT    _imgTab[7]
#define I_HALO          _imgTab[8]
#define I_HALO_SUBMIT   _imgTab[9]
#define I_HALO_SIMPLE   _imgTab[10]
#define I_BG            _imgTab[11]
#define I_LAST          12

int EMiniJeuBase::_nX = MINIJEU_ZONEX_DEFAULT;
int EMiniJeuBase::_nY = MINIJEU_ZONEY_DEFAULT;
int EMiniJeuBase::_nW = MINIJEU_ZONEWIDTH_DEFAULT;
int EMiniJeuBase::_nH = MINIJEU_ZONEHEIGHT_DEFAULT;
KGame *EMiniJeuBase::_lpMyGame = NULL;
EFont *EMiniJeuBase::_lpFontButtons = NULL;

KGraphic* GenerateImg(EFont *font, const char* text, int thickness, float blend, float tr, float tg, float tb, float ta, float ored, float og, float ob, float oa);

EMiniJeuBase::EMiniJeuBase(void)
{
   //cleanup(); 
   //init();
   
   K_LOG("MiniJeu start");
   T_TITLE = "";
   
   // Vide les buffers de chaines
   memset(_cGameNumber, 0, 3);
	
	_nGameType = MINIJEU_DIRECTGAME;
	_nGameStatus = MINIJEU_NORMAL;
	_nHintLevel = MINIJEU_NORMAL;
	_nNumberOfVariation = 1;

	// Réinitialisation systématique des paramètre de position et de zone par défaut
	_nX = MINIJEU_ZONEX_DEFAULT;
	_nY = MINIJEU_ZONEY_DEFAULT;
	_nW = MINIJEU_ZONEWIDTH_DEFAULT;
	_nH = MINIJEU_ZONEHEIGHT_DEFAULT;

	_nXImgBg = 0;
	_nYImgBg = 0;

	Move(0,0);

	_lpAnim = NULL;
	_lpSoundFinal = NULL;
	I_BG = NULL;
	
	_bSimpleUI = false;
	_bHoldOffShowEnd = true;
	_bHOSEState = true;
	_bShowRules = true;
	_bExitOnFail = false;
	_bLastSubmit = true;

	_bQueryHint1Show = true;
	_bQueryHint2Show = true;

	_fSplashTimeOut = 8000;
	_fTimeoutHoldOff = 3000;
	_fTimeoutEndAnim = 3000;
	
	_nFontRuleSize = 17;
   _lpRulesBox = new EBox(284,663,466,88,0,0,0,0);
	_lpRulesBox->SetVerticalPolicy(EBox::VCENTER);
	_lpTextRulesBox = new ETextBox("",NULL);
	_lpRulesBox->AddChild(_lpTextRulesBox);

	_lpImgIHM = I_IHM;
	
	_lpImgHint1 = new EMiniJeuPart(EImageBank::getImage("mg_hint2","png"));
	_lpImgHint2 = new EMiniJeuPart(EImageBank::getImage("mg_hint3","png"));
	_lpImgHint1->Move(0,561);
	_lpImgHint2->Move(0,561);
	_lpImgHint1->Hide();
	_lpImgHint2->Hide();

	_lpSoundSuccess = ESoundBank::getSound("mg_success");
	_lpSoundFail = ESoundBank::getSound("ghostappear");			
}

EMiniJeuBase::~EMiniJeuBase(void)
{
	XDELETE(_lpImgHint1);
	XDELETE(_lpImgHint2);
	XDELETE(_lpRulesBox);
   XDELETE(_lpAnim);
   Move(0,0);

   K_LOG("MiniJeu stop: %s", T_TITLE);
}

void EMiniJeuBase::GenerateCacheFailSuccess() {
   XDELETE(I_VMASK);
   XDELETE(I_FMASK);
   XDELETE(I_VICTORY);
   XDELETE(I_FAIL);

  // Creation des element pour l'affichage du success
   EGLL();
   EFont *_lpFont = EFontBank::getFont("NITECLUB.TTF",100);
   EGLR();
   I_VMASK = GenerateImg(_lpFont,T_SUCCES,2,1,1,1,1,0,1,1,1,1);
   I_FMASK = GenerateImg(_lpFont,T_FAIL,2,1,1,1,1,0,1,1,1,1);
   XDELETE(_lpFont);

   I_VICTORY = KPTK::createKGraphic();
   I_VICTORY->makeRenderTarget(I_VMASK->getWidth(),I_VMASK->getHeight(),true,true);
   I_FAIL = KPTK::createKGraphic();
   I_FAIL->makeRenderTarget(I_FMASK->getWidth(),I_FMASK->getHeight(),true,true);
}

void EMiniJeuBase::init(void)
{
   if (_lpMyGame != NULL)
      return;

   _lpMyGame = MyGame::getGame();

   T_EXIT         = KStr("MINIGAME_EXIT");
   T_SUBMIT       = KStr("MINIGAME_SUBMIT");
   T_RESET        = KStr("MINIGAME_RESET");
   T_SKIP         = KStr("MINIGAME_SKIP");
   T_HELP         = KStr("MINIGAME_HELP");
   T_TITLE_PREFIX = KStr("MINIGAME_TITLE_PREFIX");
   T_SUCCES       = KStr("MINIGAME_SUCCES");
   T_FAIL         = KStr("MINIGAME_FAIL");

   I_IHM          = EImageBank::getImage("mg_ihm","png");
   I_IHM_SUBMIT   = EImageBank::getImage("mg_ihm_submit","png");
   I_HALO  = EImageBank::getImage("mg_ihm_halo","png");
   I_HALO_SUBMIT  = EImageBank::getImage("mg_ihm_submit_halo","png");
   I_HALO_SIMPLE  = I_HALO_SUBMIT;
   I_VICTORY_BG   = EImageBank::getImage("mg_victory_bg.jpg");

   GenerateCacheFailSuccess();

   _pEmitterv = new ParticleEmitter(0, 0);  
   _pEmitterv->SetParticleBitmap("particule-etoile2");
   _pEmitterv->SetHeightEffect(I_VMASK->getHeight()*2.5);
   _pEmitterv->SetEmitRate(3);
   _pEmitterv->SetType(VFX_FLAKE);
   _pEmitterv->AddSegment(0, I_VMASK->getHeight(), I_VMASK->getWidth(), I_VMASK->getHeight(), 2);  /* create segment where emitter will move */   
   _pEmitterv->Start();

   _pEmitterf = new ParticleEmitter(0, 0);  
   _pEmitterf->SetParticleBitmap("particule-etoile2");
   _pEmitterf->SetHeightEffect(I_FMASK->getHeight()*2.5);
   _pEmitterf->SetEmitRate(3);
   _pEmitterf->SetType(VFX_FLAKE);
   _pEmitterf->AddSegment(0, I_FMASK->getHeight(), I_FMASK->getWidth(), I_FMASK->getHeight(), 2);  /* create segment where emitter will move */   
   _pEmitterf->Start();

  	_lpFontButtons = EFontBank::getFont("NITECLUB.TTF",32);
	_lpFontButtons->setColor(0, 0, 0, .9f);
}
void EMiniJeuBase::cleanup(void)
{
   // clean up success element
   _lpMyGame = NULL;
   XDELETE(I_VMASK);
   XDELETE(I_VICTORY);
   XDELETE(I_FMASK);
   XDELETE(I_FAIL);
   XDELETE(_pEmitterv);
   XDELETE(_pEmitterf);
   XDELETE(_lpFontButtons);
}

void EMiniJeuBase::Check () {
   if (!Mouse.Event())
      return;

	Mouse.SetRelativePos(_nX,_nY);
	if (_nGameStatus == MINIJEU_PREAMBUL) {
		GameCheckPreambul();
	}

	if (_nGameStatus >= MINIJEU_RESOLVED) {
		return;
	}
	GameCheck();
	Mouse.UnsetRelativePos();
}
void EMiniJeuBase::Logic () {
	if (_nGameStatus == MINIJEU_PREAMBUL)
		return;
   if (_nGameStatus == MINIJEU_RESOLVED) {
      _pEmitterv->Logic();
   }
   if (_nGameStatus == MINIJEU_FAILED) {
      _pEmitterf->Logic();
   }
	Mouse.SetRelativePos(_nX,_nY);
	GameLogic();
	Mouse.UnsetRelativePos();
}
void EMiniJeuBase::Draw () {
	DrawBG();
   
   SetRelWorldView();
	GameDrawSet();
	ResetWorldView();
	if (!_bSimpleUI) {
		DrawMenu();
	} else {
      I_HALO_SIMPLE->blitAlphaRectF(0,0,I_HALO_SIMPLE->getWidth(),I_HALO_SIMPLE->getHeight(),0,668);
		_lpFontButtons->drawStringCentered(T_SKIP,0,249,702,TEXTSTYLE_CENTER);
	}
	if (_nGameStatus == MINIJEU_PREAMBUL) {
		GameDrawPreambul();
	}
	
	SetRelWorldView();
	GameDrawGrab();
	ResetWorldView();
   DrawEnd();
}
bool EMiniJeuBase::IsResolved() {
	if (_nGameStatus >= MINIJEU_RESOLVED)
		return _nGameStatus == MINIJEU_RESOLVED;

	if (_nGameStatus > MINIJEU_PREAMBUL && _nGameStatus < MINIJEU_RESOLVED) {
		if (GameIsResolve()) {
			SetGameResolved();
		} else {
			if (_nGameType == MINIJEU_SUBMITGAME && _bLastSubmit)
				SetGameFailed();
		}
	}

	return _nGameStatus == MINIJEU_RESOLVED;
}
void EMiniJeuBase::Resolve() {
	if (_nGameStatus > MINIJEU_PREAMBUL && _nGameStatus < MINIJEU_RESOLVED) {
		GameResolve();
		IsResolved();
	}
}
const char* EMiniJeuBase::HintText(int nHintLevel) {
	if (_nHintLevel == MINIJEU_NORMAL && strlen(T_HINT1) > 0)
		return T_HINT1;
	if (_nHintLevel == MINIJEU_HINT1 && strlen(T_HINT2) > 0)
		return T_HINT2;
	return "";
}
const char* EMiniJeuBase::Title()   {return T_TITLE;}
const char* EMiniJeuBase::Rule()    {return T_RULE;}
const char* EMiniJeuBase::HintQuery() {
	if (_nHintLevel == MINIJEU_NORMAL && strlen(T_QUERY1) > 0)
		return T_QUERY1;
	if (_nHintLevel == MINIJEU_HINT1 && strlen(T_QUERY2) > 0)
		return T_QUERY2;
	return KStr("MINIGAME_HINT_QUERY_DEFAULT");
}
void EMiniJeuBase::ActivateHint(int nHintLevel) {
	if (nHintLevel <= _nHintLevel || _nGameStatus >= MINIJEU_RESOLVED)
		return;

	switch (nHintLevel) {
		case MINIJEU_HINT1:
			_lpImgHint1->FadIn(300,true);
			break;
		case MINIJEU_HINT2:
			_lpImgHint2->FadIn(300,true);
			break;
		case MINIJEU_RESOLVED:	
			break;
		default:
			break;
	}
	_nHintLevel = nHintLevel;
	GameSetHint(nHintLevel);
}
void EMiniJeuBase::SetGameType(int nGameType) {
	_nGameType=nGameType;
	if (_nGameType == MINIJEU_SUBMITGAME) {
		_lpImgIHM = I_IHM_SUBMIT;
		_lpRulesBox->SetSize(560,83);
		_lpRulesBox->Move(252,677);
	} else {
		_lpImgIHM = I_IHM;
		_lpRulesBox->SetSize(466,88);
		_lpRulesBox->Move(284,663);
	}
}
void EMiniJeuBase::SetTextsTrad(const char* cGameTextRef) {
	const char *tmp =KStr((std::string("CAPTION_") + cGameTextRef).c_str());
	SetTitle(tmp); KAssert(tmp != NULL);

	tmp =KStr((std::string("RULES_") + cGameTextRef).c_str());
	SetRule(tmp); KAssert(tmp != NULL);

	tmp =KStr((std::string("HINT1_") + cGameTextRef).c_str()); 
	SetHintText1(tmp); KAssert(tmp != NULL);

	tmp =KStr((std::string("HINT2_") + cGameTextRef).c_str());
	SetHintText2(tmp);
	
	tmp=KStr((std::string("HINTQUERY1_") + cGameTextRef).c_str());
	if (tmp != NULL)
		SetHintQuery1(tmp);
	tmp=KStr((std::string("HINTQUERY2_") + cGameTextRef).c_str());
	if (tmp != NULL)
		SetHintQuery2(tmp);
}
void EMiniJeuBase::SetText(char* cDst, const char* cSrc) {
	KAssert(cSrc != NULL && strlen(cSrc) <= sizeof(cDst));
	strcpy(cDst,cSrc);
}

void EMiniJeuBase::SetTitle(const char* cTitle) {
	T_TITLE = cTitle;
}
void EMiniJeuBase::SetRule(const char* cRule) {
	T_RULE = cRule;
   _lpTextRulesBox->SetText(T_RULE);
	Coord pos(_lpRulesBox->GetPos() + _lpRulesBox->GetLeftRightMargin());
	Coord size(_lpRulesBox->GetInnerSize());
	EFont *_lpFont = EFontBank::getFont("tahoma.ttf",_nFontRuleSize);
   _lpFont->setColor(1,1,1,0.9f);

	float textHeight = _lpFont->getMultilineHeight(Rule(),pos.x,pos.x+size.x,pos.y, TEXTSTYLE_CENTER);
	while (textHeight > size.y) {
		_nFontRuleSize -= 1;
      XDELETE(_lpFont);
		_lpFont = EFontBank::getFont("tahoma.ttf",_nFontRuleSize);
		_lpFont->setColor(1,1,1,0.9f);
      textHeight = _lpFont->getMultilineHeight(Rule(),pos.x,pos.x+size.x,pos.y, TEXTSTYLE_CENTER);
	}
	
   _lpTextRulesBox->SetFont(_lpFont);
    XDELETE(_lpFont);
	_lpRulesBox->UpdateUI();
}
void EMiniJeuBase::SetHintText1(const char* cHint1)         {T_HINT1    = cHint1;}
void EMiniJeuBase::SetHintQuery1(const char* cHintQuery1)   {T_QUERY1   = cHintQuery1;}
void EMiniJeuBase::SetHintText2(const char* cHint2)         {T_HINT2    = cHint2;}
void EMiniJeuBase::SetHintQuery2(const char* cHintQuery2)   {T_QUERY2   = cHintQuery2;}
void EMiniJeuBase::SetHintText(const char* cHintText) {
	if (_nHintLevel == MINIJEU_HINT1) {
		SetHintText1(cHintText);
		ShowHint(_nHintLevel);
	} else if (_nHintLevel == MINIJEU_HINT2) {
		SetHintText2(cHintText);
		ShowHint(_nHintLevel);
	}
}
void EMiniJeuBase::SetFeatures(unsigned char bFeatures) {
	_bFeatures = _bFeatures | bFeatures;
}
void EMiniJeuBase::SetGameNumber(int i, int numberOfVariation) {
	sprintf(_cGameNumber,"%2d",i);
	_nGameNumber = i;
	_nNumberOfVariation = numberOfVariation;

	// Gestion de l'enregistrement de la reussite du jeux. Jeux disponible uniquement quand toutes les variations reussi.
	if(! EGlobalBank::isExist((std::string("mg_gameresolved_") + itos(i)).c_str())) {
		SetGlobal((std::string("mg_gameresolved_") + itos(i)).c_str(),itos(1-_nNumberOfVariation).c_str());
	}
}
void EMiniJeuBase::SetBackGround(KGraphic *imgBg, int x, int y) {
   I_BG=imgBg;
   _nXImgBg=x;
   _nYImgBg=y;
}	
void EMiniJeuBase::DrawBG() {
	if (I_BG != NULL)
		I_BG->blitAlphaRect(0,0,I_BG->getWidth(),I_BG->getHeight(),_nXImgBg,_nYImgBg);

}
void EMiniJeuBase::DrawMenu() {
	_lpImgIHM->blitAlphaRect(0,0,_lpImgIHM->getWidth(),_lpImgIHM->getHeight(),0,561);
	_lpImgHint1->Draw();
	_lpImgHint2->Draw();
	I_HALO->blitAlphaRect(0,0,I_HALO->getWidth(),I_HALO->getHeight(),0,579);


	if (_nGameType == MINIJEU_SUBMITGAME) {
		I_HALO_SUBMIT->blitAlphaRect(0,0,I_HALO_SUBMIT->getWidth(),I_HALO_SUBMIT->getHeight(),387,603);
	}

	_lpRulesBox->Draw();

	_lpFontButtons->drawMultiline(T_EXIT,98, 161,678 , TEXTSTYLE_CENTER);
	_lpFontButtons->drawMultiline(T_RESET,37, 96,622 , TEXTSTYLE_CENTER);
	_lpFontButtons->drawMultiline(T_HELP,916, 981,677 , TEXTSTYLE_CENTER);

	if (_nGameType == MINIJEU_SUBMITGAME) {
		_lpFontButtons->drawMultiline(T_SUBMIT, 479, 546, 634, TEXTSTYLE_CENTER);
	}
}

float _fTotFrameElapse=0;
void EMiniJeuBase::DrawSuccess() {
   _fTotFrameElapse += _lpMyGame->getKWindow()->getFrameTime();

   int maskref=0,imgref=1;
   if (_nGameStatus == MINIJEU_FAILED) {
      maskref=2;
      imgref=3;
   }

   if (_imgTab[maskref]->isRenderTargetLost())
      GenerateCacheFailSuccess();

   float mw=_imgTab[maskref]->getWidth(),mh=_imgTab[maskref]->getHeight();
   
   Coord _pSuccess = Coord((1024-mw)/2,(768-mh)/2);

   _imgTab[maskref]->setBlitColor(0,0,0,1);
   _imgTab[maskref]->blitAlphaRectFx(0,0,mw,mh,_pSuccess.x+1,_pSuccess.y+1,0,1,0.5f);
   _imgTab[maskref]->blitAlphaRectFx(0,0,mw,mh,_pSuccess.x+2,_pSuccess.y+2,0,1,0.5f);
   _imgTab[maskref]->setBlitColor(1,1,1,1);
   float titi = 0.25+0.3*sin(_fTotFrameElapse/1000*4);
   _imgTab[maskref]->blitAlphaRectFx(0,0,mw,mh,_pSuccess.x,_pSuccess.y,0,1,titi);

   _imgTab[imgref]->beginRenderToTarget();
   I_VICTORY_BG->blitAlphaRectFx(0,0,mw,mh,0,0,0,1,0.7f);
   if (maskref == 0) {
      _pEmitterv->Render(_lpMyGame->getKWindow()->getFrameTime());
   } else {
      _pEmitterf->Render(_lpMyGame->getKWindow()->getFrameTime());
   }
   _imgTab[imgref]->endRenderToTarget();

   _imgTab[imgref]->blitAlphaRectMaskedF(0,0,mw,mh,_imgTab[maskref],
      0,0,mw,mh,_pSuccess.x,_pSuccess.y,_pSuccess.x+mw,_pSuccess.y+mh);
}
void EMiniJeuBase::ShowHint(int nHintLevel) {
	if (_nHintLevel > MINIJEU_HINT2 || _nGameStatus >= MINIJEU_RESOLVED)
		return;

	if (!SCENEDIRECTOR->isDialogboxActive()) {
   	const char *tmp = T_HINT1;
		if (nHintLevel == MINIJEU_HINT1 && _nHintLevel >= nHintLevel) {
			if (strlen(T_HINT1) == 0) {
				if (_bQueryHint1Show == false ||  strlen(T_QUERY1) == 0) {
               ESoundBank::getSound("mg_forbid")->playSample();
					return;
				}
				tmp = T_QUERY1;
				_bQueryHint1Show = false;
			}
			SCENEDIRECTOR->OpenDialogboxSpirit(tmp);
         return;
		}
		tmp = T_HINT2;
		if (nHintLevel == MINIJEU_HINT2 && _nHintLevel >= nHintLevel) {
			if (strlen(T_HINT2) == 0) {
				if (_bQueryHint2Show == false || strlen(T_QUERY2) == 0) {
					ESoundBank::getSound("mg_forbid")->playSample();
               return;
				}
				tmp = T_QUERY2;
				_bQueryHint2Show = false;
			}
			SCENEDIRECTOR->OpenDialogboxSpirit(tmp);
         return;
		}
	}
   ESoundBank::getSound("mg_forbid")->playSample();
}
void EMiniJeuBase::DrawEnd() {
   if (_nGameStatus >= MINIJEU_RESOLVED) {
      if (_lpAnim == NULL)
         return;

      double fElapsed = _lpMyGame->getKWindow()->getFrameTime();
      _lpAnim->move(fElapsed);
      DrawSuccess();
   }
}

void EMiniJeuBase::AnimateEnding() {
	if (_lpAnim != NULL)
		return;
}

bool EMiniJeuBase::IsShowingEnd() {
   if ((_nGameStatus >= MINIJEU_RESOLVED && (_bHOSEState || _lpAnim != NULL)))
      return true;
   return false;
}

void EMiniJeuBase::Reset() {
   if (_nGameStatus == MINIJEU_RESOLVED || IsShowingEnd()) {
      return;
   }

	_lpSoundFinal = NULL;
	_bHOSEState = _bHoldOffShowEnd;
	GameReset();

	if (_nGameStatus > MINIJEU_PREAMBUL)
		_nGameStatus = MINIJEU_NORMAL;
}

// Timer de fin de jeux a implémenté
bool EMiniJeuBase::End() {
	if (_nGameStatus < MINIJEU_RESOLVED || (!_bHOSEState && ! (_lpAnim == NULL ||  _lpAnim->isCompleted())))
		return false;

	// Gestion de la fin de timeout
	if (_lpAnim != NULL) {
		if ( _lpAnim->isCompleted() == false) { // Un timeout es en court
			return false;
		} else if (_bHOSEState) { // Le timeout holdof es fini
			_bHOSEState = false;
			XDELETE(_lpAnim);
		} else { // Le Timeout 
			XDELETE(_lpAnim);
			if (_nGameStatus == MINIJEU_FAILED && !_bExitOnFail) {
				Reset();
				return false;
			}
		}
	}

	// Gestion du setting du timeout et de l'animation de fin
	if (_lpAnim == NULL) {
		if (_nGameStatus == MINIJEU_SKIPED || _bHoldOffShowEnd == false)
			_bHOSEState = false;
		
		// Play end sound
		if (_lpSoundFinal == NULL) {
			switch (_nGameStatus) {
			case MINIJEU_RESOLVED:
				_lpSoundFinal = _lpSoundSuccess;
				break;
			case MINIJEU_FAILED:
				_lpSoundFinal = _lpSoundFail;
				break;
			case MINIJEU_SKIPED:
				_lpSoundFinal = NULL;
				break;
			default:
				break;
			}
			if (_lpSoundFinal != NULL)
				_lpSoundFinal->playSample();

         if (_nGameStatus == MINIJEU_RESOLVED || (_nGameStatus == MINIJEU_FAILED && _bExitOnFail))
            SCENEDIRECTOR->ChangeMusic(DIRECTOR_MUSIC_NONE);
		}

		if (_bHOSEState ) { // Timer pour le display de la solution
			XDELETE(_lpAnim);
			_lpAnim = new KCounter;
			_lpAnim->startCounter(1,0,0,_fTimeoutHoldOff,K_COUNTER_LINEAR);
			return false;
		}
	}

   if (_nGameStatus == MINIJEU_FAILED && !_bExitOnFail) {
				Reset();
				return false;
	}

	return _nGameStatus >= MINIJEU_RESOLVED;
}

void EMiniJeuBase::SetHoldEndTimeout(float _fTimeout) {
	if (_fTimeout == 0) {
		_bHoldOffShowEnd = false;
	} else {
		_bHoldOffShowEnd = true;
	}
	_fTimeoutHoldOff = _fTimeout;
}

float EMiniJeuBase::GetHoldEndTimeout() {
	return _fTimeoutHoldOff;
}

void EMiniJeuBase::SetSoundSuccess(KSound* sound) {
	_lpSoundSuccess = sound;
}
void EMiniJeuBase::SetSoundFail(KSound* sound) {
	_lpSoundFail = sound;
}

void EMiniJeuBase::SetRelWorldView() {
	_lpMyGame->_lpKWindow->setWorldView((float)_nX,(float)-_nY,0,1,false);
	Mouse.SetRelativePos(_nX,_nY);
}

void EMiniJeuBase::ResetWorldView() {
	Mouse.UnsetRelativePos();
	_lpMyGame->_lpKWindow->setWorldView(0,0,0,1,false);
}

// Fonctions défaut des jeux
void EMiniJeuBase::SetGameResolved() {
   _nGameStatus = MINIJEU_RESOLVED;
   int cvalue = GetGlobal((std::string("mg_gameresolved_")+itos(_nGameNumber)).c_str(),0);
   if (cvalue < 1)
      SetGlobal((std::string("mg_gameresolved_")+itos(_nGameNumber)).c_str(),itos(cvalue+1).c_str());		 
}
void EMiniJeuBase::SetGameFailed(){
	_nGameStatus = MINIJEU_FAILED;
	}
void EMiniJeuBase::GameDrawPreambul() {
}
void EMiniJeuBase::GameCheckPreambul() {
	_nGameStatus = MINIJEU_NORMAL;
	Mouse.EventClose();
}

// Preload images et sons
void EMiniJeuBase::Preload() {
   if (_bPreloadDone)
      return;

   EImageBank::getImage("mg_ihm","png",true,true);
   EImageBank::getImage("mg_ihm_halo","png",true,true);
   EImageBank::getImage("mg_ihm_submit_halo","png",true,true);
   EImageBank::getImage("mg_ihm_submit_halo","png",true,true);
   EImageBank::getImage("mg_hint2","png",true,true);
   EImageBank::getImage("mg_hint3","png",true,true);
   EImageBank::getImage("mg_ihm_submit","png",true,true);
   EImageBank::getImage("mg_ihm","png",true,true);
   EImageBank::getImage("minijeu_success","png",true,true);
   EImageBank::getImage("minijeu_fail","png",true,true);

   ESoundBank::getSound("mg_success",false,true);
   ESoundBank::getSound("ghostappear",false,true);

   // from EMiniJeuHelper
   EImageBank::getImage("mg_plant_addition_plus","png",true,true);
   EImageBank::getImage("mg_plant_addition_minus","png",true,true);

   ESoundBank::getSound("mg_digit",false,true);

   // from EMiniJeuHintDlg
   EImageBank::getImage("dialogbox.png","",true,true);
   EImageBank::getImage("dialogbox_halo.png","",true,true);

   ESoundBank::getSound("dialogshow",false,true);

   // son generic
   ESoundBank::getSound("mg_swap",false,true);
   ESoundBank::getSound("mg_forbid",false,true);
   ESoundBank::getSound("mg_select_deselect",false,true);
   ESoundBank::getSound("mg_success",false,true);

   _bPreloadDone = true;
}

KGraphic* GenerateImg(EFont *font, const char* text, int thickness, float blend, 
                   float tr, float tg, float tb, float ta,
                   float ored, float og, float ob, float oa) {
   KGraphic *target = KPTK::createKGraphic();
   target->setTextureQuality(true);
   target->setTextureColorPriority(true);

   float width,height;
   width = font->getMultilineWidth(text,0,1204,0,TEXTSTYLE_LEFT);                   
   height = font->getMultilineHeight(text,0,1204,0,TEXTSTYLE_LEFT);

   target->makeRenderTarget(width+2*thickness,height+2*thickness,true,true);
   target->beginRenderToTarget();

   font->setColor (ored,og,ob,oa*blend);
   float fX, fY, _x = thickness, _y=thickness;
   for (fY = -thickness; fY <= thickness; fY++) {
      for (fX = -thickness; fX <= thickness; fX++) {
         if (fX < 0 || fY < 0 || fX > 0 || fY > 0)
            font->drawMultiline(text,_x + fX,_x+width,_y + fY,TEXTSTYLE_LEFT,0);
      }
   }

   font->setColor (tr,tg,tb,ta*blend);
   font->drawMultiline(text,thickness,width,thickness,TEXTSTYLE_LEFT);
   target->endRenderToTarget();
   return target;
}

bool EMiniJeuBase::_bPreloadDone = false;

ParticleEmitter* EMiniJeuBase::_pEmitterv = NULL;
ParticleEmitter* EMiniJeuBase::_pEmitterf = NULL;

const char* EMiniJeuBase::_textTab[T_LAST];
KGraphic* EMiniJeuBase::_imgTab[I_LAST];
Coord  EMiniJeuBase::_imgTabPos[5];
