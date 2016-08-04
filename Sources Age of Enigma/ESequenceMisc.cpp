/*
 *  ESequenceMisc.cpp
 *  enigma
 *
 *  Created by Rockford on 31/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "ESequenceMisc.h"
#include "SoundBank.h"
#include "MusicBank.h"
#include "MyGame.h"
#include "EPointer.h"
#include "ImageBank.h"
#include "EBookCtrl.h"

/*********************** CALLBACK ***********************/

ESequenceCallback::ESequenceCallback(const char *szParam, ESceneDirector *lpSceneDirector)
{
   memset(_szParam, 0, 256);
   strcpy(_szParam, szParam);
   _lpSceneDirector = lpSceneDirector;
}

ESequenceCallback::~ESequenceCallback()
{
}

void ESequenceCallback::Logic()
{
}

void ESequenceCallback::Check()
{
}

void ESequenceCallback::Draw()
{
}

bool ESequenceCallback::isCompleted()
{
   return true;
}

void ESequenceCallback::Start()
{
   ESequenceBase::Start();
   
   _lpSceneDirector->GetCurrentScene()->Callback(_szParam);
   
}

/*********************** OBJECTIVE ***********************/

ESequenceObjective::ESequenceObjective(const char *szUniverse, const char *szObjective, ESceneDirector *lpSceneDirector)
{
   memset(_szUniverse, 0, 256);
   strcpy(_szUniverse, szUniverse);
   memset(_szObjective, 0, 256);
   strcpy(_szObjective, szObjective);
   _lpSceneDirector = lpSceneDirector;
   _bCheckAllowed = true;
   _bDone = false;
}

ESequenceObjective::~ESequenceObjective()
{
}

void ESequenceObjective::Logic()
{
}

void ESequenceObjective::Check()
{
   if (!_bDone && !_lpSceneDirector->getBookCtrl()->isVisible(TASK_LIST) && !_lpSceneDirector->getBookCtrl()->isClosing()) {
      EGlobalBank::AddNewObjective(_szUniverse,_szObjective);
      _bDone = true;
   }
}

void ESequenceObjective::Draw()
{
}

bool ESequenceObjective::isCompleted()
{
   return _bDone;
}

void ESequenceObjective::Start()
{
   ESequenceBase::Start();
   
   ESoundBank::getSound("newobjective")->playSample();
   
}

/*********************** VIDEO ***********************/

ESequenceGotoVideo::ESequenceGotoVideo(const char *szVideoName, const char *szMusicName, ESceneDirector *lpSceneDirector)
{
   _lpVideo = LoadVideo(szVideoName);
   _lpCounterIn = NULL;
   _lpCounterOut = NULL;
   memset(_szMusicName, 0, 256);
   strcpy(_szMusicName, szMusicName);
}

ESequenceGotoVideo::~ESequenceGotoVideo()
{
   XDELETE(_lpCounterIn);
   XDELETE(_lpCounterOut);
   EImageBank::VideoStop(_lpVideo);
}

void ESequenceGotoVideo::Logic()
{
}

void ESequenceGotoVideo::Check()
{
   if (Mouse.LeftEvent())
   {
      if (_lpCounterOut == NULL) {
         _lpCounterOut = new KCounter();
         float fromAlpha = 1.0f;
         if (_lpCounterIn != NULL)
            fromAlpha=_lpCounterIn->getCurrentValue();
         _lpCounterOut->startCounter(fromAlpha, 0.0f, 0, 500*fromAlpha, K_COUNTER_EASEIN);         
      }
      XDELETE(_lpCounterIn);
   }
}

