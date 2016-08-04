#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "FontBank.h"
#include "EGeometry.h"
#include "math.h"

#include "EMiniJeuDemelage.h"

EMiniJeuDemelage::EMiniJeuDemelage(void)
{
	SetTextsTrad("DEMELAGE");
	SetGameType(MINIJEU_DIRECTGAME);
	SetBackGround(EImageBank::getImage("mg_demelage_bg.jpg"));
	SetGameNumber(24);
	
//	_lpFontT = EFontBank::getFont("acmesa.ttf",10);
	_lpPentagram = new EMiniJeuPart(EImageBank::getImage("mg_demelage_pentagram","png"));
	_lpPentagram->Move(332,190);
	_lpPentagram->Hide();
	_fRayon = _lpPentagram->Width()/2 + 80;

	_lpImgLink = EImageBank::getImage("mg_demelage_link","png");
	_fLinkBlend =1;
	
	_vPoints.push_back(new EMiniJeuPart(EImageBank::getImage("mg_demelage_part1","png"),1,0));
	_vFinalPos.push_back(Coord(58,2));
	_vPoints.push_back(new EMiniJeuPart(EImageBank::getImage("mg_demelage_part1","png"),1,0));
	_vFinalPos.push_back(Coord(244,2));
	_vPoints.push_back(new EMiniJeuPart(EImageBank::getImage("mg_demelage_part1","png"),1,0));
	_vFinalPos.push_back(Coord(151,70));
	_vPoints.push_back(new EMiniJeuPart(EImageBank::getImage("mg_demelage_part1","png"),1,0));
	_vFinalPos.push_back(Coord(94,111));
	_vPoints.push_back(new EMiniJeuPart(EImageBank::getImage("mg_demelage_part1","png"),1,0));
	_vFinalPos.push_back(Coord(209,111));
	_vPoints.push_back(new EMiniJeuPart(EImageBank::getImage("mg_demelage_part1","png"),1,0));
	_vFinalPos.push_back(Coord(2,178));
	_vPoints.push_back(new EMiniJeuPart(EImageBank::getImage("mg_demelage_part1","png"),1,0));
	_vFinalPos.push_back(Coord(115,178));
	_vPoints.push_back(new EMiniJeuPart(EImageBank::getImage("mg_demelage_part1","png"),1,0));
	_vFinalPos.push_back(Coord(187,178));
	_vPoints.push_back(new EMiniJeuPart(EImageBank::getImage("mg_demelage_part1","png"),1,0));
	_vFinalPos.push_back(Coord(301,178));
	_vPoints.push_back(new EMiniJeuPart(EImageBank::getImage("mg_demelage_part1","png"),1,0));
	_vFinalPos.push_back(Coord(151,287));

	_fDetectionDistance = _vPoints[0]->Width()/2;
	_lpPartHalo = EImageBank::getImage("mg_demelage_part1_halo","png");
	_relPosHalo = Coord((_vPoints[0]->Width() - _lpPartHalo->getWidth())/2, (_vPoints[0]->Height() - _lpPartHalo->getHeight())/2);

	for (int i=0; i<_vFinalPos.size(); i++) {
		_vFinalPos[i] = _vFinalPos[i] + _lpPentagram->Pos() + Coord(29,43);
		_vPoints[i]->MoveCenterTo(_vFinalPos[i]);
		_vPoints[i]->SwitchRotation();
		_vPoints[i]->Hide();
		_vPoints[i]->FadIn(500);
	}

	_lpGrabedPart = NULL;

	SetLinks(_nGameStatus);
	Reset();
}

EMiniJeuDemelage::~EMiniJeuDemelage(void)
{
	for (int i=0; i<_vPoints.size(); i++) {
		XDELETE(_vPoints[i]);
	}
	XDELETE(_lpPentagram);
//   XDELETE(_lpFontT)
}

