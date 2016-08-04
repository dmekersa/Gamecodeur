/*
 *  Scene_Japan_Garden.h
 *  enigma
 *
 *  Created by Rockford on 31/08/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEJAPANGARDEN_H
#define  _ENIGMA_SCENEJAPANGARDEN_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Japan_Garden : public EScene {
public:
   Scene_Japan_Garden(ESceneDirector *lpSceneDirector);
   ~Scene_Japan_Garden();
   
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
   
protected:
   
};

#endif