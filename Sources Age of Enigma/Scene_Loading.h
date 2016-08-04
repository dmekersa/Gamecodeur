/*
 *  Scene_Loading.h
 *  enigma
 *
 *  Created by Rockford on 21/03/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENELOADING_H
#define  _ENIGMA_SCENELOADING_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Loading : public EScene {
public:
   Scene_Loading(ESceneDirector *lpSceneDirector);
   ~Scene_Loading();
   
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
   KTextFace *_lpFont;
   KTextFace *_lpFont1;
   KSound *_lpSound1;
   
   bool _bPreloadDone;
   bool _bChangeScene;
   KTextFace *font;

   int _nMaxLoading;
   KCounter* _lpCounterTimeOut;

protected:
};

#endif