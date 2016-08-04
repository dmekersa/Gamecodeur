/*
 *  Scene_Middleage_Jail.cpp
 *  enigma
 *
 *  Created by Rockford on 06/10/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EScene.h"
#include "Scene_Middleage_Jail.h"
#include "ESceneDirector.h"
#include "GlobalBank.h"
#include "EMiniJeuPlantAddition.h"
#include "EMiniJeuGardening.h"
#include "MusicBank.h"

/* Constructeur */
Scene_Middleage_Jail::Scene_Middleage_Jail(ESceneDirector *lpSceneDirector) : EScene(lpSceneDirector)
{
   _lpBgGraphic = KPTK::createKGraphic ();
   
   EMiniJeuGardening::Preload();
   EMiniJeuPlantAddition::Preload();
}

/* Destructeur */
Scene_Middleage_Jail::~Scene_Middleage_Jail()
{
   delete _lpBgGraphic;
}

void Scene_Middleage_Jail::Init()
{
   // Le contenu est prêt, on efface l'ancien
   if (TaskResolved("task_middleage_getplant4")) {
      SetupItem("middleage_contentplate_ready");
   }
   if (isVisible("middleage_contentplate_ready")) {
      StartEmitter("flake_content");
      SetVisible("middleage_contentplate", false, true);
   }
   else {
      StopEmitter("flake_content");
      if (_lpSceneDirector->getInventory()->getItemFromInventory("inv_middleage_contentplate_ready"))
      {
         SetVisible("middleage_contentplate", false, true);
      }
   }
   
   // Sorcière par défait attend
   SetVisible("middleage_witch_wait", true, true);
   // Si on n'a pas encore rencontré la sorcière
   if (!TaskResolved("task_middleage_contactwitch")) {
      SetVisible("middleage_witch_wait", true, true);
      SetVisible("middleage_witch_ask", false, true);
   }
   else {
      int x,y;
      // Attente de la sorcière en fonction de la quête en cours
      if (TaskToResolve("task_middleage_getplate")) {
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
         GetObjectPosition("middleage_witch_ask", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_ASKPLATE"), "", false);
      }
      // Attend qu'on pose l'assiette
      else if (TaskToResolve("task_middleage_putplate")) {
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_SETPLATE"), "", false);
      }
      // Attend qu'on mettre une plante dans l'assiette
      else if (TaskToResolve("task_middleage_getplant1") || 
               TaskToResolve("task_middleage_getplant2") ||
               TaskToResolve("task_middleage_getplant3") ||
               TaskToResolve("task_middleage_getplant4")) {
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
         GetObjectPosition("middleage_witch_wait", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_SOWHAT"), "", false);
      }
      // Attend la préparation terminée
      else if (TaskToResolve("task_middleage_givecontent")) {
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
         GetObjectPosition("middleage_witch_ask", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_ASKCONTENT"), "", false);
      }
      // Attend la fiole
      else if (TaskToResolve("task_middleage_giveflask")) {
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
         GetObjectPosition("middleage_witch_ask", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_ASKFLASK"), "", false);
      }
      // Attend le bouchon
      else if (TaskToResolve("task_middleage_givecap")) {
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
         GetObjectPosition("middleage_witch_ask", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_ASKCAP"), "", false);
      }
      // Plantes à préparer
      else if (TaskToResolve("task_middleage_prepareplant1") ||
          TaskToResolve("task_middleage_prepareplant2") || 
          TaskToResolve("task_middleage_prepareplant3") || 
          TaskToResolve("task_middleage_prepareplant4")) {
         int x,y;
         GetObjectPosition("middleage_witch_wait", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_PREPARE"), "", false);
      }
      else {
         _lpSceneDirector->getSequencer()->ShowImage(NULL,"middleage_witch_wait", true);
         GetObjectPosition("middleage_witch_wait", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_SOWHAT"), "", false);
      }
   }
   
   // Le plat a été posé
   if (TaskResolved("task_middleage_putplate")) {
      SetVisible("middleage_emptyplate", true);
   }

   // Les plantes dans l'assiette
   if (TaskResolved("task_middleage_startprepare") && !isVisible("middleage_contentplate_ready") && !_lpSceneDirector->getInventory()->getItemFromInventory("inv_middleage_contentplate_ready")) {
      // Affiche une plante dans l'assiette
      SetVisible("middleage_contentplate", true, true);
   }
   

}

void Scene_Middleage_Jail::Check()
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

void Scene_Middleage_Jail::Logic()
{
	EScene::Logic();
}

void Scene_Middleage_Jail::Draw()
{
   EScene::Draw();
}

void Scene_Middleage_Jail::Close()
{
}

bool Scene_Middleage_Jail::ObjectClicked(const char *szObjectName, float x, float y)
{
   // Clic sur la sorcière la 1ère fois
   if (strcmp(szObjectName, "middleage_witch_wait") == 0) {
      int x,y;
      if( !TaskResolved("task_middleage_contactwitch")) {
         ResolveTask("task_middleage_contactwitch");
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"spooky_piano");
         GetObjectPosition("middleage_witch_ask", x, y, true, false);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_RHAAA"), "", false);
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_AFFRAID"), "", false, true);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_IMNOT"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_LETMEHELP"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_POISON"), "", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_OPEN"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_DIDYOUSAVE"), "", false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_NOSAVE"), "", false, true);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_BRINGMEPLATE"), "", false);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
         _lpSceneDirector->getSequencer()->NarrationMode(NULL, SEQUENCE_NARRATION_DIALOG, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_TRUSTHER"), "", true, false);
         AddObjective("middleage","help");
         AddHint("middleage","help","plate");
         AddHint("middleage","help","pestle");
         AddTask("task_middleage_getplate");
         return true;
      }
      if (TaskToResolve("task_middleage_putplant1") || 
          TaskToResolve("task_middleage_putplant2") ||
          TaskToResolve("task_middleage_putplant3") ||
          TaskToResolve("task_middleage_putplant4")) {
         GetObjectPosition("middleage_witch_wait", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("MIDDLEAGE_WITCH_PUTINPLATE"), "", true);
      }
   }
   
   // Clic sur la sorcière qui demande quelque chose
   if (strcmp(szObjectName, "middleage_witch_ask") == 0 || strcmp(szObjectName, "middleage_witch_wait") == 0) {
      int x,y;
      GetObjectPosition("middleage_witch_ask", x, y, true, false);
      if (TaskToResolve("task_middleage_getplate")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("MIDDLEAGE_WITCH_BRINGMEPLATE"), "", false);
      }
      else if (TaskToResolve("task_middleage_putplate")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("MIDDLEAGE_WITCH_SETPLATE"), "", false);
      }
      else if (TaskToResolve("task_middleage_getplant1")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("MIDDLEAGE_WITCH_ASKPLANT1"), "", false);
      }
      else if (TaskToResolve("task_middleage_getplant2")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("MIDDLEAGE_WITCH_ASKPLANT2"), "", false);
      }
      else if (TaskToResolve("task_middleage_getplant3")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("MIDDLEAGE_WITCH_ASKPLANT3"), "", false);
      }
      else if (TaskToResolve("task_middleage_getplant4")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("MIDDLEAGE_WITCH_ASKPLANT4"), "", false);
      }
      else if (TaskToResolve("task_middleage_giveflask")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("MIDDLEAGE_WITCH_ASKFLASK"), "", false);
      }
      else if (TaskToResolve("task_middleage_givecap")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("MIDDLEAGE_WITCH_ASKCAP"), "", false);
      }
      else if (TaskToResolve("task_middleage_prepareplant1") ||
               TaskToResolve("task_middleage_prepareplant2") ||
               TaskToResolve("task_middleage_prepareplant3") ||
               TaskToResolve("task_middleage_prepareplant4")) {
         _lpSceneDirector->getSequencer()->Talk(szObjectName, x, y, KStr("MIDDLEAGE_WITCH_PREPARE"), "", false);
      }
   }
   
   // Clic sur le bol
   if (strcmp(szObjectName, "middleage_emptyplate") == 0) {
      _lpSceneDirector->getSequencer()->Talk(szObjectName, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_EMPTYPLATE"), "", true, false);
   }

   // Clic sur le contenu
   if (strcmp(szObjectName, "middleage_contentplate_ready") == 0) {
      StopEmitter("flake_content");
      PickupSimple("middleage_contentplate_ready", "inv_middleage_contentplate_ready");
   }
   
   return false;
}

