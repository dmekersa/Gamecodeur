/*
 *  ESceneSequencer.h
 *  enigma
 *
 *  Created by Rockford on 05/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef SCENESEQUENCER_H
#define SCENESEQUENCER_H

#include "KPTK.h"
#include "ESceneDirector.h"
#include "ESequenceBase.h"
#include "ESequenceVoiceOver.h"
#include "ESequenceTalk.h"
#include "ESequenceEarthQuake.h"
#include "ESequenceMisc.h"
#include "EMiniJeuBase.h"

class ESceneSequencer {
public:
   ESceneSequencer( ESceneDirector* lpSceneDirector );
   void Logic();
   void Check();
   void Draw();
   
   /* Une séquence est elle active à cet instant ? */
   bool isActive() {
      return (_lpCurrentSequence != NULL);
   }
   bool isCheckAllowed();
   bool isModal() {
      return (_lpCurrentSequence->getContent()->isModal());
   }
   bool isEmpty();
   ESequenceContainer *isAlready(const char* szSequenceName);
   
   /* Méthodes utilisées par les scènes pour lancer des séquences */
   void VoiceOver(const char *szSequenceName, const char *szText, const char *szVoice, SequenceVoiceOverColor Color = SEQUENCE_VOICEOVER_GREY);
   void Talk(const char *szSequenceName, int nFromX, int nFromY, const char *szText, const char *szVoice, bool bCheckAllowed, bool bDialogMode = false, KSound *lpSoundStream = NULL);
   void EarthQuake(const char *szSequenceName, bool b, const char *szSoundName = NULL);
   void SwitchImage(const char *szSequenceName, const char *szImageName1, const char *szImageName2);
   void ShowImage(const char *szSequenceName, const char *szImageName, bool bShow, bool bModal = false);
   void ShowEmitter(const char *szSequenceName, const char *szEmitterName, bool bShow);
   void Animation(const char *szSequenceName, const char *szAnimationName, bool bStart);
   void PlaySound(const char *szSequenceName, const char *szSoundName, bool bLoop=false);
   void PickupItem(const char *szSequenceName, const char *szItemName, float fFromX, float fFromY, int nNbPartNeeded);
   void NarrationMode(const char *szSequenceName, SceneSequenceNarrationMode mode, bool b);
   void CloseVignette(const char *szSequenceName);
   void GoToScene(const char *szSequenceName, const char *szSceneName, const char *szAdditionalName = "", bool bVignette = false);
   void Wait(const char *szSequenceName, int nDuration);
   void DialogBoxOK(const char *szSequenceName, const char *szText);
   void MiniGame(const char *szSequenceName, const char *szName, EMiniJeuBase *lpMinigame);
   void Energy(const char *szSequenceName, float fCenterX, float fCenterY, float fXRadius, float fYRadius, float fDestX, float fDestY, int nNumber);
   void Tutobox(const char *szSequenceName, const char *szMessage, int nX, int nY, int nAngle, int nRadius);
   void Objective(const char *szSequenceName, const char *szUniverse, const char *szObjective);
   void PreloadVideo(const char *szVideoName);
   void GotoVideo(const char *szSequenceName, const char *szVideoName, const char *szMusicName);
   void Callback(const char *szSequenceName, const char *szParam);
   // Informe le séquenceur qu'on a terminé de préparer un lot de séquence
   void EndSequenceBatch(); //TODO: A appeler dans le Check courant, pour indiquer que le jeu a repris la main entre temps
   SceneSequenceType getSequenceType()
   {
      if (_lpCurrentSequence) {
         return _lpCurrentSequence->_type;
      }
      else {
         return SCENESEQUENCE_UNKNOW;
      }

   }
   
   /* Reset */
   void Reset();
#ifdef DEBUG
   void DeleteCurrentSequence() {
      XDELETE(_lpCurrentSequence);
   }
   ESequenceContainer* GetCurrentSequence() {
      return _lpCurrentSequence;
   }
#endif
private:
   ESceneDirector *_lpSceneDirector;
   ESequenceContainer *_lpCurrentSequence;

   /* Méthodes de cuisine interne */
   ESequenceContainer *getNextSequence();
};

#endif