void ESequenceGotoVideo::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   float fFade = 0.0f;
   if (_lpCounterIn) {
      _lpCounterIn->move(fElapsed);
   }
   if (_lpCounterOut) {
      _lpCounterOut->move(fElapsed);
   }

   if (_lpCounterIn) {
      fFade = _lpCounterIn->getCurrentValue();
      if (_lpCounterIn->isCompleted()) {
         double total,current;
         total = _lpVideo->getTotalTime();
         current = _lpVideo->getTime();
         if ( total-current  <= 0.5) {
            XDELETE(_lpCounterIn);
            _lpCounterOut = new KCounter();
            _lpCounterOut->startCounter(1.0f, 0.0f, 0, 500, K_COUNTER_LINEAR);         
         }
      }
   }
   else if (_lpCounterOut) {
      fFade = _lpCounterOut->getCurrentValue();
      if (_lpCounterOut->isCompleted()) {
         KSound *lpMusic = EMusicBank::getMusic(_szMusicName);
         if (lpMusic) {
            if (lpMusic->isPlaying()) {
               lpMusic->stopStream();
            }
         }
      }
   }   

   /* Update video */
   _lpVideo->readFrame ();
   if (_lpVideo->isEndReached ()) {
      return;
   }
   
   if (_lpVideo->isVideoAvailable ()) {
      /* Draw video */
      _lpVideo->getKGraphic()->stretchAlphaRect (0, 0, _lpVideo->getFrameWidth(), _lpVideo->getFrameHeight(), 0, 0, 1024, 768, fFade, 0.0f, false, false);
   }
   
}

bool ESequenceGotoVideo::isCompleted()
{
   if (_lpCounterOut) {
      if (_lpCounterOut->isCompleted()) {
         return true;
      }
   }
   return false;
}

void ESequenceGotoVideo::Start()
{
   ESequenceBase::Start();
   
   _lpCounterIn = new KCounter();
   _lpCounterIn->startCounter(0.0f, 1.0f, 0, 2000, K_COUNTER_LINEAR);
   KSound *lpMusic = EMusicBank::getMusic(_szMusicName);

   EImageBank::VideoPlay(_lpVideo);
   if (lpMusic) {
      lpMusic->playStream(false);
   }
}

void ESequenceGotoVideo::PreloadVideo(const char *szVideoName) {
    LoadVideo(szVideoName,true);
}

KVideo* ESequenceGotoVideo::LoadVideo(const char *szVideoName, bool bVideoPreload) {
   std::string tmp(szVideoName);
   if (tmp.compare(0,strlen("videos/"),"videos/") == 0)
      tmp=tmp.substr(strlen("videos/"),tmp.size());
   return  EImageBank::getVideo(tmp.c_str(),false,true,bVideoPreload);
}

/*********************** SHOW IMAGE ***********************/

ESequenceShowImage::ESequenceShowImage(const char *szImageName, bool bShow, bool bModal, ESceneDirector *lpSceneDirector)
{
   memset(_szImageName, 0, 256);
   strcpy(_szImageName, szImageName);
   _bShow = bShow;
   _bModal = bModal;
   _lpSceneDirector = lpSceneDirector;
   _bCheckAllowed = true;

   // preload des images d'inventaire si necessaire.
   SceneObjectImage *refObject = _lpSceneDirector->GetCurrentScene()->GetObjectImageByName(_szImageName);
   if (_bShow && refObject != NULL && strlen(refObject->getInvImageName()) > 0)
      EImageBank::getImage(refObject->getInvImageName(),"png",true,true,true);
}

ESequenceShowImage::~ESequenceShowImage()
{
}

void ESequenceShowImage::Logic()
{
}

void ESequenceShowImage::Check()
{
}

void ESequenceShowImage::Draw()
{
}

bool ESequenceShowImage::isCompleted()
{
   // Ce n'est pas une image (surement une zone) donc c'est toujours vrai, ou le sceneobject n'existe pas 
   SceneObjectImage *refObject = _lpSceneDirector->GetCurrentScene()->GetObjectImageByName(_szImageName);
   if (refObject == NULL || refObject->GetSmartImage() == NULL) {
#ifdef DEBUG
      if (refObject == NULL)
         K_LOG("ESequenceShowImage: Unknow scene object %s", _szImageName);
#endif
      return true;
   }
   
   if (_bShow)
   {
      // Si l'objet a changé d'état entre temps, on abandonne l'attente
      if (refObject->GetSmartImage()->getState() != SMARTIMG_SHOWIN) {
         return true;
      }
      else {
         return refObject->isFadeInCompleted();
      }
   }
   else
   {
      if (refObject->GetSmartImage()->getState() != SMARTIMG_SHOWOUT) {
         return true;
      }
      else {
         return refObject->isFadeOutCompleted();
      }
   }
}

