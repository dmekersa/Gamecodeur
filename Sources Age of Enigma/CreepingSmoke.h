/*
 *  CreepingSmoke.h
 *
 *  Created by Francois Desruelles | TreeX on 02/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 *	Version : 1.0 | 01/10/2010
 *
 */


/*	
	todos
	* spline/nurbs non-linear interpolation
	* use of GBezier.h ?
	* more accurate inheritance angle calculation for mouse
*/


#include "KPTK.h"
#include "KWindow.h"
#include "KMiscTools.h"

#ifndef pi
#define pi float(3.14159265358979323846)
#endif

#ifndef hpi
#define hpi (pi/2)
#endif

#ifndef dpi
#define dpi (pi*2)
#endif

#ifndef r2d
#define r2d (180/pi)
#endif

#ifndef CS_H
#define CS_H

/* Get random number in the defined range */
inline float getRandom(float fMin, float fMax) {return float(fMin+(fMax-fMin)*rand()/(RAND_MAX + 1.0));}


class CreepingSmoke {
   
public:

	CreepingSmoke(int nSpriteDiv, int nSpriteWidth, float fSpinRandom, float fTextureRandom, bool bTrail);
	~CreepingSmoke();


	float		_fVelocity;
	bool		_bSpin;
	float		_fSize;
	

	float		_fAge;
	float		_fLife;
	float		_fMass;
	float		_fSpeed;
	bool		_bSpeedStop;
	float		_fPosX;
	float		_fPosY;
	float		_fAngle;
	float		_fSpin;
	float		_fSpinAngle;
	
	// dynamic attributes
	float		_fInherit;
	float		_fInheritAngle;
	float		_fFriction;
	float		_fTwirl;
	float		_fTwirlMin;
	float		_fTwirlMax;
	float		_fBuoyancy;
	float		_fNoise;
	float		_fNoiseFreq;
	float		_fNoiseAngleMax;
	float		_fGravity;
	float		_fWind;
	float		_fWindAngle;
	float		_fWindDelay;
	bool		_bForcesFollow;

	// vortex
	float		_fVortexX;
	float		_fVortexY;
	float		_fVortex;

	// goal
	int			_nWindowWidth;
	int			_nWindowHeight;
	float		_fGoal;
	float		_fGoalX;
	float		_fGoalY;

	// trail
	bool		_bTrail;
	float		_fTrailEmit;
	float		_fTrailOffset;

	// appearance attributes
	bool		_bTextureRate;
	float		_fTextureRate;
	float		_fTextureRandom;
	int			_nTextureLimit;
	bool		_bTextureBySpeed;
	bool		_bTextureRotate;
	float		_fOpacity;
	float		_fBlend;
	bool		_bBlendByAge;
	bool		_bBlendBySpeed;
	float		_fBlendFadeIn;
	bool		_bBlendFadeOut;
	bool		_bBlendInvert;
	bool		_bBlendSmooth;
	float		_fScale;
	float		_fScaleMin;
	bool		_bScaleByAge;
	bool		_bScaleBySpeed;
	float		_fScaleFadeIn;
	bool		_bScaleInvert;
	bool		_bScaleSmooth;
	bool		_bScaleRandom;
	float		_fTex[4];
	
	void		Logic(float fTime);


private:

	int			_nSpriteDiv;
	int			_nSpriteDim;
	float		_fDim;
	int			_nSpriteCount;

	float		_fNoiseTime;
	float		_fNoiseAngle;
	
};


class CreepingSmokeParticle : public KObjectListable {

public:
   CreepingSmokeParticle() {
      _lpParticle = NULL;
   }
   ~CreepingSmokeParticle () {
      delete _lpParticle;
      _lpParticle = NULL;
   }
	CreepingSmoke *_lpParticle;
};

class point : public KObjectListable {

public:

	float	_x;
	float	_y;
};

class keyframe : public KObjectListable {

public:

	float	_fTime;
	float	_fPos;
};



class CreepingSmokeEmitter {
   
public:

	CreepingSmokeEmitter();
	~CreepingSmokeEmitter();

	static int	particlesCount;
	static float rateFactor;

	bool		_bStarted;
	float		_fTime;

	float		_fPosX;
	float		_fPosY;
	float		_fAngle;

