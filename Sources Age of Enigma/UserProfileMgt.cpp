#include "UserProfileMgt.h"
#include "ImageBank.h"
#include "ThreadLoader.h"
#include "FontBank.h"
#include "EFont.h"
#include "errno.h"

#include "global.h"

Player::Player (char * name, bool focus){
	SetName(name);
	SetFocus(focus);
}


Player::Player (void){
;
}


Player::~Player (void){
	;
}

void Player::SetName(char *name){
	memset(Name, 0, sizeof(Name));
	strcpy(Name, name);
}

void Player::SetFocus(bool focus){
	HasFocus = focus;
}


char * Player::GetName(void){
	return Name;
}

bool Player::GetFocus(void){
	return HasFocus;
}


//*********************************************************************
//*********************************************************************
KUIMyEditField::KUIMyEditField (KUIElement *lpParentElem) : KUIEditField (lpParentElem){
   float w, h;

   lButtonGraphic = EImageBank::getImage("menu_butname.png");
   w = lButtonGraphic->getWidth();
   h = lButtonGraphic->getHeight();
   setBackgroundGraphic (K_UIELEMENT_DISABLED, lButtonGraphic, 0, 0, w, h);
   setBackgroundGraphic (K_UIELEMENT_ENABLED, lButtonGraphic, 0, 0, w, h);
   setBackgroundGraphic (K_UIELEMENT_MOUSEOVER, lButtonGraphic, 0, 0, w, h);
   setBackgroundGraphic (K_UIELEMENT_DOWN, lButtonGraphic, 0, 0, w, h);
   setActiveRect(true, 5, 0, w+5, h); // to be adapted

}
	
KUIMyEditField::~KUIMyEditField (){
//	delete lButtonGraphic;
}

// overload onInput() method in order to filter required characters
unsigned long KUIMyEditField::onInput (unsigned long c){
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '\r' || c == 233 || c == 232) 
		return c; // allow "é" and "è" as well
	else
		return 0;	
}



void KUIMyEditField::onTextValidated (void){
	UserProfiles::HandleNewPlayer();
}

//*********************************************************************
//*********************************************************************
KUIMyButton::KUIMyButton(KUIElement *lpParentElem) : KUIButton(lpParentElem){

   lFont = EFontBank::getFont("NITECLUB.ttf",60);
   setTextFont (lFont);
   setTextFlow(K_UITEXT_CENTER);
   setTextOutlineThickness (2);
   setTextOutlineColor (0, 0, 0, 0.125f);
}

KUIMyButton::~KUIMyButton(void){
   XDELETE(lFont)
}

//*********************************************************************
//*********************************************************************
KUIMyText::KUIMyText (KUIElement *lpParentElem): KUIText (lpParentElem) {

   imageRect = KPTK::createKGraphic();
   isSelect = false;

   enableElementInput(true);
	disableMessages(false);
	addClass("KUIMyText");
	setSize (250, 30);
    setTextFlow(K_UITEXT_CENTER);
	setTextVertAlignMode (K_UIELEMENT_VCENTER);
   lpFont = EFontBank::getFont("tahoma.ttf",20);
	setTextFont (lpFont);
	setTextOutlineThickness (2);
	hideFocus();
}

KUIMyText::~KUIMyText(){
	XDELETE(lpFont);
   XDELETE(imageRect);
}

void KUIMyText::blit (float fBlend, KMatrix *lpMatrix) {
   
   if(isSelect)
      imageRect->drawRect(40, 0, 220, 40, 0.0f, 0.0f, 0.0f, 0.3f);
   KUIText::blit(fBlend, lpMatrix);
}

void KUIMyText::onDown (void){

  
	if (strcmp (getName(), UserProfiles::EmptyName)){
		showFocus(); // highlight that this item got the focus, except if empty 
		UserProfiles::_pPlayer->SetName((char *) getName());
		UserProfiles::_pPlayer->SetFocus(true);
	}
   
};

void KUIMyText::showFocus (void){
   isSelect = true;
	setTextOutlineColor (1, 0, 0, 0.125f);
}; 

void KUIMyText::hideFocus (void){
   isSelect = false;
	setTextOutlineColor (0, 0, 0, 0.125f);
}; 

void KUIMyText::setEmptyText (void){
		setText (""); // to be displayed on screen
		setName (UserProfiles::EmptyName); // reserved Name string}; 
};

//*********************************************************************
//*********************************************************************

void UserProfiles::BuildFilePath(char * strPath, const char * strExt){

   char pFileName[3+15+4+1]; // take "%s/... .pro" into account
   strcpy(pFileName, "%s/");
   strcat(pFileName, _pPlayer->GetName()); // player to manage
   strcat(pFileName, strExt);// file name built according to scheme <playername>.extension
   memset(strPath, 0, K_MAXPATH);
   KStringFmt (strPath, K_MAXPATH, pFileName, KGame::getGame()->getStateFolder());
}

