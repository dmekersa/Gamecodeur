/*
 *  ESequenceTalk.cpp
 *  enigma
 *
 *  Created by Rockford on 10/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "ESequenceTalk.h"
#include "MyGame.h"
#include "SoundBank.h"
#include "FontBank.h"
#include "ImageBank.h"
#include "global.h"
#include "EPointer.h"
#include "ESceneDirector.h"

/*
 szText: text à afficher
 szVoice: nom du fichier son à jouer
 */
ESequenceTalk::ESequenceTalk(int nFromX, int nFromY, const char *szText, const char *szVoice, bool bCheckAllowed, bool bDialogMode, KSound *lpSoundStream)
{
   /* Load font */
//   _lpFontMainChar = EFontBank::getFont("OldNewspaperTypes.ttf", 22);
   _lpFontMainChar = EFontBank::getFont("jancient22");
   _lpFontPNJ = EFontBank::getFont("jancient22"); //EFontBank::getFont("OldNewspaperTypes.ttf", 20);
   _lpFontMini = EFontBank::getFont("OldNewspaperTypes.ttf", 15);
   
   /* Graphic */
   _lpGraphic = KPTK::createKGraphic();
   _lpGraphicBulle = EImageBank::getImage("ihm_bulle");
   if (nFromX == CHARACTER_POSX) {
      _bFlip = true;
   } else {
      _bFlip = false;
   }
   
   /* String cleaning */
   memset(_szText, 0, 256*4);
   memset(_szVoice, 0, 256);
   
   /* Stockage des arguments */
   strcpy(_szText, szText);
   strcpy(_szVoice, szVoice);
   _bCheckAllowed = bCheckAllowed;
   _bDialogMode = bDialogMode;
   // Test de débordement pour X
   _nFromX = nFromX;
   if (_nFromX - (SEQUENCE_TALK_BOXWIDTH / 2) < 0)
      _nFromX = SEQUENCE_TALK_BOXMARGIN + (SEQUENCE_TALK_BOXWIDTH / 2);
   if (_nFromX + (SEQUENCE_TALK_BOXWIDTH / 2) > 1024)
      _nFromX = 1024 - (SEQUENCE_TALK_BOXWIDTH / 2) - SEQUENCE_TALK_BOXMARGIN;
   _nFromY = nFromY;
   if (_nFromY - SEQUENCE_TALK_BOXMARGIN - SEQUENCE_TALK_BOXHEIGHT < 0)
      _nFromY = 0 + SEQUENCE_TALK_BOXMARGIN + SEQUENCE_TALK_BOXHEIGHT;
   
   _lpAnimIn = NULL;
   _lpAnimHold = NULL;
   _lpAnimOut = NULL;
   _lpSound = NULL;
   _lpSoundStream = lpSoundStream;
}

ESequenceTalk::~ESequenceTalk()
{
   XDELETE(_lpAnimIn);
   XDELETE(_lpAnimHold);
   XDELETE(_lpAnimOut);
   XDELETE(_lpGraphic);
   if (_lpSound) {
      _lpSound->stopSample();
   }
   if (_lpSoundStream) {
      _lpSoundStream->setVolume(0);
   }
   XDELETE(_lpFontMainChar);
   XDELETE(_lpFontPNJ);
   XDELETE(_lpFontMini); 
}

void ESequenceTalk::Logic()
{
}

void ESequenceTalk::Check()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   bool  bMousein = false;
   
   // En mode dialogue, on ne peut passer qu'en cliquant dans la zone de dialogue
   // :dmekersa:20110401 On peut maintenant skipper sans avoir la souris en bas de l'écran
//   if (_bDialogMode) {
//      if (KInput::getMouseY() > 630) {
//         bMousein = true;
//      }
//   } else {
      // Toujours vrai quand on est pas en mode dialogue
      bMousein = true;