	// inherit attributes
	float		_fInheritPosX;
	float		_fInheritPosY;

	// spiral attributes
	bool		_bSpiral;
	float		_fSpiralRadius;
	float		_fSpiralRadiusMin;
	float		_fSpiralRadiusFreq;
	float		_fSpiralRadiusSpeed;
	float		_fSpiralSpeed;
	float		_fSpiralAnglePower;
	float		_fSpiralAngle;
	float		_fSpiralTime;

	// emission attributes
	float		_fRate;
	float		_fMinRate;
	float		_fRateDuration;
	float		_fSpeed;
	float		_fSpeedMin;
	bool		_bSpeedStop;
	bool		_bAngleRandom;
	float		_fAngleCtrl;
	int			_nSpinRandom;
	bool		_bUseMouse;
	bool		_bUseKeys;
	
	// particles attributes
	float		_fLife;
	float		_fLifeMin;
	float		_fMass;
	float		_fMassMin;
	float		_fInherit;
	float		_fFriction;
	float		_fTwirl;
	float		_fTwirlMin;
	float		_fTwirlMax;
	float		_fBuoyancy;
	float		_fNoise;
	float		_fNoiseFreq;
	float		_fNoiseAngleMax;
	float		_fVortex;
	float		_fGravity;
	float		_fWind;
	float		_fWindAngle;
	float		_fWindDelay;
	bool		_bForcesFollow;

	// goal
	int			_nWindowWidth;
	int			_nWindowHeight;
	float		_fGoal;
	float		_fGoalX;
	float		_fGoalY;

	// trails
	float		_fTrailLife;
	int			_nTrailChilds;
	bool		_bTrailRandom;
	bool		_bTrailSpinRandom;

	// appearance attributes
	float		_fRed;
	float		_fGreen;
	float		_fBlue;
	bool		_bTextureRate;
	float		_fTextureRate;
	float		_fTextureRandom;
	float		_fTextureLimit;
	bool		_bTextureBySpeed;
	bool		_bTextureRotate;
	float		_fOpacity;
	bool		_bBlendByAge;
	bool		_bBlendBySpeed;
	float		_fBlendFadeIn;
	bool		_bBlendFadeOut;
	bool		_bBlendInvert;
	bool		_bBlendSmooth;
	float		_fScale;
	float		_fScaleMin;
	bool		_bScaleByAge;
	bool		_bScaleBySpeed;
	float		_fScaleFadeIn;
	bool		_bScaleInvert;
	bool		_bScaleSmooth;
	bool		_bScaleRandom;
	float		_fReScale;

	// sprites
	char		_szSpriteFile[256];
	KGraphic	*_lpSpriteGraphic;
	int			_nSpriteDiv;
	int			_nSpriteDim;

	// particles
	KList<CreepingSmokeParticle>	_particles;

	// trails
	KList<CreepingSmokeParticle>	_trails;

	// preview animation
	bool		_bPreviewAnimation;

	// animation
	float		_fDelay;
	float		_fEnd;
	KList<point>	points;
	KList<keyframe>	keyframes;
	float		_fSplinePos;
	
	// fill polygon
	bool		_bFillPolygon;

	// trace polygon
	int			_nTracePolygon;

	// fast forward
	bool		_bFastForward;

	// methods
	void		SetWindow(KWindow *lpWindow);
	void		SetSprite(const char *szSpritePath, int nSpriteDiv);
	void		ResetSpriteDim();
	void		SetGoal(float fGoal, float fGoalX, float fGoalY);
	void		Move(float fPosX, float fPosY);
	void		MovePolygon(float fPosX, float fPosY);
	void		ReScale(float fScale);
	void		Logic();
	void		Render();
	void		Start();
	void		Stop();
	void		Delete();
	bool		IsCompleted();
	bool     IsStarted() { return _bStarted; }
	void		ReadPreset(const char *szPresetPath);
	void		SetAttribute(const char *attname, float f, bool b, const char *strtoken);
	void		WritePreset(const char *szPresetPath);
	void		ReadAnimation(const char *szPresetPath);
	void		GetPosition();
	void		Reset();

	bool		_bDisplay;
	char		_attribute[256];
	char		_debug[256];
	float		_fDebug;

