/*
 *  ESceneDirector.h
 *  pngviewer
 *
 *  Created by Rockford on 12/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef ESCENEDIRECTOR_H
#define ESCENEDIRECTOR_H

#include "EScene.h"
#include "SceneMinijeu.h"
#include "ESceneSequencer.h"
#include "EInventory.h"
#include "ESmartImage.h"
#include "KSound.h"
#include "FontBank.h"
#include "MusicArranger.h"
//#include "TaskList.h"
#include "EnergyParticles.h"
#include "EMap.h"
#include "EDiary.h"

#define SCENEDIRECTOR ESceneDirector::_lpSceneDirector
#define DIRECTOR_IHMY 672

class ETaskList;
class EBookCtrl;

typedef enum {
   DIRECTOR_MOUSECURSOR_NONE = -1,
   DIRECTOR_MOUSECURSOR_POINT,
   DIRECTOR_MOUSECURSOR_WALK,
   DIRECTOR_MOUSECURSOR_WALKBACK,
   DIRECTOR_MOUSECURSOR_DOOR,
   DIRECTOR_MOUSECURSOR_ACTION,
   DIRECTOR_MOUSECURSOR_LOOK,
   DIRECTOR_MOUSECURSOR_INFO,
   DIRECTOR_MOUSECURSOR_TALK,
   DIRECTOR_MOUSECURSOR_WAIT,
   DIRECTOR_MOUSECURSOR_PUZZLE,
   DIRECTOR_MOUSECURSOR_TAKE
} DirectorMouseCursor;

typedef enum {
   DIRECTOR_SOUND_DOOR = 0,
   DIRECTOR_SOUND_WALKHEELS,
   DIRECTOR_SOUND_WALKSAND,
   DIRECTOR_SOUND_VORTEX,
   DIRECTOR_SOUND_DIVE
} DirectorSound;

typedef enum {
   DIRECTOR_MUSIC_NONE = 0,
   DIRECTOR_MUSIC_CINEMATIC,
   DIRECTOR_MUSIC_MENU,
   DIRECTOR_MUSIC_HOUSE,
   DIRECTOR_MUSIC_POLTERGEIST,
   DIRECTOR_MUSIC_ISLAND,
   DIRECTOR_MUSIC_JAPAN,
   DIRECTOR_MUSIC_INCA,
   DIRECTOR_MUSIC_DIVE30,
   DIRECTOR_MUSIC_EGYPT,
   DIRECTOR_MUSIC_HELL,
   DIRECTOR_MUSIC_MIDDLEAGE,
   DIRECTOR_MUSIC_MGHOUSE,
   DIRECTOR_MUSIC_MGHELL,
   DIRECTOR_MUSIC_MGISLAND,
   DIRECTOR_MUSIC_MGEGYPT,
   DIRECTOR_MUSIC_MGINCA,
   DIRECTOR_MUSIC_MGMIDDLEAGE,
   DIRECTOR_MUSIC_MGJAPAN
} DirectorMusic;

typedef enum {
   DIRECTOR_CHARACTER_NONE = 0,
   DIRECTOR_CHARACTER_HOUSE,
   DIRECTOR_CHARACTER_ISLAND,
   DIRECTOR_CHARACTER_JAPAN,
   DIRECTOR_CHARACTER_INCA,
   DIRECTOR_CHARACTER_EGYPT,
   DIRECTOR_CHARACTER_MIDDLEAGE,
   DIRECTOR_CHARACTER_HELL
} DirectorCharacter;

#define CHARACTER_POSX 150
#define CHARACTER_POSY 620

//class ESceneMiniJeu;
class ESceneSequencer;
class EInventory;
class EInventoryItem;
//class EMap;

class ESceneDirector {
public:
   ESceneDirector();
   ~ESceneDirector();
   
   void GoToScene(const char *szSceneName, const char *szAdditionalName = "", bool bPlayWalkSound = true);
   void GoToVignette(const char *szSceneName, const char *szAdditionalName = "", bool bAlpha = true);
   void CloseVignette()
   {
      if (!_lpVignetteCurrent) {
         K_LOG("Close vignette not existing");
         return;
      }
      _lpVignetteCurrent->StartFade(SCENE_SHOWOUT);
      PrepareMusic(_lpSceneCurrent);
      _bClosingVignette = true;
   }
   bool CloseVignetteDone()
   {
      return (_lpVignetteCurrent == NULL);
   }
   void GoToMiniGame(const char *szGameName, EMiniJeuBase *lpMiniJeu, const char *szSceneName="minijeu");
   void ShowIHM(bool b);
   void ShowMouse(bool b);
   void ShowCurrentCharacter(bool b);
   void ChangeMouseCursor(DirectorMouseCursor cursor);
   void ChangeCharacter(DirectorCharacter character);
   void ChangeCharacter(const char *szCharacter);
   void PrepareMouseCursor(DirectorMouseCursor cursor);
   void PrepareMouseCursor(const char *szMousePointer);
   void PrepareMusic(EScene *lpScene);
   void PlayDirectorSound(DirectorSound sound);
   void PlayDirectorSound(const char *szSoundName);
   void ChangeMusic(DirectorMusic music, bool bWithoutFadeout=false);
   // Tremblement de terre
   void EarthQuake(bool b, const char *szSoundName = NULL);
   bool isEarthQuakeCompleted() {
      if(_lpAnimEarthQuake) {
         return _lpAnimEarthQuake->isCompleted();
      } else {
         return true;
      }
   }
   
   const char *GetLastParsedSceneName() { return _strLastParsedSceneName.c_str();}
   EScene *GetCurrentScene();
   EScene *GetCurrentVignette();
   EScene *GetCurrentMinigame();
   EScene *GetCurrentEScene();
   ESceneSequencer *getSequencer() {
      return _lpSceneSequencer;
   }
   char *getCurrentSceneName() {
      return _szSceneCurrent;
   }
   
   /* Gestion de l'inventaire */
   EInventoryItem *PickupItem(const char *szItemName, float fFromX, float fFromY, int nNbPartNeeded = 1, int nNumPart = 1);
   bool isTravellingItem()
   {
      return _lpTravellingItem != NULL;
   }
   void HoldItem(const char *szItemName);
   void DropItem(const char *szItemName);
   EInventoryItem *getHoldItem() {
      return _lpHoldItem;
   }
   EInventory *getInventory() {
      KAssert(_lpInventory);
      return _lpInventory;
   }
   void EndTravellingItem();  // Force la fin d'un ramassage
   void resetHoldItem();
   // Intervient quand un item a été ramassé en plusieurs morceaux et qu'il est complet
   const char *MapPartialInvItemFullInvItem(const char *szItemName);
   void ItemIsFull(const char *szItemName);
   // Intervient quand un item est combiné avec un autre
   bool CombineItem(const char *szItemNameInv, const char *szItemNameHold);
   
   // Retourne le KGraphic du personnage courant
   KGraphic *getCurrentCharacter()
   {
      if (!GetCurrentScene()) {
         return NULL;
      }
      if (GetCurrentScene()->_strSceneUniverseName == "house") {
         return _lpsimgCharacterHouse->getGraphic();
      }
      if (GetCurrentScene()->_strSceneUniverseName == "house" && isDialogMode()) {
         return _lpsimgCharacterHouseDialog->getGraphic();
      }
      if (GetCurrentScene()->_strSceneUniverseName == "island") {
         return _lpsimgCharacterIsland->getGraphic();
      }
      if (GetCurrentScene()->_strSceneUniverseName == "japan") {
         return _lpsimgCharacterJapan->getGraphic();
      }
      if (GetCurrentScene()->_strSceneUniverseName == "inca") {
         return _lpsimgCharacterInca->getGraphic();
      }
      if (GetCurrentScene()->_strSceneUniverseName == "egypt") {
         return _lpsimgCharacterEgypt->getGraphic();
      }
      if (GetCurrentScene()->_strSceneUniverseName == "middleage") {
         return _lpsimgCharacterMiddleage->getGraphic();
      }
      if (GetCurrentScene()->_strSceneUniverseName == "hell") {
         return _lpsimgCharacterHell->getGraphic();
      }
      return NULL;
   }
   
   /* Gestion de l'interactivité */
   // Capture la souris, les clics ne sont plus envoyés à la scène
   bool isDialogMode() {
      return _bDialogMode;
   }
   bool isLockMode() {
      return _bLockMode;
   }
   void LockModeStart() {
      _bLockMode = true;
      // RAZ du contexte d'info en survol
      if (_lpSceneCurrent) {
         _lpSceneCurrent->SetMouseOverInfo("", "");
      }
      if (_lpVignetteCurrent) {
         _lpVignetteCurrent->SetMouseOverInfo("", "");
      }
   }
   void LockModeStop() {
      _bLockMode = false;
   }
   void DialogModeStart() {
      // RAZ du contexte d'info en survol
      if (_lpSceneCurrent) {
         _lpSceneCurrent->SetMouseOverInfo("", "");
      }
      if (_lpVignetteCurrent) {
         _lpVignetteCurrent->SetMouseOverInfo("", "");
      }
      // Rend invisible le bouton back
      if (_lpSceneCurrent) {
         if (_lpSceneCurrent->GetObjectImageByName("back")) {
            _lpSceneCurrent->SetVisible("back", false);
         }
      }
      if (_lpVignetteCurrent) {
         if (_lpVignetteCurrent->GetObjectImageByName("back")) {
            _lpVignetteCurrent->SetVisible("back", false);
         }
      }
      if (_bDialogMode) {
         return;
      }
      _bDialogMode = true;
      ShowIHM(false);
   }
   // Libère la souris, les clics sont à nouveau envoyés à la scène
   void DialogModeStop() {
      _bDialogMode = false;
      if (_bLockMode) {
         ShowIHM(false);
      }
      else {
         if (_lpVignetteCurrent) {
            if (_lpVignetteCurrent->IsNeedIHM()) {
               ShowIHM(true);
            }
         }
         else if (_lpSceneCurrent) {
            if (_lpSceneCurrent->IsNeedIHM()) {
               ShowIHM(true);
            }
         }
//         // Rend visible le bouton back
//         if (_lpSceneCurrent) {
//            if (_lpSceneCurrent->GetObjectImageByName("back")) {
//               _lpSceneCurrent->SetVisible("back", true);
//            }
//         }
//         if (_lpVignetteCurrent) {
//            if (_lpVignetteCurrent->GetObjectImageByName("back")) {
//               _lpVignetteCurrent->SetVisible("back", true);
//            }
//         }
      }
   }
   // Mode cinématique
   void CinematicModeStart() {
      // RAZ du contexte d'info en survol
      if (_lpSceneCurrent) {
         _lpSceneCurrent->SetMouseOverInfo("", "");
      }
      if (_lpVignetteCurrent) {
         _lpVignetteCurrent->SetMouseOverInfo("", "");
      }
      // Rend invisible le bouton back
      if (_lpSceneCurrent) {
         if (_lpSceneCurrent->GetObjectImageByName("back")) {
            _lpSceneCurrent->SetVisible("back", false);
         }
      }
      if (_lpVignetteCurrent) {
         if (_lpVignetteCurrent->GetObjectImageByName("back")) {
            _lpVignetteCurrent->SetVisible("back", false);
         }
      }
      _bCinematicMode = true;
      ShowIHM(false);
   }
   // Fin de mode cinématique
   void CinematicModeStop() {
      _bCinematicMode = false;
      // Rend visible le bouton back
//      if (_lpSceneCurrent) {
//         if (_lpSceneCurrent->GetObjectImageByName("back")) {
//            _lpSceneCurrent->SetVisible("back", true);
//         }
//      }
//      if (_lpVignetteCurrent) {
//         if (_lpVignetteCurrent->GetObjectImageByName("back")) {
//            _lpVignetteCurrent->SetVisible("back", true);
//         }
//      }
   }
   bool isCinematicMode() {
      return _bCinematicMode;
   }

   // Affiche le gestionnaire de tâches
   void SetTaskManager(bool b);

   // affiche le diary
   void ShowDiaryManager(bool b);

   // Affiche/cache la map
   void ShowMapManager(bool b);
   EMap* getMapManager();
   EDiary* getDiaryPtr();

   // affiche la fenetre de saisie si besoin, renvoie vrai quand pseudo tapé ou si pas eu besoin
   bool enterPseudo();
   bool hasEnterPseudo;

   bool isTaskManager()
   {
      return _bTaskManager;
   }
   // Hint system
   bool isHintAvailable()
   {
      return _fHintLevel == 100.0f;
   }
   void SetHintLevel(float f)
   {
      _fHintLevel = f;
   }
   
   // Energie
   EnergyParticlesManager *getEnergyParticlesManager()
   {
      return _lpEnergyParticlesManager;
   }
   
   // Traite le cas particulier du Back button
   void SetupBackButton();
   // Informe le director qu'au prochain changement de scène, il faut jouer le walksound
   void setWalk()
   {
      _bWalk = true;
   }
   void setZoomin(bool b)
   {
      _bSceneZoomin = b;
   }
   // Passe en mode Loading
   void setLoading(bool b)
   {
      _bLoading = b;
//      Draw();
//      if (b) {
//         KTextFace *lpFont = EFontBank::getFont("tahoma.ttf",12);
//         lpFont->setColor(1.0f, 1.0f, 1.0f, 1.0f);
//         lpFont->drawStringCentered("Loading...", 0, 1024, 620);
//         KGame::getGame()->endFrame();
//         KGame::getGame()->beginFrame();
//      }
   }
   // Affichage aléatoire d'une phrase pour utilisation infructueuse d'un objet
   void NiceTryVoiceOver();
   
   void Logic();
   void Check();
   void Draw();
   
   void CloseCurrentScene();
   void CloseCurrentMiniGame();
   
   bool _bOptionsRequest;
   bool _bNewPlayerRequest;
   bool _bPlayerRequest;
