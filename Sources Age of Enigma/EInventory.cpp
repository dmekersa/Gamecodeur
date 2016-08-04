/*
 *  EInventory.cpp
 *  enigma
 *
 *  Created by Rockford on 14/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#include "EInventory.h"
#include "ImageBank.h"
#include "MyGame.h"
#include "EPointer.h"
#include "GlobalBank.h"
#include "FontBank.h"
#include "global.h"

#define INVENTORY_BUFFERLEN 1024*4

KList<EInventoryItem> EInventoryItem::g_ListInventoryItem;

/***************************************************/
/* EInventory                                      */
/***************************************************/

/* Constructeur */
EInventory::EInventory(ESceneDirector *lpSceneDirector)
{
   int x,y;
   
   _lpSceneDirector = lpSceneDirector;
   
   y = INVENTORY_Y;
   x = INVENTORY_X;
   for (int n = 0; n < INVENTORY_CELLMAX; n++) {
      _InventoryCells[n] = new EInventoryCell();
      _InventoryCells[n]->setPos(x, y);
      _InventoryCells[n]->setParent(this);
      x += INVENTORY_CELLWIDTH + INVENTORY_CELLSPACING;
   }
   
   _lpTravellingItem = NULL;
   
   _nFirstCell = 0;
   memset(_szUniverseName, 0, 256);
   
   // Effet de particules
   _lpGlow = new SceneObjectParticleEmitter("");
   _lpGlow->SetData("inventoryglow", "creepingsmoke", 0, 0, 0, 0, false, false); 
   _lpGlow->SetCreeping("particles/ui_glitter.cse", "");
   
   Order(true);
}

EInventory::~EInventory()
{
   for (int n = 0; n < INVENTORY_CELLMAX; n++) {
      XDELETE(_InventoryCells[n]);
   }
   EInventoryItem::CleanUp(NULL);
   XDELETE(_lpGlow);
}

void EInventory::ValidateTravellingItem()
{
   if (!_lpTravellingItem) {
      K_LOG("ValidateTravellingItem -> No travellingitem - End ValidateTravellingItem");
      return;
   }
   K_LOG("ValidateTravellingItem -> Create new item _szItemName=%s _nNbPartNeeded=%d",_lpTravellingItem->_szItemName,_lpTravellingItem->_nNbPartNeeded);   
   // On crée un item tout neuf
   AddItem(_lpTravellingItem);

   // Tutorial sur les objets à combiner
   if (TestGlobal("__mode_casual__")) {
      if (_lpTravellingItem->_nNbPartNeeded == -1) {
         if (!TaskResolved("__tuto_combineitem__")) {
            ResolveTask("__tuto_combineitem__");
            int xitem,yitem;
            xitem = _lpTravellingItem->_X + (INVENTORY_CELLWIDTH/2);
            yitem = _lpTravellingItem->_Y;
            if (TestGlobal("__tutorial__")) {
               _lpSceneDirector->getSequencer()->Tutobox(NULL, KStr("GLOBAL_TUTO_COMBINEITEM"), xitem, yitem, -90, 0);
            }
         }
      }
   }
   
   _lpTravellingItem = NULL;
   K_LOG("Travellingitem added - End ValidateTravellingItem");
   _lpSceneDirector->UpdateInventoryButtons();
}

void EInventory::setTravellingItem(EInventoryItem *lpItem)
{
   K_LOG("EInventory::setTravellingItem(EInventoryItem lpItem=%s)",lpItem->_szItemName);
   // Il y avait déjà un objet en cours de transfert ?
   if (_lpTravellingItem) {
      // On valide l'inventaire pour l'intégrer
      _lpSceneDirector->EndTravellingItem();
   }
   _lpTravellingItem = lpItem;
}

