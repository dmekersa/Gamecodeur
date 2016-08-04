/*
 *  Scene_House_Basement.h
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEBASEMENT_H
#define  _ENIGMA_SCENEBASEMENT_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class SceneBasement : public EScene {
public:
   SceneBasement(ESceneDirector *lpSceneDirector);
   ~SceneBasement();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   bool ItemUsed(const char *szItemName, const char *szObjectName);
   void MiniGameDone(const char *szGameName, bool bIsRevolved);
   
   bool IsNeedIHM()
   {
      return true;
   }
private:
   KCounter *_lpTimerGhost;
   KSound *_lpGhostVoice;
   void BasementGoforUniverse();
   
protected:
   
};

#endif