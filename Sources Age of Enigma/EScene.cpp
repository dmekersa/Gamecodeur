/*
 *  EScene.cpp
 *  Classe de base pour afficher une scène de jeu
 *
 *  Created by Rockford on 05/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "KPTK.h"
#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "MusicBank.h"
#include "ESceneDirector.h"
#include "EPointer.h"
#include "GlobalBank.h"
#include "ESceneParser.h"

#define SCENE_FADE_DURATION 100

/** Constructor */

EScene::EScene (ESceneDirector* lpSceneDirector)
{
   /* Load font */
   _lpFont = EFontBank::getFont("tahoma.ttf",12);

	/* Load background */
   _lpBgGraphic2 = NULL;
   _lpBgGraphic2 = KPTK::createKGraphic ();
   
   _lpAnimIn = new KCounter();
   _lpAnimOut = new KCounter();
   _bVignette = false;
   _bVignetteAlpha = false;
   _bRefreshTarget = true;
   
   _lpSceneDirector = lpSceneDirector;
   
   memset(_szNextSceneName, 0, 256);
   memset(_szNextVignetteName, 0, 256);
   memset(_szCurrentObjectOver, 0, 256);
   memset(_szWalkSound, 0, 256);
   memset(_szSoundDesign, 0, 256);
   memset(_szMouseOverInfo, 0, 256);
   _strSceneName = "";
   _strSceneAdditionalName = "";
   _strSceneUniverseName = "";
   _bKeepHoldItem = false;
   _lpSoundDesign = NULL;
   
   _lpGraphicTarget = NULL;
   
}

EScene::~EScene()
{
   K_LOG("EScene::~EScene() : %s",_strSceneName.c_str());
   StopSoundDesign();
   XDELETE(_lpGraphicTarget);
   XDELETE(_lpBgGraphic2);
   XDELETE(_lpAnimIn);
   XDELETE(_lpAnimOut);   
   XDELETE(_lpFont);
   // Termine l'objet en cours de ramassage si besoin
   _lpSceneDirector->EndTravellingItem();
}

/**
 * Get an object stored in a KList by his name
 *
 * \param szObjectName: name of scene object
 */
SceneObjectImage *EScene::GetObjectImageByName(const char *szObjectName)
{
	EObjectContainer *obj;
	bool bGotNext = EObjectContainer::g_lpListObj.getHead (obj);
	if (bGotNext) {
		while (bGotNext) {
         if (strcmp(obj->_szObjectName, szObjectName) == 0 && obj->_type == SCENEOBJECT_IMAGE)	// Objet trouvé
			{
				return obj->_lpObjectImage;
			}
			// Suivant
			bGotNext = EObjectContainer::g_lpListObj.getNext (obj);
		}
	} 
	return NULL;
}

SceneObjectText *EScene::GetObjectTextByName(const char *szObjectName)
{
	EObjectContainer *obj;
	bool bGotNext = EObjectContainer::g_lpListObj.getHead (obj);
	if (bGotNext) {
		while (bGotNext) {
         if (strcmp(obj->_szObjectName, szObjectName) == 0)	// Objet trouvé
			{
				KAssert (obj->_type == SCENEOBJECT_TEXT);
				return obj->_lpObjectText;
			}
			// Suivant
			bGotNext = EObjectContainer::g_lpListObj.getNext (obj);
		}
	} 
	return NULL;
}

SceneObjectParticleEmitter *EScene::GetObjectEmitterByName(const char *szObjectName)
{
	EObjectContainer *obj;
	bool bGotNext = EObjectContainer::g_lpListObj.getHead (obj);
	if (bGotNext) {
		while (bGotNext) {
			if (strcmp(obj->_szObjectName, szObjectName) == 0)	// Objet trouvé
			{
				KAssert (obj->_type == SCENEOBJECT_PARTICLEEMITTER);
				return obj->_lpObjectEmitter;
			}
			// Suivant
			bGotNext = EObjectContainer::g_lpListObj.getNext (obj);
		}
	} 
	return NULL;
}

SceneObjectAnimation *EScene::GetObjectAnimationByName(const char *szObjectName)
{
	EObjectContainer *obj;
	bool bGotNext = EObjectContainer::g_lpListObj.getHead (obj);
	if (bGotNext) {
		while (bGotNext) {
			if (strcmp(obj->_szObjectName, szObjectName) == 0)	// Objet trouvé
			{
				KAssert (obj->_type == SCENEOBJECT_ANIMATION);
				return obj->_lpObjectAnime;
			}
			// Suivant
			bGotNext = EObjectContainer::g_lpListObj.getNext (obj);
		}
	} 
	return NULL;
}

void EScene::GetObjectPosition(const char *szObjectName, int &x, int &y, bool bCenterX, bool bCenterY)
{
	SceneObjectImage *obj;
	obj = EScene::GetObjectImageByName(szObjectName);
	if (obj != NULL)
	{
		x = (int)obj->getCurrentX();
      y = (int)obj->getCurrentY();
      if (bCenterX) {
         x += obj->_w / 2;
      }
      if (bCenterY) {
         y += (int)obj->_h / 2;
      }
	}
   else {
      KAssert(false);
   }
}

void EScene::setName(const std::string& strName)
{
   _strSceneName = strName;
}

void EScene::StartFade(SceneFadeState state)
{
   switch (state) {
      case SCENE_STABLE:
         _State = SCENE_STABLE;
         // mod quentin: permet de fert un skip du fadin sur une scene.
         if (!_lpAnimIn->isCompleted())
            _lpAnimIn->move(800);
         break;
      case SCENE_SHOWIN:
         _State = SCENE_SHOWIN;
         if (!isVignette()) {
            _lpAnimIn->startCounter(1.0f, 0.0f, 0, SCENE_FADE_DURATION, K_COUNTER_EASEIN);         
         }
         else {
            _lpAnimIn->startCounter(0.0f, 1.0f, 0, SCENE_FADE_DURATION, K_COUNTER_EASEIN);         
         }
         break;
      case SCENE_SHOWOUT:
         _State = SCENE_SHOWOUT;
         if (!isVignette()) {
            _lpAnimOut->startCounter(0.0f, 1.0f, 0, SCENE_FADE_DURATION, K_COUNTER_EASEIN);
         }
         else {
            _lpAnimOut->startCounter(1.0f, 0.0f, 0, SCENE_FADE_DURATION, K_COUNTER_EASEIN);         
         }
         break;
      default:
         break;
   }
}

void EScene::Init()
{
}

bool EScene::MouseOver(long mousex, long mousey, float xx, float yy, int ww, int hh)
{
   
   // Le pointeur est il dans la zone proposée ?
   if ( (mousex >= xx-10 && mousex <= xx+ww+20) && (mousey >= yy-10 && mousey <= yy+hh+20) ) {
      return true;
   }
   else {
      return false;
   }
   
   return false;
}

void EScene::SetMouseOverInfo(const char *sz1, const char *sz2)
{
   memset(_szMouseOverInfo, 0, 256);
   KAssert(strlen(sz1) + strlen(sz2) < 256);

   strcpy(_szMouseOverInfo, sz1);

   // Récupère la version traduite de sz2
   // L'ID est composé de "mo_" suivi de sz2
   char szID[256];
   const char *szKstr;
   memset(szID, 0, 256);
   strcpy(szID, sz2);
   // Enlève le [x] si besoin (pour les objets composites
   if (strstr(szID, "[")) {
      strstr(szID, "[")[0] = 0x0;
   }
   // Essai sans préfixe "mo_"
   szKstr = KStr(szID);
   if (!szKstr) {
      // Essai avec préfixe "mo_"
      strcpy(szID, "mo_");
      strcat(szID, sz2);
   }
   szKstr = KStr(szID);

   if (szKstr) {
      strcat(_szMouseOverInfo, " ");
      strcat(_szMouseOverInfo, szKstr);
   }
}

