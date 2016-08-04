/*
 *  Scene_House_Livingroom_Table.cpp
 *  enigma
 *
 *  Created by Rockford on 15/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Livingroom_Table.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuTemplate.h"

/* Constructeur */
Scene_House_Livingroom_Table::Scene_House_Livingroom_Table(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _bInition = false;
   _bRaising = false;
   _fRaising = 0;
   _lpCounterInition = new KCounter();
   _lpSoundEarthQuake = NULL;

   if (!TestStrGlobal("house_statuette_slot1")) {
      SetGlobal("house_statuette_slot1","statuette_symbol_blue1");
   }
   if (!TestStrGlobal("house_statuette_slot2")) {
      SetGlobal("house_statuette_slot2","statuette_symbol_red2");
   }
   

   EMiniJeuTemplate::Preload();
}

/* Destructeur */
Scene_House_Livingroom_Table::~Scene_House_Livingroom_Table()
{
   XDELETE(_lpCounterInition);
}

void Scene_House_Livingroom_Table::Init()
{
   if (TaskResolved("task_island_universeresolved")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("GLOBAL_ASHLEY_FINISHED"), "", true);
   }
   
   // Livre posé ?
   if (TaskResolved("task_house_livingroom_placebook")) {
      SetVisible("bookfull", true, true);
   }
   else {
      SetVisible("bookghost", true, true);
   }

   if (!TaskResolved("task_living_table_discover")) {
      ResolveTask("task_living_table_discover");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_GHOSTSTAT"), "", true, false);
   }
   
   if (getAdditionalName() == "completed") {
      ResolveTask("task_house_livingroom_repairstatuette");
      _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow_symbol1", true);
      _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "creeping_glow_symbol2", true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_MISSEYE"), "", true, true);
      _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
      AddTask("task_house_livingroom_lefteye");
      // Pas de return, l'init se continue pour afficher la statuette
   }
   
   // Statuette reconstruite ?
   if (!TaskResolved("task_house_livingroom_repairstatuette") && TaskResolved("task_house_livingroom_placebook")) {
      SetVisible("statuetteghost", true, true);
      // Affichage des différentes parties
      if (TaskResolved("task_house_statuette_part_legs")) {
         SetVisible("house_statuette_part_legs", true, true);
      }
      if (TaskResolved("task_house_statuette_part_legs")) {
         SetVisible("house_statuette_part_legs", true, true);
      }
      if (TaskResolved("task_house_statuette_part_chest")) {
         SetVisible("house_statuette_part_chest", true, true);
      }
      else if (TaskResolved("task_house_statuette_wood") && !TaskResolved("task_house_livingroom_statuette_part_flame")) {
         SetVisible("house_statuette_wood", true, true);
      }
      else if (TaskResolved("task_house_statuette_wood") && TaskResolved("task_house_livingroom_statuette_part_flame")) {
         SetVisible("house_statuette_wood", true, true);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "woodfire",true);
         _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "house_statuette_part_flame", true);
      }
      if (TaskResolved("task_house_statuette_part_jaw")) {
         SetVisible("house_statuette_part_jaw", true, true);
      }
      if (TaskResolved("task_house_statuette_part_head1")) {
         SetVisible("house_statuette_part_head1", true, true);
      }
      if (TaskResolved("task_house_statuette_part_head2")) {
         SetVisible("house_statuette_part_head2", true, true);
      }
      if (TaskResolved("task_house_statuette_part_skull")) {
         SetVisible("house_statuette_part_skull", true, true);
      }
   }
   else if TaskResolved("task_house_livingroom_repairstatuette") {
      SetVisible("statuettefull", true, true);
      SetVisible("statuette_righteye_off", true, true);
      // Slots
      // Si le slot 1 contient quelque chose, on l'affiche
      SetVisible(GetStrGlobal("house_statuette_slot1",""), true);
      SetVisible(GetStrGlobal("house_statuette_slot2",""), true);
      
      // Oeil manquant
      if (!TaskResolved("task_house_livingroom_lefteye")) {
         SetVisible("emptyeye", true);
      }
      else {
         SetVisible("emptyeye", false);
         SetVisible("statuette_lefteye_off", true);
      }
   }
   
}

