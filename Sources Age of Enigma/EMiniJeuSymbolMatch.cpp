#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "FontBank.h"
#include <list>

#include "ESceneDirector.h"

#include "EMiniJeuSymbolMatch.h"

using EMiniGame::GetRandom;

EMiniJeuSymbolMatch::EMiniJeuSymbolMatch(void)
{
	SetTextsTrad("SYMBOLMATCH");

	SetGameType(MINIJEU_SUBMITGAME);
	SetGameNumber(16);
	SetBackGround(EImageBank::getImage("mg_match_bg","jpg"));

	_nGameSize = 5;
	
	_lpImgHands = EImageBank::getImage("mg_match_hands","png");

	// Insertion des symbol
	_vSet.push_back(new EMiniJeuPart(EImageBank::getImage("mg_match_symbol1","png"),1));
	_vSet.push_back(new EMiniJeuPart(EImageBank::getImage("mg_match_symbol2","png"),1));
	_vSet.push_back(new EMiniJeuPart(EImageBank::getImage("mg_match_symbol3","png"),1));
	_vSet.push_back(new EMiniJeuPart(EImageBank::getImage("mg_match_symbol4","png"),1));
	_vSet.push_back(new EMiniJeuPart(EImageBank::getImage("mg_match_symbol5","png"),1));
//	_vSet.push_back(new EMiniJeuPart(EImageBank::getImage("mg_match_symbol5","png"),1));

	// insertion des cartes
	_vCard.push_back(new EMiniJeuPart(EImageBank::getImage("mg_match_card1","png"),1));
	_vCard.push_back(new EMiniJeuPart(EImageBank::getImage("mg_match_card2","png"),1));
	_vCard.push_back(new EMiniJeuPart(EImageBank::getImage("mg_match_card3","png"),1));
	_vCard.push_back(new EMiniJeuPart(EImageBank::getImage("mg_match_card4","png"),1));
	_vCard.push_back(new EMiniJeuPart(EImageBank::getImage("mg_match_card5","png"),1));

	// Selection de la carte candidate pour le hint
	_lpHintPart1 = _vSet[2];
	_vResponse = std::vector< EMiniJeuPart * >(_vCard.size(),( EMiniJeuPart *) NULL);
	_vSolution = std::vector< Asso >(_vCard.size(), Asso());

	// positionnement des grilles
	int cardWidth = 102, cardHeight = 164, symbolWidth = 94, innerMargin = 18;
	_lpGridCard = new Grid(_vCard.size(),1,cardWidth,cardHeight,0,innerMargin);
	_lpGridCard->Move(193,141);

	innerMargin = (cardWidth - symbolWidth) + innerMargin;
	_lpGridResponse = new Grid(_vResponse.size(),1,94,94,0,innerMargin);
	_lpGridResponse->Move(_lpGridCard->_xPos + (_lpGridCard->width() - _lpGridResponse->width())/2,
		_lpGridCard->_yPos + _lpGridCard->height()+9);

	// Creation des association cartes symbols
	for (int i=0; i< _vCard.size(); i++) {
		_vSolution[i].card = _vCard[i];
		_vSolution[i].symbol = _vSet[i];

		_vCard[i]->SwitchDrag();
		_vCard[i]->SwitchRotation();
		_vSet[i]->SwitchRotation();
	}


	RandomCard();
	RandomSet();

	_lpGridSet = NULL;
	_lpGrabedPart = NULL;

	Reset();
}

EMiniJeuSymbolMatch::~EMiniJeuSymbolMatch(void)
{
	for (int i=0; i<_vCard.size(); i++) {
		XDELETE(_vResponse[i]);
		XDELETE(_vCard[i]);
	}
	for (int i=0; i<_vSet.size(); i++) {
		XDELETE(_vSet[i]);
	}
	
	XDELETE(_lpGridResponse);
	XDELETE(_lpGridSet);
	XDELETE(_lpGridCard);

	XDELETE(_lpGrabedPart);
}

