/*
 *  SceneMenu.h
 *  enigma
 *
 *  Created by Rockford on 20/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEMENU_H
#define  _ENIGMA_SCENEMENU_H

#include "EScene.h"
#include "KPTK.h"
#include "KUIButton.h"
#include "ESmartImage.h"

class SceneMenu : public EScene {
public:
   SceneMenu(ESceneDirector *lpSceneDirector);
   ~SceneMenu();
   void Init ();
   void Close ();
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   bool IsNeedIHM()
   {
      return false;
   }
   char *getDebugString() {
      if (_nCheatCode >= 7) {
         return "**CHEAT** 1=Island 2:Corridor 3:Japan 4:1stFloor 5:M.Age 6:Inca 7:Egypt 8:Hell 9:END 0:Reset";
      }
      else {
         return "Debug Mode";
      }
   }
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);

   int _bReplyRate;

   
private:
	KGraphic *_lpBgGraphic;
   KGraphic *_lpBgGraphic0;
   ESmartImage *_lpSI0;
   ESmartImage *_lpSI1;
   KVideo *_lpVideo;
   
   /* UI */
   KTextFace *_lpFont;
   KGraphic *_lpButtonGraphic;
   KUIElement *_lpUIContainer;
   KUIButton *_lpButtonPlay;
   KUIButton *_lpButtonOptions;
   KUIButton *_lpButtonTestDM;
   KUIButton *_lpButtonTestMJ;
   static void _myUIMessageHandler (KUIMessage *lpUIMessage);
   
protected:
   int _nCheatCode;
   int _nCheatCode2;
};
#endif