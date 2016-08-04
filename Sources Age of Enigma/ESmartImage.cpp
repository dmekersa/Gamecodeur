/*
 *  ESmartImage.cpp
 *  pngviewer
 *
 *  Created by Rockford on 07/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "ESmartImage.h"
#include "MyGame.h"
#include "global.h"
#include "FontBank.h"
#include "ImageBank.h"

/* Contructeur simple */
ESmartImage::ESmartImage(KGraphic *lpGraphic, float _xx, float _yy, int nNbFrame, bool bVisible, bool bSmooth, int nFadeDuration)
{
   DefaultInit(_xx, _yy, nNbFrame, bVisible, bSmooth, nFadeDuration);
   _lpGraphic = lpGraphic;
   
   /* SetVisible démarrera une anim si besoin */
   SetVisible(bVisible, bSmooth);
}

//constructeur video
ESmartImage::ESmartImage(KVideo *lpVideo, float _xx, float _yy, bool bVisible, bool bSmooth, int nFadeDuration)
{
	DefaultInit(_xx, _yy, 1, bVisible, bSmooth, nFadeDuration);
   _lpVideo = lpVideo;
   _lpGraphic = _lpVideo->getKGraphic();
   SetFPS(24);
}
ESmartImage::ESmartImage(const char *szFileName, float _xx, float _yy, bool bVisible, bool bSmooth, int nFadeDuration, bool bLoop, int nFPS) {
   bool bSuccess = EImageBank::preLoadAnim(szFileName);
   KAssert(bSuccess);
   
   std::string fmt = szFileName;
	fmt = fmt + "/" + szFileName + "%04d.png";
	char filePath[K_MAXPATH];
	char tmpPath[K_MAXPATH];
	int i=1;

	DefaultInit(_xx, _yy, 1, bVisible, bSmooth, nFadeDuration);
   
   // Construction du path pour recherche dans le cache de ImageBank
	sprintf(filePath,fmt.c_str(),i);
   EImageBank::NormName("pictures/",filePath,"",tmpPath);
   EImageBank *o = NULL;
   while((o = EImageBank::GetCache(tmpPath)) != NULL) {
      _lpFList.push_back(o);
      if (i<3) {
         o->UploadImg();
      }
      i++;
      sprintf(filePath,fmt.c_str(),i);
      EImageBank::NormName("pictures/",filePath,"png",tmpPath);
   }
   
	_cFramesPrefix = (char *) malloc(sizeof(char [K_MAXPATH]));
	KAssert(_cFramesPrefix != NULL);
	memset(_cFramesPrefix,0,sizeof(char [K_MAXPATH]));
	strcat(_cFramesPrefix,fmt.c_str());
	
	_lpFListIterator = _lpFList.begin();
   _lpGraphic = (*_lpFListIterator)->getImage();
	_lpFListIterator++;
	_bLoop = bLoop;
    SetFPS(nFPS);
}

/* Destructeur */
ESmartImage::~ESmartImage()
{
   _lpVideo = NULL;
   _lpGraphic = NULL;
   XDELETE(_lpAnimIn);
   XDELETE(_lpAnimOut);
   XDELETE(_lpAnimRotate);
   XDELETE(_lpAnimP2Px);
   XDELETE(_lpAnimP2Py);
   XDELETE(_lpAnimMarquee);
   if (_cFramesPrefix != NULL) {
	  free(_cFramesPrefix);
	  _cFramesPrefix = NULL;
   }
   _lpFList.clear();
}