void Scene_House_Livingroom_Table::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6)) {
      ResolveTask("task_house_kapangya_effect1");
      ResolveTask("task_house_kapangya_effect2");
      ResolveTask("task_house_kapangya_effect3");
      ResolveTask("task_house_kapangya_effect4");
      AddTask("task_house_kapangya4effets");
      LaunchEffect();
   }
#endif
}

void Scene_House_Livingroom_Table::Logic()
{
	EScene::Logic();
}

void Scene_House_Livingroom_Table::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   
   if (_bInition) {
      if (_lpSoundEarthQuake) {
         _lpSoundEarthQuake->setVolume(_lpCounterInition->getCurrentValue()*100);
      }
      if (TaskResolved("task_house_livingroom_lefteye"))
      {
         if (_bRaising) {
            if (_lpCounterInition->getCurrentValue() > 0.5) {
               _fRaising += 0.5;
            } else {
               _fRaising -= 0.5;
            }
            MoveStatuette(GetRandom(1.0, 5.0)*_lpCounterInition->getCurrentValue(), GetRandom(1.0, 5.0)*_lpCounterInition->getCurrentValue()-_fRaising);
         }
         else {
            MoveStatuette(GetRandom(1.0, 5.0)*_lpCounterInition->getCurrentValue(), GetRandom(1.0, 5.0)*_lpCounterInition->getCurrentValue());
         }
      }
      _lpCounterInition->move(fElapsed);
      if (_lpCounterInition->isCompleted()) {
         ESoundBank::getSound("drawer_close")->playSample();
         SetVisible("statuette_righteye_off", true);
         SetVisible("statuette_righteye_on", false);
         if (TaskResolved("task_house_livingroom_lefteye")) {
            SetVisible("statuette_lefteye_off", true);
            SetVisible("statuette_lefteye_on", false);
         }
         if (!TaskResolved("task_house_livingroom_lefteye")) {
            _lpSceneDirector->getSequencer()->VoiceOver(NULL, "Il ne se passe rien", "");
         }
         if (_lpSoundEarthQuake) {
            _lpSoundEarthQuake->stopSample();
            _lpSoundEarthQuake = NULL;
         }
         _bInition = false;
         if(_bRaising) _bRaising = false;
         RestoreStatuette();
      }
   }

   EScene::Draw();
}

void Scene_House_Livingroom_Table::RestoreStatuette()
{
   RestorePosition("statuettefull");
   RestorePosition("statuette_righteye_on");
   RestorePosition("statuette_lefteye_on");
   RestorePosition("statuette_symbol_blue1");
   RestorePosition("statuette_symbol_red1");
   RestorePosition("statuette_symbol_blue2");
   RestorePosition("statuette_symbol_red2");   
}

void Scene_House_Livingroom_Table::MoveStatuette(float fX, float fY)
{
   RestoreStatuette();
   MovePosition("statuettefull", fX, fY);
   MovePosition("statuette_righteye_on", fX, fY);
   MovePosition("statuette_lefteye_on", fX, fY);
   MovePosition("statuette_symbol_blue1", fX, fY);
   MovePosition("statuette_symbol_red1", fX, fY);
   MovePosition("statuette_symbol_blue2", fX, fY);
   MovePosition("statuette_symbol_red2", fX, fY);
}

void Scene_House_Livingroom_Table::Close()
{
   if (_lpSoundEarthQuake) {
      _lpSoundEarthQuake->stopSample();
   }
   ESoundBank::StopSample("woodfire");
}

