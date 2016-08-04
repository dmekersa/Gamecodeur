/*
 *  ESceneDirector.cpp
 *  pngviewer
 *
 *  Created by Rockford on 12/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

/* Inclusion groupée des différentes instance de scènes possibles */
#include "ThreadLoader.h"
#include "Scene_Loading.h"
#include "Scene_Menu.h"
#include "Scene_Mode.h"
#include "SceneTest.h"
#include "SceneMinijeu.h"
#include "Scene_House_Out.h"
#include "Scene_House_Out_Car.h"
#include "Scene_House_Parvis.h"
#include "Scene_Hall.h"
#include "Scene_House_HallDoor.h"
#include "Scene_House_Hall.h"
#include "SceneCorridor.h"
#include "SceneStairsDown.h"
#include "Scene_House_LivingRoom.h"
#include "Scene_House_Livingroom_Table.h"
#include "Scene_House_Livingroom_Box.h"
#include "Scene_House_Piano.h"
#include "Scene_House_IncaBook.h"
#include "Scene_House_DiningRoom.h"
#include "Vignette_Cinematic.h"
#include "Vignette_Diningroom_Clock.h"
#include "Vignette_Diningroom_Boat.h"
#include "Vignette_Diningroom_Book.h"
#include "Vignette_Diningroom_Lib.h"
#include "Vignette_Diningroom_Picture.h"
#include "Scene_House_Kitchen.h"
#include "Scene_House_Keyhole.h"
#include "Scene_House_Basement.h"
#include "Scene_House_Basement_Grid.h"
#include "Scene_House_Closet.h"
#include "Scene_House_Firstfloor.h"
#include "Scene_House_Atticdoor.h"
#include "Scene_House_Attic_Pentacle.h"
#include "Scene_House_Room1.h"
#include "Scene_House_Room2.h"
#include "Scene_House_Room2_Bedside.h"
#include "Scene_House_Room2_Mirror.h"
#include "Scene_House_Bathroom.h"
#include "Scene_House_Bathroom_Tub.h"
#include "Scene_House_Bathroom_Cabinet.h"
#include "Scene_House_Attic.h"
#include "Scene_Island_Beach.h"
#include "Scene_Island_Falaise.h"
#include "Scene_Island_Dive.h"
#include "Scene_Island_Cave.h"
#include "Scene_Island_Creek.h"
#include "Vignette_Island_Safe.h"
#include "Scene_Japan_Mainscreen.h"
#include "Scene_Japan_Garden.h"
#include "Scene_Japan_Bedroom.h"
#include "Scene_Inca_Mainscreen.h"
#include "Scene_Inca_Passage.h"
#include "Scene_Inca_Stairs.h"
#include "Scene_Inca_Final.h"
#include "Scene_Inca_Autel.h"
#include "Vignette_Inca_Statuette.h"
#include "Scene_Middleage_Mainscreen.h"
#include "Scene_Middleage_Cloister.h"
#include "Scene_Middleage_Stairs.h"
#include "Scene_Middleage_Jail.h"
#include "Scene_Egypt_Land.h"
#include "Scene_Egypt_Shop.h"
#include "Scene_Egypt_Mazeentry.h"
#include "Scene_Egypt_Keyroom.h"
#include "Scene_Egypt_Keyroom_zoom.h"
#include "Scene_Egypt_Maze.h"
#include "Scene_Egypt_Boat.h"
#include "Scene_Egypt_Boatroom.h"
#include "Scene_Egypt_Tomb.h"
#include "Scene_Egypt_Tomb_Connect.h"
#include "Scene_Hell_Mainscreen.h"
#include "Scene_Hell_Vale.h"
#include "Scene_Hell_Styx.h"
#include "Scene_Hell_Door.h"
#include "Scene_Vortex.h"
#include "Scene_Title.h"
#include "Scene_Title_Splashscreen.h"
#include "Scene_Minigames.h"
#include "Scene_Video_Player.h"
#include "Scene_Teasing.h"
#include "Scene_Debug_David.h"
#include "Scene_Debug_Preload.h"

#include "Vignette_Hands.h"
#include "Vignette_Spirit.h"

#include "SoundBank.h"
#include "MusicBank.h"
#include "ImageBank.h"
#include "EmitterBank.h"
#include "FontBank.h"
#include "ESceneDirector.h"
#include "EScene.h"
#include "MyGame.h"
#include "EPointer.h"
#include "UserProfileMgt.h"
#include "TaskList.h"
#include "EBookCtrl.h"
#include "EMap.h"
#include "EFancyString.h"
#include "EFancyStringMultiline.h"
#include "EDiary.h"
#include "EWatchDog.h"
#include "global.h"

ESceneDirector *ESceneDirector::_lpSceneDirector = NULL;