void ESmartImage::DefaultInit(float _xx, float _yy, int nNbFrame, bool bVisible, bool bSmooth, int nFadeDuration) {
   _lpAnimIn = new KCounter();
   _lpAnimOut = new KCounter();   
   _lpAnimRotate = new KCounter();
   _lpAnimP2Px = new KCounter();
   _lpAnimP2Py = new KCounter();
   _lpGraphic = NULL;
   _lpVideo = NULL;
   _lpNextFrame = NULL;
   _bVisible = bVisible;
   _bSmooth = bSmooth;
   _nFadeDuration = nFadeDuration;
   _bHalo = false;
   _fHaloSpeed = 0.0f;
   _fHaloTarget = 0.0f;
   _fRotateAngleStart = 0;
   _fRotateAngleFinish = 0;
   _fFrameDuration = 0;
   _x = _xx;
   _y = _yy;
   _xFromP2P = 0;
   _yFromP2P = 0;
   _xDestP2P = 0;
   _yDestP2P = 0;
   _fDurationP2P = 0;
   _bRotate = false;
   _nNbFrame = nNbFrame;
   _nCurrentFrame = 1;
   _bFirstFrame = true;
   _bAnimationP2P = false;
   _bIsCompletedP2P = false;
   _nBounceDirectionP2P = 0;
   _bAnimationByFrame = false;
   _bSmoothOverlap = false;
   _bReadFrame = false;
   
   _bAnimMarquee = false;
   _lpAnimMarquee = NULL;
   _dMarqueeDuration = 0;
   _fMarqueeSize = 0;
   _nMarqueeDirection = -1;

   _lpFList.clear();
   _lpFListIterator = _lpFList.begin();
   _dFrameDisplayTime = 0;
   _dTotalFrameDisplayTime = 0;
   _bFramesVideoAnim = false;
   _bLoop = false;
   _cFramesPrefix = NULL;

   /* par défaut, le blending de l'image à l'état stable est 1 */
   _fFixedBlend = 1.0;
   /* Si l'image est visible et affichée sêchement, elle est visible à 100 % */
   if (bVisible && !bSmooth) {
      _fBlend = 1.0;
   }
   /* Sinon, on commence à 0 */
   else {
      _fBlend = 0.0;
   }
   /* On détermine l'état de départ */
   if (bSmooth && bVisible)
      _State = SMARTIMG_STABLEOUT;
   else if (bSmooth && !bVisible)
      _State = SMARTIMG_STABLEOUT;
   else if (!bSmooth && bVisible)
      _State = SMARTIMG_STABLE;
   else if (!bSmooth && !bVisible)
      _State = SMARTIMG_STABLEOUT;

   /* SetVisible démarrera une anim si besoin */
   SetVisible(bVisible, bSmooth);
}
/* Logique et affichage */

void ESmartImage::Logic()
{
	if (_lpVideo != NULL && _bReadFrame) {
		_lpVideo->readFrame();
		_bReadFrame = false;
		_lpGraphic = _lpVideo->getKGraphic();
	}
}