void ESequenceShowImage::Start()
{
   ESequenceBase::Start();
   _lpSceneDirector->GetCurrentScene()->SetVisible((const char *)_szImageName,_bShow);
}

/*********************** SWITCH IMAGE ***********************/

ESequenceSwitchImage::ESequenceSwitchImage(const char *szImageName1, const char *szImageName2, ESceneDirector *lpSceneDirector)
{
   memset(_szImageName1, 0, 256);
   strcpy(_szImageName1, szImageName1);
   memset(_szImageName2, 0, 256);
   strcpy(_szImageName2, szImageName2);
   _lpSceneDirector = lpSceneDirector;
   _bCheckAllowed = true;
}

ESequenceSwitchImage::~ESequenceSwitchImage()
{
}

void ESequenceSwitchImage::Logic()
{
}

void ESequenceSwitchImage::Check()
{
}

void ESequenceSwitchImage::Draw()
{
}

bool ESequenceSwitchImage::isCompleted()
{
   return _lpSceneDirector->GetCurrentScene()->GetObjectImageByName(_szImageName2)->isFadeInCompleted();
}

void ESequenceSwitchImage::Start()
{
   ESequenceBase::Start();
   
   _lpSceneDirector->GetCurrentScene()->SetVisible((const char *)_szImageName1,false);
   _lpSceneDirector->GetCurrentScene()->SetVisible((const char *)_szImageName2,true);
   
}

/*********************** SHOW EMITTER ***********************/

ESequenceShowEmitter::ESequenceShowEmitter(const char *szEmitterName, bool bShow, ESceneDirector *lpSceneDirector)
{
   memset(_szEmitterName, 0, 256);
   strcpy(_szEmitterName, szEmitterName);
   _bShow = bShow;
   _lpSceneDirector = lpSceneDirector;
   _bCheckAllowed = false;
}

ESequenceShowEmitter::~ESequenceShowEmitter()
{
}

void ESequenceShowEmitter::Logic()
{
}

void ESequenceShowEmitter::Check()
{
}

void ESequenceShowEmitter::Draw()
{
}

bool ESequenceShowEmitter::isCompleted()
{
   return true;
}

void ESequenceShowEmitter::Start()
{
   ESequenceBase::Start();
   
   if (_bShow) {
      _lpSceneDirector->GetCurrentScene()->StartEmitter(_szEmitterName);
   }
   else {
      _lpSceneDirector->GetCurrentScene()->StopEmitter(_szEmitterName);
   }
}

/*********************** ANIMATION ***********************/

ESequenceAnimation::ESequenceAnimation(const char *szAnimationName, bool bStart, ESceneDirector *lpSceneDirector)
{
   memset(_szAnimationName, 0, 256);
   strcpy(_szAnimationName, szAnimationName);
   _bStart = bStart;
   _lpSceneDirector = lpSceneDirector;
   _bCheckAllowed = false;
}

ESequenceAnimation::~ESequenceAnimation()
{
}

void ESequenceAnimation::Logic()
{
}

void ESequenceAnimation::Check()
{
}

void ESequenceAnimation::Draw()
{
}

