#include "math.h"
#include <algorithm>

#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "KRandom.h"
#include "EGeometry.h"

#include "global.h"

#include "EMiniJeuMikado.h"
using namespace EGeometry;

EMiniJeuMikado::EMiniJeuMikado(void)
{
	SetTextsTrad("MIKADOMG");
	SetGameType(MINIJEU_DIRECTGAME);
	SetFeatures(MINIJEU_OBSERVATION);
	SetGameNumber(4);
	SetBackGround(EImageBank::getImage("emikado_bg","jpg"),0,0);

	_nGameSize = 10;
	//_test = KPTK::createKGraphic();

	_fCc = Coord(_nW/2 +130,_nH/2);		// Centre de la couronne
	_fCi = 100;				// Rayon interne de la couronne
	_fCe = 200;				// Rayon externe de la couronne

	EMikadoPart *tmp = new EMikadoPart();
	_fPartWidth = tmp->GetWidth();
	XDELETE(tmp);

	Reset();
}

EMiniJeuMikado::~EMiniJeuMikado(void)
{
	//XDELETE(_test);

	for (int i = 0; i <(int)_vSolution.size(); i++)
		XDELETE(_vSolution[i]);
}

/** 3 steps rendering **/
void EMiniJeuMikado::GameCheck ()
{

	if (Mouse.RightEvent()) {
		std::list< EMikadoPart * > tmpList;
		EMikadoPart *tmp = NULL;
		for (int i=_vSolution.size() -1; i >= 0; i--) {
			if (_vSolution[i] != NULL && _vSolution[i]->IsSelected()) {
				tmpList.push_back(_vSolution[i]);
			}
		}
		bool skip = false;
		if (tmpList.size() > 1) {
			skip = true;
			tmp = *(tmpList.rbegin());
			tmpList.pop_back();

			while (tmpList.size() > 0)
				tmpList.pop_back();
		}
		if (! skip) {
			_nGameSize +=1;
			tmp = NULL;
			tmp = GetRandomPart();
			_vSolution[_vSolution.size() -1] = tmp;
		}
		Mouse.LeftEventClose();
		return;
	}

	if (!Mouse.LeftEvent())
		return;


	for (int i=_vSolution.size() -1; i >= 0; i--) {
		if (_vSolution[i]->IsRemoved())
			continue;
		
		_vSolution[i]->Check();
		if (!Mouse.LeftEvent()) {
			ESoundBank::getSound("mg_select_deselect")->playSample();
			IsResolved();
		}
	}
}


void EMiniJeuMikado::GameLogic ()
{
}

void EMiniJeuMikado::GameDrawSet ()
{

	for (int i=0; i< _vSolution.size(); i++) {
		_vSolution[i]->Draw();
	}

	//Coord pos;
	//for (int i=0; i< 760; i++) {
	//	pos = EGeometry::GetSegmentEnd(_fCc,_fCi,i/2);
	//	_test->plotPixel(pos.x,pos.y,1,0,0,1);
	//	pos = EGeometry::GetSegmentEnd(_fCc,_fCe,i/2);
	//	_test->plotPixel(pos.x,pos.y,1,1,0,1);
	//}
}