void ESmartImage::Draw()
{
   double x = 0,y = 0;
   float fBlend = 0, fBlendNext = 0, fRotation = 0;
   float fScale = 1.0;
   int nNextFrame = 0;
   float fMarqueePos = 0;

   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   _lpAnimOut->move(fElapsed);
   _lpAnimIn->move(fElapsed);
   if (_bRotate) {
      _lpAnimRotate->move(fElapsed);
   }
   _lpAnimP2Px->move(fElapsed);
   _lpAnimP2Py->move(fElapsed);

   if (_dFrameDisplayTime > 0) {
	   MoveFrames(fElapsed);
   }
   if (_lpVideo != NULL && !_lpVideo->isVideoAvailable()) {
      _bReadFrame = true;
	  return;
   }
   /* Fade d'affichage */
   if ( _bSmooth )
   {
      switch (_State) {
         case SMARTIMG_STABLE:
            break;
         case SMARTIMG_SHOWIN:
            if (_lpAnimIn->isCompleted())
            {
               if (!_bAnimationByFrame) {
                  _State = SMARTIMG_STABLE;
                  _fBlend = _fFixedBlend;
               } else {
                  // Next frame
                  _bFirstFrame = false;
                  StartFade(SMARTIMG_SHOWOUT);
               }
            }
            else {
               _fBlend = _lpAnimIn->getCurrentValue();
            }

            break;
         case SMARTIMG_SHOWOUT:
            if (_lpAnimOut->isCompleted())
            {
               if (!_bAnimationByFrame) {
                  _State = SMARTIMG_STABLEOUT;
				  if (_lpVideo != NULL) {
					  _lpVideo->pauseTime();
				  }
                  _fBlend = 1.0;
               } else {
                  // Next frame
                  _nCurrentFrame++;
                  if (_nCurrentFrame > _nNbFrame) _nCurrentFrame=1;
                  StartFade(SMARTIMG_SHOWOUT);
               }
            }
            else {
               _fBlend = _lpAnimOut->getCurrentValue();
            }
            break;
         default:
            break;
      }
   }

   /* Rotation */
   if (_bRotate) {
      if (_lpAnimRotate->isCompleted()) {
         // Bounce ? Alors on repart !
         if (_bRotateBounce) {
            // Nouveau sens
            _nRotateBounceDirection = (_nRotateBounceDirection == 1 ? -1 : 1);
            Rotate(true, _nRotateBounceDirection);
         } else {
            _bRotate = false;
         }
      }
   }
   /* Animation point à point */
   if (_bAnimationP2P)
   {
      if (_lpAnimP2Px->isCompleted() && _lpAnimP2Py->isCompleted()) 
      {
         if (_bBounceP2P) {
            // Inverse la direction
            if (_nBounceDirectionP2P == 1) {
               _nBounceDirectionP2P = 0;
            } else {
               _nBounceDirectionP2P = 1;
            }
            // Retour
            if (_nBounceDirectionP2P = 1) {
               _x = _lpAnimP2Px->getCurrentValue();
               _y = _lpAnimP2Py->getCurrentValue();         
               AnimateP2P(_xFromP2P, _yFromP2P, _fDurationP2P, true, K_COUNTER_EASEOUT, 0);
            }
            // Aller
            else {
               _x = _lpAnimP2Px->getCurrentValue();
               _y = _lpAnimP2Py->getCurrentValue();         
               AnimateP2P(_xDestP2P, _yDestP2P, _fDurationP2P, true, K_COUNTER_EASEOUT, 0);
            }

         }
         else {
            _bAnimationP2P = false;
            _x = _lpAnimP2Px->getCurrentValue();
            _y = _lpAnimP2Py->getCurrentValue();         
         }
      }
   }

   /* Alpha */
   switch (_State) {
      case SMARTIMG_STABLE:
         if (_fFixedBlend > 0) {
            if (_bHalo) {
               if (_fBlend > _fHaloTarget) {
                  _fBlend -= _fHaloSpeed;
                  if (_fBlend <= _fHaloTarget) {
                     _fHaloTarget = GetRandom(0.5f, _fFixedBlend);
                  }
               }
               else if (_fBlend < _fHaloTarget) {
                  _fBlend += _fHaloSpeed;
                  if (_fBlend >= _fHaloTarget) {
                     _fHaloTarget = GetRandom(0.5f, _fFixedBlend);
                  }
               }
               else {
                  _fHaloTarget = GetRandom(0.5f, _fFixedBlend);
               }
               fBlend = _fBlend;
            }
            else {
               fBlend = _fFixedBlend;
            }
         }
         else {
            fBlend = 1.0;
         }
         break;
      case SMARTIMG_STABLEOUT:
         fBlend = 0.0;
         break;
      case SMARTIMG_SHOWIN:
         fBlend = _lpAnimIn->getCurrentValue();
         fBlendNext = 1.0f - fBlend;
         if (fBlendNext<0) fBlendNext=0;
         break;
      case SMARTIMG_SHOWOUT:
         fBlend = _lpAnimOut->getCurrentValue();
         fBlendNext = 1.0f - (float)fBlend;
         if (_bAnimationByFrame) {
            if (fBlendNext<0) fBlendNext=0;
         }
         break;
      default:
         break;
   }
       
   /* Rotation */
   if ( _bRotate ){
      if ( _nRotateBounceDirection == 1 ) {
         fRotation = _lpAnimRotate->getCurrentValue();
      } else {
         fRotation = 0.0f - _lpAnimRotate->getCurrentValue();
      }
   } else {
      // !!!:dmekersa:20110413 La référence est maintenant l'angle de fin (0 si aucune rotation) pour que les images restent tournées si elles ont rotaté
      fRotation = _fRotateAngleFinish;
   }
   // Déplacement
   if (_bAnimationP2P)
   {
      x = _lpAnimP2Px->getCurrentValue();
      y = _lpAnimP2Py->getCurrentValue();
	  _bIsCompletedP2P = _lpAnimP2Px->isCompleted();
     if (_bIsCompletedP2P) _bAnimationP2P = false;
   }
   else {
      x = _x;
      y = _y;
   }
       
   /* Draw now ! */
   if (_lpGraphic) {
      if (_lpAnimMarquee) {
         if (_bAnimMarquee)
            _lpAnimMarquee->move(fElapsed);
         
         fMarqueePos = fabs(_lpAnimMarquee->getCurrentValue());
         if (_bAnimMarquee && (_lpAnimMarquee->isCompleted() || fMarqueePos > fabs(_fMarqueeSize))) {
            _lpAnimMarquee->startCounter(0,_fMarqueeSize,0,_dMarqueeDuration,K_COUNTER_LINEAR);
         }
         
         fMarqueePos = _lpAnimMarquee->getCurrentValue() + fabs(_fMarqueeSize);
         fMarqueePos = fMarqueePos - (int)(fMarqueePos/fabs(_fMarqueeSize))*fabs(_fMarqueeSize);
         
         // Pas de gestion de rotation ou de scale pour l'instant
         if (_nMarqueeDirection%2 == 0) { 
            //_lpGraphic->blitAlphaRectFxF(0, fMarqueePos, _lpGraphic->getWidth(), _lpGraphic->getHeight(), (float)x, (float)y, fRotation, fScale, fBlend );
            //_lpGraphic->blitAlphaRectFxF(0, 0, _lpGraphic->getWidth(), fMarqueePos, (float)x, (float)y +getHeight() - fMarqueePos, fRotation, fScale, fBlend );
            _lpGraphic->blitAlphaRectFxF(0, (int)fMarqueePos, _lpGraphic->getWidth(), _lpGraphic->getHeight(), (float)x, (float)y, 0, 1, fBlend );
            _lpGraphic->blitAlphaRectFxF(0, 0, _lpGraphic->getWidth(), (int)fMarqueePos, (float)x, (float)y +getHeight()-(int)fMarqueePos, 0, 1, fBlend );
         } else {
            //_lpGraphic->blitAlphaRectFxF(fMarqueePos, 0, _lpGraphic->getWidth(), _lpGraphic->getHeight(), (float)x, (float)y, fRotation, fScale, fBlend );
            //_lpGraphic->blitAlphaRectFxF(0, 0, fMarqueePos, _lpGraphic->getHeight(), (float)x + getWidth()-fMarqueePos, (float)y, fRotation, fScale, fBlend );
            _lpGraphic->blitAlphaRectFxF(0, 0, (int)fMarqueePos, _lpGraphic->getHeight(), (float)x + getWidth()-(int)fMarqueePos, (float)y, 0, 1, fBlend );
            _lpGraphic->blitAlphaRectFxF((int)fMarqueePos, 0, _lpGraphic->getWidth(), _lpGraphic->getHeight(), (float)x, (float)y, 0, 1, fBlend );
         }
         return;
      }
      if (_nNbFrame==1) {
         if (_lpVideo) {
            _lpGraphic->blitAlphaRectFxF(0, 0, _lpGraphic->getWidth(), _lpGraphic->getHeight(), (float)x, (float)y, fRotation, fScale, getFade());
         }
         else {
            if (fRotation == 0 && fScale == 1 && fBlend == 1) {
               _lpGraphic->blitAlphaRectF(0, 0, _lpGraphic->getWidth(), _lpGraphic->getHeight(), (float)x, (float)y);
            }
            else {
               _lpGraphic->blitAlphaRectFxF(0, 0, _lpGraphic->getWidth(), _lpGraphic->getHeight(), (float)x, (float)y, fRotation, fScale, fBlend );
            }
         }
      } else {
         if (_bSmoothOverlap ) {
            fBlend = fBlend * 10;
            if (fBlend > 1.0) fBlend = 1;
         }
         int nW = (int)_lpGraphic->getWidth()/_nNbFrame;
         if (fBlend != 0.0f) {
            _lpGraphic->blitAlphaRectFxF((float)(_nCurrentFrame-1)*nW, 0, (float)(_nCurrentFrame)*nW, _lpGraphic->getHeight(), (float)x, (float)y, fRotation, fScale, fBlend );
         }
      }
      /* Frame par Frame ? */
      if (_bAnimationByFrame) {
         // Calcul de la frame suivante
         nNextFrame = _nCurrentFrame+1;
         if (nNextFrame > _nNbFrame) {
            nNextFrame=1;
         }
         // Dessine la framesuivante
         if (!_bFirstFrame) {
            int nW = (int)_lpGraphic->getWidth()/_nNbFrame;
            if (fBlend != 0.0f) {
               _lpGraphic->blitAlphaRectFx((float)(nNextFrame-1)*nW, 0, (float)(nNextFrame)*nW, _lpGraphic->getHeight(), (short)x, (short)y, fRotation, fScale, fBlendNext );
            }
         }
      }
   }
}