#ifdef ENIGMA_STATS
   bool _bDebugDisplay;
#endif
   bool _bSoftCursor;      // utilisé par le watchdog
   bool _bSystemCursor;    // utilisé pour le .ini

   void SetVolume(long lVolume) {
      _lpMusicArranger->SetVolume(lVolume);
   }
   bool isSystemCursor() {
      return _bSystemCursor;
   }
   void SetSystemCursor(bool bSystemCursor) {
      if (_bSystemCursor == bSystemCursor)
         return;

      _bSystemCursor = bSystemCursor;
      _bSoftCursor = !bSystemCursor;
      
      if (_bSystemCursor) {
         KInput::showPointer ();
      } else {
         KInput::hidePointer ();
      }
   }
   void SetHexagramme(bool b)
   {
      //_bHexagram = b; // !!!:dmekersa:20110225 Obsolete, on utilise une task maintenant
      if (_lpVignetteCurrent) {
         if (_lpVignetteCurrent->IsNeedIHM()) {
            ShowIHM(true);
         }
      }
      else if (_lpSceneCurrent) {
         if (_lpSceneCurrent->IsNeedIHM()) {
            ShowIHM(true);
         }
      }
   }

   void OpenDialogbox(const char *szText);
   void CloseDialogbox();
   void OpenDialogboxSpirit(const char *szText);
   void CloseDialogboxSpirit();
   void OpenDialogboxYN(const char *szText, int *nReturn);
   void CloseDialogboxYN(bool bYesButtonsPressed=false);

   // met à jour la task list, efface tout les objectifs/hint de sa liste et recharge ceux courant.
   // moins performant que le addObjectiveTaskList pour rajouter un objectif
   void majTaskList();
   
   // ajout d'un objectif dans la task list et affichage de cet objectif en fondu
   void addObjectiveTaskList(const char* objective);

   // Est-ce qu'une boite de dialogue est affichée ?
   bool isDialogboxActive() {
      return _bDialogActive;
   }
   
   void ResetTimeFrame() {
      _bResetTimeframe = true;
   }
   bool getResetTimeFrame() {
      bool b = _bResetTimeframe;
      _bResetTimeframe = false;
      return b;
   }
   
   EMap *getMap() {
      return _lpMap;
   }
   
   EBookCtrl *getBookCtrl() {
      return _lpBookCtrl;
   }
   
   void UpdateInventoryButtons();

   static ESceneDirector *_lpSceneDirector;
