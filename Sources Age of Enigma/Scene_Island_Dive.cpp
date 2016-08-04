/*
 *  SceneFondMarin.cpp
 *
 *  Created by Rockford on 19/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Island_Dive.h"
#include "ESceneDirector.h"
#include "SoundBank.h"
#include "MyGame.h"
#include <string>
#include "EMiniJeuTemplate.h"
#include "GlobalBank.h"

SceneFondMarin::SceneFondMarin(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _bDiveEnd = false;
   _lpAnimTempoBenitier = new KCounter;
   _lpAnimApnee = new KCounter;
   /* Load font */
   
   EMiniJeuTemplate::Preload();
}

SceneFondMarin::~SceneFondMarin()
{
   XDELETE(_lpAnimTempoBenitier);
   XDELETE(_lpAnimApnee);
   XDELETE(_lpFont);
}

void SceneFondMarin::Init()
{
   _lpFont = EFontBank::getFont("NITECLUB.TTF",200);

   // 1ère visite
   if (!TaskResolved("task_island_dive_visit")) {
      ResolveTask("task_island_dive_visit");
      AddObjective("island","key");
      AddHint("island","key","how");
   }
   
   // Bénitier fermé à l'arrivée
   if (!TaskResolved("task_island_clamwood")) {
      AddTask("task_island_clamwood");
      _bBenitierOpen = false;
      SetVisible("benitieropen", false);
      SetVisible("benitierclose", true);
   }
   else {
      _bBenitierOpen = true;
      SetVisible("benitieropen", true);
      SetVisible("woodin", true);
      SetVisible("benitierclose", false);
      if (TaskResolved("task_island_getkey")) {
         SetVisible("dive_pearl", true);
      }
      else {
         SetVisible("dive_key", true);
      }
   }
   
   SetupItem("island_shovelhead");
   
   // Le baton au sol
   if (!TestGlobal("woodout")) {
      SetVisible("woodout", true);
   }
   else {
      SetVisible("woodout", false);
   }
   
//   StartAnimation("sharkanim");
   StartAnimation("fishbanc1P2P");
//   StartAnimation("wavesanim");
   StartAnimation("fish1animrot");
   StartAnimation("fish1animp2p");
   _lpSndDive = ESoundBank::getSound("island_dive");
   _lpSndDive->playSample();
   
   // Animation ouverture / fermeture du bénitier
   _lpAnimTempoBenitier->startCounter(0, 1, 0, 5000, K_COUNTER_LINEAR);
   
   // 30 secondes d'apnée !!
   _lpAnimApnee->startCounter(30, 0, 0, 30000, K_COUNTER_LINEAR);
   
   // Musique apnée
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_DIVE30);
   
   if (TaskResolved("task_island_getkey") && TaskResolved("island_shovelhead")) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIVE_MURRAY_DONE") ,"",true);
   }

}

void SceneFondMarin::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void SceneFondMarin::Logic()
{
	EScene::Logic();
}

void SceneFondMarin::Draw()
{
   EScene::Draw();
   
   // Anims
   // On n'avance que si aucun mini jeu n'est affiché
   if (_lpSceneDirector->GetCurrentMinigame() == NULL) {
      double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
      _lpAnimApnee->move(fElapsed);
      _lpAnimTempoBenitier->move(fElapsed);
   }
   // Si l'anim du bénitier est finie, on inverse l'état du bénitier et on reprend
   if (!TaskResolved("task_island_clamwood"))
   {
      if (_lpAnimTempoBenitier->isCompleted()) {
         if (_bBenitierOpen ) {
            _bBenitierOpen = false;
            SetVisible("benitieropen", false);
            SetVisible("dive_key", false);
            SetVisible("dive_pearl", false);
            SetVisible("benitierclose", true);
            StopEmitter("bubbles");
         }
         else {
            _bBenitierOpen = true;
            SetVisible("benitieropen", true);
            if (TaskResolved("task_island_getkey")) {
               SetVisible("dive_pearl", true);
            }
            else {
               SetVisible("dive_key", true);
            }
            SetVisible("benitierclose", false);
            StartEmitter("bubbles");
         }    
         _lpAnimTempoBenitier->startCounter(0, 30, 0, 5000, K_COUNTER_LINEAR);
      }
   }
   // Si l'apnée est finit, on sort !
   if (_lpAnimApnee->isCompleted() && _bDiveEnd == false)
   {
      _bDiveEnd = true;
      _lpSceneDirector->PlayDirectorSound("diveend");
      _lpSceneDirector->GoToScene("island_beach");
   }
   
   // Affiche le temps restant d'Apnée
   std::string str;
   str = itos(int(_lpAnimApnee->getCurrentValue()));
   if (int(_lpAnimApnee->getCurrentValue()) < 10) {
      _lpFont->drawStringCentered(str.c_str(), 750, 1024, 20);
   } else {
      std::string u = str.substr(1,1);
      str = str.substr(0,1);
      float cx = (1024-750)/2 + 750;
      _lpFont->drawStringFromRight(str.c_str(),cx, 20);
      _lpFont->drawStringFromLeft(u.c_str(),cx, 20);
   }
}

