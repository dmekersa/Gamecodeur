#ifndef ESEQUENCE_TEXT_FADE_H
#define ESEQUENCE_TEXT_FADE_H

// sequence d'affichage d'un text en fade-in / fade-out

#include "ESequenceBase.h"
#include "KPTK.h"
#include "KSound.h"
#include "KCounter.h"

#define TIME_IN 1500
#define TIME_HOLD 2000
#define TIME_OUT 1000

class ParticleEmitter;

class ESequenceTextFade : public ESequenceBase {

public:
   ESequenceTextFade(int nFromX, int nFromY, const char *szText, int delay = 0, int time_in = TIME_IN, int time_hold = TIME_HOLD, int time_out = TIME_OUT, KSound *lpSoundStream = NULL);
   virtual ~ESequenceTextFade();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   KTextFace *_lpFont;
   KCounter *_lpAnimIn;
   KCounter *_lpAnimHold;
   KCounter *_lpAnimOut;
   KCounter *_lpAnimParticle;
   KGraphic *_lpGraphic;
   KSound *_lpSound;
   KSound *_lpSoundStream;

   ParticleEmitter *_particle;
   float incrX;
   float incrY;

   float valY;
   float valX;

   int m_delay;
   int m_time_in;
   int m_time_hold;
   int m_time_out;
   int _x;
   int _y;
   char _szText[256*4];
};

#endif