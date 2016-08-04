/*
 *  ESceneObject.cpp
 *  enigma
 *
 *  Created by Rockford on 27/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */
#include <list>

#include "ESceneObject.h"
#include "MyGame.h"
#include "EScene.h"
#include "ImageBank.h"
#include "FontBank.h"
#include "EmitterBank.h"
#include "assert.h"
#include "ESceneDirector.h"

// *******************************************************************************
// SCENE OBJECT IMAGE
// *******************************************************************************

SceneObjectImage::~SceneObjectImage()
{
   XDELETE(_lpSmartImage);
   XDELETE(_lpGlitchEmitter);
   XDELETE(_lpGlitchCounter);
}

void SceneObjectImage::Draw()
{
   if (_lpGlitchCounter) {
      if (!_lpGlitchCounter->isCompleted()) {
         double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
         _lpGlitchCounter->move(fElapsed);
         if (_lpGlitchCounter->isCompleted()) {
            SetGlitch();
         }
      }      
   }
   SceneObject::Draw();
   if (_lpSmartImage)
      _lpSmartImage->Draw();
   if (_lpGlitchEmitter && SCENEDIRECTOR->isDialogMode() == false && SCENEDIRECTOR->isCinematicMode() == false) {
      double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
      _lpGlitchEmitter->Render(fElapsed);
   }
}

void SceneObjectImage::Logic()
{
   SceneObject::Logic();
   if (_lpSmartImage)
      _lpSmartImage->Logic();
   if (_lpGlitchEmitter) {
      _lpGlitchEmitter->Logic();
   }
}

void SceneObjectImage::SetData(const char *szFileName, int scene_id, const char *szObjectName, const char *szType, int nNbFrame, bool bFade, int nFadeDuration, float fx, float fy, int iWidth, int iHeight, bool bVis, bool bDisable, bool bHalo, float fHaloSpeed, bool bLoop, float fMaxBlend, const char *szInvImage, const char *baseDir)
{
   _type = SCENEOBJECT_IMAGE;
   strcpy(_szImageFileName, szFileName);
   strcpy(_szObjectName, szObjectName);
   strcpy(_szType, szType);
   strcpy(_szInvImageName, szInvImage);
   _bVisible = bVis;
   _bDisable = bDisable;
   _bHalo = bHalo;
   
   if ( strlen(szFileName)>0 ) {
      KAssert(_lpSmartImage == NULL);
      if ((strstr(szFileName,".") == NULL)) { 
         int nFPS = 24;
         if (nNbFrame > 1)
            nFPS = nNbFrame;
         _lpSmartImage = new ESmartImage((char *)szFileName, fx, fy, _bVisible, bFade, nFadeDuration, bLoop, nFPS);
      } else if (strstr(szFileName,".ogv") != NULL) {
         _lpVideo = EImageBank::getVideo(_szImageFileName,bLoop);
         _lpSmartImage = new ESmartImage(_lpVideo, fx, fy, _bVisible, bFade, nFadeDuration);
      } else {
         if (baseDir == NULL) {
            _lpImageGraphic = EImageBank::getImage(_szImageFileName);
         } else {
            _lpImageGraphic = EImageBank::getImage(_szImageFileName,".png",false,false,false,baseDir);
         }
         _lpSmartImage = new ESmartImage(_lpImageGraphic, fx, fy, nNbFrame, _bVisible, bFade, nFadeDuration);
      }
      assert(_lpSmartImage->IsValid());
      K_LOG("ESceneObject: create smart image %s, id:%08x",_szImageFileName,_lpSmartImage);
      if (_lpSmartImage) {
         _lpSmartImage->setFixedBlend(fMaxBlend);
         _lpSmartImage->ForceVisible(_bVisible);
         _lpSmartImage->setHalo(bHalo, fHaloSpeed);
      }
   }
   
   if (strlen(_szInvImageName) >0 && strstr(_szInvImageName,".") == NULL) {
      char szPattern[K_MAXPATH];
      memset(szPattern, 0, K_MAXPATH);
      strcpy(szPattern, _szInvImageName);
      strcat(szPattern,"_%d.png");
      int nindex = GetIndex(_szObjectName);
      sprintf(_szInvImageName, szPattern, nindex);
   }

   // Coordonnées et taille
   _x = fx;
   _y = fy;
   // Conserve une coordonnée de référence (utile pour SetPosition/RestorePosition)
   _originalx = fx;
   _originaly = fy;
   // Si les tailles ne sont pas précisées, on prend l'image
   if (iWidth != 0 && iHeight != 0) {
      _w = iWidth;
      _h = iHeight;
   }
   else if (_lpSmartImage != NULL) {
      _w = _lpSmartImage->getWidth();
      _h = _lpSmartImage->getHeight();      
   }
   // Destination
   _bExit = false;
   memset(_szDestination, 0, 256);
   // Info text
   _bInfoText = false;
   memset(_szInfoText, 0, 256);
}

