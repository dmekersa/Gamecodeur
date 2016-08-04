#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EmitterBank.h"
#include "EPointer.h"
#include "FontBank.h"
#include "EGeometry.h"
#include "ESceneDirector.h"

#include "EMiniJeuPoltergeist.h"

EMiniJeuPoltergeist::EMiniJeuPoltergeist(void)
{
	SetTextsTrad("FLACONS");
	SetGameType(MINIJEU_DIRECTGAME);
	SetSoundSuccess(ESoundBank::getSound("poltergeistgone"));
   SetGameNumber(50);

	// Grab the background scene
	_lpSceneBG = KPTK::createKGraphic();
   if (SCENEDIRECTOR->GetCurrentScene() != NULL) {
      _lpSceneBG->makeRenderTarget(1024,768,true,true);
      _lpSceneBG->beginRenderToTarget();
      SCENEDIRECTOR->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_NONE);

      char *scnName = SCENEDIRECTOR->getCurrentSceneName();
      if (strcmp(scnName,"kitchen") == 0) {
         SCENEDIRECTOR->GetCurrentScene()->StopEmitter("creeping_polt");
         SCENEDIRECTOR->GetCurrentScene()->SetVisible("kitchen_polt",false,true);
      } else if (strcmp(scnName,"house_livingroom") == 0) {
         SCENEDIRECTOR->GetCurrentScene()->SetVisible("livingroom_poltergeist",false,true);
      }  else if (strcmp(scnName,"house_room2") == 0) {
         SCENEDIRECTOR->GetCurrentScene()->StopEmitter("creeping_priest_bible");
         SCENEDIRECTOR->GetCurrentScene()->SetVisible("house_room2_poltergeist",false,true);
         SCENEDIRECTOR->GetCurrentScene()->SetVisible("house_room2_priest_underattack",false,true);
      }
      SCENEDIRECTOR->GetCurrentScene()->Draw();
      EImageBank::getImage("mg_poltergeist_bg.jpg")->blitAlphaRectFx(0,0,1024,768,0,0,0,1,0.3f);
      
      SCENEDIRECTOR->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
      _lpSceneBG->endRenderToTarget();
      SetBackGround(_lpSceneBG);
   }

	_lpImgBg = EImageBank::getImage("emastermind_bg","jpg");
	_nW = _nW + _nX;
	_nH = _nH + _nY;
	_nY = 0;
	_nX = 0;
 
	_lpImgAshley = new ESmartImage("ashley_poltergeist_anim",300,400,true,true,1500,true,12);
	_lpImgAshley->SetVisible(true,true);
	_lpImgAshley->StartVideo();
	
	_lpBigBadPol = EImageBank::getImage("poltergeistfinal","png");

	_lpVideoPol = new ESmartImage("poltergeist3",0,0,false,true,1500,true,12);
	_lpVideoPol->StartVideo();

	_lpPoltergeist = new EPoltergeist(_lpVideoPol);
	_lpPoltergeist->Hide();
	_lpPoltergeist->FadIn(1500);


	_lpImgLifeBox = EImageBank::getImage("mg_poltergeist_jauge_empty","png");
	_lpImgLife = EImageBank::getImage("mg_poltergeist_jauge","png");
	_posLifeBox = Coord((int)((_nW - _lpImgLifeBox->getWidth())/2),0);

	_lpCounterDisplayPol = new KCounter();
	_lpCounterDisplayPol->startCounter(0,1,0,3500,K_COUNTER_LINEAR);
	_lpCounter = NULL;

	_nMaxDegats = 15;
	/* Display de la force du coup
	_nDegats = 0;
	_nStartDegats = 2;
	_lpCounterDegats = new KCounter();
	_lpCounterDegats->startCounter(0,_nStartDegats,0,0,K_COUNTER_LINEAR);
	*/
   _lpEmitterHit = EEmitterBank::getEmitter("poltergeisthit","particles/sparkles.cse",NULL);
   
	_lpImgAshley->SetVisible(false,false);
	_lpImgAshley->ForceVisible(false);
	_lpImgAshley->SetVisible(true,true);
	_bShowImg = false;

	_lpFont = EFontBank::getFont("jancient22",-18);
	
	memset(_cHintPoltergeist,0,sizeof(_cHintPoltergeist));
	strcpy(_cHintPoltergeist,KStr("RULE_POLTERGEISTMG"));
	Reset();
}

