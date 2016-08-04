/*
 *  ESequenceBase.h
 *  enigma
 *
 *  Created by Rockford on 05/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef ESEQUENCEBASE_H
#define ESEQUENCEBASE_H

#include "KPTK.h"
#include <string>

/* Constantes */
#define SEQUENCE_TEXT_CHARDURATION 100

class ESequenceBase {
public:
   ESequenceBase() { _bActive = false; _bCheckAllowed = true;  };
   ~ESequenceBase() {   };
   virtual void Logic() = 0;
   virtual void Check() = 0;
   virtual void Draw() = 0;
   virtual bool isCompleted() = 0;
   virtual void Start() {
      _bActive = true;
   }
   virtual bool isModal() = 0;
   
   bool isActive() {
      return _bActive;
   }
   bool isCheckAllowed() {
      return _bCheckAllowed;
   }
protected:
   bool _bActive;
   bool _bCheckAllowed;
};

typedef enum {
   SCENESEQUENCE_UNKNOW = -1,
   SCENESEQUENCE_VOICEOVER,
   SCENESEQUENCE_TALK,
   SCENESEQUENCE_EARTHQUAKE,
   SCENESEQUENCE_IMAGESHOW,
   SCENESEQUENCE_IMAGESWITCH,
   SCENESEQUENCE_EMITTERSHOW,
   SCENESEQUENCE_PLAYSOUND,
   SCENESEQUENCE_ANIMATION,
   SCENESEQUENCE_PICKUPITEM,
   SCENESEQUENCE_DIALOGMODE,
   SCENESEQUENCE_CLOSEVIGNETTE,
   SCENESEQUENCE_GOTOSCENE,
   SCENESEQUENCE_WAIT,
   SCENESEQUENCE_MINIGAME,
   SCENESEQUENCE_ENERGY,
   SCENESEQUENCE_TUTOBOX,
   SCENESEQUENCE_OBJECTIVE,
   SCENESEQUENCE_GOTOVIDEO,
   SCENESEQUENCE_CALLBACK
} SceneSequenceType;

typedef enum {
   SEQUENCE_NARRATION_DIALOG = 0,
   SEQUENCE_NARRATION_CINEMATIC,
   SEQUENCE_NARRATION_LOCKMOUSE
} SceneSequenceNarrationMode;

class ESequenceVoiceOver;
class ESequenceTalk;
class ESequenceEarthQuake;
class ESequenceShowImage;
class ESequenceSwitchImage;
class ESequenceShowEmitter;
class ESequencePlaySound;
class ESequenceAnimation;
class ESequencePickupItem;
class ESequenceNarrationMode;
class ESequenceCloseVignette;
class ESequenceGotoScene;
class ESequenceWait;
class ESequenceDialogBoxOK;
class ESequenceMinigame;
class ESequenceEnergy;
class ESequenceTutobox;
class ESequenceObjective;
class ESequenceGotoVideo;
class ESequenceCallback;

class ESequenceContainer : public KObjectListable {
public:
   ESequenceContainer(const std::string strSceneName, const std::string strSequenceName, SceneSequenceType type);
   
	char _szSequenceName[256];
   std::string _strSceneName;
	SceneSequenceType _type;
   
   // Pointeur vers les différents types de séquences possibles
   ESequenceVoiceOver *_lpSequenceVoiceOver;
   ESequenceTalk *_lpSequenceTalk;
   ESequenceEarthQuake *_lpSequenceEarthQuake;
   ESequenceShowImage *_lpSequenceShowImage;
   ESequenceSwitchImage *_lpSequenceSwitchImage;
   ESequenceShowEmitter *_lpSequenceShowEmitter;
   ESequencePlaySound *_lpSequencePlaySound;
   ESequenceWait *_lpSequenceWait;
   ESequenceDialogBoxOK *_lpSequenceDialogBoxOK;
   ESequenceAnimation *_lpSequenceAnimation;
   ESequencePickupItem *_lpSequencePickupItem;
   ESequenceNarrationMode *_lpSequenceNarrationMode;
   ESequenceCloseVignette *_lpSequenceCloseVignette;
   ESequenceGotoScene *_lpSequenceGotoScene;
   ESequenceMinigame *_lpSequenceMinigame;
   ESequenceEnergy *_lpSequenceEnergy;
   ESequenceTutobox *_lpSequenceTutobox;
   ESequenceObjective *_lpSequenceObjective;
   ESequenceGotoVideo *_lpSequenceGotoVideo;
   ESequenceCallback *_lpSequenceCallback;
   
   // Liste
	static KList<ESequenceContainer> g_ListSequence;

   ESequenceBase *getContent();
   void DeleteContent();
   
   // Destructeur de chaque séquence
   static void CleanUp(const std::string strSceneName);
   static void CleanUpAll();
   static void Purge(const char *szName);
   
};

#endif