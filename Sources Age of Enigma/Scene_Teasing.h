/*
 *  Scene_Teasing.h
 *  enigma
 *
 *  Created by Rockford on 14/02/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENETEASING_H
#define  _ENIGMA_SCENETEASING_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Teasing : public EScene {
public:
   Scene_Teasing(ESceneDirector *lpSceneDirector);
   ~Scene_Teasing();
   
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
   
protected:
   
};

#endif