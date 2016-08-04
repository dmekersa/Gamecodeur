/*
 *  vignette_hands.h
 *  enigma
 *
 *  Created by Rockford on 19/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_VIGNETTE_HANDS_H
#define  _ENIGMA_VIGNETTE_HANDS_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Vignette_Hands : public EScene {
public:
   Vignette_Hands(ESceneDirector *lpSceneDirector);
   ~Vignette_Hands();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
   //   bool ObjectOver(char *szObjectName, float x, float y);
   //   bool ItemUsed(const char *szItemName, const char *szObjectName);
   //   void MiniGameDone(const char *szGameName, bool bIsRevolved);
   
   bool IsNeedIHM()
   {
      return false;
   }
private:
   char _szHandName[256];
   bool  _bGotoDone;
   bool _bSound;
   
protected:
   
};

#endif