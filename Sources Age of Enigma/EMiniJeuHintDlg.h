#ifndef EMINIJEUHINTDLG_H
#define EMINIJEUHINTDLG_H

#include "KPTK.h"
#include "EBox.h"
#include "KCounter.h"

class EMiniJeuBase;

class EMiniJeuHintDlg: public EImgBox {
public:
	EMiniJeuHintDlg(EMiniJeuBase *currentGame);
	~EMiniJeuHintDlg();

	void Draw();
	void Check();
	
	void Show();
	void Hide();
	bool IsVisible();

	bool IsCompleted();
private:
	void ToggleCursor();
	void CloseDialog();

	EBox *_lpButtonsSkip;
	EImgBox *_lpButtonsHelp;
	ETextBox *_lpButtonsCancel;
	ETextBox *_lpHintTextBox;
	ETextBox *_lpHintSkipTextBox;

	EFont *_lpFont;
	EFont *_lpFontHintNumber;
	EFont *_lpFontButtons;
	
	EMiniJeuBase *_lpGame;

	KCounter *_lpCounterFad;
	float _fFadDuration;
	bool _bWasOver;

	bool _bCompleted;
   bool _bSwitchDisplay;
};
#endif