/* Constructeur */
ESceneDirector::ESceneDirector()
{
	KAssert(_lpSceneDirector == NULL);
   _lpSceneDirector = this;
   
   _bOptionsRequest = false;
   _bPlayerRequest = false;
   _bNewPlayerRequest = false;
#ifdef ENIGMA_STATS
   _bDebugDisplay = false;
#endif
   _lpSceneCurrent = NULL;
   _strLastParsedSceneName = "";
   _bSceneNext = false;
   _bSceneZoomin = false;
   _bWalk = false;
   
   _lpVignetteCurrent = NULL;
   _bClosingVignette = false;
   
   _lpMiniGameCurrent = NULL;
   _bClosingMiniGame = false;
   _bWasCheckNotAllowed = false;
   _bSoftCursor = false;
   _bSystemCursor = false;

   memset(_szSceneNext, 0, 256);
   memset(_szSceneNextAdditionalName, 0, 256);
   memset(_szScenePrevious, 0, 256);
   memset(_szSceneCurrent, 0, 256);
   
   _bDialogMode = false;
   _bCinematicMode = false;
   _bLockMode = false;
   _bTaskManager = false;
   _bMapManager = false;
   _bLoading = false;
   _bResetTimeframe = false;
   hasEnterPseudo = false;
   
   _lpAnimEarthQuake = NULL;
   _lpEnergyParticlesManager = new EnergyParticlesManager();
      
   /* Music */
   _lpMusicArranger = new EMusicArranger();
   _nCurrentMainMusic = DIRECTOR_MUSIC_NONE;
   
   _lpBgGraphic = KPTK::createKGraphic ();
   
   /* IHM pictures */
   _bHexagram = false;
   _bHexagramVisible = false;
   KGraphic *lpGraphicIHM = EImageBank::getImage("ihm_main","png");
   _lpsimgIHM = new ESmartImage(lpGraphicIHM,0.0,(float)(768-lpGraphicIHM->getHeight()), 1, false, true, 250);
   /* Pendentif des âmes */
   lpGraphicIHM = EImageBank::getImage("ihm_pentacle","png");
   _lpsimgHexagramme = new ESmartImage(lpGraphicIHM, 1024.0f, (float)(768-lpGraphicIHM->getHeight()), 1, false, true, 250);
   /* Blue */
   lpGraphicIHM = EImageBank::getImage("ihm_pentacle_blue","png");
   _lpsimgHexagramme_blue = new ESmartImage(lpGraphicIHM, 1024.0f, (float)(768-lpGraphicIHM->getHeight()), 1, false, true, 250);
   _lpsimgHexagramme_blue->setHalo(true, 0.01);
   /* Purple */
   lpGraphicIHM = EImageBank::getImage("ihm_pentacle_purple","png");
   _lpsimgHexagramme_purple = new ESmartImage(lpGraphicIHM, 1024.0f, (float)(768-lpGraphicIHM->getHeight()), 1, false, true, 250);
   _lpsimgHexagramme_purple->setHalo(true, 0.01);
   /* Yellow */
   lpGraphicIHM = EImageBank::getImage("ihm_pentacle_yellow","png");
   _lpsimgHexagramme_yellow = new ESmartImage(lpGraphicIHM, 1024.0f, (float)(768-lpGraphicIHM->getHeight()), 1, false, true, 250);
   _lpsimgHexagramme_yellow->setHalo(true, 0.01);
   /* Red */
   lpGraphicIHM = EImageBank::getImage("ihm_pentacle_red","png");
   _lpsimgHexagramme_red = new ESmartImage(lpGraphicIHM, 1024.0f, (float)(768-lpGraphicIHM->getHeight()), 1, false, true, 250);
   _lpsimgHexagramme_red->setHalo(true, 0.01);
   /* Green */
   lpGraphicIHM = EImageBank::getImage("ihm_pentacle_green","png");
   _lpsimgHexagramme_green = new ESmartImage(lpGraphicIHM, 1024.0f, (float)(768-lpGraphicIHM->getHeight()), 1, false, true, 250);
   
   /* Mouse arrow pictures */
   _lpGraphicArrowPoint = EImageBank::getImage("arrow_point","png");
   _lpGraphicArrowWalk = EImageBank::getImage("arrow_walk","png");
   _lpGraphicArrowWalkBack = EImageBank::getImage("arrow_walkback","png");
   _lpGraphicArrowDoor = EImageBank::getImage("arrow_door","png");
   _lpGraphicArrowAction = EImageBank::getImage("arrow_action","png");
   _lpGraphicArrowLook = EImageBank::getImage("arrow_look","png");
   _lpGraphicArrowInfo = EImageBank::getImage("arrow_info","png");
   _lpGraphicArrowTalk = EImageBank::getImage("arrow_talk","png");
   _lpGraphicArrowTake = EImageBank::getImage("arrow_take","png");
   _lpGraphicArrowWait = EImageBank::getImage("arrow_wait","png");
   _lpGraphicArrowPuzzle = EImageBank::getImage("arrow_puzzle","png");
   
   /* Characters pictures */
   KGraphic *lpCharacterGraphic;
   lpCharacterGraphic = EImageBank::getImage("character_ihm_house");
   _lpsimgCharacterHouse = new ESmartImage(lpCharacterGraphic,19,612,1,false,true,500);
   lpCharacterGraphic = EImageBank::getImage("character_ihm_house_dialog");
   _lpsimgCharacterHouseDialog = new ESmartImage(lpCharacterGraphic,0,0,1,false,true,500);
   lpCharacterGraphic = EImageBank::getImage("character_ihm_universe_dialog");
   _lpsimgCharacterUniverseDialog = new ESmartImage(lpCharacterGraphic,0,0,1,false,true,500);
   lpCharacterGraphic = EImageBank::getImage("character_ihm_island");
   _lpsimgCharacterIsland = new ESmartImage(lpCharacterGraphic,0,608,1,false,true,500);
   lpCharacterGraphic = EImageBank::getImage("character_ihm_japan");
   _lpsimgCharacterJapan = new ESmartImage(lpCharacterGraphic,3,606,1,false,true,500);
   lpCharacterGraphic = EImageBank::getImage("character_ihm_inca");
   _lpsimgCharacterInca = new ESmartImage(lpCharacterGraphic,0,555,1,false,true,500);
   lpCharacterGraphic = EImageBank::getImage("character_ihm_middleage");
   _lpsimgCharacterMiddleage = new ESmartImage(lpCharacterGraphic,12,616,1,false,true,500);
   lpCharacterGraphic = EImageBank::getImage("character_ihm_egypt");
   _lpsimgCharacterEgypt = new ESmartImage(lpCharacterGraphic,11,623,1,false,true,500);
   lpCharacterGraphic = EImageBank::getImage("character_ihm_hell");
   _lpsimgCharacterHell = new ESmartImage(lpCharacterGraphic,26,634,1,false,true,500);
   lpCharacterGraphic = EImageBank::getImage("dialog_cache");
   _lpsimgDialogCache = new ESmartImage(lpCharacterGraphic,0,640,1,false,true,500);
   
   _lpsimgArrow = NULL;
   _cursor = DIRECTOR_MOUSECURSOR_NONE;
   PrepareMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
   
   _lpFontMouseOverInfo = EFontBank::getFont("tahoma.ttf", 12);
   
   _lpAnimSceneOut = new KCounter();
   
   /* Sequencer */
   _lpSceneSequencer = new ESceneSequencer(this);
   
   /* Inventaire */
   _lpInventory = new EInventory(this);
   _lpTravellingItem = NULL;
   _lpsimgTravellingItem = NULL;
   _lpHoldItem = NULL;
   _lpsimgHoldItem = NULL;
   _lpTravellingEmitter = new ParticleEmitter(0, 0);

   _lpTravellingEmitter->SetParticleBitmap("particule-etoile2");
   
   _lpTravellingEmitter->SetEmitRate(1);
   _lpTravellingEmitter->SetType(VFX_TRAIL);
   _lpTravellingEmitter->Stop();
   
   /* Hint System */
   _fHintLevel = 100.0f;
   
   /* Book contenant le taskList et le Diary */
   _lpBookCtrl = new EBookCtrl();
   
   // Map
   _lpMap = new EMap();
   // test utilisation map
   _lpMap->setBackgroundImg(EImageBank::getImage("map/house_map_house","png"));
   _lpMap->setImgPerso(EImageBank::getImage("map/house_map_cross","png"));
   
   _lpMap->addLayerImg(/*"Universe*/"inca", EImageBank::getImage("map/house_map_universe_inca_noVisited","png"), EImageBank::getImage("map/house_map_universe_inca","png"), 109, 346);
   _lpMap->addLayerImg(/*"Universe*/"japan", EImageBank::getImage("map/house_map_universe_japan_noVisited","png"), EImageBank::getImage("map/house_map_universe_japan","png"), 720, 206);
   _lpMap->addLayerImg(/*"Universe*/"middleage", EImageBank::getImage("map/house_map_universe_middleage_noVisited","png"), EImageBank::getImage("map/house_map_universe_middleage","png"), 88, 53);
   _lpMap->addLayerImg(/*"Universe*/"island", EImageBank::getImage("map/house_map_universe_island_noVisited","png"), EImageBank::getImage("map/house_map_universe_island","png"), 693, 484);
   _lpMap->addLayerImg(/*"Universe*/"egypt", EImageBank::getImage("map/house_map_universe_egypt_noVisited","png"), EImageBank::getImage("map/house_map_universe_egypt","png"), 298, 55);
   
   //_lpMap->addZone("Test1", 0, 0, 150, 50);
   
    // home
     _lpMap->addZone("house_livingroom", 335, 550, 160, 155);
     _lpMap->addZone("hall", 495, 488, 93, 215);
     _lpMap->addZone("house_diningroom", 585, 555, 180, 145);

     _lpMap->addZone("basement", 660, 450, 105, 105);
     _lpMap->addZone("kitchen", 588, 360, 175, 90);
     _lpMap->addZone("corridor", 495, 360, 95, 130);
     _lpMap->addZone("house_attic", 330, 405, 85, 138);
     _lpMap->addZone("house_atticdoor", 332, 330, 60, 50);
     _lpMap->addZone("firstfloor", 330, 290, 260, 40);
      _lpMap->addZone("closet", 420, 388, 75, 60);

     _lpMap->addZone("house_room2", 334, 183, 157, 110);
      _lpMap->addZone("bathroom", 490, 183, 100, 110);

     //island
      _lpMap->addZone("island_beach", 800, 617, 152, 80);
      _lpMap->addZone("island_dive", 775, 540, 100, 60);
      _lpMap->addZone("island_creek", 885, 535, 100, 70);
      _lpMap->addZone("island_cave", 840, 479, 60, 60);

      //inca
      _lpMap->addZone("inca_mainscreen", 187, 600, 120, 100);
      _lpMap->addZone("inca_stairs", 205, 492, 70, 100);
      _lpMap->addZone("inca_passage", 130, 451, 60, 140);
      _lpMap->addZone("inca_final", 116, 348, 105, 110);

      // japan
      _lpMap->addZone("japan_mainscreen", 790, 380, 100, 90);
      _lpMap->addZone("japan_garden", 790, 300, 100, 80);
      _lpMap->addZone("japan_bedroom", 795, 220, 85, 65);

      // Moyen age
      _lpMap->addZone("middleage_mainscreen", 195, 175, 100, 90);
      _lpMap->addZone("middleage_cloister", 205, 65, 75, 95);
      _lpMap->addZone("middleage_stairs", 100, 67, 83, 50);
      _lpMap->addZone("middleage_jail", 105, 185, 65, 75);

      // Egypte
      _lpMap->addZone("egypt_land", 620, 228, 133, 105);
      _lpMap->addZone("egypt_mazeentry", 682, 161, 65, 60);
      _lpMap->addZone("egypt_keyroom", 600, 161, 65, 60);
      _lpMap->addZone("egypt_maze", 552, 87, 133, 60);
      _lpMap->addZone("egypt_boatroom", 452, 72, 85, 85);
      _lpMap->addZone("egypt_tomb", 317, 67, 130, 95);
   
   // _lpMap->addString(new EFancyString("Chaine Test", _lpFontMouseOverInfo, 150, 100, 1.0f, 0.0f, 0.0f, 0.0f));
   //   _lpMap->setUniversVisited("UniversInca");
   //  _lpMap->setMarkerZone("Test1", EImageBank::getImage("arrow_point","png"));
   //  _lpMap->setVisited("Test1");
   
   // Init réels de la carte
   if (TaskResolved("task_island_universeresolved")) {
      _lpMap->setUniversVisited(/*"Universe*/"island");
   }
   if (TaskResolved("task_inca_universeresolved")) {
      _lpMap->setUniversVisited(/*"Universe*/"inca");
   }
   if (TaskResolved("task_japan_universeresolved")) {
      _lpMap->setUniversVisited(/*"Universe*/"japan");
   }
   if (TaskResolved("task_middleage_universeresolved")) {
      _lpMap->setUniversVisited(/*"Universe*/"middleage");
   }
   if (TaskResolved("task_Egypt_universeresolved")) {
      _lpMap->setUniversVisited(/*"Universe*/"egypt");
   }
   
   /* IHM */
   _lpFont = EFontBank::getFont("OldNewspaperTypes.ttf",15);

   /* Load button graphics */
   _lpButtonGraphic = EImageBank::getImage("inventorybuttons");
   
   _lpUIContainer = new KUIElement (KUIElement::getRootElement());
   _lpUIContainer->setPosition(0, 0);
   _lpUIContainer->setSize(1024, 768);
   _lpUIContainer->setMessageCallBack (_myUIMessageHandler);
   
   _lpButtonLeft = new KUIButton (_lpUIContainer);
   _lpButtonLeft->setPosition (258, 705);
   _lpButtonLeft->setSize (46, 42);
   _lpButtonLeft->setBackgroundGraphic(K_UIELEMENT_DISABLED, _lpButtonGraphic, 0, 0, 46, 42);
   _lpButtonLeft->setBackgroundGraphic(K_UIELEMENT_ENABLED, _lpButtonGraphic, 0, 0, 46, 42);
   _lpButtonLeft->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, _lpButtonGraphic, 0, 0, 46, 42);
   _lpButtonLeft->setBackgroundGraphic(K_UIELEMENT_DOWN, _lpButtonGraphic, 0+46*2, 0, 46+46*2, 42);
   _lpButtonLeft->setTextFont (_lpFont);
   _lpButtonLeft->setTextOutlineThickness (2);
   _lpButtonLeft->setTextOutlineColor (0, 0, 0, 0.125f);
   _lpButtonLeft->setVisibility(false);
   
   _lpButtonRight = new KUIButton (_lpUIContainer);
   _lpButtonRight->setPosition (751, 705);
   _lpButtonRight->setSize (46, 42);
   _lpButtonRight->setBackgroundGraphic(K_UIELEMENT_DISABLED, _lpButtonGraphic, (46), 0, (46)+46, 42);
   _lpButtonRight->setBackgroundGraphic(K_UIELEMENT_ENABLED, _lpButtonGraphic, (46), 0, (46)+46, 42);
   _lpButtonRight->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, _lpButtonGraphic, (46), 0, (46)+46, 42);
   _lpButtonRight->setBackgroundGraphic(K_UIELEMENT_DOWN, _lpButtonGraphic, (46)+46*2, 0, (46)+46+46*2, 42);
   _lpButtonRight->setTextFont (_lpFont);
   _lpButtonRight->setTextOutlineThickness (2);
   _lpButtonRight->setTextOutlineColor (0, 0, 0, 0.125f);
   _lpButtonRight->setVisibility(false);
   
   _lpButtonGraphic = EImageBank::getImage("menubutton");
   _lpButtonMenu = new KUIButton (_lpUIContainer);
   _lpButtonMenu->setPosition (1024-134, 0);
   _lpButtonMenu->setSize (134, 76);
   _lpButtonMenu->setBackgroundGraphic(K_UIELEMENT_DISABLED, _lpButtonGraphic, 0, 0, 134, 76);
   _lpButtonMenu->setBackgroundGraphic(K_UIELEMENT_ENABLED, _lpButtonGraphic, 0, 0, 134, 76);
   _lpButtonMenu->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, _lpButtonGraphic, 0, 0, 134, 76);
   _lpButtonMenu->setBackgroundGraphic(K_UIELEMENT_DOWN, _lpButtonGraphic, 0, 0, 134, 76);
   _lpButtonMenu->setTextFont(EFontBank::getFont("NITECLUB.TTF", 28), 28);
   _lpButtonMenu->setTextOutlineThickness (2);
   _lpButtonMenu->setTextOutlineColor (0, 0, 0, 0.125f);
   _lpButtonMenu->setTextColor(1.0f, 1.0f, 1.0f, 1.0f);
   _lpButtonMenu->setText(KStr("UI_MENU"));
   _lpButtonMenu->setVisibility(false);
   
   // ****************************************
   // * Bouton GUIDE                         *
   // ****************************************
   _lpIHMMiniButtonGraphic = EImageBank::getImage("ihm_minibutton");
   _lpButtonGuide = new KUIButton (_lpUIContainer);
   _lpButtonGuide->setSize (_lpIHMMiniButtonGraphic->getWidth(), 30); //_lpIHMMiniButtonGraphic->getHeight());
   _lpButtonGuide->setPosition (21, 740);
   _lpButtonGuide->setBackgroundGraphic(K_UIELEMENT_DISABLED, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonGuide->setBackgroundGraphic(K_UIELEMENT_ENABLED, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonGuide->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonGuide->setBackgroundGraphic(K_UIELEMENT_DOWN, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonGuide->setTextFont (EFontBank::getFont("OldNewspaperTypes18",-1),18);
   _lpButtonGuide->setText (KStr("UI_HINT"));
   //   _lpButtonGuide->setTextVertAlignMode(K_UIELEMENT_TOP);
   _lpButtonGuide->setVisibility(false);
   
   // ****************************************
   // * Bouton MAP                           *
   // ****************************************
   _lpIHMMiniButtonGraphic = EImageBank::getImage("ihm_minibutton");
   _lpButtonMap = new KUIButton (_lpUIContainer);
   _lpButtonMap->setSize (_lpIHMMiniButtonGraphic->getWidth(), 30);
   _lpButtonMap->setPosition (890, 740);
   _lpButtonMap->setBackgroundGraphic(K_UIELEMENT_DISABLED, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonMap->setBackgroundGraphic(K_UIELEMENT_ENABLED, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonMap->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonMap->setBackgroundGraphic(K_UIELEMENT_DOWN, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonMap->setTextFont (EFontBank::getFont("OldNewspaperTypes18",-1),18);
   _lpButtonMap->setText (KStr("UI_MAP"));
   //   _lpButtonMap->setTextVertAlignMode(K_UIELEMENT_TOP);
   _lpButtonMap->setVisibility(false);
   
   // ****************************************
   // * Bouton ENVELOPPE                     *
   // ****************************************
   _lpIHMMiniButtonGraphic = EImageBank::getImage("ihm_envelope");
   _lpButtonEnvelope = new KUIButton (_lpUIContainer);
   _lpButtonEnvelope->setSize (_lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonEnvelope->setPosition (9, 8);
   _lpButtonEnvelope->setBackgroundGraphic(K_UIELEMENT_DISABLED, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonEnvelope->setBackgroundGraphic(K_UIELEMENT_ENABLED, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonEnvelope->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonEnvelope->setBackgroundGraphic(K_UIELEMENT_DOWN, _lpIHMMiniButtonGraphic, 0, 0, _lpIHMMiniButtonGraphic->getWidth(), _lpIHMMiniButtonGraphic->getHeight());
   _lpButtonEnvelope->setTextFont (EFontBank::getFont("NITECLUB.TTF",25));
   _lpButtonEnvelope->setTextOutlineThickness (2);
   _lpButtonEnvelope->setTextOutlineColor (0, 0, 0, 0.125f);
   _lpButtonEnvelope->setTextColor(1.0f, 1.0f, 1.0f, 1.0f);
   _lpButtonEnvelope->setText ("");
   _lpButtonEnvelope->setTextFlow(K_UITEXT_RIGHT);
   _lpButtonEnvelope->setTextVertAlignMode(K_UIELEMENT_BOTTOM);
   _lpButtonEnvelope->setVisibility(false);
   
   // ****************************************
   // * Bouton DIARY                         *
   // ****************************************
   _lpIHMDiaryClosedGraphic = EImageBank::getImage("ihm_diary_closed");
   _lpButtonDiaryClosed = new KUIButton (_lpUIContainer);
   _lpButtonDiaryClosed->setZAngle(25);
   _lpButtonDiaryClosed->setSize (137,80);
   _lpButtonDiaryClosed->setPosition (106, 657);
   _lpButtonDiaryClosed->setBackgroundGraphic(K_UIELEMENT_DISABLED, _lpIHMDiaryClosedGraphic, 0, 0, _lpIHMDiaryClosedGraphic->getWidth(), _lpIHMDiaryClosedGraphic->getHeight());
   _lpButtonDiaryClosed->setBackgroundGraphic(K_UIELEMENT_ENABLED, _lpIHMDiaryClosedGraphic, 0, 0, _lpIHMDiaryClosedGraphic->getWidth(), _lpIHMDiaryClosedGraphic->getHeight());
   _lpButtonDiaryClosed->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, _lpIHMDiaryClosedGraphic, 0, 0, _lpIHMDiaryClosedGraphic->getWidth(), _lpIHMDiaryClosedGraphic->getHeight());
   _lpButtonDiaryClosed->setBackgroundGraphic(K_UIELEMENT_DOWN, _lpIHMDiaryClosedGraphic, 0, 0, _lpIHMDiaryClosedGraphic->getWidth(), _lpIHMDiaryClosedGraphic->getHeight());
   _lpButtonDiaryClosed->setTextFont (EFontBank::getFont("OldNewspaperTypes.ttf",12));
   _lpButtonDiaryClosed->setTextOutlineThickness (2);
   _lpButtonDiaryClosed->setTextOutlineColor (1, 1, 1, 0.125f);
   _lpButtonDiaryClosed->setTextColor(0.0f, 0.0f, 0.0f, 1.0f);
   _lpButtonDiaryClosed->setText (KStr("UI_DIARY"));
   _lpButtonDiaryClosed->setVisibility(false);
   _lpIHMDiaryOpenGraphic = EImageBank::getImage("ihm_diary_open");
   _lpIHMDiaryOpenSmartImage = new ESmartImage(_lpIHMDiaryOpenGraphic,126,657,1,false,true,500);
   _lpIHMDiaryOpenSmartImage->SetVisible(false, false);
   _lpButtonDiaryOpen = new KUIButton (_lpUIContainer);
   _lpButtonDiaryOpen->setSize (137,100);
   _lpButtonDiaryOpen->setPosition (125, 656);
   //   _lpButtonDiaryOpen->setBackgroundGraphic(K_UIELEMENT_DISABLED, _lpIHMDiaryOpenGraphic, 0, 0, _lpIHMDiaryOpenGraphic->getWidth(), _lpIHMDiaryOpenGraphic->getHeight());
   //   _lpButtonDiaryOpen->setBackgroundGraphic(K_UIELEMENT_ENABLED, _lpIHMDiaryOpenGraphic, 0, 0, _lpIHMDiaryOpenGraphic->getWidth(), _lpIHMDiaryOpenGraphic->getHeight());
   //   _lpButtonDiaryOpen->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, _lpIHMDiaryOpenGraphic, 0, 0, _lpIHMDiaryOpenGraphic->getWidth(), _lpIHMDiaryOpenGraphic->getHeight());
   //   _lpButtonDiaryOpen->setBackgroundGraphic(K_UIELEMENT_DOWN, _lpIHMDiaryOpenGraphic, 0, 0, _lpIHMDiaryOpenGraphic->getWidth(), _lpIHMDiaryOpenGraphic->getHeight());
   _lpButtonDiaryOpen->setVisibility(false);
   
   // Boites de dialogue
   _bDialogActive = false;
   // ****************************************
   // * Dialogbox OK                         *
   // ****************************************
   _lpDialogboxGraphic = EImageBank::getImage("dialogbox");
   _lpDialogboxButtonsGraphic = EImageBank::getImage("dialogbox_halo");
   // -- Container
   _lpUIDialogboxWindow = new KUIImage (_lpUIContainer);
   _lpUIDialogboxWindow->setPosition (floorf ((1024 - _lpDialogboxGraphic->getWidth()) / 2), floorf ((768 - _lpDialogboxGraphic->getHeight()) / 2));
   _lpUIDialogboxWindow->setSize (_lpDialogboxGraphic->getWidth(), _lpDialogboxGraphic->getHeight ());
   _lpUIDialogboxWindow->setGraphic (_lpDialogboxGraphic, 0, 0, _lpDialogboxGraphic->getWidth(), _lpDialogboxGraphic->getHeight ());
   _lpUIDialogboxWindow->setBlend (0); /* Start out as hidden */
   // -- Layout
   _lpDialogboxLayout = new KUILayout (_lpUIDialogboxWindow);
   _lpDialogboxLayout->setPosition (26, 16);
   _lpDialogboxLayout->setSize (_lpUIDialogboxWindow->getWidth() - 26 - 26, _lpUIDialogboxWindow->getHeight () - 16 - 16);
   _lpDialogboxLayout->setLayoutType (K_UILAYOUT_VERTICAL);
   _lpDialogboxLayout->setHorizAlignMode (K_UIELEMENT_HCENTER);
   _lpDialogboxLayout->setVSpacing (10);
   // -- Texte principal
   _lpDialogboxText = new KUIText (_lpDialogboxLayout);
   _lpDialogboxText->setSize (512, 190);
   _lpDialogboxText->setTextFont (EFontBank::getFont("jancient22",-10),22);
   _lpDialogboxText->setTextVertAlignMode (K_UIELEMENT_VCENTER);
   _lpDialogboxText->setTextFlow (K_UITEXT_CENTER);
   _lpDialogboxText->setText ("Dialog Box !");   
   // -- Bouton OK
   _lpDialogboxOKButton = new KUIButton (_lpDialogboxLayout);
   _lpDialogboxOKButton->setSize (_lpDialogboxButtonsGraphic->getWidth(), _lpDialogboxButtonsGraphic->getHeight());
   _lpDialogboxOKButton->setTextFont (EFontBank::getFont("NITECLUB.TTF",35));
   _lpDialogboxOKButton->setTextColor(0, 0, 0, 1);
   _lpDialogboxOKButton->setTextOutlineThickness (2);
   _lpDialogboxOKButton->setTextOutlineColor (1, 1, 1, 0.125f);
   _lpDialogboxOKButton->setText (KStr("UI_OK"));
   
   // ****************************************
   // * Dialogbox Hint System                *
   // ****************************************
   _lpDialogboxSpiritGraphic = EImageBank::getImage("dialogbox");
   _lpDialogboxSpiritButtonsGraphic = EImageBank::getImage("dialogbox_halo");
   // -- Container
   _lpUIDialogboxSpiritWindow = new KUIImage (_lpUIContainer);
   _lpUIDialogboxSpiritWindow->setPosition (floorf ((1024 - _lpDialogboxSpiritGraphic->getWidth()) / 2), floorf ((768 - _lpDialogboxSpiritGraphic->getHeight()) / 2));
   _lpUIDialogboxSpiritWindow->setSize (_lpDialogboxSpiritGraphic->getWidth(), _lpDialogboxSpiritGraphic->getHeight ());
   _lpUIDialogboxSpiritWindow->setGraphic (_lpDialogboxSpiritGraphic, 0, 0, _lpDialogboxSpiritGraphic->getWidth(), _lpDialogboxSpiritGraphic->getHeight ());
   _lpUIDialogboxSpiritWindow->setBlend (0); /* Start out as hidden */
   // -- Layout
   _lpDialogboxSpiritLayout = new KUILayout (_lpUIDialogboxSpiritWindow);
   _lpDialogboxSpiritLayout->setPosition (26, 16);
   _lpDialogboxSpiritLayout->setSize (_lpUIDialogboxSpiritWindow->getWidth() - 26 - 26, _lpUIDialogboxSpiritWindow->getHeight () - 16 - 16);
   _lpDialogboxSpiritLayout->setLayoutType (K_UILAYOUT_VERTICAL);
   _lpDialogboxSpiritLayout->setHorizAlignMode (K_UIELEMENT_HCENTER);
   _lpDialogboxSpiritLayout->setVSpacing (10);
   // -- Texte principal
   _lpDialogboxSpiritText = new KUIText (_lpDialogboxSpiritLayout);
   _lpDialogboxSpiritText->setSize (512, 190);
   _lpDialogboxSpiritText->setTextFont (EFontBank::getFont("jancient22",-15),22);
   _lpDialogboxSpiritText->setTextVertAlignMode (K_UIELEMENT_VCENTER);
   _lpDialogboxSpiritText->setTextFlow (K_UITEXT_CENTER);
   _lpDialogboxSpiritText->setText ("Dialog Box !");   
   // -- Bouton OK
   _lpDialogboxSpiritOKButton = new KUIButton (_lpDialogboxSpiritLayout);
   _lpDialogboxSpiritOKButton->setSize (_lpDialogboxSpiritButtonsGraphic->getWidth(), _lpDialogboxSpiritButtonsGraphic->getHeight());
   _lpDialogboxSpiritOKButton->setTextColor(0, 0, 0, 1);
   _lpDialogboxSpiritOKButton->setTextFont (EFontBank::getFont("NITECLUB.TTF",35));
   _lpDialogboxSpiritOKButton->setTextOutlineThickness(2);
   _lpDialogboxSpiritOKButton->setTextOutlineColor (1, 1, 1, 0.125f);
   _lpDialogboxSpiritOKButton->setText(KStr("UI_OK"));
   
   // ****************************************
   // * Dialogbox Yes/No                     *
   // ****************************************
   _lpDialogboxGraphic = EImageBank::getImage("dialogboxYN");
   // -- Container
   _lpUIDialogboxYNWindow = new KUIImage (_lpUIContainer);
   _lpUIDialogboxYNWindow->setPosition (floorf ((1024 - _lpDialogboxGraphic->getWidth()) / 2), floorf ((768 - _lpDialogboxGraphic->getHeight()) / 2));
   _lpUIDialogboxYNWindow->setSize (_lpDialogboxGraphic->getWidth(), _lpDialogboxGraphic->getHeight ());
   _lpUIDialogboxYNWindow->setGraphic (_lpDialogboxGraphic, 0, 0, _lpDialogboxGraphic->getWidth(), _lpDialogboxGraphic->getHeight ());
   _lpUIDialogboxYNWindow->setBlend (0); /* Start out as hidden */
   
   // -- VLayout
   _lpDialogboxYNVLayout = new KUILayout (_lpUIDialogboxYNWindow);
   _lpDialogboxYNVLayout->setPosition (16, 16);
   _lpDialogboxYNVLayout->setSize (_lpUIDialogboxYNWindow->getWidth() - 16 - 16, _lpUIDialogboxYNWindow->getHeight () - 16 - 16);
   _lpDialogboxYNVLayout->setLayoutType (K_UILAYOUT_VERTICAL);
   _lpDialogboxYNVLayout->setHorizAlignMode (K_UIELEMENT_HCENTER);
   _lpDialogboxYNVLayout->setVSpacing (10);
   
   // -- Texte principal
   _lpDialogboxYNText = new KUIText (_lpDialogboxYNVLayout);
   _lpDialogboxYNText->setSize (512, 210);
   _lpDialogboxYNText->setTextFont (EFontBank::getFont("jancient22",-2),22);
   _lpDialogboxYNText->setTextVertAlignMode (K_UIELEMENT_VCENTER);
   _lpDialogboxYNText->setTextFlow (K_UITEXT_CENTER);
   _lpDialogboxYNText->setText ("Yes or No Box !");   
   
   // -- HLayout
   _lpDialogboxYNHLayout = new KUILayout (_lpDialogboxYNVLayout);
   _lpDialogboxYNHLayout->setPosition(0,0);
   _lpDialogboxYNHLayout->setSize (512, _lpDialogboxButtonsGraphic->getHeight());
   _lpDialogboxYNHLayout->setLayoutType (K_UILAYOUT_HORIZONTAL);
   _lpDialogboxYNHLayout->setHorizAlignMode (K_UIELEMENT_HCENTER);
   _lpDialogboxYNHLayout->setHSpacing (0);
   
   // -- Bouton Yes
   _lpDialogboxYNYesButton = new KUIButton (_lpDialogboxYNHLayout);
   _lpDialogboxYNYesButton->setSize (512/2, 56);
   _lpDialogboxYNYesButton->setTextFont (EFontBank::getFont("NITECLUB.TTF",35));
   _lpDialogboxYNYesButton->setTextColor(0, 0, 0, 1);
   _lpDialogboxYNYesButton->setTextOutlineThickness (2);
   _lpDialogboxYNYesButton->setTextOutlineColor (1, 1, 1, 0.125f);
   _lpDialogboxYNYesButton->setText (KStr("UI_YES"));
   _nYesButtonPressed = NULL;
   // -- Bouton No
   _lpDialogboxYNNoButton = new KUIButton (_lpDialogboxYNHLayout);
   _lpDialogboxYNNoButton->setSize (512/2, 56);
   _lpDialogboxYNNoButton->setTextFont (EFontBank::getFont("NITECLUB.TTF",35));
   _lpDialogboxYNNoButton->setTextColor(0, 0, 0, 1);
   _lpDialogboxYNNoButton->setTextOutlineThickness (2);
   _lpDialogboxYNNoButton->setTextOutlineColor (1, 1, 1, 0.125f);
   _lpDialogboxYNNoButton->setText (KStr("UI_NO"));
   _nYesButtonPressed = NULL;
   
   _lpParticleEmitterDialogSpirit = new ParticleEmitter(0, 0);  
   if (!_lpParticleEmitterDialogSpirit->SetParticleBitmap("particule-etoile2")) {
      showFileError ("SmallStar.png");
      return;
   }
   _lpParticleEmitterDialogSpirit->SetHeightEffect(80);
   _lpParticleEmitterDialogSpirit->SetEmitRate(2);
   _lpParticleEmitterDialogSpirit->SetType(VFX_FLAKE);
   _lpParticleEmitterDialogSpirit->AddSegment(160, 250, 860, 250, 10);  /* create segment where emitter will move */
   
   _lpParticleEmitterDiary = new ParticleEmitter(0, 0);  
   if (!_lpParticleEmitterDiary->SetParticleBitmap("particule-etoile2")) {
      showFileError ("particle.png");
      return;
   }
   _lpParticleEmitterDiary->SetHeightEffect(80);
   _lpParticleEmitterDiary->SetEmitRate(1);
   _lpParticleEmitterDiary->SetType(VFX_FLAKE);
   _lpParticleEmitterDiary->AddSegment(173, 720, 218, 720, 5);  /* create segment where emitter will move */
}

ESceneDirector::~ESceneDirector()
{
   K_LOG("ESceneDirector::~ESceneDirector() -- Start");
   // Sauvegarde la scène en cours pour profil player
   // Si on est par sur le menu
   // TODO: gérer le cas particulier de title
   if (_lpSceneCurrent) {
      if (_lpSceneCurrent->IsNeedIHM()) {
         SetGlobal("__lastscene__", _szSceneCurrent);
         SetGlobal("__lastadditionalname__", _lpSceneCurrent->getAdditionalName().c_str());
      }
   }
   
   // Ferme les scènes
   XDELETE(_lpSceneCurrent);
   XDELETE(_lpVignetteCurrent);
   XDELETE(_lpMiniGameCurrent);

   // 17/08/11 Reset du séquenceur avant Delete du séquenceur
   // Car à ce stade, si il y avait un objet en cours de ramassage, il a été
   // mis dans l'inventaire par le destructeur de scène
   getSequencer()->Reset();

   // 17/08 Sauve après la fermeture de scène (ainsi l'inventaire est à jour)
   this->getDiaryPtr()->StorageInProfile();
   UserProfiles::SaveProfile();   
   
   XDELETE(_lpIHMDiaryOpenSmartImage);
   XDELETE(_lpsimgCharacterHouse);
   XDELETE(_lpsimgCharacterHouseDialog);
   XDELETE(_lpsimgCharacterUniverseDialog);
   XDELETE(_lpsimgCharacterIsland);
   XDELETE(_lpsimgCharacterEgypt);
   XDELETE(_lpsimgCharacterJapan);
   XDELETE(_lpsimgCharacterMiddleage);
   XDELETE(_lpsimgCharacterInca);
   XDELETE(_lpsimgCharacterHell);
   XDELETE(_lpsimgDialogCache);
   
   XDELETE(_lpBgGraphic);
   XDELETE(_lpsimgArrow);
   XDELETE(_lpsimgIHM);
   XDELETE(_lpAnimSceneOut);
   XDELETE(_lpMusicArranger);
   XDELETE(_lpSceneSequencer);
   XDELETE(_lpInventory);
   XDELETE(_lpBookCtrl);
   XDELETE(_lpMap);
   XDELETE(_lpTravellingItem);
   XDELETE(_lpsimgTravellingItem);
   XDELETE(_lpAnimEarthQuake);
   XDELETE(_lpHoldItem);
   XDELETE(_lpsimgHoldItem);
   XDELETE(_lpTravellingEmitter);
   
   XDELETE(_lpUIContainer);
   XDELETE(_lpParticleEmitterDialogSpirit);
   XDELETE(_lpParticleEmitterDiary);
   
   XDELETE(_lpsimgHexagramme);
   XDELETE(_lpsimgHexagramme_blue);
   XDELETE(_lpsimgHexagramme_green);
   XDELETE(_lpsimgHexagramme_purple);
   XDELETE(_lpsimgHexagramme_red);
   XDELETE(_lpsimgHexagramme_yellow);
   
   XDELETE(_lpEnergyParticlesManager);
   XDELETE(_lpFont);
   XDELETE(_lpFontMouseOverInfo);
   
   K_LOG("ESceneDirector::~ESceneDirector() -- End");
}

void ESceneDirector::OpenDialogbox(const char *szText)
{
   _bDialogActive = true;
   _lpDialogboxText->setText(szText);
   _lpUIDialogboxWindow->animateBlend (0, 1, 0, 250);
}

void ESceneDirector::CloseDialogbox()
{
   _bDialogActive = false;
   _lpUIDialogboxWindow->animateBlend (1, 0, 0, 250);
}

void ESceneDirector::OpenDialogboxSpirit(const char *szText)
{
   _bDialogActive = true;
   _lpDialogboxSpiritText->setText(szText);
   _lpUIDialogboxSpiritWindow->animateBlend (0, 1, 0, 300);
   ESoundBank::getSound("mg_end_counter")->playSample();
   ESoundBank::getSound("dialogshow")->playSample();
   _lpParticleEmitterDialogSpirit->Start();
}

void ESceneDirector::CloseDialogboxSpirit()
{
   _bDialogActive = false;
   _lpUIDialogboxSpiritWindow->animateBlend (1, 0, 0, 500);
   _lpParticleEmitterDialogSpirit->Stop();
}

void ESceneDirector::OpenDialogboxYN(const char *szText, int *nReturn)
{
   _bDialogActive = true;
   _nYesButtonPressed = nReturn;
	*_nYesButtonPressed = -1;
   _lpDialogboxYNText->setText(szText);
   _lpUIDialogboxYNWindow->animateBlend (0, 1, 0, 250);
}

void ESceneDirector::CloseDialogboxYN(bool bYesButtonsPressed)
{
   *_nYesButtonPressed = (int)bYesButtonsPressed;
   _nYesButtonPressed = NULL;
   _bDialogActive = false;
   _lpUIDialogboxYNWindow->animateBlend (1, 0, 0, 250);
};
void ESceneDirector::GoToScene(const char *szSceneName, const char *szAdditionalName, bool bPlayWalkSound)
{
   K_LOG("ESceneDirector::GoToScene(%s,%s)",szSceneName,szAdditionalName);
   KAssert(szSceneName!=NULL);
   KAssert(strlen(szSceneName)>0);
   
   SetTaskManager(false);
   ShowMapManager(false);
   
   // definit la scene comme visité, elle devient alors la zone courante de la map
   _lpMap->setVisited(szSceneName);
   
   // La scène demandée est déjà la scène courante
   if (strcmp(_szSceneCurrent, szSceneName) == 0)
   {
      // On prend quand même l'additionalname
      _lpSceneCurrent->setAdditionalName(szAdditionalName);
      // C'est peut être la vignette qui demande à se fermer ?
      if (_lpVignetteCurrent) {
         CloseVignette();
         return;
      }
   }
   EScene *lpScene = NULL;
   
   // Ferme la scène courante si besoin
   if (_lpSceneCurrent && _lpSceneCurrent->GetSceneState() != SCENE_SHOWOUT)
   {   
      Loader::IncrIndiceScene();
      // RAZ du contexte d'info en survol
      _lpSceneCurrent->SetMouseOverInfo("", "");
      // Demande à la scène si elle veut bien se fermer
      if (_lpSceneCurrent->AskForClose()) {
         // Pointe vers la nouvelle scène
         _bSceneNext = true;
         _lpAnimSceneOut->startCounter(1.0f, 1.4f, 0.0, 1000.0, K_COUNTER_EASEIN);
         // Termine doucement la scène courante
         _lpSceneCurrent->StartFade(SCENE_SHOWOUT);
         if (_bWalk || bPlayWalkSound) {
            _lpSceneCurrent->PlayWalkSound();
            _bWalk = false;
         }
         // Sauvegarde le contexte, en attendant la fin du fadeout de la scène courante
         strcpy(_szScenePrevious, _szSceneCurrent);
         strcpy(_szSceneNext, szSceneName);
         strcpy(_szSceneNextAdditionalName, szAdditionalName);
      }
   }
   else if (_lpSceneCurrent == NULL) {
      // Direct vers la nouvelle scène
      Loader::IncrIndiceScene();
      if (CreateSceneFromName(szSceneName, lpScene, szAdditionalName))
      {
         strcpy(_szSceneCurrent, szSceneName);
         strcpy(_szSceneNext, szSceneName);
         strcpy(_szSceneNextAdditionalName, szAdditionalName);
         _lpSceneCurrent = lpScene;
         if (_lpSceneCurrent->IsNeedIHM()) {
            // !!!dmekersa:20110708 On sauve plutôt la prochaine scène, ainsi l'état est plus cohérent avec le additionalname
            SetGlobal("__lastscene__", _szSceneNext);
            SetGlobal("__lastadditionalname__", _szSceneNextAdditionalName);
            // !!!:dmekersa:20110224 Sauvegarde systématique (sinon on perdait beaucoup de contexte en cas de plantage)
            this->getDiaryPtr()->StorageInProfile();
            UserProfiles::SaveProfile();   
         }
         _bSceneNext = false;
         SetupBackButton();
      }
   }
   
   //  if(_lpMap)
   //    _lpMap->setUniversVisited(_lpSceneCurrent->_strSceneUniverseName.c_str());
}

void ESceneDirector::GoToVignette(const char *szSceneName, const char *szAdditionalName, bool bAlpha)
{
   K_LOG("GoToVignette(%s,%s,%d", szSceneName, szAdditionalName, bAlpha);
   KAssert(szSceneName!=NULL);
   KAssert(strlen(szSceneName)>0);
   
   // On est déjà sur cette vignette
   if (_lpVignetteCurrent) {
      if (strcmp(_lpVignetteCurrent->GetName().c_str(),szSceneName) ==0 ) {
         K_LOG("ESceneDirector::GoToVignette return -> szSceneName == _lpVignetteCurrent->GetName()");
         return;
      }
   }
   
   EScene *lpScene = NULL;
   if (CreateSceneFromName(szSceneName, lpScene, szAdditionalName))
   {
      // Exécute le destructeur de la scène courante
      if (_lpVignetteCurrent)
      {
         // On devrait pas arriver ici !
         KAssert(_lpVignetteCurrent == NULL);
      }
      else {
         // On affiche la vignette
         _lpVignetteCurrent = lpScene;
         // On la passe en mode "vignette"
         _lpVignetteCurrent->setVignette(bAlpha);
      }
      SetupBackButton();
   } else {
      KAssert(lpScene != NULL);
   }
}

void ESceneDirector::GoToMiniGame(const char *szGameName, EMiniJeuBase *lpMiniJeu, const char *szSceneName)
{
   K_LOG("ESceneDirector::GoToMiniGame(szGameName=%s, szSceneName=%s)",szGameName,szSceneName);
   setLoading(true);
   SetTaskManager(false);
   ShowMapManager(false);
   _lpSceneCurrent->StopSoundDesign();
   _lpSceneCurrent->SetMouseOverInfo("", "");
   if (_lpVignetteCurrent) {
      _lpVignetteCurrent->SetMouseOverInfo("", "");
   }
   SceneMiniJeu *lpScene = new SceneMiniJeu(this, lpMiniJeu);
   KAssert(lpScene != NULL);
   std::string strName = szSceneName;
   lpScene->SetName(strName);
   lpScene->setAdditionalName(szGameName);
   
   char szSceneFile[K_MAXPATH];
   memset(szSceneFile, 0, K_MAXPATH);
   strcpy(szSceneFile, strName.c_str());
   strcat(szSceneFile, ".scn");
   char szSceneFilePath[K_MAXPATH];
   memset(szSceneFilePath, 0, K_MAXPATH);
   strcpy(szSceneFilePath, KMiscTools::makeFilePath(szSceneFile));
   lpScene->parseSceneFile(szSceneFilePath);
   lpScene->SetUniverseName(_lpSceneCurrent->_strSceneUniverseName);
   if (strName == "poltergeist") {
	   lpMiniJeu->SetSimpleUI();
	   ChangeMusic(DIRECTOR_MUSIC_POLTERGEIST);
   }
   else if (lpScene->_strSceneUniverseName == "house") {
      ChangeMusic(DIRECTOR_MUSIC_MGHOUSE);
   }
   else if (lpScene->_strSceneUniverseName == "island") {
      ChangeMusic(DIRECTOR_MUSIC_MGISLAND);
   }
   else if (lpScene->_strSceneUniverseName == "inca") {
      ChangeMusic(DIRECTOR_MUSIC_MGINCA);
   }
   else if (lpScene->_strSceneUniverseName == "japan") {
      ChangeMusic(DIRECTOR_MUSIC_MGJAPAN);
   }
   else if (lpScene->_strSceneUniverseName == "egypt") {
      ChangeMusic(DIRECTOR_MUSIC_MGEGYPT);
   }
   else if (lpScene->_strSceneUniverseName == "middleage") {
      ChangeMusic(DIRECTOR_MUSIC_MGMIDDLEAGE);
   }
   else if (lpScene->_strSceneUniverseName == "hell") {
      ChangeMusic(DIRECTOR_MUSIC_MGHELL);
   }
   
   // Déplacé ici car sinon écrase le nom du minijeu dans la séquence qui appelle cette méthode
   _lpSceneSequencer->Reset();
   
   lpScene->Init();
   PrepareMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
   lpScene->StartFade(SCENE_SHOWIN);
   // Exécute le destructeur de la scène courante
   if (_lpMiniGameCurrent)
   {
      // On devrait pas arriver ici !
      KAssert(_lpMiniGameCurrent == NULL);
   }
   else {
      // On affiche la vignette
      _lpMiniGameCurrent = lpScene;
   }
}

void ESceneDirector::ShowIHM(bool b)
{
   if (b && _bLockMode) {
      return;
   }
   // Mise à jour du bouton Enveloppe
   if (b) {
      char szNbEnvelope[256];
      memset(szNbEnvelope, 0, 256);
      strcpy(szNbEnvelope, GetStrGlobal("__envelope__","0"));
      strcat(szNbEnvelope, "/7");
      _lpButtonEnvelope->setText (szNbEnvelope);
   }
   _lpsimgIHM->SetVisible(b, true);
   _lpButtonMenu->setVisibility(b);
   _lpButtonGuide->setVisibility(b);
   if (TaskResolved("__gotmap__")) {
      _lpButtonMap->setVisibility(b);
   }
   
   _lpButtonEnvelope->setVisibility(false);
   _lpButtonDiaryOpen->setVisibility(false);
   _lpButtonDiaryClosed->setVisibility(false);
   if (GetCurrentScene()) {
      if (GetCurrentScene()->_strSceneUniverseName == "house") {
         if (strcmp(GetStrGlobal("__envelope__","0"),"0") == 0) {
            _lpButtonEnvelope->setVisibility(false);
         }
         else {
            _lpButtonEnvelope->setVisibility(b);
         }
         _lpButtonDiaryOpen->setVisibility(false);
         _lpButtonDiaryClosed->setVisibility(false);
         if (getDiaryPtr()) {
            if (getDiaryPtr()->isUnread()) {
               _lpButtonDiaryOpen->setVisibility(b);
               _lpIHMDiaryOpenSmartImage->SetVisible(true, true);
            }
            else {
               _lpButtonDiaryClosed->setVisibility(b);
               _lpIHMDiaryOpenSmartImage->SetVisible(false, true);
            }
         }
      }
      else {
         _lpIHMDiaryOpenSmartImage->SetVisible(false, true);
      }
   }
   if (TaskResolved("task_gethexagramme")) {
      bool bHexa = b;
      // Pas de médaillon ailleurs que dans la maison
      if (!_bHexagramVisible) {
         bHexa = false;
      }
      _lpsimgHexagramme->SetVisible(bHexa, true);
      // Blue - Island
      if (TaskResolved("task_island_universeresolved")) {
         _lpsimgHexagramme_blue->SetVisible(b, true);
      }
      else {
         _lpsimgHexagramme_blue->SetVisible(false, true);
      }
      // Purple - Japan
      if (TaskResolved("task_japan_universeresolved")) {
         _lpsimgHexagramme_purple->SetVisible(b, true);
      }
      else {
         _lpsimgHexagramme_purple->SetVisible(false, true);
      }
      // Yellow - Inca
      if (TaskResolved("task_inca_universeresolved")) {
         _lpsimgHexagramme_yellow->SetVisible(b, true);
      }
      else {
         _lpsimgHexagramme_yellow->SetVisible(false, true);
      }
      // Red - Egypte
      if (TaskResolved("task_egypt_universeresolved")) {
         _lpsimgHexagramme_red->SetVisible(b, true);
      }
      else {
         _lpsimgHexagramme_red->SetVisible(false, true);
      }
      // Green - Middleage
      if (TaskResolved("task_middleage_universeresolved")) {
         _lpsimgHexagramme_green->SetVisible(b, true);
      }
      else {
         _lpsimgHexagramme_green->SetVisible(false, true);
      }
      if (bHexa) {
         if (_lpsimgHexagramme->getCurrentX()>=1024) {
            _lpsimgHexagramme->AnimateP2P(1024-_lpsimgHexagramme->getWidth(), 0, 500, false, K_COUNTER_EASEOUT,0);
            _lpsimgHexagramme_blue->AnimateP2P(1024-_lpsimgHexagramme->getWidth(), 0, 500, false, K_COUNTER_EASEOUT,0);
            _lpsimgHexagramme_purple->AnimateP2P(1024-_lpsimgHexagramme->getWidth(), 0, 500, false, K_COUNTER_EASEOUT,0);
            _lpsimgHexagramme_yellow->AnimateP2P(1024-_lpsimgHexagramme->getWidth(), 0, 500, false, K_COUNTER_EASEOUT,0);
            _lpsimgHexagramme_red->AnimateP2P(1024-_lpsimgHexagramme->getWidth(), 0, 500, false, K_COUNTER_EASEOUT,0);
            _lpsimgHexagramme_green->AnimateP2P(1024-_lpsimgHexagramme->getWidth(), 0, 500, false, K_COUNTER_EASEOUT,0);
         }
      }
      else {
         if (_lpsimgHexagramme->getCurrentX()!=1024) {
            _lpsimgHexagramme->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
            _lpsimgHexagramme_blue->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
            _lpsimgHexagramme_purple->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
            _lpsimgHexagramme_yellow->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
            _lpsimgHexagramme_red->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
            _lpsimgHexagramme_green->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
         }
      }
   }
   else {
      // Referme le penta si il est visible car _bHexagramme = false
      if (_lpsimgHexagramme->getCurrentX()!=1024) {
         _lpsimgHexagramme->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
         _lpsimgHexagramme_blue->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
         _lpsimgHexagramme_purple->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
         _lpsimgHexagramme_yellow->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
         _lpsimgHexagramme_red->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
         _lpsimgHexagramme_green->AnimateP2P(1024, 0, 500, false, K_COUNTER_EASEOUT,0);
      }
   }
   
   if (_lpSceneCurrent) {
      if (_lpSceneCurrent->GetObjectImageByName("back")) {
         _lpSceneCurrent->SetVisible("back", b);
      }
   }
   if (_lpVignetteCurrent) {
      if (_lpVignetteCurrent->GetObjectImageByName("back")) {
         _lpVignetteCurrent->SetVisible("back", b);
      }
   }
   
   ShowCurrentCharacter(b);
   if (_bDialogMode) {
      ShowCurrentCharacter(true);
   }
   // Boutons de l'inventaire
   UpdateInventoryButtons();
}

void ESceneDirector::UpdateInventoryButtons()
{
   // Pas d'IHM... pas de boutons !
   if (_lpsimgIHM->isVisible() == false)
   {
      _lpButtonLeft->setVisibility(false);
      _lpButtonRight->setVisibility(false);
      return;
   }
   
   if (_lpInventory->canScrollLeft())
   {
      _lpButtonLeft->setVisibility(true);
   }
   else {
      _lpButtonLeft->setVisibility(false);
   }
   if (_lpInventory->canScrollRight())
   {
      _lpButtonRight->setVisibility(true);
   }
   else {
      _lpButtonRight->setVisibility(false);
   }
}

void ESceneDirector::ShowMouse(bool b)
{
   _lpsimgArrow->SetVisible(b, true);
}

void ESceneDirector::ChangeMouseCursor(DirectorMouseCursor cursor)
{
   switch (cursor) {
      case DIRECTOR_MOUSECURSOR_POINT:
         PrepareMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
         break;
      case DIRECTOR_MOUSECURSOR_WALK:
         PrepareMouseCursor(DIRECTOR_MOUSECURSOR_WALK);
         break;
      case DIRECTOR_MOUSECURSOR_WALKBACK:
         PrepareMouseCursor(DIRECTOR_MOUSECURSOR_WALKBACK);
         break;
      case DIRECTOR_MOUSECURSOR_DOOR:
         PrepareMouseCursor(DIRECTOR_MOUSECURSOR_DOOR);
         break;
      case DIRECTOR_MOUSECURSOR_ACTION:
         PrepareMouseCursor(DIRECTOR_MOUSECURSOR_ACTION);
         break;
      case DIRECTOR_MOUSECURSOR_LOOK:
         PrepareMouseCursor(DIRECTOR_MOUSECURSOR_LOOK);
         break;
      case DIRECTOR_MOUSECURSOR_TALK:
         PrepareMouseCursor(DIRECTOR_MOUSECURSOR_TALK);
         break;
      case DIRECTOR_MOUSECURSOR_WAIT:
         PrepareMouseCursor(DIRECTOR_MOUSECURSOR_WAIT);
         if (_lpSceneCurrent) {
            _lpSceneCurrent->SetMouseOverInfo("", "");
         }
         if (_lpVignetteCurrent) {
            _lpVignetteCurrent->SetMouseOverInfo("", "");
         }
         break;
      default:
         PrepareMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
         break;
   }
}

void ESceneDirector::PlayDirectorSound(const char *szSoundName)
{
   ESoundBank::getSound(szSoundName)->playSample();
}

void ESceneDirector::PrepareMouseCursor(const char *szMousePointer)
{
   if (strcmp(szMousePointer, "arrow_wait") == 0) {
      PrepareMouseCursor(DIRECTOR_MOUSECURSOR_WAIT);
   }
   else
      if (strcmp(szMousePointer, "arrow_action") == 0) {
         PrepareMouseCursor(DIRECTOR_MOUSECURSOR_ACTION);
      }
      else
         if (strcmp(szMousePointer, "arrow_door") == 0) {
            PrepareMouseCursor(DIRECTOR_MOUSECURSOR_DOOR);
         }
         else
            if (strcmp(szMousePointer, "arrow_look") == 0) {
               PrepareMouseCursor(DIRECTOR_MOUSECURSOR_LOOK);
            }
            else
               if (strcmp(szMousePointer, "arrow_info") == 0) {
                  PrepareMouseCursor(DIRECTOR_MOUSECURSOR_INFO);
               }
               else
                  if (strcmp(szMousePointer, "arrow_talk") == 0) {
                     PrepareMouseCursor(DIRECTOR_MOUSECURSOR_TALK);
                  }
                  else
                     if (strcmp(szMousePointer, "arrow_take") == 0) {
                        PrepareMouseCursor(DIRECTOR_MOUSECURSOR_TAKE);
                     }
                     else
                        if (strcmp(szMousePointer, "arrow_point") == 0) {
                           PrepareMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
                        }
                        else
                           if (strcmp(szMousePointer, "arrow_walk") == 0) {
                              PrepareMouseCursor(DIRECTOR_MOUSECURSOR_WALK);
                           }
                           else if (strcmp(szMousePointer, "arrow_walkback") == 0) {
                              PrepareMouseCursor(DIRECTOR_MOUSECURSOR_WALKBACK);
                           }
                           else if (strcmp(szMousePointer, "arrow_puzzle") == 0) {
                              PrepareMouseCursor(DIRECTOR_MOUSECURSOR_PUZZLE);
                           }
                           else
                           {
                              PrepareMouseCursor(DIRECTOR_MOUSECURSOR_LOOK);
                           }
}

void ESceneDirector::PrepareMouseCursor(DirectorMouseCursor cursor)
{
   // On ne fait rien si le curseur demandé est le même que celui actuel
   if (_cursor == cursor && _lpsimgArrow) {
      return;
   }
   // On supprime le curseur courant pour en faire un tout neuf
   if (_lpsimgArrow) {
      delete _lpsimgArrow;
      _lpsimgArrow = NULL;
   }
   switch (cursor) {
      case DIRECTOR_MOUSECURSOR_POINT:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowPoint,0.0,0.0, 1, false, false, 500);
         break;
      case DIRECTOR_MOUSECURSOR_WALK:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowWalk,0.0,0.0, 1, false, false, 500);
         break;
      case DIRECTOR_MOUSECURSOR_WALKBACK:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowWalkBack,0.0,0.0, 1, false, false, 500);
         break;
      case DIRECTOR_MOUSECURSOR_DOOR:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowDoor,0.0,0.0, 1, false, false, 500);
         break;
      case DIRECTOR_MOUSECURSOR_ACTION:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowAction,0.0,0.0, 1, false, false, 500);
         break;
      case DIRECTOR_MOUSECURSOR_LOOK:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowLook,0.0,0.0, 1, false, false, 500);
         break;
      case DIRECTOR_MOUSECURSOR_INFO:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowInfo,0.0,0.0, 1, false, false, 500);
         break;
      case DIRECTOR_MOUSECURSOR_TALK:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowTalk,0.0,0.0, 1, false, false, 500);
         break;
      case DIRECTOR_MOUSECURSOR_TAKE:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowTake,0.0,0.0, 1, false, false, 500);
         break;
      case DIRECTOR_MOUSECURSOR_WAIT:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowWait,0.0,0.0, 1, false, false, 500);
         break;
      case DIRECTOR_MOUSECURSOR_PUZZLE:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowPuzzle,0.0,0.0, 1, false, false, 500);
         break;
      default:
         _lpsimgArrow = new ESmartImage(_lpGraphicArrowPoint,0.0,0.0, 1, false, false, 500);
         break;
   }
   ShowMouse(true);
   _cursor = cursor;
}

