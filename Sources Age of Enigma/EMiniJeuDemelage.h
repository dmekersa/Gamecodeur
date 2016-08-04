#ifndef EMINIJEUDEMELAGE_H
#define EMINIJEUDEMELAGE_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EMiniJeuDemelage:
	public EMiniJeuBase
{
public:
	EMiniJeuDemelage(void);
	virtual ~EMiniJeuDemelage(void);

   static void Preload();

private:
	struct PartLink {
		EMiniJeuPart *first;
		EMiniJeuPart *last;

		PartLink() {
			first = NULL;
			last = NULL;
		}
		PartLink(EMiniJeuPart *f, EMiniJeuPart *l) {
			first = f;
			last = l;
		}
	};

	/** 3 steps rendering **/
	void GameCheck ();
	void GameLogic ();
	void GameDrawSet();

	bool GameIsResolve();
	void GameResolve();
	void GameReset();
	void GameSetHint(int nHintLevel);
	
	void SetLinks(int nHintLevel);
	EMiniJeuPart* GetCommonPoint(PartLink link1, PartLink link2);

	void DrawPart(EMiniJeuPart *part);
	void DrawLink(PartLink *link);
	void DrawActiveLink();
	
	EMiniJeuPart *_lpPentagram;
	float _fRayon;
	float _fLinkBlend;

	KGraphic *_lpPartHalo;
	Coord _relPosHalo;

	KGraphic *_lpImgLink;
	EMiniJeuPart *_lpGrabedPart;

	std::vector< EMiniJeuPart * > _vPoints;
	std::vector< PartLink > _vLinks;

	std::vector< Coord > _vFinalPos;

	//EFont *_lpFontT;
	float _fDetectionDistance;
};
#endif