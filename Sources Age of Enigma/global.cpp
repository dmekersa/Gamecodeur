/*
 *  global.cpp
 *  enigma
 *
 *  Created by Rockford on 25/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */
#include "KMiscTools.h"
#include "global.h"
#include "KSysLock.h"

static KSysLock EnigmaGlobalLock;

#ifdef TEST_ARCHIVE
#include "ThreadLoader.h"

void EGLL() {
   if (!Loader::HasArchive())
      return;
   EnigmaGlobalLock.acquire();
}
void EGLR() {
   if (!Loader::HasArchive())
      return;
   EnigmaGlobalLock.release();
}
#else
void EGLL() {
   EnigmaGlobalLock.acquire();
}
void EGLR() {
   EnigmaGlobalLock.release();
}
#endif

// Thread safe but not dst size safe
const char* SafeMakeFilePath (const char *lpszFileName, char *dst) {
   char buff[K_MAXPATH];
   buff[0] ='\0';

   if (*lpszFileName == '/' || *lpszFileName == ':' || *lpszFileName == '\\')
      lpszFileName++;

   if (lpszFileName[0] && lpszFileName[1] == ':') {
      /* X:\Path: absolute pathname. Don't prepend game path */
      KStringCpy (buff, lpszFileName, K_MAXPATH);
   }
   else {
      /* Relative path. Prepend absolute game path. */
      KStringFmt (buff, K_MAXPATH, "%s/%s", KMiscTools::getGameFolder(), lpszFileName);
   }

   KMiscTools::cleanPath (buff);
   KStringCpy(dst,buff,K_MAXPATH);
   return dst;
}

std::string SafeMakeFilePath (const char *lpszFileName) {
   char path[K_MAXPATH];
   memset(path,0,K_MAXPATH);

   /* Skip leading seperator, if any */
   if (*lpszFileName == '/' || *lpszFileName == ':' || *lpszFileName == '\\')
      lpszFileName++;

   if (!lpszFileName[0] || !lpszFileName[1] == ':') {
      KStringFmt(path,K_MAXPATH,"%s/%s",KMiscTools::getGameFolder()
,lpszFileName);
   } else {
      KStringCpy(path,lpszFileName,K_MAXPATH);
   }
   KMiscTools::cleanPath(path);
   return std::string(path);
}

#ifdef WIN32
char * strtok_r (char *s, const char *delim, char **save_ptr)
{
  char *token;

  if (s == NULL)
    s = *save_ptr;

  /* Scan leading delimiters.  */
  s += strspn (s, delim);
  if (*s == '\0')
    {
      *save_ptr = s;
      return NULL;
    }

  /* Find the end of the token.  */
  token = s;
  s = strpbrk (token, delim);
  if (s == NULL)
    /* This token finishes the string.  */
    *save_ptr = token + strlen (token);
  else
    {
      /* Terminate the token and make *SAVE_PTR point past it.  */
      *s = '\0';
      *save_ptr = s + 1;
    }
  return token;
}
#endif


// not thread safe
std::stringstream ss;
std::string itos(int i) {
	ss.str("");
	ss << i;
	return ss.str();
}

int GetIndex(const char *sz)
{
   const char *p1;
   const char *p2;
   char szIdx[256];
   
   memset(szIdx, 0, 256);
   
   p1 = strstr(sz, "[");
   if (p1 == sz || p1 == NULL)
      return -1;
   p1++;
   
   p2 = strstr(sz, "]");
   if (p2 == sz || p2 == NULL)
      return -1;
   
   strncpy(szIdx, p1, p2-p1);
   
   return atoi(szIdx);
}

float GetRandom(float fMin, float fMax)
{
   return fMin+float((fMax-fMin)*rand()/(RAND_MAX + 1.0));
}