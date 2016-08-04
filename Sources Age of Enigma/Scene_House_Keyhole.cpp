/*
 *  Scene_House_Keyhole.cpp
 *  enigma
 *
 *  Created by Rockford on 19/08/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Keyhole.h"
#include "ESceneDirector.h"
#include "SoundBank.h"
#include "GlobalBank.h"
#include "MusicBank.h"

/* Constructeur */
Scene_House_Keyhole::Scene_House_Keyhole(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpGhostVoice = EMusicBank::getMusic("ghost_longvoice_japan");
   _lpGhostVoice->setVolume(0);
   _lpGhostVoice->playStream(true);
   _lpCounterEye = new KCounter;
   _bPsycho = false;
   if (TaskResolved("task_house_basement_psycho")) {
      _bPsycho = true;
   }
}

/* Destructeur */
Scene_House_Keyhole::~Scene_House_Keyhole()
{
   XDELETE(_lpCounterEye);
   _lpGhostVoice->stopStream();
}

void Scene_House_Keyhole::Init()
{
   _lpCounterEye->startCounter(0.0f, 1.0f, 0, 3000, K_COUNTER_LINEAR);
   StartAnimation("keyhole_anim");
}

void Scene_House_Keyhole::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_House_Keyhole::Logic()
{
	EScene::Logic();
}

void Scene_House_Keyhole::Draw()
{
   EScene::Draw();
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   
   if (!_bPsycho) {
      _lpCounterEye->move(fElapsed);
      if (_lpCounterEye->isCompleted()) {
         _bPsycho = true;
         int x,y;
         GetObjectPosition("keyhole", x, y);
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"spooky_piano");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "eye", true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENTKEYHOLE_NATSUMI_GOAWAY"), "", false, false, _lpGhostVoice);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENTKEYHOLE_ASHLEY_DONTBE"),"", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENTKEYHOLE_NATSUMI_DOYOU"), "", false, false, _lpGhostVoice);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENTKEYHOLE_ASHLEY_NO"),"", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("HOUSE_BASEMENTKEYHOLE_NATSUMI_SOGO"), "", false, false, _lpGhostVoice);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "eye", false);
//         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENTKEYHOLE_ASHLEY_NOTEASY"),"", true, false);
         ResolveTask("task_house_basement_psycho");
      }
   }
}

void Scene_House_Keyhole::Close()
{
   
}

bool Scene_House_Keyhole::ObjectClicked(const char *szObjectName, float x, float y)
{
   // On refait sursauter le joueur
   if ( strcmp(szObjectName, "keyhole") == 0 && _bPsycho == true)
   {
      int xg,yg;
      GetObjectPosition("keyhole", xg, yg);
      _lpSceneDirector->getSequencer()->PlaySound(NULL,"spooky_piano");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "eye", true);
      _lpSceneDirector->getSequencer()->Talk(NULL, xg, yg, KStr("HOUSE_BASEMENTKEYHOLE_NATSUMI_ISAID"), "", false, false, _lpGhostVoice);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "eye", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_BASEMENTKEYHOLE_ASHLEY_OUPS"),"", true, false);
   }
   return false;
}

bool Scene_House_Keyhole::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_House_Keyhole::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if ( strcmp(szItemName, "inv_basementkey") == 0 && strcmp(szObjectName, "keyhole") == 0 ) {
      ResolveTask("task_house_basement_opendoor");
      AddTask("task_house_basement_light");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "opendoor");
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "stairsdown");
      // On peut maintenant se débarrasser de la clé
      _lpSceneDirector->DropItem("inv_basementkey");
      return true;
   }
   return false;
}