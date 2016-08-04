/*
 *  Scene_House_Statuette.cpp
 *  enigma
 *
 *  Created by Rockford on 15/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Scene_House_Statuette.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuTemplate.h"

/* Constructeur */
Scene_House_Statuette::Scene_House_Statuette(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
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
Scene_House_Statuette::~Scene_House_Statuette()
{
   XDELETE(_lpCounterInition);
}

void Scene_House_Statuette::Init()
{
   // Slots
   // Si le slot 1 contient quelque chose, on l'affiche
   SetVisible(GetStrGlobal("house_statuette_slot1",""), true);
   SetVisible(GetStrGlobal("house_statuette_slot2",""), true);
   
   // Oeil manquant
   if (!TaskResolved("task_house_livingroom_lefteye")) {
      AddTask("task_house_livingroom_lefteye");
      SetVisible("emptyeye", true);
   }
   else {
      SetVisible("emptyeye", false);
      SetVisible("statuette_lefteye_off", true);
   }
}

void Scene_House_Statuette::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_House_Statuette::Logic()
{
	EScene::Logic();
}

void Scene_House_Statuette::Draw()
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

void Scene_House_Statuette::RestoreStatuette()
{
   RestorePosition("statuette");
   RestorePosition("statuette_righteye_on");
   RestorePosition("statuette_lefteye_on");
   RestorePosition("statuette_symbol_blue1");
   RestorePosition("statuette_symbol_red1");
   RestorePosition("statuette_symbol_blue2");
   RestorePosition("statuette_symbol_red2");   
}

void Scene_House_Statuette::MoveStatuette(float fX, float fY)
{
   RestoreStatuette();
   MovePosition("statuette", fX, fY);
   MovePosition("statuette_righteye_on", fX, fY);
   MovePosition("statuette_lefteye_on", fX, fY);
   MovePosition("statuette_symbol_blue1", fX, fY);
   MovePosition("statuette_symbol_red1", fX, fY);
   MovePosition("statuette_symbol_blue2", fX, fY);
   MovePosition("statuette_symbol_red2", fX, fY);
}

void Scene_House_Statuette::Close()
{
}

// Effet de la statuette !
void Scene_House_Statuette::LaunchEffect()
{
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
      SetGlobal("house_statuette_effectdone_1", "1");
   }
   // Lumière
   if (strcmp(GetStrGlobal("house_statuette_slot1",""), "statuette_symbol_blue1") == 0
       &&
       strcmp(GetStrGlobal("house_statuette_slot2",""), "statuette_symbol_red2") == 0
       ) {
      Effect = STATUETTE_EFFECT_LIGHT;
      SetGlobal("house_statuette_effectdone_2", "1");
   }
   // Obscurité
   if (strcmp(GetStrGlobal("house_statuette_slot1",""), "statuette_symbol_red1") == 0
       &&
       strcmp(GetStrGlobal("house_statuette_slot2",""), "statuette_symbol_blue2") == 0
       ) {
      Effect = STATUETTE_EFFECT_DARKNESS;
      SetGlobal("house_statuette_effectdone_3", "1");
   }
   // Soulèvement
   if (strcmp(GetStrGlobal("house_statuette_slot1",""), "statuette_symbol_red1") == 0
       &&
       strcmp(GetStrGlobal("house_statuette_slot2",""), "statuette_symbol_red2") == 0
       ) {
      Effect = STATUETTE_EFFECT_RAISING;
	  SetGlobal("house_statuette_effectdone_4", "1");
   }
   // Mémorise le dernier effet exécuté pour que le livre s'ouvre à la bonne page
   SetGlobal("house_statuette_lasteffet",itos(Effect).c_str());

   if (Effect == STATUETTE_EFFECT_NONE) {
	   _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("HOUSE_LIVINGROOM_INCOMPLETE"), "", true);
	   return;
   }

   _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_CINEMATIC, true);
   // Effet
   switch (Effect) {
		 case STATUETTE_EFFECT_LIGHT:
			 _lpSceneDirector->getSequencer()->ShowImage(NULL, "light", true);      
			 _lpSceneDirector->getSequencer()->Wait(NULL, 2000);      
			 break;
		 case STATUETTE_EFFECT_EARTHQUAKE:
			 _lpSceneDirector->getSequencer()->EarthQuake(NULL, true);
			 _lpSceneDirector->getSequencer()->Wait(NULL, 2000);      
			 break;
		 case STATUETTE_EFFECT_DARKNESS:
			 _lpSceneDirector->getSequencer()->ShowImage(NULL, "darkness", true);      
			 _lpSceneDirector->getSequencer()->Wait(NULL, 2000);      
			 break;
		 case STATUETTE_EFFECT_RAISING:
			 _bRaising = true;
			 _fRaising = 0;
			 _lpSceneDirector->getSequencer()->Wait(NULL, 2000);      
			 break;
		 default:
			 break;
   }
   // Vue du livre TODO
   _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "flake_book", true);
   _lpSceneDirector->getSequencer()->ShowImage(NULL, "book", true);
   _lpSceneDirector->getSequencer()->PlaySound(NULL, "reveal");
   _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("HOUSE_LIVINGROOM_BOOKEFFECT"), "");
   _lpSceneDirector->getSequencer()->ShowEmitter(NULL, "flake_book", false);
   _lpSceneDirector->getSequencer()->ShowImage(NULL, "book", false);
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
}

bool Scene_House_Statuette::ObjectClicked(char *szObjectName, float x, float y)
{
	if (_bInition) {
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
		SetVisible(GetStrGlobal("house_statuette_slot1",""), false);
		SetGlobal("house_statuette_slot1","statuette_symbol_red1");
		SetVisible(GetStrGlobal("house_statuette_slot1","statuette_symbol_red1"), true);
	}
	if (strcmp(szObjectName, "statuette_symbol_blue2") == 0) {
		SetVisible(GetStrGlobal("house_statuette_slot2",""), false);
		SetGlobal("house_statuette_slot2","statuette_symbol_red2");
		SetVisible(GetStrGlobal("house_statuette_slot2",""), true);
	}
	// Red
	if (strcmp(szObjectName, "statuette_symbol_red1") == 0) {
		SetVisible(GetStrGlobal("house_statuette_slot1",""), false);
		SetGlobal("house_statuette_slot1","statuette_symbol_blue1");
		SetVisible(GetStrGlobal("house_statuette_slot1",""), true);
	}
	if (strcmp(szObjectName, "statuette_symbol_red2") == 0) {
		SetVisible(GetStrGlobal("house_statuette_slot2",""), false);
		SetGlobal("house_statuette_slot2","statuette_symbol_blue2");
		SetVisible(GetStrGlobal("house_statuette_slot2",""), true);
	}
	return false;
}

bool Scene_House_Statuette::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Scene_House_Statuette::ItemUsed(const char *szItemName, const char *szObjectName)
{
   if (_bInition) {
      return true;
   }
   // Oeil manquant
   if (strcmp(szObjectName, "emptyeye") == 0) {
      if (strcmp(szItemName, "inv_statuette_eye") == 0) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "success");
         ESoundBank::getSound("drawer_close")->playSample();
         ResolveTask("task_house_livingroom_lefteye");
         AddTask("task_house_incabookcompleted");
         _lpSceneDirector->DropItem(szItemName);
         Init();
         return true;
      }
   }

   return false;
}

void Scene_House_Statuette::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "")==0 && bIsRevolved) {
   }
}