void EScene::Check()
{
   bool bCurrentFound;
   EObjectContainer *o;
   EObjectContainer *oCurrent;
   float x,y;
   x = Mouse.x;
   y = Mouse.y;
   
   /* Récupère l'objet actuellement survollé */
   oCurrent = NULL;
   bCurrentFound = false;
   bool bGotNext = EObjectContainer::g_lpListObj.getTail (o);
   if (bGotNext) {
      while (bGotNext && !bCurrentFound) {
         switch (o->_type) {
            case SCENEOBJECT_IMAGE:
               if (strcmp(o->_lpObjectImage->_szSceneName, _strSceneName.c_str()) == 0 && !o->_lpObjectImage->isDisabled() && o->_lpObjectImage->isVisible())
               {
                  if (MouseOver((long)x, (long)y ,o->_lpObjectImage->getCurrentX(), o->_lpObjectImage->getCurrentY(), o->_lpObjectImage->_w, o->_lpObjectImage->_h) == true ) {
                     oCurrent = o;
                     bCurrentFound = true;
                  }
               }
               break;
         }
         // Suivant
         bGotNext = EObjectContainer::g_lpListObj.getPred (o);
      }
   }
   /* Mémorise l'objet actuellement survollé et exécute un traitement si il a changé */
   if (oCurrent && !_lpSceneDirector->isDialogMode() && !_lpSceneDirector->isCinematicMode()) {
      if (strcmp(_szCurrentObjectOver, oCurrent->_szObjectName) != 0) {
         SetMouseOverInfo("", "");
         /* Traitement par défaut */
         _lpSceneDirector->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
         /* L'image demande un curseur spécifique */
         if (oCurrent->_lpObjectImage->isMousePointer() && !_lpSceneDirector->getHoldItem()) {
            _lpSceneDirector->PrepareMouseCursor(oCurrent->_lpObjectImage->getMousePointer());
            if (strcmp(oCurrent->_lpObjectImage->getMousePointer(), "arrow_info") == 0) {
               if (_lpSceneDirector->isSystemCursor()) {
                  SetMouseOverInfo(KStr("MOUSEOVER_INFO"), "");
               }
//               SetMouseOverInfo(KStr("MOUSEOVER_INFO"), oCurrent->_szObjectName);
            }
            else if (strcmp(oCurrent->_lpObjectImage->getMousePointer(), "arrow_look") == 0) {
               if (_lpSceneDirector->isSystemCursor()) {
                  SetMouseOverInfo(KStr("MOUSEOVER_LOOK"), "");
               }
//               SetMouseOverInfo(KStr("MOUSEOVER_LOOK"), oCurrent->_szObjectName);
            }
            else if (strcmp(oCurrent->_lpObjectImage->getMousePointer(), "arrow_action") == 0) {
               if (_lpSceneDirector->isSystemCursor()) {
                  SetMouseOverInfo(KStr("MOUSEOVER_ACTION"), "");
               }
//               SetMouseOverInfo(KStr("MOUSEOVER_ACTION"), oCurrent->_szObjectName);
            }
            else if (strcmp(oCurrent->_lpObjectImage->getMousePointer(), "arrow_take") == 0) {
               if (_lpSceneDirector->isSystemCursor()) {
                  SetMouseOverInfo(KStr("MOUSEOVER_TAKE"), "");
               }
//               SetMouseOverInfo(KStr("MOUSEOVER_TAKE"), oCurrent->_szObjectName);
            }
            else if (strcmp(oCurrent->_lpObjectImage->getMousePointer(), "arrow_talk") == 0) {
               if (_lpSceneDirector->isSystemCursor()) {
                  SetMouseOverInfo(KStr("MOUSEOVER_TALK"), "");
               }
//               SetMouseOverInfo(KStr("MOUSEOVER_TALK"), oCurrent->_szObjectName);
            }
            else if (strcmp(oCurrent->_lpObjectImage->getMousePointer(), "arrow_puzzle") == 0) {
//               SetMouseOverInfo(KStr("MOUSEOVER_PUZZLE"), oCurrent->_szObjectName);
               SetMouseOverInfo(KStr("MOUSEOVER_PUZZLE"), "");
            }
         }
         // On tient un objet ?
         if (_lpSceneDirector->getHoldItem()) {
//            SetMouseOverInfo(KStr("MOUSEOVER_USE"), _lpSceneDirector->getHoldItem()->_szItemName);
         }
         // La scène peut faire un traitement particulier
         if (ObjectOver(oCurrent->_szObjectName, x, y) == false) {
            if (oCurrent->_lpObjectImage->isExit()) {
               if (oCurrent->_lpObjectImage->isExitDoor()) {
                  _lpSceneDirector->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_DOOR);
                  SetMouseOverInfo(KStr("MOUSEOVER_GOTO"), oCurrent->_lpObjectImage->getDestination());
               }
               else if (oCurrent->_lpObjectImage->isExitWalkTo()) {
                  _lpSceneDirector->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_WALK);
                  SetMouseOverInfo(KStr("MOUSEOVER_WALK"), oCurrent->_lpObjectImage->getDestination());
               }
               else if (oCurrent->_lpObjectImage->isExitWalkBack()) {
                  _lpSceneDirector->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_WALKBACK);
                  SetMouseOverInfo(KStr("MOUSEOVER_BACK"), oCurrent->_lpObjectImage->getDestination());
               }
               else {
                  _lpSceneDirector->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_LOOK);
               }
            }
         }
      }
      strcpy(_szCurrentObjectOver, oCurrent->_szObjectName);
   }

   // Clic en dehors
   if (!oCurrent && Mouse.LeftEvent())
   {
      if(_lpSceneDirector->GetCurrentVignette() && !_lpSceneDirector->GetCurrentMinigame())
      {
         _lpSceneDirector->CloseVignette();
         Mouse.EventClose();
      }
   }
   
   if (oCurrent && Mouse.LeftEvent())
   {
      // Traitement du clic sur l'objet courant
      switch (oCurrent->_type) {
         case SCENEOBJECT_IMAGE:
            if (strcmp(oCurrent->_lpObjectImage->_szSceneName, _strSceneName.c_str()) == 0 && oCurrent->_lpObjectImage->isVisible())
            {
               if (MouseOver((long)x, (long)y ,oCurrent->_lpObjectImage->getCurrentX(), oCurrent->_lpObjectImage->getCurrentY(), oCurrent->_lpObjectImage->_w, oCurrent->_lpObjectImage->_h) == true ) {
                  /* 1er cas, on n'avait pas d'objet dans la main */
                  if (_lpSceneDirector->getHoldItem() == NULL) {
                     K_LOG("ObjectClicked(%s,%f,%f);",oCurrent->_szObjectName, x, y);
                     if (ObjectClicked(oCurrent->_szObjectName, x, y) == false) {
                        // C'est une sortie
                        if (oCurrent->_lpObjectImage->isExit()) {
                           Mouse.LeftEventClose();
                           if (oCurrent->_lpObjectImage->isExitDoor()) {
                              _lpSceneDirector->PlayDirectorSound("opendoor");
                           }
                           if (oCurrent->_lpObjectImage->isExitWalkBack() == false) {
                              _lpSceneDirector->setZoomin(true);
                           } else {
                              _lpSceneDirector->setZoomin(false);
                           }
                           _lpSceneDirector->setWalk();
                           _lpSceneDirector->GoToScene(oCurrent->_lpObjectImage->getDestination());
                        }
                        // Il y a une info à afficher
                        if (oCurrent->_lpObjectImage->isInfoText()) {
                           if (strlen(oCurrent->_lpObjectImage->getInfoText()) > 0) {
                              if (KGame::getGame()->getString (oCurrent->_lpObjectImage->getInfoText())) {
                                 _lpSceneDirector->getSequencer()->VoiceOver("infotext", KStr(oCurrent->_lpObjectImage->getInfoText()),"");
                              }
                           }
                        }
                     }
                  }
                  else {
                     K_LOG("ItemUsed(%s,%s);",_lpSceneDirector->getHoldItem()->_szItemName, oCurrent->_szObjectName);
                     if (!ItemUsed(_lpSceneDirector->getHoldItem()->_szItemName, oCurrent->_szObjectName))
                     {
                        _lpSceneDirector->NiceTryVoiceOver();
                     }
                     // La scène a précédemnt indiqué qu'elle souhaiter garder l'item à la main après utilisation
                     if (_bKeepHoldItem == false) {
                        _lpSceneDirector->resetHoldItem();
                     }
                     else {
                        _bKeepHoldItem = false;
                     }
                     //Mouse.LeftEventClose(); // Désactivé le 03/12/10 car empéchait le séquenceur de recevoir les clics ensuite
                  }
                  bCurrentFound = true;
               }
            }
            break;
      }
   }
}