void EInventory::Order(bool bLock)
{
   if (!bLock) {
      // Déplace les éléments si l'inventaire n'est plus plein
      if (getCount(_szUniverseName) - _nFirstCell < INVENTORY_CELLMAX) {
         _nFirstCell = getCount(_szUniverseName) - INVENTORY_CELLMAX;
         if (_nFirstCell < 0) {
            _nFirstCell = 0;
         }
         SCENEDIRECTOR->UpdateInventoryButtons();
      }
   }
   // Reset
   for (int n = 0; n < INVENTORY_CELLMAX; n++) {
      _InventoryCells[n]->setItem(NULL);
      _InventoryCells[n]->setOrder(_nFirstCell+n);
   }
   // Chaine pour dumper l'inventaire dans le profil
   char szDump[INVENTORY_BUFFERLEN];
   memset(szDump, 0, INVENTORY_BUFFERLEN);
   // Positionne les items dans les cellules
   if (_nFirstCell != -1) {
      // Se positionne sur le 1er item demandé puis positionne les items dans les cellules
      int n = 0;
      int nCell = 0;
      EInventoryItem *o;
      bool bGotNext = EInventoryItem::g_ListInventoryItem.getHead (o);
      if (bGotNext) {
         while (bGotNext) {
            // Ajout au dump
            KStringCat(szDump, o->_szItemName,INVENTORY_BUFFERLEN);
            KStringCat(szDump, ",",INVENTORY_BUFFERLEN);
            KStringCat(szDump, o->_szUniverseName,INVENTORY_BUFFERLEN);
            KStringCat(szDump, ",",INVENTORY_BUFFERLEN);
            KStringCat(szDump, itos(o->_nNbPartNeeded).c_str(),INVENTORY_BUFFERLEN);
            KStringCat(szDump, ",",INVENTORY_BUFFERLEN);
            KStringCat(szDump, itos(o->_nNbPartCurrent).c_str(),INVENTORY_BUFFERLEN);
            KStringCat(szDump, ";",INVENTORY_BUFFERLEN);
            // Visible dans l'inventaire
            if (strcmp(o->_szUniverseName,_szUniverseName) == 0) {
               if (n >= _nFirstCell) {
                  _InventoryCells[nCell]->setItem(o);
                  // L'item est nouvellement instancié, on le fait clignoter si c un objet à combiner
                  if (o->_bNew) {
                     o->_bNew = false;
                     _InventoryCells[nCell]->_bFirstDisplay = true;
                     _InventoryCells[nCell]->_nNbFirstDiplayBlink = 0;
                  }
                  // Positionne l'image dans la cellule
                  int x,y;
                  x = _InventoryCells[nCell]->getPosX();
                  x += INVENTORY_CELLWIDTH/2;
                  x -= (int) o->_lpGraphic->getWidth()/2;
                  y = _InventoryCells[nCell]->getPosY();
                  y += INVENTORY_CELLHEIGHT/2;
                  y -= (int) o->_lpGraphic->getHeight()/2;
                  o->setPos((float) x, (float) y);
                  nCell++;
                  if (nCell >= INVENTORY_CELLMAX)
                     break;
               }
               n++;
            }
            bGotNext = EInventoryItem::g_ListInventoryItem.getNext (o);
         }
      }
   }
   // Sauvegarde du dump dans le profil
   SetGlobal("__inventory__", szDump);
   K_LOG("Inventory order:%s len=%d",szDump,strlen(szDump));
}

void EInventory::RestoreFromProfile()
{
   // Tokenize par ";"
   char *szItem;
   char *szSavePtr = NULL;
   char *szAttribut;
   char szBuf[INVENTORY_BUFFERLEN];
   char szItemName[256];
   char szUniverseName[256];
   int nNbPartNeeded = 0;
   int nNbPartCurrent = 0;
   int nStep;
   
   EInventoryItem::CleanUp(NULL);
   
   memset(szBuf, 0, INVENTORY_BUFFERLEN);
   memset(szItemName, 0, 256);
   memset(szUniverseName, 0, 256);
   KStringCpy(szBuf, GetStrGlobal("__inventory__",""),INVENTORY_BUFFERLEN);
   szItem = strtok_r(szBuf, ";", &szSavePtr);
   while (szItem) {
      // Tokenize par ","
      nStep = 1;
      szAttribut = strtok(szItem, ",");
      while (szAttribut) {
         // Stocke la valeur en fonction
         switch (nStep) {
            case 1:
               strcpy(szItemName, szAttribut);
               break;
            case 2:
               strcpy(szUniverseName, szAttribut);
               break;
            case 3:
               nNbPartNeeded = atoi(szAttribut);
               break;
            case 4:
               nNbPartCurrent = atoi(szAttribut);
               break;
            default:
               break;
         }
         nStep++;
         // Prochain attribut
         szAttribut = strtok(NULL, ",");
      }
      // Item lu en totalité ? On le met dans l'inventaire
      if (nStep == 5) {
         for (int n=0; n<nNbPartCurrent; n++) {
            EInventoryItem *item = new EInventoryItem(szUniverseName, szItemName, nNbPartNeeded, 0);
            AddItem(item, true);
         }
      }
      // Prochain item
      szItem = strtok_r(NULL, ";", &szSavePtr);
   }
}

