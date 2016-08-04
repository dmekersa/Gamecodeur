/*
 *  CreepingSmokeEmitter.cpp
 *
 *  Created by Francois Desruelles | TreeX on 02/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 *	Version : 1.2.1 | 26/03/2011
 *
 */

#include "global.h"
#include "stdio.h"
#include "math.h"
#ifdef K_WIN32
	#include "atlstr.h"
#else
	#include "string.h"
#endif

#include "CreepingSmoke.h"

#ifndef FRANCOIS
#include "ImageBank.h"
#include "ThreadLoader.h"
#include "ESceneParser.h"
#endif

int CreepingSmokeEmitter::particlesCount = 0;
float CreepingSmokeEmitter::rateFactor =1.0f;

CreepingSmokeEmitter::CreepingSmokeEmitter()
{
	_bUseMouse = false;
	_bUseKeys = false;

	_bStarted = false;
	_bIsActive = false;
	_bKeyState = false;

	_fPosX = 0.0f;
	_fPosY = 0.0f;
	_fAngle = 0;
	_fInheritPosX = 0.0f;
	_fInheritPosY = 0.0f;
	_bSpiral = false;
	_fSpiralRadius = 0;
	_fSpiralRadiusMin = 0;
	_fSpiralRadiusFreq = 0.01f;
	_fSpiralRadiusSpeed = 0.0f;
	_fSpiralSpeed = 0.0f;
	_fSpiralAnglePower = pi / 2;
	_fSpiralPosX = 0.0f;
	_fSpiralPosY = 0.0f;
	_fSpiralAngle = -pi / 2;
	_fSpiralTime = 0.0f;

	// emission attributes
	_fElaspedTime = 0.0f;
	_bEmit = true;
	_fTime = 0.0f;
	_nParticles = 0;
	_fRate = 10.0f;
   _fMinRate = 0.0f;
	_fRateDuration = 0.0f;
	_fSpeed = 0.0f;
	_fSpeedMin = 0.0f;
	_bSpeedStop = false;
	_bAngleRandom = false;
	_fAngleCtrl = 0.000f;
	_nSpinRandom = 0;
	
	// particles attributes
	_fLife = 1.0f;
	_fLifeMin = 10.f;
	_fMass = 1.0f;
	_fMassMin = 1.0f;
	_fInherit = 0.0f;
	_fFriction = 0.01f;
	_fTwirl = 0.0f;
	_fTwirlMin = 0.0f;
	_fTwirlMax = 0.0f;
	_fBuoyancy = 0.0f;
	_fNoise = 0.0f;
	_fNoiseFreq = 0.0f;
	_fNoiseAngleMax = hpi;
	_fVortex = 0.0f;
	_fGravity = 0.0f;
	_fWind = 0.0f;
	_fWindAngle = 0.0f;
	_fWindDelay = 0.0f;
	_bForcesFollow = false;

	// goal
	_nWindowWidth = 1024;
	_nWindowHeight = 768;
	_fGoal = 0.0f;
	_fGoalX = 512.0f;
	_fGoalY = 384.0f;

	// trails
	_fTrailLife = 0.0f;
	_nTrailChilds = 0;
	_bTrailRandom = false;
	_bTrailSpinRandom = false;
	
	// appearance attributes
	_fRed = 1.0f;
	_fGreen = 1.0f;
	_fBlue = 1.0f;
	_bTextureRate = false;
	_fTextureRate = 0.0f;
	_fTextureRandom = 1.0f;
	_fTextureLimit = 0.0f;
	_bTextureBySpeed = false;
	_bTextureRotate = false;
	_fOpacity = 1.0f;
	_bBlendByAge = false;
	_bBlendBySpeed = false;
	_fBlendFadeIn = 0;
	_bBlendFadeOut = false;
	_bBlendInvert = false;
	_bBlendSmooth = true;
	_fScale = 1.0f;
	_fScaleMin = 1.0f;
	_bScaleByAge = false;
	_bScaleBySpeed = false;
	_fScaleFadeIn = 0.0f;
	_bScaleInvert = false;
	_bScaleSmooth = true;
	_bScaleRandom = false;
	_fReScale = 1.0f;

	// animation
	_fDelay = 0.0f;
	_fEnd = 0.0f;
	_fSplinePos = 0.0f;
	_fEndSplinePos = -1.0f;

	// preview animation
	_bPreviewAnimation = false;
	
	// fast forward
	_bFastForward = false;

	// fill polygon
	_bFillPolygon = false;

	// trace polygon
	_nTracePolygon = 0;
	_nTracePolygonPoint = 0;

	// polygon bounding box
	_bBBdefined = false;
	_fMinX = 2048.0f;
	_fMaxX = -1024.0f;
	_fMinY = 2048.0f;
	_fMaxY = -1024.0f;
	// polygon offset
	_fPolygonOffsetX = 0.0f;
	_fPolygonOffsetY = 0.0f;
	
	memset(_szSpriteFile,0,256);
	
	// create graphics
	_lpBatch = KPTK::createKBatch();
//	_lpBatch->allocateBuffer(128);

	_bDisplay = false;
#ifndef IMAGEBANK_H
	_lpFontCount = new KTrueText (KMiscTools::makeFilePath ("acmesa.ttf"));
	_lpFontCount->setHeightPix (14);
	_lpFontCount->setColor(1.0f, 0.8f, 0.8f, 1.0f);
	_lpFontParameters = new KTrueText (KMiscTools::makeFilePath ("acmesa.ttf"));
	_lpFontParameters->setHeightPix (7);
	_lpFontParameters->setColor(0.8f, 1.0f, 0.8f, 1.0f);
	_lpFontAttribute = new KTrueText (KMiscTools::makeFilePath ("acmesa.ttf"));
	_lpFontAttribute->setHeightPix (20);
	_lpFontDebug = new KTrueText (KMiscTools::makeFilePath ("acmesa.ttf"));
	_lpFontDebug->setHeightPix (20);
#endif
	_bFontColor = false;
	
	_bDisplayParameters = false;
	memset(_szParameters, 0, 2000);
	memset(_attribute,0,256);
	memset(_debug,0,256);
	_fDebug = 0;

	memset(_presetName, 0, 128);
}

CreepingSmokeEmitter::~CreepingSmokeEmitter()
{
      K_LOG("Delete CreepingSmokeEmitter - start");
#ifndef IMAGEBANK_H
	delete _lpSpriteGraphic;
	delete _lpFontCount;
	delete _lpFontParameters;
	delete _lpFontAttribute;
	delete _lpFontDebug;
#else
   EImageBank::NotKeepInCache(_lpSpriteGraphic);
#endif
   if (_lpBatch->isBufferLost()) {
      K_LOG("Delete CreepingSmokeEmitter - Buffers Lost before delete");
   }
   delete _lpBatch;
      K_LOG("Delete CreepingSmokeEmitter - delete ok");

   point *op;
   point *optodelete;
   bool bGotNext = points.getHead(op);
   while (bGotNext) {
      optodelete = op;
      points.remove(op);
      bGotNext = points.getNext(op);
      XDELETE(optodelete);
   }

   keyframe *of;
   keyframe *oftodelete;
   bGotNext = keyframes.getHead(of);
   while (bGotNext) {
      oftodelete = of;
      keyframes.remove(of);
      bGotNext = keyframes.getNext(of);
      XDELETE(oftodelete);
   }

   CreepingSmokeParticle *oc;
   CreepingSmokeParticle *octodelete;
   bGotNext = _particles.getHead(oc);
   while (bGotNext) {
      octodelete = oc;
      _particles.remove(oc);
      bGotNext = _particles.getNext(oc);
      XDELETE(octodelete);
   }
   bGotNext = _trails.getHead(oc);
   while (bGotNext) {
      octodelete = oc;
      _trails.remove(oc);
      bGotNext = _trails.getNext(oc);
      XDELETE(octodelete);
   }
}

void CreepingSmokeEmitter::SetWindow(KWindow *lpWindow)
{
	_lpWindow = lpWindow;
	_nWindowWidth = lpWindow->getWindowWidth();
	_nWindowHeight = lpWindow->getWindowHeight();
}

void CreepingSmokeEmitter::SetSprite(const char *szSpritePath, int nSpriteDiv)
{
	sprintf(_szSpriteFile, szSpritePath, _szSpriteFile);
	sprintf(_attribute, szSpritePath, _szSpriteFile);

#ifndef IMAGEBANK_H
   _lpSpriteGraphic = KPTK::createKGraphic ();
   bool bSuccess = _lpSpriteGraphic->loadPicture (KMiscTools::makeFilePath (szSpritePath), true, true);
   if (!bSuccess) {
      KMiscTools::messageBox ("CreepingSmokeEmitter error", "Couldn't load sprite");
      return;
   }
	_lpSpriteGraphic->setTextureQuality (true);
#else
   // Ne peux fonctionné que si les image emitter sont déjà preloadé avec le KeepInCache actif
   _lpSpriteGraphic = EImageBank::getImage(szSpritePath,NULL,true,true);
   EImageBank::KeepInCache(_lpSpriteGraphic);

   if (_lpSpriteGraphic == NULL)
      return;
#endif
   _nSpriteDiv = nSpriteDiv;

#ifndef IMAGEBANK_H
   ResetSpriteDim();
#endif
}
void CreepingSmokeEmitter::ResetSpriteDim(){
   _lpSpriteGraphic->setBlitColor(_fRed, _fGreen, _fBlue, 1.0f);
	_nSpriteDim = int(_lpSpriteGraphic->getWidth() / _nSpriteDiv);
   _lpBatch->allocateBuffer(128);
}
void CreepingSmokeEmitter::SetGoal(float fGoal, float fGoalX, float fGoalY)
{
	_fGoal = fGoal;
	_fGoalX = fGoalX;
	_fGoalY = fGoalY;
	CreepingSmokeParticle *p;
	bool bGotNext = _particles.getHead(p);
	while (bGotNext){
		p->_lpParticle->_fGoal = fGoal;
		p->_lpParticle->_fGoalX = fGoalX;
		p->_lpParticle->_fGoalY = fGoalY;
		bGotNext = _particles.getNext(p);
	}
}

void CreepingSmokeEmitter::Move(float fPosX, float fPosY)
{
	_fPosX = fPosX;
	_fPosY = fPosY;
}

void CreepingSmokeEmitter::MovePolygon(float fPosX, float fPosY)
{
	_fPolygonOffsetX = fPosX;
	_fPolygonOffsetY = fPosY;
}

void CreepingSmokeEmitter::ReScale(float fScale)
{
	_fReScale = fScale;
}

void CreepingSmokeEmitter::Rate(bool bDown)
{
	_bEmit = false;
	if (bDown){
		_fTime = 0;
		_nParticles = 0;
	}
}

void CreepingSmokeEmitter::Spiral()
{
	if (_bSpiral){
		if (_fSpiralSpeed == 0)
			_fSpiralAngle = _fSpiralAnglePower;
		float radius = 100 * (_fSpiralRadiusMin + (_fSpiralRadius - _fSpiralRadiusMin) * cos(_fSpiralTime * _fSpiralRadiusFreq));
		radius *= _fReScale;
		_fSpiralPosX = _fPosX + radius * cos(_fSpiralAngle);
		_fSpiralPosY = _fPosY + radius * sin(_fSpiralAngle);
		_fSpiralAngle += _fSpiralSpeed;
		_fSpiralTime += _fSpiralRadiusSpeed;
	}
	else{
		_fSpiralPosX = _fPosX;
		_fSpiralPosY = _fPosY;
	}
}

