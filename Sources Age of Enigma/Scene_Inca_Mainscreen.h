/*
 *  Scene_Inca_Mainscreen.h
 *  enigma
 *
 *  Created by MEKERSA David on 04/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEINCAMAIN_H
#define  _ENIGMA_SCENEINCAMAIN_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Inca_Mainscreen : public EScene {
public:
   Scene_Inca_Mainscreen(ESceneDirector *lpSceneDirector);
   ~Scene_Inca_Mainscreen();
   
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
   KSound *_lpSoundDesign;
   
protected:
   bool  _bAnimDoor;
   KCounter *_lpAnimDoor;
   void StartEffect(const char *szEffect);
   
};

#endif