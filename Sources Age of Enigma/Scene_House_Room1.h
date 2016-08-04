/*
 *  Scene_House_Room1.h
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEROOM1_H
#define  _ENIGMA_SCENEROOM1_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class SceneRoom1 : public EScene {
public:
   SceneRoom1(ESceneDirector *lpSceneDirector);
   ~SceneRoom1();
   
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
      return true;
   }
private:
   KGraphic *_lpBgGraphic;
   
protected:
   
};

#endif