void CreepingSmokeEmitter::Emit()
{
	if (_bStarted && _bEmit){

	// display parameters
	if (_bDisplayParameters)
		ListParameters();

	// calculate bounding box for fill polygon
	if (_bFillPolygon && !_bBBdefined)
		PolygonBB();

	// get animation spline end position
	if (_fEndSplinePos < 0 && keyframes.getCount() > 0){
		keyframe *k;
		keyframes.getTail(k);
		_fEndSplinePos = k->_fPos;
	}

	// get position from mouse
	if (_bUseMouse){
		_fPosX = KInput::getMouseX ();
		_fPosY = KInput::getMouseY ();
	}
	// preview position from animation
	else if (_bPreviewAnimation)
		GetPosition();
	// animation or fill polygon
	else{
		if (_fTime >= _fDelay){
			if (!_bIsActive){
				_bIsActive = true;
				_fTime = 0;
			}
			if (_nTracePolygon > 0)
				TracePolygon();
			else if (_bFillPolygon)
				FillPolygon();
			else if (_fSplinePos < _fEndSplinePos || keyframes.getCount() == 0)
				GetAnimation();
			else
				return;
		}
	}

	// compute the rate in particles/s
	float fRate = _nParticles / _fTime;

	// spiral
	Spiral();

	CreepingSmokeParticle *p;

	// create particles and pass the emitters attributes
	while (fRate < _fMinRate + (_fRate-_fMinRate)* rateFactor){
		_nParticles++;
		fRate = _nParticles / _fTime;
		p = new CreepingSmokeParticle;
		CreepingSmoke *cs;
		cs = new CreepingSmoke(_nSpriteDiv, _nSpriteDim, -_fBuoyancy, _fTextureRandom, false);
		
		cs->_fPosX = _fSpiralPosX;
		cs->_fPosY = _fSpiralPosY;
		// parameters
		cs->_fLife = _fLifeMin + getRandom(0, _fLife - _fLifeMin);
		cs->_fMass = _fMassMin + getRandom(0, _fMass - _fMassMin);
		cs->_fSpeed = _fSpeedMin + getRandom(0, _fSpeed - _fSpeedMin);
		cs->_bSpeedStop = _bSpeedStop;
		if (_bAngleRandom){
			float fAngle = getRandom(0, dpi);
			cs->_fAngle = fAngle;
			if (_fAngleCtrl > 0){
				float fSpinRandom = 0;
				if (_nSpinRandom > 0)
					fSpinRandom = 2 * pi / _nSpinRandom * floor(getRandom(0, float(_nSpinRandom)));
				cs->_fSpinAngle = -fAngle + _fAngleCtrl + fSpinRandom;
			}
		}
		else if (_fAngleCtrl < 0 && _bSpiral){
			cs->_fAngle = _fAngleCtrl - _fSpiralAngle - _fSpiralAnglePower * cos(_fSpiralTime * _fSpiralRadiusFreq);
			if (cos(_fSpiralTime * _fSpiralRadiusFreq) < 0)
				cs->_fAngle += pi;
		}
		else if (_fAngleCtrl > 0)
			cs->_fAngle = _fAngleCtrl;
		else
			cs->_fAngle = _fAngle;
		cs->_fInherit = _fInherit;
		cs->_fInheritAngle = _fAngle;
		// dynamic parameters
		cs->_fFriction = _fFriction;
		cs->_fTwirl = _fTwirl;
		cs->_fTwirlMin = _fTwirlMin;
		cs->_fTwirlMax = _fTwirlMax;
		cs->_fBuoyancy = _fBuoyancy;
		cs->_fNoise = _fNoise * _fReScale;
		cs->_fNoiseFreq = _fNoiseFreq;
		cs->_fNoiseAngleMax = _fNoiseAngleMax;
		cs->_fGravity = _fGravity;
		cs->_fWind = _fWind * _fReScale;
		cs->_fWindAngle = _fWindAngle;
		cs->_fWindDelay = _fWindDelay;
		cs->_bForcesFollow = _bForcesFollow;
		cs->_nWindowWidth = _nWindowWidth;
		cs->_nWindowHeight = _nWindowHeight;
		cs->_fVortex = _fVortex;
		cs->_fVortexX = _fPosX;
		cs->_fVortexY = _fPosY;
		cs->_fGoal = _fGoal;
		cs->_fGoalX = _fGoalX;
		cs->_fGoalY = _fGoalY;
		// appearance
		cs->_bTextureRate = _bTextureRate;
		cs->_fTextureRate = _fTextureRate;
		cs->_nTextureLimit = int(_fTextureLimit);
		cs->_bTextureBySpeed = _bTextureBySpeed;
		cs->_bTextureRotate = _bTextureRotate;
		cs->_fOpacity = _fOpacity;
		cs->_bBlendByAge = _bBlendByAge;
		cs->_bBlendBySpeed = _bBlendBySpeed;
		cs->_fBlendFadeIn = _fBlendFadeIn;
		cs->_bBlendFadeOut = _bBlendFadeOut;
		cs->_bBlendInvert = _bBlendInvert;
		cs->_bBlendSmooth = _bBlendSmooth;
		cs->_fScale = _fScale * _fReScale;
		cs->_fScaleMin = _fScaleMin * _fReScale;
		cs->_bScaleByAge = _bScaleByAge;
		cs->_bScaleBySpeed = _bScaleBySpeed;
		cs->_fScaleFadeIn = _fScaleFadeIn;
		cs->_bScaleInvert = _bScaleInvert;
		cs->_bScaleSmooth = _bScaleSmooth;
		cs->_bScaleRandom = _bScaleRandom;

		p->_lpParticle = cs;
		_particles.addToTail(p);

		Spiral();
	}
	} // _bStarted
}

void CreepingSmokeEmitter::EmitTrail(CreepingSmokeParticle *p)
{
	p->_lpParticle->_fTrailEmit = 0.0f;
	// add particle to trail
	CreepingSmokeParticle *t;
	t = new CreepingSmokeParticle;
	CreepingSmoke *cs;
	cs = new CreepingSmoke(_nSpriteDiv, _nSpriteDim, -_fBuoyancy, _fTextureRandom, true);
	cs->_fTrailOffset = p->_lpParticle->_fAge / p->_lpParticle->_fLife;
	// copy parameters
	cs->_fVelocity = p->_lpParticle->_fVelocity * p->_lpParticle->_fInherit;
	cs->_fSize = p->_lpParticle->_fSize;
	cs->_fAge = 0.0f;
	cs->_fSpin = p->_lpParticle->_fSpin;
	if (_bTrailSpinRandom)
		cs->_fSpinAngle = getRandom(0, dpi);
	else
		cs->_fSpinAngle = p->_lpParticle->_fSpinAngle;
	cs->_fBlend = p->_lpParticle->_fBlend;
	cs->_bSpin = p->_lpParticle->_bSpin;
	// set particle parameters
	cs->_fPosX = p->_lpParticle->_fPosX;
	cs->_fPosY = p->_lpParticle->_fPosY;
	// parameters
	cs->_fLife = _fTrailLife;
	cs->_fMass = p->_lpParticle->_fMass;
	cs->_fSpeed = p->_lpParticle->_fSpeed;
	cs->_bSpeedStop = p->_lpParticle->_bSpeedStop;
	cs->_fAngle = p->_lpParticle->_fAngle;
	cs->_fInherit = p->_lpParticle->_fInherit;
	cs->_fInheritAngle = p->_lpParticle->_fInheritAngle;
	// dynamic parameters
	cs->_fFriction = p->_lpParticle->_fFriction;
	cs->_fTwirl = p->_lpParticle->_fTwirl;
	cs->_fTwirlMin = p->_lpParticle->_fTwirlMin;
	cs->_fTwirlMax = p->_lpParticle->_fTwirlMax;
	cs->_fBuoyancy = p->_lpParticle->_fBuoyancy;
	cs->_fNoise = p->_lpParticle->_fNoise;
	cs->_fNoiseFreq = p->_lpParticle->_fNoiseFreq;
	cs->_fNoiseAngleMax = p->_lpParticle->_fNoiseAngleMax;
	cs->_fGravity = p->_lpParticle->_fGravity;
	cs->_fWind = p->_lpParticle->_fWind;
	cs->_fWindAngle = p->_lpParticle->_fWindAngle;
	cs->_fWindDelay = p->_lpParticle->_fWindDelay;
	cs->_bForcesFollow = _bForcesFollow;
	cs->_nWindowWidth = _nWindowWidth;
	cs->_nWindowHeight = _nWindowHeight;
	cs->_fVortex = p->_lpParticle->_fVortex;
	cs->_fVortexX = p->_lpParticle->_fPosX;
	cs->_fVortexY = p->_lpParticle->_fPosY;
	cs->_fGoal = p->_lpParticle->_fGoal;
	cs->_fGoalX = p->_lpParticle->_fGoalX;
	cs->_fGoalY = p->_lpParticle->_fGoalY;
	// appearance
	cs->_bTextureRate = p->_lpParticle->_bTextureRate;
	if (_bTrailRandom)
		cs->_fTextureRandom = getRandom(0, _fTextureRandom);
	else
		cs->_fTextureRandom = p->_lpParticle->_fTextureRandom + p->_lpParticle->_fAge * p->_lpParticle->_fTextureRate;
	cs->_fTextureRate = p->_lpParticle->_fTextureRate;
	cs->_nTextureLimit = int(_fTextureLimit);
	cs->_bTextureBySpeed = _bTextureBySpeed;
	cs->_bTextureRotate = _bTextureRotate;
	cs->_fOpacity = p->_lpParticle->_fOpacity;
	cs->_bBlendByAge = _bBlendByAge;
	cs->_bBlendBySpeed = _bBlendBySpeed;
	cs->_fBlendFadeIn = _fBlendFadeIn;
	cs->_bBlendFadeOut = _bBlendFadeOut;
	cs->_bBlendInvert = _bBlendInvert;
	cs->_bBlendSmooth = _bBlendSmooth;
	cs->_fScale = p->_lpParticle->_fScale;
	cs->_fScaleMin = p->_lpParticle->_fScaleMin;
	cs->_bScaleByAge = _bScaleByAge;
	cs->_bScaleBySpeed = _bScaleBySpeed;
	cs->_fScaleFadeIn = _fScaleFadeIn;
	cs->_bScaleInvert = _bScaleInvert;
	cs->_bScaleSmooth = _bScaleSmooth;
	cs->_bScaleRandom = _bScaleRandom;

	// add to list
	t->_lpParticle = cs;
	_trails.addToTail(t);
}

