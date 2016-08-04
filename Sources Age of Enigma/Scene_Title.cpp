/*
 *  Scene_Title.cpp
 *  enigma
 *
 *  Created by Rockford on 31/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Title.h"
#include "ESceneDirector.h"
#include "global.h"
#include <string.h>
#include "ImageBank.h"
#include "FontBank.h"
#include "MusicBank.h"

/* Constructeur */
SceneTitle::SceneTitle(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic();
   /* Load font */
   _lpFont1 = EFontBank::getFont("jancient22",-99);
   /* Anim affichage du titre et du sous titre */
   _lpAnimEnd = new KCounter();
   _lpAnimEnd->startCounter(0, 1, 0, 5000, K_COUNTER_EASEOUT);
   
   _lpSound1 = NULL;
   _lpSound2 = NULL;
   _nTempo = 0;
   _nTempo2 = 0;
   _bEnd = false;
}

/* Destructeur */
SceneTitle::~SceneTitle()
{
   XDELETE(_lpBgGraphic);
   XDELETE(_lpAnimEnd);
   XDELETE(_lpFont1);
}

void SceneTitle::Init()
{
//   EMusicBank::getMusic("intro_speech2")->playStream(false);
   _lpSound1 = ESoundBank::getSound("car_instop");
   _lpSound1->playSample();
}

void SceneTitle::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void SceneTitle::Logic()
{
	EScene::Logic();
}

void SceneTitle::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   EScene::Draw();
   
   std::string strTitle=KStr("TITLE_TEXT1");
   std::string strSubTitle=KStr("TITLE_TEXT2");
   _lpAnimEnd->move(fElapsed);
   
   if (_lpAnimEnd->isCompleted() && !_bEnd) {
      _bEnd = true;
      _lpSceneDirector->GoToScene("house_out", "", false);
   }
   
   DrawTitle(strTitle, strSubTitle, getFade());
}

void SceneTitle::DrawTitle(std::string strTitle, std::string strSubTitle, float fBlend)
{
//   DrawFancyString(strTitle.c_str(), _lpFont1, 1024/2, 768/2-30, 1024, true, fBlend);
   //int h = _lpFont1->getMultilineHeight(strTitle.c_str(), 0, 1024, 768/2, TEXTSTYLE_CENTER);
   DrawFancyString(strTitle.c_str(), _lpFont1, 1024/2, 300, 1024, true, fBlend);
   //int w = _lpFont1->getMultilineWidth(strTitle.c_str(), 0, 1024, 768/2, TEXTSTYLE_CENTER);
   DrawFancyString(strSubTitle.c_str(), _lpFont1, 1024/2, 300+22+5, 1024, true, fBlend);
}

void SceneTitle::Close()
{
//   if (_lpSound1) {
//      _lpSound1->stopSample();
//   }
   if (_lpSound2) {
      _lpSound2->stopSample();
   }
}

bool SceneTitle::ObjectClicked(const char *szObjectName, float x, float y)
{
   return false;
}

bool SceneTitle::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Préchargement des ressources */
KGraphic *lpGraphic = NULL;
KTextFace *lpFont2 =NULL;
void SceneTitle::Load()
{
   lpGraphic = KPTK::createKGraphic();
   lpFont2 = EFontBank::getFont("NITECLUB.TTF",40);
	KMiscTools::enumerateFolder(KMiscTools::makeFilePath("pictures"), (enumProc) LoadDirectoryPictures , NULL  );
   XDELETE(lpFont2);
   XDELETE(lpGraphic);
}

bool LoadDirectoryPictures(char *filename, bool isFolder)
{
//   KGame::getGame()->_lpKWindow->setWorldView(0.0f,0.0f,0,1,true);
   
	if ((isFolder == false)) // && (strstr (filename, PYRO_EXT) != NULL))
	{
      lpGraphic = EImageBank::getImage(filename);
      lpFont2->setBlitColor(1, 1, 1, 1);
      lpFont2->drawMultiline(filename, 0, 1024, int(768/2) -
                              lpFont2->getMultilineHeight(filename,0,1024,int(768/2),TEXTSTYLE_CENTER)
                              , TEXTSTYLE_CENTER);
      MyGame::getGame()->getKWindow()->flipBackBuffer ();
      
      
	}
   
	return true;
}
