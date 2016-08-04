/**
 * Kanji example - public domain
 */
//#define TESTENDDATE "2011-02-30"

#include "KPTK.h"
#include "KSound.h"
#include "KVideo.h"
#include "ESceneDirector.h"
#include "MyGame.h"
#include "SoundBank.h"
#include "ImageBank.h"
#include "FontBank.h"
#include "MusicBank.h"
#include "EmitterBank.h"
#include "EPointer.h"
#include "UserProfileMgt.h"
#include "MusicArranger.h"
#include "ThreadLoader.h"
#include "EDiary.h"
#include "EMiniJeuBase.h"
#include "EWatchDog.h"
#include "UIOptions.h"

#include <time.h>
#include "mmutils.h"

#if defined (DUMPTTF)
void GenerateGameBitmapFont (void);
#endif

/** Constructor */
MyGame::MyGame () :
#ifdef K_WIN32
// Sous windows seulement on se permet 
   _bWindowed (false),
#else
   _bWindowed (false),
#endif
      _bSystemCursor (false),
      _fMusicVolume (0.8f),
      _fSfxVolume (1.0f) {
}

/**
 * Load settings and create game window. Derive and implement this method
 *
 * \return game window, or NULL to exit
 */


KWindow *MyGame::createGameWindow (void) {
   KWindow *lpKWindow;
   KIniReader *lpIniReader;
   bool bSuccess;

   /* Read data files from application bundle (for Mac) */
   KMiscTools::setFileSource (true);
   
   /* Add pakfile (files.pak is really files.zip). "" is for an empty password. You can set a password in the zipfile and pass it here instead */
#ifdef TEST_ARCHIVE
   Loader::InitHasArchive();
#endif
#ifdef CYPHERED_PAK
   KResource::addArchive (KMiscTools::makeFilePath ("files.pak"), K_RESARCHIVE_REQUIRED, "huA7NYfAr41JbMEwJ1cfTmUQjXL8XKts");
#else
   KResource::addArchive (KMiscTools::makeFilePath ("files.pak"), K_RESARCHIVE_REQUIRED, "");
#endif

   /* Start log */
   time_t rawtime;
   struct tm * timeinfo;
   
   time ( &rawtime );
   timeinfo = localtime ( &rawtime );

   KStringFmt (_szPathBuf, K_MAXPATH, "%s/Log Enigma %d-%d-%d-%d-%d.txt", getStateFolder(), timeinfo->tm_mday, timeinfo->tm_mon+1, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

#ifdef DEBUG_LOG
   KPTK::enableLog (KMiscTools::makeFilePath (_szPathBuf), "enigma", "1.0", false);
   KResource::logOpens (true);
#endif

#ifdef DEBUG
   /* Enable crash reporting to a dummy email address */
   enableCrashReporter (true, "dmekersa@gmail.com");
#endif
   
   /* Read settings, if they exist */
   KStringFmt (_szPathBuf, K_MAXPATH, "%s/settings.ini", getStateFolder());
   lpIniReader = new KIniReader;
   bSuccess = lpIniReader->setIni (KMiscTools::makeFilePath (_szPathBuf));
   if (bSuccess) {
      _bWindowed = (lpIniReader->getValue ("settings", "windowed", 1) != 0) ? true : false;
      _bSystemCursor = (lpIniReader->getValue ("settings", "systemcursor", 0) != 0) ? true : false;
      _fMusicVolume = lpIniReader->getFloatValue ("settings", "music", 0.8f);
      _fSfxVolume = lpIniReader->getFloatValue ("settings", "sfx", 1.0f);
      
      char strPlayer[15+1];
      memset(strPlayer, 0, 15+1);
      lpIniReader->getString ("settings", "lastplayer", strPlayer, 15+1);
      if (strlen(strPlayer) <= 0) {
         strcpy(strPlayer, "ashley");
      }
      UserProfiles::_pPlayer = new Player(strPlayer, TRUE); // Restore last player
   }

   delete lpIniReader;

   /* Create game window */
#ifdef K_WIN32
   lpKWindow = KPTK::createKWindow (K_DIRECTX9);
#else
   lpKWindow = KPTK::createKWindow (K_OPENGL);
#endif
   lpKWindow->setFixedAspectRatio (false); /* Keep 4:3 ratio when going fullscreen */
   lpKWindow->createGameWindow (1024, 768, 32, _bWindowed, "Age of Enigma");
   lpKWindow->setPTKCallBack (myEventHandler);
   
   /* Use software mouse cursor */
   KInput::hidePointer ();
   lpKWindow->displayMouse(false);

   return lpKWindow;
}

/**
 * Display missing file error
 *
 * \param lpszFileName name of missing file
 */

void MyGame::showFileError (const char *lpszFileName) {
   static char szFileMsg[K_MAXPATH];

   KStringFmt (szFileMsg, K_MAXPATH, KStr ("ERR_BODY"), lpszFileName);
   KMiscTools::messageBox (KStr ("ERR_TITLE"), szFileMsg);
}

/**
 * Run the game. Derive and implement this method
 */

void MyGame::run (void) {
   KGraphic *lpWindowGraphic;
//   KBatch *lpBatch;
//   KSound *lpMusic;
   KSound *lpButtonSnd;
   KUIElement *lpUIContainer;

   bool bOptionOpening = false;
   bool bLogImageBank = false;
   float Time = 0.0f;
   //float Phase = -100.0f;
   unsigned long g_StartTime;

//#ifndef DEBUG   
   /* Load strings table */
   readStringTable (KMiscTools::makeFilePath ("l10n/tradfile.txt"));
//#else
//#ifdef ENGLISH_BUILD
//   readStringTable (KMiscTools::makeFilePath ("english_rc.txt"));
//#else
//   readStringTable (KMiscTools::makeFilePath ("french_rc.txt"));
//#endif
//#endif
   
   // Update the window titla
   _lpKWindow->setTitle(getGameName());
  
   g_StartTime = KMiscTools::getMilliseconds();
   
   
   KUIElement::initialize (_lpKWindow);
   Loader::InitThread();
   

   // SplashScreen immédiat
   KGraphic *lpBigFishLogo = EImageBank::getImage("enigma_loading.jpg","png",false,false,false,"l10n/");
   resetFrameLoop ();
   endFrame();
   beginFrame();
   lpBigFishLogo->blitAlphaRect(0,0,1024,768,0,0);
   endFrame();

//   // lancement de la musique splashscreen, volume par défaut
//   KSound *lpSoundBigFish = EMusicBank::getMusic("splashscreen");
//   lpSoundBigFish->setVolume((long) (100 * _fMusicVolume));
//   if (lpSoundBigFish != NULL)
//	   lpSoundBigFish->playStream(false);
   
   /* SceneDirector */
   ESceneDirector *lpSceneDirector;
   lpSceneDirector = new ESceneDirector();
   EMiniJeuBase::init();
   EMiniJeuBase::Preload();

   ESoundBank::getSound("click",false,true);
   KSound::setGlobalVolumes ((long) (100 * _fMusicVolume), (long) (100 * _fSfxVolume));
   lpSceneDirector->SetVolume(long(_fMusicVolume*100));
   lpSceneDirector->SetSystemCursor(_bSystemCursor);

   // Retrieve last player settings
   if (!UserProfiles::_pPlayer) {
      // TODO: Il faut afficher l'interface de création d'un nouveau joueur
      UserProfiles::_pPlayer = new Player("Ashley Reeves", true);
   }
   UserProfiles::RestoreProfile();
   lpSceneDirector->getInventory()->RestoreFromProfile();
   lpSceneDirector->getDiaryPtr()->RestoreFromProfile();
   lpSceneDirector->getMap()->RestoreFromProfile();

   if (TaskResolved("task_gethexagramme")) {
      lpSceneDirector->SetHexagramme(true);
   }
   else {
      lpSceneDirector->SetHexagramme(false);
   } 
   
#ifdef ENIGMA_STATS
   /* Load font */
   KTextFace *lpFont;
   lpFont = EFontBank::getFont("tahoma.ttf",10);
#endif

   lpButtonSnd = ESoundBank::getSound("click");
   lpWindowGraphic = EImageBank::getImage("options_box.png");

   lpUIContainer = new KUIElement(KUIElement::getRootElement());
   lpUIContainer->setPosition (0, 0);
   lpUIContainer->setSize (1024, 768);
   lpUIContainer->setMessageCallBack (myUIMessageHandler);
   
   UIOptions optionsWindow(lpUIContainer, _bWindowed, _fMusicVolume, _fSfxVolume);
   
   /* Create vertex buffer */
//   lpBatch = KPTK::createKBatch ();
//   lpBatch->allocateBuffer (500);
      
   /* Load button sound effect */
   
   KUIPlayerListWindow   *MyPlayerListWindow = new KUIPlayerListWindow(lpUIContainer);
   KUIDeletePlayerWindow *MyDeletePlayerWindow = new KUIDeletePlayerWindow(lpUIContainer);
	
   MyNewPlayerWindow = new KUINewPlayerWindow(lpUIContainer);
      
   /* Render loop */
   resetFrameLoop ();

   Loader::ActiveScenePreload();

   // Aller a la première scene
#ifdef TESTENDDATE
   /* Start log */
   time_t rawtime;
   struct tm * timeinfo;
   
   time ( &rawtime );
   timeinfo = localtime ( &rawtime );

   int year=2011,month=8,day=15;
//   sscanf(TESTENDDATE,"%d-%d-%d",&year,&month,&day);
   if (timeinfo->tm_year > year - 1900 || timeinfo->tm_mon > month-1 || (timeinfo->tm_mon == month-1 && timeinfo->tm_mday > day)) {
      timeOut();
   } else
#endif
   {
      KInput::showPointer();
      /* Première scène du jeu */
      lpSceneDirector->GoToScene("title_splashscreen","");
      lpSceneDirector->GetCurrentScene()->StartFade(SCENE_STABLE); // skip le fadin l'ecran etant déja initialisé
   }

#if defined (DUMPTTF)
   GenerateGameBitmapFont();
#endif

   char buffer[256];
   GetSystemLocale(buffer);
   
   printf ("Locale 2 is: %s\n", buffer );

   do {
      long nTicks;
      beginFrame ();
      KUIElement::moveAll ();
            
      float PrevTime = Time;
      Time = (KMiscTools::getMilliseconds() - g_StartTime) * 0.001f;
      
      float TimeDelta = Time - PrevTime;
      float SafeTimeDelta;
      float CurTime = PrevTime;
      
      if (TimeDelta >= 0.02f)
         SafeTimeDelta = 0.02f;
      else
         SafeTimeDelta = TimeDelta;
      
      CurTime += SafeTimeDelta;
      TimeDelta -= SafeTimeDelta;
      
#ifdef DEBUG
      if (KInput::isPressed (K_VK_F2) && _bWindowed) {
         EObjectContainer *obj;
		 bool bGotNext = EObjectContainer::g_lpListObj.getHead (obj);
		 if (bGotNext) {
            while (bGotNext) {
               if (strcmp(obj->_strSceneName.c_str(), SCENEDIRECTOR->getCurrentSceneName()) == 0 && obj->_type == SCENEOBJECT_PARTICLEEMITTER) {
				   obj->_lpObjectEmitter->Stop();
			   }
			   // Suivant
			   bGotNext = EObjectContainer::g_lpListObj.getNext (obj);
			}
		 } 
	  }
      if (KInput::isPressed (K_VK_F)) {
         _lpKWindow->setMaxFrameRate(-1);
      }
      if (KInput::isPressed (K_VK_END) && !bLogImageBank) {
         bLogImageBank = true;
         EImageBank::LogAll();
      }
      if (!KInput::isPressed (K_VK_END)) {
         bLogImageBank = false;
      }
#endif
      
#ifdef SCENE_SHORTCUT   
      if (KInput::isPressed (K_VK_F1) && _bWindowed) {
         _lpKWindow->toggleFullScreen (true);
         _bWindowed = false;
      }
      if (KInput::isPressed (K_VK_F2) && !_bWindowed) {
         _lpKWindow->toggleFullScreen (false);
         _bWindowed = true;
      }
#endif
      
      if (KInput::isPressed (K_VK_ESCAPE)) {
         if (!lpSceneDirector->isDialogboxActive() && !lpSceneDirector->GetCurrentVignette() && !lpSceneDirector->GetCurrentMinigame()) {
            lpSceneDirector->resetHoldItem();
            if (strcmp(lpSceneDirector->getCurrentSceneName(),"menu")==0) {
#ifdef BFG_TEASING
               lpSceneDirector->getSequencer()->GoToScene(NULL, "teasing", "", false);
#else
//               MyGame::getGame()->getKWindow()->terminate();
#endif
            }
            else 
			{
			   lpSceneDirector->CinematicModeStop();
               lpSceneDirector->DialogModeStop();
               lpSceneDirector->GoToScene("menu", "", false);
            }
         }
      }
      
#ifdef SCENE_SHORTCUT   
      if (KInput::isPressed (K_VK_DEL)) {
         lpSceneDirector->_bOptionsRequest = true;
      }
      if (KInput::isPressed (K_VK_END)) {
         if (lpSceneDirector->isTaskManager()) {
            lpSceneDirector->SetTaskManager(true);
         }
         else {
            lpSceneDirector->SetTaskManager(false);
         }
      }
#endif
      
      bOptionOpening = false;
      optionsWindow.Check();

      if (g_bPlayButtonSound) {
         lpButtonSnd->playSample ();
         g_bPlayButtonSound = false;
      }

      /* Check UI */
      if (lpSceneDirector->_bPlayerRequest) {
         lpSceneDirector->_bPlayerRequest = false;
         /* Show the profile panel */
         MyPlayerListWindow->animateScale (0.1f, 1.0f, 0, 250, K_COUNTER_EASEOUT);
         MyPlayerListWindow->animateZAngle (20, 0, 0, 250, K_COUNTER_EASEOUT);
         MyPlayerListWindow->animatePosition (420, 320, 0, 0, 0, 250, K_COUNTER_EASEOUT);
         MyPlayerListWindow->animateBlend (0, 1, 0, 250);
      }

      if (MyPlayerListWindow->getBlend() == 1.0f) {
            MyPlayerListWindow->Show(); // disable input on other window 
       }

      // Two cases: maximum of players reached or not
      if (MyPlayerListWindow->lpUINewButton->isClicked () || lpSceneDirector->_bNewPlayerRequest == true || (lpSceneDirector->hasEnterPseudo)) {
         lpSceneDirector->_bNewPlayerRequest = false;

         if (UserProfiles::_lNbPlayers >=10){ // TODO: change max value
            KMiscTools::messageBox ("Player List", "Maximum number of players reached!"); // TODO use nice dialog box
         }
         else {
            MyNewPlayerWindow->Show();
//            if (!TaskResolved("__task__newprofile__")) {
//               MyNewPlayerWindow->lpUIEditField->setText("Ashley Reeves");
//            }
            if (MyPlayerListWindow->getBlend() >= 1.0f) {
               MyPlayerListWindow->Hide(); // disable input on other window 
            }
         }
      }
      
      if (MyPlayerListWindow->lpUIDeleteButton->isClicked ()) {
         if (UserProfiles::_lNbPlayers == 0){ // TODO: change max value
            KMiscTools::messageBox ("Player List", "No player to delete!"); // TODO use nice dialog box
         }
         else {
            if (MyPlayerListWindow->getBlend() >= 1.0f) {
               MyPlayerListWindow->Hide();
            }
            MyDeletePlayerWindow->Show(); 
         }
      }
      
      if (MyPlayerListWindow->lpUIOkButton->isClicked ()) {

         // Charge le nouveau joueur
         UserProfiles::RestoreProfile();
         lpSceneDirector->getInventory()->RestoreFromProfile();
         lpSceneDirector->getDiaryPtr()->RestoreFromProfile();
         lpSceneDirector->getMap()->RestoreFromProfile();

         /* Hide the options panel */
         MyPlayerListWindow->animateBlend (1, 0, 0, 250);
         MyPlayerListWindow->animateScale (1.0f, 0.1f, 0, 250, K_COUNTER_EASEIN);
         MyPlayerListWindow->animatePosition (floorf ((1024 - lpWindowGraphic->getWidth()) / 2), floorf ((768 - lpWindowGraphic->getHeight()) / 2), 420, 320, 0, 250, K_COUNTER_EASEIN);
         MyPlayerListWindow->animateZAngle (0, -20, 0, 250, K_COUNTER_EASEIN);
         MyPlayerListWindow->animateBlend (1, 0, 0, 250);
         //optionsWindow.lpUIOptionsButton->animateBlend (0, 1, 0, 250);
         ResolveTask("__task__newprofile__");
      }
      
      if (MyNewPlayerWindow->lpUIOkButton->isClicked ()) {
         Player *p;
         // check if valid name
         char Name[15];
         memset(Name, 0 , 15);
         strcpy(Name, MyNewPlayerWindow->lpUIEditField->getText());
         if (strlen(Name) == 0)
         {
            if(UserProfiles::_lNbPlayers >= 1) {
               MyNewPlayerWindow->Hide(); 
               lpSceneDirector->OpenDialogbox(KStr("UI_MISSINGNAME"));
            }
         }
         else {
            bool bGotNext = UserProfiles::g_PlayersList.getHead (p);
            bool bIsUnique = true;
            if (bGotNext) {
               while (bGotNext && bIsUnique) {
                  if 	(!strcmp (Name, p->GetName()) )
                     bIsUnique = false;
                  bGotNext = UserProfiles::g_PlayersList.getNext (p);
               }
            }
            if (! bIsUnique)
            {
               MyNewPlayerWindow->Hide(); 
               lpSceneDirector->OpenDialogbox(KStr("UI_ALREADYEXIST"));
            }
            else {
               UserProfiles::_pPlayer->SetName(Name); // Store current "active" player 
               
               // player name is valid
               p = new Player(Name, true);
               UserProfiles::g_PlayersList.addToTail(p);
               UserProfiles::_lNbPlayers++;

               if (TaskResolved("__task__newprofile__")) {
                  MyPlayerListWindow->RefreshUI(UserProfiles::_pPlayer);

                  if(!lpSceneDirector->hasEnterPseudo)
                     MyPlayerListWindow->Show();
               }
               MyNewPlayerWindow->Hide(); 
               
               lpSceneDirector->SetHexagramme(false);
               lpSceneDirector->getDiaryPtr()->Clean();
               lpSceneDirector->getMap()->Clean();
               EInventoryItem::CleanUp(NULL);
               EGlobalBank::CleanUp();

               // On joue si on était en mode automatique
               if (!TaskResolved("__task__newprofile__")) {
                  ResolveTask("__task__newprofile__");
                  //lpSceneDirector->GetCurrentScene()->ObjectClicked("but_play", 0, 0);
               }
                 
               lpSceneDirector->enterPseudo(); // dit qu'on a rentrer le pseudo
               MyPlayerListWindow->RefreshUI(UserProfiles::_pPlayer);
               saveSettings();
               UserProfiles::SaveProfile();
            }
         }
      }
      
      if (MyNewPlayerWindow->lpUICancelButton->isClicked ()){
         // nothing to do: just back to previuos screen
         MyNewPlayerWindow->Hide(); 

         // si on est pas dans le cas où on demande le pseudo au lancement du jeu car pas de profil enregistré
         if(UserProfiles::_lNbPlayers >= 1)
            MyPlayerListWindow->Show(); // correction bug -> nouveau -> annuler -> plus de controle
      }
      
      if (MyDeletePlayerWindow->lpUIOkButton->isClicked ()){
         // delete selected player from player list
         Player *p;
         Player *oToDelete = NULL;
         bool bGotNext = UserProfiles::g_PlayersList.getHead (p);
         bool bStillSearching = true;
         if (bGotNext) {
				while (bGotNext && bStillSearching) {
					if 	(strcmp (p->GetName(), UserProfiles::_pPlayer->GetName()) == 0 ){
                  // Delete des différents fichier de sauvegarde
						char * pFileName = new char [3+15+4]; // take "%s/... .pro" into account
                  strcpy(pFileName, "%s/");
                  strcat(pFileName,  UserProfiles::_pPlayer->GetName()); // file name = player name
                  strcat(pFileName, ".pro");// file extension
                  KStringFmt (_szPathBuf, K_MAXPATH, pFileName, getStateFolder()); // complete file path
                  remove (KMiscTools::makeFilePath(_szPathBuf));
                  pFileName[strlen(pFileName)-4] = 0;
                  strcat(pFileName, ".bak");
                  KStringFmt (_szPathBuf, K_MAXPATH, pFileName, getStateFolder());
                  remove (KMiscTools::makeFilePath(_szPathBuf));
                  pFileName[strlen(pFileName)-4] = 0;
                  strcat(pFileName, ".new");
                  KStringFmt (_szPathBuf, K_MAXPATH, pFileName, getStateFolder());
                  remove (KMiscTools::makeFilePath(_szPathBuf));
                  bStillSearching = false; // stop searching
                  UserProfiles::g_PlayersList.remove (p); // remove player from list
                  oToDelete = p;
                  UserProfiles::_lNbPlayers--;
					}
					bGotNext = UserProfiles::g_PlayersList.getNext (p);
               if (oToDelete) {
                  XDELETE(oToDelete);
               }
				}
         }
         UserProfiles::g_PlayersList.getHead (p);
         MyPlayerListWindow->RefreshUI(p); 
         MyPlayerListWindow->Show(); 
         MyDeletePlayerWindow->Hide(); 
      }
      
      if (MyDeletePlayerWindow->lpUICancelButton->isClicked ()){
         // nothing to do: just back to previous screen
         MyPlayerListWindow->Show(); 
         MyDeletePlayerWindow->Hide(); 
      }
      
      if (!optionsWindow.IsVisible() && MyPlayerListWindow->getBlend() == 0 && MyNewPlayerWindow->getBlend() == 0 && MyDeletePlayerWindow->getBlend() == 0 ) {
         // Initialisation coordonnées et événements souris
         Mouse.init();
          
         lpSceneDirector->Check();
          
         // Fermeture de tous les événements souris
         Mouse.EventClose();
      }
      
      int nNbTicks = 0;
      
	  /* Update logic in ticks of 10 milliseconds */
      nTicks = getTickCount ();
      while (nTicks > 0) {
         // Initialisation coordonnées souris
         Mouse.init();
         lpSceneDirector->Logic();
         if (lpSceneDirector->getResetTimeFrame()) {
            break;
         }
         nTicks--;
         nNbTicks++;
      }      

      /* Draw background */
      //_lpKWindow->setWorldView (0, 0, 0.0f, 1.0f, false);      
      
      
         /* Draw de la scène courante */
        lpSceneDirector->Draw();
      
//      lpBatch->endBatch ();
      
      if (MyPlayerListWindow->getBlend() > 0) {
         /* Darken screen behind the options panel */
//         lpBgGraphic->drawRect (-1, -1, 801, 601, 0, 0, 0, 0.333f * MyPlayerListWindow->getBlend());
      }

      if (optionsWindow.IsVisible()) {
         // 17/08/11
         // Repercute le changement de mode windowed et volumes si besoin
         _bWindowed = optionsWindow._bWindowed;
         if (_fSfxVolume != optionsWindow._fSfxVolume) {
            _fSfxVolume = optionsWindow._fSfxVolume;
         }
         if (_fMusicVolume != optionsWindow._fMusicVolume) {
            _fMusicVolume = optionsWindow._fMusicVolume;
         }
         
         /* Darken screen behind the options panel */
//         lpBgGraphic->drawRect(-1, -1, 801, 601, 0, 0, 0, 0.333f * lpUIOptionsWindow->getBlend());
      }
      
      EWatchDog::EndFrame();

      /* Debug */
#ifdef ENIGMA_STATS
      if (lpSceneDirector->_bDebugDisplay) {
         lpFont->setBlitColor(1.0f, 1.0f, 1.0f, 1.0f);
//         lpFont->drawStringFromRight(lastFPS, 1024-5, 5);
         char szDebug[256];
         memset(szDebug, 0, 256);
         sprintf(szDebug, "FPS: Last=%s/Av=%s", EWatchDog::LastFPS(),EWatchDog::MoyenneFPS());
         lpFont->drawStringFromRight(szDebug, 1024-5, 5);
         sprintf(szDebug, "Objects: %d", EObjectContainer::g_lpListObj.getCount());
         lpFont->drawStringFromRight(szDebug, 1024-5, 15);
         lpFont->drawMultiline(EWatchDog::TabFPS(), 5,200, 60,TEXTSTYLE_LEFT); 

         sprintf(szDebug, "Particles: %d", CreepingSmokeEmitter::particlesCount);
         lpFont->drawStringFromRight(szDebug, 1024-5, 35);
         
         sprintf(szDebug, "GetCacheRequest: %d", Loader::GetCacheRequest());
         lpFont->drawStringFromRight(szDebug, 1024-5, 85);
         sprintf(szDebug, "GetCacheMiss: %d", Loader::GetCacheMiss());
         lpFont->drawStringFromRight(szDebug, 1024-5, 100);
         sprintf(szDebug, "GetPreLoadCount: %d", Loader::GetPreLoadCount());
         lpFont->drawStringFromRight(szDebug, 1024-5, 115);

         sprintf(szDebug, "ImageBank size: %d", EImageBank::GetCount());
         lpFont->drawStringFromRight(szDebug, 1024-5, 165);
         sprintf(szDebug, "SoundBank size: %d", ESoundBank::GetCount());
         lpFont->drawStringFromRight(szDebug, 1024-5, 180);
         sprintf(szDebug, "EmitterBank size: %d", EEmitterBank::GetCount());
         lpFont->drawStringFromRight(szDebug, 1024-5, 195);
         sprintf(szDebug, "EFontBank size: %d", EFontBank::GetCount());
         lpFont->drawStringFromRight(szDebug, 1024-5, 210);
         sprintf(szDebug, "EMusicBank size: %d", EMusicBank::GetCount());
         lpFont->drawStringFromRight(szDebug, 1024-5, 225);
         short bTESTENDDATE = false;
         short bENGLISH_BUILD = false;
         short bBETA_RELEASE = false;
         short bCYPHERED_PAK = false;
         short bDEBUG_LOG = false;
         short bTEST_ARCHIVE = false;
#ifdef TESTENDDATE
         bTESTENDDATE = true;
#endif
#ifdef ENGLISH_BUILD
         bENGLISH_BUILD = true;
#endif
#ifdef BETA_RELEASE
         bBETA_RELEASE = true;
#endif
#ifdef CYPHERED_PAK
         bCYPHERED_PAK = true;
#endif
#ifdef DEBUG_LOG
         bDEBUG_LOG = true;
#endif
#ifdef TEST_ARCHIVE
         bTEST_ARCHIVE = true;
#endif
         sprintf(szDebug, "define:T%d-E%d-BR%d-C%d-DL%d-TA%d", bTESTENDDATE, bENGLISH_BUILD, bBETA_RELEASE, bCYPHERED_PAK, bDEBUG_LOG, bTEST_ARCHIVE);
         lpFont->drawStringFromRight(szDebug, 1024-5, 240);

         if (TestGlobal("__mode_casual__")) {
            lpFont->drawStringFromRight("mode: casual", 1024-5, 45);
         }
         if (TestGlobal("__mode_adventure__")) {
            lpFont->drawStringFromRight("mode: adventure", 1024-5, 45);
         }

      }
#endif
      
      // RAZ du compteur de particules
      CreepingSmokeEmitter::particlesCount = 0;
      
      /* Finish drawing this frame and present it */
      endFrame ();
      
      if (_lpKWindow->isQuit()) {
         _lpKWindow->setQuit(false);
         // Purge des objets à ramasser encore dans la liste du séquenceur
         if (lpSceneDirector->GetCurrentScene()) {
            ESequenceContainer::Purge(lpSceneDirector->GetCurrentScene()->GetName().c_str());
         }
         _lpKWindow->setQuit(true);
      }

   } while (!_lpKWindow->isQuit ());

   K_LOG("MyGame::Run() -- Main Loop exited !");
   
   _bSystemCursor = lpSceneDirector->_bSystemCursor;
   lpSceneDirector->CloseCurrentScene();
   lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE);
   delete lpSceneDirector;

   K_LOG("MyGame::Run() -- Loader::CloseThread() -- Before");
   Loader::CloseThread();
   K_LOG("MyGame::Run() -- Loader::CloseThread() -- After");

   XDELETE(lpUIContainer);   /* Deleting a UI element deletes all its children as well */   

#ifdef ENIGMA_STATS
   XDELETE(lpFont);
#endif
   K_LOG("MyGame::Run() -- THE END");
}

