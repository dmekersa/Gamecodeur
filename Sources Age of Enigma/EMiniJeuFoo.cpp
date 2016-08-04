#include "EMiniJeuFoo.h"
#include "assert.h"
#include "MyGame.h"
#include "EPointer.h"
#include "FontBank.h"

EMiniJeuFoo::EMiniJeuFoo(void)
{
   /* Load font */
   EGLL();
   _lpFont = EFontBank::getFont("acmesa.ttf",20);
   EGLR();

   // Charge l'image du token dans un KGraphics
   _lpGraphicToken = KPTK::createKGraphic ();
   bool bSuccess = _lpGraphicToken->loadPicture (KMiscTools::makeFilePath ("tile.png"), true, true);
   if (!bSuccess) {
//      MyGame::showFileError ("tile.png");
      return;
   }
   _lpGraphicToken->setTextureQuality (true);   
   /* Token */
   _lpToken = new FooToken(0.0,30.0,(int)_lpGraphicToken->getWidth(), (int)_lpGraphicToken->getHeight());
   /* Smart image affichant le token */
   _lpsimgToken = new ESmartImage(_lpGraphicToken, _lpToken->_x, _lpToken->_y, 1, true, true, 250);
   _lpsimgToken->SetRotation(-20,20,400,true,K_COUNTER_EASEINOUT);
   _lpsimgToken->Rotate(false);
   
//   _hintLevel = MINIJEUFOO_NORMAL;
   
   // Préparation de Target pour dessiner le mini jeu dedans...
   _lpGraphicTarget = KPTK::createKGraphic();
   _lpGraphicTarget->makeRenderTarget(1024, 768, true, false, true);
   _lpGraphicTarget->setTextureQuality(true);
}

EMiniJeuFoo::~EMiniJeuFoo(void)
{
   delete _lpFont;
   delete _lpGraphicToken;
   delete _lpsimgToken;
   delete _lpToken;
   delete _lpGraphicTarget;
}

/** 3 steps rendering **/
void EMiniJeuFoo::Check ()
{
   if (Mouse.LeftEvent())
      {
		  // Le token Ètait dÈj‡ tenu par la souris
      if (_lpToken->_bHold)
      {
         _lpToken->_bHold = false;
         _lpsimgToken->Rotate(false);
		 Mouse.LeftEventClose();
      }
      else
      {
         // Si le token est cliquÈ
         float x,y;
         x = Mouse.x;
		 y = Mouse.y;
         if ( (x >= _lpToken->_x && x <= _lpToken->_x+_lpToken->_w) && (y >= _lpToken->_y && y <= _lpToken->_y+_lpToken->_h))
         {
            _lpToken->_bHold = true;
			_lpsimgToken->Rotate(true);
			Mouse.LeftEventClose();
         }
      }
   }
}

void EMiniJeuFoo::Logic ()
{
   // L'image du token fait sa vie
   _lpsimgToken->Logic();
   // Le token est "tenu" par le pointeur
   if (_lpToken->_bHold)
   {
      _lpToken->_x = Mouse.x - _lpToken->_w/2;
      _lpToken->_y = Mouse.y - _lpToken->_h/2;
      _lpsimgToken->Move(_lpToken->_x,_lpToken->_y);
   }
}

void EMiniJeuFoo::Draw ()
{
   //_lpFont->drawStringFromLeft("Exemple de mini jeu (hors target)",_nX,_nY-30);
   //   _lpGraphicTarget->clearRenderTarget();
   //   _lpGraphicTarget->beginRenderToTarget();
   KGame::getGame()->_lpKWindow->setWorldView((float)_nX,(float)0-_nY,0,1,false);
   
   _lpFont->drawStringFromLeft("Exemple de mini jeu (in target)",0,0);

   // Dessine le token
   _lpsimgToken->Draw();
   
   //   _lpGraphicTarget->endRenderToTarget();
   //   _lpGraphicTarget->blitAlphaRect(0, 0, 730, 565, _nX, _nY);
   KGame::getGame()->_lpKWindow->setDefaultWorldView();
}

/* Pilotage du mini jeu */

// Choix de la variation (modèle ou sujet)
void EMiniJeuFoo::SetVariation(int nVariation)
{
   
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuFoo::IsResolved()
{
   return false;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuFoo::Resolve()
{
//   _hintLevel = MINIJEUFOO_RESOLVED;
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuFoo::ActivateHint(int nHintLevel)
{
   switch (nHintLevel) {
      case 1:
//         _hintLevel = MINIJEUFOO_HINT1;
         break;
      case 2:
//         _hintLevel = MINIJEUFOO_HINT2;
         break;
      case 3:
//         _hintLevel = MINIJEUFOO_HINT3;
         break;
      default:
         break;
   }
}


// Preload images et sons
void EMiniJeuFoo::Preload() {
   EMiniJeuBase::Preload();
}