bool ESequenceAnimation::isCompleted()
{
   SceneObjectAnimation *anim;
   anim = _lpSceneDirector->GetCurrentScene()->GetObjectAnimationByName(_szAnimationName);
   if (anim) {
      SceneObjectImage *objimg;
      objimg = _lpSceneDirector->GetCurrentScene()->GetObjectImageByName(anim->_strTarget.c_str());
      if (objimg) {
         if (strstr(anim->_strType.c_str(), ANIM_P2P)!= NULL)
         {
            return objimg->GetSmartImage()->isAnimateP2P() == false;
         }
         else if (strstr(anim->_strType.c_str(), ANIM_ROTATE)!= NULL)
         {
            return true; //objimg->GetSmartImage()->isRotate() == false;
         }
		 else if (strstr(anim->_strType.c_str(), ANIM_VIDEO)!= NULL)
		 {
			 return objimg->GetSmartImage()->getVideo()->isEndReached();
		 }
      }
      return true;
   }
   return false;
}

void ESequenceAnimation::Start()
{
   ESequenceBase::Start();
   
   if (_bStart) {
      _lpSceneDirector->GetCurrentScene()->StartAnimation(const_cast<char *>(_szAnimationName));
   }
   else {
      _lpSceneDirector->GetCurrentScene()->StopAnimation(const_cast<char *>(_szAnimationName));
   }
}

/*********************** PLAY SOUND ***********************/

ESequencePlaySound::ESequencePlaySound(const char *szSoundName, ESceneDirector *lpSceneDirector, bool bLoop)
{
   memset(_szSoundName, 0, 256);
   strcpy(_szSoundName, szSoundName);
   _lpSceneDirector = lpSceneDirector;
   _bCheckAllowed = false;
   _bLoop = bLoop;

   ESoundBank::getSound(_szSoundName,_bLoop,true,false);
}

ESequencePlaySound::~ESequencePlaySound()
{
}

void ESequencePlaySound::Logic()
{
}

void ESequencePlaySound::Check()
{
}

void ESequencePlaySound::Draw()
{
}

bool ESequencePlaySound::isCompleted()
{
   return true;
}

void ESequencePlaySound::Start()
{
   KSound *sound = NULL;
   sound = ESoundBank::getSound(_szSoundName,_bLoop);
   if (sound) {
     sound->playSample();
   }
   ESequenceBase::Start();
}

/*********************** PICK UP ITEM **********************/

ESequencePickupItem::ESequencePickupItem(const char *szItemName, float fFromX, float fFromY, int nNbPartNeeded, ESceneDirector *lpSceneDirector)
{
   _bStarted = false;
   _lpSceneDirector = lpSceneDirector;
   memset(_szItemName, 0, 256);
   strcpy(_szItemName, szItemName);
   _fFromX = fFromX;
   _fFromY = fFromY;
   _nNbPartNeeded = nNbPartNeeded;
}

ESequencePickupItem::~ESequencePickupItem()
{
   K_LOG("ESequencePickupItem::~ESequencePickupItem for %s",_szItemName);
   // La séquence n'avait pas commencé, on la force
   if (!_bStarted)
   {
      K_LOG("ESequencePickupItem::~ESequencePickupItem force start for %s",_szItemName);
      Start();
      _lpSceneDirector->EndTravellingItem();
   }
}

void ESequencePickupItem::Logic()
{
}

void ESequencePickupItem::Check()
{
}

void ESequencePickupItem::Draw()
{
}

bool ESequencePickupItem::isCompleted()
{
   return !_lpSceneDirector->isTravellingItem();
}

void ESequencePickupItem::Start()
{
   K_LOG("ESequencePickupItem::Start for %s",_szItemName);
   ESequenceBase::Start();
   _bStarted = true;
   
   // Un pickup est en cours au niveau du director ? On le termine de force !
   if (_lpSceneDirector->isTravellingItem()) {
      K_LOG("ESequencePickupItem::Start - _lpSceneDirector->isTravellingItem() returned true");
      _lpSceneDirector->EndTravellingItem();
   }
   KAssert(_lpSceneDirector->isTravellingItem() == false);
   _lpSceneDirector->PickupItem(_szItemName, _fFromX, _fFromY, _nNbPartNeeded);
   
}

