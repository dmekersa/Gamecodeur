#include "assert.h"

#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "FontBank.h"

#include "EMiniJeuMemoryPlant.h"

#define MEMORYPLANT_NARIATION 3

using EMiniGame::GetRandom;

//Coord RelPos = Coord(190,100);

EMiniJeuMemoryPlant::EMiniJeuMemoryPlant(void) {
	SetTextsTrad("PLANTMEMORY");

	SetGameType(MINIJEU_SUBMITGAME);
	SetGameNumber(20,3);

	SetBackGround(EImageBank::getImage("mg_memorygrid_bg","jpg"));
	_nXGameSize = 4;
	_nYGameSize = 3;

	_lpFont = EFontBank::getFont("tahoma.ttf",32);
	_lpFontStart = EFontBank::getFont("tahoma.ttf",40);
	_lpFontTimeout = EFontBank::getFont("tahoma.ttf",40);
	_lpFont->setColor(1,1,1,1);
	_lpFontStart->setColor(1,1,1,1);
	_lpFontTimeout->setColor(1,1,1,1);

	memset(_cTextStart,0,sizeof(_cTextStart));
	memset(_cTextTimeout,0,sizeof(_cTextTimeout));
	memset(_cTextTimeoutSeconds,0,sizeof(_cTextTimeoutSeconds));
	strcat(_cTextStart,KStr("PLANTMEMORY_START"));
	strcat(_cTextTimeout,KStr("PLANTMEMORY_SHOW"));

	KGraphic *green = EImageBank::getImage("mg_memorygrid_case_green_color","png");
	KGraphic *red = EImageBank::getImage("mg_memorygrid_case_red_color","png");
	KGraphic *yellow = EImageBank::getImage("mg_memorygrid_case_yellow_color","png");
	KGraphic *white = EImageBank::getImage("mg_memorygrid_case_white_color","png");

	_vPartSet.push_back(new EMemoryPlantPart(EImageBank::getImage("mg_memorygrid_garlic","png"),white));
	_vPartSet.push_back(new EMemoryPlantPart(EImageBank::getImage("mg_memorygrid_gingko","png"),green));
	_vPartSet.push_back(new EMemoryPlantPart(EImageBank::getImage("mg_memorygrid_pavot","png"),red));
	_vPartSet.push_back(new EMemoryPlantPart(EImageBank::getImage("mg_memorygrid_rue","png"),yellow));

	_lpGridGame = new Grid(_nXGameSize,_nYGameSize,_vPartSet[0]->Width(),_vPartSet[0]->Height(),0,10);
	_lpGridGame->Move(288, 100);

	_lpImgStart = new EMiniJeuPart(EImageBank::getImage("mg_memorygrid_start_bouton","png"));
	_lpImgStart->Move(214,464);

	_vGame = std::vector< std::vector< EMemoryPlantPart * > >(_nYGameSize, std::vector< EMemoryPlantPart * >(_nXGameSize, (EMemoryPlantPart *) NULL));

	_fShowTimeDuration = 30000;
	_bDisplayStart = true;
	_lpShowTime = NULL;
	_nSecondRemaining = 0;

	_lpQueryVariant = NULL;

	SetVariation(-1);
}

EMiniJeuMemoryPlant::~EMiniJeuMemoryPlant(void)
{
	XDELETE(_lpGridGame);
	for (int i=0; i < _vGame.size(); i++) {
		for (int j=0; j < _vGame[0].size(); j++) {
			if (_vGame[i][j] != NULL)
				XDELETE(_vGame[i][j]);
		}
	}
	for (int i=0; i < _vPartSet.size(); i++) {
		XDELETE(_vPartSet[i]);
	}
	XDELETE(_lpImgStart);
   XDELETE(_lpQueryVariant);
   XDELETE(_lpShowTime);
   XDELETE(_lpFont);
   XDELETE(_lpFontStart);
   XDELETE(_lpFontTimeout);

}