// Retourne true si le mini jeu est rŽsolu
bool EMiniJeuMikado::GameIsResolve()
{
	std::list<EMikadoPart * > lActivePart;
	std::list<EMikadoPart * >::iterator it;
	EMikadoPart *selectedPart;

	for (int i=_vSolution.size() -1; i >= 0; i--) {
		if (_vSolution[i] != NULL && !_vSolution[i]->IsRemoved()) {
			if (_vSolution[i]->IsSelected()) {
				selectedPart = _vSolution[i];
				for (it = lActivePart.begin(); it != lActivePart.end(); it++) {
					if (Intersect(selectedPart,(*it))) {
						_nGameStatus = MINIJEU_FAILED;
						return false;
					}
				}
				selectedPart->Remove();
			} else {
				lActivePart.push_back(_vSolution[i]);
			}
		}
	}

	if (lActivePart.size() == 0) {
		return true;
	}

	return false;
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuMikado::GameResolve()
{
	for (int i=_vSolution.size() -1; i >= 0; i--) {
		_vSolution[i]->Select();
		IsResolved();
	}
}

void EMiniJeuMikado::GameReset2()
{	
	// LIberation des objets
	for (int i = 0; i <(int)_vSolution.size(); i++)
		XDELETE(_vSolution[i]);

	_vSolution = std::vector< EMikadoPart * >(0, (EMikadoPart *) NULL);

	_vSolution.push_back( new EMikadoPart(571,279,97));
	_vSolution.push_back(new EMikadoPart(584,344,45));
	_vSolution.push_back(new EMikadoPart(626,375,170));
	if (_nGameSize == 3)
		return;
	_vSolution.push_back(new EMikadoPart(612,379,90));
	_vSolution.push_back(new EMikadoPart(550,380,10));
	if (_nGameSize == 5)
		return;
	_vSolution.push_back(new EMikadoPart(514,347,150));
	_vSolution.push_back(new EMikadoPart(629,391,70));
}

void EMiniJeuMikado::GameReset()
{
	// LIberation des objets
	for (int i = 0; i <(int)_vSolution.size(); i++)
		XDELETE(_vSolution[i]);

	// Submit Game
	_vSolution = std::vector< EMikadoPart * >(_nGameSize, (EMikadoPart *) NULL);
	for(int i=0; i<_nGameSize; i++) {
		_vSolution[i] = GetRandomPart();
	 }
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuMikado::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  ShowHint(MINIJEU_HINT1);
		  for (int i = 0; i <(int)_vSolution.size(); i++)
			  XDELETE(_vSolution[i]);
		  _nGameSize -= 2;
		  Reset(); 
		  break;
	  case MINIJEU_HINT2:
		  ShowHint(MINIJEU_HINT2);
		  for (int i = 0; i <(int)_vSolution.size(); i++)
			  XDELETE(_vSolution[i]);
		  _nGameSize -= 2;
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

EMikadoPart* EMiniJeuMikado::GetRandomPart() {
	Coord cTmp;
	EMikadoPart* tmpPart=NULL;
	bool bGoodPart = false, bIntersect = false;

	while (_nGameSize > _vSolution.size()) {
		_vSolution.push_back((EMikadoPart *) NULL);
	}
	// Boucle de validation de pièce  
	while(bGoodPart == false || bIntersect == false) {
		bGoodPart = true;
		bIntersect = false;

		// Genere une pièce avec au minimum  point centré dans la couronne de placement
r0:		Coord cTmp = GetRandomPos();
		double dAngle = KRandom::getRandomFloat() * 360;
		tmpPart = new EMikadoPart(cTmp.x,cTmp.y,dAngle);
		if (!IsInRange(_fCc,_fCi,tmpPart->c)) {
			XDELETE(tmpPart);
			goto r0;
		}

		return tmpPart;
		
		// Vérification de non empiètement sur les extrémités des segment précédant
		int i=0;
		for (i=0; i < _nGameSize; i++) {
			if (_vSolution[i] == NULL)
				break;
			if (IsInRange(_vSolution[i]->GetPoint1(), 60, tmpPart->GetPoint1()))
				bGoodPart = false;
			if (IsInRange(_vSolution[i]->GetPoint1(), 60, tmpPart->GetPoint2()))
				bGoodPart = false;
			if (IsInRange(_vSolution[i]->GetPoint2(), 60, tmpPart->GetPoint1()))
				bGoodPart = false;
			if (IsInRange(_vSolution[i]->GetPoint2(), 60, tmpPart->GetPoint2()))
				bGoodPart = false;
			if (Intersect(tmpPart,_vSolution[i]))
					bIntersect = true;
		}

		if (i==0)
			bIntersect = true;

		// Effacement de la pièce si non correct
		if (bGoodPart == false || bIntersect == false)
				XDELETE(tmpPart);
	}
	return tmpPart;
}

Coord EMiniJeuMikado::GetRandomPos() {
	Coord pos;

	// generation de la distance au centre de la couronne
	float fDistance = KRandom::getRandomFloat() * _fCi ;
	
	pos = EGeometry::GetSegmentEnd(_fCc,fDistance,KRandom::getRandomFloat()*360);
	return pos;
}

bool EMiniJeuMikado::Intersect(EMikadoPart* A, EMikadoPart* B) {
	Coord S;
	if(! IsSegmentIntersect(A->a, A->b, B->a, B->b))
		return false;
	return true;
}

Coord EMiniJeuMikado::LineIntersection(EMikadoPart* A, EMikadoPart* B) {
	return EGeometry::LineIntersection(A->a, A->b, B->a, B->b);
}


// ----------------------------------------- //
EMikadoPart::EMikadoPart(float x, float y, double dAngle) {
	_dAngle = dAngle;
	_bSelected = false;

	c = Coord(x,y);
	a = Coord(0,0);
	b = Coord(0,0);

	int img = (int)(KRandom::getRandomFloat() *3) %3;
	switch (img) {
		case 0:
			_lpPart = EImageBank::getImage("emikado_part1","png");
			_lpPartSelected = EImageBank::getImage("emikado_part1selected","png");
			break;
		case 1:
			_lpPart = EImageBank::getImage("emikado_part2","png");
			_lpPartSelected = EImageBank::getImage("emikado_part2selected","png");
			break;
		case 2:
			_lpPart = EImageBank::getImage("emikado_part3","png");
			_lpPartSelected = EImageBank::getImage("emikado_part3selected","png");
			break;
		default:
			KAssert(false);
	}

	KAssert(_lpPart != NULL && _lpPartSelected != NULL);

	_dPartLength = _lpPart->getWidth();
	_dDistance = _lpPart->getHeight() / 2;
	KAssert(_dPartLength > 0 && _dDistance > 0);

	SetAngle(_dAngle);

	_bRemoved = false;
}

EMikadoPart::~EMikadoPart(void) {
	_lpPart = NULL;
}

void EMikadoPart::SetAngle(double dAngle) {
	_dAngle = dAngle;
	// Forbid special case
	//if (fabs(_dAngle) < 0.001 || fabs(_dAngle - 180) < 0.001) 
	//	_dAngle += 0.002;

	// Détermination des coordonnée de l'extrémité de la pièce, repère 0,0
	b = EGeometry::GetSegmentEnd(c,_dPartLength/2,_dAngle);
	a = EGeometry::GetSegmentEnd(b,_dPartLength/2,_dAngle+180);
	a = EGeometry::GetSegmentEnd(b,_dPartLength,_dAngle+180);

	// Détermination du coéficient directeur de la droite.
	_dCoefDir = b.y / b.x;

	// Calcul du coéficient directeur de la droite horthogonal a la pièce
	_dPCoefDir = - (1 / _dCoefDir);
	_dPOrdOrig = a.y - (_dPCoefDir * a.x);

	// Détermination de l'ordonnée a l'origine
	_dOrdOrig = b.y - (_dCoefDir * b.x);

	// Determination des points de l'objets.
	double dX, dY;
	dX = sqrt( pow(_dDistance,2) / ( pow(_dPCoefDir,2) +1));
	dY = dX * _dPCoefDir;

	p1 = EGeometry::GetSegmentEnd(a,_dDistance,_dAngle + 90);
	p2 = EGeometry::GetSegmentEnd(b,_dDistance,_dAngle + 90);
	p3 = EGeometry::GetSegmentEnd(b,_dDistance,_dAngle - 90);
	p4 = EGeometry::GetSegmentEnd(a,_dDistance,_dAngle - 90);
}

void EMikadoPart::Move(float x, float y) {
	c.x = x;
	c.y = y;

	SetAngle(_dAngle);
}

void EMikadoPart::Check() {
	if (Mouse.LeftEvent()) {
		if (IsClicked()) {
			Mouse.LeftEventClose();
			_bSelected = ! _bSelected;
		}
	}
}

void EMikadoPart::Draw() {
	if (_bRemoved)
		return;

	KGraphic *tmpImg = _lpPart;
	if (IsSelected()) {
		tmpImg = _lpPartSelected;
	}
	tmpImg->blitArbitraryQuad(0,0,(float)_dPartLength,0,(float)_dPartLength,(float) (_dDistance*2),0,(float)_dDistance,p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,p4.x,p4.y);

	/*tmpImg->drawLine(p1.x,p1.y,p4.x,p4.y,0,0,1,1);
	tmpImg->drawLine(p1.x,p1.y,p2.x,p2.y,0,0,1,1);
	tmpImg->drawLine(p2.x,p2.y,p3.x,p3.y,0,0,1,1);
	tmpImg->drawLine(p3.x,p3.y,p4.x,p4.y,0,0,1,1);*/


	//tmpImg->drawLine(a.x,a.y,b.x,b.y,0,0,1,1);
}

bool EMikadoPart::IsClicked() {
	Coord cProj = Coord(Mouse.x, Mouse.y);
	Coord pos = EGeometry::GetSegmentEnd(cProj,400,_dAngle+90);

	cProj = EGeometry::LineIntersection(cProj,pos,a,b);
	float distance = EGeometry::Distance(Mouse,cProj);
	if (distance >= _dDistance)
		return false;

	// Test d'appartenance au segment de droite correspondant a la pièce
	if ( (a.x < b.x) && (a.x > cProj.x || b.x < cProj.x)) 
		return false;
	if ( (a.x >= b.x) && (b.x > cProj.x || a.x < cProj.x))
		return false;
	if ( (a.y < b.y) && (a.y > cProj.y || b.y < cProj.y)) 
		return false;
	if ( (a.y >= b.y) && (b.y > cProj.y || a.y < cProj.y))
		return false;

	// Calcule de la distance du click souris par rapport au segment de droite correspondant a la pièce.
	double Distance = sqrt(pow(cProj.y - Mouse.y,2) + pow(cProj.x - Mouse.x,2));
	if ( _dDistance >= Distance ) 
		return true;

	return false;
}

// Preload images et sons
void EMiniJeuMikado::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("emikado_bg","jpg",true,true);
   EImageBank::getImage("emikado_part1","png",true,true);
   EImageBank::getImage("emikado_part1selected","png",true,true);
   EImageBank::getImage("emikado_part2","png",true,true);
   EImageBank::getImage("emikado_part2selected","png",true,true);
   EImageBank::getImage("emikado_part3","png",true,true);
   EImageBank::getImage("emikado_part3selected","png",true,true);
}