void ESceneDirector::ChangeMusic(DirectorMusic music, bool bWithoutFadeout)
{
   // On joue déjà cette musique
   if ( _nCurrentMainMusic == music) {
      return;
   }
   // On charge la musique (ou rien si NONE)
   bool bSuccess = false;
   switch (music) {
      case DIRECTOR_MUSIC_MENU:
         _lpMusicArranger->PlayMusic("music_menu", false);
         break;
      case DIRECTOR_MUSIC_CINEMATIC:
         _lpMusicArranger->PlayMusic("music_cinematic", false);
         break;
      case DIRECTOR_MUSIC_POLTERGEIST:
         _lpMusicArranger->PlayMusic("music_poltergeist", false);
         break;
      case DIRECTOR_MUSIC_HOUSE:
         _lpMusicArranger->PlayMusic("music_house", false);
         break;
      case DIRECTOR_MUSIC_ISLAND:
         _lpMusicArranger->PlayMusic("music_island", false);
         break;
      case DIRECTOR_MUSIC_DIVE30:
         _lpMusicArranger->PlayMusic("music_island_dive", false);
         break;
      case DIRECTOR_MUSIC_JAPAN:
         _lpMusicArranger->PlayMusic("music_japan", false);
         break;
      case DIRECTOR_MUSIC_INCA:
         _lpMusicArranger->PlayMusic("music_inca", false);
         break;
      case DIRECTOR_MUSIC_EGYPT:
         _lpMusicArranger->PlayMusic("music_egypt", false);
         break;
      case DIRECTOR_MUSIC_HELL:
         _lpMusicArranger->PlayMusic("music_hell", false);
         break;
      case DIRECTOR_MUSIC_MIDDLEAGE:
         _lpMusicArranger->PlayMusic("music_middleage", false);
         break;
      case DIRECTOR_MUSIC_MGHOUSE:
         _lpMusicArranger->PlayMusic("music_mg_house", false);
         break;
      case DIRECTOR_MUSIC_MGISLAND:
         _lpMusicArranger->PlayMusic("music_mg_island", false);
         break;
      case DIRECTOR_MUSIC_MGINCA:
         _lpMusicArranger->PlayMusic("music_mg_inca", false);
         break;
      case DIRECTOR_MUSIC_MGMIDDLEAGE:
         _lpMusicArranger->PlayMusic("music_mg_middleage", false);
         break;
      case DIRECTOR_MUSIC_MGEGYPT:
         _lpMusicArranger->PlayMusic("music_mg_egypt", false);
         break;
      case DIRECTOR_MUSIC_MGJAPAN:
         _lpMusicArranger->PlayMusic("music_mg_japan", false);
         break;
      case DIRECTOR_MUSIC_MGHELL:
         _lpMusicArranger->PlayMusic("music_mg_hell", false);
         break;
      case DIRECTOR_MUSIC_NONE:
         _lpMusicArranger->ResetQueue();
         _lpMusicArranger->FadeOut(bWithoutFadeout);
         bSuccess = true;
         break;
      default:
         break;
   }
   _nCurrentMainMusic = music;
}

