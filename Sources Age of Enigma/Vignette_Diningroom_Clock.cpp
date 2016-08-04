/*
 *  Vignette_Diningroom_Clock.cpp
 *  enigma
 *
 *  Created by Rockford on 11/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Vignette_Diningroom_Clock.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"

/* Constructeur */
Vignette_Diningroom_Clock::Vignette_Diningroom_Clock(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
}

/* Destructeur */
Vignette_Diningroom_Clock::~Vignette_Diningroom_Clock()
{
   _nBigHand = 5;
   _nSmallHand = 1;
}

void Vignette_Diningroom_Clock::Init()
{
   SetVisible("p1", true);
   _nSmallHand = 1;
   SetVisible("g5", true);
   _nBigHand = 5;
   
   // Remise en état des tiroirs
   if (TaskResolved("task_house_dining_seconddrawer")) {
      SetVisible("diningclock_drawer2", true);
   }
   else
   if (TaskResolved("task_house_dining_firstdrawer")) {
      SetVisible("diningclock_drawer1", true);
   }
   
   // Remise en place des objets
   if (!TestGlobal("diningclock_pagepiece") && TaskResolved("task_house_dining_firstdrawer")) {
      SetVisible("diningclock_pagepiece", true);
   }
   if (!TestGlobal("diningclock_mastpiece") && TaskResolved("task_house_dining_seconddrawer")) {
      SetVisible("diningclock_mastpiece", true);
   }
   
}

void Vignette_Diningroom_Clock::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Vignette_Diningroom_Clock::Logic()
{
	EScene::Logic();
}

void Vignette_Diningroom_Clock::Draw()
{
   EScene::Draw();
}

void Vignette_Diningroom_Clock::Close()
{
}

bool Vignette_Diningroom_Clock::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Si le bout de papier est visible et que le joueur clique sur autre chose, on lui rappelle qu'il doit le ramasser
   if (isVisible("diningclock_pagepiece") && strcmp(szObjectName, "diningclock_pagepiece") != 0) {
      _lpSceneDirector->getSequencer()->Talk("dontknowclock", CHARACTER_POSX, CHARACTER_POSY, KStr("DININGCLOCK_ASHLEY_PAPERFIRST"), "", false);
      return true;
   }
   // Si le bout de mat est visible et que le joueur clique sur autre chose, on lui rappelle qu'il doit le ramasser
   if (isVisible("diningclock_mastpiece") && strcmp(szObjectName, "diningclock_mastpiece") != 0) {
      _lpSceneDirector->getSequencer()->Talk("dontknowclock", CHARACTER_POSX, CHARACTER_POSY, KStr("DININGCLOCK_ASHLEY_WOODFIRST"), "", false);
      return true;
   }
   if (strcmp(szObjectName, "close") == 0)
   {
      _lpSceneDirector->CloseVignette();
   }
   if (strcmp(szObjectName, "diningclock_clock") == 0)
   {
      _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("DININGCLOCK_ASHLEY_STRANGE"), "");
   }
   if (strcmp(szObjectName, "diningclock_leftbutton") == 0)
   {
      _nSmallHand++;
      if (_nSmallHand > 8) _nSmallHand= 1;
      _lpSceneDirector->getSequencer()->PlaySound(NULL,"mg_clic");
      DisplayHands(1);
   }
   if (strcmp(szObjectName, "diningclock_rightbutton") == 0)
   {
      _nBigHand++;
      if (_nBigHand > 8) _nBigHand= 1;
      _lpSceneDirector->getSequencer()->PlaySound(NULL,"mg_clic");
      DisplayHands(2);
   }
   
   // Objets à ramasser
   // Page
   if ( strcmp(szObjectName, "diningclock_pagepiece") == 0 )
   {
      PickupSimple(szObjectName,"inv_dining_page");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGCLOCK_ASHLEY_NUM13"), "", false);
      AddTask("task_house_dining_openbook");
   }
   // Mat
   if ( strcmp(szObjectName, "diningclock_mastpiece") == 0 )
   {
      PickupSimple(szObjectName,"inv_dining_mast");
      ResolveTask("task_house_livingroom_getmast");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("DININGCLOCK_ASHLEY_MAST"), "", false);
      AddTask("task_house_dining_repairboat");
   }
   
   return false;
}

void Vignette_Diningroom_Clock::DisplayHands(int mode)
{
   std::string str1, str2;
   if (!TaskResolved("task_house_dining_cleanpicture")) {
      _lpSceneDirector->getSequencer()->Talk("dontknowclock", CHARACTER_POSX, CHARACTER_POSY, KStr("DININGCLOCK_ASHLEY_DONTKNOW"), "", false);
      _nBigHand = 5;
      _nSmallHand = 1;
      return;
   }
   if (TaskResolved("task_house_dining_firstdrawer") && !TaskToResolve("task_house_dining_seconddrawer")) {
      _lpSceneDirector->getSequencer()->Talk("dontknowclock", CHARACTER_POSX, CHARACTER_POSY, KStr("DININGCLOCK_ASHLEY_2ND"), "", false);
      _nBigHand = 5;
      _nSmallHand = 1;
      return;
   }
   
   if (mode == 1) {
      SetVisible("p1", false);
      SetVisible("p2", false);
      SetVisible("p3", false);
      SetVisible("p4", false);
      SetVisible("p5", false);
      SetVisible("p6", false);
      SetVisible("p7", false);
      SetVisible("p8", false);
      str1 = "p";
      str1 = str1 + itos(_nSmallHand);
      SetVisible(str1.c_str(), true);
   }
   else {
      SetVisible("g1", false);
      SetVisible("g2", false);
      SetVisible("g3", false);
      SetVisible("g4", false);
      SetVisible("g5", false);
      SetVisible("g6", false);
      SetVisible("g7", false);
      SetVisible("g8", false);      
      str2 = "g";
      str2 = str2 + itos(_nBigHand);
      SetVisible(str2.c_str(), true);
   }
   // Test si le joueur a mis à l'heure
   if (TaskToResolve("task_house_dining_firstdrawer")) {
      if (_nBigHand == 8 && _nSmallHand == 5) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"drawer_open");
         _lpSceneDirector->getSequencer()->ShowImage(NULL,"diningclock_drawer1", true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL,"diningclock_pagepiece", true);
         ResolveTask("task_house_dining_firstdrawer");
         RemoveHint("house","treasure","firstdrawer");
         AddHint("house","treasure","seconddrawer");
      }
   }
   if (TaskToResolve("task_house_dining_seconddrawer")) {
      if (_nBigHand == 1 && _nSmallHand == 3) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"drawer_open");
         _lpSceneDirector->getSequencer()->ShowImage(NULL,"diningclock_drawer1", false);
         _lpSceneDirector->getSequencer()->ShowImage(NULL,"diningclock_drawer2", true);
         _lpSceneDirector->getSequencer()->ShowImage(NULL,"diningclock_mastpiece", true);
         ResolveTask("task_house_dining_seconddrawer");
         RemoveHint("house","treasure","seconddrawer");
         AddTask("task_house_dining_repairboat");
         AddObjective("house","repairboat");
         AddHint("house","repairboat","how");
      }
   }
}

bool Vignette_Diningroom_Clock::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}