void SceneObjectImage::SetDestination(const char *szDestination, bool bDoor, bool bWalkTo, bool bWalkBack)
{
   _bExit = true;
   _bExitDoor = bDoor;
   _bExitWalkTo = bWalkTo;
   _bExitWalkBack = bWalkBack;
   strcpy(_szDestination, szDestination);
//   SetGlitch();
}

void SceneObjectImage::SetInfoText(const char *szInfoText)
{
   if (strlen(szInfoText)>0) {
      _bInfoText = true;
      strcpy(_szInfoText, szInfoText);
   }
   else {
      _bInfoText = false;
      memset(_szInfoText, 0, 256);
   }
}

void SceneObjectImage::SetMousePointer(const char *szMousePointer)
{
   KAssert(szMousePointer != NULL);
   strcpy(_szMousePointer, szMousePointer);
   // Les zones ayant un pointer spécifique ont un glitch
   if (strlen(szMousePointer) > 0 && (strcmp(szMousePointer, "arrow_puzzle") == 0 || strcmp(szMousePointer, "arrow_look") == 0 || strcmp(szMousePointer, "arrow_action") == 0 ) ) {
      // En mode aventure, pas de glit
      if (TestGlobal("__mode_adventure__")) {
         // Nothing
      }
      else {
         SetGlitch();
      }

   }
   // Les objets à ramasser ont un glit différé
   if (strlen(szMousePointer) > 0 && strcmp(szMousePointer, "arrow_take") == 0) {
      // En mode aventure, pas de glit
      if (TestGlobal("__mode_adventure__")) {
         // Nothing
      }
      else {
         SetGlitchTempo();
      }
      
   }
}

void SceneObjectImage::SetGlitchTempo()
{
   _lpGlitchCounter = new KCounter();
   _lpGlitchCounter->startCounter(0, 1, 0, GetRandom(20000, 40000), K_COUNTER_LINEAR);
}

void SceneObjectImage::SetGlitch()
{
   // Glitch désactivé pour cette image
   if (!_bGlitch) {
      return;
   }
   // Pas de glitch sur le bouton "back"
   if (strcmp(_szObjectName, "back") == 0) {
      return;
   }
   _lpGlitchEmitter = new ParticleEmitter(_x + _w/2, _y + _h/2);  
   if (!_lpGlitchEmitter->SetParticleBitmap("glow")) {
      showFileError ("Cannot load particule-etoile2 for Glitch");
      return;
   }
   _lpGlitchEmitter->SetType(VFX_FOCUS);
   if (_bVisible) {
      _lpGlitchEmitter->Start();
   }
}

void SceneObjectImage::SetVisible(bool bVis)
{
   if (_lpSmartImage)
      _lpSmartImage->SetVisible(bVis, true);
   _bVisible = bVis;
   if (_lpGlitchEmitter) {
      if (bVis) {
         _lpGlitchEmitter->Start();
      }
      else {      
         _lpGlitchEmitter->Stop();
      }
   }
}

void SceneObjectImage::SetSmoothOverlap(bool b)
{
   if (_lpSmartImage)
      _lpSmartImage->setSmoothOverlap(true);
}

