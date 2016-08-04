/*
 *  EScene.h
 *
 *  Created by Rockford on 05/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_ESCENE_H
#define  _ENIGMA_ESCENE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "KPTK.h"
#include "KCounter.h"
#include "ESmartImage.h"
#include "KUIButton.h"
#include "ESceneObject.h"
#include "GlobalBank.h"
#include "SoundBank.h"

// *******************************************************************************
// definition for file types used in scene files
#define PYRO		"pyro"
#define IMG_PNG		"png"
#define IMG_JPG		"jpg"
#define FILENAME	"file"
#define OBJECTID	"id"
#define OBJECTNAME	"name"
#define MOUSECURSOR	"mousecursor"
#define COORDX		"x"
#define	COORDY		"y"
#define COORDX2		"x2"
#define	COORDY2		"y2"
#define WIDTH		"width"
#define HEIGHT		"height"
#define VISIBLE		"visible"
#define	STATE		"state"
#define ANIMATE		"animate"
#define ANIMTYPE	"type"
#define LOOP		"loop"
#define NBFRAME		"nbframe"
#define BFADE		"bFade"
#define NFADE		"nFade"
#define FMAXBLEND	"fMaxBlend"
#define ANIM_MARQUEE "marquee"
#define ANIM_VIDEO	"video"
#define ANIM_FRAME	"frame"
#define ANIM_P2P	"p2p"
#define ANIM_ROTATE	"rotate"
#define ANIM_TARGET	"target"
#define ANGLE1		"angle1"
#define ANGLE2		"angle2"
#define DIRECTION	"direction"
#define BOUNCE		"bounce"
#define DURATION	"duration"
#define COUNTERFUNC	"counterfunction"
#define DELAYMIN	"delaymin"
#define DELAYMAX	"delaymax"
#define INVIMAGE	"invimage"
#define BASEDIR	"basedir"

typedef enum {
   SCENE_STABLE = 0,
   SCENE_SHOWIN,
   SCENE_SHOWOUT,
} SceneFadeState;

class ESceneDirector;

class EScene {
public:
   /** Constructor */
   EScene ( ESceneDirector* lpSceneDirector );

   virtual ~EScene ( );
   
   virtual void Init ();
   virtual bool AskForClose();
   virtual void Close ();
   
   /** 3 steps rendering **/
   // Check (use this for input/UI purpose)
   virtual void Check ();
   // Logic (use this for logic, will be called during the 10ms update phase)
   virtual void Logic ();
   // Draw (use this for drawing, just drawing)
   virtual void Draw ();
   
   SceneFadeState GetSceneState() {return _State;}
   
   // Set any scene object visible or not
   void SetVisible (const char *szObjectName, bool bVis, bool bForce = false);
   bool isVisible(const char *szObjectName);
   // Change la position de l'objet
   void SetPosition(const char *szObjectName, float fX, float fY);
   // Restore la position qu'avait l'objet à sa création
   void RestorePosition(const char *szObjectName);
   // Déplace l'objet d'une distance relative
   void MovePosition(const char *szObjectName, float fX, float fY);
   // Informe l'objet qu'il devra éviter les fadein/fadeout lors des transitions entre frames
   void SetSmoothOverlap (char *szObjectName, bool b);

   // Scene Parser
   void parseSceneFile (const char *szFileName);
   
   void GetRotationStatus(char *szObjectName);
   void GetRotationAngle(char *szObjectName);
   void GetAnimationP2PStatus(char *szObjectName);
   void GetObjectPosition(const char *szObjectName, int &x, int &y, bool bCenterX=true, bool bCenterY=false);
   
   // Recherche d'un object image par son nom (donné dans le fichier scene)
   SceneObjectImage *GetObjectImageByName(const char *szObjectName);
   // Recherche d'un object Emitter par son nom (donné dans le fichier scene)
   SceneObjectParticleEmitter *GetObjectEmitterByName(const char *szObjectName);	
   // Recherche d'un object Animation par son nom (donné dans le fichier scene)
   SceneObjectAnimation *GetObjectAnimationByName(const char *szObjectName);
   // Recherche d'un object text par son nom (donné dans le fichier scene)
   SceneObjectText *GetObjectTextByName(const char *szObjectName);
   
	// Gestion des Emitters Pyro
   void AddEmitter (const char *szObjectName);
   // Démarrage Emitter
   void StartEmitter (const char *szObjectName);
	// Arrêt Emitter
   void StopEmitter(const char *szObjectName);
	// Déplacement d'un Emitter
	void MoveEmitter (const char *szObjectName, float x, float y);
   
	// Démarrage Animation
	void StartAnimation(const char *szAnime);
	// Stoppe l'animation
	void StopAnimation (const char *szAnime);

	// Gestion des zones de click - Renvoie true pour prendre la main sur le traitement par défaut
   /* Un objet de la scène est cliqué */
	virtual bool ObjectClicked (const char *szObjectName, float x, float y);
   /* Un objet de la scène est survolé */
	virtual bool ObjectOver (char *szObjectName, float x, float y);
   /* RAZ de l'objet survolé */
   void ResetMouseOver() {
      memset(_szCurrentObjectOver, 0, 256);
   }
   /* Utilisation d'un objet de m'inventaire sur un objet de la scène */
   virtual bool ItemUsed(const char *szItemName, const char *szObjectName) { return false;};
   /* Fin d'un mini jeu, avec résultat */
   virtual void MiniGameDone(const char *szGameName, bool bIsRevolved) {};
   /* Méthode spéciale de callback utilisée par la séquence Callback */
   virtual void Callback(const char *szParam) {};
   /* Info de survol */
   const char *GetMouseOverInfo() {
      return _szMouseOverInfo;
   }
   
   // Propriétés dynamiques de la scène
   virtual bool IsNeedIHM() {
      return true;
   }
   virtual char *getDebugString() {
      return "F5:Menu";
   }
   
   void PlayWalkSound();

	std::string _strSceneName;
   void SetName(std::string str)
   {
      _strSceneName = str;
   }
   std::string GetName()
   {
      return _strSceneName;
   }
   
	std::string _strSceneAdditionalName;
   void setAdditionalName(std::string str)
   {
      K_LOG("SetAdditionalName(%s);",str.c_str());
      _strSceneAdditionalName = str;
   }
   
   std::string getAdditionalName()
   {
      return _strSceneAdditionalName;
   }
   
	std::string _strSceneUniverseName;
   void SetUniverseName(std::string str)
   {
      _strSceneUniverseName = str;
   }
   
   void StartFade(SceneFadeState state);
   bool FadeOutCompleted() {
      return _lpAnimOut->isCompleted();
   }
   bool FadeInCompleted() {
      return _lpAnimIn->isCompleted();
   }
   float getFade() {
      switch (_State) {
         case SCENE_SHOWIN:
            if (!isVignette()) {
               return 1.0f-_lpAnimIn->getCurrentValue();
            }
            else {
               return _lpAnimIn->getCurrentValue();
            }
            break;
         case SCENE_SHOWOUT:
            if (!isVignette()) {
               return 1.0f-_lpAnimOut->getCurrentValue();
            }
            else {
               return _lpAnimOut->getCurrentValue();
            }
            break;
         case SCENE_STABLE:
            return 1.0f;
            break;
         default:
            break;
      }
      return 1.0f;
   }
   void setVignette(bool bAlpha) {
      _bVignette = true;
      _bVignetteAlpha = bAlpha;
      // Préparation de Target pour dessiner la scène dedans...
      _lpGraphicTarget = KPTK::createKGraphic();
      _lpGraphicTarget->makeRenderTarget(1024, 768, true, bAlpha, true);
      _lpGraphicTarget->setTextureQuality(true);
      _lpGraphicTarget->setAlphaMode(K_ALPHA_NORMAL);
      _bRefreshTarget = true;
   }
   bool isAlpha() {
      return _bVignetteAlpha;
   }
   bool isVignette() {
      return _bVignette;
   }
   
   void AddZone(const char *szName, int x, int y, int w, int h, const char *szMousePointer, bool bVisible);

   void SetMouseOverInfo(const char *sz1, const char *sz2);
   
   // Facilitateurs
   // Rend visible/invisible un objet dans la scène selon si il a été déjà ramassé ou pas
   void SetupItem(const char *szObjectName);

   bool ItemCollected(const char *szObjectName)
   {
      return TestGlobal(szObjectName) == true;
   }
   void KeepItem() {
      _bKeepHoldItem = true;
   }
   void PickupSimple(const char *szObjectName, const char *szInventoryName);
   void PickupMultiple(const char *szObjectName, const char *szInventoryName, int nNbPartNeeded);
   
   // Chaine entourée de noir
   // bCustomColor = utilise la couleur courante de la font pour le corps de texte (le contour reste noir)
   void DrawFancyString(const char *szString, KTextFace *lpFont, float fx, float fy, float fw, bool bCentered, float fBlend, bool bCustomColor = false, float r=1.0f, float v=1.0f, float b=1.0f);
   
   void DrawMultiLineFancyString(const char *szString, KTextFace *lpFont, float fx, float fx2, float fy, float fBlend, float r = 1.0f, float v = 1.0f, float b = 1.0f);
   
   void StartSoundDesign() {
      if (_lpSoundDesign) {
         _lpSoundDesign->playStream(true);
      }
   }

   void StopSoundDesign() {
      if (_lpSoundDesign) {
         _lpSoundDesign->stopStream();
      }
   }
   
