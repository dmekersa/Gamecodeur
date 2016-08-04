/*
 *  Scene_House_IncaBook.h
 *  enigma
 *
 *  Created by Rockford on 21/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_HOUSE_INCABOOK_H
#define  _ENIGMA_HOUSE_INCABOOK_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_House_IncaBook : public EScene {
public:
   Scene_House_IncaBook(ESceneDirector *lpSceneDirector);
   ~Scene_House_IncaBook();
   
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
	// Actions, helpers
	void PageTurn(bool left);
	void PageSet(int page);
   
	static const int _nPages = 6;
	int _nCurrentPage;
   KTextFace *_lpFont;
protected:

};

#endif