	char		_presetName[128];


private:

	bool		_bIsActive;
	bool		_bKeyState;
	
	float		_fElaspedTime;
	bool		_bEmit;
	int			_nParticles;

	float		_fSpiralPosX;
	float		_fSpiralPosY;
	float		_fEndSplinePos;

	int			_nTracePolygonPoint;

	bool		_bDisplayParameters;
	char		_szParameters[2000];

	KWindow		*_lpWindow;
	KBatch		*_lpBatch;
	KTextFace	*_lpFontCount;
	KTextFace	*_lpFontParameters;
	KTextFace	*_lpFontAttribute;
	KTextFace	*_lpFontDebug;
	bool		_bFontColor;

	// polygon bouding box
	bool		_bBBdefined;
	float		_fMinX;
	float		_fMaxX;
	float		_fMinY;
	float		_fMaxY;
	// polygon offset
	float		_fPolygonOffsetX;
	float		_fPolygonOffsetY;
	
	// methods
	void		Rate(bool bDown);
	void		Spiral();
	void		Emit();
	void		EmitTrail(CreepingSmokeParticle *p);
	void		GetAnimation();
	void		FillPolygon();
	void		TracePolygon();
	void		PolygonBB();
	void		ListParameters();
	void		ListKeys();
	void		Keys();

};

#endif

/* KEYS */
/*

	# toggle use mouse	ESCAPE
	# start/stop		,
	
	List parameters		return + ctrl
	list keys			return

	_bFillPolygon		back
	_nTracePolygon		back + ctrl

	_bFastForward		end

	_fRate				F8 F9 (+ ctrl)
	_fRateDuration		F8 F9 + shift

	_bSpiral			del
	_fSpiralRadius		rmb lmb
	_fSpiralRadiusMin	rmb lmb + ctrl
	_fSpiralRadiusFreq	wheel + ctrl (mmb = floor)
	_fSpiralSpeed		wheel + shift
	_fSpiralRadiusSpeed wheel (mmb = sync)
	_fSpiralAnglePower	rmb lmb + shift

	_fMass				a z
	_fMassMin			A Z
	_fMassMin=_fMass	TAB

	_fLife				w x (+ ctrl)
	_fLifeMin			W X
	_fLifeMin=_fLife	F6

	_fSpeed				down up
	_fSpeedMin			DOWN UP
	_fSpeedMin=_fSpeed	F7
	_bSpeedStop			F7 + ctrl

	_bAngleRandom		space
	_fAngleCtrl			LEFT RIGHT
	_nSpinRandom		C V

	_fFriction			left right (+ ctrl)
	
	_fInherit			j k

	_fTwirl				t y
	_fTwirlMin			e r
	_fTwirlMax			d f

	_fBuoyancy			c v	(+ ctrl)

	_fNoise				b n
	_fNoiseFreq			B N
	_fNoiseAngleMax		b n + ctrl

	_fGravity			O P

	_fWind				l m
	_fWindAngle			o p
	_fWindDelay			L M

	_fVortex			j k + ctrl

	_bForcesFollow		f + ctrl
	
	R G B				F1-F6 + ctrl

	_fTrailLife			o p + crtl
	_nTrailChilds		l m + ctrl
	_bTrailRandom		home + ctrl
	_bTrailSpinRandom	home + shift

	_bTextureRate		home
	_fTextureRate		prior next
	_fTextureRandom		prior next + ctrl
	_fTextureLimit		PRIOR NEXT
	_bTextureBySpeed	F10
	_bTextureRotate		F10 + ctrl

	_fOpacity			G H
	_bBlendByAge		F1
	_bBlendBySpeed		F2
	_fBlendFadeIn		g h
	_bBlendFadeOut		h + ctrl
	_bBlendInvert		g + ctrl
	_bBlendSmooth		F1 + shift

	_fScale				q s
	_fScaleMin			Q S
	_fScaleByAge		F3
	_fScaleBySpeed		F4
	_fScaleFadeIn		u i
	_bScaleInvert		q + ctrl
	_bScaleRandom		s + ctrl
	_bScaleSmooth		F3 + shift
	_fScaleMin=_fScale	F5
	_fReScale			a z + ctrl

	Reset()				F10 + ctrl

*/