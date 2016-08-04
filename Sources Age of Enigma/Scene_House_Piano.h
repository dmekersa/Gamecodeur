/*
 *  Scene_House_Piano.h
 *  enigma
 *
 *  Created by Rockford on 15/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_HOUSE_PIANO_H
#define  _ENIGMA_HOUSE_PIANO_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_House_Piano : public EScene {
public:
   Scene_House_Piano(ESceneDirector *lpSceneDirector);
   ~Scene_House_Piano();
   
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
   int   _nSequencePuzzle;

protected:
   
};

#endif