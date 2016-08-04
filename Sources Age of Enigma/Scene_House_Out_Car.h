/*
 *  Scene_House_Out_Car.h
 *  enigma
 *
 *  Created by Rockford on 23/03/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */


#ifndef  _ENIGMA_SCENEHOUSEOUTCAR_H
#define  _ENIGMA_SCENEHOUSEOUTCAR_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_House_Out_Car: public EScene {
public:
   Scene_House_Out_Car(ESceneDirector *lpSceneDirector);
   ~Scene_House_Out_Car();
   
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
   
protected:
   
};

#endif