// Change de personnage en fonction du nom de l'univers passé en paramètre
void ESceneDirector::ChangeCharacter(const char *szCharacter)
{
   if (strcmp(szCharacter,"house") == 0)
   {
      ChangeCharacter(DIRECTOR_CHARACTER_HOUSE);
   }
   else if (strcmp(szCharacter,"island") == 0)
   {
      ChangeCharacter(DIRECTOR_CHARACTER_ISLAND);
   }
   else if (strcmp(szCharacter,"japan") == 0)
   {
      ChangeCharacter(DIRECTOR_CHARACTER_JAPAN);
   }
   else if (strcmp(szCharacter,"middleage") == 0)
   {
      ChangeCharacter(DIRECTOR_CHARACTER_MIDDLEAGE);
   }
   else if (strcmp(szCharacter,"egypt") == 0)
   {
      ChangeCharacter(DIRECTOR_CHARACTER_EGYPT);
   }
   else if (strcmp(szCharacter,"inca") == 0)
   {
      ChangeCharacter(DIRECTOR_CHARACTER_INCA);
   }
   else if (strcmp(szCharacter,"hell") == 0)
   {
      ChangeCharacter(DIRECTOR_CHARACTER_HELL);
   }
   else
   {
      ChangeCharacter(DIRECTOR_CHARACTER_HOUSE);
   }
}

void ESceneDirector::ChangeCharacter(DirectorCharacter character)
{
   // On a déjà ce personnage affiché !
   if (_currentCharacter == character)
   {
      return;
   }
   ShowCurrentCharacter(false);
   // Affiche le nouveau personnage
   // Son de morphing
   if (_lpsimgIHM->isVisible())
      ESoundBank::getSound("ghostchange")->playSample();
   _currentCharacter = character;
   ShowCurrentCharacter(_lpsimgIHM->isVisible());
}

