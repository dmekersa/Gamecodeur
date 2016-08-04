/*
 *  Scene_Island_Falaise.cpp
 *  enigma
 *
 *  Created by Rockford on 30/04/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Island_Falaise.h"
#include "ESceneDirector.h"
#include "SoundBank.h"
#include "GlobalBank.h"
#include "EMiniJeuSimonShark.h"

/* Constructeur */
Scene_Island_Falaise::Scene_Island_Falaise(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpSndWind = ESoundBank::getSound("island_wind");
   
   EMiniJeuSimonShark::Preload();
}

/* Destructeur */
Scene_Island_Falaise::~Scene_Island_Falaise()
{
   StopAnimation("cliff_waves_anim");

}

void Scene_Island_Falaise::Init()
{
   _lpSndWind->playSample();
   StartAnimation("cliff_waves_anim");
   StartAnimation("animmouette1_1P2P");
   StartAnimation("animmouette1_2P2P");
   StartAnimation("animmouette2P2P");
//   StartAnimation("wavesanim");
   
   // Le requin a t'il été chassé ?
   if (!TaskResolved("task_island_expelshark")) {
      SetVisible("falaise_shark1", true);
      StartAnimation("shark1anim");
      SetVisible("falaise_shark2", false);
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_VIEW_MURRAY_NICE") ,"",true);
   }
   else {
      SetVisible("falaise_shark1", false);
      SetVisible("falaise_shark2", true);
      StartAnimation("shark2anim");
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_VIEW_MURRAY_DONE") ,"",true);
   }
}

void Scene_Island_Falaise::Check()
{
   EScene::Check();
#ifdef SCENE_SHORTCUT
   if (KInput::isPressed(K_VK_F5))
   {
      _lpSceneDirector->GoToScene("menu");
   }
#endif
}

void Scene_Island_Falaise::Logic()
{
	EScene::Logic();
}

void Scene_Island_Falaise::Draw()
{
   EScene::Draw();
}

void Scene_Island_Falaise::Close()
{
   _lpSndWind->stopSample();
}

bool Scene_Island_Falaise::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Le requin autour du bateau
   if (strcmp(szObjectName, "falaise_shark1") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk("clic", CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_VIEW_MURRAY_SHARK"), "", true);
      return true;
   }
   
   // Le requin qui s'est éloigné
   if (strcmp(szObjectName, "falaise_shark2") == 0)
   {
      _lpSceneDirector->getSequencer()->Talk("dents1", CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_VIEW_MURRAY_HERE"), "", true);
      return true;
   }
   
   // La mer
   if (strcmp(szObjectName, "falaise_sea") == 0 && !TaskResolved("task_island_expelshark"))
   {
      _lpSceneDirector->getSequencer()->Talk("clic", CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_VIEW_MURRAY_SEA"), "", true);
      return true;
   }   
   if (strcmp(szObjectName, "falaise_sea") == 0 && TaskResolved("task_island_expelshark"))
   {
      _lpSceneDirector->getSequencer()->Talk("clic", CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_VIEW_MURRAY_FREE"), "", true);
      return true;
   }   
   
   return false;
}

bool Scene_Island_Falaise::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

/* Un objet de l'inventaire est utilisé sur un objet de la scène */
bool Scene_Island_Falaise::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur utilise la clé sur la vitrine de droite
   if ( strcmp(szItemName, "inv_deadfish") == 0 ) {
      if (strcmp(szObjectName, "falaise_sea") == 0 || strcmp(szObjectName, "shark1") == 0)
      {
         _lpSceneDirector->getSequencer()->PlaySound(NULL, "plouf");
         EMiniJeuBase *lpMiniJeu = new EMiniJeuSimonShark();
         _lpSceneDirector->GoToMiniGame("sharksimon", lpMiniJeu);
         return true;
      }
   }
   return false;
}

void Scene_Island_Falaise::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (strcmp(szGameName, "sharksimon")==0 && bIsRevolved) {
      // On peut maintenant retirer l'objet de l'inventaire
      _lpSceneDirector->DropItem("inv_deadfish");
      // Le requin est parti au large !
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_VIEW_MURRAY_SUCCESS") ,"",true);
      ResolveTask("task_island_expelshark");
      StopAnimation("shark1anim");
      SetVisible("falaise_shark1", false);
      SetVisible("falaise_shark2", true);
      StartAnimation("shark2anim");
   }
   if (strcmp(szGameName, "sharksimon")==0 && !bIsRevolved) {
      _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("ISLAND_VIEW_MURRAY_FAILED") ,"",true);
   }
}
