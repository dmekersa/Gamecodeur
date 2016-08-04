/*
 *  SceneParvis
 *  enigma
 *
 *  Created by Rockford on 28/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_house_Parvis.h"
#include "ESceneDirector.h"
#include "MusicBank.h"

/* Constructeur */
SceneParvis::SceneParvis(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
SceneParvis::~SceneParvis()
{
   delete _lpBgGraphic;
}

void SceneParvis::Init()
{
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE, true);
   ESoundBank::getSound("gong")->playSample();
   StartAnimation("parvis_rubbon_anim");
   
   SetupItem("envelope_parvis");
   if (isVisible("envelope_parvis")) {
      StartEmitter("flake_envelope");
   }
   else {
      StopEmitter("flake_envelope");
   }

   if (!TaskResolved("task_parvis_firstvisit")) {
      ResolveTask("task_parvis_firstvisit");
   }   
   
   if (TaskResolved("task_house_openhouse")) {
      SetVisible("parvis_doorhandle", false);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "parvis_dooropen", true);
   } else {
      if (TestGlobal("__tutorial__")) {
         _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("PARVIS_DIALOG_TUTO1"), 727, 435, -45, 330);
      }
   }
}

void SceneParvis::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void SceneParvis::Logic()
{
	EScene::Logic();
}

void SceneParvis::Draw()
{
   EScene::Draw();
}

void SceneParvis::Close()
{
}

bool SceneParvis::ObjectClicked(const char *szObjectName, float x, float y)
{
   if ( strcmp(szObjectName, "parvis_dooropen") == 0 && isVisible("envelope_parvis"))
   {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("PARVIS_ASHLEY_ENVELOPE"), "", true);
      return true;
   }
   if ( strcmp(szObjectName, "envelope_parvis") == 0)
   {
      StopEmitter("flake_envelope");
      PickupSimple(szObjectName, "ihm_envelope");
      _lpSceneDirector->getDiaryPtr()->beginCreatePage("letterpaper.png");
      _lpSceneDirector->getDiaryPtr()->addStringToPage("ENVELOPE_PARVIS", FONT_DIARY_2, 0, 0);
      _lpSceneDirector->getDiaryPtr()->addImageToPage("logofraternity.png");
      _lpSceneDirector->getDiaryPtr()->endCreatePage();
      return true;
   }
   if ( strcmp(szObjectName, "parvis_notice") == 0)
   {
      int x,y;
      GetObjectPosition("parvis_notice", x, y);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("PARVIS_NOTICE"), "", true);
      return true;
   }
   if ( strcmp(szObjectName, "parvis_doorhandle") == 0)
   {
//      ResolveTask("task_house_openhouse");
//      _lpSceneDirector->getSequencer()->PlaySound(NULL, "opendoor");
//      _lpSceneDirector->getSequencer()->ShowImage(NULL, "parvis_dooropen", true);
      _lpSceneDirector->getSequencer()->PlaySound("closedsound", "doorclosed");
      _lpSceneDirector->getSequencer()->VoiceOver("closed", KStr("PARVIS_ASHLEY_DOOROPEN"), "");
      return true;
   }
   return false;
}

bool SceneParvis::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool SceneParvis::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (strcmp(szObjectName, "parvis_doorhandle") == 0) {
      if (strcmp(szItemName, "inv_key") == 0) {
         SetVisible("parvis_doorhandle", false);
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_house_openhouse");
         ResolveObjective("house","enterhouse");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "opendoor");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "parvis_dooropen", true);
         return true;
      }
   }
   
   return false;
}