void SceneObjectImage::Move(float x, float y)
{
   _x = x;
   _y = y;
   if (_lpSmartImage)
      _lpSmartImage->Move(x, y);
   if (_lpGlitchEmitter) {
      _lpGlitchEmitter->Move(x, y);
   }
}

// *******************************************************************************
// *******************************************************************************
// SCENE OBJECT PARTICLE EMITTER

void SceneObjectParticleEmitter::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   if (_lpParticleEmitter1) _lpParticleEmitter1->Render(fElapsed);
   if (_lpParticleEmitter2) _lpParticleEmitter2->Render(fElapsed);
   if (_lpCreepingEmitter) {
      _lpCreepingEmitter->Render();
   }
}

void SceneObjectParticleEmitter::Logic()
{
   if (_lpParticleEmitter1) _lpParticleEmitter1->Logic();
   if (_lpParticleEmitter2) _lpParticleEmitter2->Logic();
   if (_lpCreepingEmitter) {
      _lpCreepingEmitter->Logic();
   }
}

void SceneObjectParticleEmitter::Start()
{
   K_LOG("Start Emitter %s", _szObjectName);
   _bActive = true;
   if (_lpParticleEmitter1) _lpParticleEmitter1->Start();
   if (_lpParticleEmitter2) _lpParticleEmitter2->Start();
   if (_lpCreepingEmitter) _lpCreepingEmitter->Start();
}

void SceneObjectParticleEmitter::Stop()
{
   K_LOG("Stop Emitter %s", _szObjectName);
   _bActive = false;   
   if (_lpParticleEmitter1) _lpParticleEmitter1->Stop();
   if (_lpParticleEmitter2) _lpParticleEmitter2->Stop();
   if (_lpCreepingEmitter) _lpCreepingEmitter->Stop();
}

void SceneObjectParticleEmitter::SetVisible(char *szObjectName, bool bVis)
{
   //	_strObjectName = szObjectName; // ??
}

void SceneObjectParticleEmitter::SetData(const char *szName, const char *szType, float x, float y, int w, int h, bool bStart, bool bLoop)
{
	_type = SCENEOBJECT_PARTICLEEMITTER;
	strcpy(_szObjectName, szName);
	_fx = x;
	_fy = y;
   _w = w;
   _h = h;
   _bStart = bStart;
   
   // Init de l'émetteur
   if (strcmp(szType, "flake") == 0) {
      _lpParticleEmitter1 = new ParticleEmitter(x, y);  
      if (!_lpParticleEmitter1->SetParticleBitmap("particule-etoile2")) {
         showFileError ("particule-etoile2.png");
         return;
      }
      _lpParticleEmitter1->SetHeightEffect(w);
      _lpParticleEmitter1->SetEmitRate(1);
      _lpParticleEmitter1->SetType(VFX_FLAKE);
      _lpParticleEmitter1->AddSegment(_fx - (_w/2), _fy, _fx + (_w/2), _fy, _w/3);  /* create segment where emitter will move */
   }
   if (strcmp(szType, "fire") == 0) {
      _lpParticleEmitter1 = new ParticleEmitter(x, y);  
      if (!_lpParticleEmitter1->SetParticleBitmap("particle")) {
         showFileError ("particle.png");
         return;
      }
      _lpParticleEmitter1->SetEmitRate(2);
      _lpParticleEmitter1->SetType(VFX_FIRE);
      _lpParticleEmitter1->SetHeightEffect(w);
      _lpParticleEmitter1->AddSegment(_fx, _fy, _fx - (_w/2), _fy, 8);
      
      _lpParticleEmitter2 = new ParticleEmitter(x, y);  
      if (!_lpParticleEmitter2->SetParticleBitmap("particle")) {
         showFileError ("particle.png");
         return;
      }
      _lpParticleEmitter2->SetEmitRate(2);
      _lpParticleEmitter2->SetType(VFX_FIRE);
      _lpParticleEmitter2->SetHeightEffect(w);
      _lpParticleEmitter2->AddSegment(_fx, _fy, _fx + (_w/2), _fy, 8);
   }
   if (strcmp(szType, "creepingsmoke") == 0) {
      _lpCreepingEmitter = NULL;
   }
}