//   }
   
   if (_lpAnimIn) {
      if (Mouse.LeftEvent() && !_lpAnimIn->isCompleted() && bMousein)
      {
         _lpAnimIn->move(999999);
      }
      else if (_lpAnimIn->isCompleted() && !_lpAnimHold) { // && KInput::getLeftButtonState()) {
         _lpAnimHold = new KCounter();
         double dDuration = strlen(_szText)*SEQUENCE_TEXT_CHARDURATION;
         // Au moins 4 secondes
         if (dDuration < 4000) dDuration = 4000;  
         _lpAnimHold->startCounter(1, 0, 0, dDuration, K_COUNTER_EASEIN);
      } else {
         _lpAnimIn->move(fElapsed);      
      }
   }
   if (_lpAnimHold) {
      bool bStillPlaying = false;
      bool bClick = Mouse.LeftEvent();
      if (!bClick && _lpSound) {
         if (_lpSound->isPlaying()) {
            bStillPlaying = true;
         }
      }
      _lpAnimHold->move(fElapsed);
      if ((bClick && bMousein) || (_lpAnimHold->isCompleted() && !bStillPlaying))
      {
         XDELETE(_lpAnimHold);
         if (_lpAnimOut) {
            if(_lpAnimOut->isCompleted()) XDELETE(_lpAnimOut);
         }
         if (!_lpAnimOut) {
            _lpAnimOut = new KCounter();
            _lpAnimOut->startCounter(1, 0, 0, 200, K_COUNTER_EASEOUT);
         }
      }
   }
   if (_lpAnimOut) {
      _lpAnimOut->move(fElapsed);
      if (_lpSound) {
         _lpSound->setVolume(long(_lpAnimOut->getCurrentValue()*100.0));
      }
      if (_lpSoundStream) {
         _lpSoundStream->setVolume(long(_lpAnimOut->getCurrentValue()*100.0));
      }
   }
}