void CreepingSmokeEmitter::Logic()
{
	// get the time elasped since last frame
	float fElaspedTime = 0.01667f;
	if (_fElaspedTime > fElaspedTime)
		fElaspedTime = _fElaspedTime / (_fElaspedTime / 0.01f) * 1.667f;
	else
		fElaspedTime = _fElaspedTime;
	_fTime += fElaspedTime;

	// calculate angle
	if (_bUseMouse || _fInherit > 0){
		if (_fInheritPosX != _fPosX || _fInheritPosY != _fPosY){
			float h = (_fPosX - _fInheritPosX + 1e-6f) / (_fPosY - _fInheritPosY + 1e-6f);
			_fAngle = atan(h);
			if (_fInheritPosY > _fPosY)
				_fAngle += pi;
			_fInheritPosX = _fPosX;
			_fInheritPosY = _fPosY;
		}
	}

	// evaluate particles
	float fTrailRate = _fTrailLife / (_nTrailChilds + 1);
	CreepingSmokeParticle *p;
   CreepingSmokeParticle *oToDelete = NULL;
	bool bGotNext = _particles.getHead(p);
	while (bGotNext){
		// remove particle if it is dead
		if (p->_lpParticle->_fAge > p->_lpParticle->_fLife)
      {
			_particles.remove(p);
         oToDelete = p;
      }
      // emit trail
      if (_nTrailChilds > 0){
         if (p->_lpParticle->_fTrailEmit > fTrailRate)
            EmitTrail(p);
      }
      // evaluate
      p->_lpParticle->Logic(fElaspedTime);
      
		bGotNext = _particles.getNext(p);
      if (oToDelete != NULL) {
         XDELETE(oToDelete->_lpParticle);
         XDELETE(oToDelete);
      }
	}

	// evaluate trails
	bGotNext = _trails.getHead(p);
	while (bGotNext){
		p->_lpParticle->Logic(fElaspedTime);
		if (p->_lpParticle->_fAge > p->_lpParticle->_fLife)
      {
			_trails.remove(p);
         oToDelete = p;
      }
		bGotNext = _trails.getNext(p);
      if (oToDelete != NULL) {
         XDELETE(oToDelete->_lpParticle);
         XDELETE(oToDelete);
      }
	}

	// emit particles
	if (_fRateDuration == 0 || _fTime < _fRateDuration)
		Emit();

	// parameter keys
	if (_bUseKeys)
		Keys(); 
}

void CreepingSmokeEmitter::Render()
{
	// get the time elasped since last frame
	_fElaspedTime = float(_lpWindow->getFrameTime() * .001f);

	int nTrailsCount = _trails.getCount();
	int nCount = _particles.getCount();
   
   if (nTrailsCount == 0 && nCount == 0) {
      return;
   }

   if (_lpBatch->isBufferLost()) {
      K_LOG("Render CreepingSmokeEmitter - Buffers Lost");
      _lpBatch->allocateBuffer(128);
   }
   
	// render
	_lpBatch->beginBatch(_lpSpriteGraphic);
	CreepingSmokeParticle *p;

	// draw trails
	bool bGotNext = false;
	if (nTrailsCount > 0){
		bGotNext = _trails.getHead(p);
		while (bGotNext) {
			_lpBatch->blitAlphaRectFx(p->_lpParticle->_fTex[0], p->_lpParticle->_fTex[1], p->_lpParticle->_fTex[2], p->_lpParticle->_fTex[3], p->_lpParticle->_fPosX, p->_lpParticle->_fPosY, p->_lpParticle->_fSpin, p->_lpParticle->_fScale, p->_lpParticle->_fBlend, false, false, 0, 0);
			bGotNext = _trails.getNext(p);
		}
	}

	// draw particles
	if (nCount > 0){
		bGotNext = _particles.getHead(p);
		while (bGotNext) {
			_lpBatch->blitAlphaRectFx(p->_lpParticle->_fTex[0], p->_lpParticle->_fTex[1], p->_lpParticle->_fTex[2], p->_lpParticle->_fTex[3], p->_lpParticle->_fPosX, p->_lpParticle->_fPosY, p->_lpParticle->_fSpin, p->_lpParticle->_fScale, p->_lpParticle->_fBlend, false, false, 0, 0);
			bGotNext = _particles.getNext(p);
		}
	}

	_lpBatch->endBatch();

	particlesCount += nCount + nTrailsCount;

#ifndef IMAGEBANK_H
	if (_bDisplay){
		if (_bUseKeys){
			if (_bFillPolygon || _nTracePolygon > 0 || !_bUseMouse)
				sprintf(_debug, "%s________\ncount=%d ______\ntotal=%d _____", _presetName, nCount, nCount + nTrailsCount);
			else
				sprintf(_debug, "%s________\ncount=%d ______\ntotal=%d _____\nx=%d ____\ny=%d ___\ns=%4.3f __", _presetName, nCount, nCount + nTrailsCount, (int)_fPosX, (int)_fPosY, _fReScale);
			_lpFontCount->drawMultiline(_debug, 800, 1024, 0, TEXTSTYLE_RIGHT, 0, -1);
			_lpFontParameters->drawMultiline(_szParameters, 0, 1024, 0, TEXTSTYLE_LEFT, 0, -1);
		}
		_lpFontAttribute->drawStringFromLeft(_attribute, 400, 740);
		// debug
		sprintf(_debug, "debug = %f", _fDebug);
		_lpFontDebug->drawStringFromLeft(_debug, 100, 740);
   }
#endif
}

void CreepingSmokeEmitter::Start()
{
	_bStarted = true;
	_bIsActive = false;
	_fTime = 0;
	_nParticles = 0;
	_fSpiralAngle = 0;

	// fast forward
	if (_bFastForward){
		float fLife = (_fLife + _fLifeMin) / 2;
		_fElaspedTime = 0.01667f;
		while (_nParticles < (_fMinRate + (_fRate-_fMinRate)* rateFactor) * fLife)
			Logic();
	}
}

void CreepingSmokeEmitter::Stop()
{
	_bStarted = false;
}

void CreepingSmokeEmitter::Delete()
{
	// delete particles
	CreepingSmokeParticle *p;
	_particles.getHead(p);
	while (_particles.getCount() > 0){
		_particles.remove(p);
      XDELETE(p->_lpParticle);
      XDELETE(p);
		_particles.getHead(p);
	}
	_nParticles = 0;
}

bool CreepingSmokeEmitter::IsCompleted()
{
	if (_fTime >= _fEnd)
		return true;
	return false;
}

void CreepingSmokeEmitter::WritePreset(const char *szPresetPath)
{
	FILE *pFile;
	pFile = fopen (szPresetPath, "w");

	if (pFile == NULL){
		KMiscTools::messageBox("File does not exist", szPresetPath);
		return;
	}
	else if (KMiscTools::alertBox("Overwrite", szPresetPath, "OK", "Cancel") == 0){

	// header
	float version = 1.0;
	fprintf (pFile, "CreepingSmoke Preset version : %f\n", version);

	// trace polygon
	fprintf (pFile, "_nTracePolygon %d\n", _nTracePolygon);

	// fast forward
	fprintf (pFile, "_bFastForward %d\n", _bFastForward);

	// trails
	fprintf (pFile, "_fTrailLife %f\n", _fTrailLife);
	fprintf (pFile, "_nTrailChilds %d\n", _nTrailChilds);
	fprintf (pFile, "_bTrailRandom %d\n", _bTrailRandom);
	fprintf (pFile, "_bTrailSpinRandom %d\n", _bTrailSpinRandom);

	// spiral attributes
	fprintf (pFile, "_bSpiral %d\n", _bSpiral);
	fprintf (pFile, "_fSpiralRadius %f\n", _fSpiralRadius);
	fprintf (pFile, "_fSpiralRadiusMin %f\n", _fSpiralRadiusMin);
	fprintf (pFile, "_fSpiralRadiusFreq %f\n", _fSpiralRadiusFreq);
	fprintf (pFile, "_fSpiralSpeed %f\n", _fSpiralSpeed);
	fprintf (pFile, "_fSpiralRadiusSpeed %f\n", _fSpiralRadiusSpeed);
	fprintf (pFile, "_fSpiralAnglePower %f\n", _fSpiralAnglePower);

	// emission attributes
	fprintf (pFile, "_fRate %f\n", _fRate);
	fprintf (pFile, "_fMinRate %f\n", _fMinRate);
	fprintf (pFile, "_fRateDuration %f\n", _fRateDuration);
	fprintf (pFile, "_fSpeed %f\n", _fSpeed);
	fprintf (pFile, "_fSpeedMin %f\n", _fSpeedMin);
	fprintf (pFile, "_bSpeedStop %d\n", _bSpeedStop);	
	fprintf (pFile, "_bAngleRandom %d\n", _bAngleRandom);
	fprintf (pFile, "_fAngleCtrl %f\n", _fAngleCtrl);
	fprintf (pFile, "_nSpinRandom %d\n", _nSpinRandom);
	
	// particles attributes
	fprintf (pFile, "_fLife %f\n", _fLife);
	fprintf (pFile, "_fLifeMin %f\n", _fLifeMin);
	fprintf (pFile, "_fMass %f\n", _fMass);
	fprintf (pFile, "_fMassMin %f\n", _fMassMin);
	fprintf (pFile, "_fInherit %f\n", _fInherit);
	fprintf (pFile, "_fFriction %f\n", _fFriction);
	fprintf (pFile, "_fTwirl %f\n", _fTwirl);
	fprintf (pFile, "_fTwirlMin %f\n", _fTwirlMin);
	fprintf (pFile, "_fTwirlMax %f\n", _fTwirlMax);
	fprintf (pFile, "_fBuoyancy %f\n", _fBuoyancy);
	fprintf (pFile, "_fNoise %f\n", _fNoise);
	fprintf (pFile, "_fNoiseFreq %f\n", _fNoiseFreq);
	fprintf (pFile, "_fNoiseAngleMax %f\n", _fNoiseAngleMax);
	fprintf (pFile, "_fVortex %f\n", _fVortex);
	fprintf (pFile, "_fGravity %f\n", _fGravity);
	fprintf (pFile, "_fWind %f\n", _fWind);
	fprintf (pFile, "_fWindAngle %f\n", _fWindAngle);
	fprintf (pFile, "_fWindDelay %f\n", _fWindDelay);
	fprintf (pFile, "_bForcesFollow %d\n", _bForcesFollow);
	
	// appearance attributes
	fprintf (pFile, "_fRed %f\n", _fRed);
	fprintf (pFile, "_fGreen %f\n", _fGreen);
	fprintf (pFile, "_fBlue %f\n", _fBlue);
	fprintf (pFile, "_bTextureRate %d\n", _bTextureRate);
	fprintf (pFile, "_fTextureRate %f\n", _fTextureRate);
	fprintf (pFile, "_fTextureRandom %f\n", _fTextureRandom);
	fprintf (pFile, "_fTextureLimit %f\n", _fTextureLimit);
	fprintf (pFile, "_bTextureBySpeed %d\n", _bTextureBySpeed);
	fprintf (pFile, "_bTextureRotate %d\n", _bTextureRotate);
	fprintf (pFile, "_fOpacity %f\n", _fOpacity);
	fprintf (pFile, "_bBlendByAge %d\n", _bBlendByAge);
	fprintf (pFile, "_bBlendBySpeed %d\n", _bBlendBySpeed);
	fprintf (pFile, "_fBlendFadeIn %f\n", _fBlendFadeIn);
	fprintf (pFile, "_bBlendFadeOut %d\n", _bBlendFadeOut);
	fprintf (pFile, "_bBlendInvert %d\n", _bBlendInvert);
	fprintf (pFile, "_bBlendSmooth %d\n", _bBlendSmooth);
	fprintf (pFile, "_fScale %f\n", _fScale);
	fprintf (pFile, "_fScaleMin %f\n", _fScaleMin);
	fprintf (pFile, "_bScaleByAge %d\n", _bScaleByAge);
	fprintf (pFile, "_bScaleBySpeed %d\n", _bScaleBySpeed);
	fprintf (pFile, "_fScaleFadeIn %f\n", _fScaleFadeIn);
	fprintf (pFile, "_bScaleInvert %d\n", _bScaleInvert);
	fprintf (pFile, "_bScaleSmooth %d\n", _bScaleSmooth);
	fprintf (pFile, "_bScaleRandom %d\n", _bScaleRandom);

	//sprite
	fprintf (pFile, "_nSpriteDiv %d\n", _nSpriteDiv);
	fprintf (pFile, "_lpSpriteGraphic %s\n", _szSpriteFile);
	
	fclose (pFile);
	//KMiscTools::messageBox ("Saved Preset : ", szPresetPath);
	}	
}