private:
   
   /* KGraphic pour dessiner si on a besoin */
   KGraphic *_lpBgGraphic;
   /* Capture/Release de la souris */
   bool _bDialogMode;
   bool _bLockMode;
   /* Cinématique */
   bool _bCinematicMode;
   /* Gestionnaire de tâches */
   bool _bTaskManager;
   /* Gestionnaire de map */
   bool _bMapManager;
   /* Loading en cours */
   bool _bLoading;
   bool _bResetTimeframe;

   /* IHM */
   ESmartImage *_lpsimgIHM;
   ESmartImage *_lpsimgHexagramme;
   ESmartImage *_lpsimgHexagramme_blue;
   ESmartImage *_lpsimgHexagramme_red;
   ESmartImage *_lpsimgHexagramme_green;
   ESmartImage *_lpsimgHexagramme_yellow;
   ESmartImage *_lpsimgHexagramme_purple;
   ESmartImage *_lpsimgTaskManager;
   DirectorCharacter _currentCharacter;
   ESmartImage *_lpsimgCharacterHouse;
   ESmartImage *_lpsimgCharacterHouseDialog;
   ESmartImage *_lpsimgCharacterUniverseDialog;
   ESmartImage *_lpsimgCharacterIsland;
   ESmartImage *_lpsimgCharacterJapan;
   ESmartImage *_lpsimgCharacterInca;
   ESmartImage *_lpsimgCharacterMiddleage;
   ESmartImage *_lpsimgCharacterEgypt;
   ESmartImage *_lpsimgCharacterHell;
   ESmartImage *_lpsimgDialogCache;
   /* Curseur souris */
   KGraphic *_lpGraphicArrowPoint;
   KGraphic *_lpGraphicArrowWalk;
   KGraphic *_lpGraphicArrowWalkBack;
   KGraphic *_lpGraphicArrowDoor;
   KGraphic *_lpGraphicArrowAction;
   KGraphic *_lpGraphicArrowLook;
   KGraphic *_lpGraphicArrowInfo;
   KGraphic *_lpGraphicArrowTalk;
   KGraphic *_lpGraphicArrowTake;
   KGraphic *_lpGraphicArrowWait;
   KGraphic *_lpGraphicArrowPuzzle;
   ESmartImage *_lpsimgArrow;
   KTextFace *_lpFontMouseOverInfo;
   DirectorMouseCursor _cursor;
   bool _bHexagram;
   bool _bHexagramVisible;

   /* Music */
   int _nCurrentMainMusic;
   /* Music Arranger */
   EMusicArranger *_lpMusicArranger;
   
   /* Nom de la scène passée */
   char _szScenePrevious[256];
   /* Nom de la scène à venir */
   char _szSceneNext[256];
   char _szSceneNextAdditionalName[256];
   /* Nom de la scène en cours */
   char _szSceneCurrent[256];
   /* Pointeur vers la scène courante */
   std::string _strLastParsedSceneName;
   EScene *_lpSceneCurrent;
   /* Vers la scène suivante */
   bool _bSceneNext;
   bool _bSceneZoomin;
   /* Pointeur vers la vignette courante */
   EScene *_lpVignetteCurrent;
   /* Vignette se ferme */
   bool _bClosingVignette;
   /* Pointeur vers le mini-jeu courant */
   SceneMiniJeu *_lpMiniGameCurrent;
   /* Mini jeu se ferme */
   bool _bClosingMiniGame;
   /* On marche */
   bool _bWalk;
   /* On était en mode "Check not allowed */
   bool _bWasCheckNotAllowed;

   /* Animation de changement de scène */
   KCounter *_lpAnimSceneOut;
   
   // Sequenceurs
   ESceneSequencer *_lpSceneSequencer;
   
   // Inventaire
   EInventory *_lpInventory;
   // Liste de tâches
   //ETaskList *_lpTaskList;

   EBookCtrl *_lpBookCtrl;

   // carte
   EMap* _lpMap;
   // Item en transit de la scène vers l'inventaire
   EInventoryItem *_lpTravellingItem;
   ESmartImage *_lpsimgTravellingItem;
   ParticleEmitter *_lpTravellingEmitter;
   // Item tenu par le joueur
   EInventoryItem *_lpHoldItem;
   ESmartImage *_lpsimgHoldItem;
   
   // Hint system
   float _fHintLevel;

   /* UI */
   KTextFace *_lpFont;
   KGraphic *_lpButtonGraphic;
   KUIElement *_lpUIContainer;
   KUIButton *_lpButtonLeft;
   KUIButton *_lpButtonRight;
   KUIButton *_lpButtonGuide;
   KUIButton *_lpButtonMap;
   KUIButton *_lpButtonEnvelope;
   KUIButton *_lpButtonDiaryClosed;
   KUIButton *_lpButtonDiaryOpen;
   KUIButton *_lpButtonMenu;
   static void _myUIMessageHandler (KUIMessage *lpUIMessage) {};
   /* Dialogbox */
   bool _bDialogActive;
   // Box OK
   KUIImage *_lpUIDialogboxWindow;
   KGraphic *_lpDialogboxGraphic;
   KGraphic *_lpDialogboxButtonsGraphic;
   KGraphic *_lpIHMMiniButtonGraphic;
   KGraphic *_lpIHMDiaryClosedGraphic;
   KGraphic *_lpIHMDiaryOpenGraphic;
   ESmartImage *_lpIHMDiaryOpenSmartImage;
   KUILayout *_lpDialogboxLayout;
   KUIText *_lpDialogboxText;
   KUIButton *_lpDialogboxOKButton;
   // Box Spirit
   KUIImage *_lpUIDialogboxSpiritWindow;
   KGraphic *_lpDialogboxSpiritGraphic;
   KGraphic *_lpDialogboxSpiritButtonsGraphic;
   KUILayout *_lpDialogboxSpiritLayout;
   KUIText *_lpDialogboxSpiritText;
   KUIButton *_lpDialogboxSpiritOKButton;
   ParticleEmitter *_lpParticleEmitterDialogSpirit;
   ParticleEmitter *_lpParticleEmitterDiary;
   // Box Yes/No
   KUIImage *_lpUIDialogboxYNWindow;
   KUILayout *_lpDialogboxYNVLayout;
   KUILayout *_lpDialogboxYNHLayout;
   KUIText *_lpDialogboxYNText;
   KUIButton *_lpDialogboxYNYesButton;
   KUIButton *_lpDialogboxYNNoButton;
   int *_nYesButtonPressed;
   /* Effet Earthquake */
   KCounter *_lpAnimEarthQuake;
   /* Effet particules d'énergies */
   EnergyParticlesManager *_lpEnergyParticlesManager;
   
   friend class Scene_Debug_Preload;
   bool CreateSceneFromName(const char *szSceneName, EScene *&lpScene, const char* szAdditionalName="", bool bLoadOnly = false);
   void DrawIHM();
};

#endif