void ESequenceTalk::Draw()
{
   float fBlend;

   /* Calcul de la transparence */
   if (!_lpAnimIn && !_lpAnimOut) {
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

   if (_lpSoundStream) {
      _lpSoundStream->setVolume(long(fBlend*100.0));
   }
   
   /* Calcul des coordonnées */
   long h;
//   if (!_bDialogMode) {
//      /* La boite se trouve centrée au dessus de la coordonnées avec une marge */
//      long x,y;
//      /* Taille oqp par le texte */
//      long boxw,boxh;
//      
//      boxw = SEQUENCE_TALK_BOXWIDTH - SEQUENCE_TALK_BOXMARGIN*2;
//      boxh = SEQUENCE_TALK_BOXHEIGHT - SEQUENCE_TALK_BOXMARGIN*2;
//      y = _nFromY - SEQUENCE_TALK_BOXMARGIN - SEQUENCE_TALK_BOXHEIGHT;
//      x = _nFromX - (SEQUENCE_TALK_BOXWIDTH / 2);
//      
//      _lpFont->setColor(1, 1, 1, 1);
//      _lpFont->setBlend(fBlend);
//      h = _lpFont->getMultilineHeight(_szText, x, x+boxw, y, TEXTSTYLE_CENTER);
//      // Si c'est le perso qui parle, on inverse la bulle pour tourner la pointe vers la gauche
//      _lpGraphicBulle->blitAlphaRectFx(0, 0, _lpGraphicBulle->getWidth(), _lpGraphicBulle->getHeight(), x, y, 0, 1.0f, fBlend, _bFlip);
////      _lpGraphic->drawRect((float)x-1, (float)y-1, (float)x+SEQUENCE_TALK_BOXWIDTH+1, (float)y+SEQUENCE_TALK_BOXHEIGHT+1, 0, 0, 0, fBlend*0.8);
//      _lpFont->setBlitColor(.9f, .9f, 1, fBlend);
//      _lpFont->drawMultiline(_szText, x + SEQUENCE_TALK_BOXMARGIN, x+boxw, y + (boxh - h)/2, TEXTSTYLE_CENTER);
//   }
//   else {
   
   if (!_bDialogMode) {
      // C'est le personnage principal qui parle
      if (_bFlip) {
         // Affichage du visage du perso en mode dialogue
         KGraphic *lpCharacter = SCENEDIRECTOR->getCurrentCharacter();
         //KAssert(lpCharacter != NULL);
         if (!lpCharacter) {
            return;
         }
         float fImgH = lpCharacter->getHeight()*0.75f;
         float fImgW = lpCharacter->getWidth()*0.75f;
         
         h = _lpFontMainChar->getMultilineHeight(_szText, (fImgW*1.25)+20, 1024-15, 5, TEXTSTYLE_LEFT, -1, 23);
         
         KGame::getGame()->_lpKWindow->setWorldView(0,(fImgH+10.0f)-((fImgH+10.0f)*fBlend),0,1,false);
         _lpGraphic->drawRect(0, 0, 1024, (float)fImgH+10, .1f, .1f, .1f, fBlend); //*0.75);
         lpCharacter->blitAlphaRectFx(0, 0, lpCharacter->getWidth(), lpCharacter->getHeight(), 5, 5, 0, 0.75f, fBlend);
         _lpFontMainChar->setBlend(fBlend);
         _lpFontMainChar->drawMultiline(_szText, (fImgW*1.25)+20, 1024-15, ((fImgH+10) - h)/2, TEXTSTYLE_LEFT,-1,23);
         KGame::getGame()->_lpKWindow->setWorldView(0,0,0,1,false);
         
      }
      // C'est un PNJ qui parle
      else {
         /* La boite se trouve centrée au dessus de la coordonnées avec une marge */
         long x,y;
         /* Taille oqp par le texte */
         long boxw,boxh;
         
         boxw = SEQUENCE_TALK_BOXWIDTH - SEQUENCE_TALK_BOXMARGIN*2;
         boxh = SEQUENCE_TALK_BOXHEIGHT - SEQUENCE_TALK_BOXMARGIN*2;
         y = _nFromY - SEQUENCE_TALK_BOXHEIGHT - SEQUENCE_TALK_BOXMARGIN;
         x = _nFromX - (SEQUENCE_TALK_BOXWIDTH / 2);
         
         h = _lpFontPNJ->getMultilineHeight(_szText, x + SEQUENCE_TALK_BOXMARGIN, x+boxw, y + ((SEQUENCE_TALK_BOXHEIGHT)/2), TEXTSTYLE_CENTER, -1, 23);
         // Si c'est le perso qui parle, on inverse la bulle pour tourner la pointe vers la gauche
         _lpGraphicBulle->blitAlphaRectFx(0, 0, _lpGraphicBulle->getWidth(), _lpGraphicBulle->getHeight(), x, y, 0, 1.0f, fBlend, _bFlip);
         _lpFontPNJ->setBlend(fBlend);
         _lpFontPNJ->drawMultiline(_szText, x + SEQUENCE_TALK_BOXMARGIN, x+boxw, y + ((SEQUENCE_TALK_BOXHEIGHT-h)/2), TEXTSTYLE_CENTER,-1,23);
      }
   }
   else {
      _lpFontMainChar->setBlend(fBlend);
      h = _lpFontMainChar->getMultilineHeight(_szText, 225, 870, 660, TEXTSTYLE_LEFT,-1,23);
      _lpFontMainChar->drawMultiline(_szText, 225, 870, 660, TEXTSTYLE_LEFT, -1, 23);
      _lpFontMini->setColor(.9f, .9f, .9f, .9f);
      _lpFontMini->drawStringFromRight(KStr("DIALOG_CLOSETXT"), 1024-10, 768-7-23);
   }
//   }
}

bool ESequenceTalk::isCompleted()
{
   if (!_lpAnimIn || !_lpAnimOut) {
      return false;
   }
//   if (strlen(_szVoice) > 0)
//   {
//      _lpSound = ESoundBank::getSound(_szVoice);
//      if (_lpSound->isPlaying()) {
//         return false;
//      }
//   }
   return _lpAnimIn->isCompleted() && _lpAnimOut->isCompleted();
}

void ESequenceTalk::Start()
{
   ESequenceBase::Start();
   
   // Rattrapage si IHM non affichée
   if (_bDialogMode && !SCENEDIRECTOR->isDialogMode()) {
      _bDialogMode = false;
      SCENEDIRECTOR->ShowIHM(true);
   }
      
   _lpAnimIn = new KCounter();
   _lpAnimIn->startCounter(0, 1, 0, 200, K_COUNTER_EASEOUT);

   ESoundBank::getSound("talkdialog")->playSample();
   
   if (strlen(_szVoice) > 0)
   {
      _lpSound = ESoundBank::getSound(_szVoice);
      if (_lpSound) {
         _lpSound->setVolume(100);
         _lpSound->playSample();
      }
   }
}