/* Changement d'états */
void ESmartImage::MoveFrames(double fElapsed) {
   if (_lpFList.size() > 1 && _bFramesVideoAnim) {
      // upload de l'image suivante de l'anim a la frame
      // suivant le display d'une nouvelle image
      if (_lpNextFrame != NULL) {
         _lpNextFrame->UploadImg();
         _lpNextFrame == NULL;
      }

      _dTotalFrameDisplayTime += fElapsed;
      if (_dTotalFrameDisplayTime > _dFrameDisplayTime) {
         if (! (_lpFListIterator == _lpFList.end())) {
            _lpGraphic = (*(_lpFListIterator))->getImage();
            _lpFListIterator++;
         } else if (_bLoop) {
            _lpFListIterator = _lpFList.begin();
         } else {
            StopVideo();
         }
         _dTotalFrameDisplayTime = _dTotalFrameDisplayTime - (
                                       (int)(_dTotalFrameDisplayTime / _dFrameDisplayTime)*_dFrameDisplayTime);
         
         // preparation du chargement de la prochaine frame au blit suivant
         if (_lpFListIterator != _lpFList.end())
            _lpNextFrame = (*_lpFListIterator);
      }
   } else if (_lpVideo != NULL) {
      _dTotalFrameDisplayTime += fElapsed;
      if (_dTotalFrameDisplayTime > _dFrameDisplayTime) {
         _bReadFrame = true;
         _dTotalFrameDisplayTime = _dTotalFrameDisplayTime - (
                                                              (int)(_dTotalFrameDisplayTime / _dFrameDisplayTime)*_dFrameDisplayTime);
      }
   }
}
// Position
void ESmartImage::Move(float x, float y)
{
   _x = x;
   _y = y;
}

