/*
 *  Scene_Title_Splashscreen.h
 *  enigma
 *
 *  Created by Rockford on 21/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */


#ifndef  _ENIGMA_SCENESPLASH_H
#define  _ENIGMA_SCENESPLASH_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Title_Splashscreen : public EScene {
public:
   Scene_Title_Splashscreen(ESceneDirector *lpSceneDirector);
   ~Scene_Title_Splashscreen();
   
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
private:
   KGraphic *_lpBgGraphic;
   KSound *_lpSound;
   
   //bool _bPreloadDone;
protected:
   
};

#endif