void UserProfiles::HandleDeletePlayer(void){
	Player *p;
	bool bGotNext = g_PlayersList.getHead (p);
	bool bStillSearching = true;
	if (bGotNext) {
		while (bGotNext && bStillSearching) {
			if 	(!strcmp (p->GetName(), _pPlayer->GetName()) ){
				char strPath[K_MAXPATH];
				BuildFilePath(strPath);

				if (remove (strPath) == 0) { // delete successful
					bStillSearching = false; // stop searching
					Player * ToDelete = p;

					bGotNext = g_PlayersList.getNext (p);
					g_PlayersList.remove (ToDelete); // remove player from list
					delete ToDelete;
					_lNbPlayers--;
					_pPlayer->SetName(""); // reset active player
					_pPlayer->SetFocus(false); // reset focus
				}
			}
			if (bStillSearching)
				bGotNext = g_PlayersList.getNext (p);
		}
	}
	g_PlayersList.getHead (p);
  	MyPlayerListWindow->RefreshUI(p); 
	MyPlayerListWindow->Show(); 
	MyDeletePlayerWindow->Hide(); 
};

void UserProfiles::HandleNewPlayer(void){
	Player *p;
	// check if valid name
	char pName[15+1];
	memset(pName, 0 , sizeof(pName));
	if (MyNewPlayerWindow) {
		strcpy(pName, MyNewPlayerWindow->lpUIEditField->getText());
		if (strlen(pName) == 0)
			KMiscTools::messageBox ("Player List", "Missing Name!");
		else {
			bool bGotNext = g_PlayersList.getHead (p);
			bool bIsUnique = true;
			if (bGotNext) {
				while (bGotNext && bIsUnique) {
					if 	(!strcmp (pName, p->GetName()) )
						bIsUnique = false;
					bGotNext = g_PlayersList.getNext (p);
				}
			}
			if (! bIsUnique)
				KMiscTools::messageBox ("Player List", "Name already exists!");
			else {
				if (!_pPlayer)
					_pPlayer = new Player();
				_pPlayer->SetName(pName); // Store current "active" player 
				_pPlayer->SetFocus(true); // set focus

				// player name is valid
				p = new Player(pName, true);
				g_PlayersList.addToTail(p);
				_lNbPlayers++;

				char strPath[K_MAXPATH];
				BuildFilePath(strPath);

				FILE* f;
				f = fopen (KMiscTools::makeFilePath (strPath), "w");
				if (f)
					fclose(f);

				MyPlayerListWindow->RefreshUI(_pPlayer);
				MyPlayerListWindow->Show();
				MyNewPlayerWindow->Hide();
			}
		}
	}
};

bool UserProfiles::BuildPlayerListFromFiles(const char *lpszEntryName, bool bIsFolder, void *lpCallBackData){
	const char *strExtension = ".pro";
	size_t lExtension = strlen(strExtension);
	char *strFound;
	char strName[15+1];
   
   strFound = (char *) strstr(lpszEntryName, strExtension); // matching extension? 
	if (strFound){ 
		if (strlen( const_cast<char*> (strFound) ) == lExtension){ // yes!
			size_t lName = strlen(lpszEntryName) - lExtension; // name length 
			memset (strName, 0, 15+1);
			strncpy(strName, lpszEntryName, lName); // keep only name without extension
			Player *p = new Player(strName, FALSE); 
			g_PlayersList.addToTail(p); // add to list of players
			_lNbPlayers++;
		}
	}
	return true;
}

void UserProfiles::CleanUp() {
   Player *o;
   Player *otodelete;
   bool bGotNext = g_PlayersList.getHead(o);

   while (bGotNext) {
      otodelete = o;
      g_PlayersList.remove(o);
      bGotNext = g_PlayersList.getNext(o);
      XDELETE(otodelete);
   }
}


