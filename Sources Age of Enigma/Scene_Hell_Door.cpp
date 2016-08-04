/*
 *  Scene_Hell_Door.cpp
 *  enigma
 *
 *  Created by Rockford on 05/11/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Hell_Door.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuMatch3.h"
#include "ThreadLoader.h"

/* Constructeur */
Scene_Hell_Door::Scene_Hell_Door(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   EMiniJeuMatch3::Preload();
}

/* Destructeur */
Scene_Hell_Door::~Scene_Hell_Door()
{
   delete _lpBgGraphic;
}

void Scene_Hell_Door::Init()
{
}

void Scene_Hell_Door::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
   if (KInput::isPressed(K_VK_F6))
   {
   }
#endif
}

void Scene_Hell_Door::Logic()
{
	EScene::Logic();
}

void Scene_Hell_Door::Draw()
{
   EScene::Draw();
}

void Scene_Hell_Door::Close()
{
}

bool Scene_Hell_Door::ObjectClicked(const char *szObjectName, float x, float y)
{
   if (strcmp(szObjectName, "hell_backtohouse_zone") == 0)
   {
      EMiniJeuBase *lpMiniJeu = new EMiniJeuMatch3();
      _lpSceneDirector->getSequencer()->MiniGame(NULL, "geisha", lpMiniJeu);
      return true;
   }
   return false;
}

bool Scene_Hell_Door::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Hell_Door::ItemUsed(const char *szItemName, const char *szObjectName)
{
   
   return false;
}

void Scene_Hell_Door::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (bIsRevolved) {
      ResolveObjective("hell","escape");
      // Retour
      Loader::LoadScene("house_attic");
      _lpSceneDirector->ChangeMusic(DIRECTOR_MUSIC_NONE);
      _lpSceneDirector->getSequencer()->ShowImage(NULL, "blank", true, true);
      _lpSceneDirector->getSequencer()->GotoVideo(NULL, "videos/dark_tunnel_r.ogv","tunnel");
      _lpSceneDirector->getSequencer()->GoToScene(NULL,"house_attic","backfrombeyond", false);
   }
   if (!bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_GATHERFAILED"), "", true, false);
      return;
   }
}