/** 3 steps rendering **/
void EMiniJeuSymbolMatch::GameCheck ()
{
	Coord pos;
	EMiniJeuPart *tmp;
	
	if (!Mouse.LeftEvent())
		return;

	if (_lpGrabedPart != NULL) {
		Mouse.LeftEventClose();

		if (!(_lpGridSet->IsMouseOver() || _lpGridResponse->IsMouseOver() || _lpGridCard->IsMouseOver())) {
			//Remise au pot
			for (int i=0; i<_vSet.size(); i++) {
				if (_vSet[i] == NULL) {
					_vSet[i] = _lpGrabedPart;
					_lpGrabedPart->Release();
					_lpGrabedPart->Move(_lpGridSet->GetCellPos(i,0),300);
					_lpGrabedPart = NULL;
					ESoundBank::getSound("mg_select_deselect")->playSample();
					return;
				}
			}
		}

		pos = _lpGridResponse->GetMouseSelection();
		if (pos.x == -1)
			pos = _lpGridCard->GetMouseSelection();
		if (pos.x >=0) {
			tmp = _vResponse[pos.x]; 
			if (tmp != NULL && !tmp->IsDragable()) {
				ESoundBank::getSound("mg_forbid")->playSample();
				return;
			}
			_lpGrabedPart->Release();
			_lpGrabedPart->Move(_lpGridResponse->GetCellPos(pos),300);
			_vResponse[pos.x] = _lpGrabedPart; 
			_lpGrabedPart = tmp;

			if (_lpGrabedPart != NULL) {
				_lpGrabedPart->Grab();
				ESoundBank::getSound("mg_swap")->playSample();	
			} else {
				ESoundBank::getSound("mg_select_deselect")->playSample();
			}
			return;
		}
		
		pos = _lpGridSet->GetMouseSelection();
		if (pos.x >=0) {
			tmp = _vSet[pos.x]; 
			_lpGrabedPart->Release();
			_lpGrabedPart->Move(_lpGridSet->GetCellPos(pos),300);
			_vSet[pos.x] = _lpGrabedPart;
			_lpGrabedPart = tmp;
			

			if (_lpGrabedPart != NULL) {
				_lpGrabedPart->Grab();
				ESoundBank::getSound("mg_swap")->playSample();	
			} else {
				ESoundBank::getSound("mg_select_deselect")->playSample();
			}
			return;
		}
	}

	if (!Mouse.LeftEvent() || _lpGrabedPart != NULL)
		return;
	
	if (_lpGridResponse->IsMouseOver()) {
		Mouse.LeftEventClose();

		pos = _lpGridResponse->GetMouseSelection();
		if (_vResponse[pos.x] != NULL) {
			if (!_vResponse[pos.x]->IsDragable()) {
				ESoundBank::getSound("mg_forbid")->playSample();
				return;
			}
			_lpGrabedPart = _vResponse[pos.x];
			_lpGrabedPart->Grab();
			_vResponse[pos.x] = NULL;
			ESoundBank::getSound("mg_select_deselect")->playSample();
		}
	}

	if (_lpGridSet->IsMouseOver()) {
		Mouse.LeftEventClose();

		pos = _lpGridSet->GetMouseSelection();
		if (_vSet[pos.x] != NULL) {
			_lpGrabedPart = _vSet[pos.x];
			_lpGrabedPart->Grab();
			_vSet[pos.x] = NULL;
			ESoundBank::getSound("mg_select_deselect")->playSample();
		}
	}
}

void EMiniJeuSymbolMatch::GameLogic ()
{
	if (_lpGrabedPart != NULL)
		_lpGrabedPart->Logic();
}