EMiniJeuPoltergeist::~EMiniJeuPoltergeist(void)
{
	_lpEmitterHit->Stop();
	XDELETE(_lpVideoPol);
	//XDELETE(_lpEmitterHit);
	XDELETE(_lpCounterDisplayPol);
	XDELETE(_lpPoltergeist);
	XDELETE(_lpCounter);
	XDELETE(_lpImgAshley);
	XDELETE(_lpSceneBG);
   XDELETE(_lpFont);
	// Display de la force du coup
	// XDELETE(_lpCounterDegats);
}

/** 3 steps rendering **/
void EMiniJeuPoltergeist::GameCheck ()
{
	// Avec Gestion de la force du coup en fonction de la longueur de l'appui
	//if (Mouse.LeftEvent()) {
	//	_lpCounterDegats->startCounter(_lpCounterDegats->getCurrentValue(),_nMaxDegats,0,2000*(1-(_nDegats-_nStartDegats)/(_nMaxDegats-_nStartDegats)),K_COUNTER_LINEAR);
	//	Mouse.LeftEventClose();
	//}
	//if (Mouse.LeftReleaseEvent()) {
	//	float time=1000;
	//	if ( _lpPoltergeist->IsMouseOver()) {
	//		_lpPoltergeist->HitPol(_lpCounterDegats->getCurrentValue());
	//		ESoundBank::getSound("poltergeistgone")->playSample();
	//		time = 0;
	//	}
	//	_lpCounterDegats->startCounter(_lpCounterDegats->getCurrentValue(),_nStartDegats,0,time*((float)(_nDegats-_nStartDegats)/(float)(_nMaxDegats-_nStartDegats)),K_COUNTER_LINEAR);
	//	Mouse.LeftEventClose();
	//}

	if (!Mouse.LeftEvent() || !(Mouse.x>0 && Mouse.y>0 && Mouse.x< 1024 && Mouse.y<768))
		return;

	ESoundBank::getSound("spirit_gun")->playSample();

	// Le poltergeist es touché
	_lpEmitterHit->Move(Mouse.x,Mouse.y);
	_lpEmitterHit->Start();
	if (!_lpPoltergeist->IsMouseOver())
		return;
	_lpPoltergeist->HitPol(_nMaxDegats);
	ESoundBank::getSound("poltergeist_hit")->playSample();
	Mouse.LeftEventClose();
}

void EMiniJeuPoltergeist::GameLogic()
{
	_lpImgAshley->Logic();
	_lpPoltergeist->Logic();
	_lpPoltergeist->Check();
	_lpEmitterHit->Logic();

	IsResolved();
}

void EMiniJeuPoltergeist::GameDrawSet()
{
	float _fElapsedTime = MyGame::getGame()->getElapsedTime();

	//_lpImgBg->blitAlphaRectF(0,0,1024,768,0,0);

	if (_lpCounterDisplayPol != NULL) {
		if (_lpCounterDisplayPol->isCompleted()) {
			_lpPoltergeist->FadIn(1500);
			XDELETE(_lpCounterDisplayPol);
		} else {
			_lpCounterDisplayPol->move(_fElapsedTime);
		}
	}

	/* Display de la force du coup
	if (!_lpCounterDegats->isCompleted()) {
		_lpCounterDegats->move(_fElapsedTime);
	}
	_nDegats = (int)_lpCounterDegats->getCurrentValue();*/

	if (_lpPoltergeist->PV() > 0) {
		_lpImgLifeBox->blitAlphaRect(0,0,_lpImgLifeBox->getWidth(),_lpImgLifeBox->getHeight(),_posLifeBox.x,_posLifeBox.y);
		_lpImgLife->blitAlphaRect(0,0,_lpImgLife->getWidth()*_lpPoltergeist->PV(),_lpImgLife->getHeight(),_posLifeBox.x,_posLifeBox.y);
	}
	if (_nGameStatus != MINIJEU_RESOLVED) {
		_lpPoltergeist->DrawBack();
		_lpImgAshley->Draw();
		_lpPoltergeist->DrawFront();
	}
	// Display de la force du coup
	//if (_lpPoltergeist->PV() > 0) {
	//	_lpImgLifeBox->blitAlphaRect(0,0,_lpImgLifeBox->getWidth(),_lpImgLifeBox->getHeight(),_posLifeBox.x,768 - _lpImgLifeBox->getHeight());
	//	_lpImgLife->blitAlphaRect(0,0,_lpImgLife->getWidth()*_lpCounterDegats->getCurrentValue()/_nMaxDegats,_lpImgLife->getHeight(),_posLifeBox.x+5,768 - _lpImgLifeBox->getHeight()+3);
	//}
   _lpFont->drawStringCentered(_cHintPoltergeist,0,1024,738);
	ShowImg();
	_lpEmitterHit->Render();
}

