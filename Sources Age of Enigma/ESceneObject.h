/*
 *  ESceneObject.h
 *  enigma
 *
 *  Created by Rockford on 27/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "KPTK.h"
#include "KCounter.h"
#include "ESmartImage.h"
#include "KUIButton.h"
#include "ESceneObject.h"
#include "ParticleFactory.h"
#include "CreepingSmoke.h"

#include "global.h"

typedef enum {
   SCENEOBJECT_UNKNOW = -1,
   SCENEOBJECT_IMAGE,
   SCENEOBJECT_PARTICLEEMITTER,
   SCENEOBJECT_ANIMATION,
   SCENEOBJECT_TEXT
} SceneObjectType;

class SceneObject {
public:
   SceneObjectType _type;
   char _szObjectName[256];
   char _szSceneName[256];
   SceneObject(const char *szSceneName) : _type(SCENEOBJECT_UNKNOW) {
      memset(_szObjectName, 0, 256);
      memset(_szSceneName, 0, 256);
      strcpy(_szSceneName, szSceneName);
   };
   virtual ~SceneObject(){
      
   };
   
   virtual void Draw(){
      
   };
   
   virtual void Logic(){
      
   };
   
};

class SceneObjectText : virtual public SceneObject {
public:
   SceneObjectText(const char *szSceneName);
   ~SceneObjectText();

   virtual void Draw();
   virtual void Logic();
   void SetData(const char *szObjectName, const char *szText, const char *szFont, int nFontSize, int nStyle, float fX1, float fX2, float fY, float fRotation, int nFadeDuration);
   void SetColor(float fR, float fV, float fB, float fA) {
      _fR = fR;
      _fV = fV;
      _fB = fB;
      _fAlpha = fA;
   }
   void SetVisible(bool bVis);
   
   std::string _strObjName;
   std::string _strText;
   int _nStyle;
   float _fRotation;
   float _fX1, _fX2, _fY;
   float _fR, _fV, _fB, _fAlpha;
   int _nFadeDuration;
   bool _bVisible;
private:
   KTextFace *_lpFont;
   KCounter *_lpCounterFade;
};

class SceneObjectAnimation : virtual public SceneObject {
public:
   SceneObjectAnimation(const char *szSceneName) : SceneObject(szSceneName) { };
   virtual void Draw();
   virtual void Logic();
   virtual void SetData(int scene_id, const char *szObjectName, const char *szTarget, const char *szType, bool bLoop, int iAngle1, int iAngle2, int nDirection, bool bBounce,float x, float y, float x2, float y2, float fDuration, int iCounterF, int iDelayMin, int iDelayMax, int iDir);
   virtual ~SceneObjectAnimation();
   std::string _strObjName;
   std::string _strType;
   std::string _strTarget;
   int _iAng1, _iAng2, _iCounter, _iDir;
   bool _bBounce, _bLoop, _bIsCompleted;
   float _fDur, _fx, _fy, _fx2, _fy2;
   int nDir, _iDelayMin, _iDelayMax;
private:
   ESmartImage *_lpSmartImage;
   KGraphic *_lpImageGraphic;
   
};

class SceneObjectImage : virtual public SceneObject {
public:
   SceneObjectImage(const char *szSceneName) : SceneObject(szSceneName) {
      _bVisible = false;
      _bDisable = false;
      _bHalo = false;
      _lpSmartImage = NULL;
      _x = 0;
      _y = 0;
      _w = 0;
      _h = 0;
      _originalx = 0;
      _originaly = 0;
      memset(_szDestination, 0, 256);
      memset(_szMousePointer, 0, 256);
      memset(_szImageFileName, 0, 1024);
      memset(_szType, 0, 256);
      memset(_szObjectName, 0, 256);
      memset(_szInvImageName, 0, K_MAXPATH);
      _lpGlitchEmitter = NULL;
      _bGlitch = false;
      _lpGlitchCounter = NULL;
   };
   virtual void Draw();
   virtual void Logic();
   void SetData(const char *szFileName, int scene_id, const char *szObjectName, const char *szType, int nNbFrame=1,
                bool bFade=false, int nFadeDuration = 2000, float x=0.0f, float y=0.0f, int iWidth=0, int iHeigth=0,
                bool bVis=true, bool bDisable=false, bool bHalo = false, float fHaloSpeed = 0.0, bool bLoop=true,
                float fMaxBlend=1.0, const char *szInvImage="", const char *baseDir=NULL);
   void SetPosition(float fX, float fY)
   {
      _x = fX;
      _y = fY;
      if (_lpSmartImage) {
         _lpSmartImage->SetPosition(_x, _y);
      }
   }
   void RestorePosition()
   {
      if (_lpSmartImage) {
         _lpSmartImage->SetPosition(_originalx, _originaly);
      }
   }
   void SetMousePointer(const char *szMousePointer);
   void SetDestination(const char *szDestination, bool bDoor, bool bWalkTo, bool bWalkBack);
   void SetInfoText(const char *szInfoText);
   void SetGlitch();
   // Active le glitch, mais après une tempo
   void SetGlitchTempo();
   void EnableGlitch(bool b)
   {
      _bGlitch = b;
      if (_lpGlitchEmitter && b == false) {
         _lpGlitchEmitter->Stop();
      }
   }
   virtual void SetVisible(bool bVis);
   void SetSmoothOverlap(bool b);
   void Move(float x, float y);
   void StartAnimationByFrame(bool bStart);
   void StartRotationStatus(bool bRotate, int nDirection);
   
   virtual ESmartImage *GetSmartImage(){
      return _lpSmartImage;
   }
   SceneObjectImage();
   virtual ~SceneObjectImage();
   float _originalx, _originaly;
   float _x, _y;
   int _w, _h;
   bool isVisible() {
      return _bVisible;
   }
   bool isDisabled() {
      return _bDisable;
   }
   float getCurrentX() {
      if (_lpSmartImage != NULL)
      {
         return _lpSmartImage->getCurrentX();
      }
      else {
         return _x;
      }
   }
   float getCurrentY() {
      if (_lpSmartImage != NULL)
      {
         return _lpSmartImage->getCurrentY();
      }
      else {
         return _y;
      }
   }
   bool isFadeInCompleted() {
      if (_lpSmartImage != NULL)
      {
         return _lpSmartImage->isFadeInCompleted();
      }
      else {
         return true;
      }
   }
   bool isFadeOutCompleted() {
      if (_lpSmartImage != NULL)
      {
         return _lpSmartImage->isFadeOutCompleted();
      }
      else {
         return true;
      }
   }
   bool isExit() {
      return _bExit;
   }
   bool isExitDoor() {
      return _bExitDoor;
   }
   bool isExitWalkTo() {
      return _bExitWalkTo;
   }
   bool isExitWalkBack() {
      return _bExitWalkBack;
   }
   char *getDestination() {
      return _szDestination;
   }
   bool isInfoText() {
      return _bInfoText;
   }
   char *getInfoText() {
      return _szInfoText;
   }
   char *getMousePointer() {
      return _szMousePointer;
   }
   bool isMousePointer() {
      return strlen(_szMousePointer)>0;
   }
   const char* getInvImageName() {
      return _szInvImageName;
   }
   KGraphic *getGraphic() {
      return _lpImageGraphic;
   }
private:
   char _szImageFileName[1024]; 
   char _szType[256];
   char _szInvImageName[K_MAXPATH];
   ESmartImage *_lpSmartImage;
   /* Etats */
   bool _bVisible;
   bool _bDisable;
   bool _bHalo;
   /* S'affiche smooth */
   bool _bFade;
   /* Conduit vers une autre scène par une porte ou en marchant */
   bool _bExit;
   /* if _bExit... voilà les types des sorties */
   bool _bExitDoor;
   bool _bExitWalkTo;
   bool _bExitWalkBack;
   /* Scène de destination */
   char _szDestination[256];
   /* ID du texte d'info */
   bool _bInfoText;
   char _szInfoText[256];
   /* Pointeur souris en cas de survol */
   char _szMousePointer[256];
   /* Glitch */
   bool _bGlitch;
   KCounter *_lpGlitchCounter;
   ParticleEmitter *_lpGlitchEmitter;
   
   KGraphic *_lpImageGraphic;
   KVideo *_lpVideo;
};

