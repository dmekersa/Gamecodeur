/*
 *  Scene_House_Closet.h
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENECLOSET_H
#define  _ENIGMA_SCENECLOSET_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class SceneCloset : public EScene {
public:
   SceneCloset(ESceneDirector *lpSceneDirector);
   ~SceneCloset();
   
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