/*
 *  SceneHall.h
 *  enigma
 *
 *  Created by Rockford on 28/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEHOUSEHALL_H
#define  _ENIGMA_SCENEHOUSEHALL_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"
#include "ParticleFactory.h"

class Scene_House_Hall : public EScene {
public:
   Scene_House_Hall(ESceneDirector *lpSceneDirector);
   ~Scene_House_Hall();

   void Init ();
   void Close ();
   bool AskForClose ();
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   bool ItemUsed(const char *szItemName, const char *szObjectName);
   void MiniGameDone(const char *szGameName, bool bIsRevolved);
   void Callback(const char *szParam);
   
   bool IsNeedIHM()
   {
      return true;
   }
//   char *getDebugString() {
//      return "F6:Skip Nathan / F7:Resolve ground floor";
//   }
private:
	KGraphic *_lpBgGraphic;
   
   void ResolveGroundFloor();
   
protected:
   
};
#endif