void SceneObjectParticleEmitter::SetCreeping(const char *szPreset, const char *szAnimation, float x, float y,float scale)
{
   strcpy(_szPreset, szPreset);
   strcpy(_szAnimation, szAnimation);
   std::string tmp(_szSceneName);
   tmp += _szObjectName;
   _lpCreepingEmitter = EEmitterBank::getEmitter(tmp.c_str(),_szPreset,_szAnimation);

   if (x!=0 || y!=0)
      _lpCreepingEmitter->Move(x,y);
   if (scale != 0)
      _lpCreepingEmitter->ReScale(scale);

#ifdef DEBUGEMITTER
   _lpCreepingEmitter->_bDisplay = true;
   _lpCreepingEmitter->_bUseKeys = true;
#endif

   if (_bStart)
   {
      Start();
   }
   else {
      Stop();
   }
}

void SceneObjectParticleEmitter::SetScale(float fScale)
{
   _lpCreepingEmitter->ReScale(fScale);
}

SceneObjectParticleEmitter::~SceneObjectParticleEmitter()
{
   XDELETE(_lpParticleEmitter1);
   XDELETE(_lpParticleEmitter2);
   if (_lpCreepingEmitter != NULL)
      _lpCreepingEmitter->Delete();
   _lpCreepingEmitter = NULL;
}

// *******************************************************************************
// SCENE OBJECT ANIMATION
// *******************************************************************************

void SceneObjectAnimation::SetData(int scene_id, const char *szObjectName,const char *szTarget, const char *szType, bool bLoop, int iAngle1, int iAngle2, int nDirection, bool bBounce, float x, float y, float x2, float y2, float fDuration, int iCounterF, int iDelayMin, int iDelayMax, int iDir)
{
   _type = SCENEOBJECT_ANIMATION;
   nDir = nDirection;
   _strObjName = szObjectName;
   _strType = szType;
   _strTarget = szTarget;
   _iAng1 = iAngle1;
   _iAng2 = iAngle2;
   _iCounter = iCounterF;
   _fDur = fDuration;
   _bBounce = bBounce;
   _bLoop = bLoop;
   _fx = x;
   _fy = y;
   _fx2 = x2;
   _fy2 = y2;
   _iDelayMin = iDelayMin;
   _iDelayMax = iDelayMax;
   _iDir = iDir;
}

void SceneObjectAnimation::Draw()
{
   
}

void SceneObjectAnimation::Logic()
{
	
}

SceneObjectAnimation::~SceneObjectAnimation()
{
   
}

// *******************************************************************************
// SCENE OBJECT TEXT
// *******************************************************************************
SceneObjectText::SceneObjectText(const char *szSceneName): SceneObject(szSceneName),_lpFont(NULL),_lpCounterFade(NULL) {
}
SceneObjectText::~SceneObjectText() {
   XDELETE(_lpFont);
   XDELETE(_lpCounterFade);
}

void SceneObjectText::SetData(const char *szObjectName, const char *szText, const char *szFont, int nFontSize, int nStyle, float fX1, float fX2, float fY, float fRotation, int nFadeDuration)
{
   _type = SCENEOBJECT_ANIMATION;
   _strObjName = szObjectName;
   // Extraction du texte dans le fichier de traduction
   if (KGame::getGame()->getString (szText)) {
      _strText = KStr(szText);
   } else {
      _strText = "**TEXT NO FOUND**";
   }
   
   _fRotation = fRotation;
   _lpFont = EFontBank::getFont(szFont, nFontSize);
   _nStyle = nStyle;
   _fX1 = fX1;
   _fX2 = fX2;
   _fY = fY;
   _nFadeDuration = nFadeDuration;
   _lpCounterFade = new KCounter();
   // On passe en invisible et on force le fade à se finir
   SetVisible(false);
   _lpCounterFade->move(99999); 
}

