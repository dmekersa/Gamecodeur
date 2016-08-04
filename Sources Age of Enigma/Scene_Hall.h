/*
 *  SceneHall.h
 *  enigma
 *
 *  Created by Rockford on 28/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEHALL_H
#define  _ENIGMA_SCENEHALL_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"
#include "ParticleFactory.h"

class SceneHall : public EScene {
public:
   SceneHall(ESceneDirector *lpSceneDirector);
   ~SceneHall();

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
   
   bool IsNeedIHM()
   {
      return _bNeedIHM;
   }
//   char *getDebugString() {
//      return "F6:Skip Nathan / F7:Resolve ground floor";
//   }
private:
	KGraphic *_lpBgGraphic;
   KSound *_lpSndSwitch;
   KSound *_lpSndSwitchReward;
   ParticleEmitter *_Emitter1;  
   ParticleEmitter *_Emitter1A;  
   ParticleEmitter *_Emitter1B;  
   ParticleEmitter *_Emitter2; 
   ParticleEmitter *_Emitter3; 
   ParticleFactory *_MyFactory;
   KCounter *_lpProgress;
   bool _bForceExit;
   bool _bNeedIHM;
   
   bool _bEscape;
   void Escape();
   
protected:
   
};
#endif