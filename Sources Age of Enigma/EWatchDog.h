#ifndef EWATCHDOG_H
#define EWATCHDOG_H

class EWatchDog {
public:
   static void Enable();
   static void Disable();
   static bool IsEnable();
   static void SceneTransition(bool bSceneTransition);
   // True == begin scene transition, False == End scene transition

   static void EndFrame();

   static const char* LastFPS();
   static const char* MoyenneFPS();
   static const char* TabFPS();
};
#endif