/*
 *  Scene_House_Livingroom_Box.cpp
 *  enigma
 *
 *  Created by Rockford on 13/05/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Livingroom_Box.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuJigsawBook.h"

/* Constructeur */
Scene_House_Livingroom_Box::Scene_House_Livingroom_Box(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   EMiniJeuJigsawBook::Preload();
}

/* Destructeur */
Scene_House_Livingroom_Box::~Scene_House_Livingroom_Box()
{
}

void Scene_House_Livingroom_Box::Init()
{
   if (TaskResolved("task_livingroom_openbox")) {
      SetVisible("house_livingroom_boxtopclosed", false, true);
      SetVisible("house_livingroom_boxtopopen", true, true);
      SetupItem("livingroom_bookinca_eyestatue");
   }
   
}

void Scene_House_Livingroom_Box::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_House_Livingroom_Box::Logic()
{
	EScene::Logic();
}

void Scene_House_Livingroom_Box::Draw()
{
   EScene::Draw();
}

void Scene_House_Livingroom_Box::Close()
{
}

bool Scene_House_Livingroom_Box::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "livingroom_bookinca_eyestatue") == 0) {
      PickupSimple("livingroom_bookinca_eyestatue", "inv_statuette_eye");
      return true;
   }
   
   if (strcmp(szObjectName, "house_livingroom_boxtopclosed") == 0) {
      EMiniJeuJigsawBook *_lpMiniJeu = new EMiniJeuJigsawBook();
      _lpMiniJeu->SetVariation(0); 
      _lpSceneDirector->GoToMiniGame("livingroom_box",_lpMiniJeu);
      return true;
   }

   return false;
}

bool Scene_House_Livingroom_Box::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Scene_House_Livingroom_Box::ItemUsed(const char *szItemName, const char *szObjectName)
{   
   return false;
}

void Scene_House_Livingroom_Box::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FAILED"), "", true);
      return;
   }
   
   if (strcmp(szGameName, "livingroom_box")==0 && bIsRevolved) {
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
      ResolveTask("task_livingroom_openbox");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "livingroom_bookinca_eyestatue", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "drawer_open");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL, "house_livingroom_boxtopclosed", "house_livingroom_boxtopopen");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
   }
}
