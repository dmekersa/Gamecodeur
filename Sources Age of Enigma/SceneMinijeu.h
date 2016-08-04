#pragma once

#include "EScene.h"
#include "KPTK.h"
#include "ETutoBox.h"

class EMiniJeuBase;
class EBox;

class SceneMiniJeu : public EScene {
public:
   SceneMiniJeu(ESceneDirector *lpSceneDirector, EMiniJeuBase *lpMiniJeu);
   ~SceneMiniJeu();
   void Init ();
   void Close ();
   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();
   bool IsNeedIHM()
   {
      return false;
   }

   /* spécifique à SceneMiniJeu */
   bool HasEnded();
   bool isResolved();
   
   void CheckTuto();
private:
	KGraphic *_lpBgGraphic;
   EMiniJeuBase *_lpMiniJeu;
   /* UI */
   KTextFace *_lpFont;
   KTextFace *_lpFontNotice;
   static void _myUIMessageHandler (KUIMessage *lpUIMessage);

   bool ObjectClicked(const char *szObjectName, float x, float y);
   bool ObjectOver(char *szObjectName, float x, float y);

   bool _bResolved;

   EBox *_lpHintDialog;

   ETutoBox *_lpTuto;
};