/** 3 steps rendering **/
void EMiniJeuDemelage::GameCheck ()
{
	EMiniJeuPart *tmp;
	Coord pos;

	if (!Mouse.LeftEvent() )
		return;

	if (_lpGrabedPart != NULL) {
		Mouse.LeftEventClose();
		tmp = NULL;
		pos = _lpGrabedPart->Center();

		for (int i=		0; i<_vFinalPos.size(); i++) {
			if (EGeometry::Distance(pos,_vFinalPos[i]) < _fDetectionDistance)
				pos = _vFinalPos[i];
		}

		for (int i=0; i<_vPoints.size(); i++) {
			if (_vPoints[i] != _lpGrabedPart && _vPoints[i]->IsOver(pos)) {
				tmp = _vPoints[i];
				if (!tmp->IsDragable()) {
					ESoundBank::getSound("mg_forbid")->playSample();
					continue;
				}
				break;
			}
		}
		if (tmp != NULL && !tmp->IsDragable()) {
			ESoundBank::getSound("mg_forbid")->playSample();
			return;
		}

		_lpGrabedPart->MoveCenterTo(pos);
		_lpGrabedPart->Release();

		if (tmp != NULL) {
			tmp->Grab();
			tmp->Move(_lpGrabedPart->Pos());
			ESoundBank::getSound("mg_swap")->playSample();
			_lpGrabedPart = tmp;
			return;
		}
		
		ESoundBank::getSound("mg_select_deselect")->playSample();
		_lpGrabedPart = NULL;
		IsResolved();
		return;
	}

	for (int i=0; i<_vPoints.size(); i++) {
		if (_vPoints[i]->IsMouseOver()) {
			if (!_vPoints[i]->IsDragable()) {			
				ESoundBank::getSound("mg_forbid")->playSample();
				return;
			}
			tmp = _vPoints[i];
			tmp->Grab();
			_lpGrabedPart = tmp;
			ESoundBank::getSound("mg_select_deselect")->playSample();
			return;
		}
	}


}

void EMiniJeuDemelage::GameLogic()
{
	_lpPentagram->Logic();
	if (_lpGrabedPart != NULL)
		_lpGrabedPart->Logic();
}

void EMiniJeuDemelage::GameDrawSet()
{
	Coord pos1,pos2;
	
	// Traitement de la pièce saisie hors zone de jeux
	if (_lpGrabedPart != NULL) {
		if (!EGeometry::IsInRange(_lpPentagram->Center(),_fRayon,Mouse)) {
			if (_lpGrabedPart->IsGrab())
				_lpGrabedPart->Release();
			_lpGrabedPart->MoveCenterTo(EGeometry::GetSegmentEnd(_lpPentagram->Center(),_fRayon,EGeometry::Angle(_lpPentagram->Center(),Mouse)));
		} else if (!_lpGrabedPart->IsGrab()) {
			_lpGrabedPart->Grab();
		}
	}

	// gestion de la fin de jeux
	_fLinkBlend = 1 - _lpPentagram->GetAlpha();
	_lpPentagram->Draw();

	for (int i=0; i<_vLinks.size(); i++) {
		if (_vLinks[i].first == _lpGrabedPart || _vLinks[i].last == _lpGrabedPart)
			continue;
		DrawLink(&(_vLinks[i]));
	}
	for (int i=0; i<_vPoints.size(); i++) {
		DrawPart(_vPoints[i]);
	}
	for (int i=0; i<_vLinks.size(); i++) {
		if (_vLinks[i].first != _lpGrabedPart && _vLinks[i].last != _lpGrabedPart)
			continue;
		DrawLink(&(_vLinks[i]));		
		if (_vLinks[i].first == _lpGrabedPart) {
			DrawPart(_vLinks[i].last); 
		} else {
			DrawPart(_vLinks[i].first);
		}
	}

	if (_lpGrabedPart != NULL)
		DrawPart(_lpGrabedPart);

	// Debug
	//for (int i=0; i < 720; i++) {
	//	pos1 = EGeometry::GetSegmentEnd(_lpPentagram->Center(),_fRayon,i/2);
	//	_lpImgLink->plotPixel(pos1.x,pos1.y,1,0,0,1);
	//}

	//// Vérifie qu'il n'y a pas d'intersection de segments
	//EMiniJeuPart *orig, *a,*b;
	//PartLink tmp1, tmp2;
	//for (int i=0; i<_vLinks.size()-1; i++) {
	//	for (int j=i+1; j<_vLinks.size(); j++) {
	//		tmp1 = _vLinks[i];
	//		tmp2 = _vLinks[j];

	//		// test de surcharge de liens
	//		orig = GetCommonPoint(tmp1,tmp2);
	//		if (orig != NULL) {
	//			a = tmp1.last;
	//			if (orig != tmp1.first) 
	//				a = tmp1.first;
	//			b = tmp2.last;
	//			if (orig != tmp2.first)
	//				b = tmp2.first;

	//			float angle = EGeometry::Angle(a->Center(),orig->Center(),b->Center());
	//			if (abs(angle) < 10) {
	//				if (abs(angle) < 1) {
	//					_lpImgLink->drawLine(a->Center().x,a->Center().y,orig->Center().x,orig->Center().y,1,1,1,1);
	//					_lpImgLink->drawLine(b->Center().x,b->Center().y,orig->Center().x,orig->Center().y,1,1,1,1);
	//				}
	//			}
	//		}
	//	}
	//}
}

