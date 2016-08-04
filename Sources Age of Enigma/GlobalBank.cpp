/*
 *  GlobalBank.cpp
 *  enigma
 *
 *  Created by Rockford on 06/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "GlobalBank.h"
#include "FontBank.h"
#include "KGame.h"
#include "ESceneDirector.h"

const char *EGlobalBank::getStrValue(const char *szName, const char *szDefault)
{
   // Parcours la liste pour en récupérer les valeurs
   EGlobalBank *o;
   bool bGotNext = g_ListGlobal.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (strcmp(szName, o->_szName) == 0) {
            return o->_szValue;
         }
         bGotNext = g_ListGlobal.getNext (o);
      }
   }
   return szDefault;
}

int EGlobalBank::getIntValue(const char *szName, int nDefault)
{
   // Parcours la liste pour en récupérer les valeurs
   EGlobalBank *o;
   bool bGotNext = g_ListGlobal.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         long l = strcmp(szName, o->_szName);
         if (l == 0) {
            return atoi(o->_szValue);
         }
         bGotNext = g_ListGlobal.getNext (o);
      }
   }
   return nDefault;
}

bool EGlobalBank::isExist(const char *szName)
{
   // Parcours la liste pour en récupérer les valeurs
   EGlobalBank *o;
   bool bGotNext = g_ListGlobal.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         long l = strcmp(szName, o->_szName);
         if (l == 0) {
            return true;
         }
         bGotNext = g_ListGlobal.getNext (o);
      }
   }
   return false;
}

bool EGlobalBank::RemoveValue(const char *szName)
{
   // Parcours la liste pour trouver la valeur
   EGlobalBank *o;
   bool bGotNext = g_ListGlobal.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         long l = strcmp(szName, o->_szName);
         if (l == 0) {
            g_ListGlobal.remove(o);
            delete o;
            return true;
         }
         bGotNext = g_ListGlobal.getNext (o);
      }
   }
   return false;
}

void EGlobalBank::RemoveValueStartWith(const char *szStartWith)
{
   // Parcours la liste pour trouver les valeurs commençant par szStartWith
   EGlobalBank *o;
   EGlobalBank *otodelete;
   bool bGotNext = g_ListGlobal.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (strstr(o->_szName, szStartWith) && strcmp(o->_szName, szStartWith) != 0) {
            otodelete = o;
            g_ListGlobal.remove(o);
            bGotNext = g_ListGlobal.getNext (o);
            delete otodelete;
         }
         else {
            bGotNext = g_ListGlobal.getNext (o);
         }
      }
   }
}

void EGlobalBank::ShowTaskList(float py, const char *szUniverseName)
{
   float yOffset = py + 80.0f;
   float y = 10.0f + yOffset;
   float x = 95.0f;
   int iNb = 0;
   KTextFace *lpFont = EFontBank::getFont("tahoma.ttf", 14);
   lpFont->setColor(1, 1, 1, 1);
   // Récupère la version traduite de sz2
   const char *szKstr;
   char szUniverseTag[256];
   memset(szUniverseTag, 0, 256);
   
   // Parcours la liste pour en récupérer les valeurs
   EGlobalBank *o;
   bool bGotNext = g_ListGlobal.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (o->_bTask && strcmp(o->_szValue,"0") == 0) {
            // On construit un bout de chaine de format "_univers_"
            // Car les tâches sont nommées ainsi : task_univers_nomdetask
            strcpy(szUniverseTag, "_");
            strcat(szUniverseTag, szUniverseName);
            strcat(szUniverseTag, "_");
            // Est-ce dans l'univers demandé ?
            if (strstr(o->_szName, szUniverseTag)) {
               szKstr = KStr(o->_szName);           
               if (szKstr) {
                  lpFont->setColor(1, 1, 1, 1);
                  SCENEDIRECTOR->GetCurrentScene()->DrawFancyString(szKstr, lpFont, x, y, 0, false, 0.9f);
               }
               else {
                  lpFont->drawStringFromLeftF(o->_szName, x, y);
               }
               
               // 8 maxi pour l'instant
               iNb++;
               if (iNb >= 8) {
                  break;
               }
               
               y += lpFont->getHeightPix();

               // Changement de colonne ?
               if (y > 77 + yOffset) {
                  y = 10 + yOffset;
                  x = 477+20+24;
               }
            }
         }
         bGotNext = g_ListGlobal.getNext (o);
      }
   }
   
   // Affichage du nombre de tâches
   char szTaskNb[256];
   memset(szTaskNb, 0, 256);
   sprintf(szTaskNb, "%d %s", iNb, "objectives");
   lpFont = EFontBank::getFont("tahoma.ttf", 16);
   lpFont->setColor(0, 0, 0, 1);
   SCENEDIRECTOR->GetCurrentScene()->DrawFancyString(szTaskNb, lpFont, 498.0f, 99+yOffset, 135.0f, true, 0.9f);
//   lpFont->drawStringCenteredF(szTaskNb, 410, 545, 98 + yOffset);
}

void EGlobalBank::AddStrValue(const char *szName, const char *szValue, bool b)
{
   if (!isExist(szName))
   {
      EGlobalBank *lp;
      lp = new EGlobalBank;
      memset(lp->_szName, 0, 256);
      strcpy(lp->_szName,szName);
      memset(lp->_szValue, 0, 1024*4);
      strcpy(lp->_szValue,szValue);
      lp->_bTask = b;
      g_ListGlobal.addToTail(lp);
   } else {
      ChangeStrValue(szName, szValue);
   }
}

void EGlobalBank::AddTaskUnique(const char *szName)
{
   if (!TaskResolved(szName) && !TaskToResolve(szName)) {
      AddStrValue(szName, "0", true);

      // demande une mise à jour de la taskList
      SCENEDIRECTOR->majTaskList();
   }
}

void EGlobalBank::AddNewObjective(const char *szUniverse, const char *szName)
{
   KAssert(szUniverse != NULL);
   KAssert(strlen(szUniverse)>0);
   KAssert(szName != NULL);
   KAssert(strlen(szName)>0);
   if (!TaskResolved(szName) && !TaskToResolve(szName)) {
      // Ajout du tag "objective_"
      char szNewObjTag[512];
      memset(szNewObjTag, 0, 512);
      strcpy(szNewObjTag, "objective");
      strcat(szNewObjTag, "_");
      strcat(szNewObjTag, szUniverse);
      strcat(szNewObjTag, "_");
      strcat(szNewObjTag, szName);
      // Séquence
      // TODO: Finir la séquence
      char szNewObj[512];
      memset(szNewObj, 0, 512);
      strcpy(szNewObj, "NEW OBJECTIVE: ");
      strcat(szNewObj, szName);

      
    //  SCENEDIRECTOR->getSequencer()->VoiceOver(NULL, szNewObj, "gong");
      // Valeur dans la banque
      AddStrValue(szNewObjTag, "0", true);

      // ici -> envoyer l'objectif au taskList et lui demandé de l'afficher
      SCENEDIRECTOR->addObjectiveTaskList(szNewObjTag);

      // demande une mise à jour de la taskList
      SCENEDIRECTOR->majTaskList();
   }
}

void EGlobalBank::AddNewHint(const char *szUniverse, const char *szObjective, const char *szName)
{
   KAssert(szUniverse != NULL);
   KAssert(strlen(szUniverse)>0);
   KAssert(szObjective != NULL);
   KAssert(strlen(szObjective)>0);
   KAssert(szName != NULL);
   KAssert(strlen(szName)>0);
   if (!TaskResolved(szName) && !TaskToResolve(szName)) {
      // Ajout du tag "objective_"
      char szNewObjTag[512];
      memset(szNewObjTag, 0, 512);
      strcpy(szNewObjTag, "question");
      strcat(szNewObjTag, "_");
      strcat(szNewObjTag, szUniverse);
      strcat(szNewObjTag, "_");
      strcat(szNewObjTag, szObjective);
      strcat(szNewObjTag, "_");
      strcat(szNewObjTag, szName);
      // Valeur dans la banque
      AddStrValue(szNewObjTag, "0", true);

      // demande une mise à jour de la taskList
      SCENEDIRECTOR->majTaskList();
   }
}

void EGlobalBank::RemoveOneHint(const char *szUniverse, const char *szObjective, const char *szName)
{
   KAssert(szUniverse != NULL);
   KAssert(strlen(szUniverse)>0);
   KAssert(szObjective != NULL);
   KAssert(strlen(szObjective)>0);
   KAssert(szName != NULL);
   KAssert(strlen(szName)>0);
   // Ajout du tag "objective_"
   char szNewObjTag[512];
   memset(szNewObjTag, 0, 512);
   strcpy(szNewObjTag, "question");
   strcat(szNewObjTag, "_");
   strcat(szNewObjTag, szUniverse);
   strcat(szNewObjTag, "_");
   strcat(szNewObjTag, szObjective);
   strcat(szNewObjTag, "_");
   strcat(szNewObjTag, szName);
   // Supression dans la banque
   RemoveValue(szNewObjTag);

   // demande une mise à jour de la taskList
   SCENEDIRECTOR->majTaskList();
}

void EGlobalBank::SetTo1(const char *szName)
{
   ChangeStrValue(szName, "1");
}

void EGlobalBank::ResolveTaskHints(const char *szUniverse, const char *szObjective)
{
   char szNewObjTag[512];
   // 1) Positionne l'objectif à 1
   memset(szNewObjTag, 0, 512);
   strcpy(szNewObjTag, "objective");
   strcat(szNewObjTag, "_");
   strcat(szNewObjTag, szUniverse);
   strcat(szNewObjTag, "_");
   strcat(szNewObjTag, szObjective);
   ChangeStrValue(szNewObjTag, "1");
   // 2) Supprime les hints associés
   memset(szNewObjTag, 0, 512);
   strcpy(szNewObjTag, "question");
   strcat(szNewObjTag, "_");
   strcat(szNewObjTag, szUniverse);
   strcat(szNewObjTag, "_");
   strcat(szNewObjTag, szObjective);
   RemoveValueStartWith(szNewObjTag);

   // demande une mise à jour de la taskList
   SCENEDIRECTOR->majTaskList();
}

bool EGlobalBank::getObjectiveResolved(const char *szUniverse, const char *szName)
{
   char szNewObjTag[512];
   // 1) Positionne l'objectif à 1
   memset(szNewObjTag, 0, 512);
   strcpy(szNewObjTag, "objective");
   strcat(szNewObjTag, "_");
   strcat(szNewObjTag, szUniverse);
   strcat(szNewObjTag, "_");
   strcat(szNewObjTag, szName);
   return getIntValue(szNewObjTag, 0) == 1;
}

void EGlobalBank::ResolveOneTask(const char *szName)
{
   ChangeStrValue(szName, "1");
   // demande une mise à jour de la taskList
   SCENEDIRECTOR->majTaskList();
}

void EGlobalBank::ChangeStrValue(const char *szName, const char *szValue)
{
   // Parcours la liste pour en récupérer les valeurs
   EGlobalBank *o;
   bool bGotNext = g_ListGlobal.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         long l = strcmp(szName, o->_szName);
         if (l == 0) {
            strcpy(o->_szValue,szValue);
            return;
         }
         bGotNext = g_ListGlobal.getNext (o);
      }
   }
   // Pas trouvée ? On l'ajoute
   AddStrValue(szName, szValue);
}

KList<EGlobalBank> EGlobalBank::g_ListGlobal;