// Visibilité
void ESmartImage::SetVisible(bool bVisible, bool bSmooth)
{
   // Lance une animation
   _bVisible = bVisible;
   _bSmooth = bSmooth;
   
   if ( _bSmooth )
   {
      if (bVisible && _State != SMARTIMG_STABLE && _State != SMARTIMG_SHOWIN)
      {
         StartFade(SMARTIMG_SHOWIN);
      }
      else {
         if (!bVisible && _State != SMARTIMG_STABLEOUT && _State != SMARTIMG_SHOWOUT)
         {
            StartFade(SMARTIMG_SHOWOUT);
         }
      }
   }
}
// Forcer à être visible immédiatement même si l'image est en smooth
void ESmartImage::ForceVisible(bool bVisible)
{
   if (bVisible) {
      _State = SMARTIMG_STABLE;
      _bVisible = true;
      _fBlend = 1.0f;
   }
   else {
      _State = SMARTIMG_STABLEOUT;
      _bVisible = false;
      _fBlend = 0.0f;
   }

}

// Fixe la durée du fade, en ms
void ESmartImage::SetFadeDuration(int nDuration)
{
   _nFadeDuration = nDuration;
}

// Démarre un changement d'état de visibilité (fade ou stabilisation)
void ESmartImage::StartFade(SmartImageFadeState state)
{
   switch (state) {
      case SMARTIMG_STABLE:
         _State = SMARTIMG_STABLE;
         break;
      case SMARTIMG_STABLEOUT:
         _State = SMARTIMG_STABLEOUT;
         break;
      case SMARTIMG_SHOWIN:
         _State = SMARTIMG_SHOWIN;
         _lpAnimIn->startCounter(0.0, _fFixedBlend, 0, _nFadeDuration, K_COUNTER_LINEAR);
		 if (_lpVideo != NULL) {
			 _lpVideo->resumeTime();
		 }
         break;
      case SMARTIMG_SHOWOUT:
         _State = SMARTIMG_SHOWOUT;
         _lpAnimOut->startCounter(_fFixedBlend, 0.0, 0, _nFadeDuration, K_COUNTER_LINEAR);
         break;
      default:
         break;
   }
}