// Effet de la statuette !
void Scene_House_Livingroom_Table::LaunchEffect()
{
   _lpSceneDirector->getSequencer()->Wait(NULL, 500);      

   typedef enum {
      STATUETTE_EFFECT_NONE = 0,
      STATUETTE_EFFECT_EARTHQUAKE,
      STATUETTE_EFFECT_LIGHT,
      STATUETTE_EFFECT_DARKNESS,
      STATUETTE_EFFECT_RAISING
   } StatuetteEffectType;
   StatuetteEffectType Effect = STATUETTE_EFFECT_NONE;
   
   // Tremblement de terre
   if (strcmp(GetStrGlobal("house_statuette_slot1",""), "statuette_symbol_blue1") == 0
       &&
       strcmp(GetStrGlobal("house_statuette_slot2",""), "statuette_symbol_blue2") == 0
       ) {
      Effect = STATUETTE_EFFECT_EARTHQUAKE;
   }
   // Lumière
   if (strcmp(GetStrGlobal("house_statuette_slot1",""), "statuette_symbol_blue1") == 0
       &&
       strcmp(GetStrGlobal("house_statuette_slot2",""), "statuette_symbol_red2") == 0
       ) {
      Effect = STATUETTE_EFFECT_LIGHT;
   }
   // Obscurité
   if (strcmp(GetStrGlobal("house_statuette_slot1",""), "statuette_symbol_red1") == 0
       &&
       strcmp(GetStrGlobal("house_statuette_slot2",""), "statuette_symbol_blue2") == 0
       ) {
      Effect = STATUETTE_EFFECT_DARKNESS;
   }
   // Soulèvement
   if (strcmp(GetStrGlobal("house_statuette_slot1",""), "statuette_symbol_red1") == 0
       &&
       strcmp(GetStrGlobal("house_statuette_slot2",""), "statuette_symbol_red2") == 0
       ) {
      Effect = STATUETTE_EFFECT_RAISING;
   }

   if (Effect == STATUETTE_EFFECT_NONE) {
	   _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_INCOMPLETE"), "", true);
	   return;
   }

   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
   // Effet
   switch (Effect) {
      case STATUETTE_EFFECT_LIGHT:
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "inca_light");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "light", true);      
         _lpSceneDirector->getSequencer()->Wait(NULL, 2000);      
         _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("HOUSE_LIVINGROOM_EFFECT2DONE"), "");
         if (TaskResolved("task_house_kapangya_effect1")) {
            ResolveTask("task_house_kapangya_effect2");
         }
         break;
      case STATUETTE_EFFECT_EARTHQUAKE:
         _lpSceneDirector->getSequencer()->EarthQuake(NULL, true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 2000);      
         _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("HOUSE_LIVINGROOM_EFFECT3DONE"), "");
         if (TaskResolved("task_house_kapangya_effect2")) {
            ResolveTask("task_house_kapangya_effect3");
         }
         break;
      case STATUETTE_EFFECT_DARKNESS:
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "inca_eclipse");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "darkness", true);      
         _lpSceneDirector->getSequencer()->Wait(NULL, 2000);      
         _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("HOUSE_LIVINGROOM_EFFECT4DONE"), "");
         if (TaskResolved("task_house_kapangya_effect3")) {
            ResolveTask("task_house_kapangya_effect4");
         }
         break;
      case STATUETTE_EFFECT_RAISING:
         _bRaising = true;
         _fRaising = 0;
         _lpSceneDirector->getSequencer()->Wait(NULL, 2000);      
         _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("HOUSE_LIVINGROOM_EFFECT1DONE"), "");
         ResolveTask("task_house_kapangya_effect1");
			 break;
		 default:
			 break;
   }
   // Fin de l'effet
   switch (Effect) {
		 case STATUETTE_EFFECT_LIGHT:
			 _lpSceneDirector->getSequencer()->Wait(NULL, 1000);      
			 _lpSceneDirector->getSequencer()->ShowImage(NULL, "light", false);
			 break;
		 case STATUETTE_EFFECT_EARTHQUAKE:
			 _lpSceneDirector->getSequencer()->Wait(NULL, 1000);      
			 _lpSceneDirector->getSequencer()->EarthQuake(NULL, false);
			 break;
		 case STATUETTE_EFFECT_DARKNESS:
			 _lpSceneDirector->getSequencer()->Wait(NULL, 1000);      
			 _lpSceneDirector->getSequencer()->ShowImage(NULL, "darkness", false);
			 break;
		 case STATUETTE_EFFECT_RAISING:
			 _lpSceneDirector->getSequencer()->Wait(NULL, 1000);      
			 break;
		 default:
			 break;
   }
   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
   _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_livingroom", "");
}

