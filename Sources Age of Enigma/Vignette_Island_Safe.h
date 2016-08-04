/*
 *  Vignette_Island_Safe.h
 *  enigma
 *
 *  Created by Rockford on 18/08/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */


#ifndef  _ENIGMA_VIGNETTE_ISLANDSAFE_H
#define  _ENIGMA_VIGNETTE_ISLANDSAFE_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Vignette_Island_Safe : public EScene {
public:
   Vignette_Island_Safe(ESceneDirector *lpSceneDirector);
   ~Vignette_Island_Safe();
   
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
   
protected:
   
};

#endif