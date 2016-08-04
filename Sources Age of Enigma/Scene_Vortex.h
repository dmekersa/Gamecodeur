/*
 *  SceneVortexEgypt.h
 *  enigma
 *
 *  Created by Rockford on 26/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEVORTEX_H
#define  _ENIGMA_SCENEVORTEX_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Vortex : public EScene {
public:
   Scene_Vortex(ESceneDirector *lpSceneDirector);
   ~Scene_Vortex();
   
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
   KTextFace *_lpFont;
   KSound *_lpSndEgypt;
   KCounter *_lpAnimEffect;
   char _szGhost1[256];
   char _szGhost2[256];
   
protected:
   
};

#endif