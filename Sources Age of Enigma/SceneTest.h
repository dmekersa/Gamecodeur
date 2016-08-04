/*
 *  SceneTest.h
 *
 *  Created by Rockford on 05/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENETEST_H
#define  _ENIGMA_SCENETEST_H

#include "EScene.h"
#include "KPTK.h"

class SceneTest : public EScene {
public:
   SceneTest(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector) {};
   ~SceneTest();
   void Init ();
   void Close ();
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();

private:
	KGraphic *_lpBgGraphic;
	KGraphic *_lpMouseCursor;
	KGraphic *_lpButtonGraphic;
	KUIElement *_lpUIContainer;
	static void _myUIMessageHandler (KUIMessage *lpUIMessage);
   
protected:

};
#endif