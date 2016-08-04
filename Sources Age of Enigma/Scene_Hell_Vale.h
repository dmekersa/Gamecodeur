/*
 *  Scene_Hell_Vale.h
 *  enigma
 *
 *  Created by Rockford on 05/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEHELLVALE_H
#define  _ENIGMA_SCENEHELLVALE_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Hell_Vale : public EScene {
public:
   Scene_Hell_Vale(ESceneDirector *lpSceneDirector);
   ~Scene_Hell_Vale();
   
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
   void SoulRequest(int nSoulNumber);
   void SoulCheck(int nSoulNumber);
   
protected:
   
};

#endif