int removeFile(const char *szPath,const char *fileName,int line) {
   int res = remove(szPath);
   if (res == 0) {
      K_LOG("%s:%d delete file:%s" ,fileName,line, szPath);
   } else {
      res = errno;
      K_LOG("%s:%d unable to delete file:%s, errno:%s",fileName,line, szPath,strerror(res));
   }
   return res;
}
int renameFile(const char *szOldName, const char *szNewName,const char *fileName,int line) {
   int res = rename(szOldName, szNewName);
   if (res == 0) {
      K_LOG("%s:%d move %s to %s",fileName,line,szOldName, szNewName);
   } else {
      res = errno; 
      K_LOG("%s:%d unable to move %s to %s. errno:%s",fileName,line,szOldName, szNewName,strerror(res));
   }
   return res;
}
bool UserProfiles::SaveProfile(void){
   
   char strPathBck[K_MAXPATH];
   char strPath[K_MAXPATH];
   char strPathNew[K_MAXPATH];
   FILE *f=NULL;
   BuildFilePath(strPathBck,".bak");
   BuildFilePath(strPath);
   BuildFilePath(strPathNew,".new");

   K_LOG("SaveProfile(%s)",strPathNew);
   f = fopen (KMiscTools::makeFilePath (strPathNew), "w");
   if (f) {
	   EGlobalBank *currentBankItem;
	   char FieldToStore[256+1+1024*4+1+1+1]; // 256 + 1 + 1024*4 + 1 + 1 + 1
	   bool bGotNext = EGlobalBank::g_ListGlobal.getHead (currentBankItem);
      
      fprintf(f,"__player_name__ %s\n",_pPlayer->GetName());
      if (bGotNext) {
			   while (bGotNext) {
				   memset(FieldToStore, 0, 256+1+1024*4+1+1+1);
				   strcpy(FieldToStore, (char *)currentBankItem->_szName);
				   strcat(FieldToStore, " "); // space as separator

				   if (strlen(currentBankItem->_szValue) == 0)
					   strcat(FieldToStore, "*"); // asterisk used to identify NULL _szValue
				   else
					   strcat(FieldToStore, (char *) currentBankItem->_szValue);

				   strcat(FieldToStore, " "); // space as separator
				   
				   if (currentBankItem->_bTask) {
						strncat(FieldToStore, "1", 1);
				   }
				   else {
						strncat(FieldToStore, "0", 1);
				   }
				   fprintf(f, "%s\n", FieldToStore);
				   bGotNext = EGlobalBank::g_ListGlobal.getNext (currentBankItem);
			   }
	   }
      fprintf(f,"__end_save__");
      fflush (f);
	   fclose (f);
      K_LOG("SaveProfile(%s) - Close OK",strPathNew);

      removeFile(strPathBck,__FILE__,__LINE__);
      renameFile(strPath, strPathBck,__FILE__,__LINE__);
      renameFile(strPathNew, strPath,__FILE__,__LINE__);
      return true;
      
      // store a backup of current .pro file with a .bak extension
      // !!!dmekersa:20110708 On n'a jamais eu besoin de .bak donc je vire ce code risquŽ
      f = fopen (KMiscTools::makeFilePath (strPath), "r");
      if (f) {
         fclose(f);
         BuildFilePath(strPathNew, ".bak");

         // rename peut echouer si le .bak existe deja, on supprime le .bak avant de renommer le .pro
         // peut echouer si n'existe pas
         remove(strPathNew);
         int i = rename(strPath, strPathNew);

         KAssert(i == 0);
      }

	   return true;
   }
   else
   {
      K_LOG("SaveProfile(%s) - Open Failed",strPath);
	   return false;
   }
}

