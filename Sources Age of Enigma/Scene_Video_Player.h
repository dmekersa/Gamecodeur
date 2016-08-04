/*
 *  Scene_Video_Player.h
 *  enigma
 *
 *  Created by Rockford on 03/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEVIDEO_H
#define  _ENIGMA_SCENEVIDEO_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"
#include "KVideo.h"

class Scene_Video_Player : public EScene {
public:
   Scene_Video_Player(ESceneDirector *lpSceneDirector);
   ~Scene_Video_Player();
   
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
   KVideo *_lpVideo;
   KSound *_lpSpeech;
   void EndScene();
   
   bool _bSubtitleDone[11];
   KTextFace *_lpFont;
protected:
   bool _bEnd;
};

#endif