bool EInventory::canScrollLeft()
{
   if (_nFirstCell>0) {
      K_LOG("Can scroll left");
   }
   return _nFirstCell>0;
}

bool EInventory::canScrollRight()
{
   return (_nFirstCell + INVENTORY_CELLMAX) < getCount(_szUniverseName);
}

void EInventory::ScrollLeft()
{
   if (_nFirstCell > 0)
      _nFirstCell--;
   Order();
}

void EInventory::ScrollRight()
{
   _nFirstCell++;
   Order();
}

bool EInventory::Check()
{
   bool bResult = false;
   // Demande à chaque cellule si elle a traité la souris
   for (int n = 0; n < INVENTORY_CELLMAX; n++) {
      if (_InventoryCells[n]->Check()) {
         bResult = true;
      }
   }
   // Est-ce qu'un bouton a été cliqué
   return bResult;
}

void EInventory::Draw(float fFade)
{  
   _lpGlow->Logic();
   _lpGlow->Draw();
   
   // Dessine les cellules
   for (int n = 0; n < INVENTORY_CELLMAX; n++) {
      if (_InventoryCells[n]->getItem() && _lpSceneDirector->getHoldItem())
      {
         if (strcmp(_lpSceneDirector->getHoldItem()->_szItemName,_InventoryCells[n]->getItem()->_szItemName)==0) {
            _InventoryCells[n]->Draw(fFade/2);
         }
         else {
            _InventoryCells[n]->Draw(fFade);
         }
      }
      else {
         _InventoryCells[n]->Draw(fFade);
      }
   }
   
//   char szDebug[256];
//   sprintf(szDebug, "fs:%d / tot:%d",_nFirstCell,getCount(_szUniverseName));
//   KTextFace *lpFont = EFontBank::getFont("tahoma.ttf", 12);
//   lpFont->drawStringFromRight(szDebug, 1024, 700);
}

void EInventory::HoldItem(EInventoryItem *lpItem)
{
   if (lpItem) {
      _lpSceneDirector->HoldItem(lpItem->_szItemName);
   }
   else {
      _lpSceneDirector->resetHoldItem();
   }
}

EInventoryItem *EInventory::getHoldItem()
{
   return _lpSceneDirector->getHoldItem();
}

bool EInventory::CombineItem(const char *szItemNameInv, const char *szItemNameHold)
{
   bool bSuccess;
   bSuccess = _lpSceneDirector->CombineItem(szItemNameInv,szItemNameHold);
   return bSuccess;
}

EInventoryItem *EInventory::getItemFromInventory(const char *szItemName)
{
   EInventoryItem *lpRes = NULL;
   EInventoryItem *o = NULL;
   bool bGotNext = EInventoryItem::g_ListInventoryItem.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (strcmp(o->_szItemName, szItemName) == 0) {
            lpRes = o;
            break;
         }
         bGotNext = EInventoryItem::g_ListInventoryItem.getNext (o);
      }
   }
   return lpRes;
}

