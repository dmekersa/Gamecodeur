/*
 *  Scene_Mode.h
 *  enigma
 *
 *  Created by Rockford on 22/03/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEMODE_H
#define  _ENIGMA_SCENEMODE_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Mode : public EScene {
public:
   Scene_Mode(ESceneDirector *lpSceneDirector);
   ~Scene_Mode();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   
   bool IsNeedIHM()
   {
      return false;
   }
   
   void Load();
private:
   KGraphic *_lpBgGraphic;
   KTextFace *_lpFont1;
   KSound *_lpSound1;
   
protected:
};

#endif