/*************************** Mouse capture *************************************/
ESequenceNarrationMode::ESequenceNarrationMode(SceneSequenceNarrationMode mode, bool b, ESceneDirector *lpSceneDirector)
{
   _lpSceneDirector = lpSceneDirector;
   _b = b;
   _mode = mode;
}

ESequenceNarrationMode::~ESequenceNarrationMode()
{
}

void ESequenceNarrationMode::Start()
{
   ESequenceBase::Start();
   _lpSceneDirector->GetCurrentScene()->ResetMouseOver();
   switch (_mode) {
      case SEQUENCE_NARRATION_CINEMATIC:
         if (_b)
            _lpSceneDirector->CinematicModeStart();
         else
         {
            _lpSceneDirector->CinematicModeStop();
         }
         break;
      case SEQUENCE_NARRATION_DIALOG:
         if (_b)
            _lpSceneDirector->DialogModeStart();
         else
            _lpSceneDirector->DialogModeStop();
         break;
      case SEQUENCE_NARRATION_LOCKMOUSE:
         if (_b)
            _lpSceneDirector->LockModeStart();
         else
            _lpSceneDirector->LockModeStop();
         break;
      default:
         break;
   }
}

/*************************** Close Vignette **********************************/
ESequenceCloseVignette::ESequenceCloseVignette(ESceneDirector *lpSceneDirector)
{
   _lpSceneDirector = lpSceneDirector;
}

ESequenceCloseVignette::~ESequenceCloseVignette()
{
}

void ESequenceCloseVignette::Start()
{
   ESequenceBase::Start();
   _lpSceneDirector->CloseVignette();
}

bool ESequenceCloseVignette::isCompleted()
{
   return _lpSceneDirector->CloseVignetteDone();
}

/*************************** Goto Scene **********************************/
ESequenceGotoScene::ESequenceGotoScene(const char *szSceneName, const char *szAdditionalName, bool bVignette, ESceneDirector *lpSceneDirector)
{
   memset(_szSceneName, 0, 256);
   strcpy(_szSceneName, szSceneName);
   memset(_szAdditionalName, 0, 256);
   strcpy(_szAdditionalName, szAdditionalName);
   _bVignette = bVignette;
   _lpSceneDirector = lpSceneDirector;
}

ESequenceGotoScene::~ESequenceGotoScene()
{
}

void ESequenceGotoScene::Start()
{
   ESequenceBase::Start();
   if (!_bVignette) {
      _lpSceneDirector->setZoomin(true); // TODO: gérer un paramètre
      _lpSceneDirector->GoToScene(_szSceneName, _szAdditionalName, false);
   }
   else {
      _lpSceneDirector->GoToVignette(_szSceneName, _szAdditionalName);
   }
}

bool ESequenceGotoScene::isCompleted()
{
   if (!_bVignette) {
      return _lpSceneDirector->GetCurrentScene()->FadeInCompleted();
   }
   else {
      return _lpSceneDirector->GetCurrentVignette()->FadeInCompleted();
   }
}

/*************************** Wait **********************************/
ESequenceWait::ESequenceWait(int nDuration, ESceneDirector *lpSceneDirector)
{
   _nDuration = nDuration;
   _lpCounter = new KCounter();
   _lpSceneDirector = lpSceneDirector;
}

ESequenceWait::~ESequenceWait()
{
   XDELETE(_lpCounter);
}

void ESequenceWait::Start()
{
   ESequenceBase::Start();
   _lpCounter->startCounter(0.0f, 1.0f, 0, _nDuration, K_COUNTER_LINEAR);
}

void ESequenceWait::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   _lpCounter->move(fElapsed);
}

bool ESequenceWait::isCompleted()
{
   return _lpCounter->isCompleted();
}

/*************************** Mini Game **********************************/
ESequenceMinigame::ESequenceMinigame(EMiniJeuBase *lpMiniJeu, const char *szName, ESceneDirector *lpSceneDirector)
{
   memset(_szName, 0, 256);
   strcpy(_szName, szName);
   _lpMiniJeu = lpMiniJeu;
   _lpSceneDirector = lpSceneDirector;
}