void EMiniJeuDemelage::GameReset() {
	_lpGrabedPart = NULL;
	Coord pos, pos2;
m0:	for (int i=0; i<_vPoints.size(); i++) {
		_vPoints[i]->Release();
		if (!_vPoints[i]->IsDragable()) {
			continue;
		}
		_vPoints[i]->StopMove();
		_vPoints[i]->Move(_lpPentagram->Center());
m1:		int t = 0;

		float angle = GetRandom(0,360);
		float distance = GetRandom(0, _fRayon);
		pos = EGeometry::GetSegmentEnd(_lpPentagram->Center(),distance,angle);
		//pos = Coord(_posPentagram.x + EMiniGame::GetRandom(_lpPentagram->getWidth()+50) - 100 , _posPentagram.y + EMiniGame::GetRandom(_lpPentagram->getHeight()+ 50) - 100);
		//pos = Coord(EMiniGame::GetRandom(_nW-100)+50 , EMiniGame::GetRandom(_nH-50)+25);

		for (int j=0; j<i; j++) {
			pos2 = _vPoints[j]->GetDest();
			pos2 = Coord(pos2.x + _vPoints[j]->Width()/2, pos2.y + _vPoints[j]->Height()/2);
			if (EGeometry::Distance(pos,pos2) < (1.5 * _vPoints[0]->Width())) {
				t++;
				if (t> 10)
					goto m0;
				goto m1;
			}
		}
		_vPoints[i]->MoveCenterTo(pos,1500);
		t = 0;
	}
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuDemelage::GameIsResolve()
{
	// Vérifie que toutes les pièces ont bien été posé au dessus du pentagramme
	for (int i=0; i<_vFinalPos.size(); i++) {
		bool isOver = false;
		for (int j=0; j<_vPoints.size(); j++) {
			if (_vPoints[j]->IsOver(_vFinalPos[i])) {
				isOver = true;
				break;
			}
		}
		if (!isOver)
			return false;
	}

	// Vérifie qu'il n'y a pas d'intersection de segments
	EMiniJeuPart *orig, *a,*b;
	PartLink tmp1, tmp2;
	for (int i=0; i<_vLinks.size()-1; i++) {
		for (int j=i+1; j<_vLinks.size(); j++) {
			tmp1 = _vLinks[i];
			tmp2 = _vLinks[j];

			// test de surcharge de liens
			orig = GetCommonPoint(tmp1,tmp2);
			if (orig != NULL) {
				a = tmp1.last;
				if (orig != tmp1.first) 
					a = tmp1.first;
				b = tmp2.last;
				if (orig != tmp2.first)
					b = tmp2.first;

				float angle = EGeometry::Angle(a->Center(),orig->Center(),b->Center());
				if (abs(angle) < 10)
					if (abs(angle) < 1)
						return false;
				continue;
			}
			if ( i == 4 && j == 11) {
				itos(1);
			}
			if (EGeometry::IsSegmentIntersect(
				tmp1.first->Center().x,tmp1.first->Center().y,tmp1.last->Center().x,tmp1.last->Center().y,
				tmp2.first->Center().x,tmp2.first->Center().y,tmp2.last->Center().x,tmp2.last->Center().y,false)) {
				return false;
			}
		}
	}
	_lpPentagram->FadIn(600);
	return true;
}

// Resoud automatiquement le mini jeu (ou indique la bonne rŽponde dans le cas d'un quiz)
void EMiniJeuDemelage::GameResolve()
{
	for (int i=0; i<_vPoints.size(); i++) {
		_vPoints[i]->MoveCenterTo(_vFinalPos[i],200);
		if (_vPoints[i]->IsDragable()) {
			_vPoints[i]->SwitchDrag();
		}
	}
	_lpPentagram->FadIn(600);
	_nGameStatus = MINIJEU_RESOLVED;
   SetGameResolved();
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuDemelage::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
		case MINIJEU_HINT1:
			SetLinks(nHintLevel);
			ShowHint(MINIJEU_HINT1);
			Reset(); 
			break;
		case MINIJEU_HINT2:
			ShowHint(MINIJEU_HINT2);
			_vPoints[0]->MoveCenterTo(_vFinalPos[0],500);
			_vPoints[0]->Release(); _vPoints[0]->SwitchDrag();
			
			_vPoints[1]->MoveCenterTo(_vFinalPos[1],500);
			_vPoints[1]->Release(); _vPoints[1]->SwitchDrag();
			
			_vPoints[5]->MoveCenterTo(_vFinalPos[5],500);
			_vPoints[5]->Release(); _vPoints[5]->SwitchDrag();
			
			_vPoints[8]->MoveCenterTo(_vFinalPos[8],500);
			_vPoints[8]->Release(); _vPoints[8]->SwitchDrag();
			
			_vPoints[9]->MoveCenterTo(_vFinalPos[9],500);
			_vPoints[9]->Release(); _vPoints[9]->SwitchDrag();
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

void EMiniJeuDemelage::DrawPart(EMiniJeuPart *part) {
	Coord posHalo = part->Pos() + _relPosHalo;
	part->Draw();
	_lpPartHalo->blitAlphaRectFx(0,0,_lpPartHalo->getWidth(),_lpPartHalo->getHeight(),posHalo.x,posHalo.y,0,1,1);

}
void EMiniJeuDemelage::DrawLink(PartLink *link) {
	Coord pos1 = link->first->Center(), pos2 = link->last->Center();
	Coord cLink = Coord(pos1.x + (pos2.x - pos1.x)/2, pos1.y + (pos2.y - pos1.y)/2);
	cLink.y += _lpImgLink->getHeight()/2;
	float linkLenght = EGeometry::Distance(pos1, pos2);

	//_lpImgLink->drawLine(pos1.x,pos1.y,pos2.x,pos2.y,1,1,1,1);
	_lpImgLink->blitAlphaRectFxF(0,0,linkLenght,_lpImgLink->getHeight(),cLink.x - linkLenght/2, cLink.y - _lpImgLink->getHeight(),EGeometry::Angle(pos1,pos2),1,_fLinkBlend);
}

void EMiniJeuDemelage::SetLinks(int nHintLevel) {
	_vLinks.clear();
	if (nHintLevel <= MINIJEU_NORMAL) {
		_vLinks.push_back(PartLink(_vPoints[0],_vPoints[1]));
		_vLinks.push_back(PartLink(_vPoints[0],_vPoints[2]));
		_vLinks.push_back(PartLink(_vPoints[0],_vPoints[3]));
		_vLinks.push_back(PartLink(_vPoints[0],_vPoints[5]));
		_vLinks.push_back(PartLink(_vPoints[1],_vPoints[2]));
		_vLinks.push_back(PartLink(_vPoints[1],_vPoints[4]));
		_vLinks.push_back(PartLink(_vPoints[1],_vPoints[8]));
		_vLinks.push_back(PartLink(_vPoints[2],_vPoints[3]));
		_vLinks.push_back(PartLink(_vPoints[2],_vPoints[4]));
		_vLinks.push_back(PartLink(_vPoints[3],_vPoints[5]));
		_vLinks.push_back(PartLink(_vPoints[3],_vPoints[6]));
		_vLinks.push_back(PartLink(_vPoints[4],_vPoints[7]));
		_vLinks.push_back(PartLink(_vPoints[4],_vPoints[8]));
		_vLinks.push_back(PartLink(_vPoints[5],_vPoints[6]));
		_vLinks.push_back(PartLink(_vPoints[5],_vPoints[9]));
		_vLinks.push_back(PartLink(_vPoints[6],_vPoints[7]));
		_vLinks.push_back(PartLink(_vPoints[6],_vPoints[9]));
		_vLinks.push_back(PartLink(_vPoints[7],_vPoints[8]));
		_vLinks.push_back(PartLink(_vPoints[7],_vPoints[9]));
		_vLinks.push_back(PartLink(_vPoints[8],_vPoints[9]));
	} else {
		_vLinks.push_back(PartLink(_vPoints[0],_vPoints[2]));
		_vLinks.push_back(PartLink(_vPoints[0],_vPoints[3]));
		_vLinks.push_back(PartLink(_vPoints[1],_vPoints[2]));
		_vLinks.push_back(PartLink(_vPoints[1],_vPoints[4]));
		_vLinks.push_back(PartLink(_vPoints[2],_vPoints[3]));
		_vLinks.push_back(PartLink(_vPoints[2],_vPoints[4]));
		_vLinks.push_back(PartLink(_vPoints[3],_vPoints[5]));
		_vLinks.push_back(PartLink(_vPoints[3],_vPoints[6]));
		_vLinks.push_back(PartLink(_vPoints[4],_vPoints[7]));
		_vLinks.push_back(PartLink(_vPoints[4],_vPoints[8]));
		_vLinks.push_back(PartLink(_vPoints[5],_vPoints[6]));
		_vLinks.push_back(PartLink(_vPoints[6],_vPoints[7]));
		_vLinks.push_back(PartLink(_vPoints[6],_vPoints[9]));
		_vLinks.push_back(PartLink(_vPoints[7],_vPoints[8]));
		_vLinks.push_back(PartLink(_vPoints[7],_vPoints[9]));
	}
}

EMiniJeuPart* EMiniJeuDemelage::GetCommonPoint(PartLink link1, PartLink link2) {
	if (link1.first == link2.first || link1.first == link2.last) return link1.first;
	if (link1.last == link2.first || link1.last == link2.last)   return link1.last;
	return NULL;
}


// Preload images et sons
void EMiniJeuDemelage::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_demelage_bg.jpg","",true,true);
   EImageBank::getImage("mg_demelage_pentagram","png",true,true);
   EImageBank::getImage("mg_demelage_link","png",true,true);
   EImageBank::getImage("mg_demelage_part1","png",true,true);
   EImageBank::getImage("mg_demelage_part1","png",true,true);
   EImageBank::getImage("mg_demelage_part1","png",true,true);
   EImageBank::getImage("mg_demelage_part1","png",true,true);
   EImageBank::getImage("mg_demelage_part1","png",true,true);
   EImageBank::getImage("mg_demelage_part1","png",true,true);
   EImageBank::getImage("mg_demelage_part1","png",true,true);
   EImageBank::getImage("mg_demelage_part1","png",true,true);
   EImageBank::getImage("mg_demelage_part1","png",true,true);
   EImageBank::getImage("mg_demelage_part1","png",true,true);
   EImageBank::getImage("mg_demelage_part1_halo","png",true,true);
}
