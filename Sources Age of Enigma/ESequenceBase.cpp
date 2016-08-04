/*
 *  ESequenceBase.cpp
 *  enigma
 *
 *  Created by Rockford on 05/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "ESequenceBase.h"
#include "ESequenceVoiceOver.h"
#include "ESequenceTalk.h"
#include "ESequenceEarthQuake.h"
#include "ESequenceMisc.h"
#include "global.h"

ESequenceContainer::ESequenceContainer(const std::string strSceneName, const std::string strSequenceName, SceneSequenceType type)
{
   _strSceneName = strSceneName;
   _type = type;
   memset(_szSequenceName,0, 256);
   strcpy(_szSequenceName, strSequenceName.c_str());
   
   /* Ne contient rien au départ, penser à mettre à nul toutes les branches possibles */
   _lpSequenceVoiceOver = NULL;
   _lpSequenceTalk = NULL;
   _lpSequenceEarthQuake = NULL;
   _lpSequenceShowImage = NULL;
   _lpSequenceSwitchImage = NULL;
   _lpSequenceShowEmitter = NULL;
   _lpSequencePlaySound = NULL;
   _lpSequenceAnimation = NULL;
   _lpSequencePickupItem = NULL;
   _lpSequenceNarrationMode = NULL;
   _lpSequenceCloseVignette = NULL;
   _lpSequenceGotoScene = NULL;
   _lpSequenceWait = NULL;
   _lpSequenceDialogBoxOK = NULL;
   _lpSequenceMinigame = NULL;
   _lpSequenceEnergy = NULL;
   _lpSequenceTutobox = NULL;
   _lpSequenceObjective = NULL;
   _lpSequenceGotoVideo = NULL;
   _lpSequenceCallback = NULL;
}

// Destructeur de chaque séquence
void ESequenceContainer::CleanUp(const std::string strSceneName)
{
   // Parcours la liste pour en récupérer les valeurs et la vide
   ESequenceContainer *o;
   ESequenceContainer *otodelete;
   bool bGotNext = g_ListSequence.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (o->_strSceneName == strSceneName || strSceneName == "") {
            g_ListSequence.remove(o);
            otodelete = o;
         }
         else {
            otodelete = NULL;
         }

         bGotNext = g_ListSequence.getNext (o);
         if (otodelete) {
            otodelete->DeleteContent();
            XDELETE(otodelete);
         }
      }
   }
}

// Destructeur de chaque séquence
void ESequenceContainer::CleanUpAll()
{
   // Parcours la liste pour en récupérer les valeurs et la vide
   ESequenceContainer *o;
   ESequenceContainer *otodelete;
   bool bGotNext = g_ListSequence.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         g_ListSequence.remove(o);
         otodelete = o;
         bGotNext = g_ListSequence.getNext (o);
         otodelete->DeleteContent();
         XDELETE(otodelete);
      }
   }
}

void ESequenceContainer::Purge(const char *szName)
{
   K_LOG("ESequenceContainer::PurgePickup(%s)",szName);
   // Parcours la liste pour en récupérer les valeurs et la vide
   ESequenceContainer *o;
   bool bGotNext = g_ListSequence.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
//         // Traite l'objet qui devait être obtenu (pour éviter les pertes en cas de quit pendant une cinématique)
//         if (o->_type == SCENESEQUENCE_PICKUPITEM) {
//            K_LOG("ESequenceContainer::PurgePickup -> Purge a SCENESEQUENCE_PICKUPITEM");
//            o->getContent()->Start();
//            SCENEDIRECTOR->EndTravellingItem();
//         }
         if (o->_type == SCENESEQUENCE_OBJECTIVE) {
            o->getContent()->Start();
         }
         
         bGotNext = g_ListSequence.getNext (o);
      }
   }
}

ESequenceBase *ESequenceContainer::getContent() {
   if (_lpSequenceVoiceOver) {
      return (ESequenceBase *)_lpSequenceVoiceOver;
   }
   else if (_lpSequenceTalk) {
      return (ESequenceBase *)_lpSequenceTalk;
   }
   else if (_lpSequenceEarthQuake) {
      return (ESequenceBase *)_lpSequenceEarthQuake;
   }
   else if (_lpSequenceEarthQuake) {
      return (ESequenceBase *)_lpSequenceEarthQuake;
   }
   else if (_lpSequenceShowImage) {
      return (ESequenceBase *)_lpSequenceShowImage;
   }
   else if (_lpSequenceSwitchImage) {
      return (ESequenceBase *)_lpSequenceSwitchImage;
   }
   else if (_lpSequenceShowEmitter) {
      return (ESequenceBase *)_lpSequenceShowEmitter;
   }
   else if (_lpSequencePlaySound) {
      return (ESequenceBase *)_lpSequencePlaySound;
   }
   else if (_lpSequenceAnimation) {
      return (ESequenceBase *)_lpSequenceAnimation;
   }
   else if (_lpSequencePickupItem) {
      return (ESequenceBase *)_lpSequencePickupItem;
   }
   else if (_lpSequenceNarrationMode) {
      return (ESequenceBase *)_lpSequenceNarrationMode;
   }
   else if (_lpSequenceCloseVignette) {
      return (ESequenceBase *)_lpSequenceCloseVignette;
   }
   else if (_lpSequenceGotoScene) {
      return (ESequenceBase *)_lpSequenceGotoScene;
   }
   else if (_lpSequenceWait) {
      return (ESequenceBase *)_lpSequenceWait;
   }
   else if (_lpSequenceDialogBoxOK) {
      return (ESequenceBase *)_lpSequenceDialogBoxOK;
   }
   else if (_lpSequenceMinigame) {
      return (ESequenceBase *)_lpSequenceMinigame;
   }
   else if (_lpSequenceEnergy) {
      return (ESequenceBase *)_lpSequenceEnergy;
   }
   else if (_lpSequenceTutobox) {
      return (ESequenceBase *)_lpSequenceTutobox;
   }
   else if (_lpSequenceObjective) {
      return (ESequenceBase *)_lpSequenceObjective;
   }
   else if (_lpSequenceGotoVideo) {
      return (ESequenceBase *)_lpSequenceGotoVideo;
   }
   else if (_lpSequenceCallback) {
      return (ESequenceBase *)_lpSequenceCallback;
   }
   return NULL;
}

void ESequenceContainer::DeleteContent()
{
   XDELETE(_lpSequenceTalk);
   XDELETE(_lpSequenceVoiceOver);
   XDELETE(_lpSequenceEarthQuake);
   XDELETE(_lpSequenceShowImage);
   XDELETE(_lpSequenceSwitchImage);
   XDELETE(_lpSequenceShowEmitter);
   XDELETE(_lpSequencePlaySound);
   XDELETE(_lpSequencePickupItem);
   XDELETE(_lpSequenceNarrationMode);
   XDELETE(_lpSequenceCloseVignette);
   XDELETE(_lpSequenceGotoScene);
   XDELETE(_lpSequenceWait);
   XDELETE(_lpSequenceDialogBoxOK);
   XDELETE(_lpSequenceMinigame);
   XDELETE(_lpSequenceEnergy);
   XDELETE(_lpSequenceTutobox);
   XDELETE(_lpSequenceObjective);
   XDELETE(_lpSequenceGotoVideo);
   XDELETE(_lpSequenceCallback);
   XDELETE(_lpSequenceAnimation);
}
