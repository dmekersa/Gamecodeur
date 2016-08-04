/*
 *  ESceneSequencer.cpp
 *  enigma
 *
 *  Created by Rockford on 05/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "ESceneSequencer.h"

KList<ESequenceContainer> ESequenceContainer::g_ListSequence;

ESceneSequencer::ESceneSequencer( ESceneDirector* lpSceneDirector )
{
   /* On conserve une ref vers le director pour plus de facilité */
   _lpSceneDirector = lpSceneDirector;
   
   /* aucune séquence pour l'instant */
   _lpCurrentSequence = NULL;
}

void ESceneSequencer::Logic()
{
   /* Si il n'y a aucune séquence en court, on prend la suivante dans la liste */
   if (_lpCurrentSequence == NULL && ESequenceContainer::g_ListSequence.getCount()>0)
   {
      // Recherche la prochaine séquence non active
      _lpCurrentSequence = getNextSequence();
      // Aucune séquence à venir, on a finit...
      if (_lpCurrentSequence == NULL) {
         return;
      } else {
         _lpCurrentSequence->getContent()->Start();
      }
   }
   /* Si la séquence courante est terminée, on la supprime de la liste et on prend la suivante dans la liste */
   if (_lpCurrentSequence) {
      bool bCompleted = false;

      bCompleted = _lpCurrentSequence->getContent()->isCompleted();
      if (bCompleted) {
         // Supprime la séquence de la liste
         ESequenceContainer::g_ListSequence.remove(_lpCurrentSequence);
         _lpCurrentSequence->DeleteContent();
         XDELETE(_lpCurrentSequence);
         
         // Recherche la prochaine séquence non active
         _lpCurrentSequence = getNextSequence();
         // Aucune séquence à venir, on a finit...
         if (_lpCurrentSequence == NULL) {
            return;
         } else {
            _lpCurrentSequence->getContent()->Start();
         }
      }
   }
   
   if (_lpCurrentSequence) {
      if (_lpCurrentSequence->getContent()->isActive()) {
         _lpCurrentSequence->getContent()->Logic();
      }
   }
}

void ESceneSequencer::Check()
{
   if (_lpCurrentSequence) {
      if (_lpCurrentSequence->getContent()->isActive()) {
         _lpCurrentSequence->getContent()->Check();
      }
   }
}

void ESceneSequencer::Draw()
{
   if (_lpCurrentSequence) {
      if (_lpCurrentSequence->getContent()->isActive()) {
         _lpCurrentSequence->getContent()->Draw();
      }
   }
}

bool ESceneSequencer::isCheckAllowed()
{
   if (_lpCurrentSequence) {
      return _lpCurrentSequence->getContent()->isCheckAllowed();
   }
   return true;
}

