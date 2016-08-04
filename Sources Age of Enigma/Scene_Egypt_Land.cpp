/*
 *  Scene_House_Kitchen.cpp
 *  enigma
 *
 *  Created by Rockford on 20/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_Egypt_Land.h"
#include "ESceneDirector.h"

/* Constructeur */
Scene_Egypt_Land::Scene_Egypt_Land(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_Egypt_Land::~Scene_Egypt_Land()
{
   delete _lpBgGraphic;
}

void Scene_Egypt_Land::Init()
{
   if (getAdditionalName() == "completed" && _lpSceneDirector->getInventory()->getItemFromInventory("inv_egypt_key")==NULL) {
      int x,y;
      GetObjectPosition("egypt_shop", x, y);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_MAIN_SHOP_CONGRAT"), "", false);
      GetObjectPosition("egypt_shop", x, y, true, true);
      _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_egypt_key", x, y, 1);
      GetObjectPosition("egypt_shop", x, y);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_MAIN_SHOP_ENTER"), "", false);
   }
   
   if (!TaskResolved("task_egypt_firstvisit")) {
      ResolveTask("task_egypt_firstvisit");
      AddTask("task_egypt_openmaze");
      AddObjective("egypt","entertomb");
      AddHint("egypt","entertomb","how");
      AddHint("egypt","entertomb","key");
   }
   
#ifdef BFG_TEASING
   _lpSceneDirector->getSequencer()->GoToScene(NULL, "teasing", "", false);
#endif
   
}

void Scene_Egypt_Land::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      _lpSceneDirector->GoToScene("egypt_tomb");
   }
#endif
}

void Scene_Egypt_Land::Logic()
{
	EScene::Logic();
}

void Scene_Egypt_Land::Draw()
{
   EScene::Draw();
}

void Scene_Egypt_Land::Close()
{
   
}

bool Scene_Egypt_Land::ObjectClicked(const char *szObjectName, float x, float y)
{
   if ( strcmp(szObjectName, "egypt_walkingguy") == 0)
   {
      int x,y;
      GetObjectPosition("egypt_walkingguy", x, y);
      _lpSceneDirector->getSequencer()->Talk("walkingguy", x, y, KStr("EGYPT_MAIN_PEOPLE1") , "", true);
      return true;
   }
   else
   if ( strcmp(szObjectName, "egypt_travelers") == 0)
   {
      int x,y;
      GetObjectPosition("egypt_travelers", x, y);
      _lpSceneDirector->getSequencer()->Talk("travelers", x, y, KStr("EGYPT_MAIN_PEOPLE2"), "", true);
      return true;
   }
   else if ( strcmp(szObjectName, "egypt_shop") == 0)
   {
      int x,y;
      GetObjectPosition("egypt_shop", x, y);
      if (TaskResolved("task_egypt_talkshop")) {
         if (TaskResolved("task_egypt_getkey")) {
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_MAIN_SHOP_ENTER"), "", false);
         }
         else {
            // Dialogue simplifié
            _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
            _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_MAIN_SHOP_SHOWME"), "", false);
            _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
            _lpSceneDirector->getSequencer()->GoToScene(NULL, "egypt_shop", "play", false);
         }
      }
      else {
         // Dialogue complet
         ResolveTask("task_egypt_talkshop");
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_MAIN_SHOP_HELLO"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_MAIN_NEFERES_ENTER"), "", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_MAIN_SHOP_IHAVEKEY"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("EGYPT_MAIN_NEFERES_IAM"), "", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("EGYPT_MAIN_SHOP_SHOWME"), "", false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "egypt_shop", "play", false);
      }
      return true;
   }
   return false;
}

bool Scene_Egypt_Land::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

