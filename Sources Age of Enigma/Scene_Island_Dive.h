/*
 *  SceneFondMarin.h
 *
 *  Created by Rockford on 19/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEFONDMARIN_H
#define  _ENIGMA_SCENEFONDMARIN_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class SceneFondMarin : public EScene {
public:
   SceneFondMarin(ESceneDirector *lpSceneDirector);
   ~SceneFondMarin();
   //   SceneFondMarin(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector) {};
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
   
private:
	KGraphic *_lpBgGraphic;
   KSound *_lpSndDive;
   // Bénitier
   bool _bBenitierOpen;
   KCounter *_lpAnimTempoBenitier;
   bool _bBubblesStopDone;
   // Apnée
   KTextFace *_lpFont;
   KCounter *_lpAnimApnee;
   bool _bDiveEnd;
   
protected:
   
};
#endif