void EMiniJeuMemoryPlant::SetVariation (int nVariation) {
	int _nVariation = nVariation;
	if (_nVariation == -1)
		_nVariation = GetRandom(MEMORYPLANT_NARIATION);

   XDELETE(_lpQueryVariant);
	switch (_nVariation) {
		case 0:
			_lpQueryVariant =  new QueryVariante0(this);
			break;
		case 1:
			_lpQueryVariant =  new QueryVariante1(this);
			break;
		case 2:
			_lpQueryVariant =  new QueryVariante2(this);
			break;
		default:
			KAssert(false);
	}
	Reset();
}

/** 3 steps rendering **/
void EMiniJeuMemoryPlant::GameCheck ()
{
	if (!Mouse.LeftEvent())
		return;

	if (_bDisplayStart) {
		if (!_lpImgStart->IsMouseOver()) 
			return;

		ESoundBank::getSound("mg_apparition")->playSample();
		_bDisplayStart = false;
		Mouse.LeftEventClose();
		for (int i=0; i < _vGame.size(); i++) {
			for (int j=0; j < _vGame[0].size(); j++) {
				_vGame[i][j]->FadIn(500);
			}
		}
		if (_lpShowTime != NULL)
			XDELETE(_lpShowTime);
		_lpShowTime = new KCounter();
		_lpShowTime->startCounter(-3,_fShowTimeDuration/1000,0,(double)_fShowTimeDuration+3000,K_COUNTER_LINEAR);

		return;
	}

	if (!_lpShowTime->isCompleted() && Mouse.LeftEvent()) {
		_lpShowTime->move(_fShowTimeDuration);
		Mouse.LeftEventClose();
		return;
	}

	if (_lpShowTime->isCompleted())
		_lpQueryVariant->Check();
}

void EMiniJeuMemoryPlant::GameLogic ()
{
	if (_bDisplayStart)
		return;

	if (!_lpShowTime->isCompleted()) {
		for (int i=0; i < _vGame.size(); i++) {
			for (int j=0; j < _vGame[0].size(); j++) {
				_vGame[i][j]->Logic();
			}
		}
		return;
	}
}

void EMiniJeuMemoryPlant::GameDrawSet()
{
	Coord pos;

	if (_bDisplayStart) {
		pos = _lpImgStart->Pos();
		pos.y = pos.y + (100 - 46)/2;
		_lpImgStart->Draw();
		_lpFontStart->drawStringCentered(_cTextStart,pos.x +44,pos.x + _lpImgStart->Width() -30,pos.y);
		return;
	}
	
	if (!_lpShowTime->isCompleted()) {
		double fElapsed = _lpMyGame->getKWindow()->getFrameTime();

		//_lpImgStart->Draw();
		_lpShowTime->move(fElapsed);

		for (int i=0; i < _vGame.size(); i++) {
			for (int j=0; j < _vGame[0].size(); j++) {
				_vGame[i][j]->Draw();
			}
		}
		pos = _lpImgStart->Pos();
		pos.y = pos.y + (100 - 46)/2;
		
		if (_nSecondRemaining != (int)((_fShowTimeDuration/1000) - _lpShowTime->getCurrentValue())) {
			_nSecondRemaining = (_fShowTimeDuration/1000) - _lpShowTime->getCurrentValue();
			memset(_cTextTimeoutSeconds,0,sizeof(_cTextTimeoutSeconds));
			strcat(_cTextTimeoutSeconds,(itos(_nSecondRemaining)+KStr("MINIGAME_SECONDS")).c_str());
			if (_nSecondRemaining <=(int)(_fShowTimeDuration /1000))
				ESoundBank::getSound("mg_counter_tic")->playSample();
		} 

		if (_nSecondRemaining > _fShowTimeDuration/1000)
			return;

		_lpFont->drawStringCentered(_cTextTimeout,pos.x +44,pos.x + _lpImgStart->Width() -30,pos.y);
		_lpFontTimeout->drawStringCentered(_cTextTimeoutSeconds,pos.x +44,pos.x + _lpImgStart->Width() -30,pos.y + 55);

		return;
	} else {
		if (_nSecondRemaining >= 0) {
			ESoundBank::getSound("mg_disparition")->playSample();
			ESoundBank::getSound("mg_end_counter")->playSample();
			_nSecondRemaining = -1;
		}
	}

	_lpQueryVariant->Draw();
}

