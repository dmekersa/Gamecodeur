/*
 *  Scene_House_Keyhole.h
 *  enigma
 *
 *  Created by Rockford on 19/08/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */


#ifndef  _ENIGMA_SCENEKEYHOLE_H
#define  _ENIGMA_SCENEKEYHOLE_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_House_Keyhole : public EScene {
public:
   Scene_House_Keyhole(ESceneDirector *lpSceneDirector);
   ~Scene_House_Keyhole();
   
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
   KCounter *_lpCounterEye;
   bool  _bPsycho;
   KSound *_lpGhostVoice;
   
protected:
   
};

#endif