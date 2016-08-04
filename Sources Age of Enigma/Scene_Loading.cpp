/*
 *  Scene_Loading.cpp
 *  enigma
 *
 *  Created by Rockford on 21/03/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */
#include "ThreadLoader.h"

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Loading.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EPointer.h"
#include "ThreadLoader.h"

/* Constructeur */
Scene_Loading::Scene_Loading(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _bPreloadDone = false;
   _bChangeScene = false;
   _lpBgGraphic = KPTK::createKGraphic ();
    font = EFontBank::getFont("jancient.ttf",46);
   _lpFont = EFontBank::getFont("tahoma.ttf",12);

    _nMaxLoading = 0;
   _lpCounterTimeOut = new KCounter();
   _lpCounterTimeOut->startCounter(0,1,0,5000,K_COUNTER_LINEAR);
}

/* Destructeur */
Scene_Loading::~Scene_Loading()
{
   XDELETE(_lpBgGraphic);
   XDELETE(font);
   XDELETE(_lpFont);
}

void Scene_Loading::Init()
{
   _lpSceneDirector->setZoomin(false);
   _lpSceneDirector->PrepareMouseCursor(DIRECTOR_MOUSECURSOR_NONE);
   
   StartAnimation("ashleymenu");
}

void Scene_Loading::Check()
{
   EScene::Check();

#ifdef SCENE_SHORTCUT
   if (Mouse.LeftEvent()) {

      _lpSceneDirector->enterPseudo(); // vrai si le pseudo a été rentré

      _lpSceneDirector->setZoomin(false);
      _lpSceneDirector->GoToScene( "menu", "", false);
   }
#endif
}

void Scene_Loading::Logic()
{
	EScene::Logic();
}

void Scene_Loading::Draw()
{
   EScene::Draw();
   
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   _lpCounterTimeOut->move(fElapsed);
   

   if (!_bPreloadDone && _State == SCENE_STABLE) {
      _bPreloadDone = true;
      Loader::LoadScene(GetStrGlobal("__lastscene__","parvis"));
   }

   if (Loader::GetPreLoadCount() > _nMaxLoading)
      _nMaxLoading = Loader::GetPreLoadCount();

   // SŽcuritŽ en cas d'aucun preload
   if (_lpCounterTimeOut->isCompleted()) {
      if (_nMaxLoading <= 0) {
         _bChangeScene = true;
         _lpSceneDirector->getSequencer()->GoToScene("end", "menu", "");
      }
   }
   
   if (Loader::GetPreLoadCount() <=0 && !_bChangeScene && _bPreloadDone && _nMaxLoading>0) {     
      // input du pseudo
      if(_lpSceneDirector->enterPseudo()) { // vrai si le pseudo a été rentré
         _bChangeScene = true;
         _lpSceneDirector->setZoomin(false);
         _lpSceneDirector->getSequencer()->Wait("wend",2000);
         _lpSceneDirector->getSequencer()->GoToScene("end", "menu", "");
      }
   } else if (_State == SCENE_STABLE) {
      font->drawStringCentered(KStr("LOADING"),0,1024,600);

      // Display de la bar de chargement.
      int width = 200;
      Coord pos = Coord((1024-width)/2,650);
      _lpBgGraphic->drawRect(pos.x,pos.y,pos.x+width,pos.y+10,0,0,0,0.5f); 
      
      float displayWidth = ((_nMaxLoading - Loader::GetPreLoadCount())/(float)_nMaxLoading)*(width-4);
      if (displayWidth <0)
         displayWidth=0;

      _lpBgGraphic->drawRect(pos.x+2,pos.y+2,pos.x+displayWidth,pos.y+8,0.5,0.5,0.5,0.9);      
   }
   
   char sDebugString[256];
   sprintf(sDebugString, "%s", MyGame::getGame()->getGameVersion());
   DrawFancyString(sDebugString, _lpFont, 5, 5, 1024, false, .5);
   
}

void Scene_Loading::Close()
{
}

bool Scene_Loading::ObjectClicked(const char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Loading::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}