/**
 * Close game window and save settings. Derive and implement this method
 */

void MyGame::destroyGameWindow (KWindow *lpKWindow) {
   K_LOG("MyGame::destroyGameWindow() -- Start");
   // Survey
#ifdef BFG_TEASING
   FILE *urlfile;
   urlfile = fopen(KMiscTools::makeFilePath("SurveyLink.txt"),"r");

   if (urlfile != NULL) {
      char url[K_MAXPATH];
      fscanf(urlfile,"%s",url);
      fclose(urlfile);

      KMiscTools::launchURL (url, true);
      KSysThread::sleep(1000);
   }
#endif

   /* Close game window */
   lpKWindow->terminate();
   XDELETE(lpKWindow);

   saveSettings();
   XDELETE(UserProfiles::_pPlayer);

   // nettoyage des données en background fenetre fermé.
   KUIElement::cleanup();
   ESequenceContainer::CleanUpAll();
   EObjectContainer::CleanUpAll();
   UserProfiles::CleanUp();
   EMiniJeuBase::cleanup();

   // Banks cleanup a faire en dernier
   ESoundBank::CleanUp();
   EMusicBank::CleanUp();
   EEmitterBank::CleanUp();
   EGlobalBank::CleanUp();
   EImageBank::CleanUp();
   EFontBank::CleanUp(); 
   
   K_LOG("MyGame::destroyGameWindow() -- End");
}