/* ANIMATION */

void CreepingSmokeEmitter::GetPosition()
{
	int nCount = points.getCount();
	int nPos = int(nCount * _fSplinePos) + 1;
	point *a;
	a = new point;
	point *b;
	b = new point;
	point *c;
	c = new point;
	point *d;
	d = new point;
	point *p;
	points.getHead(p);
	if (!points.getCount())
		return;
	if (nPos < 2){
		a->_x = p->_x;
		a->_y = p->_y;
		points.getNext(p);
		b->_x = p->_x;
		b->_y = p->_y;
		points.getNext(p);
		c->_x = p->_x;
		c->_y = p->_y;
		points.getNext(p);
		d->_x = p->_x;
		d->_y = p->_y;
	}
	else if (nPos > nCount - 3){
		points.getTail(p);
		d->_x = p->_x;
		d->_y = p->_y;
		points.getPred(p);
		c->_x = p->_x;
		c->_y = p->_y;
		points.getPred(p);
		b->_x = p->_x;
		b->_y = p->_y;
		points.getPred(p);
		a->_x = p->_x;
		a->_y = p->_y;
		nPos = nCount - 3;
	}
	else{
		for (int i = 0; i <= nPos; i++)
			points.getNext(p);
		c->_x = p->_x;
		c->_y = p->_y;
		points.getNext(p);
		d->_x = p->_x;
		d->_y = p->_y;
		points.getPred(p);
		points.getPred(p);
		b->_x = p->_x;
		b->_y = p->_y;
		points.getPred(p);
		a->_x = p->_x;
		a->_y = p->_y;
	}

	// get the % position in the spline part
	double t = 0;
	if (nPos < 2)
		t = _fSplinePos * nCount / nPos * 2 / 3.0;
	else
		t = 1 / 3.0 * ( 1 + (_fSplinePos * nCount - nPos + 1) );
	
	float x0 = float(pow((1 - t), 2) * a->_x + 2 * t * (1 - t) * b->_x + pow(t, 2) * c->_x);
	float y0 = float(pow((1 - t), 2) * a->_y + 2 * t * (1 - t) * b->_y + pow(t, 2) * c->_y);
	float x1 = float(pow((1 - t), 2) * b->_x + 2 * t * (1 - t) * c->_x + pow(t, 2) * d->_x);
	float y1 = float(pow((1 - t), 2) * b->_y + 2 * t * (1 - t) * c->_y + pow(t, 2) * d->_y);
	_fPosX = float(x0 * (1 - t) + x1 * t);
	_fPosY = float(y0 * (1 - t) + y1 * t);

   delete a;
   delete b;
   delete c;
   delete d;
}

void CreepingSmokeEmitter::GetAnimation()
{
	if (keyframes.getCount() > 0){
		float fTime = _fTime - _fDelay;
		keyframe *k;
		bool bGotNext = keyframes.getHead(k);
		float tLast = k->_fTime;
		float fLast = k->_fPos;
		keyframes.getNext(k);
		float t = k->_fTime;
		float f = k->_fPos;
		while (t <= fTime){
			bGotNext = keyframes.getNext(k);
			if (bGotNext){
				tLast = t;
				fLast = f;
				t = k->_fTime;
				f = k->_fPos;
			}
			else
				break;
		}
		_fSplinePos = fLast + (f - fLast) * (fTime - tLast) / (t - tLast);
		GetPosition();
	}
	else if (points.getCount() > 0){
		point *p;
		points.getHead(p);
		_fPosX = p->_x;
		_fPosY = p->_y;
	}
}

void CreepingSmokeEmitter::ReadAnimation(const char *szPresetPath)
{
	// clear positions
	point *p;
	point *oToDelete = NULL;
	bool bGotNext = points.getHead(p);
	while (bGotNext){
		points.remove(p);
      oToDelete = p;
		bGotNext = points.getNext(p);
      if (oToDelete != NULL) {
         XDELETE(oToDelete);
      }
	}
	// clear keyframes
	keyframe *k;
   keyframe *oToDeletekf;
	bGotNext = keyframes.getHead(k);
	while (bGotNext){
		keyframes.remove(k);
      oToDeletekf = k;
		bGotNext = keyframes.getNext(k);
      if (oToDeletekf != 0) {
         XDELETE(oToDeletekf);
      }
	}
	
	_fTime = 0;
	
   
   ESceneParser::RTextFile file(szPresetPath);
	
   if (!file.IsOpen()){
		KMiscTools::messageBox("File does not exist", szPresetPath);
		return;
	}

	char str[16];
   std::string strtoken;

	float f;
	// points
   sscanf(file.GetNextStrToken(strtoken), "%f", &f);
	while(f != -1){
		p = new point;
		p->_x = f;
		sscanf(file.GetNextStrToken(strtoken), "%f", &f);
		p->_y = f;
		points.addToTail(p);
		sscanf(file.GetNextStrToken(strtoken), "%f", &f);
	}
	// delay
	sscanf(file.GetNextStrToken(strtoken), "%s", str);
	sscanf(file.GetNextStrToken(strtoken), "%f", &f);
	_fDelay = f;
	// end
	sscanf(file.GetNextStrToken(strtoken), "%s", str);
	sscanf(file.GetNextStrToken(strtoken), "%f", &f);
	_fEnd = f;
	// keyframes
	sscanf(file.GetNextStrToken(strtoken), "%f", &f);
   while(!file.eof()){
		k = new keyframe;	
		k->_fTime = f;
		sscanf(file.GetNextStrToken(strtoken), "%f", &f);
		k->_fPos = f;
		keyframes.addToTail(k);
		sscanf(file.GetNextStrToken(strtoken), "%f", &f);
	}
	
	if (keyframes.getCount() == 0 && points.getCount() > 1)
		_bFillPolygon = true;
	else
		_bFillPolygon = false;
	_bBBdefined = false;
}

void CreepingSmokeEmitter::PolygonBB()
{
	_fMinX = 2048.0f;
	_fMaxX = -1024.0f;
	_fMinY = 2048.0f;
	_fMaxY = -1024.0f;
	// get bouding box
	point *p;
	bool bGotNext = points.getHead(p);
	while (bGotNext){
		if (p->_x < _fMinX)
			_fMinX = p->_x;
		if (p->_x > _fMaxX)
			_fMaxX = p->_x;
		if (p->_y < _fMinY)
			_fMinY = p->_y;
		if (p->_y > _fMaxY)
			_fMaxY = p->_y;
		bGotNext = points.getNext(p);
	}
	_bBBdefined = true;
}

void CreepingSmokeEmitter::FillPolygon()
{	
	// get a random point inside the polygon
	bool nInside = false;
	float x, y;
	float px, py;
	int nIntersections = 0;
	int nTests = 0;
	point *p;
	int nCount = points.getCount() - 1;
	while (!nInside){
		x = getRandom(_fMinX, _fMaxX);
		y = getRandom(_fMinY, _fMaxY);
		// check if the point is inside the polygon
		nIntersections = 0;
		nTests = 0;
		points.getHead(p);
		px = p->_x;
		py = p->_y;
		int i = 0;
		while (i < nCount){
			points.getNext(p);
			// horizontal scan
			if ((y > py && y < p->_y) || (y < py && y > p->_y)){
				nTests++;
				if ((x > px && x < p->_x) || (x < px && x > p->_x))
					nIntersections++;
				else if (x > px && x > p->_x)
					nIntersections += 2;
			}
			px = p->_x;
			py = p->_y;
			i++;
		}
		if (nIntersections > 1 && nIntersections < nTests * 2 - 1 && nIntersections % 2 == 0)
			nInside = true;
	}
	_fPosX = x + _fPolygonOffsetX;
	_fPosY = y + _fPolygonOffsetY;
}

void CreepingSmokeEmitter::TracePolygon()
{
	int n = points.getCount() - 1;
	if (n > 0){
		if (_nTracePolygon > 1)
			n = int(getRandom(0, n + 0.99f));
		else{
			if (++_nTracePolygonPoint > n)
				_nTracePolygonPoint = 0;
			n = _nTracePolygonPoint;
		}
		point *p;
		points.getHead(p);
		for (int i = 0; i < n; i++)
			points.getNext(p);
		_fPosX = p->_x;
		_fPosY = p->_y;
	}
}