ESequenceMinigame::~ESequenceMinigame()
{
   XDELETE(_lpMiniJeu);
}

void ESequenceMinigame::Start()
{
   ESequenceBase::Start();
   EMiniJeuBase *tmp = _lpMiniJeu;
   _lpMiniJeu = NULL;
   if (strstr(_szName, "poltergeist")) {
      _lpSceneDirector->GoToMiniGame(const_cast<char *>(_szName), tmp, "poltergeist");
   }
   else {
      _lpSceneDirector->GoToMiniGame(const_cast<char *>(_szName), tmp);
   }
   
}

void ESequenceMinigame::Draw()
{
}

bool ESequenceMinigame::isCompleted()
{
   return true;
}

/*************************** DialogOK **********************************/
ESequenceDialogBoxOK::ESequenceDialogBoxOK(const char *szText, ESceneDirector *lpSceneDirector)
{
   memset(_szText, 0, 1024);
   KAssert(strlen(szText) < 1024);
   strcpy(_szText, szText);
   _lpSceneDirector = lpSceneDirector;
}

ESequenceDialogBoxOK::~ESequenceDialogBoxOK()
{
}

void ESequenceDialogBoxOK::Start()
{
   ESequenceBase::Start();
   _lpSceneDirector->OpenDialogbox(_szText);
}

void ESequenceDialogBoxOK::Draw()
{
}

bool ESequenceDialogBoxOK::isCompleted()
{
   return _lpSceneDirector->isDialogboxActive() == false;
}

/*************************** Energy **********************************/
ESequenceEnergy::ESequenceEnergy(float fCenterX, float fCenterY, float fXRadius, float fYRadius, float fDestX, float fDestY, int nNumber, ESceneDirector *lpSceneDirector)
{
   _fCenterX = fCenterX;
   _fCenterY = fCenterY;
   _fXRadius = fXRadius;
   _fYRadius = fYRadius;
   _fDestX = fDestX;
   _fDestY = fDestY;
   _nNumber = nNumber;
   _nEmitted = 0;
   _lpEmitCounter = new KCounter();
   _lpEllipse = new KEllipse(_fCenterX, _fCenterY, _fXRadius, _fYRadius);
   _lpSceneDirector = lpSceneDirector;
}

ESequenceEnergy::~ESequenceEnergy()
{
   XDELETE(_lpEmitCounter);
}

void ESequenceEnergy::Start()
{
   ESequenceBase::Start();
   
   _lpEmitCounter->startCounter(0, 1, 0, 1000, K_COUNTER_LINEAR);
   Emit();

}

void ESequenceEnergy::Emit()
{
   float x,y;
   int nLimit;
   KVector2 v;
   
   x = GetRandom(_fCenterX - _fXRadius, _fCenterX + _fXRadius);
   y = GetRandom(_fCenterY - _fYRadius, _fCenterY + _fYRadius);
   nLimit = 0;
   v.x = x;
   v.y = y;
   while (!_lpEllipse->contains(v)) {
      x = GetRandom(_fCenterX - _fXRadius, _fCenterX + _fXRadius);
      y = GetRandom(_fCenterY - _fYRadius, _fCenterY + _fYRadius);
      nLimit++;
      if (nLimit == 100) {
         break;
      }
   }
   ESoundBank::getSound("energy")->playSample();
   _lpSceneDirector->getEnergyParticlesManager()->AddEnergy(x, y, _fDestX, _fDestY, 4000.0f, 1);
   _nEmitted++;
}

void ESequenceEnergy::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   _lpEmitCounter->move(fElapsed);
   if (_lpEmitCounter->isCompleted() && _nEmitted < _nNumber) {
      Emit();
      _lpEmitCounter->startCounter(0, 1, 0, 1000, K_COUNTER_LINEAR);
   }
}

bool ESequenceEnergy::isCompleted()
{
   return _lpSceneDirector->getEnergyParticlesManager()->isCompleted(1);
}