/* Event handler */
bool MyGame::myEventHandler (KEvent *lpKEvent) {
   bool bProcessed;
   int x,y;

   /* Send event to UI first */
   bProcessed = KUIElement::dispatchEvent (lpKEvent);
   if (!bProcessed) {
      /* Not caught by UI, handle */

      switch (lpKEvent->type) {
         case K_EVENT_ACTIVATED:
            K_LOG("K_EVENT_ACTIVATED");

            // remet la souris dans la fenetre
            x=KInput::getMouseX(), y=KInput::getMouseY();
            if (x>=1024)   x=1023;
            if (x<=0)      x=1;
            if (y>=768)    y=767;
            if (y<=0)      y=765;

            KInput::mousePointerTo(x,y);

            /* Window got focus; resume audio */
            EImageBank::ResumeAllVideos();
            KSound::resume();
            break;
            
         case K_EVENT_DEACTIVATED:
            K_LOG("K_EVENT_DEACTIVATED");
            // Loading / Clic to start
            MyGame::getGame()->getKWindow()->setWorldView (0, 0, 0.0f, 1.0f, true);
            MyGame::getGame()->getKWindow()->flipBackBuffer(false);
            
            /* Window lost focus; suspend audio */
            EImageBank::PauseAllVideos();
            KSound::suspend ();
            break;
      default:
         break;
      }
   }

   return true;
}