void SceneObjectText::SetVisible(bool bVis)
{
   float fCurrent = _lpCounterFade->getCurrentValue();
   // On passe de visible à invisible
   if (_bVisible && !bVis) {
      _lpCounterFade->startCounter(fCurrent, 0.0f, 0, _nFadeDuration, K_COUNTER_LINEAR);
   }
   // On passe de invisible à visible
   else if (!_bVisible && bVis)
   {
      _lpCounterFade->startCounter(fCurrent, _fAlpha, 0, _nFadeDuration, K_COUNTER_LINEAR);
   }
   _bVisible = bVis;
}

void SceneObjectText::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   _lpCounterFade->move(fElapsed);
   
   if (_bVisible == false) {
      return;
   }
   
   // Dessine le texte
   //#define TEXTSTYLE_LEFT        0  /**< Align lines left */
   //#define TEXTSTYLE_RIGHT       1  /**< Align lines right */
   //#define TEXTSTYLE_CENTER      2  /**< Center each line horizontally */
   //#define TEXTSTYLE_JUSTIFIED   3  /**< Make each line the same width by increasing the width of spaces between words to fill the width */
   //#define TEXTSTYLE_WRAP        4  /**< Make each line the same width by breaking words in the middle if necessary */
   //#define TEXTSTYLE_EDIT        5  /**< Make each line the same width by breaking words in the middle if necessary, and don't eliminate spaces at the end of each line */
   _lpFont->setColor(_fR, _fV, _fB, _lpCounterFade->getCurrentValue());
   
   if (_fRotation != 0) {
      KGame::getGame()->_lpKWindow->setWorldView(0, 0, _fRotation, 1, false);
   }
   _lpFont->drawMultiline(_strText.c_str(), _fX1, _fX2, _fY, _nStyle);
   if (_fRotation != 0) {
      KGame::getGame()->_lpKWindow->setWorldView(0, 0, 0, 1, false);
   }
}

void SceneObjectText::Logic()
{	
}

EObjectContainer::EObjectContainer(const std::string strSceneName, const std::string strObjectName) : _type(SCENEOBJECT_UNKNOW) {
   _strSceneName = strSceneName;
   _lpObjectImage = NULL;
   _lpObjectEmitter = NULL;
   _lpObjectAnime = NULL;
   _lpObjectText = NULL;
   memset(_szObjectName,0, 256);
   strcpy(_szObjectName, strObjectName.c_str());
   K_LOG("EObjectContainer: create SceneObject %s, id:%08x",_szObjectName,this);
}

EObjectContainer::~EObjectContainer()
{
   XDELETE(_lpObjectImage);
   XDELETE(_lpObjectEmitter);
   XDELETE(_lpObjectAnime);
   XDELETE(_lpObjectText);
   K_LOG("EObjectContainer: SceneObject deleted %s, id:%08x",_szObjectName,this);
}


void EObjectContainer::CleanUp(const std::string strSceneName){
   // Parcours la liste pour en récupérer les valeurs et la vide
   K_LOG("EObjectContainer cleanup scene: %s",strSceneName.c_str());

   int nDeleted = 0;
   EObjectContainer *o;
   EObjectContainer *otodelete;
   bool bGotNext = g_lpListObj.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (o->_strSceneName == strSceneName) {
            nDeleted++;
            g_lpListObj.remove(o);
            otodelete = o;
         } else {
            otodelete = NULL;
         }
         bGotNext = g_lpListObj.getNext (o);
         XDELETE(otodelete);
      }
   }
   if (nDeleted >0)
      K_LOG("EObjectContainer cleanup %04d object deleted in scene %s",nDeleted,strSceneName.c_str());
}

void EObjectContainer::CleanUpAll(){
   // Parcours la liste pour en récupérer les valeurs et la vide
   EObjectContainer *o;
   EObjectContainer *otodelete;
   bool bGotNext = g_lpListObj.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         g_lpListObj.remove(o);
         otodelete = o;
         bGotNext = g_lpListObj.getNext (o);
         XDELETE(otodelete);
      }
   }
}