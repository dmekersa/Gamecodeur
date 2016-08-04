/*
 *  SceneTest.cpp
 *  pngviewer
 *
 *  Created by Rockford on 05/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */
#include "ImageBank.h"
#include "EScene.h"
#include "SceneTest.h"
#include "ESceneDirector.h"

void SceneTest::Init()
{
	 /* Load button graphics */
   _lpButtonGraphic = EImageBank::getImage("buttons.png");
	StartAnimation("crabeP2P");

}

void SceneTest::_myUIMessageHandler (KUIMessage *lpUIMessage) {
   
}

void SceneTest::Check()
{
	long mx, my;

	mx = KInput::getMouseX ();
    my = KInput::getMouseY ();

	/* Image à la place du pointeur de souris 
	objimg = EScene::GetObjectImageByName("key.png");
	objimg->Move(mx, my);*/
	
	/* Emitter qui suit la souris */
	MoveEmitter("shortcirc",(float)mx,(float)my);

	if (KInput::getLeftButtonState()) {
		
      /* Emitter qui est bougé sur clic souris */
		MoveEmitter("explosion", (float)mx, (float)my);
   }

}

void SceneTest::Logic()
{
	EScene::Logic();
}

void SceneTest::Draw()
{
   EScene::Draw();
}

void SceneTest::Close()
{
   EObjectContainer::CleanUp(_strSceneName);
}

SceneTest::~SceneTest()
{

}

