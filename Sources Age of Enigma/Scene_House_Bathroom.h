/*
 *  Scene_House_Room1.h
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEBATHROOM_H
#define  _ENIGMA_SCENEBATHROOM_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class SceneBathroom : public EScene {
public:
   SceneBathroom(ESceneDirector *lpSceneDirector);
   ~SceneBathroom();
   
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

   char *getDebugString() {
      return "F6:Resolve";
   }
   
   bool IsNeedIHM()
   {
      return true;
   }
private:
   KGraphic *_lpBgGraphic;
   KCounter *_lpTempo;
   KSound *_lpGhostVoice;
   
   void CheckBlokoban();
   
protected:
   
};

#endif