void EInventory::getFreeCell(const char *szItemName, int &x, int &y)
{
   int nFreeCellPos;

   // On regarde si on n'a pas déjà cet objet dans l'inventaire
   // et si il n'est pas en plusieurs morceaux
   EInventoryItem *lpItem = NULL;
   lpItem = getItemFromInventory(szItemName);
   
   if (lpItem != NULL)
   {
      // Oui, on a bien cet objet et il est incomplet
      if (lpItem->_nNbPartCurrent < lpItem->_nNbPartNeeded)
      {
         // On le rend visible quitte à scroller l'inventaire
         // et on renvoie ses coordonnées
         setItemVisible(szItemName);
         nFreeCellPos = getItemCell(szItemName);
         x = _InventoryCells[nFreeCellPos]->getPosX() + INVENTORY_CELLWIDTH/2;
         y = _InventoryCells[nFreeCellPos]->getPosY() + INVENTORY_CELLHEIGHT/2;
         return;
      }
   }
   
   // Combien y a t'il d'objets dans l'inventaire
   int nNbInv = getCount(_szUniverseName);

   // Si il y en a moins que de cellule, c'est facile
   if (nNbInv < INVENTORY_CELLMAX) {
      nFreeCellPos = nNbInv-_nFirstCell;
   }
   // Si l'inventaire déborde, il faut décaller les items visibles pour libérer une cellule
   else {
      // On se positionne sur la fin de la liste d'objet, en laissant une case vide
      _nFirstCell = nNbInv - INVENTORY_CELLMAX + 1;
      Order(true);
      nFreeCellPos = INVENTORY_CELLMAX - 1;
   }
   
   x = _InventoryCells[nFreeCellPos]->getPosX() + INVENTORY_CELLWIDTH/2;
   y = _InventoryCells[nFreeCellPos]->getPosY() + INVENTORY_CELLHEIGHT/2;
}

int EInventory::getCount(const char *szUniverseName)
{
   int nb = 0;
   EInventoryItem *o = NULL;
   bool bGotNext = EInventoryItem::g_ListInventoryItem.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (strcmp(o->_szUniverseName,_szUniverseName) == 0) {
            nb++;
         }
         bGotNext = EInventoryItem::g_ListInventoryItem.getNext (o);
      }
   }
   return nb;
}

void EInventory::setItemVisible(const char *szItemName)
{
   // L'item est déjà visible
   if (getItemCell(szItemName) >= 0)
      return;

   // Compte la position de l'item dans la liste globale
   bool bFind = false;
   int nPosInList = 0;
   EInventoryItem *o = NULL;
   bool bGotNext = EInventoryItem::g_ListInventoryItem.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (strcmp(o->_szItemName, szItemName) == 0) {
            bFind = true;
            break;
         }
         if (strcmp(o->_szUniverseName, _szUniverseName) == 0) {
            nPosInList++;
         }
         bGotNext = EInventoryItem::g_ListInventoryItem.getNext (o);
      }
   }

   // Pas de raison qu'on demande à rendre visible un item qui n'est pas dans l'inventaire
   KAssert(bFind == true);
   if (!bFind) return;

   // On scrolle jusqu'à que cet item soit dans les items visibles
   // Scroll vers les éléments de droite
   if (nPosInList > INVENTORY_CELLMAX)
   {
      _nFirstCell = (nPosInList - INVENTORY_CELLMAX) + 1;
   }
   else
   if (nPosInList < INVENTORY_CELLMAX)
   {
      _nFirstCell = nPosInList;
   }
   Order(true);
}

int EInventory::getItemCell(const char *szItemName)
{
   bool bFind = false;
   int n;
   for (n = 0; n < INVENTORY_CELLMAX; n++) {
      if (_InventoryCells[n]->getItem()) {
         if (strcmp(_InventoryCells[n]->getItem()->_szItemName, szItemName) == 0) {
            bFind = true;
            break;
         }
      }
   }
   if (bFind)
   {
      return n;
   }
   else {
      return -1;
   }
}

void EInventory::BlingItem(const char *szItemName)
{
   EInventoryItem *lpItemAlready = NULL;
   lpItemAlready = getItemFromInventory(szItemName);
   if (lpItemAlready) {
      setItemVisible(szItemName);
      ESoundBank::getSound("success")->playSample();
      int x,y;
      int nCell;
      nCell = getItemCell(lpItemAlready->_szItemName);
      if (nCell != -1) {
         KAssert(_InventoryCells[nCell]->getItem() != NULL);
         x = _InventoryCells[nCell]->getPosX() + INVENTORY_CELLWIDTH/2;
         y = _InventoryCells[nCell]->getPosY() + INVENTORY_CELLHEIGHT/2;
         _lpGlow->setPos(x,y);
         _lpGlow->Start();
      }
      else {
         K_LOG("BlingItem failed for %s",szItemName);
      }
   }
}

void EInventory::AddItem(char *szUniverseName, char *szItemName)
{
   EInventoryItem *item = new EInventoryItem(szUniverseName, szItemName, 1, 1);
   AddItem(item);
   setItemVisible(szItemName);
//   BlingItem(szItemName);
}