// Initialise les paramètres d'une rotation
void ESmartImage::SetRotation(float fAngleStart, float fAngleFinish, float fRotationDuration, bool bBounce, KCounterFunction counterFunction)
{
   // Sauve les valeurs
   _fRotateAngleStart = fAngleStart;
   _fRotateAngleFinish = fAngleFinish;
   _fRotateDuration = fRotationDuration;
   _bRotateBounce = bBounce;
   _counterFunctionForRotation = counterFunction;
}

// Active l'effet de rotation ou le stoppe
// nDirection : 1 pour sens des aiguilles d'une montre, -1 pour sens inverse
void ESmartImage::Rotate(bool bRotate, int nDirection)
{
   _bRotate = bRotate;
   _nRotateBounceDirection = nDirection;
   if (bRotate) {
      _lpAnimRotate->startCounter(_fRotateAngleStart, _fRotateAngleFinish, 0, _fRotateDuration, _counterFunctionForRotation);
   }
}

// Démarre l'animation image par image ou le stoppe
void ESmartImage::AnimateByFrame(bool bAnimate)
{
   _bAnimationByFrame = bAnimate;
   if (bAnimate == false) {
      if (_bVisible)
         _State = SMARTIMG_STABLE;
      else
         _State = SMARTIMG_STABLEOUT;
   } else {
      _State = SMARTIMG_SHOWIN;
   }

}

// Déplace l'image vers un autre point
// float _xDest, float _yDest             : Destination
// float _fDuration                       : durée du déplacement en ms
// KCounterFunction counterFunction       : Effet, voir KCounter
void ESmartImage::AnimateP2P(float _xDest, float _yDest, float _fDuration, bool bBounce, KCounterFunction counterFunction, float fRandom)
{
   _bIsCompletedP2P = false;
   _bAnimationP2P = true;
   _bBounceP2P = bBounce;
   _xFromP2P = _x;
   _yFromP2P = _y;
   _xDestP2P = _xDest;
   _yDestP2P = _yDest;
   _fDurationP2P = _fDuration;
   _lpAnimP2Px->startCounter(_x, _xDest, fRandom, _fDuration, counterFunction);
   _lpAnimP2Py->startCounter(_y, _yDest, fRandom, _fDuration, counterFunction);
}

