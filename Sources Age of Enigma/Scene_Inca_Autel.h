/*
 *  Scene_Inca_Autel.h
 *  enigma
 *
 *  Created by MEKERSA David on 17/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */


#ifndef  _ENIGMA_INCA_AUTEL_H
#define  _ENIGMA_INCA_AUTEL_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Inca_Autel : public EScene {
public:
   Scene_Inca_Autel(ESceneDirector *lpSceneDirector);
   ~Scene_Inca_Autel();
   
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
   void TestAutel();
   
protected:
   
};

#endif