void ESceneDirector::ShowCurrentCharacter(bool b)
{
   switch (_currentCharacter){
      case DIRECTOR_CHARACTER_NONE:
         break;
      case DIRECTOR_CHARACTER_HOUSE:
         if (_bDialogMode) {
            _lpsimgCharacterHouseDialog->SetVisible(b,true);
            _lpsimgCharacterHouse->SetVisible(false,true);
         }
         else {
            _lpsimgCharacterHouseDialog->SetVisible(false,true);
            _lpsimgCharacterHouse->SetVisible(b,true);
         }
         break;
      case DIRECTOR_CHARACTER_ISLAND:
         if (_bDialogMode) {
            _lpsimgCharacterIsland->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(b,true);
         }
         else {
            _lpsimgCharacterIsland->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(false,true);
         }
         break;
      case DIRECTOR_CHARACTER_JAPAN:
         if (_bDialogMode) {
            _lpsimgCharacterJapan->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(b,true);
         }
         else {
            _lpsimgCharacterJapan->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(false,true);
         }
         break;
      case DIRECTOR_CHARACTER_INCA:
         if (_bDialogMode) {
            _lpsimgCharacterInca->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(b,true);
         }
         else {
            _lpsimgCharacterInca->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(false,true);
         }
         break;
      case DIRECTOR_CHARACTER_EGYPT:
         if (_bDialogMode) {
            _lpsimgCharacterEgypt->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(b,true);
         }
         else {
            _lpsimgCharacterEgypt->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(false,true);
         }
         break;
      case DIRECTOR_CHARACTER_MIDDLEAGE:
         if (_bDialogMode) {
            _lpsimgCharacterMiddleage->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(b,true);
         }
         else {
            _lpsimgCharacterMiddleage->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(false,true);
         }
         break;
      case DIRECTOR_CHARACTER_HELL:
         if (_bDialogMode) {
            _lpsimgCharacterHell->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(b,true);
         }
         else {
            _lpsimgCharacterHell->SetVisible(b,true);
            _lpsimgDialogCache->SetVisible(false,true);
         }
         break;
   }
}

/* Prépare la musique en fonction de l'univers */
void ESceneDirector::PrepareMusic(EScene *lpScene)
{
   if (lpScene->_strSceneUniverseName == "house")
   {
      ChangeMusic(DIRECTOR_MUSIC_HOUSE);
   }
   if (lpScene->_strSceneUniverseName == "japan")
   {
      ChangeMusic(DIRECTOR_MUSIC_JAPAN);
   }
   if (lpScene->_strSceneUniverseName == "inca")
   {
      ChangeMusic(DIRECTOR_MUSIC_INCA);
   }
   if (lpScene->_strSceneUniverseName == "middleage")
   {
      ChangeMusic(DIRECTOR_MUSIC_MIDDLEAGE);
   }
   if (lpScene->_strSceneUniverseName == "egypt")
   {
      ChangeMusic(DIRECTOR_MUSIC_EGYPT);
   }
   if (lpScene->_strSceneUniverseName == "island")
   {
      ChangeMusic(DIRECTOR_MUSIC_ISLAND);
   }
   if (lpScene->_strSceneUniverseName == "hell")
   {
      ChangeMusic(DIRECTOR_MUSIC_HELL);
   }
   if (lpScene->_strSceneUniverseName == "none")
   {
      ChangeMusic(DIRECTOR_MUSIC_NONE);
   }
}

EScene *ESceneDirector::GetCurrentScene()
{
   return _lpSceneCurrent;
}

EScene *ESceneDirector::GetCurrentVignette()
{
   return _lpVignetteCurrent;
}

EScene *ESceneDirector::GetCurrentMinigame()
{
   return _lpMiniGameCurrent;
}

EScene *ESceneDirector::GetCurrentEScene()
{
   if (_lpMiniGameCurrent != NULL)
      return _lpMiniGameCurrent;
   if (_lpVignetteCurrent != NULL)
      return _lpVignetteCurrent;
   if (_lpSceneCurrent != NULL)
      return _lpSceneCurrent;
   return NULL;
}

void ESceneDirector::Logic()
{
   bool bShowIHMdone = false;
   _lpsimgIHM->Logic();
   _lpsimgHexagramme->Logic();
   _lpsimgHexagramme_blue->Logic();
   _lpsimgHexagramme_purple->Logic();
   _lpsimgHexagramme_yellow->Logic();
   _lpsimgHexagramme_red->Logic();
   _lpsimgHexagramme_green->Logic();
   _lpsimgArrow->Logic();
   _lpIHMDiaryOpenSmartImage->Logic();
   _lpsimgCharacterHouse->Logic();
   _lpsimgCharacterIsland->Logic();
   _lpsimgCharacterEgypt->Logic();
   _lpsimgCharacterJapan->Logic();
   _lpsimgCharacterInca->Logic();
   _lpsimgCharacterMiddleage->Logic();
   _lpsimgCharacterHell->Logic();
   _lpsimgDialogCache->Logic();
   
   _lpBookCtrl->Logic();
   
   /********************/
   /* IHM ou pas IHM ? */
   /********************/
   /* On vérifie si la séquence en court est modale ou pas
    et en fonction de ça on affiche ou pas l'IHM */
   if (_lpSceneSequencer->isActive()) {
      // Si oui on cache l'IHM
      if (_lpSceneSequencer->isModal()) {
         if (!bShowIHMdone) ShowIHM(false);
         bShowIHMdone = true;
         ShowMouse(true);
      }
   }
   else if (_bCinematicMode || _bDialogMode) {
      if (!bShowIHMdone) ShowIHM(false);
      bShowIHMdone = true;
      ShowMouse(true);
   }
   else if (_lpMap->isVisible() || _lpBookCtrl->isVisible(TASK_LIST) || _lpBookCtrl->isVisible(DIARY)) {
      if (_lpsimgIHM->isVisible()) {
         if (!bShowIHMdone) ShowIHM(false);
         bShowIHMdone = true;
      }
   }
   
   else if (_lpSceneCurrent)
   {
      // Sinon on réaffiche, si besoin, l'IHM et le curseur
      bool _bNeedIHM = false;
      if (_lpVignetteCurrent) {
         _bNeedIHM = _lpVignetteCurrent->IsNeedIHM();
      }
      else if (_lpSceneCurrent)
      {
         _bNeedIHM = _lpSceneCurrent->IsNeedIHM();
      }
      if (!_lpsimgIHM->isVisible() && _bNeedIHM && !_lpMiniGameCurrent && !_bSceneNext && ESequenceContainer::g_ListSequence.getCount() == 0 && !_bLockMode && !_bDialogMode && !_bCinematicMode)
      {
         if (!bShowIHMdone) ShowIHM(true);
         bShowIHMdone = true;
         ShowMouse(true);
      }
   }
   // Dans tous les cas, si un mini jeu est en cours, pas d'IHM!
   if (_lpMiniGameCurrent && _lpsimgIHM->isVisible()) {
      if (!bShowIHMdone) ShowIHM(false);
      bShowIHMdone = true;
   }
   
   // Scène courante
   if (_lpSceneCurrent)
      _lpSceneCurrent->Logic();
   
   // Vignette courante
   if (_lpVignetteCurrent) {
      _lpVignetteCurrent->Logic();
      if (_bClosingVignette) {
         if (_lpVignetteCurrent->FadeOutCompleted()) {
            _lpVignetteCurrent->Close();
            _bClosingVignette = false;
            EObjectContainer::CleanUp(_lpVignetteCurrent->_strSceneName);
            ESequenceContainer::CleanUp(_lpVignetteCurrent->_strSceneName);
            XDELETE(_lpVignetteCurrent);
            _lpVignetteCurrent = NULL;
            // On rappelle l'init, en cas d'actions à effet de bord dans la vignette
            _lpSceneCurrent->Init();
         }
      }
   }
   
   // Mini Jeu courant
   if (_lpMiniGameCurrent) {
      _lpMiniGameCurrent->Logic();
      if (_bClosingMiniGame) {
         if (_lpMiniGameCurrent->FadeOutCompleted()) {
            K_LOG("ESceneDirector::Logic() -> Closing minigame");
            _bClosingMiniGame = false;
            _lpMiniGameCurrent->Close();
            EObjectContainer::CleanUp(_lpMiniGameCurrent->_strSceneName);
            ESequenceContainer::CleanUp(_lpMiniGameCurrent->_strSceneName);
            _lpSceneSequencer->Reset();
            
            // On informe la scène qu'un mini jeu est terminé et son résultat
            if (_lpMiniGameCurrent->HasEnded()) {
               if (_lpVignetteCurrent) {
                  _lpVignetteCurrent->MiniGameDone(_lpMiniGameCurrent->_strSceneAdditionalName.c_str(),_lpMiniGameCurrent->isResolved());
               }
               else {
                  _lpSceneCurrent->MiniGameDone(_lpMiniGameCurrent->_strSceneAdditionalName.c_str(),_lpMiniGameCurrent->isResolved());
                  _lpSceneCurrent->StartFade(SCENE_SHOWIN);
                  _lpSceneCurrent->StartSoundDesign();
               }
            }
            else {
               if (_lpVignetteCurrent) {
                  _lpVignetteCurrent->MiniGameDone(_lpMiniGameCurrent->_strSceneAdditionalName.c_str(),false);
               }
               else {
                  _lpSceneCurrent->MiniGameDone(_lpMiniGameCurrent->_strSceneAdditionalName.c_str(),false);
                  _lpSceneCurrent->StartFade(SCENE_SHOWIN);
                  _lpSceneCurrent->StartSoundDesign();
               }
            }
            
            XDELETE(_lpMiniGameCurrent);
            _lpMiniGameCurrent = NULL;
         }
      }
   }
   
   // Une transition vers une nouvelle scène est en cours, est elle terminée ?
   if (_bSceneNext) {
      if (_lpSceneCurrent->FadeOutCompleted()) {
         K_LOG("ESceneDirector::Logic() -> Closing current scene");
         _lpSceneCurrent->Close();
         _lpSceneCurrent->EScene::Close();
         EObjectContainer::CleanUp(_lpSceneCurrent->_strSceneName);
         ESequenceContainer::CleanUp(_lpSceneCurrent->_strSceneName);
         _lpSceneSequencer->Reset();
         // !!!:dmekersa:20110502 Fermer la TaskList/Diary et cie
         XDELETE(_lpSceneCurrent);

         // On valide le changement de scène
         strcpy(_szSceneCurrent, _szSceneNext);
         memset(_szSceneNext, 0, 255);
         EScene *lpScene = NULL;
         if (CreateSceneFromName(_szSceneCurrent, lpScene, _szSceneNextAdditionalName))
         {
            _lpSceneCurrent = lpScene;
            if (_lpSceneCurrent->IsNeedIHM()) {
               // !!!:dmekersa:20110224 Sauvegarde systématique (sinon on perdait beaucoup de contexte en cas de plantage)
               this->getDiaryPtr()->StorageInProfile();
               SetGlobal("__lastscene__", _szSceneCurrent);
               SetGlobal("__lastadditionalname__", _szSceneNextAdditionalName);
               UserProfiles::SaveProfile();   
            }
            _bSceneNext = false;
            SetupBackButton();
            //            _lpSceneCurrent->Logic();
            //            KGame::getGame()->resetFrameLoop();
            //            KGame::getGame()->beginFrame();
            //            _lpSceneCurrent->Draw();
            //            _bResetTimeframe = true;
            //            return;  // !!!:dmekersa:20110203 Il semble que ça améliore les saccades après le loading
         } else {
            KAssert(false);
         }
         
      }
   }
   
   // Une vignette est affichée, est-ce que le joueur clique en dehors ?
   // TODO : gérer une taille et une position sur EScene
   
   /* Déplace le Mouse pointer à la position de la souris */
   long x,y;
   x = Mouse.x;
   y = Mouse.y;
   // Pointeur classique
   if (_cursor == DIRECTOR_MOUSECURSOR_POINT || _cursor == DIRECTOR_MOUSECURSOR_WAIT) {
      _lpsimgArrow->Move((float)x-4, (float)y);
   } 
   else if (_cursor == DIRECTOR_MOUSECURSOR_POINT || _cursor == DIRECTOR_MOUSECURSOR_WAIT) {
      _lpsimgArrow->Move((float)x-4, (float)y);
   }
   else if (_cursor == DIRECTOR_MOUSECURSOR_TAKE && !isSystemCursor()) {
      _lpsimgArrow->Move((float)x - 10, (float)y - 10);
   }
   // Pointeur image, donc on centre
   else {
      _lpsimgArrow->Move((float)x - _lpsimgArrow->getWidth()/2, (float)y - _lpsimgArrow->getHeight()/2);
   }
   
   if (_lpHoldItem) {
      _lpHoldItem->setPos((float)(x-_lpHoldItem->_lpsimgImage->getWidth()/2), (float)(y-_lpHoldItem->_lpsimgImage->getHeight()/2));
   }
   
   /* Séquenceur, ne fonctionne que quand la scène est totalement visible */
   bool bFadeInCompleted = false;
   if (!_lpVignetteCurrent)
   {
      bFadeInCompleted = _lpSceneCurrent->FadeInCompleted();
   }
   else {
      bFadeInCompleted = _lpVignetteCurrent->FadeInCompleted();
   }
   
   // Inventaire
   _lpTravellingEmitter->Logic();
   if (_lpTravellingItem) {
      _lpTravellingItem->Logic();
      _lpTravellingEmitter->SetPosition(_lpTravellingItem->_X + _lpTravellingItem->_lpGraphic->getWidth()/2, _lpTravellingItem->_Y + _lpTravellingItem->_lpGraphic->getHeight()/2);
      // Terminé ? L'item est ajouté à l'inventaire
      if (!_lpTravellingItem->_lpsimgImage->isAnimateP2P()) {
         EndTravellingItem();
      }
   }
   
   if (_lpHoldItem) {
      _lpHoldItem->Logic();
   }
   
   if (bFadeInCompleted && !_lpTravellingItem ) {
      _lpSceneSequencer->Logic();
   }
   
   //   // Recherche du hint system
   //   _lpParticleEmitterDialogSpirit->Logic();
   _lpParticleEmitterDiary->Logic();
   //   if (_lpSceneCurrent) {
   //      if (_lpsimgIHM->isVisible()) {
   //         if (_fHintLevel < 100.0f) {
   //            _fHintLevel += 0.01;
   //            if (_fHintLevel > 100.0f) {
   //               _fHintLevel = 100.0f;
   //            }
   //            // Effet "rempli !"
   //            if (_fHintLevel == 100.0f) {
   //               ESoundBank::getSound("gong")->playSample();
   //               _lpParticleEmitterDiary->Start();
   //            }
   //         }
   //      }
   //   }
   
   // Particules d'énergie
   _lpEnergyParticlesManager->Logic();
}

void ESceneDirector::EndTravellingItem()
{
   if (_lpTravellingItem) {
      K_LOG("EndTravellingItem:%s",_lpTravellingItem->_szItemName);
   }
   else {
      K_LOG("EndTravellingItem - _lpTravellingItem==NULL");
      return;
   }
   // Fin de l'effet visuel et habillage sonore
   _lpTravellingEmitter->Stop();
   ESoundBank::getSound("successpickup")->playSample();
   // Ajout final de l'item à l'inventaire si ce n'est pas l'hexagramme ou une sphere
   if (strcmp(_lpTravellingItem->_szItemName, "inv_hexagramme") == 0) {
      ResolveTask("task_gethexagramme");
      XDELETE(_lpTravellingItem);
   }
   else if (strcmp(_lpTravellingItem->_szItemName, "inv_map") == 0) {
      ResolveTask("__gotmap__");
      ShowIHM(true);
      XDELETE(_lpTravellingItem);
   }
   else if (strcmp(_lpTravellingItem->_szItemName, "ihm_envelope") == 0) {
      int nbEnvelope = 0;
      nbEnvelope = EGlobalBank::getIntValue("__envelope__", 0);
      nbEnvelope++;
      SetGlobal("__envelope__",itos(nbEnvelope).c_str());
      char szNbEnvelope[256];
      memset(szNbEnvelope, 0, 256);
      strcpy(szNbEnvelope, GetStrGlobal("__envelope__","0"));
      strcat(szNbEnvelope, "/7");
      _lpButtonEnvelope->setText (szNbEnvelope);
      _lpSceneDirector->ShowDiaryManager(true);
      XDELETE(_lpTravellingItem);
   }
   else {
      if (_lpInventory->getTravellingItem()) {
         K_LOG("Inventory got a Travelling Item");
         _lpInventory->ValidateTravellingItem();
      }
      else
      {
         K_LOG("Inventory got no Travelling Item");
      }
      _lpTravellingItem = NULL;
   }
}

