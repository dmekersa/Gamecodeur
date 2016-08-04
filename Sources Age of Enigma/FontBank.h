/*
 *  FontBank.h
 *  enigma
 *
 *  Created by Rockford on 22/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef FONTBANK_H
#define FONTBANK_H

#include "KPTK.h"
#include "global.h"
#include <list>

class EFont;

class EFontBank : public KObjectListable {
public:
   enum FontType {KTEXT,KTRUETEXT};

   // Demande une font à la banque
   static EFont* getFont(const char *szFontName, int nHeight=-1);

   // Destructeur de chaque élement
   static void CleanUp();

   static int GetCount() {return g_ListFont.getCount();}

private:
   friend class EFont;

   static EFont* getTrueTextFont(const char *szFontName, int nHeight);
   static EFont* getBitmapFont(const char *szFontName, int nHeight=0);

   EFontBank();
   ~EFontBank();

   char _szName[256];
   KTextFace *_lpFont;
   int _nHeight;

   EFont *_lpLastMod;
   std::list< EFont * > _lpEFontRef;
   FontType _type;
   
   static bool _bCleanUp;
   static KList<EFontBank> g_ListFont;
};

#endif