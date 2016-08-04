/**
 * Kanji example - our game class definitions - public domain
 */

/* Include definitions */
#include "KGame.h"

/* One particle */

class Particle : public KObjectListable {
public:
   /* Current center coordinates */
   float x, y;

   /* Velocity over x, y */
   float vx, vy;

   /* Color */
   float fColor[4];

   /* Scale */
   float fScale;

   /* Current age */
   float fAge;

   /* Age increase at each tick */
   float fAgeIncr;
};

/* Our game class definition */

class KUINewPlayerWindow;

class MyGame : public KGame {
public:
   /** Constructor */
   MyGame ();

   /**
    * Get the name of this game
    *
    * \return game's name
    */
   const char *getGameName (void) { return KStr("GAME_TITLE"); }

   /**
    * Get the name of this game, as a safe string that can be used to create folders and files
    *
    * \return safe game's name
    */
#ifndef BFG_TEASING
   const char *getSafeGameName (void) { return "Age Of Enigma"; }
#else
   const char *getSafeGameName (void) { return "Age Of Enigma survey2i"; }
#endif

   void saveSettings();
   /**
    * Get the name of this game's development studio, as a safe string that can be used to create folders and files
    *
    * \return safe game's developer name
    */
#ifdef BFG_TEASING
   const char *getSafeDeveloperName (void) { return "Casual Box Survey"; }
#else
   const char *getSafeDeveloperName (void) { return "Casual Box"; }
#endif
   /**
    * Get the version of the game that this engine is running
    *
    * \return game's version
    */
   const char *getGameVersion (void) { return "1.0.4"; }
   // 1.0.3c : splashs screens affichés à durées égales sans fondu enchainé

   /**
    * Load settings and create game window. Derive and implement this method
    *
    * \return game window, or NULL to exit
    */
   virtual KWindow *createGameWindow (void);

   /**
    * Run the game. Derive and implement this method
    */
   virtual void run (void);

   /**
    * Close game window and save settings. Derive and implement this method
    */
   virtual void destroyGameWindow (KWindow *_lpKWindow);

   /**
    * Display missing file error
    *
    * \param lpszFileName name of missing file
    */
   static void showFileError (const char *lpszFileName);
   
   KUINewPlayerWindow * MyNewPlayerWindow;
   
private:

   /* Event handler */
   static bool myEventHandler (KEvent *lpKEvent);

   /* UI message handler */
   static void myUIMessageHandler (KUIMessage *lpUIMessage);

   /** true if currently in windowed mode, false if fullscreen */
   bool _bWindowed;
   
   bool _bSystemCursor;

   /** Current music volume */
   float _fMusicVolume;
   
   /** Current SFX volume */
   float _fSfxVolume;

   /** Buffer for composing paths to game state files */
   char _szPathBuf[K_MAXPATH];

   /** true if we need to play the 'button clicked' sound effect */
   static bool g_bPlayButtonSound;

   /** List of Particles */
   static KList<Particle> g_lParticles;
   
   void BuildOPtionsWindow (KUIElement *lpUIConainer, bool bWindowed);
};

void showFileError (const char *lpszFileName);

void timeOut();