void ESceneDirector::Check()
{
   bool bCheckAllowed = true;
   
#ifdef ENIGMA_STATS
   // Debug
   if (KInput::isPressed(K_VK_TAB)) {
      _bDebugDisplay = true;
   }
#endif

   /*if (_bTaskManager) {
    bCheckAllowed = false;
    }*/
   
   if(_lpBookCtrl->isVisible(TASK_LIST) || _lpBookCtrl->isVisible(DIARY)) {
      bCheckAllowed = false;
      // Gestionnaire de tâches ?
      if(!this->_bCinematicMode) {
         _lpBookCtrl->Check();
      }
   }
   if(_lpMap->isVisible()) {
      bCheckAllowed = false;
      _lpMap->Check();
   }
   
   // Dialogbox affichée
   if (_bDialogActive)
   {
      _lpSceneCurrent->SetMouseOverInfo("", "");
      bCheckAllowed = false;
      if (_lpDialogboxOKButton->isClicked()) {
         CloseDialogbox();
      }
      if (_lpDialogboxSpiritOKButton->isClicked()) {
         CloseDialogboxSpirit();
      }
      if (_lpDialogboxYNYesButton->isClicked()) {
         CloseDialogboxYN(true);
      }
      if (_lpDialogboxYNNoButton->isClicked()) {
         CloseDialogboxYN(false);
      }
      
   }
   
   if (ETutoBox::GetCurrentTuto() != NULL)
	   ETutoBox::GetCurrentTuto()->Check();
   
   // Si une séquence modale est en cours, on cache l'IHM et on empèche le check de la scène
   if (_lpSceneSequencer->isActive()) {
      if (_lpSceneSequencer->isModal() == true || _lpSceneSequencer->isCheckAllowed() == false) {
         bCheckAllowed = false;
      }
   }
   
   // La souris est capturée si on est en mode dialogue de personnage
   if (_bDialogMode || _bCinematicMode || _bLockMode)
   {
      bCheckAllowed = false;
   }
   
   // Si l'interraction est autorisée, on commence par voir si le clic concerne l'IHM
   if (_lpsimgIHM->isVisible() && !_bDialogActive)
   {
      // Inventaire ?
      if (_lpInventory->Check()) {
         //bCheckAllowed = false;
      }
   }
   
   // Est-ce que le clic concerne un des boutons de l'inventaire ?
   if (_lpsimgIHM->isVisible() && _lpButtonLeft->isClicked() && !_lpMiniGameCurrent)
   {
      bCheckAllowed = false;
      if (!_lpTravellingItem)
      {
         _lpInventory->ScrollLeft();
      }
      UpdateInventoryButtons();
      Mouse.LeftEventClose();
      return;
   }
   if (_lpsimgIHM->isVisible() && _lpButtonRight->isClicked() && !_lpMiniGameCurrent)
   {
      bCheckAllowed = false;
      if (!_lpTravellingItem)
      {
         _lpInventory->ScrollRight();
      }
      UpdateInventoryButtons();
      Mouse.LeftEventClose();
      return;
   }
   // affichage de la carte, raccourci clavier temporaire
   if (_lpsimgIHM->isVisible() && KInput::isPressed(K_VK_T) && !_lpMiniGameCurrent) {
      this->ShowMapManager(true);
      ESoundBank::getSound("mapopen")->playSample();
   }
   if (_lpsimgIHM->isVisible() && KInput::isPressed(K_VK_Y) && !_lpMiniGameCurrent) {
      this->ShowMapManager(false);
      ESoundBank::getSound("mapclose")->playSample();
   }
   
   if (_lpsimgIHM->isVisible() && KInput::isPressed(K_VK_U) && !_lpMiniGameCurrent) {
      this->ShowDiaryManager(true);
   }
   if (_lpsimgIHM->isVisible() && KInput::isPressed(K_VK_I) && !_lpMiniGameCurrent) {
      this->ShowDiaryManager(false);
   }   
   
   if (_lpsimgIHM->isVisible() && _lpButtonEnvelope->isClicked())
   {
      _lpSceneDirector->OpenDialogbox(KStr("IHM_INFO_ENVELOPE"));
      
      Mouse.LeftEventClose();
      return;
   }
   if (_lpsimgIHM->isVisible() && _lpButtonGuide->isClicked() && !_lpMiniGameCurrent)
   {
      if (!_lpBookCtrl->isVisible(TASK_LIST)  && !_lpBookCtrl->isClosing()) {
         SetTaskManager(true);
      }
      else {
         SetTaskManager(false);
      }
      
      Mouse.LeftEventClose();
      return;
   }
   // Diary
   // Mise à jour, affiche le Diary correspondant
   if (GetCurrentScene() != NULL) {
      if (GetCurrentScene()->_strSceneUniverseName == "house") {
         if (_lpsimgIHM->isVisible() && !_lpMiniGameCurrent) {
            if (getDiaryPtr()->isUnread() == true && _lpButtonDiaryOpen->getVisibility() == false) {
               _lpButtonDiaryOpen->setVisibility(true);
               _lpButtonDiaryClosed->setVisibility(false);
               _lpIHMDiaryOpenSmartImage->SetVisible(true, true);
            }
            else if (getDiaryPtr()->isUnread() == false && _lpButtonDiaryClosed->getVisibility() == false) {
               _lpButtonDiaryOpen->setVisibility(false);
               _lpButtonDiaryClosed->setVisibility(true);
               _lpIHMDiaryOpenSmartImage->SetVisible(false, true);
            }
         }
      }
      
   }
   if (ETutoBox::GetCurrentTuto() == NULL) {
      if (_lpsimgIHM->isVisible() && _lpButtonMap->isClicked() && !_lpMiniGameCurrent)
      {
         if (!_lpMap->isVisible() && !_lpMap->isClosing()) {
            ShowMapManager(true);
            ESoundBank::getSound("mapopen")->playSample();
         }
         else {
            ShowMapManager(false);
            ESoundBank::getSound("mapclose")->playSample();
         }
         
         Mouse.LeftEventClose();
         return;
      }
   }
   if (_lpsimgIHM->isVisible() && (_lpButtonDiaryClosed->isClicked() || _lpButtonDiaryOpen->isClicked()) && !_lpMiniGameCurrent)
   {
      if (!_lpBookCtrl->isVisible(DIARY)  && !_lpBookCtrl->isClosing()) {
         ShowDiaryManager(true);
      }
      else {
         ShowDiaryManager(false);
      }
      
      Mouse.LeftEventClose();
      return;
   }
   if (_lpsimgIHM->isVisible() && _lpButtonMenu->isClicked() && !_lpMiniGameCurrent)
   {
      K_LOG("ESceneDirector::Check()-> _lpButtonMenu clicked");
      _lpSceneDirector->getSequencer()->Reset();
      if (_lpVignetteCurrent) {
         _lpSceneDirector->getSequencer()->CloseVignette(NULL);
      }
      _lpSceneDirector->getSequencer()->GoToScene(NULL, "menu");
      
      Mouse.LeftEventClose();
      return;
   }
   
   // Est-ce que le clic est en dehors de la zone de scène ?
   if (Mouse.y > DIRECTOR_IHMY && _lpsimgIHM->isVisible())
   {
      bCheckAllowed = false;
   }
   
   bool bFadeInCompleted = false;
   bool bFadeOutCompleted = false;
   /* SCENE */
   if (!_lpVignetteCurrent && !_lpMiniGameCurrent)
   {
      bFadeInCompleted = _lpSceneCurrent->FadeInCompleted();
      bFadeOutCompleted = _lpSceneCurrent->FadeOutCompleted();
      if (bFadeInCompleted && bFadeOutCompleted && bCheckAllowed) {
         _lpSceneCurrent->Check();
      }
   }
   else {
      /* VIGNETTE */
      if (_lpVignetteCurrent && !_lpMiniGameCurrent) {
         bFadeInCompleted = _lpVignetteCurrent->FadeInCompleted();
         bFadeOutCompleted = _lpVignetteCurrent->FadeOutCompleted();
         if (bFadeInCompleted && bFadeOutCompleted && bCheckAllowed) {
            _lpVignetteCurrent->Check();
         }
      }
      else
      /* MINI JEU */
         if (_lpMiniGameCurrent) {
            bFadeInCompleted = _lpMiniGameCurrent->FadeInCompleted();
            bFadeOutCompleted = _lpMiniGameCurrent->FadeOutCompleted();
            if (bFadeInCompleted && bFadeOutCompleted && bCheckAllowed) {
               _lpMiniGameCurrent->Check();
            }
         }
   }
   
   /* Séquenceur, ne fonctionne que quand la scène est totalement visible */
   //   if (bFadeInCompleted && bFadeOutCompleted && _lpSceneSequencer->isActive()) {
   if (bFadeInCompleted && _lpSceneSequencer->isActive()) {
      _lpSceneSequencer->Check();
   }
   
   // Curseur souris selon la situation...
   if (!bCheckAllowed && _bWasCheckNotAllowed == false)
   {
      _bWasCheckNotAllowed = true;
      // Si on est sur l'inventaire : pointeur normal
      if (_lpInventory->Check() || Mouse.y > DIRECTOR_IHMY) {
         ChangeMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
      } else {
         // Si on est dans la scène, pointeur indiquant qu'on ne peut pas agir
         ChangeMouseCursor(DIRECTOR_MOUSECURSOR_WAIT);
      }
      // Reset le système de survol de la scène, car elle a perdu le focus au profit de l'inventaire
      if (_lpSceneCurrent) _lpSceneCurrent->ResetMouseOver();
      if (_lpVignetteCurrent) _lpVignetteCurrent->ResetMouseOver();
   } else {
      // On repasse d'un état Check not allowed à Check allowed, donc on reset le survol souris
      if (bCheckAllowed &&_bWasCheckNotAllowed) {
         _bWasCheckNotAllowed = false;
         if (_lpSceneCurrent) _lpSceneCurrent->ResetMouseOver();
         if (_lpVignetteCurrent) _lpVignetteCurrent->ResetMouseOver();
      }
   }
}

void ESceneDirector::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   
   KGame::getGame()->_lpKWindow->setClearColor(0, 0, 0, 1);
   
   // Musique
   _lpMusicArranger->Render(fElapsed);
   
   if (_lpAnimEarthQuake) {
      _lpAnimEarthQuake->move(fElapsed);
      if (_lpAnimEarthQuake->isCompleted()) {
         XDELETE(_lpAnimEarthQuake);
      }
   }
   
   if (!_lpAnimSceneOut->isCompleted())
   {
      _lpAnimSceneOut->move(fElapsed);
   }
   
   /* Tremblement de terre */
   if (_lpAnimEarthQuake) {
      if (_lpAnimEarthQuake->isCompleted() == false) {
         int rndx,rndy,rnda;
         rndx = (((5 - (-5))+1)*(KRandom::getRandomFloat()) + (-5)) * _lpAnimEarthQuake->getCurrentValue();
         rndy = (((5 - (-5))+1)*(KRandom::getRandomFloat()) + (-5)) * _lpAnimEarthQuake->getCurrentValue();
         rnda = 0;//(((1 - (-1))+1)*(KRandom::getRandomFloat()) + (-1));
         KGame::getGame()->_lpKWindow->setWorldView(rndx,rndy,rnda,1,true);
      }
   }
   
   if (_lpSceneCurrent && !_lpMiniGameCurrent)
   {
      // :dmekersa:20110323 Ne pas dessiner la scène si il y a une vignette non transparente dessus
      bool _bDrawUnderScene = true;
      if (_lpVignetteCurrent) {
         if (_lpVignetteCurrent->isAlpha() == false) {
            _bDrawUnderScene = false;
         }
      }
      if (_bDrawUnderScene) {
         bool bWorldView = false;
         if (_bSceneNext && _bSceneZoomin) {
            bWorldView = true;
            KGame::getGame()->_lpKWindow->setWorldView(0,0,0,_lpAnimSceneOut->getCurrentValue(),false);
         }
         _lpSceneCurrent->Draw();
         if (bWorldView) {
            KGame::getGame()->_lpKWindow->setDefaultWorldView();
         }
      }
   }
   
   if (_lpVignetteCurrent && !_lpMiniGameCurrent)
   {
      _lpVignetteCurrent->Draw();
   }
   
   if (_lpMiniGameCurrent)
   {
      _lpMiniGameCurrent->Draw();
   }
   
   /* IHM */
   if (!_bLoading) {
      DrawIHM();
      _lpParticleEmitterDiary->Render(fElapsed);
   }
   
   // Interraction avec l'inventaire
   
   _lpTravellingEmitter->Render(fElapsed);
   if (_lpTravellingItem) {
      _lpTravellingItem->Draw(1);
   }
   
   /* Mode cinématique : cinémascope */
   if (_bCinematicMode) {
      _lpBgGraphic->drawRect(0, 0, 1024, 100, 0, 0, 0, 0.7f);
      _lpBgGraphic->drawRect(0, 768-100, 1024, 768, 0, 0, 0, 0.7f);
   }   
   
   // Emetteur pour l'effet spirit du hint system
   //   _lpParticleEmitterDialogSpirit->Render(fElapsed);
   
   /* Draw UI */
   KUIElement::blitAll (0);
   
   bool bFadeInCompleted = false;
   if (!_lpVignetteCurrent)
   {
      if (_lpSceneCurrent) {
         bFadeInCompleted = _lpSceneCurrent->FadeInCompleted();
      }
   }
   else {
      if (_lpVignetteCurrent) {
         bFadeInCompleted = _lpVignetteCurrent->FadeInCompleted();
      }
   }
   
   // affiche la sequence -> nouveau objectif -> si existe
   if(!_bDialogActive) 
      _lpBookCtrl->getTaskListPtr()->renderSequence();
   
   if (bFadeInCompleted) {
      _lpSceneSequencer->Draw();
   }
   
   // Liste des tâches
   if (_lpSceneCurrent) {
      _lpBookCtrl->Render();
   }
   
   // affichage carte
   _lpMap->Render();   
   
   /* Item inventaire */
   if (_lpHoldItem) {
      _lpHoldItem->Draw(.5f);
   }
   
   /* Particules d'énergie */
   _lpEnergyParticlesManager->Render();
   
   /* Loading mode */
   if (_bLoading) {
      KTextFace *lpFont = EFontBank::getFont("tahoma.ttf",12);
      lpFont->setColor(1.0f, 1.0f, 1.0f, 1.0f);
      lpFont->drawStringCentered("Loading...", 0, 1024, 620);
   }
   
   /* Mouse pointer */
   if (!_bSystemCursor && _bSoftCursor )
      _lpsimgArrow->Draw();
   /* Mouse pointer info */
   if (_lpsimgArrow->getCurrentY()<DIRECTOR_IHMY) {
      const char *szMOInfo = NULL;
      int nXMO, nYMO;
      int nFontWidth;
      int nFontHeight;
      if (_lpVignetteCurrent) {
         szMOInfo = _lpVignetteCurrent->GetMouseOverInfo();
      }
      else if (_lpSceneCurrent) {
         szMOInfo = _lpSceneCurrent->GetMouseOverInfo();
      } 
      if (szMOInfo) {
         nXMO = _lpsimgArrow->getCurrentX() + (_lpsimgArrow->getWidth()/2);
         nYMO = _lpsimgArrow->getCurrentY() + _lpsimgArrow->getHeight();
         nFontWidth = _lpFontMouseOverInfo->getMultilineWidth(szMOInfo, nXMO, nXMO, nYMO, TEXTSTYLE_LEFT);
         nFontHeight = _lpFontMouseOverInfo->getHeightPix();
         if (nXMO + (nFontWidth/2) > 1024) {
            nXMO = 1024 - (nFontWidth/2);
         }
         if (nXMO - (nFontWidth/2) < 0) {
            nXMO = 0 + (nFontWidth/2);
         }
         if (nYMO > 768) {
            nYMO = _lpsimgArrow->getCurrentY() - nFontHeight;
         }
         _lpSceneCurrent->DrawFancyString(szMOInfo, _lpFontMouseOverInfo, nXMO, nYMO, 0, true, 1);
      }
   }

   EImageBank::PurgeCache();
   ESoundBank::PurgeCache();
   EEmitterBank::PurgeCache();
}

/* Dessine l'IHM */
void ESceneDirector::DrawIHM()
{
   // IHM et inventaire
   _lpsimgIHM->Draw();
   _lpsimgHexagramme->Draw();
   _lpsimgHexagramme_blue->Draw();
   _lpsimgHexagramme_purple->Draw();
   _lpsimgHexagramme_yellow->Draw();
   _lpsimgHexagramme_red->Draw();
   _lpsimgHexagramme_green->Draw();
   // Inventaire
   if (_lpsimgIHM->isVisible()) {
      _lpInventory->Draw(_lpsimgIHM->getFade());
   }
   
   // Personnage courant
   _lpsimgDialogCache->Draw();
   _lpsimgCharacterHouse->Draw();
   _lpsimgCharacterHouseDialog->Draw();
   _lpsimgCharacterUniverseDialog->Draw();
   _lpsimgCharacterIsland->Draw();
   _lpsimgCharacterJapan->Draw();
   _lpsimgCharacterInca->Draw();
   _lpsimgCharacterEgypt->Draw();
   _lpsimgCharacterMiddleage->Draw();
   _lpsimgCharacterHell->Draw();
   
   if (_lpsimgIHM->isVisible()) {
      if (getDiaryPtr()->isUnread()) {
         _lpIHMDiaryOpenSmartImage->Draw();
         if (!_lpParticleEmitterDiary->IsStarted() && _lpIHMDiaryOpenSmartImage->isVisible())
            _lpParticleEmitterDiary->Start();
      }
      else {
         if (_lpParticleEmitterDiary->IsStarted())
            _lpParticleEmitterDiary->Stop();
      }
   }
   else {
      _lpParticleEmitterDiary->Stop();
   }
}