bool UserProfiles::RestoreProfile(const char * strExt){
   bool bSuccess = false;
   int res;
   char strPath[K_MAXPATH];
   FILE *f=NULL;

   BuildFilePath(strPath,strExt);
 
   // Gestion de la sequence de restauration "securisé"
   if (strcmp(strExt,".pro") == 0) {
      char strPathNew[K_MAXPATH];
      BuildFilePath(strPathNew,".new");

      FILE *f = fopen(strPathNew,"r");
      if (f != NULL) {
         // Un fichier de sauvegarde .new existe et n'a pas été éffacé.
         K_LOG("RestoreProfile: save with '.new' extension exist. Try to restore from %s",strPathNew);
         fclose(f);
         bSuccess = RestoreProfile(".new");
         
         if (bSuccess) {
            // Le fichier ".new" a correctement été restauré. décalage de tout les fichiers
            char strPathBck[K_MAXPATH];
            BuildFilePath(strPathBck,".bak");

            // move .pro to .bak
            removeFile(strPathBck,__FILE__,__LINE__);
            renameFile(strPath, strPathBck,__FILE__,__LINE__);
            renameFile(strPathNew, strPath,__FILE__,__LINE__);
            return true;
         } else {
            // remove .new
            res = removeFile(strPathNew,__FILE__,__LINE__);
            K_LOG("RestoreProfile: unable to load profile from %s, file removed.",strPathNew);
         }
      }
   }
   
   // Restore effectif du fichier
   EGlobalBank::CleanUp();

   // Retrieve last player settings
   K_LOG("RestoreProfile: Try to restore from %s",strPath);
   f = fopen (KMiscTools::makeFilePath (strPath), "r"); 
   if (f) {
      char name[256];
      char value[1024*4];
      char task;
      
      memset(name, 0, 256);
      memset(value, 0, 1024*4);
      fscanf(f,"%s",name);
      
      if (strcmp(name,"__player_name__") != 0) {
         bSuccess = false;
      } else {
         fscanf(f,"%[^\n]",name);
         _pPlayer->SetName(name+1);
         while (fscanf(f, "%s %s %c\n", name, value, &task) != EOF) {
            if (strcmp(name,"__end_save__") == 0) {
               bSuccess = true;
               break;
            }
            if (strcmp(value, "*") == 0)
               memset(value, 0, 1024*4);
            bool b = true;
            if (task == '0')
               b = false;
            EGlobalBank::AddStrValue(name, value, b); // add BankItem to EGlobalBank::g_ListGlobal
            memset(name, 0, 256);
            memset(value, 0, 1024*4);
         }
      }
      if (bSuccess) {
         K_LOG("RestoreProfile: profile restored from %s",strPath);
         Loader::LoadScene(GetStrGlobal("__lastscene__","parvis"));
      } else {
         K_LOG("RestoreProfile: invalide profile from %s",strPath);
         EGlobalBank::CleanUp();
      }
      fclose (f);
   }

   if (!bSuccess) {
      if (strcmp(strExt,".pro") == 0) { 
          bSuccess = RestoreProfile(".bak");
          if (!bSuccess) 
             K_LOG("RestoreProfile: Unable to restore profile for player %s",_pPlayer->GetName());
      }
   } else if (strcmp(strExt,".bak") == 0) {
      char strPathPro[K_MAXPATH];
      BuildFilePath(strPathPro,".pro");
      removeFile(strPathPro,__FILE__,__LINE__);

      // copy du fichier
      FILE *bck, *pro;
      bck = fopen(KMiscTools::makeFilePath (strPath), "rb"); 
      pro = fopen(KMiscTools::makeFilePath (strPathPro), "wb");
      KAssert(bck != NULL && pro != NULL);

      while(!feof(bck)) {
         char ch = getc(bck);
         if(ferror(bck)) {
            K_LOG("copy of .bck to .pro failed");
            clearerr(bck);
            break;
         } else {
            if(!feof(bck)) putc(ch, pro);
            if(ferror(pro)) {
               K_LOG("copy of .bck to .pro failed");
               clearerr(pro);
               break;
            }
         }
      }
      fclose(bck);
      fflush(pro);
      fclose(pro);
   }
   return bSuccess;
}


