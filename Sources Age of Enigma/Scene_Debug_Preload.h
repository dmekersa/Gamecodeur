/*
 *  Scene_Debug_Preload.h
 *  enigma
 *
 *  Created by Rockford on 14/06/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_DEBUGPRELOAD_H
#define  _ENIGMA_DEBUGPRELOAD_H

#include "EScene.h"
#include "EMiniJeuBase.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Debug_Preload : public EScene {
public:
   Scene_Debug_Preload(ESceneDirector *lpSceneDirector);
   ~Scene_Debug_Preload();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   void Callback(const char *szParam);
   
   void cleanGotoScene();
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   
   bool IsNeedIHM()
   {
      return false;
   }

   const char* GetCurSceneName() {if (_lpScene!=NULL) return _lpScene->_strSceneName.c_str(); return "none";}
  
private:
   KGraphic *_lpBgGraphic;
   EScene *_lpScene;
   EMiniJeuBase *_lpMiniJeu;

   void GenNextSceneIndex();

   bool _bTestScene;
   bool _bTestMiniJeu;
   const char *_szCurrentScene;
   int _nSceneIndex;
   int _numFrame;
   bool _bRandomizeNextScene;
   
protected:
   
};

#endif