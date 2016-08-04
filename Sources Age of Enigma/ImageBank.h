/*
 *  ImageBank.h
 *  enigma
 *
 *  Created by Rockford on 14/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */

#ifndef IMAGEBANK_H
#define IMAGEBANK_H

#include "KPTK.h"
#include "KVideo.h"
#include <list>

class EImageBank : public KObjectListable {
public:
   // Specifique aux images
   KGraphic* getImage() {return _lpImage;}
   static KGraphic* getImage(const char *szImageName, const char *szExt = "png", bool bQuiet=false, bool preload=false, bool bPriorityLoad=false, const char *baseDir="pictures/");
   static bool preLoadAnim(const char *szAnimName, bool bPriorityLoad=false);

   bool UploadImg(bool bQuiet=true);
   bool IsUploaded() {return _bUpLoaded;}
   const char* GetImgPath() { return _szName;}

   // Specifique aux videos
   static KVideo* getVideo(const char *szVideoName, bool loop=false, bool bSeekable=false, bool bVideoPreload=true, bool bQuiet=false);

   static void VideoPlay(KVideo *video);
   static void VideoStop(KVideo *video);

   static void SetVideoStatus(KVideo *video, bool isPlaying);
   static void PauseAllVideos();
   static void ResumeAllVideos();
   static bool IsVideoPlaying(KVideo *video);
   static void ReleaseAllVideos();
   
   // Gestion de la bank
   static EImageBank* GetCache(KGraphic *img);
   static EImageBank* GetCache(KVideo *video);
   static EImageBank* GetCache(const char *szObjRef);
   
   static bool IsInCache(const char *szObjRef);
   static void KeepInCache(KGraphic *img);
   static void NotKeepInCache(KGraphic *img);
   static void PurgeCache();
   static void CleanBankNeded(bool enable=true);

   // gestion video et images
   static void Release(KGraphic *);
   static void CleanUp();
   static void NormName(const char *szRepName, const char *szImgName, const char *szExt, char *dst);
   static void showFileError (const char *lpszFileName);

   static int GetCount() { return g_ListImage.getCount();}
   
   static void LogAll();
private:
   friend class Loader;

   EImageBank(const char* szImageName);
   ~EImageBank();

   bool LoadImage(const char *szImageName, bool bPreload=false,bool bPriorityLoad=false); 
   bool LoadVideo(const char *szVideoName, bool loop = true, bool bSeekable=false, bool bVideoPreload=true); 

   static bool ImageExist(const char* szNormedImageName);
   bool CouldBeDrop();

   char _szName[K_MAXPATH];
   KGraphic *_lpImage;
   KVideo *_lpVideo;
   KVideo *_lpVideoMask;

  // video status
   bool _bVideoPlaying;
   static int _nVideoQuality;

   int _nTry;
   int _nKeepInCache;
   bool _bLoaded;
   bool _bUpLoaded;
   int _nLastSceneInd;
   KSysLock _objLock;
   bool _bPriorityLoad;

   // Liste
   static KSysLock _listLock;
   static KList<EImageBank> g_ListImage;
   static bool _bPurgeEnabled;
};

#endif