//*********************************************************************
//*********************************************************************
KUIPlayerListWindow::KUIPlayerListWindow(KUIElement *lpParentElem) : KUIImage(lpParentElem){
   
   char strPath[K_MAXPATH];
   UserProfiles::_lNbPlayers = 0; // init nb of players
   float w, h;
   setName("playerlist");

   enumProc lpEnumCallBack = &UserProfiles::BuildPlayerListFromFiles; // set function pointer to callback
   KStringFmt (strPath, K_MAXPATH, "%s/", KGame::getGame()->getStateFolder()); // define root folder
   KMiscTools::enumerateFolder (strPath,lpEnumCallBack, NULL); // call to list files in root folder

   lpWindowGraphic = EImageBank::getImage("menu_playermenu.jpg");

   float WinW = lpWindowGraphic->getWidth();
   float WinH = lpWindowGraphic->getHeight ();
   fX = floorf((1024 - WinW)/2);
   fY = floorf((768 - WinH)/2);
   setPosition (fX, fY);
   setSize (WinW, WinH);
   setGraphic (lpWindowGraphic, 0, 0, WinW, WinH);
   setBlend (0); /* Start out as hidden */
   
   lpUILayout = new KUILayout (this);
   lpUILayout->setPosition (0, 0);
//   lpUILayout->setSize (this->getWidth(), this->getHeight ());
   lpUILayout->setSize (WinW, WinH);
   lpUILayout->setLayoutType (K_UILAYOUT_FIXED);

   lFont = EFontBank::getFont("NITECLUB.TTF",42);

   lTitleButtonGraphic = EImageBank::getImage("menu_butlist.png");	
   lpUITitleButton = new KUIMyButton (lpUILayout);
   lpUITitleButton->setPosition(308, 49);
   w = lTitleButtonGraphic->getWidth();
   h = lTitleButtonGraphic->getHeight();
   lpUITitleButton->setSize(w, h); 
   lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_DISABLED, lTitleButtonGraphic, 0, 0, w, h);  
   lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_ENABLED, lTitleButtonGraphic, 0, 0, w, h);  
   lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, lTitleButtonGraphic, 0, 0, w, h);  
   lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_DOWN, lTitleButtonGraphic, 0, 0, w, h);  
   lpUITitleButton->setText (KStr ("UI_PLAYERLIST"));   
   lpUITitleButton->setTextFont(lFont);
   Player * p;
   KUIMyText *lUIText;

   bool bGotNext = UserProfiles::g_PlayersList.getHead (p); //
   long i = 0;
	
   if (bGotNext) {
	   if (!UserProfiles::_pPlayer)
		   UserProfiles::_pPlayer = new Player(p->GetName(), true); // init to first player in the list by default
	   while (bGotNext) {
		   lUIText = new KUIMyText (lpUILayout);
		   lUIText->setText (p->GetName()); // to be displayed on screen
		   lUIText->setName (p->GetName()); // to be associated to Text UI element
		   lUIText->setPosition(388, (float) (145+ i*43));
		   i++;
		   if (!strcmp(p->GetName(), UserProfiles::_pPlayer->GetName())){
			   p->SetFocus(true);
			   lUIText->showFocus ();
		   }
		   else
			   p->SetFocus(false);
         
		   MyUITextList.addToTail(lUIText);	
		   bGotNext = UserProfiles::g_PlayersList.getNext (p);
	   }
   }
   // fill full list with empty 
   long j;
   for (j = UserProfiles::_lNbPlayers; j< UserProfiles::_lNbMaxPlayers; j++){ 
        lUIText = new KUIMyText (lpUILayout);
		lUIText->setEmptyText (); // to be displayed on screen
	    lUIText->setPosition(388, (float) (145+ i*43));
		i++;
		MyUITextList.addToTail(lUIText);	
   }

   lNewButtonGraphic = EImageBank::getImage("menu_butmainnew.png");	
   lpUINewButton = new KUIMyButton (lpUILayout);
   w = lNewButtonGraphic->getWidth();
   h = lNewButtonGraphic->getHeight();
   lpUINewButton->setSize(w, h);
   lpUINewButton->setBackgroundGraphic(K_UIELEMENT_DISABLED, lNewButtonGraphic, 0, 0, w, h);  
   lpUINewButton->setBackgroundGraphic(K_UIELEMENT_ENABLED, lNewButtonGraphic, 0, 0, w, h);  
   lpUINewButton->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, lNewButtonGraphic, 0, 0, w, h);  
   lpUINewButton->setBackgroundGraphic(K_UIELEMENT_DOWN, lNewButtonGraphic, 0, 0, w, h);  
   lpUINewButton->setPosition(46, 631);  
   lpUINewButton->setText (KStr ("UI_NEW"));   

   lDeleteButtonGraphic = EImageBank::getImage("menu_butmaindelete.png");	
   lpUIDeleteButton = new KUIMyButton (lpUILayout);
   w = lDeleteButtonGraphic->getWidth();
   h = lDeleteButtonGraphic->getHeight();
   lpUIDeleteButton->setSize(w, h);
   lpUIDeleteButton->setBackgroundGraphic(K_UIELEMENT_DISABLED, lDeleteButtonGraphic, 0, 0, w, h);  
   lpUIDeleteButton->setBackgroundGraphic(K_UIELEMENT_ENABLED, lDeleteButtonGraphic, 0, 0, w, h);  
   lpUIDeleteButton->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, lDeleteButtonGraphic, 0, 0, w, h);  
   lpUIDeleteButton->setBackgroundGraphic(K_UIELEMENT_DOWN, lDeleteButtonGraphic, 0, 0, w, h);  
   lpUIDeleteButton->setPosition(371, 631);  
   lpUIDeleteButton->setText (KStr ("UI_DELETE"));   

   lOkButtonGraphic = EImageBank::getImage("menu_butmainok.png");	
   lpUIOkButton = new KUIMyButton (lpUILayout);
   w = lOkButtonGraphic->getWidth();
   h = lOkButtonGraphic->getHeight();
   lpUIOkButton->setSize(w, h);
   lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_DISABLED, lOkButtonGraphic, 0, 0, w, h);  
   lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_ENABLED, lOkButtonGraphic, 0, 0, w, h);  
   lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, lOkButtonGraphic, 0, 0, w, h);  
   lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_DOWN, lOkButtonGraphic, 0, 0, w, h);  
   lpUIOkButton->setPosition(696, 631);  
   lpUIOkButton->setText (KStr ("UI_OK"));
}

void KUIPlayerListWindow::RefreshUI(Player*  pPlayer){
   Player * p;
   KUIMyText *lUIText;
   
   bool bGotNextPlayer = UserProfiles::g_PlayersList.getHead (p); 
   bool bGotNextText = MyUITextList.getHead(lUIText);
   long i;
	
   for (i=0; i <UserProfiles::_lNbMaxPlayers; i++){
      if (bGotNextPlayer) {
		   lUIText->setText (p->GetName()); // to be displayed on screen
		   lUIText->setName (p->GetName()); // to be associated to Text UI element
		   if (!strcmp(p->GetName(), pPlayer->GetName())){
			   p->SetFocus(true);
			   lUIText->showFocus();
			   UserProfiles::_pPlayer->SetName( (char *) p->GetName());
		   }
		   else {
			   lUIText->hideFocus();
			   p->SetFocus(false);
		   }
         
		   bGotNextPlayer = UserProfiles::g_PlayersList.getNext (p);
	   }
      else {
         lUIText->setEmptyText(); // to be displayed on screen
         lUIText->hideFocus(); // no focus
      }
      bGotNextText = MyUITextList.getNext(lUIText);
   }
   
}

