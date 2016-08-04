/*
 *  FontBank.cpp
 *  enigma
 *
 *  Created by Rockford on 22/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */
#include "FontModeDefs.h"

#include "FontBank.h"
#include "KPTK.h"
#include "MyGame.h"
#include "EFont.h"

EFontBank::EFontBank(): _lpFont(NULL),_nHeight(0),_lpLastMod(NULL), _type(KTRUETEXT) {
   memset(_szName,0,sizeof(_szName));
}
EFontBank::~EFontBank() {
   g_ListFont.remove(this);
   XDELETE(_lpFont);
   if (_lpEFontRef.size() >0) {
      EFont *todelete;
      for (std::list< EFont *>::iterator it=_lpEFontRef.begin();it!=_lpEFontRef.end();) {
        (*it)->_lpBankInst = NULL;
        (*it)->_lpFont = NULL;
        it++;
      }
   }
   if (_type == KTRUETEXT)
      K_LOG("EFontBank: Drop KTrueText Font ('%s',%d)",_szName,_nHeight);
   else
      K_LOG("EFontBank: Drop KText Font ('%s',%d)",_szName,_nHeight);

}

EFont *EFontBank::getFont(const char *szFontName, int nHeight) {
   K_LOG("EFontBank: getFont('%s',%d)",szFontName,nHeight);
   if (nHeight <= 0)
      return getBitmapFont(szFontName, nHeight);

#ifndef NOTTF
   return getTrueTextFont(szFontName, nHeight);
#else
   std::string bitmapName(szFontName);
   bitmapName = bitmapName.substr(0,bitmapName.size()-4)+itos(nHeight)+"-0";
   
   return getBitmapFont(bitmapName.c_str(),-nHeight-100);
#endif
}

EFont *EFontBank::getTrueTextFont(const char *szFontName, int nHeight)
{
#ifdef NOTTF
   KAssert(false);
#endif
   // Recherche la font dans la banque, si elle n'y est pas on la charge...
   EFontBank *o;
   bool bGotNext = g_ListFont.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         if (strcmp(o->_szName,szFontName) == 0 && o->_nHeight == nHeight) {
            K_LOG("EFontBank: Get From Cache getTrueTextFont ('%s',%d)",szFontName,nHeight);
            // !!!:dmekersa:20110202 On remet le blend à 1 et les couleurs en blanc
            return  new EFont(o);
         }
         bGotNext = g_ListFont.getNext (o);
      }
   }
   
   // A ce stade, la font n'a pas été trouvé
   // On le charge et on l'ajoute à la liste
   EFontBank *lpFontBank;
   // On crée et charge la font en mémoire
   lpFontBank = new EFontBank;
   strcpy(lpFontBank->_szName, szFontName);
   lpFontBank->_nHeight = nHeight;
   EGLL();
   lpFontBank->_lpFont = new KTrueText(KMiscTools::makeFilePath ((std::string("fonts/")+szFontName).c_str()));
   EGLR();
   ((KTrueText *)(lpFontBank->_lpFont))->setHeightPix(nHeight);
   lpFontBank->_type = KTRUETEXT;
   g_ListFont.addToTail(lpFontBank);

#ifdef DUMPTTF
   // Dump bitmap automatique des fonts en mode debug 
   if (nHeight == 101 || nHeight == 33)
      return NULL;
   std::string bitmapName(szFontName);
   bitmapName = "fonts/"+bitmapName.substr(0,bitmapName.size()-4)+itos(nHeight)+"-0";
   ((KTrueText *)lpFontBank->_lpFont)->saveFontTableAndBitmap((bitmapName+".txt").c_str(),(bitmapName+".png").c_str(),K_IMAGE_PNG);
#endif

   K_LOG("getTrueTextFont: Create Font getTrueTextFont('%s',%d)",szFontName,nHeight);
   return new EFont(lpFontBank);
}

EFont *EFontBank::getBitmapFont(const char *szFontName, int nHeight)
{
   char digit[] = "1234567890";
   const char *tmp = szFontName + strcspn(szFontName,digit);
   int size = atoi(tmp);
   KAssert(size>0);
   
   EFontBank *o;
   bool bGotNext = g_ListFont.getHead (o);
   
   if (bGotNext) {
      while (bGotNext) {
         if (strcmp(o->_szName,szFontName) == 0) {
            K_LOG("EFontBank: Get From Cache getBitmapFont('%s',%d)",szFontName,nHeight);
            return new EFont(o);
         }
         bGotNext = g_ListFont.getNext (o);
      }
   }
   
   // A ce stade, la font n'a pas été trouvé
   // On le charge et on l'ajoute à la liste
   EFontBank *lpFontBank;
   // On crée et charge la font en mémoire
   lpFontBank = new EFontBank;
   strcpy(lpFontBank->_szName, szFontName);
   lpFontBank->_nHeight = size;
   
   // Nom du fichier png
   char szFontBitmap[K_MAXPATH];
   memset(szFontBitmap,0,K_MAXPATH);
   strcpy(szFontBitmap,"fonts/");
   strcat(szFontBitmap, szFontName);
   strcat(szFontBitmap, ".png");
   // Nom de la table
   char szFontTable[K_MAXPATH];
   memset(szFontTable,0,K_MAXPATH);
   strcpy(szFontTable,"fonts/");
   strcat(szFontTable, szFontName);
   strcat(szFontTable, ".txt");
   // Chemins complets
   char szFontBitmapF[K_MAXPATH];
   char szFontTableF[K_MAXPATH];
   KStringCpy(szFontBitmapF, KMiscTools::makeFilePath(szFontBitmap),K_MAXPATH);
   KStringCpy(szFontTableF, KMiscTools::makeFilePath(szFontTable), K_MAXPATH);
   
   bool bSuccess;
   EGLL();
   lpFontBank->_lpFont = new KText();
   bSuccess = ((KText *)lpFontBank->_lpFont)->loadFontTableAndBitmap(szFontTableF,szFontBitmapF);
   EGLR();
   if (!bSuccess)
   {
      showFileError("loadFontTableAndBitmap");
   }
   lpFontBank->_type = KTEXT;
   g_ListFont.addToTail(lpFontBank);
   K_LOG("EFontBank: Create Font getBitmapFont('%s',%d)",szFontName,nHeight);
   return new EFont(lpFontBank);
}

// Destructeur de chaque élement
void EFontBank::CleanUp(){
   _bCleanUp = true;
   // Parcours la liste pour en récupérer les valeurs et la vide
   EFontBank *o;
   EFontBank *otodelete;
   bool bGotNext = g_ListFont.getHead (o);
   if (bGotNext) {
      while (bGotNext) {
         otodelete = o;
         bGotNext = g_ListFont.getNext (o);
         XDELETE(otodelete);
      }
   }
}

bool EFontBank::_bCleanUp = false;
KList<EFontBank> EFontBank::g_ListFont;
