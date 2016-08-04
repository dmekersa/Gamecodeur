/*
 *  Scene_Egypt_Tomb_Connect.h
 *  enigma
 *
 *  Created by Rockford on 28/12/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef  _ENIGMA_SCENEEGYPTTOMBCONNECT_H
#define  _ENIGMA_SCENEEGYPTTOMBCONNECT_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Scene_Egypt_Tomb_Connect: public EScene {
public:
   Scene_Egypt_Tomb_Connect(ESceneDirector *lpSceneDirector);
   ~Scene_Egypt_Tomb_Connect();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);
   bool ItemUsed(const char *szItemName, const char *szObjectName);
   void MiniGameDone(const char *szGameName, bool bIsRevolved);
   
   bool IsNeedIHM()
   {
      return true;
   }
private:
   KGraphic *_lpBgGraphic;
   KCounter *_lpAnimDoor;
   bool _bAnimDoor;
   void StartEffect();
   /* affiche les symboles dans leurs emplacements */
   void SetupGods(bool bClear);
   /* vérifie si les dieux sont dans le bon ordre */
   void CheckGods();
   /* Renvoie la zone contenant ce symbole */
   char _szZoneBuffer[256];
   char *getZoneContaining(const char *szSymbolName);
   
protected:
};

#endif