/* UI message handler */

void MyGame::myUIMessageHandler (KUIMessage *lpUIMessage) {
   if (lpUIMessage->nMessage == K_UIBUTTON_MSGFIRSTDOWN ||
       (lpUIMessage->nMessage == K_UISLIDER_MSGFIRSTDOWN && strcmp (lpUIMessage->lpElement->getName(), "music_slider"))) {
      /* Play a sound when any button or slider is clicked, except for the music volume slider */
      g_bPlayButtonSound = true;
   }
   
	if ( (lpUIMessage->nMessage == K_UIELEMENT_MSGDOWN) && lpUIMessage->lpElement->isOfClass("KUIMyText")) {
		Player * p;
		bool bGotNext = UserProfiles::g_PlayersList.getHead (p);
		if (bGotNext) {
			while (bGotNext) {
				if 	(strcmp (lpUIMessage->lpElement->getName(), p->GetName()) && strcmp (lpUIMessage->lpElement->getName(), UserProfiles::EmptyName) ) {
					KUIMyText * MyText = static_cast <KUIMyText *> (lpUIMessage->lpElement->getElementByName(p->GetName()));
					MyText->setTextOutlineColor (0, 0, 0, 0.125f);
               MyText->hideFocus();
					p->SetFocus(false);
				}
				else {
					UserProfiles::_pPlayer->SetName(p->GetName());
					UserProfiles::_pPlayer->SetFocus(true);
				}
            
				bGotNext = UserProfiles::g_PlayersList.getNext (p);
			}
	   }
   }
}