void EMiniJeuSymbolMatch::GameDrawSet()
{	

//	_lpGridCard->DrawBaseGrid(_lpImgHands);
//	_lpGridResponse->DrawBaseGrid(_lpImgHands);
//	_lpGridSet->DrawBaseGrid(_lpImgHands);

	for (int i=0; i<_vCard.size(); i++) {
		_vCard[i]->Draw();
	}

	_lpImgHands->blitAlphaRect(0,0,_lpImgHands->getWidth(),_lpImgHands->getHeight(),66,0);

	for (int i=0; i<_vSet.size(); i++) {
		if (_vSet[i] != NULL)
			_vSet[i]->Draw();
	}
	for (int i=0; i<_vResponse.size(); i++) {
		if (_vResponse[i] != NULL)
			_vResponse[i]->Draw();
	}
}
void EMiniJeuSymbolMatch::GameDrawGrab() {
	for (int i=0; i<_vSet.size(); i++) {
		if (_vSet[i] != NULL && !_vSet[i]->TransformComplete())
			_vSet[i]->Draw();
	}
	for (int i=0; i<_vResponse.size(); i++) {
		if (_vResponse[i] != NULL && !_vResponse[i]->TransformComplete())
			_vResponse[i]->Draw();
	}
	if (_lpGrabedPart != NULL)
		_lpGrabedPart->Draw();
}
void EMiniJeuSymbolMatch::GameReset() {

	Coord pos;

	// Reset du plateau
	for (int i=0; i<_vSolution.size(); i++) {
		// Solution fixé
		if (_vResponse[i] == NULL || !_vResponse[i]->IsDragable())
			continue;

		// placement de la pièce dans le set de jeux
		for (int j=0; j<_vSet.size(); j++) {
			if (_vSet[j] == NULL) {
				if (_lpGrabedPart != NULL) {
					_vSet[j] = _lpGrabedPart;
					_lpGrabedPart->Release();
					_lpGrabedPart = NULL;
					continue;
				}
				_vSet[j] = _vResponse[i];
				_vResponse[i] = NULL;
				break;
			}
		}
	}

	XDELETE(_lpGridSet);
	_lpGridSet = new Grid(_vSet.size(),1,_vSet[0]->Width(),_vSet[0]->Height(),0,0);
	_lpGridSet->Move((_nW - _lpGridSet->width())/2,_lpGridResponse->_yPos + _lpGridResponse->height() + 20);

	// Déplacement des pièces
	for (int i=0; i< _vSet.size(); i++) {
		if (_vSet[i] != NULL)
			_vSet[i]->Move(_lpGridSet->GetCellPos(i,0),300);
	}
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuSymbolMatch::GameIsResolve()
{
	//for (int i=0; i<_vResponse.size(); i++) {
	//	if (_vResponse[i] == NULL || _vCard[i] != GetCard(_vResponse[i]))
	//		return false;
	//}
	for (int i=0; i<_vResponse.size(); i++) {
		if (_vResponse[i] == NULL || i != GetResponseIndex(_vResponse[i]))
			return false;
	}
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuSymbolMatch::GameResolve()
{
	EMiniJeuPart *tmpSymb = NULL;
	int posSol;
	for (int i=0; i<_vResponse.size();) {
		posSol = GetResponseIndex(_vResponse[i]);
		if ( posSol<0 || i == posSol) {
			i++;
			continue;
		}
		tmpSymb = _vResponse[posSol];
		_vResponse[posSol] = _vResponse[i];
		_vResponse[i]=tmpSymb;
		_vResponse[posSol]->Move(_lpGridResponse->GetCellPos(posSol,0),300);
	}
	for (int i=0; i<_vSet.size();i++) {
		posSol = GetResponseIndex(_vSet[i]);
		if (posSol >= 0) {
			_vResponse[posSol] = _vSet[i];
			_vSet[i] = NULL;
			_vResponse[posSol]->Move(_lpGridResponse->GetCellPos(posSol,0),300);
		}
	}
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuSymbolMatch::GameSetHint(int nHintLevel)
{
	EMiniJeuPart *tmp = NULL;
	int indexSol = 0;

	switch (nHintLevel) {
		case MINIJEU_HINT1:
			ShowHint(MINIJEU_HINT1);

			while (GetCard(_lpHintPart1) != _vCard[indexSol] && indexSol < _vCard.size())
				indexSol++;

			//suppression des références a la pièce hint
			for (int i=0; i<_vSet.size(); i++) {
				if (_vSet[i] == _lpHintPart1) {
					_vSet[i] = NULL;
					break;
				} else if (i < _vResponse.size() && _vResponse[i] == _lpHintPart1) {
					_vResponse[i] = NULL;
					break;
				}
			}

			tmp = _vResponse[indexSol];
			if (tmp != NULL) {
				_vResponse[indexSol] = NULL;
				// place occupé
				for (int i=0; i<_vSet.size(); i++) {
					if (_vSet[i] == NULL) {
						_vSet[i] = tmp;
						tmp->Move(_lpGridSet->GetCellPos(i,0),300);
						break;
					}
				}
			}
			
			_lpHintPart1->Move(_lpGridResponse->GetCellPos(indexSol,0),300);
			_lpHintPart1->SwitchDrag();
			_vResponse[indexSol] = _lpHintPart1;

			tmp = _vSet[_vSet.size() -1];
			_vSet.pop_back();
			indexSol = _vSet.size() -1;
			while (tmp != NULL) {
				if (_vSet[indexSol] == NULL) {
					_vSet[indexSol] = tmp;
					break;
				}
				indexSol--;
			}
			Reset(); 
			break;
		case MINIJEU_HINT2:
			ShowHint(MINIJEU_HINT2);
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

EMiniJeuPart* EMiniJeuSymbolMatch::GetCard(EMiniJeuPart *symbol) {
	if (symbol != NULL) {
		for (int i=0; i<_vSolution.size(); i++) {
			if (*(_vSolution[i].symbol) == *(symbol))
				return _vSolution[i].card;
		}
	}
	return NULL;
}
int EMiniJeuSymbolMatch::GetResponseIndex(EMiniJeuPart *symbol) {
	EMiniJeuPart *card = GetCard(symbol);
	if (card != NULL) {
		for (int i=0; i<_vCard.size(); i++) {
			if (*(_vCard[i]) == *(card))
				return i;
		}
	}
	return -1;
}
void EMiniJeuSymbolMatch::RandomCard() {
	std::list< EMiniJeuPart * > tmp;
	std::list< EMiniJeuPart * >::iterator it;
	
	// preparation pour la randomisation de la position initial des cartes
	for (int i=0; i<_vCard.size(); i++) {
		tmp.push_back(_vCard[i]);
		_vCard[i] = NULL;
	}

	int pos;
	while (tmp.size() >0) {
		pos = GetRandom(_vCard.size());
		if (_vCard[pos] != NULL)
			continue;

		_vCard[pos] = *(tmp.rbegin());
		tmp.pop_back();
	}
	
	// Positionnement des carte
	for (int i=0; i< _vCard.size(); i++) {
		_vCard[i]->Move(_lpGridCard->GetCellPos(i,0));
	}

}
void EMiniJeuSymbolMatch::RandomSet() {
	std::list< EMiniJeuPart * > tmp;
	std::list< EMiniJeuPart * >::iterator it;
	
	for (int i=0; i<_vSet.size(); i++) {
		tmp.push_back(_vSet[i]);
		_vSet[i] = NULL;
	}

	int pos;
	while (tmp.size() >0) {
		pos = GetRandom(_vSet.size());
		if (_vSet[pos] != NULL)
			continue;

		_vSet[pos] = *(tmp.rbegin());
		tmp.pop_back();
	}
}

// Preload images et sons
void EMiniJeuSymbolMatch::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_match_bg","jpg",true,true);
   EImageBank::getImage("mg_match_hands","png",true,true);
   EImageBank::getImage("mg_match_symbol1","png",true,true);
   EImageBank::getImage("mg_match_symbol2","png",true,true);
   EImageBank::getImage("mg_match_symbol3","png",true,true);
   EImageBank::getImage("mg_match_symbol4","png",true,true);
   EImageBank::getImage("mg_match_symbol5","png",true,true);
   EImageBank::getImage("mg_match_symbol5","png",true,true);
   EImageBank::getImage("mg_match_card1","png",true,true);
   EImageBank::getImage("mg_match_card2","png",true,true);
   EImageBank::getImage("mg_match_card3","png",true,true);
   EImageBank::getImage("mg_match_card4","png",true,true);
   EImageBank::getImage("mg_match_card5","png",true,true);
}
