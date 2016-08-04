/*
 *  SceneStairsDown.h
 *  enigma
 *
 *  Created by Rockford on 29/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */


#ifndef  _ENIGMA_SCENESTAIRSDOWN_H
#define  _ENIGMA_SCENESTAIRSDOWN_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class SceneStairsDown : public EScene {
public:
   SceneStairsDown(ESceneDirector *lpSceneDirector);
   ~SceneStairsDown();
   
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