void MyGame::saveSettings() {
   /* Store settings, if possible */
   FILE *f;
   char szPathBuf[K_MAXPATH];
   KStringFmt (szPathBuf, K_MAXPATH, "%s/settings.ini", getStateFolder());
   f = fopen (KMiscTools::makeFilePath (szPathBuf), "w");
   if (f) {
      fprintf (f, "[settings]\n");
      fprintf (f, "windowed=%d\n", _bWindowed ? 1 : 0);
      fprintf (f, "systemcursor=%d\n", _bSystemCursor ? 1 : 0);
      fprintf (f, "music=%f\n", _fMusicVolume);
      fprintf (f, "sfx=%f\n", _fSfxVolume);
      fprintf (f, "lastplayer=%s\n", UserProfiles::_pPlayer->GetName()); // Record last player in settings.ini
      fflush(f);
      fclose (f);
   }
}

void showFileError (const char *lpszFileName) {
   static char szFileMsg[K_MAXPATH];
   
   KStringFmt (szFileMsg, K_MAXPATH, KStr ("ERR_BODY"), lpszFileName);
   KMiscTools::messageBox (KStr ("ERR_TITLE"), szFileMsg);
}

/** true if we need to play the 'button clicked' sound effect */
bool MyGame::g_bPlayButtonSound = false;