void EScene::Logic()
{
   EObjectContainer *o;   
   
   bool bGotNext = EObjectContainer::g_lpListObj.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         // Traitement associé à l'objet de la scène
         switch (o->_type) {
            case SCENEOBJECT_UNKNOW:
               // Error!
               break;
            case SCENEOBJECT_IMAGE:
               if (strcmp(o->_lpObjectImage->_szSceneName, _strSceneName.c_str()) == 0)
               {
                  o->_lpObjectImage->Logic();
               }
               break;
            case SCENEOBJECT_PARTICLEEMITTER:
               if (strcmp(o->_lpObjectEmitter->_szSceneName, _strSceneName.c_str()) == 0)
               {
                  o->_lpObjectEmitter->Logic();
               }
               break;
            case SCENEOBJECT_TEXT:
               if (strcmp(o->_lpObjectText->_szSceneName, _strSceneName.c_str()) == 0)
               {
                  o->_lpObjectText->Logic();
               }
               break;
            case SCENEOBJECT_ANIMATION:
               if (strcmp(o->_lpObjectAnime->_szSceneName, _strSceneName.c_str()) == 0)
               {
                  if (o->_lpObjectAnime->_bLoop == true)	// Animation avec boucle, on redémarre si le KCounter IsCompleted = true
                  {
                     SceneObjectImage *objimg;
                     char szTmpString[256];
                     objimg = EScene::GetObjectImageByName(o->_lpObjectAnime->_strTarget.c_str());
                     if (objimg != NULL) {
                        if (objimg->GetSmartImage()->isCompleted() == true)
                        {
                           memset (szTmpString, 0, 256);
                           strcpy(szTmpString,o->_lpObjectAnime->_strObjName.c_str());
                           EScene::StartAnimation(szTmpString);
                        }
                     }
                  }
               }
               //o->_lpObjectAnime->Logic();
               break;
         }
         // Suivant
         bGotNext = EObjectContainer::g_lpListObj.getNext (o);
      }
   }   
}

void EScene::Draw()
{
   double fElapsed = MyGame::getGame()->getKWindow()->getFrameTime();
   switch (_State) {
      case SCENE_STABLE:
         break;
      case SCENE_SHOWIN:
         if (!_lpAnimIn->isCompleted()) _lpAnimIn->move(fElapsed);
         if (_lpAnimIn->isCompleted())
         {
            _State = SCENE_STABLE;
            // La scène vient de finir de s'afficher, on vérifie que le personnage affiché
            // correspond à l'univers de la scène, sinon on le change
            _lpSceneDirector->ChangeCharacter(_strSceneUniverseName.c_str());
         }
         break;
      case SCENE_SHOWOUT:
         if (!_lpAnimOut->isCompleted()) _lpAnimOut->move(fElapsed);
         if (_lpAnimOut->isCompleted())
         {
            // RAZ du zoom in
            _lpSceneDirector->setZoomin(false);
         }
         break;
      default:
         break;
   }   
   
   // En mode vignette on fait des effets
   if (isVignette()) {
//      _lpGraphicTarget->clearRenderTarget();
//      _lpGraphicTarget->beginRenderToTarget();
      if (_State == SCENE_SHOWIN) {
//         KGame::getGame()->_lpKWindow->setWorldView(0,0,0,1.0f-_lpAnimIn->getCurrentValue(),false);
      }
      if (_State == SCENE_SHOWOUT) {
//         KGame::getGame()->_lpKWindow->setWorldView(0,0,0,_lpAnimOut->getCurrentValue(),false);
      }
   }

   EObjectContainer *o;
   bool bGotNext = EObjectContainer::g_lpListObj.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (o->_strSceneName == _strSceneName) {
            // Traitement associé à l'objet de la scène
            switch (o->_type) {
               case SCENEOBJECT_UNKNOW:
                  // Error!
                  break;
               case SCENEOBJECT_IMAGE:
                  o->_lpObjectImage->Draw();
                  // On affiche un gabarit pour repérer les zones et images
#ifdef DEBUG
                  if (Mouse.MiddleState()) {
                     _lpFont->setColor(.5, .5, .5, 1);
                     _lpFont->drawStringFromLeft(o->_szObjectName, (long)o->_lpObjectImage->_x, (long)o->_lpObjectImage->_y);
                     if (o->_lpObjectImage->GetSmartImage()) {
                        // Pas le background
                        if (o->_lpObjectImage->GetSmartImage()->getWidth() < 1024) {
                           _lpBgGraphic2->drawRect(o->_lpObjectImage->GetSmartImage()->getCurrentX(), o->_lpObjectImage->GetSmartImage()->getCurrentY(), o->_lpObjectImage->GetSmartImage()->getCurrentX()+o->_lpObjectImage->GetSmartImage()->getWidth(), o->_lpObjectImage->GetSmartImage()->getCurrentY()+o->_lpObjectImage->GetSmartImage()->getHeight(), 0, 0, 0, .4f);
                        }
                     } else {
                        // Cadre autour de la zone
                        _lpBgGraphic2->drawLine(o->_lpObjectImage->_x, o->_lpObjectImage->_y, o->_lpObjectImage->_x+o->_lpObjectImage->_w, o->_lpObjectImage->_y, 0, 0, 0, 1);
                        _lpBgGraphic2->drawLine(o->_lpObjectImage->_x, o->_lpObjectImage->_y, o->_lpObjectImage->_x, o->_lpObjectImage->_y+o->_lpObjectImage->_h, 0, 0, 0, 1);
                        _lpBgGraphic2->drawLine(o->_lpObjectImage->_x+o->_lpObjectImage->_w, o->_lpObjectImage->_y, o->_lpObjectImage->_x+o->_lpObjectImage->_w, o->_lpObjectImage->_y+o->_lpObjectImage->_h, 0, 0, 0, 1);
                        _lpBgGraphic2->drawLine(o->_lpObjectImage->_x, o->_lpObjectImage->_y+o->_lpObjectImage->_h, o->_lpObjectImage->_x+o->_lpObjectImage->_w, o->_lpObjectImage->_y+o->_lpObjectImage->_h, 0, 0, 0, 1);
                     }
                  }
#endif
                  break;
               case SCENEOBJECT_PARTICLEEMITTER:
                  o->_lpObjectEmitter->Draw();
                  break;
               case SCENEOBJECT_ANIMATION:
                  break;
               case SCENEOBJECT_TEXT:
                  if (o->_lpObjectText->_bVisible) {
                     o->_lpObjectText->Draw();
                  }
                  break;
            }
         }
         // Suivant
         bGotNext = EObjectContainer::g_lpListObj.getNext (o);
      }
   } 
     
#ifdef ENIGMA_STATS
   // Debug info
   if (_lpSceneDirector->_bDebugDisplay) {
      char sDebugString[256];
      sprintf(sDebugString, "%s - %s (c) Casual Box 2010", getDebugString(), MyGame::getGame()->getGameVersion());
      DrawFancyString(sDebugString, _lpFont, 10, 30, 1024, false, 1);
   }
