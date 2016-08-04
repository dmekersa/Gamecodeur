/*
 *  ESmartImage.h
 *  pngviewer
 *
 *  Created by Rockford on 07/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */
#include <list>
#ifndef _ENIGMA_ESMARTIMAGE_H
#define _ENIGMA_ESMARTIMAGE_H

#include "KPTK.h"
#include "KCounter.h"
#include "KVideo.h"

class EImageBank;

/* Etats pour l'affichage */
typedef enum {
   SMARTIMG_STABLE = 0,    // Visible et stable
   SMARTIMG_STABLEOUT,     // Invisible et stable
   SMARTIMG_SHOWIN,        // En train de devenir visible
   SMARTIMG_SHOWOUT,       // En train de devenir invisible
} SmartImageFadeState;

class ESmartImage : public KObjectListable {

public:
   /* Consctructeur pour image fixe ou frame/frame */
   ESmartImage(KVideo *lpVideo, float _xx, float _yy, bool bVisible=true, bool bSmooth=true, int nFadeDuration=0);
   ESmartImage(KGraphic *lpGraphic, float _xx, float _yy, int nNbFrame=1, bool bVisible=true, bool bSmooth=true, int nFadeDuration=0);

   // Constructeur Destructeur exceptionels, liberation impérative des resources au delete du SmartImage
   ESmartImage(const char *lszFileName, float _xx, float _yy, bool bVisible=true, bool bSmooth=true, int nFadeDuration=0, bool bLoop=true, int nFPS=24);
   
   /* Destructeur */
   ~ESmartImage();
   
   /* Logique et affichage */
   void Logic();
   void Draw();
   
   /* Changement de position */
   void Move(float x, float y);
   /* Changement d'états */
   // Visibilité
   void SetVisible(bool bVisible, bool bSmooth);
   void ForceVisible(bool bVisible);
   // Détermine la durée d'exécution du fadein/out
   void SetFadeDuration(int);
   /* Rotation */
   void SetRotation(float fAngleStart, float fAngleFinish, float fRotationDuration, bool bBounce, KCounterFunction counterFunction);
   // Démarre la rotation
   void Rotate(bool bRotate, int nDirection=1);
   // Démarre l'animation
   void AnimateByFrame(bool bAnimate);
   // Démarre un point à point
   void AnimateP2P(float _xDest, float _yDest, float _fDuration, bool bBounce, KCounterFunction counterFunction, float fRandom);
   // Accessors
   void AnimateMarquee(double dMarqueeDuration,int nMarqueDirection=0);
   void SetPosition(float fx, float fy)
   {
      _x = fx;
      _y = fy;
   }
   bool IsValid() {
      return _lpGraphic || _lpVideo || _lpFList.size()>0;
   }
   bool isVisible() {
      return _bVisible;
   }
   bool isRotate() {
      return _bRotate;
   }
   float getRotationValue(){
      return _lpAnimRotate->getCurrentValue();
   }
   bool isAnimateP2P(){
      return _bAnimationP2P;
   }
   bool isVideoPlaying();
   bool isMarqueRuning() {
	   return _bAnimMarquee;
   }
   bool isCompleted(){
      if (_bAnimationP2P) {
         if (_lpAnimP2Px->isCompleted()==false || (_lpVideo != NULL && !_lpVideo->isEndReached())) 
            return false;
         else
            return _lpAnimP2Px->isCompleted();
      }
      if (_bRotate) {
         if (_lpAnimRotate->isCompleted()==false) 
            return false;
         else
            return true;
      }
      return true;
   }
   bool isFadeInCompleted()
   {
      if (_bAnimationByFrame == false) {
         return _lpAnimIn->isCompleted() && _State == SMARTIMG_STABLE;
      }
      else {
         return _lpAnimIn->isCompleted();
      }
   }
   bool isFadeOutCompleted()
   {
      if (_bAnimationByFrame == false) {
         return _lpAnimOut->isCompleted() && _State == SMARTIMG_STABLEOUT;
      }
      else {
         return _lpAnimOut->isCompleted();
      }
   }
   float getCurrentX() {
      if (!_bAnimationP2P) {
         return _x;
      } else {
         return _lpAnimP2Px->getCurrentValue();
      }
   }
   float getCurrentY() {
      if (!_bAnimationP2P) {
         return _y;
      } else {
         return _lpAnimP2Py->getCurrentValue();
      }
   }
   int getWidth() {
      if(_lpVideo)
      {
         return (int)(_lpVideo->getFrameWidth());
      }
      else if (_lpGraphic)
      {
         return (int)(_lpGraphic->getWidth() / _nNbFrame);
      }
      else return 0;
   }
   int getHeight() {
      if(_lpVideo)
      {
         return (int)(_lpVideo->getFrameHeight());
      }
      else if (_lpGraphic)
      {
         return (int)_lpGraphic->getHeight();
      }
      else return 0;
   }
   void setSmoothOverlap(bool b)
   {
      _bSmoothOverlap = b;
   }
   void setHalo(bool b, float fSpeed)
   {
      _bHalo = b;
      _fHaloSpeed = fSpeed;
      _fHaloTarget = 0.5f;
   }
   SmartImageFadeState getState()
   {
      return _State;
   }
   float getFade()
   {
      switch (_State) {
         case SMARTIMG_SHOWIN:
            return _lpAnimIn->getCurrentValue();
            break;
         case SMARTIMG_SHOWOUT:
            return _lpAnimOut->getCurrentValue();
            break;
         case SMARTIMG_STABLEOUT:
            return 0;
            break;
         case SMARTIMG_STABLE:
            return _fFixedBlend;
            break;
         default:
            return 1;
            break;
      }
   }
   void setFixedBlend(float f) {
      _fFixedBlend = f;
   }
   KGraphic *getGraphic() {
      return _lpGraphic;
   }
   KVideo *getVideo() {
      return _lpVideo;
   }
   void StartVideo();
   void StopVideo();
   void ResumeVideo();

