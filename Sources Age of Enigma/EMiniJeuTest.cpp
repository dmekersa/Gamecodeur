#include "assert.h"

#include "EPointer.h"
#include "EGeometry.h"
#include "ImageBank.h"
#include "FontBank.h"
#include "EMiniJeuTest.h"

EMiniJeuTest::EMiniJeuTest(void)
{
	SetTitle("Connect the colored wires");
	SetRule("Connect the 3 pairs of colored wires by placing the appropriate tiles on the grid. Right clic to rotate a tile.\n\nClick on HELP to lower the difficulty.");
	SetGameType(MINIJEU_DIRECTGAME);
	SetBackGround(EImageBank::getImage("mg_poltergeist_bg.jpg"));

	vpol = EBox::UP;

	box = new EImgBox(EImageBank::getImage("dialogbox.png"),10);
	box->Move(200,200);

	textbox = new ETextBox();
	textbox->SetMargin(10,10,5,5);
	textbox->SetGeometry(30,30,350,80);
	textbox->SetText("Bonjour\n\nje suis actellement a la recherche du plus somptueux des palaces.\n\nPourriez vous m'aider?");
	textbox->Move(100,400);
	textbox->SetFont(EFontBank::getFont("tahoma.ttf",20));
	textbox->SetCentered(true);

	ebox = new EImgExpendableBox(EImageBank::getImage("tuto_img_bg.png"),6,6,6,6);
	ebox->SetSize(700,200);

	
	button1 = new EBox(10,10,60,20,2,2,2,2);
	button2 = new EBox(10,10,50,20,2,2,2,2);
	textbox->AddChild(button1);
	textbox->AddChild(button2);
	textbox->AddChild(textbox);

	tuto = new ETutoBox("hello,\n\n\n ceci es un test de tutorial.\nVous devriez voir en dessous de cette zone. Une indication sur la manire dont le tuto doit es rŽussi\n",
		30,30,120);
	tuto->Show();

	hintbox = new EMiniJeuHintDlg(this);
}
EMiniJeuTest::~EMiniJeuTest(void)
{
	XDELETE(textbox);
	XDELETE(box);
	XDELETE(tuto);
	XDELETE(ebox);
}

/** 3 steps rendering **/
void EMiniJeuTest::GameCheck ()
{
	hintbox->Check();
	//tuto->Check();

	//if (Mouse.LeftEvent()) {
	//	int i = int(vpol);
	//	i++;
	//	i = i %3;
	//	vpol = (EBox::VerticalPolicy)i;
	//	box->SetVerticalPolicy(vpol);
	//}
	//if (Mouse.LeftEvent()) {
	//	mouseSel = Mouse;
	//	exwidth = ebox->GetSize();
	//	expos = ebox->GetPos();
	//	Mouse.LeftEventClose();
	//}
	//if (Mouse.RightEvent()) {
	//	if (mouseSel < Coord(0,0)) {
	//		if (ebox->GetHMargin() != 0) {
	//			ebox->SetMargin(0,0,0,0);
	//		} else {
	//			ebox->SetMargin(10,10,6,6);
	//		}
	//	}
	//}
	//if (Mouse.LeftReleaseEvent()) {
	//	mouseSel = Coord(-1,-1);
	//}

	if (Mouse.LeftEvent() && !Mouse.RightState() && tuto->IsOver()) {
		mouseSel = Mouse;
		expos = tuto->GetArrowPos();
		Mouse.LeftEventClose();
	}
	if (Mouse.RightEvent() && !Mouse.LeftState()) {
		mouseSel = Mouse;
	}
	if (Mouse.LeftReleaseEvent() || Mouse.RightReleaseEvent()) {
		mouseSel = Coord(-1,-1);
		Mouse.LeftEventClose();
	}
}

void EMiniJeuTest::GameLogic ()
{	
}

void EMiniJeuTest::GameDrawSet ()
{
	if (Mouse.LeftState() && ! Mouse.RightState() && mouseSel >= Coord(0,0)) {
		Coord delta = Mouse - mouseSel;
		delta = Coord((int)delta.x,(int)delta.y);
		if (delta != Coord(0,0)) {
			tuto->MoveArrow(expos + delta);
			expos = tuto->GetArrowPos();
			mouseSel = Mouse;
		}
	}
	if (Mouse.RightState() && !Mouse.LeftState() && tuto->HasArrow() && mouseSel >= Coord(0,0)) {

		float angle = (int)EGeometry::Angle(tuto->GetArrowPos(),Mouse) - (int)EGeometry::Angle(tuto->GetArrowPos(),mouseSel);
		if (angle != 0) {
			tuto->SetArrowDir(tuto->GetArrowDir() + angle);
			mouseSel = Mouse;
		}
	}
	
	ebox->UpdateUI();

	/*textbox->Draw();
	box->Draw();
	tuto->Draw();

	*/
	hintbox->Draw();

//	ebox->Draw();
	IsResolved();
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuTest::GameIsResolve()
{
	return _nGameStatus == MINIJEU_RESOLVED;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuTest::GameResolve()
{
}

// Active un niveau d'indices (_nGameStatus = 1, 2 ou 3)
void EMiniJeuTest::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	case MINIJEU_HINT1:
		Reset();
		break;
	case MINIJEU_HINT2:
		Reset();
		break;
	case MINIJEU_RESOLVED:
		  Resolve();
		  break;
	  default:
		  KAssert(false);
		  break;
	}
}

// Preload images et sons
void EMiniJeuTest::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_poltergeist_bg.jpg","",true,true);
   EImageBank::getImage("dialogbox.png","",true,true);
   EImageBank::getImage("tuto_img_bg.png","",true,true);

}