KUIPlayerListWindow::~KUIPlayerListWindow(void){
   
	XDELETE(lpUITitleButton);
	XDELETE(lpUINewButton);
	XDELETE(lpUIDeleteButton);
	XDELETE(lpUIOkButton);
	//delete	lTitleButtonGraphic;
	//delete	lDeleteButtonGraphic;
	//delete	lNewButtonGraphic;
	//delete	lOkButtonGraphic;
	XDELETE(lpUILayout);
   //delete  lpWindowGraphic;
   XDELETE(lFont);
}

void KUIPlayerListWindow::Show(void){
	setState(K_UIELEMENT_ENABLED);  // input enabled
	setBlend (1); //  visible
	setPosition (fX, fY); // restore position
	setScale(1);
	setZAngle(0);
}

void KUIPlayerListWindow::Hide(void){
	setState(K_UIELEMENT_DISABLED); // disable input 
    animateBlend (1, 0, 0, 250);
    animateScale (1.0f, 0.1f, 0, 250, K_COUNTER_EASEIN);
 //   animatePosition (37, 13, 0, 0, 0, 250, K_COUNTER_EASEIN);
    animatePosition (fX, fY, 420, 320, 0, 250, K_COUNTER_EASEIN);
	animateZAngle (0, -20, 0, 250, K_COUNTER_EASEIN);

}

//*********************************************************************
//*********************************************************************
KUIDeletePlayerWindow::KUIDeletePlayerWindow(KUIElement *lpParentElem) : KUIImage(lpParentElem) {
	float w, h;

   lFont = EFontBank::getFont("NITECLUB.TTF",60);

   lpWindowGraphic = EImageBank::getImage("menu_deleteplayer.png"); 

	fX = 0;
	fY = 0;
	setPosition (fX, fY);
	float WinW = lpWindowGraphic->getWidth();
	float WinH = lpWindowGraphic->getHeight();
    fX = floorf((1024 - WinW)/2);
    fY = floorf((768 - WinH)/2);
	setSize (WinW, WinH);
	setGraphic (lpWindowGraphic, 0, 0, WinW, WinH);
	setBlend (0); // disable until required...
   
	lpUILayout = new KUILayout (this);
	lpUILayout->setPosition (0, 0);
	lpUILayout->setSize (WinW, WinH);
	lpUILayout->setLayoutType (K_UILAYOUT_FIXED);
  
    lTitleButtonGraphic = EImageBank::getImage("menu_butnew.png");	
    lpUITitleButton = new KUIMyButton (lpUILayout);
    lpUITitleButton->setPosition(300, 134); 
	w = lTitleButtonGraphic->getWidth();
	h = lTitleButtonGraphic->getHeight();
	lpUITitleButton->setSize(w, h);
    lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_DISABLED, lTitleButtonGraphic, 0, 0, w, h);  
    lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_ENABLED, lTitleButtonGraphic, 0, 0, w, h);  
    lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, lTitleButtonGraphic, 0, 0, w, h);  
    lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_DOWN, lTitleButtonGraphic, 0, 0, w, h);  
	lpUITitleButton->setText (KStr ("UI_DELETEPLAYER"));   

    lPlayerButtonGraphic = EImageBank::getImage("menu_butname.png");	
    lpUIPlayerButton = new KUIMyButton (lpUILayout);
    lpUIPlayerButton->setPosition(235, 215);
	w = lPlayerButtonGraphic->getWidth();
   h = lPlayerButtonGraphic->getHeight();
   lpUIPlayerButton->setSize(w, h); 
   lpUIPlayerButton->setBackgroundGraphic(K_UIELEMENT_DISABLED, lPlayerButtonGraphic, 0, 0, w, h);  
   lpUIPlayerButton->setBackgroundGraphic(K_UIELEMENT_ENABLED, lPlayerButtonGraphic, 0, 0, w, h);  
   lpUIPlayerButton->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, lPlayerButtonGraphic, 0, 0, w, h);  
   lpUIPlayerButton->setBackgroundGraphic(K_UIELEMENT_DOWN, lPlayerButtonGraphic, 0, 0, w, h);  
   lpUIPlayerButton->setText ("");   

   lFont2 = EFontBank::getFont("tahoma.ttf",42);
   lpUIPlayerButton->setTextFont (lFont2);

	lOkButtonGraphic = EImageBank::getImage("menu_butok.png");	 
	lpUIOkButton = new KUIMyButton (lpUILayout);
	lpUIOkButton->setPosition (216, 318);   
	w = lOkButtonGraphic->getWidth();
	h = lOkButtonGraphic->getHeight();
	lpUIOkButton->setSize(w, h);
    lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_DISABLED, lOkButtonGraphic, 0, 0, w, h);  
    lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_ENABLED, lOkButtonGraphic, 0, 0, w, h);  
    lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, lOkButtonGraphic, 0, 0, w, h);  
    lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_DOWN, lOkButtonGraphic, 0, 0, w, h);  
	lpUIOkButton->setText (KStr ("UI_OK"));   

	lCancelButtonGraphic = EImageBank::getImage("menu_butcancel.png");
	lpUICancelButton = new KUIMyButton (lpUILayout);
	lpUICancelButton->setPosition (455, 318);
	w = lCancelButtonGraphic->getWidth();
	h = lCancelButtonGraphic->getHeight();
	lpUICancelButton->setSize(w, h);
    lpUICancelButton->setBackgroundGraphic(K_UIELEMENT_DISABLED, lCancelButtonGraphic, 0, 0, w, h);  
    lpUICancelButton->setBackgroundGraphic(K_UIELEMENT_ENABLED, lCancelButtonGraphic, 0, 0, w, h);  
    lpUICancelButton->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, lCancelButtonGraphic, 0, 0, w, h);  
    lpUICancelButton->setBackgroundGraphic(K_UIELEMENT_DOWN, lCancelButtonGraphic, 0, 0, w, h);  
	lpUICancelButton->setText (KStr ("UI_CANCEL"));
}

