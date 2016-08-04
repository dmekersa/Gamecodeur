/*
 *  Scene_House_Out.h
 *  enigma
 *
 *  Created by Rockford on 22/03/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEHOUSEOUT_H
#define  _ENIGMA_SCENEHOUSEOUT_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_House_Out: public EScene {
public:
   Scene_House_Out(ESceneDirector *lpSceneDirector);
   ~Scene_House_Out();
   
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
   void Callback(const char *szParam);
   
   bool IsNeedIHM()
   {
      return true;
   }
private:
   KGraphic *_lpBgGraphic;
   int   _nYesForTutorial;
   bool  _bTutorial;
   bool  _bTutorialAsked;
   
protected:
   
};

#endif