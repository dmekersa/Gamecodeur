#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef DEBUGSTUDIO
   // remplacement du assert Kanji
	#include "assert.h"
   #undef KAssert
   #define KAssert(__cond) assert(__cond)

   #ifdef WIN32
      #ifdef DEBUG
         #pragma warning( once : 4244 4018 4305 4244)
      #endif
      #ifdef _VLD
         // Enhanced Memory Leak Detection http://vld.codeplex.com/
         #include "vld.h"
      #else
         // Standart Microsoft Report des memory leak sur new et malloc
         #define _CRTDBG_MAP_ALLOC

         #include <iostream>
         #include <crtdbg.h>
      
         #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
         #define new DEBUG_NEW
      #endif
   #endif
#endif

#include <string>
#include <iostream>
#include <sstream>

#define L10NREP "l10n/"
#define XDELETE(pointer) if((pointer)) \
                           { delete (pointer); pointer = NULL; } 

#ifdef BETA_RELEASE
#define DEBUG_LOG
#define CYPHERED_PAK
//#define TEST_ARCHIVE
#endif

// Fonction de gestion de lock global
void EGLL();   // Acquisition du verou global
void EGLR();   // Relache du verou global

// Thread safe makeFilePath but not dst size safe
const char* SafeMakeFilePath (const char *lpszFileName, char *dst);

// string format helper
std::string itos(int i);

// Récupère l'index dans une chaine de type xxxxxx[index]
int GetIndex(const char *sz);

float GetRandom(float fMin, float fMax);

#ifdef WIN32
#ifndef _STRTOK_R
#define _STRTOK_R
char * strtok_r (char *s, const char *delim, char **save_ptr);
#endif
#endif

#define ___eprintf(...) fprintf (stderr, __VA_ARGS__)

#endif