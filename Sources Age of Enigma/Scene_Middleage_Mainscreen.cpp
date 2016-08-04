/*
 *  Scene_Middleage_Mainscreen.cpp
 *  enigma
 *
 *  Created by Rockford on 06/10/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Middleage_Mainscreen.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "MusicBank.h"
#include "EMiniJeuMemoryPlant.h"

/* Constructeur */
Scene_Middleage_Mainscreen::Scene_Middleage_Mainscreen(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   
   EMiniJeuMemoryPlant::Preload();
}

/* Destructeur */
Scene_Middleage_Mainscreen::~Scene_Middleage_Mainscreen()
{
   delete _lpBgGraphic;
}

void Scene_Middleage_Mainscreen::Init()
{
   SetupItem("middleage_wood");
   
   SetupItem("middleage_rue_img");
   SetupItem("middleage_aneth_img");
   SetupItem("middleage_cerfeuil_img");
   SetupItem("middleage_ail_img");
   SetupItem("middleage_pavot_img");
   SetupItem("middleage_fenouil_img");
   SetupItem("middleage_ginkgo_img");
   SetupItem("middleage_millepertuis_img");
   
   if (TaskResolved("task_middleage_getcork")) {
      SetVisible("middleage_oak_zone", false);
   }

   if (TaskResolved("task_middleage_gather_ginkgo")) {
      SetVisible("middleage_ginkgo_zone", false);
   }

   if (TaskResolved("task_middleage_gather_rue")) {
      SetVisible("middleage_rue_zone", false);
   }

   if (TaskResolved("task_middleage_gather_dill")) {
      SetVisible("middleage_aneth_zone", false);
   }

   if (TaskResolved("task_middleage_gather_chervil")) {
      SetVisible("middleage_cerfeuil_zone", false);
   }

   if (TaskResolved("task_middleage_gather_garlic")) {
      SetVisible("middleage_ail_zone", false);
   }

   if (TaskResolved("task_middleage_gather_poppy")) {
      SetVisible("middleage_pavot_zone", false);
   }

   if (TaskResolved("task_middleage_gather_fennel")) {
      SetVisible("middleage_fenouil_zone", false);
   }

   if (TaskResolved("task_middleage_gather_wort")) {
      SetVisible("middleage_millepertuis_zone", false);
   }
       
#ifdef BFG_TEASING
   _lpSceneDirector->getSequencer()->GoToScene(NULL, "teasing", "", false);
#endif
   
}

void Scene_Middleage_Mainscreen::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
   }
#endif
}

void Scene_Middleage_Mainscreen::Logic()
{
	EScene::Logic();
}

void Scene_Middleage_Mainscreen::Draw()
{
   EScene::Draw();
}

void Scene_Middleage_Mainscreen::Close()
{
}

bool Scene_Middleage_Mainscreen::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "middleage_wood") == 0)
   {
      PickupMultiple("middleage_wood", "inv_middleage_wood",-1);
      return true;
   }
   if (strcmp(szObjectName, "middleage_ginkgo_zone") == 0
       || strcmp(szObjectName, "middleage_rue_zone") == 0
       || strcmp(szObjectName, "middleage_aneth_zone") == 0
       || strcmp(szObjectName, "middleage_cerfeuil_zone") == 0
       || strcmp(szObjectName, "middleage_ail_zone") == 0
       || strcmp(szObjectName, "middleage_pavot_zone") == 0
       || strcmp(szObjectName, "middleage_fenouil_zone") == 0
       || strcmp(szObjectName, "middleage_millepertuis_zone") == 0
       || strcmp(szObjectName, "middleage_oak_zone") == 0
       )
   {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_NEEDTOOL"), "", true, false);
      return true;
   }
   return false;
}

