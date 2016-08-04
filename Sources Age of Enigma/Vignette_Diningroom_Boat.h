/*
 *  Vignette_Diningroom_Boat.h
 *  enigma
 *
 *  Created by Rockford on 13/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_VIGNETTE_DININGBOAT_H
#define  _ENIGMA_VIGNETTE_DININGBOAT_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Vignette_Diningroom_Boat : public EScene {
public:
   Vignette_Diningroom_Boat(ESceneDirector *lpSceneDirector);
   ~Vignette_Diningroom_Boat();
   
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