bool Scene_House_Livingroom_Table::ObjectClicked(const char *szObjectName, float x, float y)
{
	if (_bInition) {
		return true;
	}
   
   // Livre
   if (strcmp(szObjectName, "bookfull") == 0) {
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_incabook", "", false);
      return true;
   }
   
	// Mécanisme de la statuette Inca
	if (strcmp(szObjectName, "statuette_righteye_off") == 0 || strcmp(szObjectName, "statuette_lefteye_off") == 0)
	{
		// Allume la statuette
		_bInition = true;
		// Si il y a tous les symboles et que tout est prêt pour un effet, ça dure plus longtemps
		if (TaskResolved("task_house_livingroom_lefteye")) {
				_lpCounterInition->startCounter(1.0f, 0.0f, 0, 10000, K_COUNTER_LINEAR);
		}
		else {
			_lpCounterInition->startCounter(1.0f, 0.0f, 0, 2000, K_COUNTER_LINEAR);
		}
		ESoundBank::getSound("drawer_open")->playSample();
		SetVisible("statuette_righteye_on", true);
		SetVisible("statuette_righteye_off", false);
		if (TaskResolved("task_house_livingroom_lefteye")) {
			SetVisible("statuette_lefteye_on", true);
			SetVisible("statuette_lefteye_off", false);
		}
		if (TaskResolved("task_house_livingroom_lefteye"))
		{
			_lpSoundEarthQuake = ESoundBank::getSound("earthquake");
			if (_lpSoundEarthQuake)
				_lpSoundEarthQuake->playSample();
		}
		// On lance l'effet si il y a les 3 symboles
		if (TaskResolved("task_house_livingroom_lefteye")) {
			LaunchEffect();
		}
		return true;
	}

	// Blue
	if (strcmp(szObjectName, "statuette_symbol_blue1") == 0) {
		ESoundBank::getSound("kapangya_symbol")->playSample();
		SetVisible(GetStrGlobal("house_statuette_slot1",""), false);
		SetGlobal("house_statuette_slot1","statuette_symbol_red1");
		SetVisible(GetStrGlobal("house_statuette_slot1","statuette_symbol_red1"), true);
	}
	if (strcmp(szObjectName, "statuette_symbol_blue2") == 0) {
		ESoundBank::getSound("kapangya_symbol")->playSample();
		SetVisible(GetStrGlobal("house_statuette_slot2",""), false);
		SetGlobal("house_statuette_slot2","statuette_symbol_red2");
		SetVisible(GetStrGlobal("house_statuette_slot2",""), true);
	}
	// Red
	if (strcmp(szObjectName, "statuette_symbol_red1") == 0) {
		ESoundBank::getSound("kapangya_symbol")->playSample();
		SetVisible(GetStrGlobal("house_statuette_slot1",""), false);
		SetGlobal("house_statuette_slot1","statuette_symbol_blue1");
		SetVisible(GetStrGlobal("house_statuette_slot1",""), true);
	}
	if (strcmp(szObjectName, "statuette_symbol_red2") == 0) {
		ESoundBank::getSound("kapangya_symbol")->playSample();
		SetVisible(GetStrGlobal("house_statuette_slot2",""), false);
		SetGlobal("house_statuette_slot2","statuette_symbol_blue2");
		SetVisible(GetStrGlobal("house_statuette_slot2",""), true);
	}
	return false;
}

bool Scene_House_Livingroom_Table::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Scene_House_Livingroom_Table::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (_bInition) {
      return true;
   }
   
   // Livre
   if (strcmp(szItemName, "inv_livingroom_book") == 0) {
      if (strcmp(szObjectName, "bookghost") == 0) {
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->SwitchImage(NULL, "bookghost", "bookfull");
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "statuetteghost", true, true);
         _lpSceneDirector->getSequencer()->Wait(NULL, 1000);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, false);
//         if (TestGlobal("__mode_casual__")) {
            _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_incabook", "discover", true);