#endif

   if (!isVignette())
   {
      DrawFadeOverlay();
   }
   else {
//      _lpGraphicTarget->endRenderToTarget();
      float x = 0;
      float y = 0;
      if (!_lpSceneDirector->isEarthQuakeCompleted()) {
         x = (((5 - (-5))+1)*(KRandom::getRandomFloat()) + (-5)) * 1;
         y = (((5 - (-5))+1)*(KRandom::getRandomFloat()) + (-5)) * 1;
      }
      if (_State == SCENE_SHOWIN) {
//         _lpGraphicTarget->blitAlphaRectFxF(0.0f, 0.0f, 1024.0f, 768.0f, x, y, 0, 1, 1.0f-_lpAnimIn->getCurrentValue());
      }
      if (_State == SCENE_STABLE) {
//         _lpGraphicTarget->blitAlphaRectF(0.0f, 0.0f, 1024.0f, 768.0f, x, y);
      }
      if (_State == SCENE_SHOWOUT) {
//         _lpGraphicTarget->blitAlphaRectFx(0.0f, 0.0f, 1024.0f, 768.0f, x, y, 0, 1, _lpAnimOut->getCurrentValue());
      }
//      KGame::getGame()->_lpKWindow->setWorldView(0,0,0,1,false);
   }

}

void EScene::DrawFadeOverlay()
{
   /* Fadein/out : affichage d'un rectangle de masquage */
   switch (_State) {
      case SCENE_SHOWOUT:
         _lpBgGraphic2->drawRect(-1, -1, 1024+1, 768+1, 0, 0, 0, _lpAnimOut->getCurrentValue());
         break;
      case SCENE_SHOWIN:
         _lpBgGraphic2->drawRect(-1, -1, 1024+1, 768+1, 0, 0, 0, _lpAnimIn->getCurrentValue());
         break;
      default:
         break;
   }
}

void EScene::Close()
{
}

bool EScene::AskForClose()
{
   return true;
}

bool EScene::ObjectClicked (const char *szObjectName, float x, float y)
{
   // A implémenter dans les classes dérivées pour être informé qu'un objet visible a été cliqué
   return false;
}

bool EScene::ObjectOver (char *szObjectName, float x, float y)
{
   // A implémenter dans les classes dérivées pour être informé qu'un objet visible est survolé
   return false;
}

/**
 * Display missing file error
 *
 * \param lpszFileName name of missing file
 */

void EScene::showFileError (const char *lpszFileName) {
   static char szFileMsg[K_MAXPATH];
   
   KStringFmt (szFileMsg, K_MAXPATH, KStr ("ERR_BODY"), lpszFileName);
   KMiscTools::messageBox (KStr ("ERR_TITLE"), szFileMsg);
}

/**
 * Anime un objet frame par frame (bStart=true) ou le stoppe (bStart=false)
 * Le type de l'animation est décrit dans le fichier scene
 * \param bStart: boolean 
 */
void SceneObjectImage::StartAnimationByFrame(bool bStart)
{
	_lpSmartImage->AnimateByFrame(bStart);
}
/**
 * Démarre ou stoppe la rotation d'une image (bRotate)
 * Donne aussi le sens de rotation de l'animation (récupéré de l'objet Anime)
 * \param bStart: boolean 
 */
void SceneObjectImage::StartRotationStatus(bool bRotate, int nDirection)
{
	_lpSmartImage->Rotate(bRotate, nDirection);
}

void EScene::GetRotationStatus(char *szObjectName)
{
   
}

void EScene::GetRotationAngle(char *szObjectName)
{
   
}

void EScene::GetAnimationP2PStatus(char *szObjectName)
{
   
}

/**
 * Set visibility of scene objects
 *
 * \param szObjectName: name of scene object, bVis: boolean 
 */
void EScene::SetVisible(const char *szObjectName, bool bVis, bool bForce)
{
   K_LOG("SetVisible(%s,%d,%d);", szObjectName, bVis, bForce);
	SceneObjectImage *obj;
	obj = EScene::GetObjectImageByName(szObjectName);
	if (obj != NULL)
	{
		obj->SetVisible(bVis);
      if (bForce) {
         if (obj->GetSmartImage()) {
            obj->GetSmartImage()->ForceVisible(bVis);
         }
      }
	}
   else {
      SceneObjectText *objtxt;
      objtxt = EScene::GetObjectTextByName(szObjectName);
      if (objtxt != NULL)
      {
         objtxt->SetVisible(bVis);
      }
      //KAssert(objtxt != NULL);
   }
}

void EScene::SetPosition(const char *szObjectName, float fX, float fY)
{
	SceneObjectImage *obj;
	obj = EScene::GetObjectImageByName(szObjectName);
   KAssert(obj!=NULL);
	if (obj != NULL)
	{
		obj->SetPosition(fX, fY);
	}
}

void EScene::RestorePosition(const char *szObjectName)
{
	SceneObjectImage *obj;
	obj = EScene::GetObjectImageByName(szObjectName);
   KAssert(obj!=NULL);
	if (obj != NULL)
	{
		obj->RestorePosition();
	}
}

void EScene::MovePosition(const char *szObjectName, float fX, float fY)
{
	SceneObjectImage *obj;
   int x,y;
	obj = EScene::GetObjectImageByName(szObjectName);
   KAssert(obj!=NULL);
	if (obj != NULL)
	{
      GetObjectPosition(szObjectName, x, y, false, false);
      obj->SetPosition((float)x + fX, (float)y + fY);
	}
}

bool EScene::isVisible(const char *szObjectName)
{
	SceneObjectImage *obj;
	obj = EScene::GetObjectImageByName(szObjectName);
	if (obj != NULL)
	{
		return obj->isVisible();
	}
   else {
      return false;
   }

}

void EScene::SetSmoothOverlap(char *szObjectName, bool bVis)
{
	SceneObjectImage *obj;
	obj = EScene::GetObjectImageByName(szObjectName);
	if (obj != NULL)
	{
		obj->SetSmoothOverlap(bVis);
	}
}

/**
 * Gestion des Emitters - Démarrage d'un emitter
 *
 * \param szObjectName: nom de l'emitter 
 */
void EScene::AddEmitter(const char *szObjectName)
{
}

void EScene::StartEmitter(const char *szObjectName)
{
   K_LOG("StartEmitter(%s);",szObjectName);
	SceneObjectParticleEmitter *objemit;
	objemit = EScene::GetObjectEmitterByName(szObjectName);
	if (objemit != NULL)
	{
		objemit->Start();
	}	
}

/**
 * Gestion des Emitters - Arrêt d'un emitter
 *
 * \param szObjectName: nom de l'emitter 
 */
void EScene::StopEmitter(const char *szObjectName)
{
   K_LOG("StopEmitter(%s);",szObjectName);
	SceneObjectParticleEmitter *objemit;
	objemit = EScene::GetObjectEmitterByName(szObjectName);
	if (objemit != NULL)
	{
      objemit->Stop();
	}	
}

/**
 * Gestion des Emitters - Déplacement d'un emitter
 *
 * \param szObjectName: nom de l'emitter, x,y: coordonnées du nouvel emplacement
 */
void EScene::MoveEmitter(const char *szObjectName, float x, float y)
{
	SceneObjectParticleEmitter *objemit;
	objemit = EScene::GetObjectEmitterByName(szObjectName);
	if (objemit != NULL)
	{
	}	
}

/**
 * Gestion des Animations - Démarrage d'une animation
 *
 * \param szAnime: nom de l'animation 
 */