void SceneFondMarin::Close()
{
   _lpSndDive->stopSample();
}

bool SceneFondMarin::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "benitieropen") == 0 || strcmp(szObjectName, "benitierclose") == 0)
   {
      if (_bBenitierOpen) {
         // Referme le bénitier
         _lpAnimTempoBenitier->move(99999);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIVE_MURRAY_WHY"), "", true);
         return true;
      }
   }
   if (strcmp(szObjectName, "dive_key") == 0)
   {
      // Referme le bénitier
      _lpAnimTempoBenitier->move(99999);
      // TODO:Animer 2 bouts de baton ?
      if (TaskResolved("task_island_clamwood")) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "brokenstick");
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIVE_MURRAY_STICKNOT"), "", true);
         // On annule la tâche
         ResetTask("task_island_clamwood");
         SetVisible("woodin", false);
         SetVisible("woodout", true);
         SetGlobal("woodout","0");  // Remet l'objet au sol
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIVE_MURRAY_WHY"), "", true);
      }

      return true;
   } 

   if (strcmp(szObjectName, "woodout") == 0)
   {
      PickupSimple(szObjectName, "inv_island_wood");
      return true;
   }

   if (strcmp(szObjectName, "island_shovelhead") == 0)
   {
      PickupMultiple(szObjectName, "inv_island_shovelhead",-1);
      return true;
   }
   
   if (strcmp(szObjectName, "woodin") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIVE_MURRAY_STICK"), "", true);
      return true;
   }

   if (strcmp(szObjectName, "dive_pearl") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIVE_MURRAY_PEARL"), "", true);
      return true;
   }

   _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIVE_MURRAY_BLOUP"), "", true);
   return false;
}

bool SceneFondMarin::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool SceneFondMarin::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur utilise la perle dans le bénitier ouvert
   if ( strcmp(szItemName, "inv_island_pearl") == 0 ) {
      if (_lpAnimApnee->getCurrentValue() > 3.0f) {
         if ((strcmp(szObjectName, "dive_key") == 0 || strcmp(szObjectName, "benitieropen") == 0) && TaskResolved("task_island_clamwood"))
         {
            // On peut maintenant retirer l'objet de l'inventaire
            _lpSceneDirector->DropItem("inv_pearl");
            // On a obtenu la clé !
            ESoundBank::getSound("success")->playSample();
            int x, y;
            GetObjectPosition("dive_key", x, y, false, false);
            _lpSceneDirector->getSequencer()->ShowImage(NULL,"dive_key", false);
            _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_island_key", (float)x, (float)y, 1);
            _lpSceneDirector->getSequencer()->ShowImage(NULL, "dive_pearl", true);
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIVE_MURRAY_SUCCESS") ,"",true);
            ResolveTask("task_island_getkey");
            ResolveObjective("island","key");
            _lpSceneDirector->DropItem("inv_island_pearl");
            return true;
         } else if (strcmp(szObjectName, "dive_key") == 0 && !TaskResolved("task_island_clamwood"))
         {
            // Referme le bénitier
            _lpAnimTempoBenitier->move(99999);
            // TODO:Animer 2 bouts de baton ?
            _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIVE_MURRAY_TOFAST"), "", true);
            return true;
         }
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIVE_MURRAY_TOOLATE"), "", true);
      }
   }
   // Le joueur utilise le bout de bois sur l'intérieur du bénitier
   if ( strcmp(szItemName, "inv_island_wood") == 0 ) {
      if (strcmp(szObjectName, "benitieropen") == 0 || strcmp(szObjectName, "dive_key") == 0)
      {
         ESoundBank::getSound("success")->playSample();
         ResolveTask("task_island_clamwood");
         AddTask("task_island_getkey");
         SetVisible("woodin", true);
         _lpSceneDirector->DropItem("inv_island_wood");
         return true;
      }
      if (strcmp(szObjectName, "benitierclose") == 0)
      {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_DIVE_MURRAY_MISSED") ,"",true);
         return true;
      }
   }
   return false;
}

void SceneFondMarin::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
}