/* LIST PARAMETERS */
void CreepingSmokeEmitter::ListParameters()
{
	memset(_szParameters, 0, 2000);

	// spiral attributes
   /*sprintf(_szParameters, "_bFillPolygon :   %d\n_nTracePolygon :   %d\n_bFastForward :   %d\n_bSpiral :   %d\n_fSpiralRadius :   %f\n_fSpiralRadiusMin :   %f\n_fSpiralRadiusFreq :   %f\n_fSpiralSpeed :   %f\n_fSpiralRadiusSpeed :   %f\n_fSpiralAnglePower :   %f\n_fRate :   %f\n_fMinRate :   \n_fRateDuration :   %f\n_fSpeed :   %f\n_fSpeedMin :   %f\n_bSpeedStop :   %d\n_bAngleRandom :   %d\n_fAngleCtrl :   %f\n_nSpinRandom :   %d\n_fLife :   %f\n_fLifeMin :   %f\n_fMass :   %f\n_fMassMin :   %f\n_fInherit :   %f\n_fFriction :   %f\n_fTwirl :   %f\n_fTwirlMin :   %f\n_fTwirlMax :   %f\n_fBuoyancy :   %f\n_fNoise :   %f\n_fNoiseFreq :   %f\n_fNoiseAngleMax :   %f\n_fVortex :   %f\n_fGravity :   %f\n_fWind :   %f\n_fWindAngle :   %f\n_fWindDelay :   %f\n_bForcesFollow :   %d\n_fTrailLife :   %f\n_nTrailChilds :   %d\n_bTrailRandom :   %d\n_bTrailSpinRandom :   %d\n_fRed :   %f\n_fGreen :   %f\n_fBlue :   %f\n_bTextureRate :   %d\n_bTextureRate :   %f\n_fTextureRandom :   %f\n_fTextureLimit :   %f\n_bTextureBySpeed :   %d\n_bTextureRotate :   %d\n_fOpacity :   %f\n_bBlendByAge :   %d\n_bBlendBySpeed :   %d\n_fBlendFadeIn :   %f\n_bBlendFadeOut :   %d\n_bBlendInvert :   %d\n_bBlendSmooth :   %d\n_fScale :   %f\n_fScaleMin :   %f\n_bScaleByAge :   %d\n_bScaleBySpeed :   %d\n_fScaleFadeIn :   %f\n_bScaleInvert :   %d\n_bScaleSmooth :   %d\n_bScaleRandom :   %d\n_szSpriteFile :   %s",
		_bFillPolygon, _nTracePolygon, _bFastForward, _bSpiral, _fSpiralRadius, _fSpiralRadiusMin, _fSpiralRadiusFreq, _fSpiralSpeed,_fSpiralRadiusSpeed, _fSpiralAnglePower,
		_fRate, _fMinRate, _fRateDuration, _fSpeed, _fSpeedMin, _bSpeedStop, _bAngleRandom, _fAngleCtrl, _nSpinRandom,
		_fLife, _fLifeMin, _fMass, _fMassMin, _fInherit, _fFriction, _fTwirl, _fTwirlMin, _fTwirlMax, _fBuoyancy, _fNoise, _fNoiseFreq, _fNoiseAngleMax, _fVortex, _fGravity, _fWind, _fWindAngle, _fWindDelay, _bForcesFollow,
		_fTrailLife, _nTrailChilds, _bTrailRandom, _bTrailSpinRandom, _fRed, _fGreen, _fBlue, _bTextureRate, _fTextureRate, _fTextureRandom, _fTextureLimit, _bTextureBySpeed, _bTextureRotate,
		_fOpacity, _bBlendByAge, _bBlendBySpeed, _fBlendFadeIn, _bBlendFadeOut, _bBlendInvert, _bBlendSmooth,
		_fScale, _fScaleMin, _bScaleByAge, _bScaleBySpeed, _fScaleFadeIn, _bScaleInvert, _bScaleSmooth, _bScaleRandom,
		_szSpriteFile);*/
}

/* LIST KEYS */
void CreepingSmokeEmitter::ListKeys()
{
	memset(_szParameters, 0, 2000);

	sprintf(_szParameters, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s",
	"_bFillPolygon        back",
	"_nTracePolygon        back + ctrl",
	"_bFastForward        end",
	"_fRate        F8 F9 (+ ctrl)",
	"_fRateDuration        F8 F9 + shift",
	"_bSpiral        del",
	"_fSpiralRadius        rmb lmb",
	"_fSpiralRadiusMin        rmb lmb + ctrl",
	"_fSpiralRadiusFreq        wheel + ctrl (mmb = floor)",
	"_fSpiralSpeed        wheel + shift",
	"_fSpiralRadiusSpeed        wheel (mmb = sync)",
	"_fSpiralAnglePower        rmb lmb + shift",
	"_fMass        a z",
	"_fMassMin        A Z",
	"_fMassMin = _fMass        TAB",
	"_fLife        w x (+ ctrl)",
	"_fLifeMin        W X",
	"_fLifeMin = _fLife        F6",
	"_fSpeed        down up",
	"_fSpeedMin        DOWN UP",
	"_fSpeedMin = _fSpeed        F7",
	"_bSpeedStop        F7 + ctrl",
	"_bAngleRandom        space",
	"_fAngleCtrl        LEFT RIGHT",
	"_nSpinRandom        C V",
	"_fFriction        left right",
	"_fInherit        j k",
	"_fTwirl        t y",
	"_fTwirlMin        e r",
	"_fTwirlMax        d f",
	"_fBuoyancy        c v (+ ctrl) fSpinRandom",
	"_fNoise        b n",
	"_fNoiseFreq        B N",
	"_fNoiseAngleMax        B N + ctrl",
	"_fVortex        j k + ctrl",
	"_fGravity        O P",
	"_fWind        l m",
	"_fWindAngle        o p",
	"_fWindDelay        L M",
	"_bForcesFollow        f + ctrl",
	"_fTrailLife        o p + ctrl",
	"_bTrailChilds        l m + ctrl",
	"_bTrailRandom        home + ctrl",
	"_bTrailSpinRandom        home + shift",
	"_bTextureRate        home",
	"_fTextureRate        prior next",
	"_fTextureRandom        prior next + ctrl",
	"_fTextureLimit        PRIOR NEXT",
	"_bTextureBySpeed        F10",
	"_bTextureRotate        F10 + ctrl",
	"_fOpacity        G H",
	"_bBlendByAge        F1",
	"_bBlendBySpeed        F2",
	"_fBlendFadeIn        g h",
	"_bBlendFadeOut        h + ctrl",
	"_bBlendInvert        g + ctrl",
	"_bBlendSmooth        F1 + shift",
	"_fScale        q s",
	"_fScaleMin        Q S",
	"_fScaleByAge        F3",
	"_fScaleBySpeed        F4",
	"_fScaleFadeIn        u i",
	"_bScaleInvert        q + ctrl",
	"_bScaleRandom        s + ctrl",
	"_bScaleSmooth        F3 + shift",
	"_fScaleMin = _fScale        F5",
	"_fReScale        a z + ctrl");

}


/* KEYS */