bool Scene_Middleage_Mainscreen::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Middleage_Mainscreen::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (strcmp(szItemName, "inv_middleage_sickle") == 0) {
      if (strcmp(szObjectName, "middleage_oak_zone") == 0) {
         if (!TaskResolved("task_middleage_getcork")) {
            ResolveTask("task_middleage_getcork");
            PickupMultiple("middleage_oak_zone", "inv_middleage_cork",-1);
            SetVisible("middleage_oak_zone", false);
            return true;
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_ALREADY"), "", true, false);
            return true;
         }
      }

      if (strcmp(szObjectName, "middleage_ginkgo_zone") == 0) {
         if (!TaskResolved("task_middleage_gather_ginkgo")) {
            EMiniJeuBase *lpMiniJeu = new EMiniJeuMemoryPlant();
			((EMiniJeuMemoryPlant *)lpMiniJeu)->SetVariation(0);
            _lpSceneDirector->GoToMiniGame("ginkgo", lpMiniJeu);
            return true;
         }
         else if (TaskResolved("task_middleage_gather_ginkgo")) {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_ALREADY"), "", true, false);
            return true;
         }
      }

      if (strcmp(szObjectName, "middleage_rue_zone") == 0) {
         if (!TaskResolved("task_middleage_gather_rue")) {
            EMiniJeuBase *lpMiniJeu = new EMiniJeuMemoryPlant();
			((EMiniJeuMemoryPlant *)lpMiniJeu)->SetVariation(1);
            _lpSceneDirector->GoToMiniGame("rue", lpMiniJeu);
            return true;
         }
         else if (TaskResolved("task_middleage_gather_rue")) {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_ALREADY"), "", true, false);
            return true;
         }      
      }

      if (strcmp(szObjectName, "middleage_aneth_zone") == 0) {
         if (!TaskResolved("task_middleage_gather_dill")) {
            SetVisible("middleage_aneth_zone", false);
            PickupSimple("middleage_aneth_img", "inv_middleage_dill");
            ResolveTask("task_middleage_gather_dill");
            return true;
         }
         else if (TaskResolved("task_middleage_gather_dill")) {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_ALREADY"), "", true, false);
            return true;
         }
      }
      
      if (strcmp(szObjectName, "middleage_cerfeuil_zone") == 0) {
         if (!TaskResolved("task_middleage_gather_chervil")) {
            SetVisible("middleage_cerfeuil_zone", false);
            PickupSimple("middleage_cerfeuil_img", "inv_middleage_chervil");
            ResolveTask("task_middleage_gather_chervil");
            return true;
         }
         else if (TaskResolved("task_middleage_gather_chervil")) {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_ALREADY"), "", true, false);
            return true;
         }      
      }
      
      if (strcmp(szObjectName, "middleage_ail_zone") == 0) {
         if (!TaskResolved("task_middleage_gather_garlic")) {
            EMiniJeuBase *lpMiniJeu = new EMiniJeuMemoryPlant();
			((EMiniJeuMemoryPlant *)lpMiniJeu)->SetVariation(1);
            _lpSceneDirector->GoToMiniGame("ail", lpMiniJeu);
            return true;
         }
         else if (TaskResolved("task_middleage_gather_garlic")) {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_ALREADY"), "", true, false);
            return true;
         }      
      }
      
      if (strcmp(szObjectName, "middleage_pavot_zone") == 0) {
         if (!TaskResolved("task_middleage_gather_poppy")) {
            EMiniJeuBase *lpMiniJeu = new EMiniJeuMemoryPlant();
			((EMiniJeuMemoryPlant *)lpMiniJeu)->SetVariation(2);
            _lpSceneDirector->GoToMiniGame("pavot", lpMiniJeu);
            return true;
         }
         else if (TaskResolved("task_middleage_gather_poppy")) {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_ALREADY"), "", true, false);
            return true;
         }
      }
      
      if (strcmp(szObjectName, "middleage_fenouil_zone") == 0) {
         if (!TaskResolved("task_middleage_gather_fennel")) {
            SetVisible("middleage_fenouil_zone", false);
            PickupSimple("middleage_fenouil_img", "inv_middleage_fennel");
            ResolveTask("task_middleage_gather_fennel");
            return true;
         }
         else if (TaskResolved("task_middleage_gather_fennel")) {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_ALREADY"), "", true, false);
            return true;
         }
      }
      
      if (strcmp(szObjectName, "middleage_millepertuis_zone") == 0) {
         if (!TaskResolved("task_middleage_gather_wort")) {
            SetVisible("middleage_millepertuis_zone", false);
            PickupSimple("middleage_millepertuis_img", "inv_middleage_wort");
            ResolveTask("task_middleage_gather_wort");
            return true;
         }
         else if (TaskResolved("task_middleage_gather_wort")) {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_ALREADY"), "", true, false);
            return true;
         }
      }
      
   }
   
   return false;
}

void Scene_Middleage_Mainscreen::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
   if (strcmp(szGameName, "ginkgo") == 0 && bIsRevolved) {
      PickupSimple("middleage_ginkgo_img", "inv_middleage_ginkgo");
      ResolveTask("task_middleage_gather_ginkgo");
      SetVisible("middleage_ginkgo_zone", false);
      return;
   }
   if (strcmp(szGameName, "rue") == 0 && bIsRevolved) {
      PickupSimple("middleage_rue_img", "inv_middleage_rue");
      ResolveTask("task_middleage_gather_rue");
      SetVisible("middleage_rue_zone", false);
      return;
   }
   if (strcmp(szGameName, "ail") == 0 && bIsRevolved) {
      PickupSimple("middleage_ail_img", "inv_middleage_garlic");
      ResolveTask("task_middleage_gather_garlic");
      SetVisible("middleage_ail_zone", false);
      return;
   }
   if (strcmp(szGameName, "pavot") == 0 && bIsRevolved) {
      PickupSimple("middleage_pavot_img", "inv_middleage_poppy");
      ResolveTask("task_middleage_gather_poppy");
      SetVisible("middleage_pavot_zone", false);
      return;
   }
}