void EScene::StartAnimation(const char *szAnime)
{
   K_LOG("StartAnimation(%s);",szAnime);
	SceneObjectAnimation *objanim;
	float fRandom;
	objanim = EScene::GetObjectAnimationByName(szAnime);
	if (objanim != NULL)
	{
		SceneObjectImage *objimg;
		objimg = EScene::GetObjectImageByName(objanim->_strTarget.c_str());
      if (objimg == NULL) {
         K_LOG("StartAnimation(%s) Failed Image Not Found;",szAnime);
         return;
      }
		if (strstr(objanim->_strType.c_str(), ANIM_FRAME)!= NULL)
			objimg->StartAnimationByFrame(true);
		else if (strstr(objanim->_strType.c_str(), ANIM_P2P)!= NULL)
		{
			//			objimg->GetSmartImage()->AnimateByFrame(false);
			objimg->Move(objanim->_fx, objanim->_fy);
			fRandom = (((objanim->_iDelayMax - objanim->_iDelayMin)+1)*(KRandom::getRandomFloat())+ objanim->_iDelayMin);
			switch (objanim->_iCounter)
		 {
			case 0:
				objimg->GetSmartImage()->AnimateP2P(objanim->_fx2, objanim->_fy2,objanim->_fDur, objanim->_bBounce, K_COUNTER_LINEAR, fRandom);
				break;
			case 1:
				objimg->GetSmartImage()->AnimateP2P(objanim->_fx2, objanim->_fy2,objanim->_fDur, objanim->_bBounce, K_COUNTER_EASEIN, fRandom);
				break;
			case 2:
				objimg->GetSmartImage()->AnimateP2P(objanim->_fx2, objanim->_fy2,objanim->_fDur, objanim->_bBounce, K_COUNTER_EASEOUT, fRandom);
				break;
			case 3:
				objimg->GetSmartImage()->AnimateP2P(objanim->_fx2, objanim->_fy2,objanim->_fDur, objanim->_bBounce, K_COUNTER_EASEINOUT, fRandom);
				break;
			}

		}
		else if (strstr(objanim->_strType.c_str(), ANIM_ROTATE)!= NULL)
		{
			switch (objanim->_iCounter)
			{
			case 0:
				objimg->GetSmartImage()->SetRotation((float)objanim->_iAng1, (float)objanim->_iAng2,objanim->_fDur,objanim->_bBounce,K_COUNTER_LINEAR);
				break;
			case 1:
				objimg->GetSmartImage()->SetRotation((float)objanim->_iAng1, (float)objanim->_iAng2,objanim->_fDur,objanim->_bBounce,K_COUNTER_EASEIN);
				break;
			case 2:
				objimg->GetSmartImage()->SetRotation((float)objanim->_iAng1, (float)objanim->_iAng2,objanim->_fDur,objanim->_bBounce,K_COUNTER_EASEOUT);
				break;
			case 3:
				objimg->GetSmartImage()->SetRotation((float)objanim->_iAng1, (float)objanim->_iAng2,objanim->_fDur,objanim->_bBounce,K_COUNTER_EASEINOUT);
				break;
			}

			objimg->StartRotationStatus(true,objanim->nDir);
		}
		else if (strstr(objanim->_strType.c_str(), ANIM_VIDEO)!= NULL)
		{
			if (objimg) {
				objimg->GetSmartImage()->StartVideo();
			}
		}
		else if (strstr(objanim->_strType.c_str(), ANIM_MARQUEE)!=NULL) 
		{
			objimg->GetSmartImage()->AnimateMarquee(objanim->_fDur,objanim->_iDir);
		}
	}
}

/**
 * Gestion des Animations - Arrêt d'une animation
 *
 * \param szAnime: nom de l'animation 
 */
void EScene::StopAnimation(const char *szAnime)
{
   K_LOG("StopAnimation(%d);",szAnime);
	SceneObjectAnimation *objanim;
	objanim = EScene::GetObjectAnimationByName(szAnime);
	if (objanim != NULL)
	{
		SceneObjectImage *objimg;
		objimg = EScene::GetObjectImageByName(objanim->_strTarget.c_str());
		if (strstr(objanim->_strType.c_str(), ANIM_FRAME)!= NULL)
			objimg->StartAnimationByFrame(false);
		else if (strstr(objanim->_strType.c_str(), ANIM_P2P)!= NULL)
			objimg->StartAnimationByFrame(false);
		else if (strstr(objanim->_strType.c_str(), ANIM_ROTATE)!= NULL)
		{
			objimg->StartRotationStatus(false,objanim->nDir);
		}
		else if (strstr(objanim->_strType.c_str(), ANIM_VIDEO)!= NULL)
		{
			if (objimg) {
				objimg->GetSmartImage()->StopVideo();
			}
		}
		else if (strstr(objanim->_strType.c_str(), ANIM_MARQUEE)!= NULL)
			objimg->GetSmartImage()->StopMarquee();

	}
}

void EScene::PlayWalkSound()
{
   if (strlen(_szWalkSound)>0 && strcmp(_szWalkSound, "none") != 0) {
      KSound *sound = ESoundBank::getSound(_szWalkSound);
      if (sound) {
         sound->playSample();
      }
   }
}

bool EScene::GetStringToken (const char *szLine, const char *szTokenName, std::string& strOutput)
{
	const char *first_of, *last_of;
	char szToken[256], szResult[256];
   
	memset (szToken, 0, 256);
	memset (szResult, 0, 256);

   // Construit une chaine contenant le nom du token + ":"
	strcpy (szToken, szTokenName);
	strcat (szToken, ":");
   
   // Recherche le token dans la chaine
	first_of = strstr(szLine, szToken);
	if (first_of != NULL)
	{
      // On se place juste après les ":"
		first_of += strlen (szToken);
      
      // On recherche la fin de la valeur
      // Soit le pipe suivant
		last_of = strstr(first_of, "|");
      // Soit la fin de la ligne
		if (last_of == NULL)
			last_of = strstr(first_of, 0);
      // On n'a pas trouvé de fin de ligne ? C'est la merde...
		if (last_of == NULL)
      {
         KAssert(last_of != NULL);
         return false;
      }

      // On récupère la valeur dans une chaine temporaire
      if (last_of - first_of < 256) {
         strncpy(szResult, first_of, last_of - first_of);
         // On cloture proprement la chaine par un caract 0
         szResult[last_of-first_of]='\0';
      } else {
         K_LOG("GetStringToken overflow");
         return false;
      }
      
      // On transfère le résultat dans la chaine de retour
		strOutput = szResult;
      // On trim à gauche et à droite
		strOutput.erase(strOutput.find_last_not_of(" ") + 1);	//RTrim
		strOutput.erase(0, strOutput.find_first_not_of(" "));	//LTrim
         
		return true;
	}
	else
	{
		strOutput.clear();
		return false;
	}
}

int EScene::GetIntToken(const char *szLine, char *szTokenName)
{
   std::string str;
	if ( GetStringToken (szLine, szTokenName, str) ) {
      return atoi(str.c_str());
   }
   else {
      return 0;
   }
}

float EScene::GetFloatToken(const char *szLine, char *szTokenName, float fDefault)
{
   std::string str;
	if ( GetStringToken (szLine, szTokenName, str) ) {
      return (float)atof(str.c_str());
   }
   else {
      return fDefault;
   }
}

bool EScene::GetBoolToken(const char *szLine, const char *szTokenName, bool bDefault)
{
   std::string str;
	if ( GetStringToken (szLine, szTokenName, str) ) {
      if (str == "true")
         return true;
      else if (str == "false")
         return false;
      else
         return bDefault;
   }
   else {
      return bDefault;
   }
}

/**
 * Parse Scene file and load data in a KList object
 *
 * \param lpszFileName: name of scene file
 */
