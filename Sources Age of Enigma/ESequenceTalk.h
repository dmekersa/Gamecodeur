/*
 *  ESequenceTalk.h
 *  enigma
 *
 *  Created by Rockford on 10/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef ESEQUENCETALK_H
#define ESEQUENCETALK_H

/* Constantes */
#define SEQUENCE_TALK_BOXWIDTH 500
#define SEQUENCE_TALK_BOXHEIGHT 120
#define SEQUENCE_TALK_BOXMARGIN 20

#include "ESequenceBase.h"
#include "KPTK.h"
#include "KSound.h"
#include "KCounter.h"

class ESequenceTalk : public ESequenceBase {
public:
   ESequenceTalk(int nFromX, int nFromY, const char *szText, const char *szVoice, bool bCheckAllowed = true, bool bDialogMode = false, KSound *lpSoundStream = NULL);
   virtual ~ESequenceTalk();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   KTextFace *_lpFontMainChar;
   KTextFace *_lpFontPNJ;
   KTextFace *_lpFontMini;
   KCounter *_lpAnimIn;
   KCounter *_lpAnimHold;
   KCounter *_lpAnimOut;
   KGraphic *_lpGraphic;
   KGraphic *_lpGraphicBulle;
   KSound *_lpSound;
   KSound *_lpSoundStream;

   bool _bDialogMode;
   bool _bFlip;   // inversion de la bulle si perso qui parle
   int _nFromX;
   int _nFromY;
   char _szText[256*4];
   char _szVoice[256];
};

#endif