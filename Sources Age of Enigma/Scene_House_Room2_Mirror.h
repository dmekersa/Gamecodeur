/*
 *  Scene_House_Room2_Mirror.h
 *  enigma
 *
 *  Created by Rockford on 22/10/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEROOM2MIRROR_H
#define  _ENIGMA_SCENEROOM2MIRROR_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_House_Room2_Mirror : public EScene {
public:
   Scene_House_Room2_Mirror(ESceneDirector *lpSceneDirector);
   ~Scene_House_Room2_Mirror();
   
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
   
   void CheckMirrorFull();
   
   bool IsNeedIHM()
   {
      return true;
   }
private:
   KGraphic *_lpBgGraphic;
   
protected:
   
};

#endif