void EScene::parseSceneFile (const char *lpszFileName)
{
   bool bSuccess;
   // Pour la scène
   std::string strWalkSound="", strUniverse="none", strSoundDesign="";
   // Pour les élements de la scène
	std::string strLine = "", strFileName = "", strInvImage = "", strType = "", strTarget = "", strObjectName = "", 
      strVisible = "", strLoop = "", strFade = "", strTypeObject = "", strMouse = "", strState = "",
      strCounterFunction = "", strDestination = "", strMousePointer="", strInfoText = "",
      strPresetFile = "", strAnimationFile = "", strBaseDir = "";
	float fx=0.0f, fx2=0.0f;
	float fy=0.0f, fy2=0.0f;
   float fScale = 0.0f;
	float fDuration = 0.0, fHaloSpeed = 0.0, fMaxBlend = 1.0;
	bool bVisible=false, bDisable=false, bLoop=false, bFade=false, bEmitState=false, bBounce=false, bExit=false, bExitWalkTo=false, bExitWalkBack=false, bExitDoor=false, bHalo=false, bGlitch = false;
	int id=0, iFade=0, iFrame=0, iAngle1=0, iAngle2=0, iDir=0, iCounterFunction=0, iWidth=0, iHeight=0, iDelayMin=0, iDelayMax=0;
	EObjectContainer *lpObjContainer;
   bool bSceneSection = false;

   K_LOG("parseSceneFile(%s)", lpszFileName);
//   _lpSceneDirector->setLoading(true);
   ESceneParser::RTextFile scene_file(lpszFileName);

   // Valeurs par défaut
   _strSceneUniverseName = strUniverse;
   
   if (scene_file.IsOpen() ) // ce test échoue si le fichier n'est pas ouvert
   {
      // cette boucle s'arrête dès qu'une erreur de lecture survient
      while (scene_file.GetLine(strLine))
      {
         /* On traite les 3 types et les exceptions
			 * On identifie le type en recherchant la string "object" dans la ligne (strLine.c_str()): 
			 * => "image" (SCENEOBJECT_IMAGE) 
			 * => "emitter" (SCENEOBJECT_PARTICLEEMITTER)
			 * => "anime" (SCENEOBJECT_ANIME)
			 * => "imgzone" (SCENEOBJECT_IMAGEZONE) Zone clickable
          */
         K_LOG("Parse %s",strLine.c_str());
			bSuccess = GetStringToken (strLine.c_str(), "object", strTypeObject);
         
         /****************************************************************/
         /* ARGUMENTS POUR LA SCENE                                      */
         /****************************************************************/
         if (strTypeObject=="scene")
			{
            bSceneSection = true;
            bSuccess = GetStringToken (strLine.c_str(), "walksound", strWalkSound);
            if(bSuccess == true)
            {
               strcpy(_szWalkSound, strWalkSound.c_str());
            }
            else {
               strcpy(_szWalkSound, "");
            }
            bSuccess = GetStringToken (strLine.c_str(), "sounddesign", strSoundDesign);
            if(bSuccess == true)
            {
               strcpy(_szSoundDesign, strSoundDesign.c_str());
               _lpSoundDesign = EMusicBank::getMusic(_szSoundDesign);
            }
            else {
               strcpy(_szSoundDesign, "");
            }
            
            bSuccess = GetStringToken (strLine.c_str(), "universe", strUniverse);
            if (!bSuccess) {
               // Par défaut, la maison
               strUniverse = "none";
            }
            _strSceneUniverseName = strUniverse;
            this->_lpSceneDirector->getMap()->setUniversVisited(_strSceneUniverseName.c_str());
         }
         /****************************************************************/
         /* IMAGE                                                        */
         /****************************************************************/
			else if (strTypeObject=="image" || strTypeObject=="zone")
			{
            if (strTypeObject == "image")
            {
               bSuccess = GetStringToken (strLine.c_str(), FILENAME, strFileName);	// Traitement du champ "file"
               KAssert (bSuccess == true);
               bSuccess = GetStringToken (strLine.c_str(), ANIMTYPE, strType);		// Traitement du champ type de mouvement ("type" dans le fichier)
               KAssert (bSuccess == true);
               bSuccess = GetStringToken (strLine.c_str(), INVIMAGE, strInvImage);
               bSuccess = GetStringToken (strLine.c_str(), BASEDIR, strBaseDir);
            }
            else {
               strFileName = "";
               strType = "";
               strInvImage = "";
            }

            bSuccess = GetStringToken (strLine.c_str(), OBJECTNAME, strObjectName);	// Traitement de l'ObjectName
            KAssert (bSuccess == true);
            
            fx = GetFloatToken (strLine.c_str(), COORDX);				// Traitement des coordonnées x,y
            fy = GetFloatToken (strLine.c_str(), COORDY);
            
            iWidth = GetIntToken (strLine.c_str(), WIDTH);				// Traitement des données Largeur/Hauteur de l'image
            iHeight = GetIntToken (strLine.c_str(), HEIGHT);
            
            bVisible = GetBoolToken (strLine.c_str(), VISIBLE, true);		// Traitement du champ Visible
            bDisable = GetBoolToken (strLine.c_str(), "disable", false);
            bLoop = GetBoolToken (strLine.c_str(), LOOP, false);			// Traitement du champ "loop"
            bGlitch = GetBoolToken (strLine.c_str(), "glitch", true);			// Traitement du champ "glitch", true par défaut
            iFrame = GetIntToken (strLine.c_str(), NBFRAME);			// Traitement du champ nbFrame
            if (iFrame == 0) iFrame = 1;
            bFade = GetBoolToken (strLine.c_str(), BFADE, false);			// Traitement du champ bFade
            iFade = GetIntToken (strLine.c_str(), NFADE);				// Traitement du champ nFade
            bHalo = GetBoolToken (strLine.c_str(), "halo", false);
            fHaloSpeed = GetFloatToken(strLine.c_str(), "halospeed");
            fMaxBlend = GetFloatToken(strLine.c_str(), FMAXBLEND,1.0);

            /* Traitement par défaut */
            bExit = GetBoolToken (strLine.c_str(), "exit", false);
            bExitDoor = GetBoolToken (strLine.c_str(), "exitdoor", false);
            bExitWalkTo = GetBoolToken (strLine.c_str(), "exitwalkto", false);
            bExitWalkBack = GetBoolToken (strLine.c_str(), "exitwalkback", false);
            if (bExit)
            {
               bSuccess = GetStringToken (strLine.c_str(), "destination", strDestination);
               KAssert (bSuccess == true);
            }
            bSuccess = GetStringToken (strLine.c_str(), "mousepointer", strMousePointer);
            if (!bSuccess) {
               strMousePointer = "";
            }
            
            // Infos par défaut (uniquement pour les curseurs arrow_info)
            strInfoText = "";
//            if (strMousePointer == "arrow_info" || strMousePointer == "arrow_puzzle")
            {
               bSuccess = GetStringToken (strLine.c_str(), "infotext", strInfoText);
               if (!bSuccess) {
                  strInfoText = "";
               }
            }
            
            // Données traitées, chargement dans la KList
            lpObjContainer = new EObjectContainer(_strSceneName, strObjectName);
            lpObjContainer->_type = SCENEOBJECT_IMAGE;
            SceneObjectImage *objimg;
            objimg = new SceneObjectImage(_strSceneName.c_str());
            char szFileName[1024];
            memset(szFileName, 0, 1024);
            strcpy(szFileName, strFileName.c_str());
            char szInvImageName[K_MAXPATH];
            memset(szInvImageName, 0, K_MAXPATH);
            strcpy(szInvImageName, strInvImage.c_str());
            char szObjectName[256];
            memset(szObjectName, 0, 256);
            strcpy(szObjectName, strObjectName.c_str());
            char szType[256];
            memset(szType, 0, 256);
            strcpy(szType, strType.c_str());
            const char *szBaseDir= NULL;
            if (strBaseDir.size() > 0) {
               szBaseDir = strBaseDir.c_str();
            }
            objimg->EnableGlitch(bGlitch);
            objimg->SetData(szFileName, id, szObjectName, szType, iFrame, bFade, iFade ,fx, fy, iWidth, iHeight, bVisible, bDisable, bHalo, fHaloSpeed, bLoop, fMaxBlend, szInvImageName, szBaseDir);
            objimg->SetMousePointer(strMousePointer.c_str());
            if (bExit)
            {
               objimg->SetDestination(strDestination.c_str(), bExitDoor, bExitWalkTo, bExitWalkBack);
            }
            objimg->SetInfoText(strInfoText.c_str());
            objimg->EnableGlitch(bGlitch);
            lpObjContainer->_lpObjectImage = objimg;
            EObjectContainer::g_lpListObj.addToTail(lpObjContainer);
         }
         /****************************************************************/
         /* PARTICLE EMITTER                                             */
         /****************************************************************/
         else if (strTypeObject == "emitter")
         {
            bSuccess = GetStringToken (strLine.c_str(), OBJECTNAME, strObjectName);	// Traitement de l'ObjectName
            KAssert(bSuccess == true);

            bSuccess = GetStringToken (strLine.c_str(), "type", strType);
            KAssert(bSuccess == true);
            
            // Si c'est un creepingsmoke on lit les 2 fichiers de param
            if (strType == "creepingsmoke")
            {
               fScale = GetFloatToken (strLine.c_str(), "scale");
               bSuccess = GetStringToken (strLine.c_str(), "presetfile", strPresetFile);
               KAssert(bSuccess == true);
               bSuccess = GetStringToken (strLine.c_str(), "animationfile", strAnimationFile);
               if (!bSuccess) {
                  strAnimationFile = "";
               }
            }
            
            fx = GetFloatToken (strLine.c_str(), COORDX);
            fy = GetFloatToken (strLine.c_str(), COORDY);
            iWidth = GetIntToken (strLine.c_str(), WIDTH);
            iHeight = GetIntToken (strLine.c_str(), HEIGHT);
            
            bEmitState = GetBoolToken (strLine.c_str(), "start", true);			// Traitement du champ State (emitter démarré ou pas)
            bLoop = GetBoolToken (strLine.c_str(),"loop", false);				// Traitement du champ "loop"
            
            // Données traitées, chargement dans la KList
            lpObjContainer = new EObjectContainer(_strSceneName, strObjectName);
            lpObjContainer->_type = SCENEOBJECT_PARTICLEEMITTER;
            SceneObjectParticleEmitter *objemit;
            objemit = new SceneObjectParticleEmitter(_strSceneName.c_str());
            objemit->SetData(strObjectName.c_str(), strType.c_str(), fx, fy, iWidth, iHeight, bEmitState, bLoop); 
            if (strType == "creepingsmoke")
            {
               objemit->SetCreeping(strPresetFile.c_str(), strAnimationFile.c_str(),fx,fy,fScale);
            }
            lpObjContainer->_lpObjectEmitter = objemit;
            EObjectContainer::g_lpListObj.addToTail(lpObjContainer);
         }
         /****************************************************************/
         /* TEXT                                                         */
         /****************************************************************/
         else if (strTypeObject == "text")
         {
            // Nom de l'objet
            bSuccess = GetStringToken (strLine.c_str(), OBJECTNAME, strObjectName);
            KAssert(bSuccess == true);
            // Texte
            std::string strTextID = "";
            bSuccess = GetStringToken (strLine.c_str(), "textid", strTextID);
            KAssert(bSuccess == true);
            // Texte
            std::string strFontName = "";
            bSuccess = GetStringToken (strLine.c_str(), "fontname", strFontName);
            KAssert(bSuccess == true);
            // Font size
            int nFontSize = GetIntToken (strLine.c_str(), "fontsize");
            // Style
            std::string strStyle = "";
            int nStyle = TEXTSTYLE_CENTER;
            bSuccess = GetStringToken (strLine.c_str(), "style", strStyle);
            if (bSuccess) {
               if (strStyle == "left") {
                  nStyle = TEXTSTYLE_LEFT;
               } else if (strStyle == "right") {
                  nStyle = TEXTSTYLE_RIGHT;
               } else if (strStyle == "center") {
                  nStyle = TEXTSTYLE_CENTER;
               }                 
            }
            // X1
            float fX1 = GetFloatToken (strLine.c_str(), "x1");
            // X2
            float fX2 = GetFloatToken (strLine.c_str(), "x2");
            // Y
            float fY = GetFloatToken (strLine.c_str(), "y");
            // Rotation
            float fRotation = GetFloatToken (strLine.c_str(), "rotation");
            // Fade
            int nFontFade = GetFloatToken (strLine.c_str(), "fade");
            // Couleur
            float fRed = GetFloatToken (strLine.c_str(), "R");
            float fGreen = GetFloatToken (strLine.c_str(), "G");
            float fBlue = GetFloatToken (strLine.c_str(), "B");
            float fAlpha = GetFloatToken (strLine.c_str(), "A");
            // Visibilité
            bVisible = GetBoolToken (strLine.c_str(), VISIBLE, true);		// Traitement du champ Visible
            // Données traitées, chargement dans la KList
            lpObjContainer = new EObjectContainer(_strSceneName, strObjectName);
            lpObjContainer->_type = SCENEOBJECT_TEXT;
            SceneObjectText *objtext;
            objtext = new SceneObjectText(_strSceneName.c_str());
            objtext->SetData(strObjectName.c_str(), strTextID.c_str(), strFontName.c_str(), nFontSize, nStyle, fX1, fX2, fY, fRotation, nFontFade); 
            objtext->SetColor(fRed, fGreen, fBlue, fAlpha);
            objtext->SetVisible(bVisible);
            lpObjContainer->_lpObjectText = objtext;
            EObjectContainer::g_lpListObj.addToTail(lpObjContainer);
         }
         /****************************************************************/
         /* ANIMATION SCRIPT                                             */
         /****************************************************************/
         else if (strTypeObject == "anime")
         {
            bSuccess = GetStringToken (strLine.c_str(), OBJECTNAME, strObjectName);		// Traitement de l'ObjectName
            KAssert(bSuccess == true);
            bSuccess = GetStringToken (strLine.c_str(), ANIM_TARGET, strTarget);			// Traitement du champ "target:"
            KAssert(bSuccess == true);
            bSuccess = GetStringToken (strLine.c_str(), ANIMTYPE, strType);				// Traitement du champ type de mouvement ("type" dans le fichier)
            KAssert(bSuccess == true);
            
            bLoop = GetBoolToken (strLine.c_str(), LOOP, false);					// Traitement du champ "loop"
            
            if (strType == "rotate")
            {
               iAngle1 = GetIntToken (strLine.c_str(),ANGLE1);					// Traitement des champs Angles de départ et arrivée
               iAngle2 = GetIntToken (strLine.c_str(),ANGLE2);
               iDir = GetIntToken (strLine.c_str(),DIRECTION);					// Traitemtent du champ direction (sens de rotation)
               bBounce = GetBoolToken (strLine.c_str(), BOUNCE, false);			// Traitement du champ Bounce (effet de rebond)
               bLoop = GetBoolToken (strLine.c_str(), LOOP, true);			// Traitement du champ loop (vrai par défaut)
               
               bSuccess = GetStringToken (strLine.c_str(), COUNTERFUNC, strCounterFunction);		// Traitement du champ COUNTER TYPE (FUNCTION)
               if (!bSuccess)
                  iCounterFunction = K_COUNTER_LINEAR;
               else if (strCounterFunction == "EASEIN")
                  iCounterFunction = K_COUNTER_EASEIN;						// Start slowly and then accelerate to the target value
               else if (strCounterFunction == "EASEOUT")
                  iCounterFunction = K_COUNTER_EASEOUT;						// Start like the linear interpolation and then decelerate to the target value
               else if (strCounterFunction == "EASEINOUT")
                  iCounterFunction = K_COUNTER_EASEINOUT;						// Start slowly, accelerate and then decelerate to the target value
               else if (strCounterFunction == "LINEAR")
                  iCounterFunction = K_COUNTER_LINEAR; 						// Linear interpolation
            }
            
            if (strType == "p2p")
            {
               fx = GetFloatToken (strLine.c_str(), COORDX);					// Traitement des coordonnées x,y,x2,y2
               fy = GetFloatToken (strLine.c_str(), COORDY);
               fx2 = GetFloatToken (strLine.c_str(), COORDX2);
               fy2 = GetFloatToken (strLine.c_str(), COORDY2);
               iDelayMin = GetIntToken (strLine.c_str(),DELAYMIN);				// Traitement des délais (utilisés si Loop = true) pour le redémarrage aléatoire de l'anim
               iDelayMax = GetIntToken (strLine.c_str(),DELAYMAX);
               bBounce = GetBoolToken (strLine.c_str(), BOUNCE, false);			// Traitement du champ bounce
               bLoop = GetBoolToken (strLine.c_str(), LOOP, false);			// Traitement du champ loop (recommence au début)
            }
			if (strType == "marquee") {
               iDir = GetIntToken (strLine.c_str(),DIRECTION);					// Traitemtent du champ direction (sens de rotation)
               fDuration = GetFloatToken (strLine.c_str(),DURATION);			// Traitement du champ Duration
			}
            if (strType != "frame" || strType != "video")
            {
               fDuration = GetFloatToken (strLine.c_str(),DURATION);			// Traitement du champ Duration
            }
            else {
               fDuration = 0;
            }
            
            // Données traitées, chargement dans la KList
            lpObjContainer = new EObjectContainer(_strSceneName, strObjectName);
            lpObjContainer->_type = SCENEOBJECT_ANIMATION;
            SceneObjectAnimation *objanim;
            objanim = new SceneObjectAnimation(_strSceneName.c_str());
            objanim->SetData(id, strObjectName.c_str(),strTarget.c_str(),strType.c_str(), bLoop, iAngle1,iAngle2,iDir,bBounce,fx,fy,fx2,fy2,fDuration,iCounterFunction,iDelayMin,iDelayMax,iDir);
            lpObjContainer->_lpObjectAnime = objanim;
            EObjectContainer::g_lpListObj.addToTail(lpObjContainer);
            
         }
         else
            KAssert (strTypeObject.c_str());
      }
   }
   else {
      //showFileError(lpszFileName);
   }

   if (!bSceneSection) {
      K_LOG("No scene section for %s",lpszFileName);
   }
   K_LOG("-end-parseSceneFile(%s)",lpszFileName);
   _lpSceneDirector->setLoading(false);
}

