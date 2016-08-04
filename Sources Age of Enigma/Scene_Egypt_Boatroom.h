/*
 *  Scene_Egypt_Boatroom.h
 *  enigma
 *
 *  Created by Rockford on 21/12/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEEGYPTBOATROOM_H
#define  _ENIGMA_SCENEEGYPTBOATROOM_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Egypt_Boatroom: public EScene {
public:
   Scene_Egypt_Boatroom(ESceneDirector *lpSceneDirector);
   ~Scene_Egypt_Boatroom();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   void Callback(const char *szParam);
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
   KCounter *_lpAnimDoor;
   bool _bAnimDoor;
   void StartEffect();
   
protected:
};

#endif