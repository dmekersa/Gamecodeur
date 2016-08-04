/*
 *  Scene_House_Statuette.h
 *  enigma
 *
 *  Created by Rockford on 15/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_HOUSE_STATUETTE_H
#define  _ENIGMA_HOUSE_STATUETTE_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_House_Statuette : public EScene {
public:
   Scene_House_Statuette(ESceneDirector *lpSceneDirector);
   ~Scene_House_Statuette();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   bool ObjectClicked(char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   bool ItemUsed(const char *szItemName, const char *szObjectName);
   void MiniGameDone(const char *szGameName, bool bIsRevolved);
   
   bool IsNeedIHM()
   {
      return true;
   }
private:
   bool _bInition;
   bool _bRaising;
   float _fRaising;
   KSound *_lpSoundEarthQuake;
   KCounter *_lpCounterInition;
   void  LaunchEffect();
   void RestoreStatuette();
   void MoveStatuette(float fX, float fY);
   
protected:
   
};

#endif