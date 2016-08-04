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
#include "Scene_House_Kitchen.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "MusicBank.h"
#include "EMiniJeuPoltergeist.h"

/* Constructeur */
SceneKitchen::SceneKitchen(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();

//   EMiniJeuPoltergeist::Preload();
}

/* Destructeur */
SceneKitchen::~SceneKitchen()
{
   delete _lpBgGraphic;
}

void SceneKitchen::Init()
{
   // Etats
   // Le poltergeist n'apparait que si on tente d'ouvrir le robinet
   SetVisible("kitchen_polt", false);
   // La casserole
   SetupItem("kitchen_pan");
   // L'eau
   if (TestGlobal("kitchen_faucet"))
   {
      SetVisible("kitchen_water", true);
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "faucet");
   }
   else {
      SetGlobal("kitchen_faucet","0");
      SetVisible("kitchen_water", false);
   }
   
   // Les objets
   SetupItem("basement_rag");
   SetupItem("house_kitchen_knife");
   
   if (TaskResolved("task_kitchen_look999")) {
      GetObjectImageByName("kitchen_call999_zone")->EnableGlitch(false);
   }

}

void SceneKitchen::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void SceneKitchen::Logic()
{
	EScene::Logic();
}

void SceneKitchen::Draw()
{
   EScene::Draw();
}

void SceneKitchen::Close()
{
   ESoundBank::StopSample("poltergeistloop");
}

bool SceneKitchen::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (isVisible("kitchen_call999")) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "kitchen_call999", false);
      return true;
   }
   
//   // Clic sur le poltergeist
//   if (strcmp(szObjectName, "kitchen_polt") == 0) {
//     StopEmitter("creeping_polt");
//	  SetVisible("kitchen_polt",false,false);
//      ESoundBank::getSound("poltergeistloop",true)->stopSample();
//	  EMiniJeuPoltergeist *_lpMiniJeu = new EMiniJeuPoltergeist();
//     _lpSceneDirector->GoToMiniGame("poltergeist",_lpMiniJeu,"poltergeist");
//     return true;
//   }

   if ( strcmp(szObjectName, "basement_rag") == 0)
   {
      PickupSimple(szObjectName, "inv_house_basement_rag");
      return true;
   }

   if ( strcmp(szObjectName, "house_kitchen_knife") == 0)
   {
      PickupSimple(szObjectName, "inv_house_knife");
      return true;
   }
   
   if ( strcmp(szObjectName, "kitchen_badpan") == 0 )
   {
      _lpSceneDirector->getSequencer()->Talk("badpan", CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_KITCHEN_ASHLEY_DAD"),"", true);
   }
   
   if ( strcmp(szObjectName, "kitchen_pan") == 0 )
   {
      PickupSimple(szObjectName, "inv_pan");
   }
   
   if (strcmp(szObjectName, "kitchen_call999_zone") == 0) {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "kitchen_call999", true);
      _lpSceneDirector->getSequencer()->Talk("badpan", CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_KITCHEN_ASHLEY_CALL999"),"", true);
      GetObjectImageByName("kitchen_call999_zone")->EnableGlitch(false);
      ResolveTask("task_kitchen_lookcall999");
      return true;
   }

   if ( strcmp(szObjectName, "kitchen_faucet") == 0 )
   {
//      if (!TaskResolved("task_house_kitchen_killwatermarc")) {
//         StartAnimation("poltergeist_anim");
//         StartEmitter("creeping_polt");
//         ESoundBank::getSound("poltergeistloop",true)->playSample();
//         SetVisible("kitchen_polt", true);
//         int x,y;
//         GetObjectPosition("kitchen_polt", x, y);
////         _lpSceneDirector->getSequencer()->Talk("thankyou", x, y, KStr("HOUSE_KITCHEN_POLTER_WATER"), "", true);
//         _lpSceneDirector->getSequencer()->Talk("badpan", CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_KITCHEN_POLTER_WATER"),"", true);
//         ESoundBank::getSound("poltergeist")->playSample();
//      }
//      else 
      if (!TestGlobal("kitchen_faucet")) {
         SetGlobal("kitchen_faucet","1");
         SetVisible("kitchen_water", true);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "faucet");
      }
      else {
         SetGlobal("kitchen_faucet","0");
         SetVisible("kitchen_water", false);
      }

   }

   return false;
}

bool SceneKitchen::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool SceneKitchen::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if ( strcmp(szItemName, "inv_pan") == 0 && strcmp(szObjectName, "kitchen_faucet") == 0 ) {
      /* Essaye de remplir la casserole */
      if (!TestGlobal("kitchen_faucet"))
      {
         _lpSceneDirector->getSequencer()->VoiceOver("no water", KStr("HOUSE_KITCHEN_VOICEOVER_NOWATER"), "");
         return true;
      }
      /* Remplie la casserole */
      if (!TaskResolved("task_house_kitchen_fill_pan") && TestGlobal("kitchen_faucet"))
      {
         ESoundBank::getSound("success")->playSample();
         // Remplace la casserole vide par une casserole pleine
         _lpSceneDirector->DropItem("inv_pan");
//         EInventoryItem *item = new EInventoryItem(_strSceneUniverseName.c_str(),"inv_panfull",1,1);
//         _lpSceneDirector->getInventory()->AddItem(item);
//         _lpSceneDirector->getInventory()->BlingItem("inv_panfull");
         int x,y;
         GetObjectPosition("kitchen_water", x, y);
         _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_panfull", x, y, 1);
         ResolveTask("task_house_kitchen_fill_pan");
         _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("HOUSE_KITCHEN_VOICEOVER_FILL"), "");
         return true;
      }
      // Déjà pleine
      if (TestGlobal("task_house_kitchen_fill_pan") && TestGlobal("faucet"))
      {
         _lpSceneDirector->getSequencer()->VoiceOver("pan already filled", KStr("HOUSE_KITCHEN_ASHLEY_ENOUGH"), "");
         return true;
      }
   }
   return false;
}

void SceneKitchen::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "poltergeist")==0) {
      StopEmitter("creeping_polt");
      _lpSceneDirector->getSequencer()->ShowImage(NULL,"kitchen_polt",false);
      ESoundBank::StopSample("poltergeistloop");
      // Tâche résolue
      ResolveTask("task_house_kitchen_killwatermarc");
      AddTask("task_house_kitchen_fill_pan");
   }
}
