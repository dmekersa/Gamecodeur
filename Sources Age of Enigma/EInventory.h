/*
 *  EInventory.h
 *  enigma
 *
 *  Created by Rockford on 14/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef EINVENTORY_H
#define EINVENTORY_H

#include "KPTK.h"
#include "ESmartImage.h"
#include "ESceneDirector.h"
#include "ESceneObject.h"

/***** Constantes *****/
#define INVENTORY_CELLMAX 6
#define INVENTORY_X  309
#define INVENTORY_Y  681
#define INVENTORY_CELLWIDTH   71
#define INVENTORY_CELLHEIGHT  71
#define INVENTORY_CELLSPACING 2

// ---------------------------------------------------
/* NOTE : comment créer un objet en plusieurs morceaux

 xxxxx = nom de l'objet normé, exemple hell_item_eyeball
 
 1) Les images
    - pour la scène : pas de nom particulier
    - Pour l'inventaire, en morceaux : inv_xxxxx_part
                         complet     : inv_xxxxx_full
    - Pour le trajet vers l'inventaire : inv_xxxxx_part_1
                                         inv_xxxxx_part_2 ...
 2) Dans le .scn
    - name:xxxx[1]
 3) Dans la scène :
    (Init)
    SetupItem("xxxx[x]");
    (Objectclicked)
    if ( strncmp(szObjectName, "xxxxx[", strlen("xxxxx[")) == 0 )
    {
       PickupMultiple(szObjectName, "inv_xxxxx_part", {nPartNeeded});
    }
 4) Dans le director (dans ItemIsFull)
    if (strcmp(szItemName, "inv_xxxxx_part") == 0)
    {
       DropItem(szItemName);
       _lpInventory->AddItem({univers},"inv_xxxxx_full");
    }
 
*/// -------------------------------------------------

/****************************************************/
/* Représente visuellement une cellule d'inventaire */
/****************************************************/

class EInventoryItem;
class EInventory;

class EInventoryCell {
public:
   /* Conctructeur */
   EInventoryCell();
   ~EInventoryCell();

   /** 3 steps rendering **/
   bool Check ();
   void Logic ();
   void Draw (float fFade);
   
   EInventoryItem *getItem() {
      return _lpItem;
   }
   void setItem(EInventoryItem *o) {
      _lpItem = o;
   }
   
   void setPos(int x, int y)
   {
      _X = x;
      _Y = y;
   }
   
   void setParent(EInventory *lp)
   {
      _lpInventory = lp;
   }
   
   int getPosX() {
      return _X;
   }

   int getPosY() {
      return _Y;
   }

   void setOrder(int n) {
      _nOrder = n;
   }
   int getOrder() {
      return _nOrder;
   }
   void resetOver() {
      _bOver = false;
   }

   bool _bFirstDisplay;
   int _nNbFirstDiplayBlink;
   
private:
   /* parent */
   EInventory *_lpInventory;
   EInventoryItem *_lpItem;
   KGraphic *_lpGraphics;
   KGraphic *_lpPuzzleGraphic;
   float _fPuzzleScale;
   short _nPuzzleDirection;
   KTextFace *_lpFont;
   
   int _X;
   int _Y;
   int _nOrder;
   bool _bOver;
   bool _bPuzzle;
};

/*******************************************************************/
/* Représente visuellement et conceptuellement l'inventaire du jeu */
/*******************************************************************/

class EInventory {
public:
   /* Constructeur */
   EInventory(ESceneDirector *lpSceneDirector);
   ~EInventory();
   
   /** 3 steps rendering **/
   bool Check ();
   void Logic ();
   void Draw (float fFade);
   
   /* Positionne les items dans l'inventaire */
   void Order(bool bLock = false);
   /* Restaure l'inventaire depuis une sauvegarde */
   void RestoreFromProfile();
   