/* Démarre un tremblement de terre ! */
void ESceneDirector::EarthQuake(bool b, const char *szSoundName)
{
   if (b)
   {
      if (!_lpAnimEarthQuake) {
         _lpAnimEarthQuake = new KCounter;
         _lpAnimEarthQuake->startCounter(1, 0, 0, 30000, K_COUNTER_EASEOUT);
         if (szSoundName == NULL) {
            ESoundBank::getSound("earthquake")->playSample();
         }
         else {
            ESoundBank::getSound(szSoundName)->playSample();
         }
      }
   }
   else {
      // On force l'anim à finir
      if (_lpAnimEarthQuake)
      {
         _lpAnimEarthQuake->move(999999);
         if (szSoundName == NULL) {
            ESoundBank::StopSample("earthquake");
         }
         else {
            if (strlen(szSoundName) > 0) {
               ESoundBank::StopSample(szSoundName);
            }
         }
      }
   }
   
}

/* Démarre le trajet d'un objet depuis la scène vers l'inventaire... */
EInventoryItem *ESceneDirector::PickupItem(const char *szItemName, float fFromX, float fFromY, int nNbPartNeeded, int nNumPart)
{
   // 17/08/11 Programmation défensive pour éviter Crash
   if (!_lpSceneCurrent) {
      return NULL;
   }
   
   // Il y a déjà un objet en cours de ramassage, on l'intègre direct à l'inventaire
   if (_lpInventory->getTravellingItem())
   {
      EndTravellingItem();
   }
   // Item visuellement
   std::string strUniverse;
   strUniverse = _lpSceneCurrent->_strSceneUniverseName;

   _lpTravellingItem = new EInventoryItem(strUniverse.c_str(), szItemName, nNbPartNeeded, nNumPart);
   _lpTravellingItem->setPos(fFromX, fFromY, true);
#ifdef DEBUG
   const char *invImgPath = EImageBank::GetCache(_lpTravellingItem->_lpsimgImage->getGraphic())->GetImgPath();
   K_LOG("InventoryItem: pickupscenediretor, scene:%s, object:%s, invimg:%s",_lpSceneDirector->GetCurrentEScene()->_strSceneName.c_str(), szItemName,invImgPath);
#endif

   // Destination
   int xDest, yDest;
   // Si ce n'est pas l'hexagramme ou une sphere, cela va dans l'inventaire
   if (strcmp(szItemName, "inv_hexagramme") == 0) {
      xDest = 965;
      yDest = 666;
   }
   else if (strcmp(szItemName, "inv_map") == 0) {
      xDest = 955;
      yDest = 750;
   }
   else if (strcmp(szItemName, "ihm_envelope") == 0) {
      xDest = 44;
      yDest = 34;
   }   
   else {
      _lpInventory->getFreeCell(szItemName, xDest, yDest);
      _lpInventory->setTravellingItem(_lpTravellingItem);
   }
   UpdateInventoryButtons();
   
   // Centre l'image sur cette destination
   fFromX -= (_lpTravellingItem->_lpsimgImage->getGraphic()->getWidth()/2);
   fFromY -= (_lpTravellingItem->_lpsimgImage->getGraphic()->getHeight()/2);
   xDest -= (int)(_lpTravellingItem->_lpsimgImage->getGraphic()->getWidth()/2);
   yDest -= (int)(_lpTravellingItem->_lpsimgImage->getGraphic()->getHeight()/2);
   // Démarre une interpolation entre la position de l'objet à l'écran et la cellule visée
   // Pour plus de facilité on utilise la smartimage de l'item
   _lpTravellingItem->_lpsimgImage->Move(fFromX, fFromY);
   _lpTravellingItem->_lpsimgImage->SetRotation(0, 360, 800.0f, false, K_COUNTER_EASEIN);
   _lpTravellingItem->_lpsimgImage->Rotate(true, -1);
   _lpTravellingItem->_lpsimgImage->AnimateP2P((float)xDest, (float)yDest, 800.0f, false, K_COUNTER_EASEOUT, 0);
   ESoundBank::getSound("slide")->playSample();
   /* Effet de particules */
   _lpTravellingEmitter->Start();
   
   return _lpTravellingItem;
}

void ESceneDirector::HoldItem(const char *szItemName)
{
   // On lache l'objet qu'on avait déjà
   if (_lpHoldItem) {
      delete _lpHoldItem;
      _lpHoldItem = NULL;
   }
   // On prend l'objet désigné par son nom
   _lpHoldItem = new EInventoryItem("", szItemName, 1, 1);
}

void ESceneDirector::resetHoldItem()
{
   XDELETE(_lpHoldItem);
}

void ESceneDirector::DropItem(const char *szItemName)
{
   _lpInventory->DropItem(szItemName);
}

const char* ESceneDirector::MapPartialInvItemFullInvItem(const char* szInvItemName) {
   // Hall / Interrupteur
   if (strcmp(szInvItemName, "inv_switchpieces") == 0)
      return "inv_switchrepaired";
   // Diningroom / Vase
   if (strcmp(szInvItemName, "inv_vasepieces") == 0)
     return "inv_dining_vase";
   // Livingroom / pages
   if (strcmp(szInvItemName, "inv_livingroom_page") == 0)
      return "inv_livingroom_book";
   // Island Pendentif
   if (strcmp(szInvItemName, "inv_island_neckless1") == 0)
      return "inv_island_neckless_full";
   // Bathroom
   if (strcmp(szInvItemName, "inv_bath_flacon_yellow_part") == 0)
      return "inv_bath_flacon_yellow_full";
   if (strcmp(szInvItemName, "inv_bath_flacon_red_part") == 0)
      return "inv_bath_flacon_red_full";
   if (strcmp(szInvItemName, "inv_bath_flacon_blue_part") == 0)
      return "inv_bath_flacon_blue_full";
   if (strcmp(szInvItemName, "inv_bath_flacon_green_part") == 0)
      return "inv_bath_flacon_green_full";
   // Moyen Age / Fiole
   if (strcmp(szInvItemName, "inv_middleage_flaskpieces") == 0)
      return "inv_middleage_flask_empty";
   // Inca / Lance
   if (strcmp(szInvItemName, "inv_inca_spearbroken") == 0)
      return "inv_inca_spearrepaired";
   // Inca / Fresque1
   if (strcmp(szInvItemName, "inv_inca_fresco1broken") == 0)
      return "inv_inca_fresco1repaired";
   // Inca / Fresque2
   if (strcmp(szInvItemName, "inv_inca_fresco2broken") == 0)
      return "inv_inca_fresco2repaired";
   // Japon / Table
   if (strcmp(szInvItemName, "inv_japan_tablepieces") == 0)
      return "inv_japan_table";
   // Enfer / Canne
   if (strcmp(szInvItemName, "inv_hell_item_cane_part") == 0)
      return "inv_hell_item_cane_full";
   return "";
}

void ESceneDirector::ItemIsFull(const char *szItemName)
{
   const char * tmp = MapPartialInvItemFullInvItem(szItemName);
   KAssert(strlen(tmp)>0);

   if (strcmp(szItemName, "inv_vasepieces") == 0)
   {
      getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INVENTORY_ASHLEY_BESTMOSAIC"),"", true);
   }
   // Island Pendentif
   if (strcmp(szItemName, "inv_island_neckless1") == 0)
   {
      GoToScene("island_beach", "completed", false);
   }
   // Inca / Lance
   if (strcmp(szItemName, "inv_inca_spearbroken") == 0)
   {
      ResolveTask("task_inca_findspear");
      AddTask("task_inca_givespear");
   }
   // Inca / Fresque1
   if (strcmp(szItemName, "inv_inca_fresco1broken") == 0)
   {
      ResolveTask("task_inca_findfresco1");
      AddTask("task_inca_repairfresco1");
   }
   // Inca / Fresque2
   if (strcmp(szItemName, "inv_inca_fresco2broken") == 0)
   {
      ResolveTask("task_inca_findfresco2");
      AddTask("task_inca_repairfresco2");
   }
   _lpInventory->ReplaceItem(szItemName, tmp);
}

bool ESceneDirector::CombineItem(const char *szItemNameInv, const char *szItemNameHold)
{
   // Dans la cave, on combine la pince cassée et la vis
   if ((strcmp(szItemNameInv, "inv_house_basement_gripbroken") == 0 && strcmp(szItemNameHold, "inv_house_basement_bolt") == 0)) {
      ESoundBank::getSound("success")->playSample();
      _lpInventory->ReplaceItem(szItemNameInv,"inv_house_basement_grip");
      _lpInventory->DropItem(szItemNameHold);
      resetHoldItem();
      getSequencer()->VoiceOver(NULL, KStr("INVENTORY_ASHLEY_WRISTFIXED"),"");
      return true;
   }
   // Au japon, on combile meal et boite à bento
   if ((strcmp(szItemNameInv, "inv_japan_bentoempty") == 0 && strcmp(szItemNameHold, "inv_japan_meal") == 0)) {
      ESoundBank::getSound("success")->playSample();
      _lpInventory->DropItem(szItemNameHold);
      _lpInventory->ReplaceItem(szItemNameInv,"inv_japan_bentofull");
      resetHoldItem();
      getSequencer()->Talk("ashleyglobalnoeffect", CHARACTER_POSX, CHARACTER_POSY, KStr("INVENTORY_ASHLEY_BENTO"),"", true);
      return true;
   }
   // Au japon, on combile stick et holder pour l'encens
   if ((strcmp(szItemNameInv, "inv_japan_encensholder") == 0 && strcmp(szItemNameHold, "inv_japan_encensstick") == 0)) {
      ESoundBank::getSound("success")->playSample();
      _lpInventory->DropItem(szItemNameHold);
      _lpInventory->ReplaceItem(szItemNameInv,"inv_japan_encensfull");
      resetHoldItem();
      return true;
   }
   // Au japon, on combile encens et torche pour l'encens
   if ((strcmp(szItemNameInv, "inv_japan_encensfull") == 0 && strcmp(szItemNameHold, "inv_japan_torchon") == 0)) {
      resetHoldItem();
      getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INVENTORY_ASHLEY_ENCENSNO"),"", true);
      return true;
   }
   if ((strcmp(szItemNameInv, "inv_japan_encensstick") == 0 && strcmp(szItemNameHold, "inv_japan_torchon") == 0)) {
      resetHoldItem();
      getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INVENTORY_ASHLEY_ENCENSNO"),"", true);
      return true;
   }
   // Au moyen age on utilise la serpe sur le bout de liège
   if ((strcmp(szItemNameInv, "inv_middleage_cork") == 0 && strcmp(szItemNameHold, "inv_middleage_sickle") == 0)) {
      ESoundBank::getSound("success")->playSample();
      // On n'enlève pas la serpe de l'inventaire !
      _lpInventory->ReplaceItem(szItemNameInv,"inv_middleage_cap");
      resetHoldItem();
      return true;
   }
   // Au moyen age on utilise la serpe sur le bout de bois
   if ((strcmp(szItemNameInv, "inv_middleage_wood") == 0 && strcmp(szItemNameHold, "inv_middleage_sickle") == 0)) {
      ESoundBank::getSound("success")->playSample();
      // On n'enlève pas la serpe de l'inventaire !
      _lpInventory->ReplaceItem(szItemNameInv,"inv_middleage_pestle");
      getSequencer()->VoiceOver(NULL, KStr("INVENTORY_MONK_PESTLE"),"");
      RemoveHint("middleage","help","pestle");
      resetHoldItem();
      return true;
   }
   // En enfer, on combine le sable et le sablier
   if ((strcmp(szItemNameInv, "inv_hell_item_hourglass_empty") == 0 && strcmp(szItemNameHold, "inv_hell_sand") == 0)
       || (strcmp(szItemNameHold, "inv_hell_item_hourglass_empty") == 0 && strcmp(szItemNameInv, "inv_hell_sand") == 0)
       ) {
      ESoundBank::getSound("success")->playSample();
      _lpInventory->DropItem(szItemNameHold);
      _lpInventory->ReplaceItem(szItemNameInv,"inv_hell_item_hourglass_full");
      resetHoldItem();
      return true;
   }
   // En enfer, on combine la corde et le baton pour faire une fronde
   if ((strcmp(szItemNameInv, "inv_hell_sling_rope") == 0 && strcmp(szItemNameHold, "inv_hell_sling_stick") == 0)
       || 
       (strcmp(szItemNameHold, "inv_hell_sling_rope") == 0 && strcmp(szItemNameInv, "inv_hell_sling_stick") == 0)
       ) {
      ESoundBank::getSound("success")->playSample();
      _lpInventory->DropItem(szItemNameHold);
      _lpInventory->ReplaceItem(szItemNameInv,"inv_hell_sling");
      _lpInventory->getItemFromInventory("inv_hell_sling")->_nNbPartNeeded = -1;
      resetHoldItem();
      return true;
   }
   // En enfer, on combine la pierre et la fronde pour l'armer
   if ((strcmp(szItemNameInv, "inv_hell_sling") == 0 && strcmp(szItemNameHold, "inv_hell_stone") == 0)
       || (strcmp(szItemNameHold, "inv_hell_sling") == 0 && strcmp(szItemNameInv, "inv_hell_stone") == 0)
       ) {
      ESoundBank::getSound("success")->playSample();
      _lpInventory->DropItem(szItemNameHold);
      _lpInventory->ReplaceItem(szItemNameInv,"inv_hell_sling_armed");
      resetHoldItem();
      return true;
   }
   // Dans la maison, on combine la corde et les brindilles pour faire un fagot
   if ((strcmp(szItemNameInv, "inv_house_brushwood") == 0 && strcmp(szItemNameHold, "inv_house_cordon") == 0)
       || 
       (strcmp(szItemNameHold, "inv_house_brushwood") == 0 && strcmp(szItemNameInv, "inv_house_cordon") == 0)
       ) {
      ESoundBank::getSound("success")->playSample();
      _lpInventory->DropItem(szItemNameHold);
      _lpInventory->ReplaceItem(szItemNameInv,"inv_house_brushwood_fagot");
      resetHoldItem();
      return true;
   }
   
   // Dans la maison, on combine le briquet et l'essence à briquet
   if ((strcmp(szItemNameInv, "inv_house_zippo_empty") == 0 && strcmp(szItemNameHold, "inv_house_zippo_fluid") == 0)
       || 
       (strcmp(szItemNameHold, "inv_house_zippo_empty") == 0 && strcmp(szItemNameInv, "inv_house_zippo_fluid") == 0)
       ) {
      ESoundBank::getSound("success")->playSample();
      _lpInventory->DropItem(szItemNameHold);
      _lpInventory->ReplaceItem(szItemNameInv,"inv_house_zippo_full");
      getSequencer()->VoiceOver(NULL, KStr("INVENTORY_ASHLEY_ZIPPO"),"");
      resetHoldItem();
      return true;
   }
   
   // Dans la maison, le joueur essayer d'enflammer les brindilles dans son inventaire
   if ((strcmp(szItemNameInv, "inv_house_brushwood_fagot") == 0 && strcmp(szItemNameHold, "inv_house_zippo_full") == 0)) {
      getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INVENTORY_ASHLEY_FIRE"),"", true);
      resetHoldItem();
      return true;
   }
   
   // Sur l'ile, on combine la sarbacane et la baie
   if ((strcmp(szItemNameInv, "inv_island_berry") == 0 && strcmp(szItemNameHold, "inv_island_blowpipe") == 0)
       || 
       (strcmp(szItemNameHold, "inv_island_berry") == 0 && strcmp(szItemNameInv, "inv_island_blowpipe") == 0)
       ) {
      ESoundBank::getSound("success")->playSample();
      _lpInventory->DropItem(szItemNameHold);
      _lpInventory->ReplaceItem(szItemNameInv,"inv_island_blowpipearmed");
      getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("INVENTORY_MURRAY_BLOWPIPE"),"", true);
      resetHoldItem();
      return true;
   }
   
   // Sur l'ile, on combine le manche de pelle et la tête de pelle
   if ((strcmp(szItemNameInv, "inv_island_shovelhandle") == 0 && strcmp(szItemNameHold, "inv_island_shovelhead") == 0)
       || 
       (strcmp(szItemNameHold, "inv_island_shovelhandle") == 0 && strcmp(szItemNameInv, "inv_island_shovelhead") == 0)
       ) {
      ESoundBank::getSound("success")->playSample();
      _lpInventory->DropItem(szItemNameHold);
      _lpInventory->ReplaceItem(szItemNameInv,"inv_island_shovelfull");
      resetHoldItem();
      return true;
   }
   
   // Sur l'ile, on combine la corde sur la pelle cassée
   if ((strcmp(szItemNameInv, "inv_island_shovelbroken") == 0 && strcmp(szItemNameHold, "inv_island_rope") == 0)
       || 
       (strcmp(szItemNameHold, "inv_island_rope") == 0 && strcmp(szItemNameInv, "inv_island_shovelfull") == 0)
       ) {
      ResolveObjective("island","shovel");
      ESoundBank::getSound("success")->playSample();
      _lpInventory->DropItem(szItemNameHold);
      _lpInventory->ReplaceItem(szItemNameInv,"inv_island_shovelstrong");
      resetHoldItem();
      return true;
   }
   
   // On combine la corde et le grapin
   if ((strcmp(szItemNameInv, "inv_house_rope") == 0 && strcmp(szItemNameHold, "inv_house_hook") == 0)
       || 
       (strcmp(szItemNameHold, "inv_house_rope") == 0 && strcmp(szItemNameInv, "inv_house_hook") == 0)
       ) {
      ESoundBank::getSound("success")->playSample();
      _lpInventory->DropItem(szItemNameHold);
      _lpInventory->ReplaceItem(szItemNameInv,"inv_house_ropehook");
      resetHoldItem();
      return true;
   }
   
   getSequencer()->VoiceOver(NULL, KStr("GLOBAL_ASHLEY_NOEFFECT"),"");
   resetHoldItem();
   return true;
}