void EInventory::ReplaceItem(const char *szFromItemName, const char *szToItemName)
{
   EInventoryItem *oOld;
   EInventoryItem *oNew;
   int nCell;

   setItemVisible(szFromItemName);
   nCell = getItemCell(szFromItemName);
   if (nCell == -1) {
      K_LOG("EInventory::ReplaceItem Error:%s not found",szFromItemName);
   }
   if (nCell != -1) {
      oOld = getItemFromInventory(szFromItemName);
      KAssert(oOld != NULL);
      if (oOld) {
         oNew = new EInventoryItem(oOld->_szUniverseName, szToItemName, 1, 1);
         _InventoryCells[nCell]->setItem(oNew);
         EInventoryItem::g_ListInventoryItem.addBefore(oOld,oNew);
         DropItem(szFromItemName);
         BlingItem(szToItemName);
      }
   }
   else {
      // On fait à l'ancienne !
      // !!!:dmekersa:20110615 Un bug semble faire que parfois l'item n'est pas visible parmis les 6 cellules de l'inventaire !
      // !!!:dmekersa:20110701 2ème correction car j'utilisais oOld pour récupérer l'univers et sans l'initialiser
      K_LOG("EInventory::ReplaceItem workaround for %s",szFromItemName);
      DropItem(szFromItemName);
      // Ligne supprimée car oOld non initialisé :
      //oNew = new EInventoryItem(oOld->_szUniverseName, szToItemName, 1, 1);
      oNew = new EInventoryItem(_szUniverseName, szToItemName, 1, 1);
      AddItem(oNew);
   }

}

void EInventory::AddItem(EInventoryItem *lpItem, bool preload)
{
#ifdef DEBUG
   K_LOG("AddItem(EInventoryItem *lpItem) start for %s",lpItem->_szItemName);
   if (SCENEDIRECTOR != NULL && SCENEDIRECTOR->GetCurrentEScene() != NULL)
      K_LOG("InventoryItem: additem, scene:%s, object:%s, invimg:%s",
                  SCENEDIRECTOR->GetCurrentEScene()->_strSceneName.c_str(),
                  lpItem->_szItemName,
                  EImageBank::GetCache(lpItem->_lpsimgImage->getGraphic())->GetImgPath());
#endif

   // Ajoute l'item
   EInventoryItem *lpItemAlready = NULL;
   lpItemAlready = getItemFromInventory(lpItem->_szItemName);
   
   if (lpItemAlready != NULL && lpItemAlready->_nNbPartNeeded > 1)
   {
      XDELETE(lpItem);
      // Oui, on a bien cet objet et il est incomplet
      if (lpItemAlready->_nNbPartCurrent < lpItemAlready->_nNbPartNeeded)
      {
         lpItemAlready->_nNbPartCurrent++;
         K_LOG("AddItem _nNbPartCurrent incremented for %s (now is %d)", lpItemAlready->_szItemName, lpItemAlready->_nNbPartCurrent);
         KAssert(lpItemAlready->_nNbPartCurrent <= lpItemAlready->_nNbPartNeeded);
         if (lpItemAlready->_nNbPartCurrent >= lpItemAlready->_nNbPartNeeded)
         {
            K_LOG("AddItem %s is now completed (%d)", lpItemAlready->_szItemName, lpItemAlready->_nNbPartCurrent);
            ESoundBank::getSound("success")->playSample();
            int x,y;
            int nCell;
            nCell = getItemCell(lpItemAlready->_szItemName);
            x = _InventoryCells[nCell]->getPosX() + INVENTORY_CELLWIDTH/2;
            y = _InventoryCells[nCell]->getPosY() + INVENTORY_CELLHEIGHT/2;
            // Effet objet terminé
            // Objet complété ? On en informe le director.
            if (!preload) {
               _lpSceneDirector->ItemIsFull(lpItemAlready->_szItemName);
            }
         }
      }
      else
      {
         // Aucune raison qu'on dépasse le nombre de morceaux
         K_LOG("AddItem(EInventoryItem *lpItem) error: lpItemAlready->_nNbPartCurrent > lpItemAlready->_nNbPartNeeded for %s",lpItem->_szItemName);
         KAssert(false);
      }
   }
   else {
      // Ajout réel dans l'inventaire
      K_LOG("AddItem(EInventoryItem *lpItem) addToTail for %s",lpItem->_szItemName);
      // Reprend l'image originale
      lpItem->initImage();
      EInventoryItem::g_ListInventoryItem.addToTail(lpItem);
   }
   Order();
}