KUIDeletePlayerWindow::~KUIDeletePlayerWindow(void){
   
	XDELETE(lpUITitleButton);
	XDELETE(lpUIPlayerButton);
	XDELETE(lpUIOkButton);
	XDELETE(lpUICancelButton);
	//delete lTitleButtonGraphic;
	//delete lPlayerButtonGraphic;
	//delete lOkButtonGraphic;
	//delete lCancelButtonGraphic;
	XDELETE(lpUILayout);
	//delete lpWindowGraphic;
   XDELETE(lFont);
   XDELETE(lFont2);
}

void KUIDeletePlayerWindow::Show(void){
   
	//update player name
    lpUIPlayerButton->setText (UserProfiles::_pPlayer->GetName()); 
   	setState(K_UIELEMENT_ENABLED);  // input enabled
	setBlend (1); //  visible
	setPosition (fX, fY); // restore position
	setScale(1);
	setZAngle(0);
}

void KUIDeletePlayerWindow::Hide(){
	setState(K_UIELEMENT_DISABLED); // disable input 
    animateBlend (1, 0, 0, 250);
    animateScale (1.0f, 0.1f, 0, 250, K_COUNTER_EASEIN);
    animatePosition (fX, fY, 420, 320, 0, 250, K_COUNTER_EASEIN);
    animateZAngle (0, -20, 0, 250, K_COUNTER_EASEIN);
}

