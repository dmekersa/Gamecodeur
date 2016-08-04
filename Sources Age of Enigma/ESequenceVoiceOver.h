/*
 *  ESequenceVoiceOver.h
 *  enigma
 *
 *  Created by Rockford on 05/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef ESEQUENCEVOICEOVER_H
#define ESEQUENCEVOICEOVER_H

#include "ESequenceBase.h"
#include "KPTK.h"
#include "KCounter.h"
#include "SoundBank.h"

typedef enum {
   SEQUENCE_VOICEOVER_BROWN = -1,
   SEQUENCE_VOICEOVER_GREY,
   SEQUENCE_VOICEOVER_RED
} SequenceVoiceOverColor;

class ESequenceVoiceOver : public ESequenceBase {
public:
   ESequenceVoiceOver(const char *szText, const char *szVoice, SequenceVoiceOverColor Color = SEQUENCE_VOICEOVER_GREY);
   virtual ~ESequenceVoiceOver();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   KTextFace *_lpFont;
   KCounter *_lpAnimIn;
   KCounter *_lpAnimOut;
   KGraphic *_lpGraphic;
   SequenceVoiceOverColor _Color;
   char _szText[256*4];
   char _szVoice[256];
   KSound *_lpSound;
};

#endif