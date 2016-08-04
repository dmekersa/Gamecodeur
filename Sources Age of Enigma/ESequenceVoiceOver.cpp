/*
 *  ESequenceVoiceOver.cpp
 *  enigma
 *
 *  Created by Rockford on 05/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "ESequenceVoiceOver.h"
#include "MyGame.h"
#include "EPointer.h"
#include "FontBank.h"
#include "ImageBank.h"
#include "ESceneDirector.h"

/*
 szText: text à afficher
 szVoice: nom du fichier son à jouer
 */
ESequenceVoiceOver::ESequenceVoiceOver(const char *szText, const char *szVoice, SequenceVoiceOverColor Color)
{
   /* Load font */
//   EGLL();
//   switch (Color) {
//      case SEQUENCE_VOICEOVER_RED:
//         _lpFont = EFontBank::getBitmapFont("jancientred22");
//         break;
//      case SEQUENCE_VOICEOVER_GREY:
//         _lpFont = EFontBank::getBitmapFont("jancientgrey22");
//         break;
//      case SEQUENCE_VOICEOVER_BROWN:
         _lpFont = EFontBank::getFont("jancient22");
//         break;
//      default:
//         _lpFont = EFontBank::getBitmapFont("jancientgrey22");
//         break;
//   }
//   EGLR();
   
   /* Graphic */
   _lpGraphic = KPTK::createKGraphic();
   
   /* String cleaning */
   memset(_szText, 0, 256*4);
   memset(_szVoice, 0, 256);
   
   /* Stockage des arguments */
   strcpy(_szText, szText);
   strcpy(_szVoice, szVoice);
   _Color = Color;
   
   _lpAnimIn = NULL;
   _lpAnimOut = NULL;
   _lpSound = NULL;
}

ESequenceVoiceOver::~ESequenceVoiceOver()
{
   if (_lpAnimIn) delete _lpAnimIn;
   if (_lpAnimOut) delete _lpAnimOut;
   delete _lpGraphic;
   delete _lpFont;
}

void ESequenceVoiceOver::Logic()
{
}

void ESequenceVoiceOver::Check()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   if (_lpAnimIn) {
      if (_lpAnimIn->isCompleted() && !_lpAnimOut) {
         _lpAnimOut = new KCounter();
         _lpAnimOut->startCounter(1, 0, strlen(_szText)*SEQUENCE_TEXT_CHARDURATION, 1000, K_COUNTER_EASEIN);
      } else {
         _lpAnimIn->move(fElapsed);      
      }
   }
   if (_lpAnimOut) _lpAnimOut->move(fElapsed);

   // Si le bouton est pressé
   if (Mouse.LeftEvent())
   {
      /* Shut up :) */
      if (_lpSound) {
         _lpSound->stopSample();
      }
      Mouse.LeftEventClose();
      
      if (!_lpAnimOut) {
         _lpAnimIn->move(99999);
         _lpAnimOut = new KCounter();
         _lpAnimOut->startCounter(1, 0, 0, 500, K_COUNTER_EASEIN);
      }
      else {
         _lpAnimOut->move(9999);
      }
   }
}

void ESequenceVoiceOver::Draw()
{
   float fBlend;
   long h;

   if (!_lpAnimIn) {
      return;
   }
   if (!_lpAnimIn->isCompleted()) {
      fBlend = _lpAnimIn->getCurrentValue();
   } else {
      if (_lpAnimOut) {
         fBlend = _lpAnimOut->getCurrentValue();
      } else {
         fBlend = 1.0;
      }
   }
   
   _lpFont->setColor(1, 1, 1, 1);
   _lpFont->setBlend(fBlend);

   float fImgH = 100;
   KGame::getGame()->_lpKWindow->setWorldView(0,(fImgH+10.0f)-((fImgH+10.0f)*fBlend),0,1,false);
   if (!SCENEDIRECTOR->isCinematicMode()) {
      if (_Color == SEQUENCE_VOICEOVER_RED) {
         KGraphic *lpBg = EImageBank::getImage("voiceover_red");
         lpBg->blitAlphaRect(0, 0, 1024, 100, 0, 0);
      }
      else {
         _lpGraphic->drawRect(0, 0, 1024, (float)fImgH+10, .1f, .1f, .1f, fBlend); //*0.75);
      }
   }
   _lpFont->setBlend(fBlend);
   h = _lpFont->getMultilineHeight(_szText, 15, 1024-15, 5, TEXTSTYLE_CENTER, -1, 23);
   _lpFont->drawMultiline(_szText, 15, 1024-15, ((100+10) - h)/2, TEXTSTYLE_CENTER,-1,23);
   KGame::getGame()->_lpKWindow->setWorldView(0,0,0,1,false);
}

bool ESequenceVoiceOver::isCompleted()
{
   if (!_lpAnimIn || !_lpAnimOut) {
      return false;
   }
   if (strlen(_szVoice) > 0)
   {
      _lpSound = ESoundBank::getSound(_szVoice);
      if (_lpSound->isPlaying()) {
         return false;
      }
   }
   return _lpAnimIn->isCompleted() && _lpAnimOut->isCompleted();
}

void ESequenceVoiceOver::Start()
{
   ESequenceBase::Start();
   
   _lpAnimIn = new KCounter();
   _lpAnimIn->startCounter(0, 1, 0, 500, K_COUNTER_EASEOUT);
   
   if (strlen(_szVoice) > 0)
   {
      _lpSound = ESoundBank::getSound(_szVoice);
      if (_lpSound) {
         _lpSound->playSample();
      }
   }
   else {
      ESoundBank::getSound("talkdialog")->playSample();
   }

}