void EInventory::DropItem(const char *szItemName)
{
   bool bFind = false;
   EInventoryItem *o;
   bool bGotNext = EInventoryItem::g_ListInventoryItem.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (strcmp(o->_szItemName, szItemName) == 0 && strcmp(o->_szUniverseName,_szUniverseName) == 0) {
            EInventoryItem::g_ListInventoryItem.remove(o);
            bFind = true;
            delete o;
            break;
         }
         bGotNext = EInventoryItem::g_ListInventoryItem.getNext (o);
      }
   }
   if (!bFind) {
      K_LOG("Not found DropItem(%s) current _szUniverseName=%s",szItemName,_szUniverseName);
   }
   Order();
}

/***************************************************/
/* EInventoryCell                                  */
/***************************************************/

/* Constructeur */
EInventoryCell::EInventoryCell()
{
   /* Kanji */
   _lpGraphics = KPTK::createKGraphic();
   _lpPuzzleGraphic = EImageBank::getImage("arrow_puzzle");
   _lpItem = NULL;
  
   _lpFont = EFontBank::getFont("tahoma.ttf", 15);

   _nOrder = 0;
   _X = 0;
   _Y = 0;
   
   _bOver = false;
   _bPuzzle = false;
   _fPuzzleScale = 0.0f;
   _nPuzzleDirection = 0;
   _bFirstDisplay = true;
   _nNbFirstDiplayBlink = 0;
}

/* Destructeur */
EInventoryCell::~EInventoryCell()
{
   XDELETE(_lpGraphics);
   XDELETE(_lpFont);
}

bool EInventoryCell::Check()
{
   // Est-ce sur la cellule ?
   float x,y;
   x = Mouse.x;
   y = Mouse.y;
   
   if (x >= _X && x <= _X+INVENTORY_CELLWIDTH && y >= _Y && y <= _Y+INVENTORY_CELLHEIGHT) {
      _bOver = true;
      // Item à associer
      if (_lpItem) {
         if (_lpItem->_nNbPartNeeded == -1)
         {
            _bPuzzle = true;
         }
         else {
            _bPuzzle = false;
         }
      }
      else {
         SCENEDIRECTOR->PrepareMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
      }
   }
   else {
      _bOver = false;
   }

   // Cliquée
   if (Mouse.LeftEvent() && _bOver)
   {	 
      // Est-ce qu'il y a un item dans la cellule
      if (_lpItem) {
         // On regarde ce qu'on a déjà en main
         if (_lpInventory->getHoldItem()) {
            // Si ce n'est pas le même item qui est cliqué
            if (strcmp(_lpItem->_szItemName,_lpInventory->getHoldItem()->_szItemName) != 0)
            {
               // On essaye de les combiner, si ça n'a rien donné, on remplace l'item tenu par le nouveau cliqué
               if (_lpInventory->CombineItem(_lpItem->_szItemName,_lpInventory->getHoldItem()->_szItemName) == false)
               {
                  // On informe le parent que cet item va être tenu
                  _lpInventory->HoldItem(_lpItem);
               }
            }
            else {
               _lpInventory->HoldItem(NULL);
            }
         }
         else
         {
            // On informe le parent que cet item va être tenu
            _lpInventory->HoldItem(_lpItem);
         }
      }
      else {
         _lpInventory->HoldItem(NULL);
      }
      Mouse.LeftEventClose();
   }
   
   return _bOver;
}

