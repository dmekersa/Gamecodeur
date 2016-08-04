/*
 *  UserProfileMgt.h
 *  enigma
 *
 *  Created by ZarthArn on 10/09/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */
#pragma once
#include "KGame.h"
#include "GlobalBank.h"
#include <string> 

class EFont;

class Player : public KObjectListable {
public:
	Player(void);
	Player (char * name, bool focus);
	~Player(void);

	void SetName(char *name);
	void SetFocus(bool focus);
	char * GetName(void);
	bool GetFocus(void);

private:
	char Name[15+1];
	bool HasFocus;
};


class KUIMyButton : public KUIButton{
public:
	KUIMyButton(KUIElement *lpParentElem);
	~KUIMyButton(void);

	EFont *lFont;
};


class KUIMyEditField : public KUIEditField{
public:
	// Constructor
	KUIMyEditField(KUIElement *lpParentElem);
	// Destructor
	~KUIMyEditField ();	

	KGraphic *lButtonGraphic;
	unsigned long onInput (unsigned long c); // derives from original method of KUIEditField
	void onTextValidated (void); // derives from original method of KUIEditField

};

class KUIMyText : public KUIText{
public:
	// Constructor
	KUIMyText (KUIElement *lpParentElem);

	// Destructor
	~KUIMyText ();	

   static KUIMyText *isMyText (KUIElement *lpElement);
	void onDown (void); // derives from original method of KUIElement
	void showFocus (void); 
	void hideFocus (void); 
	void setEmptyText (void);
   void blit (float fBlend, KMatrix *lpMatrix);

   EFont *lpFont;
   KGraphic* imageRect;
   bool isSelect;
};

class KUIPlayerListWindow : public KUIImage{
public:
	// constructor
	KUIPlayerListWindow(KUIElement *lpParentElem);
	// destructor
	~KUIPlayerListWindow(void);

	void RefreshUI(Player*  pPlayer);
	void Show(void);
	void Hide(void);

	float		fX;
	float		fY;
	KUILayout   * lpUILayout;
	KUIMyButton * lpUITitleButton;
	KGraphic	* lTitleButtonGraphic;
	KGraphic	* lNewButtonGraphic;
	KGraphic	* lDeleteButtonGraphic;
	KGraphic	* lOkButtonGraphic;
	KUIMyButton * lpUINewButton;
	KUIMyButton * lpUIDeleteButton;
	KUIMyButton * lpUIOkButton;
	KGraphic	* lpWindowGraphic;
	KList<KUIMyText> MyUITextList; // list of displayed players

   EFont *lFont;
};

class KUIDeletePlayerWindow : public KUIImage{
public:
	// constructor
	KUIDeletePlayerWindow(KUIElement *lpParentElem);
	// destructor
	~KUIDeletePlayerWindow(void);

	void Show(void);
	void Hide(void);

	float		fX;
	float		fY;
	KUILayout   * lpUILayout;
	KGraphic	* lTitleButtonGraphic;
	KGraphic	* lPlayerButtonGraphic;
	KGraphic	* lCancelButtonGraphic;
	KGraphic	* lOkButtonGraphic;
	KUIMyButton * lpUITitleButton;
	KUIMyButton * lpUIPlayerButton;
	KUIMyButton * lpUIOkButton;
	KUIMyButton * lpUICancelButton;
	KGraphic	* lpWindowGraphic;

   EFont *lFont;
   EFont *lFont2;
};

class KUINewPlayerWindow : public KUIImage{
public:
	// constructor
	KUINewPlayerWindow(KUIElement *lpParentElem);
	// destructor
	~KUINewPlayerWindow(void);

	void Show(void);
	void Hide(void);

	float		fX;
	float		fY;
	KUILayout	   * lpUILayout;
	KUIMyEditField * lpUIEditField; // display new edit field where to enter player name
	KGraphic	   * lTitleButtonGraphic;
	KGraphic	   * lOkButtonGraphic;
	KGraphic	   * lCancelButtonGraphic;
	KUIMyButton    * lpUITitleButton;
	KUIMyButton    * lpUIOkButton;
	KUIMyButton    * lpUICancelButton;
	KGraphic	   * lpWindowGraphic;

   EFont *lFont;
   EFont *lFont2;
};

class UserProfiles {
public:
	UserProfiles(void);
	~UserProfiles(void);

    static long _lNbPlayers; // current number of players
	static Player * _pPlayer;
    static KList<Player> g_PlayersList; // list of players handled to display/store/restore from settings DB
	static const long _lNbMaxPlayers; // maximum number of players allowed
	static const char EmptyName[8];

	static KUIPlayerListWindow		* MyPlayerListWindow;
	static KUIDeletePlayerWindow	* MyDeletePlayerWindow;
	static KUINewPlayerWindow		* MyNewPlayerWindow;

	static bool BuildPlayerListFromFiles(const char *lpszEntryName, bool bIsFolder, void *lpCallBackData);
	static bool SaveProfile(void);
	static bool RestoreProfile(const char * strExt = ".pro");
	static void HandleNewPlayer(void); // called when button "OK" or "Enter" is hit from New Player window	
	static void BuildFilePath(char * strPath, const char * strExt = ".pro"); // create path to player file to save/restore/create
	static void HandleDeletePlayer(void); // called when button "Delete" is hit from Delete window

   static void CleanUp();
};
