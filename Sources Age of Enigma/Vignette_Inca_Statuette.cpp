/* vignette_inca_statuettebg
 *  Vignette_Inca_Statuette.cpp
 *  enigma
 *
 *  Created by Rockford on 15/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Vignette_Inca_Statuette.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuTemplate.h"

/* Constructeur */
Vignette_Inca_Statuette::Vignette_Inca_Statuette(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   if (!TestStrGlobal("inca_statuette_slot1")) {
      SetGlobal("inca_statuette_slot1","inca_statuette_symbol_blue1");
   }
   if (!TestStrGlobal("inca_statuette_slot2")) {
      SetGlobal("inca_statuette_slot2","inca_statuette_symbol_red2");
   }
   
   EMiniJeuTemplate::Preload();
}

/* Destructeur */
Vignette_Inca_Statuette::~Vignette_Inca_Statuette()
{
}

void Vignette_Inca_Statuette::Init()
{
   SetVisible(GetStrGlobal("inca_statuette_slot1",""), true);
   SetVisible(GetStrGlobal("inca_statuette_slot2",""), true);  
}

void Vignette_Inca_Statuette::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Vignette_Inca_Statuette::Logic()
{
	EScene::Logic();
}

void Vignette_Inca_Statuette::Draw()
{
   EScene::Draw();
}

void Vignette_Inca_Statuette::Close()
{
}

bool Vignette_Inca_Statuette::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "back") == 0)
   {
      _lpSceneDirector->CloseVignette();
   }

   // Blue
   if (strcmp(szObjectName, "inca_statuette_symbol_blue1") == 0) {
		ESoundBank::getSound("kapangya_symbol")->playSample();
      SetVisible(GetStrGlobal("inca_statuette_slot1",""), false);
      SetGlobal("inca_statuette_slot1","inca_statuette_symbol_red1");
      SetVisible(GetStrGlobal("inca_statuette_slot1","inca_statuette_symbol_red1"), true);
      return true;
   }
   if (strcmp(szObjectName, "inca_statuette_symbol_blue2") == 0) {
		ESoundBank::getSound("kapangya_symbol")->playSample();
      SetVisible(GetStrGlobal("inca_statuette_slot2",""), false);
      SetGlobal("inca_statuette_slot2","inca_statuette_symbol_red2");
      SetVisible(GetStrGlobal("inca_statuette_slot2",""), true);
      return true;
   }
   // Red
   if (strcmp(szObjectName, "inca_statuette_symbol_red1") == 0) {
		ESoundBank::getSound("kapangya_symbol")->playSample();
      SetVisible(GetStrGlobal("inca_statuette_slot1",""), false);
      SetGlobal("inca_statuette_slot1","inca_statuette_symbol_blue1");
      SetVisible(GetStrGlobal("inca_statuette_slot1",""), true);
      return true;
   }
   if (strcmp(szObjectName, "inca_statuette_symbol_red2") == 0) {
		ESoundBank::getSound("kapangya_symbol")->playSample();
      SetVisible(GetStrGlobal("inca_statuette_slot2",""), false);
      SetGlobal("inca_statuette_slot2","inca_statuette_symbol_blue2");
      SetVisible(GetStrGlobal("inca_statuette_slot2",""), true);
      return true;
   }
   
   if (strcmp(szObjectName, "inca_statuette_eye_zone") != 0) {
	   return false;
   }

   ESoundBank::getSound("drawer_open")->playSample();
   
   // On informe la scène appelante de l'effet utilisé et de l'object visé, via un appel à "ItemUsed"
   // Tremblement de terre
   if (strcmp(GetStrGlobal("inca_statuette_slot1",""), "inca_statuette_symbol_blue1") == 0
       &&
       strcmp(GetStrGlobal("inca_statuette_slot2",""), "inca_statuette_symbol_blue2") == 0
       ) {
      _lpSceneDirector->GetCurrentScene()->ItemUsed("statuette_effect_earthquake", getAdditionalName().c_str());
      _lpSceneDirector->CloseVignette();
      return true;
   }
   // Lumière
   if (strcmp(GetStrGlobal("inca_statuette_slot1",""), "inca_statuette_symbol_blue1") == 0
       &&
       strcmp(GetStrGlobal("inca_statuette_slot2",""), "inca_statuette_symbol_red2") == 0
       ) {
      _lpSceneDirector->GetCurrentScene()->ItemUsed("statuette_effect_light", getAdditionalName().c_str());
      _lpSceneDirector->CloseVignette();
      return true;
   }
   // Obscurité
   if (strcmp(GetStrGlobal("inca_statuette_slot1",""), "inca_statuette_symbol_red1") == 0
       &&
       strcmp(GetStrGlobal("inca_statuette_slot2",""), "inca_statuette_symbol_blue2") == 0
       ) {
      _lpSceneDirector->GetCurrentScene()->ItemUsed("statuette_effect_dark", getAdditionalName().c_str());
      _lpSceneDirector->CloseVignette();
      return true;
   }
   // Soulèvement
   if (strcmp(GetStrGlobal("inca_statuette_slot1",""), "inca_statuette_symbol_red1") == 0
       &&
       strcmp(GetStrGlobal("inca_statuette_slot2",""), "inca_statuette_symbol_red2") == 0
       ) {
      _lpSceneDirector->GetCurrentScene()->ItemUsed("statuette_effect_raise", getAdditionalName().c_str());
      _lpSceneDirector->CloseVignette();
      return true;
   }

   return false;
}

bool Vignette_Inca_Statuette::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Vignette_Inca_Statuette::ItemUsed(const char *szItemName, const char *szObjectName)
{
   return false;
}

void Vignette_Inca_Statuette::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
}