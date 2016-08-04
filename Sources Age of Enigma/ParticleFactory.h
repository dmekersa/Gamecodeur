#pragma once
/* Include definitions */
#include "KGame.h"
#include "KPTK.h"

/* currently enum, could be also as many children of main ParticleEmitter as type of effect */ 
enum TypeOfEffect {
	VFX_FIRE,
	VFX_SMOKE,
	VFX_EXPLOSION,
	VFX_SPRAY,
	VFX_TRAIL,
	VFX_MOVING,
	VFX_FLAKE,
	VFX_FOCUS
};

/* this is a list of points (KVector2) used to describe a curve */ 
class Vertice : public KObjectListable {
public:
	Vertice(void);
	Vertice (float x, float y); 
	~Vertice(void);
	float Getx(void);
	float Gety(void);
	long GetIndex(void); 
	void SetIndex(long i); 

private:
	KVector2 *Point2D;
	long	 fIndex;
};

/* a segment is made of an origin and a destination */
class Segment : public KObjectListable {
public:
	Segment (float x1, float y1, float x2, float y2, float fIncrement);
   ~Segment(void);

	float fX1, fX2, fY1, fY2;
	KList<Vertice> SampledSegment; /* list of points to walk through the segment */ 

private:
	float length (void);
	void Sampling(float lIntervalLength); /* generate the Sampled Segment based on lIntervalLength, length of a single interval = sampled piece of segment */ 
	
};

/* One particle */
class VfxParticle : public KObjectListable {
public:
   /* Current center coordinates */
   float x, y;

   /* Velocity over x, y */
   float vx, vy;

   /* Color */
   float fColor[4];

   /* Scale */
   float fScale;

   /* Current age */
   float fAge;

   /* Age increase at each tick */
   float fAgeIncr;
};

/* Emitter in charge of emitting particles of a given type */ 
class ParticleEmitter : public KObjectListable {
public:
	ParticleEmitter(void);
	ParticleEmitter(float x, float y);
	~ParticleEmitter(void);
 
	void SetEmitRate(long x);

	void SetType (long type) {lTypeOfEffect = type; };

	bool SetParticleBitmap(const char *lpszBitmapName);

	void SetParticleAge(float x);
	
	void SetHeightEffect(long x);
	
	void SetPosition(float x, float y);

	void Move(float x, float y);

	void Init(void); /* initial start of particle emission, from source */

	void Logic(void); /* particle emission to next positions */

	void Stop(void); /* stop emitting only */ 

	void Kill(void); /* stop emitting and particles already emitted */ 

	void Start(void); /* restart emitting and display */ 

	bool IsKilled(void); /* check if Kill() has been invoked */

	bool IsStarted(void); /* check if Kill() has been invoked */

	void Render(double fElapsed);  /* display effect */
	
	void AddSegment(float x1, float y1, float x2, float y2, float increment); /* add segment to Polygon */

	void AddSegment(Segment *s); /* add segment to Polygon */

	float		fMaxEmitter; /* to be adapted to the length of segments/polygon : longer => higher */
	float		fProgressPerFrame;
	float		fDisplayEveryLogic;
	KCounter	*lpProgress;
   KGraphic	*GetParticleBitmap(void);
	KList<VfxParticle> g_lParticles;
	KBatch *	lpBatch;

private:
	KGraphic *lpParticleGraphic;   
	float fAge;
	long lRate;
	long lTypeOfEffect;
	float fX, fY;
    KList<Segment> Polygon;	
	bool bStart; 
	bool  bKilled;

};

class ParticleFactory {
public:
	ParticleFactory(void);
	~ParticleFactory(void);
	void Render(double fElapsed);
	void Logic(void);
	void AddEmitter(ParticleEmitter *pe);

	KList<ParticleEmitter> lParticlesEmitter;
};


