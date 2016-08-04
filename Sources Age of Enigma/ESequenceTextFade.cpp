/*
 *  ESequenceTextFade.cpp
 *  enigma
 *
 *  Created by Rockford on 10/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "ESequenceTextFade.h"
#include "MyGame.h"
#include "SoundBank.h"
#include "FontBank.h"
#include "ImageBank.h"
#include "global.h"
#include "EPointer.h"
#include "ESceneDirector.h"

// par rapport au _x et _y
#define POS_FINAL_PARTICLE_X -420.0f
#define POS_FINAL_PARTICLE_Y 500.0f

/*
 szText: text à afficher
 szVoice: nom du fichier son à jouer
 */
ESequenceTextFade::ESequenceTextFade(int X, int Y, const char *szText, int delay, int time_in, int time_hold, int time_out, KSound *lpSoundStream)
{
   /* Load font */
//   _lpFont = EFontBank::getFont("tahoma.ttf", 20);
   _lpFont = EFontBank::getFont("jancientgrey22",-4);
   
   /* Graphic */
   _lpGraphic = KPTK::createKGraphic();
   
   /* String cleaning */
   memset(_szText, 0, 256*4);
   
   /* Stockage des arguments */
   strcpy(_szText, szText);

   _x = X;
   _y = Y;

   valY = (_y + POS_FINAL_PARTICLE_Y) / (_x + (-POS_FINAL_PARTICLE_X));
   valX = (_x + (-POS_FINAL_PARTICLE_X)) / (_y + POS_FINAL_PARTICLE_Y);

   m_delay = delay;
   m_time_in = time_in;
   m_time_hold = time_hold;
   m_time_out = time_out;
   _particle = NULL;
   _lpAnimIn = NULL;
   _lpAnimHold = NULL;
   _lpAnimOut = NULL;
   _lpAnimParticle = NULL;
   _lpSound = NULL;
   _lpSoundStream = lpSoundStream;

   incrX = 0;
   incrY = 0;
}

ESequenceTextFade::~ESequenceTextFade()
{
   XDELETE(_lpGraphic);
   XDELETE(_lpAnimIn);
   XDELETE(_lpAnimOut)
   XDELETE(_lpAnimHold);
   if(_particle)
      _particle->Kill();
   XDELETE(_particle);
   XDELETE(_lpFont);
}

void ESequenceTextFade::Logic()
{
   if(_particle)
      _particle->Logic();
}

void ESequenceTextFade::Check()
{
}

void ESequenceTextFade::Draw()
{     
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();

   if(_lpAnimIn && _lpAnimIn->isCompleted()) {
      XDELETE(_lpAnimIn);
      
      // le temps que le texte va rester affiché
      _lpAnimHold = new KCounter();
      _lpAnimHold->startCounter(0, 1, 0, m_time_hold, K_COUNTER_LINEAR);
   }
   if(_lpAnimHold && _lpAnimHold->isCompleted()) {
      XDELETE(_lpAnimHold);
      
      // disparition du texte
      _lpAnimOut = new KCounter();
      _lpAnimOut->startCounter(1, 0, 0, m_time_out, K_COUNTER_EASEIN);
   }
   if(_lpAnimOut && _lpAnimOut->isCompleted()) {
      XDELETE(_lpAnimOut);
      _particle = new ParticleEmitter(0, 0);
      if (_particle->SetParticleBitmap("particule-etoile2")) {
         _particle->SetEmitRate(1);
         _particle->SetType(VFX_TRAIL);
         _particle->AddSegment(-10, -10, 10, 10, 1);
         _particle->SetPosition(_x + 50, _y);
         _particle->Start();
      }
   }
   
   if(_particle) {
      _particle->Render(fElapsed);
      if(!isCompleted()) {
         _particle->SetPosition(_x + incrX, _y + incrY);
         incrX -= valX*(fElapsed/2); incrY += valY*(fElapsed/2);
      }
      return;
   }

   if(_lpAnimIn)
      _lpAnimIn->move(fElapsed);
   else if(_lpAnimHold)
      _lpAnimHold->move(fElapsed);
   else if(_lpAnimOut)
      _lpAnimOut->move(fElapsed);

   float alpha = 0.0f;
   int widthCh = _lpFont->getStringWidth(_szText);

   
   if(_lpAnimIn)
      alpha = _lpAnimIn->getCurrentValue();
   else if(_lpAnimHold)
      alpha = 1.0f;
   else if(_lpAnimOut) {
      if(!_lpAnimOut->isCompleted())
         alpha = _lpAnimOut->getCurrentValue();
      else
         return;
   }
   _lpFont->setColor(1.0f, 1.0f, 1.0f, alpha);

   //int marge = 5;
   //int epaisseurRectFond = 2;
   
//   _lpGraphic->drawRect(_x - (widthCh/2) - marge - epaisseurRectFond, _y - (_lpFont->getHeightPix() /2) - marge - epaisseurRectFond, _x + (widthCh/2) + marge + epaisseurRectFond, _y + (_lpFont->getHeightPix() /2) + marge + epaisseurRectFond, 1.0f, 1.0f, 1.0f, alpha/2);
//   _lpGraphic->drawRect(_x - (widthCh/2) - marge, _y - (_lpFont->getHeightPix() /2) - marge, _x + (widthCh/2) + marge, _y + (_lpFont->getHeightPix() /2) + marge, 0.1f, 0.1f, 0.1f, alpha);
   
   _lpFont->drawStringCentered(_szText, 0, 1024, _y - (_lpFont->getHeightPix() /2));
   SCENEDIRECTOR->GetCurrentScene()->DrawFancyString(_szText, _lpFont, _x - (widthCh/2), _y - (_lpFont->getHeightPix() /2), 0.0f, false, alpha);
}

bool ESequenceTextFade::isCompleted()
{
   if(incrX <= POS_FINAL_PARTICLE_X) {
      if(_particle)
         _particle->Stop();
      return true;
   }
   return false;
 //  if(_lpAnimParticle)
  //    return _lpAnimParticle->isCompleted();
 //  if(_lpAnimOut)
   //   return _lpAnimOut->isCompleted();
 //  else
   //   return false;
}

void ESequenceTextFade::Start()
{
   XDELETE(_lpAnimOut)
   XDELETE(_lpAnimHold);

   _lpAnimIn = new KCounter();
   _lpAnimIn->startCounter(0, 1, m_delay, m_time_in, K_COUNTER_EASEOUT);
}
