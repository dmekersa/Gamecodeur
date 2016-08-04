
#include "KGraphic.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "FontBank.h"
#include "MyGame.h"
#include "EPointer.h"

#include "ESceneDirector.h"

#include "EMiniJeuBase.h"
#include "EMiniJeuHintDlg.h"

EMiniJeuHintDlg::EMiniJeuHintDlg(EMiniJeuBase *currentGame): EImgBox(EImageBank::getImage("dialogbox.png")){
	KAssert(currentGame != NULL);
	Coord tmp = GetSize() - Coord(119,155);
   ETextBox *tmpTextBox = NULL;

	SetMargin(26,26,16,26);

	SetCentered(true);
	SetBoxCenter(1024/2,768/2);
	SetVerticalPolicy(DOWN);

	_lpFont = EFontBank::getFont("jancient22",-16);
	_lpFontHintNumber = EFontBank::getFont("tahoma.ttf",14);
	_lpFontButtons = EFontBank::getFont("NITECLUB.TTF",35);
	//_lpFont->setColor(0,0,0,1);
	_lpFontHintNumber->setColor(0,0,0,1);

	_lpButtonsSkip = new EImgBox(EImageBank::getImage("dialogbox_halo.png"));
	_lpButtonsSkip->SetSizePolicy(FREE);
   _lpButtonsSkip->SetSize(GetInnerWidth()/2,35);

	_lpButtonsHelp = new EImgBox(EImageBank::getImage("dialogbox_halo.png"));
	_lpButtonsHelp->SetSize(GetInnerWidth()/2,35);

	/*_lpButtonsCancel = new EImgBox(EImageBank::getImage("dialogbox_halo.png"));
	_lpButtonsCancel->SetSizePolicy(FREE);
	_lpButtonsCancel->SetSize(200,35);*/
   tmpTextBox = new ETextBox(KStr("MINIGAME_HINT_SKIP_NOW"),_lpFontButtons,350);
   tmpTextBox->SetTextColor(0,0,0,1);
   tmpTextBox->SetTextOutlineThickness(2);
   tmpTextBox->SetTextOutlineColor(1,1,1,0.125f);
	_lpButtonsSkip->AddChild(tmpTextBox);
   tmpTextBox = new ETextBox(KStr("MINIGAME_HINT_GET_HINT"),_lpFontButtons,350);
   tmpTextBox->SetTextColor(0,0,0,1);
   tmpTextBox->SetTextOutlineThickness(2);
   tmpTextBox->SetTextOutlineColor(1,1,1,0.125f);
	_lpButtonsHelp->AddChild(tmpTextBox);
	_lpButtonsCancel = new ETextBox(KStr("MINIGAME_HINT_NO_THANKS"),_lpFontButtons,350);
   _lpButtonsCancel->SetTextColor(0,0,0,1);
   _lpButtonsCancel->SetTextOutlineThickness(2);
   _lpButtonsCancel->SetTextOutlineColor(1,1,1,0.125f);

	EBox *tmp1 = new EBox(0,0,0,0);
	tmp1->SetLayoutType(HORIZONTAL);
	tmp1->SetVerticalPolicy(UP);
	tmp1->SetSizePolicy(FREE);
 
	_lpHintTextBox = new ETextBox("test",_lpFontHintNumber,200);
	
	EBox *tmp2 = new EBox(0,0,0,0);
	tmp2->AddChild(_lpButtonsHelp);
	tmp2->AddChild(_lpHintTextBox);
	
	tmp1->AddChild(tmp2);
	tmp1->AddChild(_lpButtonsSkip);
	
	_lpHintSkipTextBox = new ETextBox(KStr("MINIGAME_SKIP_MORE_HELP_QUERY"),NULL,519);
   _lpHintSkipTextBox->SetFont(_lpFont,22);
	_lpHintSkipTextBox->SetMargin(20,30,20,30);
	_lpHintSkipTextBox->SetSizePolicy(FREE);

   tmp2 = new EBox(0,0,0,0);
   tmp2->SetSizePolicy(EBox::MAXIMIZE);
   tmp2->AddChild(_lpHintSkipTextBox);
   AddChild(tmp2);
	
   AddChild(tmp1);
	AddChild(_lpButtonsCancel);
	UpdateUI();

	_lpGame = currentGame;

	_lpCounterFad = new KCounter();
	_lpCounterFad->startCounter(0,0,0,0,K_COUNTER_LINEAR);
	_lpCounterFad->move(1);

	_fFadDuration = 1000;
	_bWasOver = false;
	_bCompleted = false;
   _bSwitchDisplay = false;
	SetBlend(0);
}
EMiniJeuHintDlg::~EMiniJeuHintDlg(){
	XDELETE(_lpButtonsHelp);
	XDELETE(_lpCounterFad);
	_lpButtonsSkip = NULL;
	_lpButtonsHelp = NULL;
	_lpButtonsCancel = NULL;
	_lpGame = NULL;
   XDELETE(_lpFont);
   XDELETE(_lpFontButtons);
   XDELETE(_lpFontHintNumber);
}