bool Scene_Middleage_Jail::ObjectOver(char *szObjectName, float x, float y)
{
   return false;
}

bool Scene_Middleage_Jail::ItemUsed(const char *szItemName, const char *szObjectName)
{
   // Le joueur donne quelque chose à la sorcière
   if (strcmp(szObjectName, "middleage_witch_wait") == 0) {
      int x,y;
      GetObjectPosition("middleage_witch_wait", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_WHATAREU"), "", false);
      return true;
   }
   // Le joueur donne quelque chose à la sorcière
   if (strcmp(szObjectName, "middleage_witch_ask") == 0) {
      int x,y;
      GetObjectPosition("middleage_witch_ask", x, y, true, false);
      // Donne l'assiette
      if (strcmp(szItemName, "inv_middleage_plate") == 0) {
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_SETPLATE"), "", false);
         return true;
      }
      // Donne la plante 1
      else if (strcmp(szItemName, "inv_middleage_poppy") == 0 && TaskToResolve("task_middleage_getplant1")) {
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_PUTINPLATE"), "", false);
         return true;
      }
      // Donne la plante 2
      else if (strcmp(szItemName, "inv_middleage_ginkgo") == 0 && TaskToResolve("task_middleage_getplant2")) {
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_PUTINPLATE"), "", false);
         return true;
      }
      // Donne la plante 3
      else if (strcmp(szItemName, "inv_middleage_garlic") == 0 && TaskToResolve("task_middleage_getplant3")) {
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_PUTINPLATE"), "", false);
         return true;
      }
      // Donne la plante 4
      else if (strcmp(szItemName, "inv_middleage_rue") == 0 && TaskToResolve("task_middleage_getplant4")) {
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_PUTINPLATE"), "", false);
         return true;
      }
      // Donne la préparation
      else if (strcmp(szItemName, "inv_middleage_contentplate_ready") == 0 && TaskToResolve("task_middleage_givecontent")) {
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_ASKFLASK"), "", false);
         ResolveObjective("middleage","plant");
         ResolveTask("task_middleage_givecontent");
         AddTask("task_middleage_giveflask");
         AddHint("middleage","help","flask");
         return true;
      }
      // Donne la fiole
      else if (strcmp(szItemName, "inv_middleage_flask_empty") == 0 && TaskToResolve("task_middleage_giveflask")) {
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_ASKCAP"), "", false);
         ResolveTask("task_middleage_giveflask");
         AddTask("task_middleage_givecap");
         return true;
      }
      // Donne le bouchon, on obtient le remède !!
      else if (strcmp(szItemName, "inv_middleage_cap") == 0 && TaskToResolve("task_middleage_givecap")) {
         _lpSceneDirector->DropItem(szItemName);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         // On obtient la fiole
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
         _lpSceneDirector->getSequencer()->PickupItem(NULL, "inv_middleage_flask_full", 450, 475, 1);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_GONOW"), "", false);
         ResolveObjective("middleage","help");
         ResolveTask("task_middleage_giveflask");
         AddTask("task_middleage_treatabbot");
         return true;
      }
      // Donne le liège
      else if (strcmp(szItemName, "inv_middleage_cork") == 0 && TaskToResolve("task_middleage_givecap")) {
         _lpSceneDirector->getSequencer()->VoiceOver(NULL, KStr("MIDDLEAGE_MONK_NEEDSHAPE"), "");
         return true;
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_NOTSUITABLE"), "", false);
         return true;
      }
   }
   // Le joueur pose l'assiette sur l'emplacement prévu
   if (strcmp(szObjectName, "middleage_plate_zone") == 0) {
      if (strcmp(szItemName, "inv_middleage_plate") == 0 && TaskResolved("task_middleage_contactwitch")) {
         SetVisible("middleage_emptyplate", true);
         // On n'a plus besoin de l'assiette dans l'inventaire
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_middleage_getplate");
         ResolveTask("task_middleage_putplate");
         int x,y;
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"spooky_piano");
         GetObjectPosition("middleage_witch_ask", x, y, true, false);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_ASKPLANT1"), "", false);
         AddTask("task_middleage_getplant1");
         RemoveHint("middleage","help","plate");
         AddObjective("middleage","plant");
         AddHint("middleage","plant","nextpoppy");
         return true;
      } else {
         int x,y;
         GetObjectPosition("middleage_witch_ask", x, y, true, false);
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_WHATAREU"), "", false);
         return true;
      }
   }
   // Le joueur pose la plante dans l'assiette
   if (strcmp(szObjectName, "middleage_emptyplate") == 0 || strcmp(szObjectName, "middleage_contentplate") == 0) {
      int x,y;
      GetObjectPosition("middleage_witch_wait", x, y, true, false);
      if (strcmp(szItemName, "inv_middleage_poppy") == 0 && TaskToResolve("task_middleage_getplant1")) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_middleage_startprepare");
         ResolveTask("task_middleage_getplant1");
         AddTask("task_middleage_prepareplant1");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "middleage_contentplate", true);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_PREPARE"), "", false);
         return true;
      }
      else if (strcmp(szItemName, "inv_middleage_ginkgo") == 0 && TaskToResolve("task_middleage_getplant2")) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_middleage_getplant2");
         AddTask("task_middleage_prepareplant2");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "middleage_contentplate", true);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_PREPARE"), "", false);
         return true;
      }
      else if (strcmp(szItemName, "inv_middleage_garlic") == 0 && TaskToResolve("task_middleage_getplant3")) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_middleage_startprepare");
         ResolveTask("task_middleage_getplant3");
         AddTask("task_middleage_prepareplant3");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "middleage_contentplate", true);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_PREPARE"), "", false);
         return true;
      }
      else if (strcmp(szItemName, "inv_middleage_rue") == 0 && TaskToResolve("task_middleage_getplant4")) {
         _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
         _lpSceneDirector->DropItem(szItemName);
         ResolveTask("task_middleage_startprepare");
         ResolveTask("task_middleage_getplant4");
         AddTask("task_middleage_prepareplant4");
         RemoveHint("middleage","plant","nextrue");
         AddHint("middleage","plant","how");
         _lpSceneDirector->getSequencer()->ShowImage(NULL, "middleage_contentplate", true);
         _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_ask", "middleage_witch_wait");
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_PREPARE"), "", false);
         return true;
      }
      else {
         _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_NOTSUITABLE"), "", false);
      }
   }
   // Le joueur utilise le pilon sur le contenu de l'assiette
   if (strcmp(szObjectName, "middleage_emptyplate") == 0 || strcmp(szObjectName, "middleage_contentplate") == 0) {
      // Morceau de bois non façonné
      if (strcmp(szItemName, "inv_middleage_wood") == 0) {
         _lpSceneDirector->getSequencer()->Talk(NULL, CHARACTER_POSX, CHARACTER_POSY, KStr("MIDDLEAGE_MONK_NEEDSHAPE"), "", false, false);
         return true;
      }
      // Pilon ok
      if (strcmp(szItemName, "inv_middleage_pestle") == 0 && TaskToResolve("task_middleage_prepareplant1")) {
         EMiniJeuBase *lpMiniJeu = new EMiniJeuPlantAddition();
         _lpSceneDirector->GoToMiniGame("prepare_plant1", lpMiniJeu);
         return true;
      }
      if (strcmp(szItemName, "inv_middleage_pestle") == 0 && TaskToResolve("task_middleage_prepareplant2")) {
         EMiniJeuBase *lpMiniJeu = new EMiniJeuGardening();
         _lpSceneDirector->GoToMiniGame("prepare_plant2", lpMiniJeu);
         return true;
      }
      if (strcmp(szItemName, "inv_middleage_pestle") == 0 && TaskToResolve("task_middleage_prepareplant3")) {
         EMiniJeuBase *lpMiniJeu = new EMiniJeuPlantAddition();
         _lpSceneDirector->GoToMiniGame("prepare_plant3", lpMiniJeu);
         return true;
      }
      if (strcmp(szItemName, "inv_middleage_pestle") == 0 && TaskToResolve("task_middleage_prepareplant4")) {
         EMiniJeuBase *lpMiniJeu = new EMiniJeuGardening();
         _lpSceneDirector->GoToMiniGame("prepare_plant4", lpMiniJeu);
         return true;
      }
   }
   
   return false;
}

