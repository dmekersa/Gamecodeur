/*
 *  Scene_Minigames.h
 *  enigma
 *
 *  Created by Rockford on 20/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEMINIGAMES_H
#define  _ENIGMA_SCENEMINIGAMES_H


#include "EBox.h"

#include "EScene.h"
#include "KPTK.h"
#include "KUIButton.h"
#include "EMiniJeuBase.h"

class SceneMinigames : public EScene {
public:
   SceneMinigames(ESceneDirector *lpSceneDirector);
   ~SceneMinigames();
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
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   void MiniGameDone(const char *szGameName, bool bIsRevolved);
   char *getDebugString() {
      return "For testing purpose. F5 to return to Menu screen";
   }
   
private:
	void LauchGame(int nGameNumber,bool bPreloadOnly=false);

	char _cMenuText[256];

	KGraphic *_lpBgGraphic;

	EFont *_lpFontTitle;
	EFont *_lpFontSubTitle;
	EFont *_lpFontMenu;

	EMiniJeuBase *_lpMiniJeu;

	EBox *_lpTitleBox;
	EBox *_lpSubTitleBox;
	EBox *_lpScreenShotBox;
};
#endif