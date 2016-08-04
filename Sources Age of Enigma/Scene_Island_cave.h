/*
 *  Scene_Island_cave.h
 *  enigma
 *
 *  Created by Rockford on 01/06/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */


#ifndef  _ENIGMA_Scene_Island_Cave_H
#define  _ENIGMA_Scene_Island_Cave_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Island_Cave : public EScene {
public:
   Scene_Island_Cave(ESceneDirector *lpSceneDirector);
   ~Scene_Island_Cave();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   void MiniGameDone(const char *szGameName, bool bIsRevolved);
   
   bool IsNeedIHM()
   {
      return true;
   }
private:
   KGraphic *_lpBgGraphic;
   
protected:
   
};

#endif