//            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_BOOKHERE"),"", true, false); 
//         }
         ResolveTask("task_house_livingroom_placebook");
         AddTask("task_house_livingroom_repairstatuette");
         ResolveObjective("house","living_book");
         AddObjective("house","living_buildkapangya");
         AddHint("house","living_buildkapangya","how");
         return true;
      }
   }
    
   // Dépose d'objets sur la sillouette de la statuette
   if (strcmp(szObjectName, "statuetteghost") == 0) {
      if (!TaskResolved("task_house_livingroom_placebook")) {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_NEEDBOOK"),"", true, false); 
         return true;
      }
      // Base
      if (strcmp(szItemName, "inv_house_statuette_part_legs") == 0) {
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_statuette_part_legs", true);
         ResolveTask("task_house_statuette_part_legs");
         AddTask("task_house_livingroom_statuette_part_flame");
         return true;
      }
      if (TaskResolved("task_house_statuette_part_legs")) {
         // Brindilles sans fagot
         if (strcmp(szItemName, "inv_house_brushwood") == 0) {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_BRUSHWOOD"),"", true, false); 
            return true;
         }      
         // Brindilles en fagot
         if (strcmp(szItemName, "inv_house_brushwood_fagot") == 0) {
            _lpSceneDirector->DropItem(szItemName);
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_statuette_wood", true);
            ResolveTask("task_house_statuette_wood");
            return true;
         }      
      }
      // Enflamme le fagot de brindille
      // Zippo rempli
      if (TaskResolved("task_house_statuette_wood")) {
         if (strcmp(szItemName, "inv_house_zippo_full") == 0) {
            _lpSceneDirector->DropItem(szItemName);
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "zippo");
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "woodfire",true);
            _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "house_statuette_part_flame", true);
            ResolveTask("task_house_livingroom_statuette_part_flame");
            return true;
         }
         // Zippo vide
         if (strcmp(szItemName, "inv_house_zippo_empty") == 0) {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_ZIPEMPTY"),"", true, false); 
            return true;
         }
      }
      // Chest
      if (strcmp(szItemName, "inv_house_statuette_part_chest") == 0) {
         if (TaskResolved("task_house_livingroom_statuette_part_flame")) {
            _lpSceneDirector->DropItem(szItemName);
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "house_statuette_part_flame", false);
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_statuette_part_chest", true);
            ResolveTask("task_house_statuette_part_chest");
            ResolveObjective("house","living_buildbase");
            AddObjective("house","living_buildhead");
            if (!TaskResolved("task_house_livingroom_playpiano")) {
               AddHint("house","living_buildhead","jaw");
               AddHint("house","living_buildhead","piano");
            }
            AddHint("house","living_buildhead","eye");
            return true;
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_ORDERFLAME"),"", true, false); 
            return true;
         }
      }
      // Jaw
      if (strcmp(szItemName, "inv_house_statuette_part_jaw") == 0) {
         if (TaskResolved("task_house_statuette_part_chest")) {
            _lpSceneDirector->DropItem(szItemName);
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_statuette_part_jaw", true);
            ResolveTask("task_house_statuette_part_jaw");
            return true;
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_ORDER"),"", true, false); 
            return true;
         }
      }
      // Head 1
      if (strcmp(szItemName, "inv_house_statuette_part_head1") == 0) {
         if (TaskResolved("task_house_statuette_part_jaw")) {
            _lpSceneDirector->DropItem(szItemName);
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_statuette_part_head1", true);
            ResolveTask("task_house_statuette_part_head1");
            return true;
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_ORDER"),"", true, false); 
            return true;
         }
      }
      // Head 2
      if (strcmp(szItemName, "inv_house_statuette_part_head2") == 0) {
         if (TaskResolved("task_house_statuette_part_jaw")) {
            _lpSceneDirector->DropItem(szItemName);
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_statuette_part_head2", true);
            ResolveTask("task_house_statuette_part_head2");
            return true;
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_ORDER"),"", true, false); 
            return true;
         }
      }
      // Skull
      if (strcmp(szItemName, "inv_house_statuette_part_skull") == 0) {
         if (TaskResolved("task_house_statuette_part_head1") && TaskResolved("task_house_statuette_part_head2")) {
            _lpSceneDirector->DropItem(szItemName);
            _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "house_statuette_part_skull", true);
            ResolveTask("task_house_statuette_part_skull");
            setAdditionalName("completed");
            Init();
            return true;
         }
         else {
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_ASHLEY_ORDER"),"", true, false); 
            return true;
         }
      }
   }

   // Oeil manquant
   if (strcmp(szObjectName, "emptyeye") == 0) {
      if (strcmp(szItemName, "inv_statuette_eye") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         ESoundBank::getSound("drawer_close")->playSample();
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "statuette_lefteye_off", true, true);
         ResolveTask("task_house_livingroom_lefteye");
         RemoveHint("house","living_buildhead","eye");
         ResolveObjective("house","living_buildhead");
         ResolveObjective("house","living_buildkapangya");
         AddTask("task_house_kapangya4effets");
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->GoToScene(NULL, "house_livingroom", "ask4");
         return true;
      }
   }

   return false;
}

void Scene_House_Livingroom_Table::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "")==0 && bIsRevolved) {
   }
}
