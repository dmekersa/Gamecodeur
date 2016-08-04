/*
 *  Scene_Title.h
 *  enigma
 *
 *  Created by Rockford on 31/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */
#ifndef  _ENIGMA_SCENETITLE_H
#define  _ENIGMA_SCENETITLE_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

bool	LoadDirectoryPictures(char *filename, bool isFolder);

class SceneTitle : public EScene {
public:
   SceneTitle(ESceneDirector *lpSceneDirector);
   ~SceneTitle();
   
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
   KCounter *_lpAnimShowTitle;
   KCounter *_lpAnimShowText;
   KCounter *_lpAnimEnd;
   KSound *_lpSound1;
   KSound *_lpSound2;
   
   void DrawTitle(std::string, std::string, float fBlend);
   int _nTempo, _nTempo2;
   
protected:
   bool _bEnd;
};

#endif