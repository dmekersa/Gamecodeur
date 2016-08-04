/*
 *  SceneLivingRoom.h
 *  enigma
 *
 *  Created by Rockford on 29/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENELIVING_H
#define  _ENIGMA_SCENELIVING_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class SceneLivingRoom : public EScene {
public:
   SceneLivingRoom(ESceneDirector *lpSceneDirector);
   ~SceneLivingRoom();
   
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
   KGraphic *_lpBgGraphic;
   KSound *_lpGhostVoice;
   void AskEffect();
   
protected:
   
};

#endif