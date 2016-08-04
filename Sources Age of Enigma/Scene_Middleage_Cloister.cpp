/*
 *  Scene_Middleage_Cloister.cpp
 *  enigma
 *
 *  Created by Rockford on 06/10/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Middleage_Cloister.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "ThreadLoader.h"

/* Constructeur */
Scene_Middleage_Cloister::Scene_Middleage_Cloister(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
}

/* Destructeur */
Scene_Middleage_Cloister::~Scene_Middleage_Cloister()
{
   delete _lpBgGraphic;
}

void Scene_Middleage_Cloister::Init()
{
   SetupItem("middleage_plate");
   SetupItem("flaskpieces[1]");
   SetupItem("flaskpieces[2]");
   SetupItem("flaskpieces[3]");
   SetupItem("flaskpieces[4]");
   
   // Première visite
   if (!TaskResolved("task_middleage_visitabbot")) {
      ResolveTask("task_middleage_visitabbot");
      AddObjective("middleage","healabbot");
      AddHint("middleage","healabbot","how");
   }
   
   // Abbe mourant
   if (!TaskResolved("task_middleage_treatabbot")) {
      SetVisible("middleage_dyingabbot", true, true);
   }
   
   if (getAdditionalName() == "completed") {
      // Epilogue
      int x, y;
      ResolveObjective("middleage","healabbot");
      GetObjectPosition("middleage_dyingabbot", x, y, true, false);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_ABBOT_TREAT"), "", false);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_dyingabbot", "middleage_epilogue01");
      GetObjectPosition("middleage_epilogue01", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_ABBOT_EPILOG1"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_ABBOT_EPILOG2"), "", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_EPILOG1"), "", false, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_ABBOT_EPILOG4"), "", false);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_epilogue01", "middleage_epilogue02");
      _lpSceneDirector->getSequencer()->Wait(NULL, 3000);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      ResolveTask("task_middleage_universeresolved");
    //  _lpSceneDirector->getMap()->setUniversVisited("UniverseMiddleage");
      // Retour
      Loader::LoadScene("house_room2");
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "blank", true, true);
      _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/soul_tunnel_r.ogv","tunnel");
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"house_room2","backfrombeyond", false);
   }
}

void Scene_Middleage_Cloister::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
      _lpSceneDirector->getInventory()->AddItem("middleage", "inv_middleage_flask_full");
      AddTask("task_middleage_treatabbot");
      Init();
   }
#endif
}

void Scene_Middleage_Cloister::Logic()
{
	EScene::Logic();
}

void Scene_Middleage_Cloister::Draw()
{
   EScene::Draw();
}

void Scene_Middleage_Cloister::Close()
{
   
}

bool Scene_Middleage_Cloister::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Empèche de quitter le lieu si on doit prendre la pierre
   if (TaskToResolve("task_middleage_enigmastone") && !strcmp(szObjectName, "middleage_epilogue02") == 0) {
      return true;
   }
   
   // Assiette
   if (strcmp(szObjectName, "middleage_plate") == 0)
   {
      PickupSimple("middleage_plate", "inv_middleage_plate");
      return true;
   }

   /* Morceaux de flask */
   if ( strncmp(szObjectName, "flaskpieces", strlen("flaskpieces")) == 0 )
   {
      PickupMultiple(szObjectName,"inv_middleage_flaskpieces",4);
   }
   
   // Abbe mourant
   if (strcmp(szObjectName, "middleage_dyingabbot") == 0) {
      int x, y;
      GetObjectPosition(szObjectName, x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_ABBOT_DYING"), "", true);
   }
   
   return false;
}

bool Scene_Middleage_Cloister::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Middleage_Cloister::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // On donne le remède à l'abbe
   if (strcmp(szObjectName, "middleage_dyingabbot") == 0) {
      if (strcmp(szItemName, "inv_middleage_flask_full") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"reveal");
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL,"creeping_glow",true);
         _lpSceneDirector->getSequencer()->Wait(NULL,1000);
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_middleage_treatabbot");
         setAdditionalName("completed");
         Init();
         return true;
      }
      else {
         int x, y;
         GetObjectPosition(szObjectName, x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_ABBOT_NOGOOD"), "", true);
         return true;
      }
   }
   
   return false;
}

void Scene_Middleage_Cloister::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
}
