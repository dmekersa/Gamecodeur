/*
 *  Scene_Island_Beach.h
 *  enigma
 *
 *  Created by Rockford on 30/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEISCLANDBEACH_H
#define  _ENIGMA_SCENEISCLANDBEACH_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Island_Beach : public EScene {
public:
   Scene_Island_Beach(ESceneDirector *lpSceneDirector);
   ~Scene_Island_Beach();
   
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
   KCounter *_lpCounterWave;
   KCounter *_lpCounterCrab;   
   
protected:
   void ReturnToHouse();
   
};

#endif