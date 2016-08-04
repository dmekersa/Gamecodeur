/*
 *  Scene_House_Basement_Grid.h
 *  enigma
 *
 *  Created by Rockford on 24/08/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */


#ifndef  _ENIGMA_SCENEBASEMENTGRID_H
#define  _ENIGMA_SCENEBASEMENTGRID_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_House_Basement_Grid : public EScene {
public:
   Scene_House_Basement_Grid(ESceneDirector *lpSceneDirector);
   ~Scene_House_Basement_Grid();
   
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