void EMiniJeuPoltergeist::GameReset() {
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuPoltergeist::GameIsResolve()
{
	if (_lpPoltergeist->IsDead()) {
		_nGameStatus = MINIJEU_RESOLVED;
		_lpCounter = new KCounter();
		_lpCounter->startCounter(0,1,0,1500,K_COUNTER_LINEAR);
		_bShowImg = true;
		return true;
	}
	return false;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuPoltergeist::GameResolve()
{
	_lpPoltergeist->KillPol();
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuPoltergeist::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
		case MINIJEU_RESOLVED:
			Resolve();
			break;
		default:
			KAssert(false);
			break;
	}
}

void EMiniJeuPoltergeist::ShowImg() {
	if (!_bShowImg)
		return;
	
	if (!_lpCounter->isCompleted())
		_lpCounter->move(MyGame::getGame()->getKWindow()->getFrameTime());

	float _fAlpha;
	if (_lpCounter->getCurrentValue() > 0.5) {
		_fAlpha = 1 - (_lpCounter->getCurrentValue() - 0.5)*2;
	} else {
		_fAlpha = _lpCounter->getCurrentValue()*2;
	}

	float pw = _lpBigBadPol->getWidth(), ph = _lpBigBadPol->getHeight();

	float scale = 0.5+(_lpCounter->getCurrentValue()*5);
	_lpBigBadPol->blitAlphaRectFx(0,0, pw,ph,(_nW - pw)/2, (_nH - ph -10)/2,
		0,scale,_fAlpha);
}

EPoltergeist::EPoltergeist(ESmartImage *sPol): EMiniJeuPart(sPol->getGraphic(),1,0,0,0,1) {
	_bSkipDraw=true;
	_fAngle = 0;

	_nXforbid=300;
	_nYforbid=400;
	_nWforbid=500;
	_nHforbid=500;
	
	_fBaseBlend = 0.5;
	_fDeltaBlend = 0.5;
	SetMaxBlend(_fBaseBlend+_fDeltaBlend);

	_nMaxPV = 50;
	_prevPV =_nPV = _nMaxPV;
	_lpCounterPV = new KCounter();
	_lpCounterPV->startCounter(0,_nPV,0,0,K_COUNTER_LINEAR);
   _lpCounterPV->move(1);
	_bHit = false;

	_fHealTime = 200;
	_lpCounterHeal = new KCounter();

	_lpImgPol = sPol;
	
	_fScale = 1;
	_bFlip = false;

	_nOldZone = _nCurrentZone = EMiniGame::GetRandom(_nImagePol);
	RandomMove();
	FinishMove();
	SetDrawParm();

	_lpImg = _lpImgPol->getGraphic();
	
	_bSwitchZone = false;
	
	//_lpEmitterTail = new CreepingSmokeEmitter();
	//_lpEmitterTail->SetWindow(MyGame::getGame()->getKWindow());
	//_lpEmitterTail->ReadPreset(KMiscTools::makeFilePath("particles/poltergeist_tail.cse"));
	//_lpEmitterTail->Start();
}
EPoltergeist::~EPoltergeist(void) {
	_lpImgPol = NULL;
	
	//_lpEmitterTail->Stop();
	//XDELETE(_lpEmitterTail);
	XDELETE(_lpCounterPV);
	XDELETE(_lpCounterHeal);
}
void EPoltergeist::Logic() {
	_lpImgPol->Logic();
}
void EPoltergeist::DrawBack() {
	if (!_bFromBehind)
		return;
	Draw();
}
void EPoltergeist::DrawFront() {
	if (_bFromBehind)
		return;
	Draw();
}
void EPoltergeist::Draw() {
	double _fElapsedTime = MyGame::getGame()->getElapsedTime();
	if (!_lpCounterPV->isCompleted()) {
		_lpCounterPV->move(_fElapsedTime);
		SetMaxBlend(_fBaseBlend + _fDeltaBlend*PV());
	} else if (_bHit) {
		_bHit = false;
		_lpCounterHeal->startCounter(0,1,0,_fHealTime,K_COUNTER_LINEAR);
	} else if(_lpCounterHeal->isCompleted()) {
		if (_nPV < _nMaxPV && _nPV > 0)
			HitPol(-2);
	} else {
		_lpCounterHeal->move(_fElapsedTime);
	}
	if (MoveComplete())
		RandomMove();
	
	// smoke
	//_lpEmitterTail->Move(Center().x,Center().y);
	//_lpEmitterTail->_fOpacity = _fBlend;
	//_lpEmitterTail->Render();

	// ghost
	
	if (_bSwitchZone) {
		if (GetCurrentZone() == _nCurrentZone) {
			_nOldZone = _nCurrentZone;
			_bSwitchZone = false;
			SetDrawParm();
		}
	}

	_lpImgPol->MoveFrames(_fElapsedTime);
	_lpImg = _lpImgPol->getGraphic();
	EMiniJeuPart::Draw();
	_lpImg->blitAlphaRectFxF(0, 0, _w, _h, _x, _y, _fAngle, 1, _fBlend,_bFlip);
}

void EPoltergeist::HitPol(int nPV) {
	if (_lpCounterPV->getCurrentValue() > (float)_nPV)
		return;

	_prevPV = _nPV;
	_nPV -= nPV;
	if (_nPV > _nMaxPV)
		_nPV = _nMaxPV;
	_lpCounterPV->startCounter(_lpCounterPV->getCurrentValue(),_nPV,0,1000,K_COUNTER_LINEAR);

	_bHit = true;

	if (_nPV <= 0) {
		KillPol();
	} else if (_prevPV> _nPV) {
		RandomZoneMove();
	}
}
float EPoltergeist::PV() {
	float tmp = _lpCounterPV->getCurrentValue()/(float)_nMaxPV;
	if (tmp > 0)
		return tmp;
	return 0;
}
void EPoltergeist::RandomZoneMove() {
	StopMove();

	_bFromBehind = false;
	if ((int)(_nCurrentZone/_nNumZoneX) == 0)
		_bFromBehind = true;
	
	_nOldZone =  _nCurrentZone;
	while (_nOldZone == _nCurrentZone)
		_nCurrentZone = EMiniGame::GetRandom(_nImagePol);

	_bSwitchZone = true;
	RandomMove();
}
void EPoltergeist::RandomMove() {
	Coord zone,pos,posScaled;
	
	posScaled = Coord((_w / _fScale) / 2, (_h /_fScale) /2);
	
	zone = Coord((int)(1024/_nNumZoneX),(int)(768/_nNumZoneY));
	pos.x = (_nCurrentZone % _nNumZoneX)*zone.x + _w*_fScale + EMiniGame::GetRandom(zone.x- 2*(_w*_fScale)) - _w/2;
	pos.y = (_nCurrentZone / _nNumZoneX)*zone.y + _h*_fScale + EMiniGame::GetRandom(zone.y- 2*(_h*_fScale)) - _h/2;

	Move(pos.x,pos.y,1000,false,K_COUNTER_EASEINOUT);

	_bFromBehind = false;
	if ((int)(_nCurrentZone/_nNumZoneX) == 0)
		_bFromBehind = true;
}

int EPoltergeist::GetCurrentZone() {
	Coord pos = Center();
	pos.y = (int)((pos.y / 768) * _nNumZoneY);
	pos.x =(int)((pos.x /1024) * _nNumZoneX);

	return pos.y * _nNumZoneX + pos.x; 
}

void EPoltergeist::KillPol() {
	HitPol(_nMaxPV*2);
	FadOut(500);
}

KVideo* EPoltergeist::GetFinalVideo() {
	return EImageBank::getVideo((_sImageNameBase + "_last"+".ogv").c_str(),false);
}


double interpolLagrange(std::vector< float > *X, std::vector< float > *Y, float x) {
	KAssert(X->size() == Y->size());
	double interp=0;
	int n = X->size();

	for ( int i=0; i<= n; i++) {
		double temp = 0;
		for (int j=0; j<=n; j++) {
			if (j!=i)
				temp= temp * (x - (*X)[i]) / ((*X)[j] - (*X)[i]);
		}
		interp= interp + (*Y)[i]*temp;
	}
	return interp;
}
void EPoltergeist::SetDrawParm() {
	_bFlip = (bool)(_nCurrentZone % 2);
	_fAngle = 80-80*((int)(_nCurrentZone/2));
	if (!_bFlip)
		_fAngle = -80+80*((int)(_nCurrentZone/2));
}

// Preload images et sons
void EMiniJeuPoltergeist::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::preLoadAnim("poltergeist3");
   EImageBank::preLoadAnim("ashley_poltergeist_anim");
   EImageBank::getImage("mg_poltergeist_bg.jpg","",true,true);
   EImageBank::getImage("poltergeistfinal","png",true,true);
   EImageBank::getImage("mg_poltergeist_jauge_empty","png",true,true);
   EImageBank::getImage("mg_poltergeist_jauge","png",true,true);

   ESoundBank::getSound("poltergeistgone",false,true);
   ESoundBank::getSound("spirit_gun",false,true);
   ESoundBank::getSound("poltergeist_hit",false,true);
}
