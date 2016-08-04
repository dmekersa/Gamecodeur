/*
 *  CreepingSmoke.cpp
 *
 *  Created by Francois Desruelles | TreeX on 02/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 *	Version : 1.1.0 | 26/03/2011
 *
 */


#include "math.h"

#include "CreepingSmoke.h"


CreepingSmoke::CreepingSmoke(int nSpriteDiv, int nSpriteWidth, float fSpinRandom, float fTextureRandom, bool bTrail)
{	
	// number of sprites, in a row/column
	_nSpriteDiv = nSpriteDiv;
	_nSpriteDim = nSpriteWidth;
	_nSpriteCount = _nSpriteDiv * _nSpriteDiv;

	// sprite center
	_fDim = float(_nSpriteDim / 2);

	// basic parameters
	_bTrail = bTrail;
	_fTrailEmit = 0.0f;
	_fTrailOffset = 0.0f;
	_fAge = 0.0f;
	_fSpin = 0.0f;
	_fSpinAngle = 0.0f;
	_fBlend = 0.0f;
	_fNoiseTime = 0.0f;
	_fNoiseAngle = 0.0f;

	// random parameters
	_bSpin = false;
	if (getRandom(-1, 1) < 0)
		_bSpin = true;
	if (fSpinRandom > 0)
		_fSpinAngle = fSpinRandom * getRandom(0, dpi);
	_fTextureRandom = getRandom(0, fTextureRandom);
}

CreepingSmoke::~CreepingSmoke() {}


float c2p(float x, float y)
{
	float a = 0;
	if (x == 0){
		if (y > 0)
			a = hpi;
		else if (y < 0)
			a = -hpi;
	}
	else{
		a = atan(y / x);
		if (x < 0 && y >= 0)
			a += pi;
		else if (x < 0 && y < 0)
			a -= pi;
	}
	return a;
}


