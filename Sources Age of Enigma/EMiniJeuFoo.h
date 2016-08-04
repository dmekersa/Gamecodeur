#ifndef EMINIJEUFOO_H
#define EMINIJEUFOO_H

#include "eminijeubase.h"

#include "eminijeuhelper.h"
#include "eminijeuconnect.h"
#include "ESmartImage.h"
#include "KPTK.h"

class FooToken;

class EMiniJeuFoo :
   public EMiniJeuBase
{
public:
   EMiniJeuFoo(void);
   virtual ~EMiniJeuFoo(void);

   /** 3 steps rendering **/
   void Check ();
   void Logic ();
   void Draw ();

   /* Pilotage du mini jeu */
   // Choix de la variation (modèle ou sujet)
   virtual void SetVariation(int nVariation);
   // Retourne true si le mini jeu est résolu
   virtual bool IsResolved();
   // Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
   virtual void Resolve();
   // Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
   virtual void ActivateHint(int nHintLevel);

   static void Preload();

private:
   KGraphic *_lpGraphicTarget;
   EFont *_lpFont;
   KGraphic *_lpGraphicToken;
   ESmartImage *_lpsimgToken;
   FooToken *_lpToken;
   int nVariation;
   int nHintLevel;
 //  MiniJeuHintLevel _hintLevel;   
};

class FooToken
{
public:
   FooToken(float x, float y, int w, int h)
   {
      _x = x;
      _y = y;
      _w = w;
      _h = h;
      _bHold = false;
   }
   float _x;
   float _y;
   int _w;
   int _h;
   bool _bHold;
};

#endif