void EInventoryCell::Draw(float fFade)
{
   _lpFont->setBlitColor(1,1,1,1);
#ifdef DEBUG
   if (KInput::getMiddleButtonState())
   {
//      _lpGraphics->drawRect((float)_X, (float)_Y, (float)_X + INVENTORY_CELLWIDTH, (float)_Y + INVENTORY_CELLHEIGHT, 46/255, 30/255, 7/255, fFade);
      // Affiche le numéro d'ordre (pour Debug)
      char buf[256];
      memset(buf,0,256);
      sprintf(buf,"Order:%d",getOrder());
      _lpFont->drawStringFromLeft(buf,_X,_Y);
   }
#endif   
   if (_lpItem) {
      _lpItem->Draw(fFade);
      // Légende en survol
      if (_bOver)
      {
         const char *sz = KStr(_lpItem->_szItemName);
         _lpGraphics->drawRect(416-30, 627, 416+205+30, 627+26, 0, 0, 0, 0.66f);
         if (sz)
         {
            _lpFont->drawStringCentered(sz, 416, 416+205, 630);
         }
         else {
            _lpFont->drawStringCentered("Unknown item", 416, 416+205, 630);
         }
      }
      if (_bOver || _bFirstDisplay) {
         // Objet à combiner
         if (_lpItem->_nNbPartNeeded == -1) {            
            _lpPuzzleGraphic->blitAlphaRectFx(0, 0, _lpPuzzleGraphic->getHeight(), _lpPuzzleGraphic->getWidth(), (_X+(INVENTORY_CELLWIDTH/2))-(_lpPuzzleGraphic->getWidth()/2), (_Y+(INVENTORY_CELLHEIGHT/2))-(_lpPuzzleGraphic->getHeight()/2),0,1.0f,_fPuzzleScale);
            if (_nPuzzleDirection == 0) {
               _fPuzzleScale += 0.025f;
               if (_fPuzzleScale >= 1.0f) {
                  _fPuzzleScale = 1.0f;
                  _nPuzzleDirection = -1;
               }
            }
            else {
               _fPuzzleScale -= 0.025f;
               if (_fPuzzleScale <= 0.0f) {
                  _fPuzzleScale = 0.0f;
                  _nPuzzleDirection = 0;
                  _nNbFirstDiplayBlink++;
               }
            }
         }
         // La pièce de puzzle clignote plusieurs fois
         if (_bFirstDisplay) {
            if (_nNbFirstDiplayBlink == 3) {
               _bFirstDisplay = false;
            }
         }
      }
      // Item en morceaux
      if (_lpItem->_nNbPartNeeded > 1 && _lpItem->_nNbPartCurrent != _lpItem->_nNbPartNeeded)
      {
         char buf[256];
         memset(buf,0,256);
         sprintf(buf,"%d/%d",_lpItem->_nNbPartCurrent,_lpItem->_nNbPartNeeded);
         _lpGraphics->drawRect(_X+10, _Y+INVENTORY_CELLHEIGHT-_lpFont->getHeightPix(), _X+INVENTORY_CELLWIDTH-10, _Y+INVENTORY_CELLHEIGHT, 0.0f, 0.0f, 0.0f, 0.6f);
         _lpFont->drawStringCentered(buf,_X,_X+INVENTORY_CELLWIDTH,_Y+INVENTORY_CELLHEIGHT-_lpFont->getHeightPix());
      }
      // Erreur ?
      if (_lpItem->_nNbPartNeeded > 1 && _lpItem->_nNbPartCurrent >= _lpItem->_nNbPartNeeded)
      {
         K_LOG("EInventoryCell::Draw - Error: NbPartneed = NbPartCurrent for %s",_lpItem->_szItemName);
      }
   }

}

/***************************************************/
/* EInventoryItem                                  */
/***************************************************/

