/**
 * Enigma
 * The Casual Adventure Game
 *
 */
#ifdef DEBUGSTUDIO
   #ifdef _VLD
      // Enhanced Memory Leak Detection http://vld.codeplex.com/
      #include "vld.h"
   #endif
#endif

#include "KPTK.h"
#include "global.h"
#include "MyGame.h"
/* Windows uses WinMain(). Mac, iPhone and iPad use kanjiMain() */

#ifdef K_WIN32
INT WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT iCmdShow) {
#ifdef _VLD
   VLDEnable();
#endif
#else
int kanjiMain (int argc, char **argv) {
#endif
   MyGame *lpGame;


   /* Create, execute and release game */

   lpGame = new MyGame;
   lpGame->main ();
   delete lpGame;

#ifdef DUMP_MEMORY_LEAK
   _CrtDumpMemoryLeaks();
#endif
   return 0;
}