void CreepingSmokeEmitter::Keys()
{
	if (!_bKeyState){
		if (KInput::isPressed(K_VK_ESCAPE)){
			_bUseMouse = !_bUseMouse;
			if (!_bUseMouse){
				ReadAnimation("particles\\temp.csa");
				Start();
				_fSplinePos = 0;
			}
			_bKeyState = true;
		}	
		else if (KInput::isPressed(K_VK_OEM_COMMA)){
			if (_bStarted)
				Stop();
			else
				Start();
			_bKeyState = true;
		}
		else if (KInput::isPressed(K_VK_RETURN)){
			if (KInput::isPressed(K_VK_L_CONTROL)){
				if (_bDisplayParameters)
					memset(_szParameters, 0, 2000);
				_bDisplayParameters = !_bDisplayParameters;
			}
			else if (strlen(_szParameters) > 0)
				memset(_szParameters, 0, 2000);
			else
				ListKeys();
			_bKeyState = true;
		}
	}

	if (!KInput::isPressed(K_VK_RETURN) && !KInput::isPressed(K_VK_F8) && !KInput::isPressed(K_VK_F9) && !KInput::isPressed(K_VK_F10) && !KInput::isPressed(K_VK_ESCAPE) && !KInput::isPressed(K_VK_OEM_COMMA)){
		_bEmit = true;
		_bKeyState = false;
	}

	if (KInput::isPressed(K_VK_L_CONTROL)){
		if (KInput::getRightButtonState()){
			_fSpiralRadiusMin -= 0.01f;
			sprintf(_attribute,"spiral radius min = %f\0", _fSpiralRadiusMin);
		}
		else if (KInput::getLeftButtonState()){
			_fSpiralRadiusMin += 0.01f;
			sprintf(_attribute,"spiral radius min = %f\0", _fSpiralRadiusMin);
		}
		else if (KInput::getMiddleButtonState())
			_fSpiralRadiusFreq = floor(_fSpiralRadiusFreq);
		else if (KInput::isPressed(K_VK_L_SHIFT))
			_fSpiralRadiusFreq += KInput::getWheelValue() * 0.000001f;
		else
			_fSpiralRadiusFreq += KInput::getWheelValue() * 0.001f;
	}
	else if (KInput::isPressed(K_VK_L_SHIFT)){
		if (KInput::getRightButtonState()){
			_fSpiralAnglePower -= 0.01f;
			sprintf(_attribute,"spiral angle power = %f\0", _fSpiralAnglePower);
		}
		else if (KInput::getLeftButtonState()){
			_fSpiralAnglePower += 0.01f;
			sprintf(_attribute,"spiral angle power = %f\0", _fSpiralAnglePower);
		}
		else if (KInput::getMiddleButtonState())
			_fSpiralSpeed = floor(_fSpiralSpeed);
		else{
			float speed = KInput::getWheelValue();
			if (speed != 0){
				_fSpiralSpeed += speed * 0.00001f;
				_fSpiralRadiusSpeed = _fSpiralSpeed;
			}
		}
	}
	else if (KInput::getMiddleButtonState())
		_fSpiralRadiusSpeed = _fSpiralSpeed;
	else
		_fSpiralRadiusSpeed += KInput::getWheelValue() * 0.00001f;

	// ctrl
	if (KInput::isPressed(K_VK_L_CONTROL)){
		if (KInput::isPressed(K_VK_BACK)){
			_nTracePolygon++;
			if (_nTracePolygon > 2){
				_nTracePolygon = 0;
				_bFillPolygon = false;
			}
			sprintf(_attribute,"trace polygon = %d\0", _nTracePolygon);
		}
		else if (KInput::isPressed(K_VK_O)){
			_fTrailLife -= 0.01f;
			sprintf(_attribute,"trail life = %f\0", _fTrailLife);
		}
		else if (KInput::isPressed(K_VK_P)){
			_fTrailLife += 0.01f;
			sprintf(_attribute,"trail life = %f\0", _fTrailLife);
		}
		else if (KInput::isPressed(K_VK_L)){
			_nTrailChilds -= 1;
			sprintf(_attribute,"trail childs = %d\0", _nTrailChilds);
		}
		else if (KInput::isPressed(K_VK_M)){
			_nTrailChilds += 1;
			sprintf(_attribute,"trail childs = %d\0", _nTrailChilds);
		}
		if (KInput::isPressed(K_VK_HOME)){
			_bTrailRandom = !_bTrailRandom;
			sprintf(_attribute,"_bTrailRandom = %d\0", _bTrailRandom);
		}
		if (KInput::isPressed(K_VK_F7)){
			_bSpeedStop = !_bSpeedStop;
			sprintf(_attribute,"_bSpeedStop = %d\0", _bSpeedStop);
		}
		else if (KInput::isPressed(K_VK_NEXT)){
			_fTextureRandom -= 0.01f;
			sprintf(_attribute,"texture random = %f\0", _fTextureRandom);
		}
		else if (KInput::isPressed(K_VK_PRIOR)){
			_fTextureRandom += 0.01f;
			sprintf(_attribute,"texture random = %f\0", _fTextureRandom);
		}
		else if (KInput::isPressed(K_VK_F10)){
			_bTextureRotate = !_bTextureRotate;
			sprintf(_attribute,"texture rotate = %d\0", _bTextureRotate);
		}
		else if (KInput::isPressed(K_VK_F8)){
			_fRate -= 0.001f;
			sprintf(_attribute,"rate = %f\0", _fRate);
			Rate(true);
		}
		else if (KInput::isPressed(K_VK_F9)){
			_fRate += 0.001f;
			sprintf(_attribute,"rate = %f\0", _fRate);
			Rate(false);
		}
		else if (KInput::isPressed(K_VK_DOWN)){
			_fSpeed -= 0.1f;
			sprintf(_attribute,"speed = %f\0", _fSpeed);
		}
		else if (KInput::isPressed(K_VK_UP)){
			_fSpeed += 0.1f;
			sprintf(_attribute,"speed = %f\0", _fSpeed);
		}
		else if (KInput::isPressed(K_VK_W)){
			_fLife -= 0.1f;
			sprintf(_attribute,"life = %f\0", _fLife);
		}
		else if (KInput::isPressed(K_VK_X)){
			_fLife += 0.1f;
			sprintf(_attribute,"life = %f\0", _fLife);
		}
		else if (KInput::isPressed(K_VK_F)){
			_bForcesFollow = !_bForcesFollow;
			sprintf(_attribute,"forces follow = %d\0", _bForcesFollow);
		}
		else if (KInput::isPressed(K_VK_LEFT)){
			_fFriction -= 0.0001f;
			sprintf(_attribute,"friction = %f\0", _fFriction);
		}
		else if (KInput::isPressed(K_VK_RIGHT)){
			_fFriction += 0.0001f;
			sprintf(_attribute,"friction = %f\0", _fFriction);
		}
		else if (KInput::isPressed(K_VK_B)){
			_fNoiseAngleMax -= 0.01f;
			sprintf(_attribute,"noise angle max = %f\0", _fNoiseAngleMax);
		}
		else if (KInput::isPressed(K_VK_N)){
			_fNoiseAngleMax += 0.01f;
			sprintf(_attribute,"noise angle max = %f\0", _fNoiseAngleMax);
		}
		else if (KInput::isPressed(K_VK_J)){
			_fVortex -= 0.01f;
			sprintf(_attribute,"vortex = %f\0", _fVortex);
		}
		else if (KInput::isPressed(K_VK_K)){
			_fVortex += 0.01f;
			sprintf(_attribute,"vortex = %f\0", _fVortex);
		}
		else if (KInput::isPressed(K_VK_F1)){
			_fRed -= 0.01f;
			sprintf(_attribute,"red = %f\0", _fRed);
		}
		else if (KInput::isPressed(K_VK_F2)){
			_fRed += 0.01f;
			sprintf(_attribute,"red = %f\0", _fRed);
		}
		else if (KInput::isPressed(K_VK_F3)){
			_fGreen -= 0.01f;
			sprintf(_attribute,"green = %f\0", _fGreen);
		}
		else if (KInput::isPressed(K_VK_F4)){
			_fGreen += 0.01f;
			sprintf(_attribute,"green = %f\0", _fGreen);
		}
		else if (KInput::isPressed(K_VK_F5)){
			_fBlue -= 0.01f;
			sprintf(_attribute,"blue = %f\0", _fBlue);
		}
		else if (KInput::isPressed(K_VK_F6)){
			_fBlue += 0.01f;
			sprintf(_attribute,"blue = %f\0", _fBlue);
		}
		else if (KInput::isPressed(K_VK_H)){
			_bBlendFadeOut = !_bBlendFadeOut;
			sprintf(_attribute,"blend fade out = %d\0", _bBlendFadeOut);
		}
		else if (KInput::isPressed(K_VK_G)){
			_bBlendInvert = !_bBlendInvert;
			sprintf(_attribute,"blend invert = %d\0", _bBlendInvert);
		}
		else if (KInput::isPressed(K_VK_Q)){
			_bScaleInvert = !_bScaleInvert;
			sprintf(_attribute,"scale invert = %d\0", _bScaleInvert);
		}
		else if (KInput::isPressed(K_VK_S)){
			_bScaleRandom = !_bScaleRandom;
			sprintf(_attribute,"scale random = %d\0", _bScaleRandom);
		}
		else if (KInput::isPressed(K_VK_A)){
			_fReScale -= 0.01f;
			sprintf(_attribute,"rescale = %f\0", _fReScale);
		}
		else if (KInput::isPressed(K_VK_Z)){
			_fReScale += 0.01f;
			sprintf(_attribute,"rescale = %f\0", _fReScale);
		}
		else if (KInput::isPressed(K_VK_END)){
			_bFontColor = !_bFontColor;
			if (_bFontColor)
				_lpFontParameters->setColor(0.8f, 1.0f, 0.8f, 1.0f);
			else
				_lpFontParameters->setColor(0.0f, 0.0f, 0.0f, 1.0f);
		}
		// reset
		else if (KInput::isPressed(K_VK_F10) && !_bKeyState){
			Reset();
			sprintf(_attribute,"RESET");
			_bKeyState = true;
		}
		_lpSpriteGraphic->setBlitColor(_fRed, _fGreen, _fBlue, 1.0f);
	}

	// shift
	else if (KInput::isPressed(K_VK_L_SHIFT)){
		if (KInput::isPressed(K_VK_HOME)){
			_bTrailSpinRandom = !_bTrailSpinRandom;
			sprintf(_attribute,"trail spin random = %d\0", _bTrailSpinRandom);
		}
		else if (KInput::isPressed(K_VK_F8)){
			_fRateDuration -= 0.01f;
			sprintf(_attribute,"rate duration = %f\0", _fRateDuration);
		}
		else if (KInput::isPressed(K_VK_F9)){
			_fRateDuration += 0.01f;
			sprintf(_attribute,"rate duration = %f\0", _fRateDuration);
		}
		else if (KInput::isPressed(K_VK_C)){
			_nSpinRandom -= 1;
			sprintf(_attribute,"spin random = %d\0", _nSpinRandom);
		}
		else if (KInput::isPressed(K_VK_V)){
			_nSpinRandom += 1;
			sprintf(_attribute,"spin random = %d\0", _nSpinRandom);
		}
		if (KInput::isPressed(K_VK_A)){
			_fMassMin -= 0.01f;
			sprintf(_attribute,"mass min = %f\0", _fMassMin);
		}
		else if (KInput::isPressed(K_VK_Z)){
			_fMassMin += 0.01f;
			sprintf(_attribute,"mass min = %f\0", _fMassMin);
		}
		else if (KInput::isPressed(K_VK_W)){
			_fLifeMin -= 0.01f;
			sprintf(_attribute,"life min = %f\0", _fLifeMin);
		}
		else if (KInput::isPressed(K_VK_X)){
			_fLifeMin += 0.01f;
			sprintf(_attribute," life min = %f\0", _fLifeMin);
		}
		else if (KInput::isPressed(K_VK_DOWN)){
			_fSpeedMin -= 0.01f;
			sprintf(_attribute,"speed min = %f\0", _fSpeedMin);
		}
		else if (KInput::isPressed(K_VK_UP)){
			_fSpeedMin += 0.01f;
			sprintf(_attribute,"speed min = %f\0", _fSpeedMin);
		}
		else if (KInput::isPressed(K_VK_LEFT)){
			_fAngleCtrl -= 0.01f;
			sprintf(_attribute,"angle = %f\0", _fAngleCtrl);
		}
		else if (KInput::isPressed(K_VK_RIGHT)){
			_fAngleCtrl += 0.01f;
			sprintf(_attribute,"angle = %f\0", _fAngleCtrl);
		}
		else if (KInput::isPressed(K_VK_B)){
			_fNoiseFreq -= 0.01f;
			sprintf(_attribute,"noise freq = %f\0", _fNoiseFreq);
		}
		else if (KInput::isPressed(K_VK_N)){
			_fNoiseFreq += 0.01f;
			sprintf(_attribute,"noise freq = %f\0", _fNoiseFreq);
		}
		else if (KInput::isPressed(K_VK_O)){
			_fGravity -= 0.01f;
			sprintf(_attribute,"gravity = %f\0", _fGravity);
		}
		else if (KInput::isPressed(K_VK_P)){
			_fGravity += 0.01f;
			sprintf(_attribute,"gravity = %f\0", _fGravity);
		}
		else if (KInput::isPressed(K_VK_L)){
			_fWindDelay -= 0.01f;
			sprintf(_attribute,"wind delay = %f\0", _fWindDelay);
		}
		else if (KInput::isPressed(K_VK_M)){
			_fWindDelay += 0.01f;
			sprintf(_attribute,"wind delay = %f\0", _fWindDelay);
		}
		else if (KInput::isPressed(K_VK_PRIOR)){
			_fTextureLimit -= 0.1f;
			sprintf(_attribute,"texture limit = %f\0", _fTextureLimit);
		}
		else if (KInput::isPressed(K_VK_NEXT)){
			_fTextureLimit += 0.1f;
			sprintf(_attribute,"texture limit = %f\0", _fTextureLimit);
		}
		else if (KInput::isPressed(K_VK_G)){
			_fOpacity -= 0.01f;
			sprintf(_attribute,"opacity = %f\0", _fOpacity);
		}
		else if (KInput::isPressed(K_VK_H)){
			_fOpacity += 0.01f;
			sprintf(_attribute,"opacity = %f\0", _fOpacity);
		}
		else if (KInput::isPressed(K_VK_F1)){
			_bBlendSmooth = !_bBlendSmooth;
			sprintf(_attribute,"blend smooth = %d\0", _bBlendSmooth);
		}
		else if (KInput::isPressed(K_VK_Q)){
			_fScaleMin -= 0.01f;
			sprintf(_attribute,"scale min = %f\0", _fScaleMin);
		}
		else if (KInput::isPressed(K_VK_S)){
			_fScaleMin += 0.01f;
			sprintf(_attribute,"scale min = %f\0", _fScaleMin);
		}
		else if (KInput::isPressed(K_VK_F3)){
			_bScaleSmooth = !_bScaleSmooth;
			sprintf(_attribute,"scale smooth = %d\0", _bScaleSmooth);
		}
	}

	else if (KInput::isPressed(K_VK_END)){
		_bFastForward = !_bFastForward;
		sprintf(_attribute,"fast forward = %d\0", _bFastForward);
	}

	else if (KInput::getRightButtonState()){
		_fSpiralRadius -= 0.01f;
		sprintf(_attribute,"spiral radius = %f\0", _fSpiralRadius);
	}
	else if (KInput::getLeftButtonState()){
		_fSpiralRadius += 0.01f;
		sprintf(_attribute,"spiral radius = %f\0", _fSpiralRadius);
	}
	else if (KInput::isPressed(K_VK_BACK)){
		_bFillPolygon = !_bFillPolygon;
		sprintf(_attribute,"fill polygon = %d\0", _bFillPolygon);
	}
	else if (KInput::isPressed(K_VK_F8)){
		_fRate -= 1.0f;
		sprintf(_attribute,"rate = %f\0", _fRate);
		Rate(true);
	}
	else if (KInput::isPressed(K_VK_F9)){
		_fRate += 1.0f;
		sprintf(_attribute,"rate = %f\0", _fRate);
		Rate(false);
	}
	else if (KInput::isPressed(K_VK_DEL)){
		_bSpiral = !_bSpiral;
		sprintf(_attribute,"spiral = %d\0", _bSpiral);
	}
	else if (KInput::isPressed(K_VK_A)){
		_fMass -= 0.01f;
		sprintf(_attribute,"mass = %f\0", _fMass);
	}
	else if (KInput::isPressed(K_VK_Z)){
		_fMass += 0.01f;
		sprintf(_attribute,"mass = %f\0", _fMass);
	}
	else if (KInput::isPressed(K_VK_TAB)){
		_fMassMin = _fMass;
		sprintf(_attribute,"mass min = mass", _fMassMin);
	}
	else if (KInput::isPressed(K_VK_W)){
		_fLife -= 0.01f;
		sprintf(_attribute,"life = %f\0", _fLife);
	}
	else if (KInput::isPressed(K_VK_X)){
		_fLife += 0.01f;
		sprintf(_attribute,"life = %f\0", _fLife);
	}
	else if (KInput::isPressed(K_VK_F6)){
		_fLifeMin = _fLife;
		sprintf(_attribute,"life min = life", _fLifeMin);
	}
	else if (KInput::isPressed(K_VK_DOWN)){
		_fSpeed -= 0.01f;
		sprintf(_attribute,"speed = %f\0", _fSpeed);
	}
	else if (KInput::isPressed(K_VK_UP)){
		_fSpeed += 0.01f;
		sprintf(_attribute,"speed = %f\0", _fSpeed);
	}
	else if (KInput::isPressed(K_VK_F7)){
		_fSpeedMin = _fSpeed;
		sprintf(_attribute,"speed min = speed", _fSpeedMin);
	}
	else if (KInput::isPressed(K_VK_SPACE)){
		_bAngleRandom = !_bAngleRandom;
		sprintf(_attribute,"angle rand = %d\0", _bAngleRandom);
	}
	else if (KInput::isPressed(K_VK_LEFT)){
		_fFriction -= 0.001f;
		sprintf(_attribute,"friction = %f\0", _fFriction);
	}
	else if (KInput::isPressed(K_VK_RIGHT)){
		_fFriction += 0.001f;
		sprintf(_attribute,"friction = %f\0", _fFriction);
	}
	else if (KInput::isPressed(K_VK_J)){
		_fInherit -= 0.001f;
		sprintf(_attribute,"inherit = %f\0", _fInherit);
	}
	else if (KInput::isPressed(K_VK_K)){
		_fInherit += 0.001f;
		sprintf(_attribute,"inherit = %f\0", _fInherit);
	}
	else if (KInput::isPressed(K_VK_T)){
		_fTwirl -= 0.001f;
		sprintf(_attribute,"twirl = %f\0", _fTwirl);
	}
	else if (KInput::isPressed(K_VK_Y)){
		_fTwirl += 0.001f;
		sprintf(_attribute,"twirl = %f\0", _fTwirl);
	}
	else if (KInput::isPressed(K_VK_E)){
		_fTwirlMin -= 0.01f;
		sprintf(_attribute,"twirl min = %f\0", _fTwirlMin);
	}
	else if (KInput::isPressed(K_VK_R)){
		_fTwirlMin += 0.01f;
		sprintf(_attribute,"twirl min = %f\0", _fTwirlMin);
	}
	else if (KInput::isPressed(K_VK_D)){
		_fTwirlMax -= 0.01f;
		sprintf(_attribute,"twirl max = %f\0", _fTwirlMax);
	}
	else if (KInput::isPressed(K_VK_F)){
		_fTwirlMax += 0.01f;
		sprintf(_attribute,"twirl max = %f\0", _fTwirlMax);
	}
	else if (KInput::isPressed(K_VK_C)){
		_fBuoyancy -= 0.001f;
		sprintf(_attribute,"buoyancy = %f\0", _fBuoyancy);
	}
	else if (KInput::isPressed(K_VK_V)){
		_fBuoyancy += 0.001f;
		sprintf(_attribute,"buoyancy = %f\0", _fBuoyancy);
	}
	else if (KInput::isPressed(K_VK_B)){
		_fNoise -= 0.01f;
		sprintf(_attribute,"noise = %f\0", _fNoise);
	}
	else if (KInput::isPressed(K_VK_N)){
		_fNoise += 0.01f;
		sprintf(_attribute,"noise = %f\0", _fNoise);
	}
	else if (KInput::isPressed(K_VK_L)){
		_fWind -= 0.01f;
		sprintf(_attribute,"wind = %f\0", _fWind);
	}
	else if (KInput::isPressed(K_VK_M)){
		_fWind += 0.01f;
		sprintf(_attribute,"wind = %f\0", _fWind);
	}
	else if (KInput::isPressed(K_VK_O)){
		_fWindAngle -= 0.01f;
		sprintf(_attribute,"wind angle = %f\0", _fWindAngle);
	}
	else if (KInput::isPressed(K_VK_P)){
		_fWindAngle += 0.01f;
		sprintf(_attribute,"wind angle = %f\0", _fWindAngle);
	}	
	else if (KInput::isPressed(K_VK_HOME)){
		_bTextureRate = !_bTextureRate;
		sprintf(_attribute,"texture rate on = %d\0", _bTextureRate);
	}
	else if (KInput::isPressed(K_VK_NEXT)){
		_fTextureRate -= 0.01f;
		sprintf(_attribute,"texture rate = %f\0", _fTextureRate);
	}
	else if (KInput::isPressed(K_VK_PRIOR)){
		_fTextureRate += 0.01f;
		sprintf(_attribute,"texture rate = %f\0", _fTextureRate);
	}
	else if (KInput::isPressed(K_VK_F10)){
		_bTextureBySpeed = !_bTextureBySpeed;
		sprintf(_attribute,"tex by speed = %d\0", _bTextureBySpeed);
	}
	else if (KInput::isPressed(K_VK_F1)){
		_bBlendByAge = !_bBlendByAge;
		sprintf(_attribute,"blend by age = %d\0", _bBlendByAge);
	}
	else if (KInput::isPressed(K_VK_F2)){
		_bBlendBySpeed = !_bBlendBySpeed;
		sprintf(_attribute,"blend by speed = %d\0", _bBlendBySpeed);
	}
	else if (KInput::isPressed(K_VK_G)){
		_fBlendFadeIn -= 0.001f;
		sprintf(_attribute,"blend fade in = %f\0", _fBlendFadeIn);
	}
	else if (KInput::isPressed(K_VK_H)){
		_fBlendFadeIn += 0.001f;
		sprintf(_attribute,"blend fade in = %f\0", _fBlendFadeIn);
	}
	else if (KInput::isPressed(K_VK_Q)){
		_fScale -= 0.01f;
		sprintf(_attribute,"scale = %f\0", _fScale);
	}
	else if (KInput::isPressed(K_VK_S)){
		_fScale += 0.01f;
		sprintf(_attribute,"scale = %f\0", _fScale);
	}
	else if (KInput::isPressed(K_VK_F3)){
		_bScaleByAge = !_bScaleByAge;
		sprintf(_attribute,"scale by age = %d\0", _bScaleByAge);
	}
	else if (KInput::isPressed(K_VK_F4)){
		_bScaleBySpeed = !_bScaleBySpeed;
		sprintf(_attribute,"scale by speed = %d\0", _bScaleBySpeed);
	}
	else if (KInput::isPressed(K_VK_U)){
		_fScaleFadeIn -= 0.001f;
		sprintf(_attribute,"scale fade in = %f\0", _fScaleFadeIn);
	}
	else if (KInput::isPressed(K_VK_I)){
		_fScaleFadeIn += 0.001f;
		sprintf(_attribute,"scale fade in = %f\0", _fScaleFadeIn);
	}
	else if (KInput::isPressed(K_VK_F5)){
		_fScaleMin = _fScale;
		sprintf(_attribute,"scale min = scale", _fScaleMin);
	}

	if (_fRate < 0)
		_fRate = 0;
	if (_fRateDuration < 0)
		_fRateDuration = 0;
	if (_fLife < 0)
		_fLife = 0;
	if (_fGravity < 0)
		_fGravity = 0;
	if (_fNoise < 0)
		_fNoise = 0;
	if (_fNoiseFreq < 0)
		_fNoiseFreq = 0;
	if (_fNoiseAngleMax < 0)
		_fNoiseAngleMax = 0;
	if (_fTwirlMin < 0)
		_fTwirlMin = 0;
	if (_fTwirlMax < 0)
		_fTwirlMax = 0;
	if (_fBuoyancy < -1)
		_fBuoyancy = -1;
	if (_fTrailLife < 0)
		_fTrailLife = 0;
	if (_nTrailChilds < 0)
		_nTrailChilds = 0;
	if (_fRed < 0)
		_fRed = 0;
	else if (_fRed > 1)
		_fRed = 1;
	if (_fGreen < 0)
		_fGreen = 0;
	else if (_fGreen > 1)
		_fGreen = 1;
	if (_fBlue < 0)
		_fBlue = 0;
	else if (_fBlue > 1)
		_fBlue = 1;
	if (_fTextureRandom < 0)
		_fTextureRandom = 0;
	else if (_fTextureRandom > 1)
		_fTextureRandom = 1;
	if (_fTextureLimit < 0)
		_fTextureLimit = 0;
	if (_fOpacity < 0)
		_fOpacity = 0;
	else if (_fOpacity > 1)
		_fOpacity = 1;
}

