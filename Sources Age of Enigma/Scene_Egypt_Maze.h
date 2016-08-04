/*
 *  Scene_Egypt_Maze.h
 *  enigma
 *
 *  Created by Rockford on 05/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEEGYPTMAZE_H
#define  _ENIGMA_SCENEEGYPTMAZE_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Egypt_Maze: public EScene {
public:
   Scene_Egypt_Maze(ESceneDirector *lpSceneDirector);
   ~Scene_Egypt_Maze();
   
   void Init ();
   void Close ();
   
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
      return true;
   }
private:
   KGraphic *_lpBgGraphic;
   KTextFace *_lpFont;
protected:
   int   _nRoomNumber;
   
};

#endif