void ESceneDirector::NiceTryVoiceOver()
{
   char sz[256];
   memset(sz, 0, 256);
   strcpy(sz, "GLOBAL_ASHLEY_NOEFFECT");
   int rnd  = 0;
   while (rnd <= 0 || rnd > 8) {
      rnd = (((8 - (1))+1)*(KRandom::getRandomFloat()) + (1));
   }
   strcat(sz, itos(rnd).c_str());
   getSequencer()->VoiceOver("nice try", KStr(sz), "");
}

bool ESceneDirector::CreateSceneFromName(const char *szSceneName, EScene *&lpScene, const char *szAdditionalName, bool bLoadOnly)
{
   EWatchDog::SceneTransition(true);
   KAssert(lpScene == NULL);
   KAssert(Loader::IsImgThreadEnabled());

   _strLastParsedSceneName = szSceneName;

   if (strcmp(szSceneName, "menu") == 0)
   {
      lpScene = new SceneMenu(this);
   }
   else if (strcmp(szSceneName, "mode") == 0)
   {
      lpScene = new Scene_Mode(this);
   }
   else if (strcmp(szSceneName, "loading") == 0)
   {
      lpScene = new Scene_Loading(this);
   }
   else if (strcmp(szSceneName, "title") == 0)
   {
      lpScene = new SceneTitle(this);
   }
   else if (strcmp(szSceneName, "video_player") == 0)
   {
      lpScene = new Scene_Video_Player(this);
   }
   else if (strcmp(szSceneName, "title_splashscreen") == 0)
   {
      lpScene = new Scene_Title_Splashscreen(this);
   }
   else if (strcmp(szSceneName, "pirate") == 0)
   {
      lpScene = new SceneTest(this);
   }
   else if (strcmp(szSceneName, "island_dive") == 0)
   {
      lpScene = new SceneFondMarin(this);
   }
   else if (strcmp(szSceneName, "house_out") == 0)
   {
      lpScene = new Scene_House_Out(this);
   }
   else if (strcmp(szSceneName, "house_out_car") == 0)
   {
      lpScene = new Scene_House_Out_Car(this);
   }
   else if (strcmp(szSceneName, "parvis") == 0)
   {
      lpScene = new SceneParvis(this);
   }
   else if (strcmp(szSceneName, "hall") == 0)
   {
      lpScene = new SceneHall(this);
   }
   else if (strcmp(szSceneName, "house_hall") == 0)
   {
      lpScene = new Scene_House_Hall(this);
   }
   else if (strcmp(szSceneName, "halldoor") == 0)
   {
      lpScene = new SceneHallDoor(this);
   }
   else if (strcmp(szSceneName, "corridor") == 0)
   {
      lpScene = new SceneCorridor(this);
   }
   else if (strcmp(szSceneName, "keyhole") == 0)
   {
      lpScene = new Scene_House_Keyhole(this);
   }
   else if (strcmp(szSceneName, "stairsdown") == 0)
   {
      lpScene = new SceneStairsDown(this);
   }
   else if (strcmp(szSceneName, "house_livingroom") == 0)
   {
      lpScene = new SceneLivingRoom(this);
   }
   else if (strcmp(szSceneName, "house_livingroom_table") == 0)
   {
      lpScene = new Scene_House_Livingroom_Table(this);
   }
   else if (strcmp(szSceneName, "house_livingroom_box") == 0)
   {
      lpScene = new Scene_House_Livingroom_Box(this);
   }
   else if (strcmp(szSceneName, "house_livingroom_piano") == 0)
   {
      lpScene = new Scene_House_Piano(this);
   }
   else if (strcmp(szSceneName, "house_incabook") == 0)
   {
      lpScene = new Scene_House_IncaBook(this);
   }
   else if (strcmp(szSceneName, "house_diningroom") == 0)
   {
      lpScene = new SceneDiningRoom(this);
   }
   else if (strcmp(szSceneName, "closet") == 0)
   {
      lpScene = new SceneCloset(this);
   }
   else if (strcmp(szSceneName, "kitchen") == 0)
   {
      lpScene = new SceneKitchen(this);
   }
   else if (strcmp(szSceneName, "basement") == 0)
   {
      lpScene = new SceneBasement(this);
   }
   else if (strcmp(szSceneName, "basement_grid") == 0)
   {
      lpScene = new Scene_House_Basement_Grid(this);
   }
   else if (strcmp(szSceneName, "firstfloor") == 0)
   {
      lpScene = new SceneFirstfloor(this);
   }
   else if (strcmp(szSceneName, "room1") == 0)
   {
      lpScene = new SceneRoom1(this);
   }
   else if (strcmp(szSceneName, "house_room2") == 0)
   {
      lpScene = new SceneRoom2(this);
   }
   else if (strcmp(szSceneName, "house_room2_bedside") == 0)
   {
      lpScene = new Scene_House_Room2_Bedside(this);
   }
   else if (strcmp(szSceneName, "house_room2_mirror") == 0)
   {
      lpScene = new Scene_House_Room2_Mirror(this);
   }
   else if (strcmp(szSceneName, "bathroom") == 0)
   {
      lpScene = new SceneBathroom(this);
   }
   else if (strcmp(szSceneName, "bathroom_tub") == 0)
   {
      lpScene = new Scene_House_Bathroom_Tub(this);
   }
   else if (strcmp(szSceneName, "bathroom_cabinet") == 0)
   {
      lpScene = new Scene_House_Bathroom_Cabinet(this);
   }
   else if (strcmp(szSceneName, "house_atticdoor") == 0)
   {
      lpScene = new SceneAtticDoor(this);
   }
   else if (strcmp(szSceneName, "house_attic") == 0)
   {
      lpScene = new SceneAttic(this);
   }
   else if (strcmp(szSceneName, "house_attic_pentacle") == 0)
   {
      lpScene = new Scene_House_Attic_Pentacle(this);
   }
   else if (strcmp(szSceneName, "island_beach") == 0)
   {
      lpScene = new Scene_Island_Beach(this);
   }
   else if (strcmp(szSceneName, "island_falaise") == 0)
   {
      lpScene = new Scene_Island_Falaise(this);
   }
   else if (strcmp(szSceneName, "island_cave") == 0)
   {
      lpScene = new Scene_Island_Cave(this);
   }
   else if (strcmp(szSceneName, "island_creek") == 0)
   {
      lpScene = new Scene_Island_Creek(this);
   }
   else if (strcmp(szSceneName, "vignette_island_safe") == 0)
   {
      lpScene = new Vignette_Island_Safe(this);
   }
   else if (strcmp(szSceneName, "japan_mainscreen") == 0)
   {
      lpScene = new Scene_Japan_Mainscreen(this);
   }
   else if (strcmp(szSceneName, "japan_garden") == 0)
   {
      lpScene = new Scene_Japan_Garden(this);
   }
   else if (strcmp(szSceneName, "japan_bedroom") == 0)
   {
      lpScene = new Scene_Japan_Bedroom(this);
   }
   else if (strcmp(szSceneName, "inca_mainscreen") == 0)
   {
      lpScene = new Scene_Inca_Mainscreen(this);
   }
   else if (strcmp(szSceneName, "inca_passage") == 0)
   {
      lpScene = new Scene_Inca_Passage(this);
   }
   else if (strcmp(szSceneName, "inca_stairs") == 0)
   {
      lpScene = new Scene_Inca_Stairs(this);
   }
   else if (strcmp(szSceneName, "inca_final") == 0)
   {
      lpScene = new Scene_Inca_Final(this);
   }
   else if (strcmp(szSceneName, "inca_autel") == 0)
   {
      lpScene = new Scene_Inca_Autel(this);
   }
   else if (strcmp(szSceneName, "vignette_inca_statuette") == 0)
   {
      lpScene = new Vignette_Inca_Statuette(this);
   }
   else if (strcmp(szSceneName, "middleage_mainscreen") == 0)
   {
      lpScene = new Scene_Middleage_Mainscreen(this);
   }
   else if (strcmp(szSceneName, "middleage_cloister") == 0)
   {
      lpScene = new Scene_Middleage_Cloister(this);
   }
   else if (strcmp(szSceneName, "middleage_stairs") == 0)
   {
      lpScene = new Scene_Middleage_Stairs(this);
   }
   else if (strcmp(szSceneName, "middleage_jail") == 0)
   {
      lpScene = new Scene_Middleage_Jail(this);
   }
   else if (strcmp(szSceneName, "egypt_land") == 0)
   {
      lpScene = new Scene_Egypt_Land(this);
   }
   else if (strcmp(szSceneName, "egypt_shop") == 0)
   {
      lpScene = new Scene_Egypt_Shop(this);
   }
   else if (strcmp(szSceneName, "egypt_mazeentry") == 0)
   {
      lpScene = new Scene_Egypt_Mazeentry(this);
   }
   else if (strcmp(szSceneName, "egypt_keyroom") == 0)
   {
      lpScene = new Scene_Egypt_Keyroom(this);
   }
   else if (strcmp(szSceneName, "egypt_keyroom_zoom") == 0)
   {
      lpScene = new Scene_Egypt_Keyroom_Zoom(this);
   }
   else if (strcmp(szSceneName, "egypt_maze") == 0)
   {
      lpScene = new Scene_Egypt_Maze(this);
   }
   else if (strcmp(szSceneName, "egypt_boat") == 0)
   {
      lpScene = new Scene_Egypt_Boat(this);
   }
   else if (strcmp(szSceneName, "egypt_boatroom") == 0)
   {
      lpScene = new Scene_Egypt_Boatroom(this);
   }
   else if (strcmp(szSceneName, "egypt_tomb") == 0)
   {
      lpScene = new Scene_Egypt_Tomb(this);
   }
   else if (strcmp(szSceneName, "egypt_tomb_connect") == 0)
   {
      lpScene = new Scene_Egypt_Tomb_Connect(this);
   }
   else if (strcmp(szSceneName, "hell_mainscreen") == 0)
   {
      lpScene = new Scene_Hell_Mainscreen(this);
   }
   else if (strcmp(szSceneName, "hell_vale") == 0)
   {
      lpScene = new Scene_Hell_Vale(this);
   }
   else if (strcmp(szSceneName, "hell_styx") == 0)
   {
      lpScene = new Scene_Hell_Styx(this);
   }
   else if (strcmp(szSceneName, "hell_door") == 0)
   {
      lpScene = new Scene_Hell_Door(this);
   }
   else if (strcmp(szSceneName, "vignette_spirit") == 0)
   {
      lpScene = new Vignette_Spirit(this);
   }
   else if (strcmp(szSceneName, "vignette_diningroom_clock") == 0)
   {
      lpScene = new Vignette_Diningroom_Clock(this);
   }
   else if (strcmp(szSceneName, "vignette_diningroom_boat") == 0)
   {
      lpScene = new Vignette_Diningroom_Boat(this);
   }
   else if (strcmp(szSceneName, "vignette_diningroom_book") == 0)
   {
      lpScene = new Vignette_Diningroom_Book(this);
   }
   else if (strcmp(szSceneName, "vignette_diningroom_lib") == 0)
   {
      lpScene = new Vignette_Diningroom_Lib(this);
   }
   else if (strcmp(szSceneName, "vignette_diningroom_picture") == 0)
   {
      lpScene = new Vignette_Diningroom_Picture(this);
   }
   else if (strcmp(szSceneName, "vignette_cinematic") == 0)
   {
      lpScene = new Vignette_Cinematic(this);
   }
   else if (strcmp(szSceneName, "vignette_hands") == 0)
   {
      lpScene = new Vignette_Hands(this);
   }
   else if (strcmp(szSceneName, "vignette_hands_inca") == 0)
   {
      lpScene = new Vignette_Hands(this);
   }
   else if (strcmp(szSceneName, "vignette_hands_japan") == 0)
   {
      lpScene = new Vignette_Hands(this);
   }
   else if (strcmp(szSceneName, "vignette_hands_island") == 0)
   {
      lpScene = new Vignette_Hands(this);
   }
   else if (strcmp(szSceneName, "vignette_hands_egypt") == 0)
   {
      lpScene = new Vignette_Hands(this);
   }
   else if (strcmp(szSceneName, "vignette_hands_middleage") == 0)
   {
      lpScene = new Vignette_Hands(this);
   }
   else if (strcmp(szSceneName, "minigames") == 0)
   {
      lpScene = new SceneMinigames(this);
   }
   else if (strcmp(szSceneName, "teasing") == 0)
   {
      lpScene = new Scene_Teasing(this);
   }
   else if (strcmp(szSceneName, "debug_david") == 0)
   {
      lpScene = new Scene_Debug_David(this);
   }
   else if (strcmp(szSceneName, "debug_preload") == 0)
   {
      lpScene = new Scene_Debug_Preload(this);
   }
   else {
      KAssert(false);
      return false;
   }
   
   KAssert(lpScene != NULL);
   Loader::LoadScene(szSceneName,true);
   // Init des arguments de la scène fraichement créée
   std::string strName = szSceneName;
   std::string strAdditionalName = szAdditionalName;
   lpScene->SetName(strName);
   lpScene->setAdditionalName(strAdditionalName);
   
   char szSceneFile[K_MAXPATH];
   memset(szSceneFile, 0, K_MAXPATH);
   strcpy(szSceneFile, strName.c_str());
   strcat(szSceneFile, ".scn");
   char szSceneFilePath[K_MAXPATH];
   memset(szSceneFilePath, 0, K_MAXPATH);
   strcpy(szSceneFilePath, KMiscTools::makeFilePath(szSceneFile));
   lpScene->parseSceneFile(szSceneFilePath);
   
   // Chargement en mémoire seulement, on ne fait rien qui puisse influencer l'affichage ou l'environnement sonore
   if (bLoadOnly) {
      EObjectContainer::CleanUp(szSceneName);
      ESequenceContainer::CleanUp(szSceneName);
      return true;
   }
   
   PrepareMusic(lpScene);
   _lpInventory->SetUniverse(lpScene->_strSceneUniverseName.c_str());
   _lpBookCtrl->getTaskListPtr()->SetUniverse(lpScene->_strSceneUniverseName.c_str());
   lpScene->Init();
   // Rétablie le fonctionnement de l'IHM si le sequencer est vide
   if (!getSequencer()->isActive()) {
      DialogModeStop();
      CinematicModeStop();
      LockModeStop();
   }
   lpScene->SetMouseOverInfo("", "");
   PrepareMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
   
   // Hexagramme visible uniquement dans la maison
   if (lpScene->_strSceneUniverseName == "house") {
      _bHexagramVisible = true;
   }
   else {
      _bHexagramVisible = false;
   }
   
   if (lpScene->IsNeedIHM() && !_bDialogMode && !_bCinematicMode && !_bLockMode)
   {
//      ShowIHM(true);
   }
   else
   {
      ShowIHM(false);
   }
   
   // Démarrage du sound design
   lpScene->StartSoundDesign();
   
   lpScene->StartFade(SCENE_SHOWIN);
   EWatchDog::SceneTransition(false);
   return true;
}

void ESceneDirector::SetupBackButton()
{
   if (_bDialogMode)
   {
      DialogModeStart();
   }
   if (_bCinematicMode)
   {
      CinematicModeStart();
   }
}

void ESceneDirector::CloseCurrentScene()
{
   if (_lpSceneCurrent)
      _lpSceneCurrent->Close();
}

void ESceneDirector::CloseCurrentMiniGame()
{
   if (_lpMiniGameCurrent)
   {
      _bClosingMiniGame = true;
      _lpMiniGameCurrent->StartFade(SCENE_SHOWOUT);
      PrepareMusic(_lpSceneCurrent);
   }
}

void ESceneDirector::majTaskList() {
   
   _lpBookCtrl->getTaskListPtr()->maj();
}

void ESceneDirector::addObjectiveTaskList(const char* objective) {
   
   _lpBookCtrl->getTaskListPtr()->addObjective(objective);
}

void ESceneDirector::SetTaskManager(bool b)
{
   if (!getSequencer()->isEmpty()) {
      return;
   }
   _bTaskManager = b;
   if (b) {
      _lpBookCtrl->Show(TASK_LIST);
   }
   else {
      _lpBookCtrl->Hide();
   }
}

void ESceneDirector::ShowMapManager(bool b) {
   _bMapManager = b;
   if(b) {
      _lpMap->Show();
   }
   else {
      _lpMap->Hide();
   }
}

void ESceneDirector::ShowDiaryManager(bool b) {
   ESoundBank::getSound("book_pageturn")->playSample();
   if(b)
      _lpBookCtrl->Show(DIARY);
   else
      _lpBookCtrl->Hide();
}

EMap* ESceneDirector::getMapManager() {
   return _lpMap;
}

EDiary* ESceneDirector::getDiaryPtr() {
   return _lpBookCtrl->getDiaryPtr();
}

bool ESceneDirector::enterPseudo() {
   if(UserProfiles::_lNbPlayers >= 1) {
      hasEnterPseudo = false;    
      return true;
   }
   else 
      hasEnterPseudo = true;
   return false;
}