/*
 *  Vignette_Cinematic.h
 *  enigma
 *
 *  Created by Rockford on 13/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */


#ifndef  _ENIGMA_VIGNETTE_CINEMATIC_H
#define  _ENIGMA_VIGNETTE_CINEMATIC_H

#include "EScene.h"
#include "KPTK.h"
#include "KSound.h"

class Vignette_Cinematic : public EScene {
public:
   Vignette_Cinematic(ESceneDirector *lpSceneDirector);
   ~Vignette_Cinematic();
   
   void Init ();
   void Close ();
   
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   
   bool ObjectClicked(const char *szObjectName, float x, float y);
//   bool ObjectOver(char *szObjectName, float x, float y);
//   bool ItemUsed(const char *szItemName, const char *szObjectName);
//   void MiniGameDone(const char *szGameName, bool bIsRevolved);
   
   bool IsNeedIHM()
   {
      return false;
   }
private:
   KTextFace *_lpFont;
   KGraphic *_lpGraphicMask;
   float _fScrollY;
   long _lHeight;
   void EndCinematic();
   bool _bClosing;
   const char *_lpString;
   
protected:
   
};

#endif