/** List of Particles */
KList<Particle> MyGame::g_lParticles;

void timeOut() {
	// metre toute les procedures de nettoyages
	
	FILE *f= fopen(KMiscTools::makeFilePath("files.pak"),"wb");
	if (f != NULL) {
		fwrite("",1,1,f);
		fclose(f);
	}

   MyGame::getGame()->getKWindow()->terminate();
}

#if defined (DUMPTTF)
void GenerateGameBitmapFont (void) {
   EFontBank::getFont("NITECLUB.ttf",100);
   //EFontBank::getFont("NITECLUB.ttf",200); number only
   EFontBank::getFont("NITECLUB.ttf",25);
   EFontBank::getFont("NITECLUB.ttf",28);
   EFontBank::getFont("NITECLUB.ttf",32);
   EFontBank::getFont("NITECLUB.ttf",35);
   EFontBank::getFont("NITECLUB.ttf",40);
   EFontBank::getFont("NITECLUB.ttf",42);
   EFontBank::getFont("NITECLUB.ttf",44);
   EFontBank::getFont("NITECLUB.ttf",45);
   EFontBank::getFont("NITECLUB.ttf",46);
   EFontBank::getFont("NITECLUB.ttf",60);
   EFontBank::getFont("OldNewspaperTypes.ttf",12);
   EFontBank::getFont("OldNewspaperTypes.ttf",14);
   EFontBank::getFont("OldNewspaperTypes.ttf",15);
   EFontBank::getFont("OldNewspaperTypes.ttf",18);
   EFontBank::getFont("OldNewspaperTypes.ttf",18);
   EFontBank::getFont("OldNewspaperTypes.ttf",20);
   EFontBank::getFont("OldNewspaperTypes.ttf",22);
   EFontBank::getFont("OldNewspaperTypes.ttf",38);
   EFontBank::getFont("acmesa.ttf",12);
   EFontBank::getFont("acmesa.ttf",15);
   EFontBank::getFont("acmesa.ttf",30);
   EFontBank::getFont("acmesa.ttf",40);
   EFontBank::getFont("acmesa.ttf",60);
   EFontBank::getFont("armadillo.ttf",16);
   EFontBank::getFont("armadillo.ttf",20);
   EFontBank::getFont("armadillo.ttf",24);
   EFontBank::getFont("armadillo.ttf",28);
   EFontBank::getFont("jancient.ttf",20);
   EFontBank::getFont("jancient.ttf",22);
   EFontBank::getFont("jancient.ttf",24);
   EFontBank::getFont("jancient.ttf",30);
   EFontBank::getFont("jancient.ttf",36);
   EFontBank::getFont("jancient.ttf",46);
   EFontBank::getFont("jancient.ttf",50);
   EFontBank::getFont("renaissance.ttf",32);
   EFontBank::getFont("tahoma.ttf",10);
   EFontBank::getFont("tahoma.ttf",12);
   EFontBank::getFont("tahoma.ttf",14);
   EFontBank::getFont("tahoma.ttf",15);
   EFontBank::getFont("tahoma.ttf",16);
   EFontBank::getFont("tahoma.ttf",17);
   EFontBank::getFont("tahoma.ttf",18);
   EFontBank::getFont("tahoma.ttf",20);
   EFontBank::getFont("tahoma.ttf",24);
   EFontBank::getFont("tahoma.ttf",30);
   EFontBank::getFont("tahoma.ttf",32);
   EFontBank::getFont("tahoma.ttf",40);
   EFontBank::getFont("tahoma.ttf",42);
}
#endif