   /* Filtre l'inventaire en fonction de l'univers */
   void SetUniverse(const char *szUniversName)
   {
      if (strcmp(szUniversName, _szUniverseName) != 0) {
         strcpy(_szUniverseName, szUniversName);
         _nFirstCell = 0;
         Order();
      }
   }
   // Compte le nombre d'item dans l'inventaire (pour un univers donné)
   int getCount(const char *szUniverseName);

   /* Possibilités de scroll */
   bool canScrollLeft();
   bool canScrollRight();
   void ScrollLeft();
   void ScrollRight();
   
   /* Interraction avec l'inventaire */
   void HoldItem(EInventoryItem *lpItem);
   EInventoryItem *getHoldItem();
   // Tente de combiner 2 items, renvoie false si la combinaison n'a rien donné
   bool CombineItem(const char *szItemNameInv, const char *szItemNameHold);
   
   // Crée une cellule libre et retourne sa position (centre)
   void getFreeCell(const char *szItemName, int &x, int &y);

   // Retourne l'item si il existe déjà à ce nom
   EInventoryItem *getItemFromInventory(const char *szItemName);

   // S'assure que l'item portant ce nom soit visible dans l'inventaire
   void setItemVisible(const char *szItemName);

   // Retourne l'index de la cellule affichant l'item portant ce nom
   int getItemCell(const char *szItemName);

   // Ajoute un item à l'inventaire
   void AddItem(EInventoryItem *lpItem, bool preload=false);
   void AddItem(char *szUniverseName, char *szItemName);
   void ReplaceItem(const char *szFromItemName, const char *szToItemName);
   
   // Fait blinger un item de l'inventaire
   void BlingItem(const char *szItemName);
   
   // Enlève un item de l'inventaire
   void DropItem(const char *szItemName);
   
   // Stocke une référence sur l'objet en cours de ramassage
   void setTravellingItem(EInventoryItem *lpItem);
   EInventoryItem *getTravellingItem() {
      return _lpTravellingItem;
   }
   
   // Valide l'inventaire : intègre l'objet en cours de ramassage
   void ValidateTravellingItem();
   
private:
   // Parent
   ESceneDirector *_lpSceneDirector;
   
   // Les cellules visibles
   EInventoryCell *_InventoryCells[INVENTORY_CELLMAX];
   
   // L'objet en cours de ramassage
   EInventoryItem *_lpTravellingItem;

   // Effet de particules
   SceneObjectParticleEmitter *_lpGlow;
      
   // La première cellule visible (base 0)
   //   -> c'est l'item de g_ListInventoryItem affiché en premier
   //   -> contient -1 si l'inventaire est vide
   int _nFirstCell;
   char _szUniverseName[256];
   
   KGraphic *_lpGraphics;
};

/*****************************************************/
/* Représente un objet collectable dans l'inventaire */
/*****************************************************/

class EInventoryItem : public KObjectListable {
public:
   /* Constructeur et destructeur */
   EInventoryItem(const char *szUniverseName, const char *szItemName, int nNbPartNeeded, int nNumPart);
   ~EInventoryItem();

   void Logic();
   void Draw (float fFade);
   void setPos(float x, float y, bool bCentered=false)
   {
      if (bCentered) {
         _X = x - _lpsimgImage->getWidth()/2;
         _Y = y - _lpsimgImage->getHeight()/2;
      }
      else {
         _X = x;
         _Y = y;
      }
      _lpsimgImage->Move(_X,_Y);
   }

   bool _bNew;
   float _X;
   float _Y;
   int _nNbPartCurrent;
   int _nNbPartNeeded;
   KGraphic *_lpGraphic;
   ESmartImage *_lpsimgImage;
   KTextFace *_lpFont;
   char _szUniverseName[256];
   char _szItemName[256];
   
   // Liste
	static KList<EInventoryItem> g_ListInventoryItem;
   
   // Réinitialise l'image
   void initImage();
      
   // Destructeur de chaque item
   static void CleanUp(const char *szUniverseName);   
private:
};

#endif