#include "assert.h"

#include "MyGame.h"
#include "ImageBank.h"
#include "EPointer.h"

#include "EMiniJeuTemplate.h"

EMiniJeuTemplate::EMiniJeuTemplate(void)
{

	SetGameType(MINIJEU_SUBMITGAME);
	SetTextsTrad("TEMPLATEMG");
	SetBackGround(EImageBank::getImage("mg_poltergeist_bg.jpg"));

	// Charge l'image du token dans un KGraphics
	_lpGraphicToken = KPTK::createKGraphic ();
	_lpGraphicToken = EImageBank::getImage("EConnectBRT.png");

	/* Token */
	_lpToken = new FooToken(0.0,30.0,(int)_lpGraphicToken->getWidth(), (int)_lpGraphicToken->getHeight());
	/* Smart image affichant le token */
	_lpsimgToken = new ESmartImage(_lpGraphicToken, _lpToken->_x, _lpToken->_y, 1, true, true, 250);
	_lpsimgToken->SetRotation(-20,20,400,true,K_COUNTER_EASEINOUT);
	_lpsimgToken->Rotate(false);

	_nGameStatus = MINIJEU_NORMAL;
}

EMiniJeuTemplate::~EMiniJeuTemplate(void)
{
	delete _lpsimgToken;
	delete _lpToken;
}

/** 3 steps rendering **/
void EMiniJeuTemplate::GameCheck ()
{
	if (Mouse.LeftEvent())
	{
		// Le token était déjà tenu par la souris
		if (_lpToken->_bHold)
		{
			_lpToken->_bHold = false;
			_lpsimgToken->Rotate(false);
			Mouse.LeftEventClose();
		}
		else
		{
			// Si le token est cliqué
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

void EMiniJeuTemplate::GameLogic ()
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

void EMiniJeuTemplate::GameDrawSet()
{
}

// Retourne true si le mini jeu est rŽsolu
bool EMiniJeuTemplate::GameIsResolve()
{
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuTemplate::GameResolve()
{
	_nGameStatus = MINIJEU_RESOLVED;
}

void EMiniJeuTemplate::GameReset()
{	
	_lpToken->_x = 0;
	_lpToken->_y =0;
	_lpsimgToken->Move(0,0);
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuTemplate::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  Reset(); 
		  break;
	  case MINIJEU_HINT2:
		  Reset();
		  break;
	  case MINIJEU_RESOLVED:
		  Resolve();
		  break;
	  default:
		  KAssert(false);
		  break;
	}
}

// Preload images et sons
void EMiniJeuTemplate::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_poltergeist_bg.jpg","",true,true);
   EImageBank::getImage("EConnectBRT.png","",true,true);

}