void CreepingSmoke::Logic(float fTime)
{
	// life %
	float fLife = _fAge / _fLife;
	// trail age remap
	if (_bTrail)
		fLife = _fTrailOffset + fLife * (1 - _fTrailOffset);

	// translation
	float fMoveX = 0;
	float fMoveY = 0;

	// position
	float fAngle = 0.0f;
	if (_fAge > 0){

		// velocity
		_fVelocity -= _fVelocity * _fFriction;
		_fFriction /= pow(_fAge + 1, 2);
		if (_bSpeedStop)
			_fVelocity = 0;
		fAngle = _fAngle + _fInherit * _fInheritAngle;
		fMoveX = _fVelocity * sin(fAngle);
		fMoveY = _fVelocity * cos(fAngle);

		// noise
		if (_fNoise > 0){
			if (_fNoiseTime > _fNoiseFreq){
				_fNoiseTime = 0.0f;
				_fNoiseAngle = getRandom(-_fNoiseAngleMax, _fNoiseAngleMax);
			}
			if (_fNoiseFreq == 0) // for old behaviour : v1.0.9
				fAngle = getRandom(0, dpi);
			fAngle = _fAngle + _fNoiseAngle * sin(pi * _fNoiseTime / _fNoiseFreq);
			fMoveX += _fMass * _fNoise * cos(fAngle);
			fMoveY += _fMass * _fNoise * sin(fAngle);
			_fNoiseTime += fTime;
		}
		
		// gravity
		if (_fGravity > 0)
			fMoveY += _fMass * _fGravity * pow(_fAge, 2);
		// wind
		if (_fWind > 0 && _fAge > _fWindDelay){
			fMoveX += _fMass * _fWind * sin(_fWindAngle);
			fMoveY += _fMass * _fWind * -cos(_fWindAngle);
		}
		// vortex
		if (_fVortex != 0){
			float x = _fPosX - _fVortexX + _fDim;
			float y = _fPosY - _fVortexY + _fDim;
			float r = sqrt(pow(x, 2) + pow(y, 2));
			float a = c2p(x, y) + hpi;
			float p = r * _fVortex * 0.01f * _fMass;
			fMoveX += p * cos(a);
			fMoveY += p * sin(a);
		}
		// goal
		if (_fGoal != 0){
			fMoveX += _fMass * _fGoal * (_fGoalX - _fPosX - _fDim) / _nWindowWidth;
			fMoveY += _fMass * _fGoal * (_fGoalY - _fPosY - _fDim) / _nWindowHeight;
		}
		// trail
		if (_bTrail){
			fMoveX *= _fInherit;
			fMoveY *= _fInherit;
		}
		_fPosX += fMoveX;
		_fPosY += fMoveY;
	}
	else if (!_bTrail){
		// init
		_fPosX -= _fDim;
		_fPosY -= _fDim;

		// noise
		_fNoiseFreq = 1 / _fNoiseFreq;
		_fNoiseAngle = getRandom(-_fNoiseAngleMax, _fNoiseAngleMax);

		// scale
		if (_bScaleRandom)
			_fSize = getRandom(_fScaleMin, _fScale);
		else
			_fSize = _fScale;

		// velocity
		_fVelocity = _fSpeed;
	}


	// angle
	// forces follow
	if (_bForcesFollow){
		if (!_bTrail){
			if (_fAge > 0){
				_fSpin = atan(fMoveX / fMoveY);
				if (fMoveY < 0)
					_fSpin -= pi;
				if (_fVortex != 0)
					_fSpin += _fAngle;
				_fSpin *= r2d;
			}
			else
				_fSpin = (_fAngle + _fSpinAngle) * r2d;
		}
	}

	// rotation & spin
	else{
		float fRotate = getRandom(_fTwirlMin,_fTwirlMax);
		if (_bSpin){
			_fAngle += _fTwirl * fRotate;
			if (_fBuoyancy > 0)
				_fSpinAngle += _fBuoyancy * fRotate;
		}
		else{
			_fAngle -= _fTwirl * fRotate;
			if (_fBuoyancy > 0)
				_fSpinAngle -= _fBuoyancy * fRotate;
		}
		_fSpin = (_fAngle + _fSpinAngle) * r2d;
	}

	// speed %
	float fSpeed = _fVelocity / _fSpeed;


	// texture
	int num = 0;
	if (_bTextureRate){
		float fAge = _fTextureRandom + _fAge * _fTextureRate;
		num = int(floor(_nSpriteCount * (fAge - floor(fAge))));
	}
	else if (_bTextureRotate){
		fAngle = _fSpin - floor(_fSpin / 360) * 360;
		num = int(floor(_nSpriteCount * fAngle / 360));
		if (fAngle < 0)
			num = _nSpriteCount - num;
		_fSpin = 0;
	}
	else if (_bTextureBySpeed)
		num = int(floor(_nSpriteCount * fSpeed));
	else
		num = int(floor(_nSpriteCount * fLife));
	// trail separation
	if (_nTextureLimit > 0){
		if (_bTrail)
			num = _nTextureLimit + num % (_nSpriteCount - _nTextureLimit);
		else
			num = num % _nTextureLimit;
	}
	_fTex[0] = floor(float(num % _nSpriteDiv)) * _nSpriteDim;
	_fTex[1] = floor(float(num / _nSpriteDiv)) * _nSpriteDim;
	_fTex[2] = _fTex[0] + _nSpriteDim;
	_fTex[3] = _fTex[1] + _nSpriteDim;
	

	// scale
	if (fLife < _fScaleFadeIn){
		if (_bScaleSmooth)
			_fScale = sin(fLife / _fScaleFadeIn * pi / 2);
		else
			_fScale = fLife / _fScaleFadeIn;
	}
	else if (_bScaleByAge){
		if (_bScaleSmooth)
			_fScale = 1 - sin((fLife - _fScaleFadeIn) / (1 - _fScaleFadeIn) * pi / 2);
		else
			_fScale = 1 - (fLife - _fScaleFadeIn) / (1 - _fScaleFadeIn);
	}
	else
		_fScale = 1;
	if (_bScaleBySpeed)
		_fScale *= fSpeed;
	if (_bScaleInvert)
		_fScale = _fSize - _fScaleMin * _fScale;
	else
		_fScale = _fScaleMin + (_fSize - _fScaleMin) * _fScale;


	// blend
	if (fLife < _fBlendFadeIn){
		if (_bBlendSmooth)
			_fBlend = sin(fLife / _fBlendFadeIn * pi / 2);
		else
			_fBlend = fLife / _fBlendFadeIn;
	}
	else if (_bBlendByAge){
		if (_bBlendSmooth)
			_fBlend = 1 - sin((fLife - _fBlendFadeIn) / (1 - _fBlendFadeIn) * pi / 2);
		else
			_fBlend = 1 - (fLife - _fBlendFadeIn) / (1 - _fBlendFadeIn);
	}
	else if (_bBlendFadeOut){
		if (fLife > 1 - _fBlendFadeIn){
			if (_bBlendSmooth)
				_fBlend = sin((1 - fLife) / _fBlendFadeIn * pi / 2);
			else
				_fBlend = (1 - fLife) / _fBlendFadeIn;
		}
	}
	else
		_fBlend = 1;
	if (_bBlendBySpeed)
		_fBlend *= fSpeed;
	if (_bBlendInvert)
		_fBlend = 1 - _fBlend;
	_fBlend *= _fOpacity;


	// life in sec
	_fAge += fTime;
	_fTrailEmit += fTime;
}