//*********************************************************************
//*********************************************************************
KUINewPlayerWindow::KUINewPlayerWindow(KUIElement *lpParentElem) : KUIImage(lpParentElem) {
   lFont = NULL;
   lFont2 = NULL;

   // 17/08/11 RŽduit ˆ 14 au lieu de 16, car WWWWWWWWWWWWWWWW rentrait pas...
   // Et que j'ai pas rŽussi ˆ agrandir le champ avec toutes ces valeurs en dur...
	long lMaxChar = 14;
	float w, h;
   lFont = EFontBank::getFont("NITECLUB.TTF",60);

   lpWindowGraphic = EImageBank::getImage("menu_newplayer.png"); 


	float WinW = lpWindowGraphic->getWidth();
	float WinH = lpWindowGraphic->getHeight();
    fX = floorf((1024 - WinW)/2);
    fY = floorf((768 - WinH)/2);
	setPosition (fX, fY);
	setSize (WinW, WinH);
	setGraphic (lpWindowGraphic, 0, 0, WinW, WinH);
	setBlend (0); // disable until required...
   
	lpUILayout = new KUILayout (this);
	lpUILayout->setPosition (0, 0);
   lpUILayout->setSize (WinW, WinH);
   lpUILayout->setLayoutType (K_UILAYOUT_FIXED);

   lTitleButtonGraphic = EImageBank::getImage("menu_butnew.png");	
   lpUITitleButton = new KUIMyButton (lpUILayout);
   lpUITitleButton->setPosition(300, 134);
   w = lTitleButtonGraphic->getWidth();
   h = lTitleButtonGraphic->getHeight();
   lpUITitleButton->setSize(w, h); 
   lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_DISABLED, lTitleButtonGraphic, 0, 0, w, h);  
   lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_ENABLED, lTitleButtonGraphic, 0, 0, w, h);  
   lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, lTitleButtonGraphic, 0, 0, w, h);  
   lpUITitleButton->setBackgroundGraphic(K_UIELEMENT_DOWN, lTitleButtonGraphic, 0, 0, w, h);  
   lpUITitleButton->setText (KStr ("UI_NEWPLAYER"));   

   lpUIEditField = new KUIMyEditField (lpUILayout);
   lpUIEditField->setSize (392, 71);
   lpUIEditField->setPosition(235, 215);
   lpUIEditField->setMaxChars(lMaxChar); 

   lFont2 = EFontBank::getFont("tahoma.ttf",42);
   lpUIEditField->setTextFont (lFont2);
   lpUIEditField->setText ("");
   lpUIEditField->setSecureMode(false, lMaxChar); // lMaxChar is useless here,just to provide a dummy param
   

	lOkButtonGraphic = EImageBank::getImage("menu_butok.png");	 
	lpUIOkButton = new KUIMyButton (lpUILayout);
	lpUIOkButton->setPosition (216, 318);   
	w = lOkButtonGraphic->getWidth();
    h = lOkButtonGraphic->getHeight();
	lpUIOkButton->setSize(w, h);
    lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_DISABLED, lOkButtonGraphic, 0, 0, w, h);  
    lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_ENABLED, lOkButtonGraphic, 0, 0, w, h);  
    lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, lOkButtonGraphic, 0, 0, w, h);  
    lpUIOkButton->setBackgroundGraphic(K_UIELEMENT_DOWN, lOkButtonGraphic, 0, 0, w, h);  
	lpUIOkButton->setText (KStr ("UI_OK"));   

	lCancelButtonGraphic = EImageBank::getImage("menu_butcancel.png");
	lpUICancelButton = new KUIMyButton (lpUILayout);
	lpUICancelButton->setPosition (455, 318);
	w = lCancelButtonGraphic->getWidth();
    h = lCancelButtonGraphic->getHeight();
	lpUICancelButton->setSize(w, h);
    lpUICancelButton->setBackgroundGraphic(K_UIELEMENT_DISABLED, lCancelButtonGraphic, 0, 0, w, h);  
    lpUICancelButton->setBackgroundGraphic(K_UIELEMENT_ENABLED, lCancelButtonGraphic, 0, 0, w, h);  
    lpUICancelButton->setBackgroundGraphic(K_UIELEMENT_MOUSEOVER, lCancelButtonGraphic, 0, 0, w, h);  
    lpUICancelButton->setBackgroundGraphic(K_UIELEMENT_DOWN, lCancelButtonGraphic, 0, 0, w, h);  
	lpUICancelButton->setText (KStr ("UI_CANCEL"));
}

void KUINewPlayerWindow::Show(void){
	lpUIEditField->setFocus(); // focus on edit field where to input new player's name
	setState(K_UIELEMENT_ENABLED);  // input enabled
	setBlend (1); //  visible
	setPosition (fX, fY); // restore position
	setScale(1);
	setZAngle(0);

}

void KUINewPlayerWindow::Hide(void){
	lpUIEditField->setText ("");
	setState(K_UIELEMENT_DISABLED); // disable input 
//    animateBlend (1, 0, 0, 250);
   animateBlend (1, 0, 0, 25);
    //animateScale (1.0f, 0.1f, 0, 250, K_COUNTER_EASEIN);
    //animatePosition (fX, fY, 420, 320, 0, 250, K_COUNTER_EASEIN);
    //animateZAngle (0, -20, 0, 250, K_COUNTER_EASEIN);
}

KUINewPlayerWindow::~KUINewPlayerWindow(void){
	XDELETE(lpUIEditField); 
	XDELETE(lpUIOkButton);
	XDELETE(lpUICancelButton);
	//delete lOkButtonGraphic;
	//delete lCancelButtonGraphic;
	XDELETE(lpUILayout);
	//delete lpWindowGraphic;

   XDELETE(lFont);
   XDELETE(lFont2);
}
////////////////////////////////////////////

KList<Player> UserProfiles::g_PlayersList;
long UserProfiles::_lNbPlayers;
Player * UserProfiles::_pPlayer;
const char UserProfiles::EmptyName[8] = "-EMPTY-";
const long UserProfiles::_lNbMaxPlayers = 10; // maximum number of players allowed
KUIPlayerListWindow		* UserProfiles::MyPlayerListWindow;
KUIDeletePlayerWindow	* UserProfiles::MyDeletePlayerWindow;
KUINewPlayerWindow		* UserProfiles::MyNewPlayerWindow;

