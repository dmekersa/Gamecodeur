/*
 *  Scene_House_HallDoor.h
 *  enigma
 *
 *  Created by Rockford on 23/06/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEHALLDOOR_H
#define  _ENIGMA_SCENEHALLDOOR_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class SceneHallDoor : public EScene {
public:
   SceneHallDoor(ESceneDirector *lpSceneDirector);
   ~SceneHallDoor();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   
   bool IsNeedIHM()
   {
      return false;
   }
private:
   KGraphic *_lpBgGraphic;
   bool _bInitDone;
   
protected:
   
};

#endif