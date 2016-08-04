/*
 *  Vignette_Cinematic.cpp
 *  enigma
 *
 *  Created by Rockford on 13/07/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "MyGame.h"
#include "EScene.h"
#include "Vignette_Cinematic.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "ImageBank.h"
#include "FontBank.h"
#include "EPointer.h"

/* Constructeur */
Vignette_Cinematic::Vignette_Cinematic(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   /* Load font */
   EGLL();
   _lpFont = EFontBank::getFont("jancient22"),22;
   EGLR();
//   _lpFont->setHeightPix (20);
   _fScrollY = 690;
   _lHeight = 0;
   _lpString = NULL;
   _bClosing = false;

   /* Masque */
   _lpGraphicMask = EImageBank::getImage("vignette_cinematicmask");
}

/* Destructeur */
Vignette_Cinematic::~Vignette_Cinematic()
{
}

void Vignette_Cinematic::Init()
{
   _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_CINEMATIC);

   // Text
   if (_strSceneAdditionalName == "island") {
      _lpString = KStr("CINEMATIC_ISLAND_PROLOG");
   }
   else if (_strSceneAdditionalName == "japan") {
      _lpString = KStr("CINEMATIC_JAPAN_PROLOG");
   }
   else if (_strSceneAdditionalName == "inca") {
      _lpString = KStr("CINEMATIC_INCA_PROLOG");
   }
   else if (_strSceneAdditionalName == "egypt") {
      _lpString = KStr("CINEMATIC_EGYPT_PROLOG");
   }
   else if (_strSceneAdditionalName == "middleage") {
      _lpString = KStr("CINEMATIC_MIDDLEAGE_PROLOG");
   }
   else if (_strSceneAdditionalName == "hell") {
      _lpString = KStr("CINEMATIC_HELL_PROLOG");
   }
   else {
      _lpString = KStr("CINEMATIC_CREDITS");
   }
   
   _lHeight = _lpFont->getMultilineHeight(_lpString, 490, 940, _fScrollY, TEXTSTYLE_CENTER,0,22);
   
   if (_strSceneAdditionalName == "island") {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "island1", true);
   }
   else if (_strSceneAdditionalName == "japan") {
      SetVisible("japan1", true);   // TODO: Pourquoi devoir faire ça?
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan1", true);
      _lpSceneDirector->getSequencer()->Wait(NULL, 20000);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "japan2", true);
   }
   else if (_strSceneAdditionalName == "inca") {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "inca", true);
   }
   else if (_strSceneAdditionalName == "middleage") {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "middleage", true);
   }
   else if (_strSceneAdditionalName == "egypt") {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "egypt", true);
   }
   else {
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "picture_tbd", true);
   }

}

void Vignette_Cinematic::Check()
{
   EScene::Check();
   // TODO: tout refaire!
   if (Mouse.LeftEvent())
   {
      Mouse.EventClose();
      EndCinematic();
   }
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->CloseVignette();
   }
#endif
}

void Vignette_Cinematic::EndCinematic()
{
   if (_bClosing) {
      return;
   }
   _bClosing = true;
   _lpSceneDirector->getSequencer()->Reset();
   _lpSceneDirector->setZoomin(false);
   if (_strSceneAdditionalName == "island") {
      _lpSceneDirector->GoToScene("video_player", "soul_tunnel_island", false);
      SetGlobal("cinematic_island_done","1");
   }
   else if (_strSceneAdditionalName == "japan") {
      _lpSceneDirector->GoToScene("video_player", "soul_tunnel_japan", false);
      SetGlobal("cinematic_japan_done","1");
   }
   else if (_strSceneAdditionalName == "inca") {
      _lpSceneDirector->GoToScene("video_player", "soul_tunnel_inca", false);
      SetGlobal("cinematic_inca_done","1");
   }
   else if (_strSceneAdditionalName == "middleage") {
      _lpSceneDirector->GoToScene("video_player", "soul_tunnel_middleage", false);
      SetGlobal("cinematic_middleage_done","1");
   }
   else if (_strSceneAdditionalName == "egypt") {
      _lpSceneDirector->GoToScene("video_player", "soul_tunnel_egypt", false);
      SetGlobal("cinematic_egypt_done","1");
   }
   else if (_strSceneAdditionalName == "credits") {
      _lpSceneDirector->GoToScene("menu");
   }
}

void Vignette_Cinematic::Logic()
{
	EScene::Logic();
   _fScrollY -= 0.2;
}

void Vignette_Cinematic::Draw()
{
   EScene::Draw();
   
//   if (_State == SCENE_STABLE) {
      // Text
      _lpFont->setColor(1.0f, 1.0f, 1.0f, getFade());
      _lpFont->drawMultiline(_lpString, 490, 940, _fScrollY, TEXTSTYLE_CENTER,0,22);
      
      // Masque
      _lpGraphicMask->blitAlphaRect(0, 0, 1024, 768, 0, 0);
      
      // Skip
      KTextFace *lpFont = EFontBank::getFont("tahoma.ttf", 20);
      lpFont->setColor(1.0f, 1.0f, 1.0f, getFade());
      lpFont->drawStringCentered(KStr("CINEMATIC_CLOSETXT"), 0, 1024, 720);
      
      // Test de fin
      if ((_fScrollY + _lHeight < 0)) {
         EndCinematic();
      }
//   }
   
}

void Vignette_Cinematic::Close()
{
}

bool Vignette_Cinematic::ObjectClicked(const char *szObjectName, float x, float y)
{
//   _lpSceneDirector->getSequencer()->Reset();
//   if (isVisible("island1")) SetVisible("island1", false);
//   if (isVisible("japan1")) SetVisible("japan1", false);
//   if (isVisible("japan2")) SetVisible("japan2", false);
//   _lpSceneDirector->getSequencer()->Wait(NULL, 2000);
//   if (strcmp(szObjectName, "close") == 0) {
//      if (_strSceneAdditionalName == "island") {
//         SetGlobal("cinematic_island_done","1");
//      }
//      if (_strSceneAdditionalName == "japan") {
//         SetGlobal("cinematic_japan_done","1");
//      }
//      _lpSceneDirector->getSequencer()->CloseVignette(NULL);
//   }
   return false;
}