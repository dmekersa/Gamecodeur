/*
 *  ESequenceMisc.h
 *  enigma
 *
 *  Created by Rockford on 31/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef ESEQUENCEMISC_H
#define ESEQUENCEMISC_H

#include "ESequenceBase.h"
#include "ESceneDirector.h"
#include "KPTK.h"
#include "KCounter.h"
#include "EMiniJeuBase.h"
#include "KMath2d.h"
#include "ETutoBox.h"

class ESequenceCallback : public ESequenceBase {
public:
   ESequenceCallback(const char *szParam, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceCallback();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   char _szParam[256];
   ESceneDirector *_lpSceneDirector;
};

class ESequenceGotoVideo : public ESequenceBase {
public:
   ESequenceGotoVideo(const char *szVideoName, const char *szMusicName, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceGotoVideo();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return true; }
   
   static void PreloadVideo(const char *szVideoName);

private:
   static KVideo* LoadVideo(const char *szVideoName, bool bVideoPreload=false);

   KVideo *_lpVideo;
   char _szMusicName[256];
   KCounter *_lpCounterIn;
   KCounter *_lpCounterOut;
   ESceneDirector *_lpSceneDirector;
};

class ESequenceObjective : public ESequenceBase {
public:
   ESequenceObjective(const char *szUniverse, const char *szObjective, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceObjective();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   bool _bDone;
   char _szUniverse[256];
   char _szObjective[256];
   ESceneDirector *_lpSceneDirector;
};

class ESequenceShowImage : public ESequenceBase {
public:
   ESequenceShowImage(const char *szImageName, bool bShow, bool bModal, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceShowImage();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return _bModal; }
   
private:
   char _szImageName[256];
   bool  _bShow;
   bool  _bModal;
   ESceneDirector *_lpSceneDirector;
};

class ESequenceSwitchImage : public ESequenceBase {
public:
   ESequenceSwitchImage(const char *szImageName1, const char *szImageName2, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceSwitchImage();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   char _szImageName1[256];
   char _szImageName2[256];
   ESceneDirector *_lpSceneDirector;
};

class ESequenceShowEmitter : public ESequenceBase {
public:
   ESequenceShowEmitter(const char *szEmitterName, bool bShow, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceShowEmitter();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   char _szEmitterName[256];
   bool _bShow;
   ESceneDirector *_lpSceneDirector;
};

class ESequenceAnimation : public ESequenceBase {
public:
   ESequenceAnimation(const char *szAnimationName, bool bStart, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceAnimation();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   char _szAnimationName[256];
   bool _bStart;
   ESceneDirector *_lpSceneDirector;
};

class ESequencePlaySound : public ESequenceBase {
public:
   ESequencePlaySound(const char *szSoundName, ESceneDirector *lpSceneDirector, bool bLoop=false);
   virtual ~ESequencePlaySound();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   char _szSoundName[256];
   bool _bLoop;
   ESceneDirector *_lpSceneDirector;
};

class ESequenceWait : public ESequenceBase {
public:
   ESequenceWait(int nDuration, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceWait();
   virtual void Logic(){};
   virtual void Check(){};
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   int _nDuration;
   KCounter *_lpCounter;
   ESceneDirector *_lpSceneDirector;
};

class ESequenceMinigame : public ESequenceBase {
public:
   ESequenceMinigame(EMiniJeuBase *lpMiniJeu, const char* szName, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceMinigame();
   virtual void Logic(){};
   virtual void Check(){};
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   char _szName[256];
   EMiniJeuBase *_lpMiniJeu;
   ESceneDirector *_lpSceneDirector;
};

class ESequenceDialogBoxOK : public ESequenceBase {
public:
   ESequenceDialogBoxOK(const char *szText, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceDialogBoxOK();
   virtual void Logic(){};
   virtual void Check(){};
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   ESceneDirector *_lpSceneDirector;
   char _szText[1024];
};

class ESequenceEnergy : public ESequenceBase {
public:
   ESequenceEnergy(float fCenterX, float fCenterY, float fXRadius, float fYRadius, float fDestX, float fDestY, int nNumber, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceEnergy();
   virtual void Logic(){};
   virtual void Check(){};
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return true; }
   
private:
   ESceneDirector *_lpSceneDirector;
   float _fCenterX;
   float _fCenterY;
   float _fXRadius;
   float _fYRadius;
   float _fDestX;
   float _fDestY;
   bool _bWait;
   int _nNumber;
   int _nEmitted;
   KCounter *_lpEmitCounter;
   KEllipse *_lpEllipse;
   void Emit();
};

class ESequencePickupItem : public ESequenceBase {
public:
   ESequencePickupItem(const char *szItemName, float fFromX, float fFromY, int nNbPartNeeded, ESceneDirector *lpSceneDirector);
   virtual ~ESequencePickupItem();
   virtual void Logic();
   virtual void Check();
   virtual void Draw();
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   char _szItemName[256];
   int _nNbPartNeeded;
   
private:
   bool _bStarted;
   float _fFromX;
   float _fFromY;
   ESceneDirector *_lpSceneDirector;
};

class ESequenceNarrationMode : public ESequenceBase {
public:
   ESequenceNarrationMode(SceneSequenceNarrationMode mode, bool b, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceNarrationMode();
   virtual void Logic() {};
   virtual void Check() {};
   virtual void Draw() {};
   virtual void Start();
   virtual bool isCompleted()
   {
      return true;
   }
   virtual bool isModal() { return false; }
   
private:
   bool _b;
   SceneSequenceNarrationMode _mode;
   ESceneDirector *_lpSceneDirector;
};

class ESequenceCloseVignette : public ESequenceBase {
public:
   ESequenceCloseVignette(ESceneDirector *lpSceneDirector);
   virtual ~ESequenceCloseVignette();
   virtual void Logic() {};
   virtual void Check() {};
   virtual void Draw() {};
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   ESceneDirector *_lpSceneDirector;
};

class ESequenceGotoScene : public ESequenceBase {
public:
   ESequenceGotoScene(const char *szSceneName, const char *szAdditionalName, bool bVignette, ESceneDirector *lpSceneDirector);
   virtual ~ESequenceGotoScene();
   virtual void Logic() {};
   virtual void Check() {};
   virtual void Draw() {};
   virtual void Start();
   virtual bool isCompleted();
   virtual bool isModal() { return false; }
   
private:
   char _szSceneName[256];
   char _szAdditionalName[256];
   bool _bVignette;
   ESceneDirector *_lpSceneDirector;
};

class ESequenceTutobox : public ESequenceBase {
public:
   ESequenceTutobox(const char *szMessage, int nX, int nY, int nAngle, int nRadius, ESceneDirector *lpSceneDirector)
   {
      _lpTutobox = NULL;
      strcpy(_szMessage, szMessage);
      _nX = nX;
      _nY = nY;
      _nAngle = nAngle;
      _nRadius = nRadius;
      _lpTutobox = new ETutoBox(_szMessage, _nX, _nY, _nAngle, _nRadius);
      _lpSceneDirector = lpSceneDirector;
   };
   virtual ~ESequenceTutobox() { XDELETE(_lpTutobox); };
   virtual void Logic() { };
   virtual void Check() {
      _lpTutobox->Check();
   };
   virtual void Draw() {
      _lpTutobox->Draw();
   };
   virtual void Start() {
      ESequenceBase::Start();
      _lpTutobox->Show();
   };
   virtual bool isCompleted() { return _lpTutobox->IsCompleted(); };
   virtual bool isModal() { return false; }
   
private:
   ESceneDirector *_lpSceneDirector;
   ETutoBox *_lpTutobox;
   char _szMessage[1024];
   int _nX;
   int _nY;
   int _nAngle;
   int _nRadius;
};

#endif