void EScene::AddZone(const char *szName, int x, int y, int w, int h, const char *szMousePointer, bool bVisible)
{
   // Données traitées, chargement dans la KList
   std::string str;
   str = szName;
   EObjectContainer *lpObjContainer = new EObjectContainer(_strSceneName, str);
   lpObjContainer->_type = SCENEOBJECT_IMAGE;
   SceneObjectImage *objimg;
   objimg = new SceneObjectImage(_strSceneName.c_str());
   objimg->SetData("", 0, szName, "static", 1, false, 0, x, y, w, h, bVisible);
   objimg->SetMousePointer(szMousePointer);
   lpObjContainer->_lpObjectImage = objimg;
   EObjectContainer::g_lpListObj.addToTail(lpObjContainer);
}
void EScene::SetupItem(const char *szObjectName)
{
   K_LOG("SetupItem(%s);", szObjectName);
   
   if (!TestGlobal(szObjectName)) {
      SceneObjectImage *tmp = GetObjectImageByName(szObjectName);
#ifdef DEBUG
      if (tmp == NULL)
         K_LOG("SetupItem: setupitem call for undef objectimage: %s, scene:%s",szObjectName,_strSceneName.c_str());
#endif
      KAssert(tmp!=NULL);
      SetVisible(szObjectName, true, true);
      if (strlen(tmp->getInvImageName())>0) {
         EImageBank::getImage(tmp->getInvImageName(),"png",true,true,true);
      } else {
         K_LOG("SetupItem: undefined inv image name for object: %s, scene:%s",szObjectName,_strSceneName.c_str());
      }
   } else {
      SetVisible(szObjectName, false, true);
   }
}
void EScene::PickupSimple(const char *szObjectName, const char *szInventoryName)
{
#ifdef DEBUG
   K_LOG("PickupSimple(%s,%s);",szObjectName,szInventoryName);
   K_LOG("InventoryItem: pickupsimple, scene:%s, object:%s, invimg:%s",_strSceneName.c_str(), szObjectName,szInventoryName);
#endif
   // Prend la position de l'objet dans la scène
   int x, y;
   GetObjectPosition(szObjectName, x, y, true, true);
   // Ramasse l'objet
   _lpSceneDirector->getSequencer()->PickupItem(NULL, szInventoryName, (float)x, (float)y, 1);
   // Fais disparaitre l'objet de la scène si il existe
   if (isVisible(szObjectName)) {
      SetVisible(szObjectName, false);
   }
   // Mémorise dans le contexte que l'objet a été ramassé
   SetGlobal(szObjectName,"1");
}