void Scene_Middleage_Jail::MiniGameDone(const char *szGameName, bool bIsRevolved)
{
   if (!bIsRevolved) {
      int x,y;
      GetObjectPosition("middleage_witch_wait", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_WHATAREU"), "", true);
      return;
   }
   _lpSceneDirector->getSequencer()->PlaySound(NULL,"success");
   if (strcmp(szGameName, "prepare_plant1") == 0 && bIsRevolved) {
      int x,y;
      GetObjectPosition("middleage_witch_wait", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_WELLDONE"), "", true);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_ASKPLANT2"), "", true);
      ResolveTask("task_middleage_prepareplant1");
      AddTask("task_middleage_getplant2");
      RemoveHint("middleage","plant","nextpoppy");
      AddHint("middleage","plant","nextginkgo");
      return;
   }
   if (strcmp(szGameName, "prepare_plant2") == 0 && bIsRevolved) {
      int x,y;
      GetObjectPosition("middleage_witch_wait", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_WELLDONE"), "", true);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_ASKPLANT3"), "", true);
      ResolveTask("task_middleage_prepareplant2");
      AddTask("task_middleage_getplant3");
      RemoveHint("middleage","plant","nextginkgo");
      AddHint("middleage","plant","nextgarlic");
      return;
   }
   if (strcmp(szGameName, "prepare_plant3") == 0 && bIsRevolved) {
      int x,y;
      GetObjectPosition("middleage_witch_wait", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_WELLDONE"), "", true);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_ASKPLANT4"), "", true);
      ResolveTask("task_middleage_prepareplant3");
      AddTask("task_middleage_getplant4");
      RemoveHint("middleage","plant","nextgarlic");
      AddHint("middleage","plant","nextrue");
      return;
   }
   if (strcmp(szGameName, "prepare_plant4") == 0 && bIsRevolved) {
      int x,y;
      GetObjectPosition("middleage_witch_wait", x, y, true, false);
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_WELLDONE"), "", true);
      _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_witch_wait", "middleage_witch_ask");
      _lpSceneDirector->getSequencer()->SwitchImage(NULL,"middleage_contentplate", "middleage_contentplate_ready");
      _lpSceneDirector->getSequencer()->Talk(NULL, x, y, KStr("MIDDLEAGE_WITCH_ASKCONTENT"), "", true);
      ResolveTask("task_middleage_prepareplant4");
      AddTask("task_middleage_givecontent");
      StartEmitter("flake_content");
      return;
   }
}