/*
 *  Scene_Egypt_Maze.cpp
 *  enigma
 *
 *  Created by Rockford on 05/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Egypt_Maze.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "FontBank.h"

/* Constructeur */
Scene_Egypt_Maze::Scene_Egypt_Maze(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   _nRoomNumber = 1;
   _lpFont = EFontBank::getFont("jancient.ttf", 50);
}

/* Destructeur */
Scene_Egypt_Maze::~Scene_Egypt_Maze()
{
   delete _lpBgGraphic;
   delete _lpFont;
}

void Scene_Egypt_Maze::Init()
{
   // Numéro de salle reçu en paramètre
   if (getAdditionalName() != "") {
      _nRoomNumber = atoi(getAdditionalName().c_str());
      KAssert(_nRoomNumber > 0 && _nRoomNumber <= 9);
   }
      
   // Figurines au sol
   switch (_nRoomNumber) {
      case 1:
         SetupItem("egypt_maze_fig02");
         break;
      case 2:
         SetupItem("egypt_maze_fig01");
         SetVisible("bg02", true);
         break;
      case 3:
         SetupItem("egypt_maze_fig05");
         SetVisible("bg03", true);
         break;
      case 4:
         SetupItem("egypt_maze_fig03");
         SetVisible("bg04", true);
         break;
      case 5:
         SetupItem("egypt_maze_fig04");
         SetVisible("bg05", true);
         break;
      case 6:
         SetupItem("egypt_maze_fig06");
         SetVisible("bg06", true);
         break;
      case 7:
         SetupItem("egypt_maze_fig07");
         SetVisible("bg07", true);
         break;
      case 8:
         SetupItem("egypt_maze_fig08");
         SetVisible("bg08", true);
         break;
      case 9:
         SetupItem("egypt_maze_fig09");
         SetupItem("egypt_symbol_god07h");
         SetVisible("bg09", true);
         break;
      default:
         break;
   }
}

void Scene_Egypt_Maze::Check()
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

void Scene_Egypt_Maze::Logic()
{
	EScene::Logic();
}

void Scene_Egypt_Maze::Draw()
{
   EScene::Draw();
   
   char szRoom[256];
   memset(szRoom, 0, 256);
   strcpy(szRoom, KStr("EGYPT_MAZE_ROOM"));
   strcat(szRoom, " ");
   strcat(szRoom, itos(_nRoomNumber).c_str());
   strcat(szRoom, "/9");
   
   DrawFancyString(szRoom, _lpFont, 1024/2, 65, 1024, true, getFade());
}

void Scene_Egypt_Maze::Close()
{
}

bool Scene_Egypt_Maze::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Ramassage des statuettes
   if (strcmp(szObjectName, "egypt_maze_fig01") == 0) {
      PickupSimple(szObjectName, "inv_egypt_fig01");
   }
   else if (strcmp(szObjectName, "egypt_maze_fig02") == 0) {
      PickupSimple(szObjectName, "inv_egypt_fig02");
   }
   else if (strcmp(szObjectName, "egypt_maze_fig03") == 0) {
      PickupSimple(szObjectName, "inv_egypt_fig03");
   }
   else if (strcmp(szObjectName, "egypt_maze_fig04") == 0) {
      PickupSimple(szObjectName, "inv_egypt_fig04");
   }
   else if (strcmp(szObjectName, "egypt_maze_fig05") == 0) {
      PickupSimple(szObjectName, "inv_egypt_fig05");
   }
   else if (strcmp(szObjectName, "egypt_maze_fig06") == 0) {
      PickupSimple(szObjectName, "inv_egypt_fig06");
   }
   else if (strcmp(szObjectName, "egypt_maze_fig07") == 0) {
      PickupSimple(szObjectName, "inv_egypt_fig07");
   }
   else if (strcmp(szObjectName, "egypt_maze_fig08") == 0) {
      PickupSimple(szObjectName, "inv_egypt_fig08");
   }
   else if (strcmp(szObjectName, "egypt_maze_fig09") == 0) {
      PickupSimple(szObjectName, "inv_egypt_fig09");
   }

   if (strcmp(szObjectName, "egypt_symbol_god07h") == 0) {
      PickupSimple(szObjectName, "inv_egypt_god07");
   }
   
   _lpSceneDirector->setZoomin(true);
   if (strcmp(szObjectName, "egypt_maze_door1") == 0) {
      if (_nRoomNumber == 1) {
         _lpSceneDirector->GoToScene("egypt_maze", "2", true);
      }
      else if (_nRoomNumber == 4) {
         _lpSceneDirector->GoToScene("egypt_maze", "5", true);
      }
      else if (_nRoomNumber == 5) {
         _lpSceneDirector->GoToScene("egypt_maze", "6", true);
      }
      else if (_nRoomNumber == 8) {
         _lpSceneDirector->GoToScene("egypt_maze", "9", true);
      }
      else {
         _lpSceneDirector->GoToScene("egypt_mazeentry", "lost", true);
      }
   }
   else if (strcmp(szObjectName, "egypt_maze_door2") == 0) {
      if (_nRoomNumber == 2) {
         _lpSceneDirector->GoToScene("egypt_maze", "3", true);
      }
      else if (_nRoomNumber == 3) {
         _lpSceneDirector->GoToScene("egypt_maze", "4", true);
      }
      else if (_nRoomNumber == 9) {
         _lpSceneDirector->GoToScene("egypt_boatroom", "from_maze", true);
      }
      else {
         _lpSceneDirector->GoToScene("egypt_mazeentry", "lost", true);
      }
   }
   else if (strcmp(szObjectName, "egypt_maze_door3") == 0) {
      if (_nRoomNumber == 6) {
         _lpSceneDirector->GoToScene("egypt_maze", "7", true);
      }
      else if (_nRoomNumber == 7) {
         _lpSceneDirector->GoToScene("egypt_maze", "8", true);
      }
      else {
         _lpSceneDirector->GoToScene("egypt_mazeentry", "lost", true);
      }
   }
   return false;
}

bool Scene_Egypt_Maze::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Egypt_Maze::ItemUsed(const char *szItemName, const char *szObjectName)
{
   
   return false;
}

void Scene_Egypt_Maze::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