void CreepingSmokeEmitter::Reset()
{
	_fAngle = 0;
	_fInheritPosX = 0.0f;
	_fInheritPosY = 0.0f;
	_bSpiral = false;
	_fSpiralRadius = 0;
	_fSpiralRadiusMin = 0;
	_fSpiralRadiusFreq = 0.01f;
	_fSpiralRadiusSpeed = 0.0f;
	_fSpiralSpeed = 0.0f;
	_fSpiralAnglePower = pi / 2;
	_fSpiralPosX = 0.0f;
	_fSpiralPosY = 0.0f;
	_fSpiralAngle = -pi / 2;
	_fSpiralTime = 0.0f;

	// emission attributes
	_bEmit = true;
	_fTime = 0;
	_nParticles = 0;
	_fRate = 10.0f;
   _fMinRate = 0.0f;
	_fRateDuration = 0.0f;
	_fSpeed = 0.0f;
	_fSpeedMin = 0.0f;
	_bSpeedStop = false;
	_bAngleRandom = false;
	_fAngleCtrl = 0.000f;
	_nSpinRandom = 0;
	
	// particles attributes
	_fLife = 1.0f;
	_fLifeMin = 10.f;
	_fMass = 1.0f;
	_fMassMin = 1.0f;
	_fInherit = 0.0f;
	_fFriction = 0.01f;
	_fTwirl = 0.0f;
	_fTwirlMin = 0.0f;
	_fTwirlMax = 0.0f;
	_fBuoyancy = 0.0f;
	_fNoise = 0.0f;
	_fGravity = 0.0f;
	_fWind = 0.0f;
	_fWindAngle = 0.0f;
	_fWindDelay = 0.0f;
	_bForcesFollow = false;

	// vortex
	_fVortex = 0.0f;

	// goal
	_fGoal = 0.0f;
	_fGoalX = 512.0f;
	_fGoalY = 384.0f;

	// trails
	_fTrailLife = 0.0f;
	_nTrailChilds = 0;
	_bTrailRandom = false;
	_bTrailSpinRandom = false;
	
	// appearance attributes
	_fRed = 1.0f;
	_fGreen = 1.0f;
	_fBlue = 1.0f;
	_bTextureRate = false;
	_fTextureRate = 0.0f;
	_fTextureRandom = 1.0f;
	_fTextureLimit = 0.0f;
	_bTextureBySpeed = false;
	_fOpacity = 1.0f;
	_bBlendByAge = false;
	_bBlendBySpeed = false;
	_fBlendFadeIn = 0;
	_bBlendFadeOut = false;
	_bBlendInvert = false;
	_bBlendSmooth = true;
	_fScale = 1.0f;
	_fScaleMin = 1.0f;
	_bScaleByAge = false;
	_bScaleBySpeed = false;
	_fScaleFadeIn = 0.0f;
	_bScaleInvert = false;
	_bScaleSmooth = true;
	_bScaleRandom = false;
	_fReScale = 1.0f;

	// animation
	_fDelay = 0.0f;
	_fEnd = 0.0f;
	_fSplinePos = 0.0f;
	_fEndSplinePos = -1.0f;

	// fast forward
	_bFastForward = false;
	
	_bStarted = true;
	_bUseMouse = true;
	_bPreviewAnimation = false;
	Delete();
}