   void StartMarquee();
   void StopMarquee();

   void SetFPS(int FPS) {_dFrameDisplayTime = 1000/FPS;}
   void SetLoop(bool bLoop) {_bLoop = bLoop;}
   void MoveFrames(double fElapsed);
   bool IsVideoEndReached();

private:
   void DefaultInit(float _xx, float _yy, int nNbFrame, bool bVisible, bool bSmooth, int nFadeDuration);

   KGraphic *_lpGraphic;
   KVideo *_lpVideo;
   
   /* Etat courant */
   SmartImageFadeState _State;
   /* Etats */
   bool _bVisible;
   /* Affichage smooth avec un fadein/out */
   bool _bSmooth;
   bool _bSmoothOverlap;
   int _nFadeDuration;
   float _x, _y;
   void StartFade(SmartImageFadeState state);
   float _fBlend;
   float _fFixedBlend;
   /* Animations utilisées pour l'affichage smooth */
   KCounter *_lpAnimIn;
   KCounter *_lpAnimOut;
   /* Animation utilisée pour le mode halo */
   bool _bHalo;
   float _fHaloSpeed;
   float _fHaloTarget;
   /* Rotation automatique */
   KCounter *_lpAnimRotate;
   bool _bRotate;                                  // true si la rotation est active
   float _fRotateAngleStart, _fRotateAngleFinish;  // Angle de début et de fin (0..359 pour une rotation complète)
   float _fRotateDuration;                         // Durée de la rotation en ms
   bool _bRotateBounce;                            // La rotation fait un aller/retour si true
   int _nRotateBounceDirection;                    // Sens de rotation
   KCounterFunction _counterFunctionForRotation;   // Type de rotation basée sur KCounter (linéaire ou easyin/out
   /* Animation frame par frame */
   bool _bAnimationByFrame;   // L'image contient plusieurs frame et est animée
   int _nNbFrame;             // Nombre de frame dans l'image (sous forme de strip)
   int _nCurrentFrame;
   bool _bFirstFrame;
   float _fFrameDuration;     // Durée d'affichage de chaque frame
   /* Animation point à point */
   bool _bAnimationP2P, _bIsCompletedP2P, _bBounceP2P; 
   short _nBounceDirectionP2P;
   float _xFromP2P, _yFromP2P;
   float _xDestP2P, _yDestP2P;
   float _fOffsetP2P;
   float _fDurationP2P;
   KCounter *_lpAnimP2Px;
   KCounter *_lpAnimP2Py;
   /* Animation Marquee */
   KCounter *_lpAnimMarquee;
   bool _bAnimMarquee;
   double _dMarqueeDuration;
   float _fMarqueeSize;
   int _nMarqueeDirection;
   /* Animation image par image */
   std::list< EImageBank * > _lpFList;
   std::list< EImageBank * >::iterator _lpFListIterator;
   EImageBank *_lpNextFrame;
   double _dFrameDisplayTime;
   double _dTotalFrameDisplayTime;
   bool _bFramesVideoAnim;
   bool _bLoop;
   char *_cFramesPrefix;

   bool _bReadFrame;
};
#endif