/* Constructeur */
EInventoryItem::EInventoryItem(const char *szUniverseName, const char *szItemName, int nNbPartNeeded, int nNumPart)
{
   _bNew = true;
   _X = 0;
   _Y = 0;
   _lpGraphic = NULL;
   _lpsimgImage = NULL;
   
   memset(_szUniverseName, 0, 256);
   strcpy(_szUniverseName, szUniverseName);
   
   memset(_szItemName, 0, 256);
   strcpy(_szItemName, szItemName);
   
   // Récupère le visuel de l'objet dans la banque d'objets (todo)
   //_lpsimgImage = new ESmartImage(KGraphic *lpGraphic, float _xx, float _yy, int nNbFrame=1, bool bVisible=true, bool bSmooth=true, int nFadeDuration=0);
   _lpsimgImage = NULL; // Temporaire
   
   _lpFont = EFontBank::getFont("OldNewspaperTypes.ttf", 15);
   
   /* Image correspondante */
   
   // L'objet est en 1 seule partie, pas de soucis on prend l'image du même nom
   // Idem si c'est la représentation du groupe de morceaux (nNumPart = 0)
   if (nNbPartNeeded <= 1 || nNumPart == 0) {
      _lpGraphic = EImageBank::getImage(szItemName);
      _lpsimgImage = new ESmartImage(_lpGraphic,0,0);
      K_LOG("EInventoryItem: image %08x, name %s",_lpGraphic,szItemName);
   }
   // L'objet est en plusieurs parties, on cherche l'image de chaque morceau
   else if (nNbPartNeeded > 0) {
      // On crée un nom d'image de type itemname_index
      char szImageName[256];
      memset(szImageName, 0, 256);
      strcpy(szImageName, szItemName);
      strcat(szImageName, "_");
      strcat(szImageName, itos(nNumPart).c_str());
      // On teste si l'image existe (en muet, pas de message d'erreur si pas trouvée)
      _lpGraphic = EImageBank::getImage(szImageName, "png", true);
      if (_lpGraphic) {
         _lpsimgImage = new ESmartImage(_lpGraphic,0,0);
         K_LOG("EInventoryItem: image %08x, name %s",_lpGraphic,szImageName);
      } else {
         // Par défaut on prend l'item de base comme image
         _lpGraphic = EImageBank::getImage(szItemName);
         _lpsimgImage = new ESmartImage(_lpGraphic,0,0);
         K_LOG("EInventoryItem: image %08x, name %s",_lpGraphic,szItemName);
      }

   }
   if (_lpGraphic)
       EImageBank::KeepInCache(_lpGraphic);
   
   _nNbPartNeeded = nNbPartNeeded;
   _nNbPartCurrent = 1;
}

EInventoryItem::~EInventoryItem()
{
   if (_lpGraphic) {
      K_LOG("~EInventoryItem: release %08x",_lpGraphic);
      EImageBank::NotKeepInCache(_lpGraphic);
      _lpGraphic = NULL;
   }
   XDELETE(_lpsimgImage);
   XDELETE(_lpFont);
}

void EInventoryItem::initImage()
{
   XDELETE(_lpsimgImage);
   if (_lpGraphic) {
      K_LOG("~EInventoryItem: release %08x",_lpGraphic);
      EImageBank::NotKeepInCache(_lpGraphic);
      _lpGraphic = NULL;
   }
   
   _lpGraphic = EImageBank::getImage(_szItemName);
   EImageBank::KeepInCache(_lpGraphic);
   
   _lpsimgImage = new ESmartImage(_lpGraphic,0,0);
}

void EInventoryItem::Logic()
{
   _lpsimgImage->Logic();
}

void EInventoryItem::Draw(float fFade)
{
   // Si l'item est en train de se déplacer à l'écran, on prend la position de sa SmartImage car c'est elle qui gère l'interpolation
   if (_lpsimgImage->isAnimateP2P()) {
      _X = _lpsimgImage->getCurrentX();
      _Y = _lpsimgImage->getCurrentY();
   } else {
      _lpsimgImage->Move(_X, _Y);
   }

   // Pas d'image ? On affiche un texte à la place.
   if (_lpsimgImage->getGraphic() == NULL) {
      _lpFont->setBlitColor(1,1,1,1);
      _lpFont->setBlend(fFade);
      _lpFont->drawStringCentered(_szItemName, (long)_X, (long)_X, (long)_Y);
      _lpFont->setBlend(1);
   }
   else {
      _lpsimgImage->setFixedBlend(fFade);
      _lpsimgImage->Draw();
   }
}

void EInventoryItem::CleanUp(const char *szUniverseName)
{
   // Parcours la liste pour en récupérer les valeurs et la vide
   EInventoryItem *o;
   EInventoryItem *otodelete;
   bool bGotNext = g_ListInventoryItem.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (szUniverseName) {
            if ( strcmp(o->_szUniverseName, szUniverseName) == 0) {
               g_ListInventoryItem.remove(o);
               otodelete = o;
            }
            else {
               otodelete = NULL;
            }
         }
         else {
            g_ListInventoryItem.remove(o);
            otodelete = o;
         }

         bGotNext = g_ListInventoryItem.getNext (o);
         XDELETE(otodelete);
      }
   }
}
