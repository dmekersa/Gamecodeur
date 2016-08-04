/*
 *  Scene_House_Out.cpp
 *  enigma
 *
 *  Created by Rockford on 22/03/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Out.h"
#include "ESceneDirector.h"
#include "EMiniJeuPentagram.h"
#include "MusicBank.h"

/* Constructeur */
Scene_House_Out::Scene_House_Out(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _nYesForTutorial = -1;
   _bTutorial = false;
   _bTutorialAsked = false;

   EMiniJeuPentagram::Preload();
}

/* Destructeur */
Scene_House_Out::~Scene_House_Out()
{
   delete _lpBgGraphic;
}

void Scene_House_Out::Init()
{
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE, true);
   _lpSceneDirector->ShowIHM(true);

   // Glit sur la portière
   if (TaskResolved("car_box_map") && TaskResolved("car_box_key")) {
      GetObjectImageByName("out_car_zone")->EnableGlitch(false);
   }
   
   if (!TaskResolved("task_out_firstvisit")) {
      ResolveTask("task_out_firstvisit");
      AddObjective("house","getmap");
      AddHint("house","getmap","how");
      _lpSceneDirector->getSequencer()->Callback(NULL, "tutoask");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage();
      _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_out.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_OUT", FONT_DIARY_1, 0, 0);
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
   }
   
   if (TaskResolved("out_house_zeps")) {
      GetObjectImageByName("out_house_zeps")->EnableGlitch(false);
   }
         
   if (getAdditionalName() == "gotohouse") {
      AddObjective("house","enterhouse");
      AddHint("house","enterhouse","how");
      if (TestGlobal("__tutorial__")) {
         _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("HOUSE_OUT_TUTO_ENTERHOUSE"), 273, 204, 180, 200);
      }
   }

   _lpSceneDirector->getSequencer()->PreloadVideo("videos/zeppelins.ogv");
}

void Scene_House_Out::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_House_Out::Logic()
{
	EScene::Logic();
   
   // Le joueur a répondu à la boite Yes/No demandant si il voulait un tuto
   if (_nYesForTutorial != -1) {
      // Activation du tutorial
      if (_nYesForTutorial == 1) {
         _bTutorial = true;
         // Démarre le tutorial
         _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("HOUSE_OUT_TUTO_CAR"), 776, 367, -45, 200);         
         SetGlobal("__tutorial__","1");
      }
      else {
         SetGlobal("__tutorial__","0");
      }
      _nYesForTutorial = -1;
   }
}

void Scene_House_Out::Draw()
{
   EScene::Draw();
}

void Scene_House_Out::Close()
{
}

bool Scene_House_Out::ObjectClicked(const char *szObjectName, float x, float y)
{
   if ( strcmp(szObjectName, "out_car_zone") == 0)
   {
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "cardoor");
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_out_car", "", false);
      return true;
   }
   if ( strcmp(szObjectName, "out_house_zeps") == 0)
   {
      ResolveTask("task_out_zeps");
      GetObjectImageByName("out_house_zeps")->EnableGlitch(false);
      _lpSceneDirector->getSequencer()->GotoVideo(szObjectName, "videos/zeppelins.ogv", "zeppelins");
      _lpSceneDirector->getSequencer()->Talk("zeps", CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_OUT_ASHLEY_ZEPS"), "", true);
      if (!TaskResolved("task_out_zeps_page")) {
         ResolveTask("task_out_zeps_page");
         _lpSceneDirector->getDiaryPtr()->beginCreatePage();
         _lpSceneDirector->getDiaryPtr()->addImageToPage("diary_zeps.png");
         _lpSceneDirector->getDiaryPtr()->addStringToPage("DIARY_ZEPPELINS", FONT_DIARY_1, 0, 0);
         _lpSceneDirector->getDiaryPtr()->endCreatePage();
      }
      return true;
   }
   if ( strcmp(szObjectName, "out_house_zone") == 0)
   {
      if (ObjectiveResolved("house","getmap")) {
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "parvis", "", false);
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_OUT_ASHLEY_NEEDMAP"), "", true);
         if (TestGlobal("__tutorial__")) {
            _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("HOUSE_OUT_TUTO_CAR"), 913, 504, -45, 200);         
         }
      }
      return true;
   }
   if (strcmp(szObjectName, "house_out_isaac_quiet") == 0) {
      ESoundBank::getSound("isaac_bark")->playSample();
      if (!ObjectiveResolved("house","getmap")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_OUT_ASHLEY_ISAAC1"), "", true);
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_OUT_ASHLEY_ISAAC2"), "", true);
      }
   }
   
   return false;
}

bool Scene_House_Out::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_House_Out::ItemUsed(const char *szItemName, const char *szObjectName)
{
   return false;
}

void Scene_House_Out::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (bIsRevolved) {
      return;
   }
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_ASHLEY_GASP"), "", true, false);
      return;
   }
}

void Scene_House_Out::Callback(const char *szParam)
{
   if (strcmp(szParam, "tutoask") == 0) {
      if (!TaskResolved("__reply_tutorial__")) {
         ResolveTask("__reply_tutorial__");
         _lpSceneDirector->OpenDialogboxYN(KStr("HOUSE_OUT_TUTOASK"), &_nYesForTutorial);
      }
   }
}