class SceneObjectParticleEmitter : virtual public SceneObject {
public:
   SceneObjectParticleEmitter(const char *szSceneName) : SceneObject(szSceneName) {
      _lpParticleEmitter1 = NULL;
      _lpParticleEmitter2 = NULL;
      _lpCreepingEmitter = NULL;
      _fx = 0.0f;
      _fy = 0.0f;
      _w = 0;
      _h = 0;
      _bLoop = false;
      _bActive = false;
      _bStart = false;
      memset(_szPreset, 0, K_MAXPATH);
      memset(_szAnimation, 0, K_MAXPATH);
   };
	virtual void Draw();
	virtual void Logic();
	virtual void SetData(const char *szName, const char *szType, float x, float y, int w, int h, bool bStart, bool bLoop);
   void SetCreeping(const char *szPreset, const char *szAnimation, float x=0, float y=0, float scale=0);
   void SetScale(float fScale);
	virtual void SetVisible(char *szObjectName, bool bVis);
	virtual void Start();
	virtual void Stop();
   CreepingSmokeEmitter *getCreepingSmokeEmitter() {
      return _lpCreepingEmitter;
   }
   void setPos(float x, float y) {
      _fx = x;
      _fy = y;
      if (_lpCreepingEmitter) {
         _lpCreepingEmitter->Move(x, y);
      }
   }
	SceneObjectParticleEmitter();
	virtual ~SceneObjectParticleEmitter();
private:
	ParticleEmitter *_lpParticleEmitter1;
	ParticleEmitter *_lpParticleEmitter2;
   CreepingSmokeEmitter *_lpCreepingEmitter;
	float _fx, _fy;
   int   _w, _h;
   bool _bLoop;
   bool _bActive;
   bool _bStart;
   char _szPreset[K_MAXPATH];
   char _szAnimation[K_MAXPATH];
private:
	std::string _strPyroFileName;
};

class EObjectContainer : public KObjectListable {
public:
   EObjectContainer(const std::string strSceneName, const std::string strObjectName);
   ~EObjectContainer();

   static void CleanUp(const std::string strSceneName);
   static void CleanUpAll();

   std::string _strSceneName;
	SceneObjectType _type;
	char _szObjectName[256];
	static KList<EObjectContainer> g_lpListObj;
	SceneObjectImage *_lpObjectImage;
	SceneObjectParticleEmitter *_lpObjectEmitter;
	SceneObjectAnimation *_lpObjectAnime;
   SceneObjectText *_lpObjectText;
};