void EMiniJeuHintDlg::Draw(){

	if (!_lpCounterFad->isCompleted()) {
		_lpCounterFad->move(KGame::getGame()->getElapsedTime());
		SetBlend(_lpCounterFad->getCurrentValue());
	}
	EImgBox::Draw();
}
void EMiniJeuHintDlg::Check(){
	if (!Mouse.LeftEvent() || !IsOver()) {
		Mouse.EventClose();
		return;
	}

	if (_lpButtonsSkip->IsOver()) {
		ESoundBank::getSound("mg_select_deselect")->playSample();
		_lpGame->ActivateHint(MINIJEU_RESOLVED);
		CloseDialog();
	} else if (_lpGame->HintLevel() < MINIJEU_HINT2 && _lpButtonsHelp->IsOver()) {
      ESoundBank::getSound("mg_select_deselect")->playSample();
		if (_lpGame->HintLevel() == MINIJEU_NORMAL) {
			_lpGame->ActivateHint(MINIJEU_HINT1);
         UpdateUI();
		} else if (_lpGame->HintLevel() == MINIJEU_HINT1) {
			_lpGame->ActivateHint(MINIJEU_HINT2);
			_bSwitchDisplay = true;
		}
		CloseDialog();
	} else if (_lpButtonsCancel->IsOver()) {
		ESoundBank::getSound("mg_select_deselect")->playSample();
		CloseDialog();
	}
	Mouse.EventClose();
}

void EMiniJeuHintDlg::Show(){
   if (_bSwitchDisplay) {
      SetImg(EImageBank::getImage("dialogboxYN","png"));
      delete _lpButtonsHelp->GetParent();
      _lpButtonsHelp = NULL;

      EBox *tmpBox = _lpButtonsSkip->GetParent();
      EBox *tmpTextBox = _lpButtonsSkip->GetChild(0);
      _lpButtonsSkip->RemoveChild(tmpTextBox);
      tmpTextBox->SetWidth(GetInnerWidth()/2);
      XDELETE(_lpButtonsSkip);
      _lpButtonsSkip = tmpTextBox;

      RemoveChild(_lpButtonsCancel);
      _lpButtonsCancel->SetWidth(tmpTextBox->GetWidth());
      
      tmpBox->SetMargin(0,0,0,0);
      tmpBox->AddChild(tmpTextBox);
      tmpBox->AddChild(_lpButtonsCancel);
      
      UpdateUI();
      _bSwitchDisplay = false;
   }
	_lpCounterFad->startCounter(_lpCounterFad->getCurrentValue(),1,0,400,K_COUNTER_LINEAR);
   ESoundBank::getSound("dialogshow")->playSample();
	switch (_lpGame->HintLevel()) {
		case MINIJEU_NORMAL:
			_lpHintTextBox->SetText(KStr("MINIGAME_HINT_NUMBER"));
			sprintf(const_cast< char * >(_lpHintTextBox->GetText()),_lpHintTextBox->GetText(),2);
			if (strcmp(_lpGame->HintText(_lpGame->HintLevel()+1),"") == 0)
				_lpHintSkipTextBox->SetText(KStr("MINIGAME_SKIP_MORE_HELP_QUERY"));
			else
				_lpHintSkipTextBox->SetText(KStr("MINIGAME_SKIP_MORE_HELP_QUERY_HINT"));
			break;
		case MINIJEU_HINT1:
			_lpHintTextBox->SetText(KStr("MINIGAME_HINT_NUMBER"));
			sprintf(const_cast< char * >(_lpHintTextBox->GetText()),_lpHintTextBox->GetText(),1);
			if (strcmp(_lpGame->HintText(_lpGame->HintLevel()+1),"") == 0)
				_lpHintSkipTextBox->SetText(KStr("MINIGAME_SKIP_MORE_HELP_QUERY"));
			else
				_lpHintSkipTextBox->SetText(KStr("MINIGAME_SKIP_MORE_HELP_QUERY_HINT"));
			break;
		default:
			//_lpHintTextBox->SetText(KStr("MINIGAME_HINT_NUMBER"));
			//sprintf(const_cast< char * >(_lpHintTextBox->GetText()),_lpHintTextBox->GetText(),0);
			_lpHintSkipTextBox->SetText(KStr("MINIGAME_SKIP_QUERY"));
	}
}
void EMiniJeuHintDlg::Hide(){
	_lpCounterFad->startCounter(_lpCounterFad->getCurrentValue(),0,0,400,K_COUNTER_LINEAR);
}
bool EMiniJeuHintDlg::IsVisible(){
	return _lpCounterFad->getCurrentValue() > 0;
}
bool EMiniJeuHintDlg::IsCompleted() {
	return _bCompleted && !IsVisible();
}
void EMiniJeuHintDlg::ToggleCursor() {
	if (IsOver()) {
		if (_bWasOver)
			return;
		SCENEDIRECTOR->ChangeMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
		EScene *lpSceneCurrent = SCENEDIRECTOR->GetCurrentScene();
		if (lpSceneCurrent != NULL) {
			lpSceneCurrent->SetMouseOverInfo("", "");
		}
		lpSceneCurrent = SCENEDIRECTOR->GetCurrentVignette();
		if (lpSceneCurrent != NULL) {
			lpSceneCurrent->SetMouseOverInfo("", "");
		}
		_bWasOver = true;
	} else if (_bWasOver) {
		_bWasOver = false;
	}
}
void EMiniJeuHintDlg::CloseDialog() {
	Hide();
	_bCompleted = true;
}