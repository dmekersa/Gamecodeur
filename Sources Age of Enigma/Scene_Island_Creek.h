/*
 *  Scene_Island_Creek.h
 *  enigma
 *
 *  Created by Rockford on 21/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_Scene_Island_CREEK_H
#define  _ENIGMA_Scene_Island_CREEK_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Island_Creek : public EScene {
public:
   Scene_Island_Creek(ESceneDirector *lpSceneDirector);
   ~Scene_Island_Creek();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   bool ItemUsed(const char *szItemName, const char *szObjectName);
   
   bool IsNeedIHM()
   {
      return true;
   }
private:
   KGraphic *_lpBgGraphic;
   
protected:
   
};

#endif