/*
 *  Scene_Debug_David.h
 *  enigma
 *
 *  Created by Rockford on 14/06/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_DEBUGDAVID_H
#define  _ENIGMA_DEBUGDAVID_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Debug_David : public EScene {
public:
   Scene_Debug_David(ESceneDirector *lpSceneDirector);
   ~Scene_Debug_David();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   void Callback(const char *szParam);
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   
   bool IsNeedIHM()
   {
      return true;
   }
private:
   KGraphic *_lpBgGraphic;
   KCounter *_lpCounter;
   
protected:
   
};

#endif