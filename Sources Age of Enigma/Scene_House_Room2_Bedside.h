/*
 *  Scene_House_Room2_Bedside.h
 *  enigma
 *
 *  Created by Rockford on 21/10/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */
#ifndef  _ENIGMA_SCENEROOM2BS_H
#define  _ENIGMA_SCENEROOM2BS_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_House_Room2_Bedside : public EScene {
public:
   Scene_House_Room2_Bedside(ESceneDirector *lpSceneDirector);
   ~Scene_House_Room2_Bedside();
   
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