bool ESmartImage::isVideoPlaying() {
   bool res = false;
   if (_lpVideo != NULL) {
      res = EImageBank::IsVideoPlaying(_lpVideo);
   } else if (_lpFList.size() != 0) {
	   res = _bFramesVideoAnim;   
   }
   return res;
}
bool ESmartImage::IsVideoEndReached() {
   bool res = false;
   if (_lpVideo != NULL) {
      res = _lpVideo->isEndReached();
   } else if (_lpFList.size() != 0) {
	  res = !_bLoop && _lpFListIterator == _lpFList.end();
   }
   return res;
}
void ESmartImage::StartVideo() {
   if (_lpVideo != NULL) {
      _lpVideo->resumeTime();
   // // !!!:dmekersa:20101217 Ces 2 lignes provoquaient le bug des vagues sur la falaise. Inutile de faire un rewind quand la vidéo n'a jamais été démarrée.
   //_lpVideo->rewind();
   //_lpVideo->readFrame();
	  EImageBank::SetVideoStatus(_lpVideo,true);
      if (_lpVideo->isVideoAvailable()) {
         _lpVideo->rewind();
         _lpVideo->readFrame();
      }
   } else if (_lpFList.size() != 0) {
      _bFramesVideoAnim = true;
      // !!!:dmekersa:20110203 La vidéo img/img ne redémarrait pas, j'ajoute cette ligne :
      _lpFListIterator = _lpFList.begin();
   }
}
void ESmartImage::StopVideo() {
   if(_lpVideo != NULL) {
      _lpVideo->pauseTime();
	  EImageBank::SetVideoStatus(_lpVideo,false);  
   } else if (_lpFList.size() != 0)  {
      _bFramesVideoAnim = false;
   }
}
void ESmartImage::ResumeVideo() {
   if (_lpVideo != NULL) {
      _lpVideo->resumeTime();
  	  EImageBank::SetVideoStatus(_lpVideo,true);
   } else if (_lpFList.size() != 0)  {
      _bFramesVideoAnim = true;
   }
}

void ESmartImage::StartMarquee() {
   if (!_lpAnimMarquee)
      return;

   float currentval = _lpAnimMarquee->getCurrentValue();
   _lpAnimMarquee->startCounter(currentval,currentval+_fMarqueeSize,0,_dMarqueeDuration,K_COUNTER_LINEAR);
   _bAnimMarquee = true;
}
void ESmartImage::StopMarquee() {
   if (!_lpAnimMarquee)
      return;

   float currentval = _lpAnimMarquee->getCurrentValue();
   _lpAnimMarquee->startCounter(currentval,currentval,0,0,K_COUNTER_LINEAR);
   _lpAnimMarquee->move(1);
   _bAnimMarquee = false;
}
void ESmartImage::AnimateMarquee(double dMarqueeDuration, int nMarqueDirection) {
	KAssert(_lpVideo == NULL); // N'es pas une video
	int oldDirection = _nMarqueeDirection;

	_dMarqueeDuration = dMarqueeDuration;
	_nMarqueeDirection = nMarqueDirection;
	if (_nMarqueeDirection == 0)
		_nMarqueeDirection = -1;

	if (_nMarqueeDirection %2 == 0) {
		_fMarqueeSize = getHeight();
	} else {
		_fMarqueeSize = getWidth();
	}

	if (_nMarqueeDirection < 0)
		_fMarqueeSize = -_fMarqueeSize;

	if (_lpAnimMarquee == NULL) {
		_lpAnimMarquee = new KCounter();
		_lpAnimMarquee->startCounter(0,0,0,0,K_COUNTER_LINEAR);
	} else {
		float tmp = _lpAnimMarquee->getCurrentValue();
		while (tmp <=0 && _fMarqueeSize <=0) {
			tmp = tmp - _fMarqueeSize;
		} 
		_lpAnimMarquee->startCounter(tmp,tmp,0,0,K_COUNTER_LINEAR);
		KAssert(abs(_nMarqueeDirection) == abs(oldDirection));
	}
	_lpAnimMarquee->move(1);
	StartMarquee();
}
