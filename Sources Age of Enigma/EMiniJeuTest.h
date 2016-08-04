#ifndef EMINIJEUTEST_H
#define EMINIJEUTEST_H

#include "global.h"
#include "EMiniJeuBase.h"
#include "KPTK.h"
#include "EBox.h"
#include "ETutoBox.h"
#include "EMiniJeuHintDlg.h"

class EMiniJeuTest: public EMiniJeuBase {
public:
	EMiniJeuTest (void);
	virtual ~EMiniJeuTest (void);
	
   static void Preload();

private:
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();
	
	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset() {;}
	virtual void GameSetHint(int nHintLevel);

	EFont *_lpFont;

	ETextBox *textbox;
	EBox *button1, *button2;
	EImgBox *box;

	Coord mouseSel;
	Coord exwidth;
	Coord expos;

	EBox::VerticalPolicy vpol;

	ETutoBox *tuto;
	EMiniJeuHintDlg *hintbox;
	EImgExpendableBox *ebox;
};
#endif
