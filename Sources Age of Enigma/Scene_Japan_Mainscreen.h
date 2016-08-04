/*
 *  Scene_Japan_Mainscreen.h
 *  enigma
 *
 *  Created by Rockford on 30/08/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEJAPANMAIN_H
#define  _ENIGMA_SCENEJAPANMAIN_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Japan_Mainscreen : public EScene {
public:
   Scene_Japan_Mainscreen(ESceneDirector *lpSceneDirector);
   ~Scene_Japan_Mainscreen();
   
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
   void ReturnToHouse();
   
protected:
   
};

#endif