/*
 Ajoute une séquence de Voice Over
 szText : Texte à afficher
 szVoice : désigne le fichier son à jouer en //
*/
void ESceneSequencer::VoiceOver(const char *szSequenceName, const char *szText, const char *szVoice, SequenceVoiceOverColor Color)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Vérifie le texte */
   KAssert(szText != NULL);
   if (!szText)
   {
      return;
   }
   
   /* Termine la séquence en cours si c'est un Talk ou un Voiceover */
   if (_lpCurrentSequence) {
      if (_lpCurrentSequence->_type == SCENESEQUENCE_VOICEOVER || _lpCurrentSequence->_type == SCENESEQUENCE_TALK) {
         ESequenceContainer::g_ListSequence.remove(_lpCurrentSequence);
         _lpCurrentSequence->DeleteContent();
         XDELETE(_lpCurrentSequence);
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_VOICEOVER);
   /* Une séquence VoiceOver */
   ESequenceVoiceOver *o = new ESequenceVoiceOver(szText, szVoice, Color);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceVoiceOver = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

/*
 Ajoute une séquence de Talk
 szText : Texte à afficher
 szVoice : désigne le fichier son à jouer en //
 */
void ESceneSequencer::Talk(const char *szSequenceName, int nFromX, int nFromY, const char *szText, const char *szVoice, bool bCheckAllowed, bool bDialogMode, KSound *lpSoundStream)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }

   /* Vérifie le texte */
   KAssert(szText != NULL);
   if (!szText)
   {
      return;
   }

   /* Termine la séquence en cours si c'est un Talk ou un Voiceover */
   if (_lpCurrentSequence) {
      if (_lpCurrentSequence->_type == SCENESEQUENCE_VOICEOVER || _lpCurrentSequence->_type == SCENESEQUENCE_TALK) {
         ESequenceContainer::g_ListSequence.remove(_lpCurrentSequence);
         _lpCurrentSequence->DeleteContent();
         XDELETE(_lpCurrentSequence);
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_TALK);
   /* Une séquence Talk */
   ESequenceTalk *o = new ESequenceTalk(nFromX, nFromY, szText, szVoice, bCheckAllowed, bDialogMode, lpSoundStream);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }

   oc->_lpSequenceTalk = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::EarthQuake(const char *szSequenceName, bool b, const char *szSoundName)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_EARTHQUAKE);
   /* Une séquence EarthQuake */
   ESequenceEarthQuake *o = new ESequenceEarthQuake(b, szSoundName, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceEarthQuake = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::ShowImage(const char *szSequenceName, const char *szImageName, bool bShow, bool bModal)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_IMAGESHOW);
   /* Une séquence */
   ESequenceShowImage *o = new ESequenceShowImage(szImageName, bShow, bModal, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceShowImage = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::SwitchImage(const char *szSequenceName, const char *szImageName1, const char *szImageName2)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_IMAGESWITCH);
   /* Une séquence */
   ESequenceSwitchImage *o = new ESequenceSwitchImage(szImageName1, szImageName2, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceSwitchImage = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::PlaySound(const char *szSequenceName, const char *szSoundName, bool bLoop)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_PLAYSOUND);
   /* Une séquence */
   ESequencePlaySound *o = new ESequencePlaySound(szSoundName, _lpSceneDirector, bLoop);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequencePlaySound = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::Wait(const char *szSequenceName, int nDuration)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_WAIT);
   /* Une séquence */
   ESequenceWait *o = new ESequenceWait(nDuration, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceWait = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::DialogBoxOK(const char *szSequenceName, const char *szText)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_WAIT);
   /* Une séquence */
   ESequenceDialogBoxOK *o = new ESequenceDialogBoxOK(szText, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceDialogBoxOK = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::ShowEmitter(const char *szSequenceName, const char *szEmitterName, bool bShow)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_EMITTERSHOW);
   /* Une séquence */
   ESequenceShowEmitter *o = new ESequenceShowEmitter(szEmitterName, bShow, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceShowEmitter = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::Animation(const char *szSequenceName, const char *szAnimationName, bool bStart)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_ANIMATION);
   /* Une séquence */
   ESequenceAnimation *o = new ESequenceAnimation(szAnimationName, bStart, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceAnimation = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::PickupItem(const char *szSequenceName, const char *szItemName, float fFromX, float fFromY, int nNbPartNeeded)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_PICKUPITEM);
   /* Une séquence */
   ESequencePickupItem *o = new ESequencePickupItem(szItemName, fFromX, fFromY, nNbPartNeeded, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequencePickupItem = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::NarrationMode(const char *szSequenceName, SceneSequenceNarrationMode mode, bool b)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_DIALOGMODE);
   /* Une séquence */
   ESequenceNarrationMode *o = new ESequenceNarrationMode(mode, b, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceNarrationMode = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::CloseVignette(const char *szSequenceName)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_CLOSEVIGNETTE);
   /* Une séquence */
   ESequenceCloseVignette *o = new ESequenceCloseVignette(_lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceCloseVignette = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::GoToScene(const char *szSequenceName, const char *szSceneName, const char *szAdditionalName, bool bVignette)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_GOTOSCENE);
   /* Une séquence */
   ESequenceGotoScene *o = new ESequenceGotoScene(szSceneName, szAdditionalName, bVignette, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceGotoScene = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::MiniGame(const char *szSequenceName, const char *szName, EMiniJeuBase *lpMinigame)
{
   KAssert(szName != NULL);
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         XDELETE(lpMinigame);
         return;
      }
   } else if (isAlready(szName)) {
      XDELETE(lpMinigame);
      return;
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_MINIGAME);
   /* Une séquence */
   ESequenceMinigame *o = new ESequenceMinigame(lpMinigame, szName, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, szName);
   }
   oc->_lpSequenceMinigame = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::Energy(const char *szSequenceName, float fCenterX, float fCenterY, float fXRadius, float fYRadius, float fDestX, float fDestY, int nNumber)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_ENERGY);
   /* Une séquence */
   ESequenceEnergy *o = new ESequenceEnergy(fCenterX, fCenterY, fXRadius, fYRadius, fDestX, fDestY, nNumber, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceEnergy = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::Tutobox(const char *szSequenceName, const char *szMessage, int nX, int nY, int nAngle, int nRadius)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_TUTOBOX);
   /* Une séquence */
   ESequenceTutobox *o = new ESequenceTutobox(szMessage, nX, nY, nAngle, nRadius, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceTutobox = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::Objective(const char *szSequenceName, const char *szUniverse, const char *szObjective)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_OBJECTIVE);
   /* Une séquence */
   ESequenceObjective *o = new ESequenceObjective(szUniverse, szObjective, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceObjective = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::PreloadVideo(const char *szVideoName) {
   ESequenceGotoVideo::PreloadVideo(szVideoName);
}

void ESceneSequencer::GotoVideo(const char *szSequenceName, const char *szVideoName, const char *szMusicName)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_GOTOVIDEO);
   /* Une séquence */
   ESequenceGotoVideo *o = new ESequenceGotoVideo(szVideoName, szMusicName, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceGotoVideo = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

void ESceneSequencer::Callback(const char *szSequenceName, const char *szParam)
{
   /* Vérifie que cette même séquence n'est pas déjà dans la pile */
   if (szSequenceName) {
      if (isAlready(szSequenceName)) {
         return;
      }
   }
   
   /* Un container */
   ESequenceContainer *oc = new ESequenceContainer(_lpSceneDirector->getCurrentSceneName(), "", SCENESEQUENCE_CALLBACK);
   /* Une séquence */
   ESequenceCallback *o = new ESequenceCallback(szParam, _lpSceneDirector);
   /* On relie les 2 */
   if (szSequenceName) {
      strcpy(oc->_szSequenceName, szSequenceName);
   }
   else {
      strcpy(oc->_szSequenceName, "");
   }
   oc->_lpSequenceCallback = o;
   /* On ajoute au séquenceur */
   ESequenceContainer::g_ListSequence.addToTail(oc);
}

bool ESceneSequencer::isEmpty()
{
   return ESequenceContainer::g_ListSequence.getCount() == 0;
}

/**************************************************************************************************************************************************************/

/* Retourne la prochaine séquence non active */
ESequenceContainer *ESceneSequencer::isAlready(const char* szSequenceName)
{
   ESequenceContainer *o;
   bool bGotNext = ESequenceContainer::g_ListSequence.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         // Traitement associé à la séquence
         if (strcmp(o->_szSequenceName, szSequenceName) == 0) {
            K_LOG("ESceneSequencer::isAlready return true for %s",szSequenceName);
            return o;
         }
         // Suivant
         bGotNext = ESequenceContainer::g_ListSequence.getNext (o);
      }
   } 
   return NULL;
}
/* Retourne la prochaine séquence non active */
ESequenceContainer *ESceneSequencer::getNextSequence()
{
   ESequenceContainer *o;
   bool bGotNext = ESequenceContainer::g_ListSequence.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         // Traitement associé à la séquence
         if (!o->getContent()->isActive()) {
            K_LOG("ESceneSequencer::getNextSequence() return type %d",o->_type);
            return o;
         }
         // Suivant
         bGotNext = ESequenceContainer::g_ListSequence.getNext (o);
      }
   } 
   return NULL;
}

void ESceneSequencer::Reset()
{
   // D'abord, termine les séquences importantes
   if (_lpSceneDirector->GetCurrentScene()) {
      ESequenceContainer::Purge(_lpSceneDirector->GetCurrentScene()->GetName().c_str());
   }
   
   _lpCurrentSequence = NULL;
   ESequenceContainer::CleanUp("");
}