void EScene::PickupMultiple(const char *szObjectName, const char *szInventoryName, int nNbPartNeeded)
{
#ifdef DEBUG
   K_LOG("PickupMultiple(%s,%s,%d);",szObjectName,szInventoryName,nNbPartNeeded);
   K_LOG("InventoryItem: pickupmultiple, scene:%s, object:%s, invimg:%s",_strSceneName.c_str(), szObjectName,szInventoryName);
#endif
   // Prend la position de l'objet dans la scène
   int x, y;
   GetObjectPosition(szObjectName, x, y, false, false);
   int nIndex = GetIndex(szObjectName);
   // Ramasse l'objet
   if (_lpSceneDirector->PickupItem(szInventoryName, (float)x, (float)y, nNbPartNeeded, nIndex) != NULL)
   {
      // Fais disparaitre l'objet de la scène
      SetVisible(szObjectName, false);
      // Mémorise dans le contexte que l'objet a été ramassé
      SetGlobal(szObjectName,"1");
   }
   EImageBank::getImage(szInventoryName,"png",true,true,true);
   const char *fullItemName = SCENEDIRECTOR->MapPartialInvItemFullInvItem(szInventoryName);
   EImageBank::getImage(fullItemName,"png",true,true,true);
}

void EScene::DrawFancyString(const char *szString, KTextFace *lpFont, float fx, float fy, float fw, bool bCentered, float fBlend, bool bCustomColor, float r, float v, float b)
{
   float rr,vv,bb;
   
   if (bCustomColor) {
      rr = r;
      vv = v;
      bb = b;
   }
   else {
      rr = 1.0f;
      vv = 1.0f;
      bb = 1.0f;
   }

   lpFont->setBlend(fBlend);
   lpFont->setBlitColor(0, 0, 0, fBlend*0.125f);
   for (int y = -2; y <= 2; y++) {
      for (int x = -2; x <= 2; x++) {
         if (bCentered) {
            lpFont->drawMultiline(szString, fx-(fw/2)+x, fx+(fw/2)+x, fy + y, TEXTSTYLE_CENTER);
         } else {
            lpFont->drawStringFromLeftF (szString, fx + x, fy + y);
         }
      }
   }
   lpFont->setBlitColor (rr, vv, bb, fBlend);
   if (bCentered) {
      lpFont->drawMultiline(szString, fx-(fw/2), fx+(fw/2), fy, TEXTSTYLE_CENTER);
   } else {
      lpFont->drawStringFromLeftF(szString, fx, fy);
   }
   lpFont->setBlitColor (1, 1, 1, 1);
   lpFont->setBlend(1);
}

void EScene::DrawMultiLineFancyString(const char *szString, KTextFace *lpFont, float fx, float fx2, float fy, float fBlend, float r, float v, float b) {
   
   lpFont->setBlend(fBlend);
   lpFont->setBlitColor(0, 0, 0, fBlend*0.125f);
   for (int y = -2; y <= 2; y++) {
      for (int x = -2; x <= 2; x++) {
         lpFont->drawMultiline(szString, fx+x, fx2+x, fy + y, TEXTSTYLE_LEFT);
      }
   }
   lpFont->setBlitColor (r, v, b, fBlend);
   lpFont->drawMultiline(szString, fx, fx2, fy, TEXTSTYLE_LEFT);

   lpFont->setBlitColor (1, 1, 1, 1);
   lpFont->setBlend(1);
}

KList<EObjectContainer> EObjectContainer::g_lpListObj;