/*
K_VK_LEFT, K_VK_UP, K_VK_DOWN, K_VK_RIGHT,
K_VK_SPACE, K_VK_L_SHIFT, K_VK_R_SHIFT, K_VK_RETURN, K_VK_R_CONTROL, K_VK_L_CONTROL,
K_VK_F1, K_VK_F2, K_VK_F3, K_VK_F4, K_VK_F5, K_VK_F6, K_VK_F7, K_VK_F8, K_VK_F9, K_VK_F10, K_VK_F11, K_VK_F12,
K_VK_BACK, K_VK_TAB, K_VK_ESCAPE,
K_VK_A, K_VK_B, K_VK_C, K_VK_D, K_VK_E, K_VK_F, K_VK_G, K_VK_H, K_VK_I, K_VK_J, K_VK_K, K_VK_L, K_VK_M, K_VK_N, K_VK_O, K_VK_P,
K_VK_Q, K_VK_R, K_VK_S, K_VK_T, K_VK_U, K_VK_V, K_VK_W, K_VK_X, K_VK_Y, K_VK_Z,
K_VK_0, K_VK_1, K_VK_2, K_VK_3, K_VK_4, K_VK_5, K_VK_6, K_VK_7, K_VK_8, K_VK_9,
K_VK_NUM0, K_VK_NUM1, K_VK_NUM2, K_VK_NUM3, K_VK_NUM4, K_VK_NUM5, K_VK_NUM6, K_VK_NUM7, K_VK_NUM8, K_VK_NUM9, K_VK_MULTIPLY, K_VK_ADD, K_VK_SUBTRACT, K_VK_DECIMAL, K_VK_DIVIDE, K_VK_CLEAR,
K_VK_MENU, K_VK_LWIN, K_VK_RWIN, K_VK_NUMLOCK, K_VK_SCROLL,
K_VK_OEM_1, K_VK_OEM_PLUS, K_VK_OEM_COMMA, K_VK_OEM_MINUS, K_VK_OEM_PERIOD, K_VK_OEM_2, K_VK_OEM_3, K_VK_OEM_4, K_VK_OEM_5, K_VK_OEM_6, K_VK_OEM_7,
K_VK_END, K_VK_HOME, K_VK_DEL, K_VK_INSERT, K_VK_SNAPSHOT, K_VK_PRIOR, K_VK_NEXT,
*/


/* READ/WRITE PRESET */
void CreepingSmokeEmitter::SetAttribute(const char *attname, float f, bool b, const char *strtoken) {
      // trace polygon
		if (strcmp(attname, "_nTracePolygon") == 0) _nTracePolygon = (int)f;

		// fast forward
		if (strcmp(attname, "_bFastForward") == 0) _bFastForward = b;

		// trails
		if (strcmp(attname, "_fTrailLife") == 0) _fTrailLife = f;
		if (strcmp(attname, "_nTrailChilds") == 0) _nTrailChilds = (int)f;
		if (strcmp(attname, "_bTrailRandom") == 0) _bTrailRandom = b;
		if (strcmp(attname, "_bTrailSpinRandom") == 0) _bTrailSpinRandom = b;

		// spiral attributes
		if (strcmp(attname, "_bSpiral") == 0) _bSpiral = b;
		if (strcmp(attname, "_fSpiralRadius") == 0) _fSpiralRadius = f;
		if (strcmp(attname, "_fSpiralRadiusMin") == 0) _fSpiralRadiusMin = f;
		if (strcmp(attname, "_fSpiralRadiusFreq") == 0) _fSpiralRadiusFreq = f;
		if (strcmp(attname, "_fSpiralSpeed") == 0) _fSpiralSpeed = f;
		if (strcmp(attname, "_fSpiralRadiusSpeed") == 0) _fSpiralRadiusSpeed = f;
		if (strcmp(attname, "_fSpiralAnglePower") == 0) _fSpiralAnglePower = f;

		// emission attributes
      if (strcmp(attname, "_fRate") == 0) {
         _fRate = f;
         if (_fMinRate == 0.0f || _fRate < _fMinRate)
            _fMinRate = _fRate/2;
      }
		if (strcmp(attname, "_fMinRate") == 0)
         _fMinRate = f;
		if (strcmp(attname, "_fRateDuration") == 0) _fRateDuration = f;
		if (strcmp(attname, "_fSpeed") == 0) _fSpeed = f;
		if (strcmp(attname, "_fSpeedMin") == 0) _fSpeedMin = f;
		if (strcmp(attname, "_bSpeedStop") == 0) _bSpeedStop = b;	
		if (strcmp(attname, "_bAngleRandom") == 0) _bAngleRandom = b;
		if (strcmp(attname, "_fAngleCtrl") == 0) _fAngleCtrl = f;
		if (strcmp(attname, "_nSpinRandom") == 0) _nSpinRandom = int(f);
		
		// particles attributes
		if (strcmp(attname, "_fLife") == 0) _fLife = f;
		if (strcmp(attname, "_fLifeMin") == 0) _fLifeMin = f;
		if (strcmp(attname, "_fMass") == 0) _fMass = f;
		if (strcmp(attname, "_fMassMin") == 0) _fMassMin = f;
		if (strcmp(attname, "_fInherit") == 0) _fInherit = f;
		if (strcmp(attname, "_fFriction") == 0) _fFriction = f;
		if (strcmp(attname, "_fTwirl") == 0) _fTwirl = f;
		if (strcmp(attname, "_fTwirlMin") == 0) _fTwirlMin = f;
		if (strcmp(attname, "_fTwirlMax") == 0) _fTwirlMax = f;
		if (strcmp(attname, "_fBuoyancy") == 0) _fBuoyancy = f;
		if (strcmp(attname, "_fNoise") == 0) _fNoise = f;
		if (strcmp(attname, "_fNoiseFreq") == 0) _fNoiseFreq = f;
		if (strcmp(attname, "_fNoiseAngleMax") == 0) _fNoiseAngleMax = f;
		if (strcmp(attname, "_fVortex") == 0) _fVortex = f;
		if (strcmp(attname, "_fGravity") == 0) _fGravity = f;
		if (strcmp(attname, "_fWind") == 0) _fWind = f;
		if (strcmp(attname, "_fWindAngle") == 0) _fWindAngle = f;
		if (strcmp(attname, "_fWindDelay") == 0) _fWindDelay = f;
		if (strcmp(attname, "_bForcesFollow") == 0) _bForcesFollow = b;
		
		// appearance attributes
		if (strcmp(attname, "_fRed") == 0) _fRed = f;
		if (strcmp(attname, "_fGreen") == 0) _fGreen = f;
		if (strcmp(attname, "_fBlue") == 0) _fBlue = f;
		if (strcmp(attname, "_bTextureRate") == 0) _bTextureRate = b;
		if (strcmp(attname, "_fTextureRate") == 0) _fTextureRate = f;
		if (strcmp(attname, "_fTextureRandom") == 0) _fTextureRandom = f;
		if (strcmp(attname, "_fTextureLimit") == 0) _fTextureLimit = f;
		if (strcmp(attname, "_bTextureBySpeed") == 0) _bTextureBySpeed = b;
		if (strcmp(attname, "_bTextureRotate") == 0) _bTextureRotate = b;
		if (strcmp(attname, "_fOpacity") == 0) _fOpacity = f;
		if (strcmp(attname, "_bBlendByAge") == 0) _bBlendByAge = b;
		if (strcmp(attname, "_bBlendBySpeed") == 0) _bBlendBySpeed = b;
		if (strcmp(attname, "_fBlendFadeIn") == 0) _fBlendFadeIn = f;
		if (strcmp(attname, "_bBlendFadeOut") == 0) _bBlendFadeOut = b;
		if (strcmp(attname, "_bBlendInvert") == 0) _bBlendInvert = b;
		if (strcmp(attname, "_bBlendSmooth") == 0) _bBlendSmooth = b;
		if (strcmp(attname, "_fScale") == 0) _fScale = f;
		if (strcmp(attname, "_fScaleMin") == 0) _fScaleMin = f;
		if (strcmp(attname, "_bScaleByAge") == 0) _bScaleByAge = b;
		if (strcmp(attname, "_bScaleBySpeed") == 0) _bScaleBySpeed = b;
		if (strcmp(attname, "_fScaleFadeIn") == 0) _fScaleFadeIn = f;
		if (strcmp(attname, "_bScaleInvert") == 0) _bScaleInvert = b;
		if (strcmp(attname, "_bScaleSmooth") == 0) _bScaleSmooth = b;
		if (strcmp(attname, "_bScaleRandom") == 0) _bScaleRandom = b;

		// sprite
		if (strcmp(attname, "_nSpriteDiv") == 0) _nSpriteDiv = int(f);
#ifdef IMAGEBANK_H
		if (strcmp(attname, "_lpSpriteGraphic") == 0) strcpy(_szSpriteFile,strtoken);
#else
		if (strcmp(attname, "_lpSpriteGraphic") == 0) SetSprite(strtoken,_nSpriteDim);
#endif
}

#ifdef IMAGEBANK_H
void CreepingSmokeEmitter::ReadPreset(const char *szPresetPath)
{
	sprintf(_presetName, szPresetPath);
   
   std::string line;
	char att[256], token2[256];
	float f;
	bool b;
   
   ESceneParser::RTextFile presetFile(szPresetPath);
   
   while(presetFile.GetLine(line)){
      int res = sscanf(line.c_str(), "%s", att);
      		
      sscanf (line.substr(line.find_first_of(' '),line.size()).c_str(), "%s", token2);
      if (strcmp(att, "_lpSpriteGraphic") != 0){
		   res = sscanf (token2, "%f", &f);
         if (res != 1)
            continue;
      }
		b = (f == 1)?true:false;
      SetAttribute(att,f,b,token2);
   }
}
#else
void CreepingSmokeEmitter::ReadPreset(const char *szPresetPath)
{
	sprintf(_presetName, szPresetPath);

	char str[256];
	float f;
	bool b;
   FILE *pFile;

   pFile = fopen (szPresetPath, "r");
   if (pFile == NULL){
      KMiscTools::messageBox("File does not exist", szPresetPath);
      return;
   }

	while(!feof(pFile)){
		int res = fscanf(pFile, "%s", str);
      if (strcmp(str, "_lpSpriteGraphic") == 0){
         fscanf (pFile, "%s", str);
         SetAttribute("_lpSpriteGraphic",f,b,str);
      }
      else
         fscanf (pFile, "%f", &f);     
    	b = (f == 1)?true:false;
      SetAttribute(str,f,b,"");
   }
   fclose(pFile); 
}
#endif