protected:
   KGraphic *_lpGraphicTarget;
   bool _bRefreshTarget;
   ESceneDirector *_lpSceneDirector;
   SceneFadeState _State;
	KCounter *_lpAnimIn;
	KCounter *_lpAnimOut;
   bool _bVignette;
   bool _bVignetteAlpha;
   char _szNextSceneName[256];
   char _szNextVignetteName[256];
   char _szCurrentObjectOver[256];
   char _szMouseOverInfo[256];
   void showFileError (const char *lpszFileName);
   bool MouseOver(long mousex, long mousey, float xx, float yy, int ww, int hh);
   void DrawFadeOverlay();
   
private:
   char _szWalkSound[256];
   char _szSoundDesign[256];
   KSound *_lpSoundDesign;
   KTextFace *_lpFont;
	KGraphic *_lpBgGraphic2;
	KGraphic *_lpMousePointerGraphic;
	char *_strFileName;
	char *_szObjectName;
	bool bVisible;
   bool _bKeepHoldItem;  // indique qu'on va garder l'item à la main après ItemUsed
	void setName(const std::string& something);
	float GetFloatToken (const char *szLine, char *szTokenName, float fDefault=0.0);
	int GetIntToken (const char *szLine, char *szTokenName);
   bool GetStringToken (const char *szLine, const char *szTokenName, std::string& strOutput);
   bool GetBoolToken(const char *szLine, const char *szTokenName, bool bDefault);
};

#endif