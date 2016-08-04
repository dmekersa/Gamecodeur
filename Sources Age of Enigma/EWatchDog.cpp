#include "MyGame.h"
#include "CreepingSmoke.h"
#include "ESceneDirector.h"

#include "EWatchDog.h"

#define FRAMEWINDOW 30

namespace {
   bool _bWatchDogEnabled = false;
   bool _bLastSceneTransitionState = false;
   int nFPS = 0;
   long lFPS = 0;
   int lastFPS[FRAMEWINDOW];
   int indexFPS = 0;
   int mFPS = 0;

   char buff[3*FRAMEWINDOW+1+FRAMEWINDOW/5+1];
}

void EWatchDog::Enable() {
   if (IsEnable())
      return;

   _bWatchDogEnabled = true;
   for (int i=0; i<FRAMEWINDOW; i++)
      lastFPS[i] = 60;
}
void EWatchDog::Disable() {
   _bWatchDogEnabled = false;
}
bool EWatchDog::IsEnable() {
   return _bWatchDogEnabled;
}
void EWatchDog::SceneTransition(bool bState) {
   if (_bLastSceneTransitionState == bState)
      return;

   _bLastSceneTransitionState = bState;
   if (!bState) {
      lFPS = 0;
      nFPS = 1;
   }
}

void EWatchDog::EndFrame() {
   if (!IsEnable() || _bLastSceneTransitionState)
      return;

   KWindow *lpKWindow = MyGame::getGame()->getKWindow();
   /* Calc FPS */
   lFPS += lpKWindow->getFrameTime();
   if (lFPS > 1000) {
      indexFPS = (indexFPS +1) % FRAMEWINDOW;
      lastFPS[indexFPS] = --nFPS;
      lFPS -= 1000;
      nFPS = 0;

      // calcul de la moyenne du nombre de FPS sur une minutes
      mFPS =0;
      for (int i=0; i<FRAMEWINDOW; i++)
         mFPS += lastFPS[i];
      mFPS /= FRAMEWINDOW;

      // gestion pointeur système
      if (!SCENEDIRECTOR->_bSystemCursor ) {
// DESACTIVE ! Les joueurs passeront pas l'option.
//         if (mFPS < 20 && SCENEDIRECTOR->_bSoftCursor) {
//            K_LOG("EWatchDog::EndFrame -- SCENEDIRECTOR->_bSoftCursor = false");
//            SCENEDIRECTOR->_bSoftCursor = false;
//            KInput::showPointer ();   
//         } else if (mFPS > 20 && !SCENEDIRECTOR->_bSoftCursor) {
//            K_LOG("EWatchDog::EndFrame -- SCENEDIRECTOR->_bSoftCursor = true");
//            SCENEDIRECTOR->_bSoftCursor = true;
//            KInput::hidePointer ();
//         }
         if (mFPS > 20 && !SCENEDIRECTOR->_bSoftCursor) {
            K_LOG("EWatchDog::EndFrame -- SCENEDIRECTOR->_bSoftCursor = true");
            SCENEDIRECTOR->_bSoftCursor = true;
            KInput::hidePointer ();
         }
      }

      // ajustement du frameRate des emetteurs
      if (mFPS < 30 && CreepingSmokeEmitter::rateFactor > 0) {
         CreepingSmokeEmitter::rateFactor -= 0.035f;
      } else if (mFPS > 35 && CreepingSmokeEmitter::rateFactor < 1){
         CreepingSmokeEmitter::rateFactor += 0.035;
      }
   }
   else {
      nFPS++;
   }
}

const char* EWatchDog::LastFPS() {
   sprintf(buff,"%02d",lastFPS[indexFPS]);
   return buff;
}
const char* EWatchDog::MoyenneFPS() {
   sprintf(buff,"%02d",mFPS);
   return buff;
}
const char* EWatchDog::TabFPS() {
   memset(buff,0,sizeof(buff));
   for (int i=0; i<FRAMEWINDOW; i++) {
      sprintf(buff+i*3,"%02d ",lastFPS[(indexFPS+i+1) % FRAMEWINDOW]);
   }
   return buff;
}