void EMiniJeuMemoryPlant::GameReset() {
		
	EMemoryPlantPart *tmpPart;
	
	int r;
m0:	for (int i=0; i < _vGame.size(); i++) {
		for (int j=0; j < _vGame[0].size(); j++) {
			if (_vGame[i][j] != NULL)
				XDELETE(_vGame[i][j]);

			r = GetRandom(_vPartSet.size());
			tmpPart = new EMemoryPlantPart(_vPartSet[r]);
			_vGame[i][j] = tmpPart;
			tmpPart->Move(_lpGridGame->GetCellPos(j,i));
			tmpPart->Hide();
			tmpPart->FadIn(3000);
		}
	}

	// Vérifie que toutes les parties sont utilisé
	for (int k=0; k<_vPartSet.size(); k++){
		bool pres = false;
		tmpPart = _vPartSet[k];
		for (int i=0; i < _vGame.size(); i++) {
			for (int j=0; j < _vGame[0].size(); j++) {
				if (*tmpPart == *(_vGame[i][j])) {
					pres = true;
				}
			}
		}
		if (!pres)
			goto m0;
	}

	if (_lpShowTime != NULL)
		XDELETE(_lpShowTime);
	_bDisplayStart = true;
	_lpQueryVariant->InitQuery();
}

// Retourne true si le mini jeu est rŽsolu
bool EMiniJeuMemoryPlant::GameIsResolve()
{
	return _lpQueryVariant->IsResolved();
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuMemoryPlant::GameResolve()
{
	_lpQueryVariant->Resolve();
	_bDisplayStart = false;
	XDELETE(_lpShowTime);
	_lpShowTime = new KCounter;
	_lpShowTime->startCounter(0,_fShowTimeDuration,0,0,K_COUNTER_LINEAR);
	_lpShowTime->move(150);
   _nSecondRemaining = -1;
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuMemoryPlant::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  _lpQueryVariant->SetHint();
		  Reset(); 
		  break;
	  case MINIJEU_HINT2:
		  _lpQueryVariant->SetHint();
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

// ------------------------------------------------------ //
EMemoryPlantQuery::EMemoryPlantQuery(EMiniJeuMemoryPlant *game, const char *cQuery) {
		MG = game;
		_lpImgQuery = EImageBank::getImage("mg_memorygrid_query","png");
		_lpImgError = EImageBank::getImage("minigame_middleage_wrong","png");

		memset(_cTextQuery,0,sizeof(_cTextQuery));
		memset(_cTextHint1,0,sizeof(_cTextHint1));
		memset(_cTextHint2,0,sizeof(_cTextHint2));
		strcpy(_cTextQuery,cQuery);

		_lpFont = EFontBank::getFont("tahoma.ttf",32);
		_lpFont->setColor(1,1,1,1);

		_cTextQueryCentered = Coord (0,1024);
		_fYTextQuerry = MG->_lpGridGame->_yPos + MG->_lpGridGame->height() +10;
		_fQueryHeight = _lpFont->getMultilineHeight(_cTextQuery,_cTextQueryCentered.x,_cTextQueryCentered.y,_fYTextQuerry,TEXTSTYLE_CENTER);
}
EMemoryPlantQuery::~EMemoryPlantQuery(void) {
   XDELETE(_lpFont);
}
void EMemoryPlantQuery::DrawQuery() {
	//_lpImgError->drawRect(_pBlendBox1.x,_pBlendBox1.y,_pBlendBox2.x,_pBlendBox2.y,0,0,0,0.5);
	_lpFont->drawMultiline(_cTextQuery,_cTextQueryCentered.x,_cTextQueryCentered.y,_fYTextQuerry,TEXTSTYLE_CENTER);
}

// --------------------- Variante 0 --------------------------------- //

QueryVariante0::QueryVariante0(EMiniJeuMemoryPlant *game): EMemoryPlantQuery(game,KStr("PLANTMEMORY_V0_QUERY")) {
	MG->SetGameType(MINIJEU_SUBMITGAME);
	_vSelect = std::vector< std::vector< bool > >(MG->_vGame.size(), std::vector< bool >(MG->_vGame[0].size(), false));
	_lpImgPlant = NULL;
	_lpHintPlant1 = NULL;
	_lpHintPlant2 = NULL;

	_lpImgBoxPlant = EImageBank::getImage("mg_memorygrid_case_variante","png");

	// Calcul de la position de la chaine de caractere et de la box
	_cTextQueryCentered = Coord (0,500);
	float widthQuery = _lpFont->getMultilineWidth(_cTextQuery,_cTextQueryCentered.x,_cTextQueryCentered.y,_fYTextQuerry,TEXTSTYLE_CENTER);
	_cTextQueryCentered.x = (1024 - (widthQuery + 10 + _lpImgBoxPlant->getWidth()) )/ 2;
	_cTextQueryCentered.y = _cTextQueryCentered.x + widthQuery;
	_fQueryHeight = _lpFont->getMultilineHeight(_cTextQuery,_cTextQueryCentered.x,_cTextQueryCentered.y,_fYTextQuerry,TEXTSTYLE_CENTER);	

	float deltaHeight = (_fQueryHeight - (_lpImgBoxPlant->getHeight() - 7 - 20))/2;
	_fYTextQuerry = MG->_lpGridGame->_yPos + MG->_lpGridGame->height() + 30 - deltaHeight;
	
	posBox = Coord (_cTextQueryCentered.y+10,_fYTextQuerry + deltaHeight - 7);
	posPlant = Coord (posBox.x + 24, posBox.y+13);

	_pBlendBox1.x = _cTextQueryCentered.x - 15;
	_pBlendBox1.y = posBox.y - 7;
	_pBlendBox2.x = posBox.x + _lpImgBoxPlant->getWidth() + 15;
	_pBlendBox2.y = posBox.y + _lpImgBoxPlant->getHeight() + 7;;

	strcpy(_cTextHint1,KStr("PLANTMEMORY_V0_HINT1"));
	strcpy(_cTextHint2,KStr("PLANTMEMORY_V0_HINT2"));
}

QueryVariante0::~QueryVariante0(void){;}

void QueryVariante0::InitQuery(){
	Coord pos = Coord(GetRandom(_vSelect[0].size()), GetRandom(_vSelect.size()));
	_lpImgPlant = MG->_vGame[pos.y][pos.x]->GetImg();
	_lpHintPlant1 = NULL;
	_lpHintPlant2 = NULL;

	_vSelect = std::vector< std::vector< bool > >(MG->_vGame.size(), std::vector< bool >(MG->_vGame[0].size(), false));
	_vHint = std::vector< std::vector< KGraphic * > >(_vSelect.size(),std::vector< KGraphic * > (_vSelect[0].size(), (KGraphic *) NULL));

	if (MG->_nHintLevel < MINIJEU_HINT1)
		return;

	for (int i=0; i < _vSelect.size(); i++) {
		for (int j=0; j < _vSelect[0].size(); j++) {
			if (_lpHintPlant1 == NULL && MG->_vGame[i][j]->GetImg() != _lpImgPlant)
				_lpHintPlant1 = MG->_vGame[i][j]->GetImg();
			if (MG->_vGame[i][j]->GetImg() == _lpHintPlant1)
				_vHint[i][j] = _lpHintPlant1;
		}
	}

	if (MG->_nHintLevel < MINIJEU_HINT2)
		return;
	for (int i=0; i < _vSelect.size(); i++) {
		for (int j=0; j < _vSelect[0].size(); j++) {
			if (_lpHintPlant2 == NULL && (MG->_vGame[i][j]->GetImg() != _lpImgPlant) && (MG->_vGame[i][j]->GetImg() != _lpHintPlant1))
				_lpHintPlant2 = MG->_vGame[i][j]->GetImg();
			if (MG->_vGame[i][j]->GetImg() == _lpHintPlant2)
				_vHint[i][j] = _lpHintPlant2;
		}
	}
}

void QueryVariante0::Draw(){
	Coord pos;
	
	if (MG->_nGameStatus == MINIJEU_NORMAL) {
		for (int i=0; i < _vSelect.size(); i++) {
			for (int j=0; j < _vSelect[0].size(); j++) {
				if (_vSelect[i][j]) {
					MG->_lpGridGame->FillCell(_lpImgPlant,j,i,1,1);
				} else {
					if (_vHint[i][j] != NULL) {
						MG->_vGame[i][j]->GetImg()->setBlitColor(0.3,0.3,0.3,1);
						MG->_vGame[i][j]->Draw();
						MG->_vGame[i][j]->GetImg()->setBlitColor(1,1,1,1);
					} else {
						MG->_lpGridGame->FillCell(_lpImgQuery,j,i,1,1);
					}
				}
			}
		}
		DrawQuery();
		_lpImgBoxPlant->blitAlphaRect(0,0,_lpImgBoxPlant->getWidth(),_lpImgBoxPlant->getHeight(),posBox.x,posBox.y);
		_lpImgPlant->blitAlphaRect(0,0,_lpImgPlant->getWidth(),_lpImgPlant->getHeight(),posPlant.x,posPlant.y);
	} else {
		for (int i=0; i < _vSelect.size(); i++) {
			for (int j=0; j < _vSelect[0].size(); j++) {
				if (_vSelect[i][j]) {
					MG->_vGame[i][j]->Draw();
					if (_vSelect[i][j] != (MG->_vGame[i][j]->GetImg() == _lpImgPlant))
						MG->_lpGridGame->FillCell(_lpImgError,j,i,1,1);
				} else {
						MG->_vGame[i][j]->GetImg()->setBlitColor(0.3,0.3,0.3,1);
						MG->_vGame[i][j]->Draw();
						MG->_vGame[i][j]->GetImg()->setBlitColor(1,1,1,1);
				}
			}
		}
	}

}

void QueryVariante0::Check(){
	Coord pos;

	if (!MG->_lpGridGame->IsMouseOver())
		return;

	Mouse.LeftEventClose();
	ESoundBank::getSound("mg_select_deselect")->playSample();
	pos = MG->_lpGridGame->GetMouseSelection();
	if (_vHint[pos.y][pos.x] != NULL)
		return;
	_vSelect[pos.y][pos.x] = !_vSelect[pos.y][pos.x];

}

bool QueryVariante0::IsResolved() {
	bool res = true;

	for (int i=0; i < _vSelect.size(); i++){
		for (int j=0; j < _vSelect[0].size(); j++) {
			if (_vSelect[i][j] != (MG->_vGame[i][j]->GetImg() == _lpImgPlant)) {
				res = false;
			}
			MG->_vGame[i][j]->Hide();
			MG->_vGame[i][j]->FadIn(1000);
		}
	}
	return res;
}

void QueryVariante0::Resolve(){
	for (int i=0; i < _vSelect.size(); i++){
		for (int j=0; j < _vSelect[0].size(); j++) {
			_vSelect[i][j] = false;
			if (MG->_vGame[i][j]->GetImg() == _lpImgPlant) {
				_vSelect[i][j] = true;
			}
		}
	}
}

void QueryVariante0::SetHint() {
	switch (MG->_nHintLevel) {
		case MINIJEU_HINT1:
			MG->SetHintText(_cTextHint1);
			break;
		case MINIJEU_HINT2:
			MG->SetHintText(_cTextHint2);
			break;
		default:
			break;
	}
}

// --------------------- Variante 1 --------------------------------- //

QueryVariante1::QueryVariante1(EMiniJeuMemoryPlant *game): EMemoryPlantQuery(game,KStr("PLANTMEMORY_V1_QUERY")) {
	MG->SetGameType(MINIJEU_SUBMITGAME);
	_vSelect = std::vector< std::vector< bool > >(MG->_vGame.size(), std::vector< bool >(MG->_vGame[0].size(), false));
	_lpImgColor = NULL;
	_lpHintPlant1 = NULL;
	_lpHintPlant2 = NULL;
		
	strcpy(_cTextHint1,KStr("PLANTMEMORY_V1_HINT1"));
	strcpy(_cTextHint2,KStr("PLANTMEMORY_V1_HINT2"));
}

QueryVariante1::~QueryVariante1(void){;}

void QueryVariante1::InitQuery(){
	Coord pos = Coord(GetRandom(_vSelect[0].size()), GetRandom(_vSelect.size()));
	_lpImgColor = MG->_vGame[pos.y][pos.x]->_Color;
	_lpHintPlant1 = NULL;
	_lpHintPlant2 = NULL;

	// Calcul de la position de la chaine de caractere et de la couleur
	_cTextQueryCentered = Coord (0,500);
	float widthQuery = _lpFont->getMultilineWidth(_cTextQuery,_cTextQueryCentered.x,_cTextQueryCentered.y,_fYTextQuerry,TEXTSTYLE_CENTER);
	_cTextQueryCentered.x = (1024 - (widthQuery + 10 + _lpImgColor->getWidth()) )/ 2;
	_cTextQueryCentered.y = _cTextQueryCentered.x + widthQuery;
	_fQueryHeight = _lpFont->getMultilineHeight(_cTextQuery,_cTextQueryCentered.x,_cTextQueryCentered.y,_fYTextQuerry,TEXTSTYLE_CENTER);	

	float deltaHeight = (_fQueryHeight - (_lpImgColor->getHeight() - 7 - 21))/2;
	_fYTextQuerry = MG->_lpGridGame->_yPos + MG->_lpGridGame->height() + 30 - deltaHeight;
	
	posColor = Coord (_cTextQueryCentered.y+10,_fYTextQuerry + deltaHeight - 7);

	_pBlendBox1.x = _cTextQueryCentered.x - 15;
	_pBlendBox1.y = posColor.y - 7;
	_pBlendBox2.x = posColor.x + _lpImgColor->getWidth() + 15;
	_pBlendBox2.y = posColor.y + _lpImgColor->getHeight() + 7;;


	_vSelect = std::vector< std::vector< bool > >(MG->_vGame.size(), std::vector< bool >(MG->_vGame[0].size(), false));
	_vHint = std::vector< std::vector< KGraphic * > >(_vSelect.size(),std::vector< KGraphic * > (_vSelect[0].size(), (KGraphic *) NULL));

	if (MG->_nHintLevel < MINIJEU_HINT1)
		return;

	for (int i=0; i < _vSelect.size(); i++) {
		for (int j=0; j < _vSelect[0].size(); j++) {
			if (_lpHintPlant1 == NULL && MG->_vGame[i][j]->_Color != _lpImgColor)
				_lpHintPlant1 = MG->_vGame[i][j]->GetImg();
			if (MG->_vGame[i][j]->GetImg() == _lpHintPlant1)
				_vHint[i][j] = _lpHintPlant1;
		}
	}

	if (MG->_nHintLevel < MINIJEU_HINT2)
		return;
	for (int i=0; i < _vSelect.size(); i++) {
		for (int j=0; j < _vSelect[0].size(); j++) {
			if (_lpHintPlant2 == NULL && (MG->_vGame[i][j]->_Color != _lpImgColor) && (MG->_vGame[i][j]->_Color != _lpHintPlant1))
				_lpHintPlant2 = MG->_vGame[i][j]->GetImg();
			if (MG->_vGame[i][j]->GetImg() == _lpHintPlant2)
				_vHint[i][j] = _lpHintPlant2;
		}
	}
}

void QueryVariante1::Draw(){
	Coord pos;

	if (MG->_nGameStatus == MINIJEU_NORMAL) {
		for (int i=0; i < _vSelect.size(); i++) {
			for (int j=0; j < _vSelect[0].size(); j++) {
				if (_vSelect[i][j]) {
					MG->_lpGridGame->FillCell(_lpImgColor,j,i,1,1);
				} else {
					if (_vHint[i][j] != NULL) {
						MG->_vGame[i][j]->GetImg()->setBlitColor(0.3,0.3,0.3,1);
						MG->_vGame[i][j]->Draw();
						MG->_vGame[i][j]->GetImg()->setBlitColor(1,1,1,1);
					} else {
						MG->_lpGridGame->FillCell(_lpImgQuery,j,i,1,1);
					}
				}
			}
		}
		DrawQuery();
		_lpImgColor->blitAlphaRect(0,0,_lpImgColor->getWidth(),_lpImgColor->getHeight(),posColor.x,posColor.y);
	} else {
		for (int i=0; i < _vSelect.size(); i++) {
			for (int j=0; j < _vSelect[0].size(); j++) {
				if (_vSelect[i][j]) {
					MG->_vGame[i][j]->Draw();
					if (_vSelect[i][j] != (MG->_vGame[i][j]->_Color == _lpImgColor))
						MG->_lpGridGame->FillCell(_lpImgError,j,i,1,1);
				} else {
					MG->_vGame[i][j]->GetImg()->setBlitColor(0.3,0.3,0.3,1);
					MG->_vGame[i][j]->Draw();
					MG->_vGame[i][j]->GetImg()->setBlitColor(1,1,1,1);
				}
			}
		}
	}
}

void QueryVariante1::Check(){
	Coord pos;

	if (!MG->_lpGridGame->IsMouseOver())
		return;

	Mouse.LeftEventClose();
	pos = MG->_lpGridGame->GetMouseSelection();
	ESoundBank::getSound("mg_select_deselect")->playSample();
	if (_vHint[pos.y][pos.x] != NULL)
		return;
	_vSelect[pos.y][pos.x] = !_vSelect[pos.y][pos.x];

}

bool QueryVariante1::IsResolved() {
	bool res = true;

	for (int i=0; i < _vSelect.size(); i++){
		for (int j=0; j < _vSelect[0].size(); j++) {
			if (_vSelect[i][j] != (MG->_vGame[i][j]->_Color == _lpImgColor)) {
				res = false;
			}
			MG->_vGame[i][j]->Hide();
			MG->_vGame[i][j]->FadIn(1000);
		}
	}
	return res;
}

void QueryVariante1::Resolve(){
	for (int i=0; i < _vSelect.size(); i++){
		for (int j=0; j < _vSelect[0].size(); j++) {
			_vSelect[i][j] = false;
			if (MG->_vGame[i][j]->_Color == _lpImgColor) {
				_vSelect[i][j] = true;
			}
		}
	}
}

void QueryVariante1::SetHint() {
	switch (MG->_nHintLevel) {
		case MINIJEU_HINT1:
			MG->SetHintText(_cTextHint1);
			break;
		case MINIJEU_HINT2:
			MG->SetHintText(_cTextHint2);
			break;
		default:
			break;
	}
}

// --------------------- Variante 2 --------------------------------- //

QueryVariante2::QueryVariante2(EMiniJeuMemoryPlant *game): EMemoryPlantQuery(game,KStr("PLANTMEMORY_V2_QUERY")) {
	_lpColorResponse = NULL;

	MG->SetGameType(MINIJEU_DIRECTGAME);

	strcpy(_cTextHint1,KStr("PLANTMEMORY_V2_HINT1"));
	strcpy(_cTextHint2,KStr("PLANTMEMORY_V2_HINT2"));

	for (int i=0; i < MG->_vPartSet.size(); i++) {
		bool contain = false;
		for (int j=0; j<_vColor.size(); j++) {
			if (_vColor[j] == MG->_vPartSet[i]->_Color)
				contain = true;
		}
		if (!contain)
			_vColor.push_back(MG->_vPartSet[i]->_Color);
	}

	_cTextQueryCentered = Coord (0,1024);
	float widthQuery = _lpFont->getMultilineWidth(_cTextQuery,_cTextQueryCentered.x,_cTextQueryCentered.y,_fYTextQuerry,TEXTSTYLE_CENTER);
	_cTextQueryCentered.x = (1024 - widthQuery)/ 2;
	_cTextQueryCentered.y = _cTextQueryCentered.x + widthQuery;
	_fYTextQuerry = MG->_lpGridGame->_yPos + MG->_lpGridGame->height() + 30;

	_fScaleColorBox = 0.9f;
	_lpGridColor = new Grid(_vColor.size(),1,_vColor[0]->getWidth() * _fScaleColorBox,_vColor[0]->getHeight() * _fScaleColorBox,0,5);
	_lpGridColor->Move((MG->_nW - _lpGridColor->width())/2, _fYTextQuerry + _fQueryHeight +5);

	_pBlendBox1.x = _cTextQueryCentered.x - 15;
	_pBlendBox1.y = _fYTextQuerry - 7;
	_pBlendBox2.x = _cTextQueryCentered.y + 15;
	_pBlendBox2.y = _lpGridColor->_yPos + _lpGridColor->height() - 7;
}

QueryVariante2::~QueryVariante2(void){
	XDELETE(_lpGridColor);
}

void QueryVariante2::InitQuery(){
	posQuery = Coord(GetRandom(MG->_vGame[0].size()), GetRandom(MG->_vGame.size()));
	_lpColorResponse = MG->_vGame[posQuery.y][posQuery.x]->_Color;
}

void QueryVariante2::Draw(){
	Coord pos;

	if (MG->_nGameStatus == MINIJEU_NORMAL) {
		MG->_lpGridGame->FillCell(_lpImgQuery,posQuery.x,posQuery.y,1,1);
		DrawQuery();
		for (int i=0; i<_vColor.size();i++) {	
			_lpGridColor->FillCell(_vColor[i],i,0,true,1);
		}
	} else {
		for (int i=0; i < MG->_vGame.size(); i++) {
			for (int j=0; j < MG->_vGame[0].size(); j++) {
				if (Coord(j,i) == posQuery) {
					MG->_vGame[i][j]->Draw();
				} else {
					MG->_vGame[i][j]->GetImg()->setBlitColor(0.3,0.3,0.3,1);
					MG->_vGame[i][j]->Draw();
					MG->_vGame[i][j]->GetImg()->setBlitColor(1,1,1,1);
				}
			}
		}
	}
	
	
}

void QueryVariante2::Check(){
	Coord pos;

	if (!_lpGridColor->IsMouseOver())
		return;

	Mouse.LeftEventClose();
	pos = _lpGridColor->GetMouseSelection();
	_lpColorResponse = _vColor[pos.x];
	ESoundBank::getSound("mg_select_deselect")->playSample();

	if (!MG->IsResolved())
		MG->_nGameStatus = MINIJEU_FAILED;
}

bool QueryVariante2::IsResolved() {
	for (int i=0; i < MG->_vGame.size(); i++) {
		for (int j=0; j < MG->_vGame[0].size(); j++) {
			MG->_vGame[i][j]->Hide();
			MG->_vGame[i][j]->FadIn(1000);
		}
	}
	MG->_vGame[posQuery.y][posQuery.x]->Show();

	if (_lpColorResponse == MG->_vGame[posQuery.y][posQuery.x]->_Color)
		return true;
	return false;
}

void QueryVariante2::Resolve(){
	_lpColorResponse = MG->_vGame[posQuery.y][posQuery.x]->_Color;
}

void QueryVariante2::SetHint() {
	switch (MG->_nHintLevel) {
		case MINIJEU_HINT1:
			MG->SetHintText(_cTextHint1);
			break;
		case MINIJEU_HINT2:
			MG->SetHintText(_cTextHint2);
			break;
		default:
			break;
	}
}

// Preload images et sons
void EMiniJeuMemoryPlant::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_memorygrid_bg","jpg",true,true);
   EImageBank::getImage("mg_memorygrid_case_green_color","png",true,true);
   EImageBank::getImage("mg_memorygrid_case_red_color","png",true,true);
   EImageBank::getImage("mg_memorygrid_case_yellow_color","png",true,true);
   EImageBank::getImage("mg_memorygrid_case_white_color","png",true,true);
   EImageBank::getImage("mg_memorygrid_garlic","png",true,true);
   EImageBank::getImage("mg_memorygrid_gingko","png",true,true);
   EImageBank::getImage("mg_memorygrid_pavot","png",true,true);
   EImageBank::getImage("mg_memorygrid_rue","png",true,true);
   EImageBank::getImage("mg_memorygrid_start_bouton","png",true,true);
   EImageBank::getImage("mg_memorygrid_query","png",true,true);
   EImageBank::getImage("minigame_middleage_wrong","png",true,true);
   EImageBank::getImage("mg_memorygrid_case_variante","png",true,true);

   ESoundBank::getSound("mg_apparition",false,true);
   ESoundBank::getSound("mg_counter_tic",false,true);
   ESoundBank::getSound("mg_disparition",false,true);
   ESoundBank::getSound("mg_end_counter",false,true);
}
