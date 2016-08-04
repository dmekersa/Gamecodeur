#ifndef EMINIJEUPICROSS_H
#define EMINIJEUPICROSS_H

#include "eminijeubase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class EMiniJeuPicross:
	public EMiniJeuBase
{
public:
	EMiniJeuPicross (void);
	virtual ~EMiniJeuPicross (void);

	/** 3 steps rendering **/
	void Check ();
	void Logic ();
	void Draw ();

	// Retourne true si le mini jeu est résolu
	virtual bool IsResolved ();

	// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
	virtual void Resolve ();

	// Active un niveau d'indices (_nHintLevel = 1, 2 ou 3)
	virtual void ActivateHint (int HintLevel); 


   static void Preload();

private:
	static const int _nTextMargin = 15;
	
	KGraphic * _lpGraphicTarget;
	KGraphic * _lpGraphicToken;
	EFont * _lpHintFont;

	std::vector < std::vector < bool > >_vCurrentGame;

	Grid * _lpGrid;
	Grid * _lpHintLineGrid;
	Grid * _lpHintColGrid;
	std::vector < std::string > _vHintLine;
	std::vector < std::string > _vHintCol;

	Grid * _lpHintGrid;
	
	int _nCol, _nLine, _nHintCol;
	float _nWidthCell, _nMaxSizeText, gameBlend;
};
#endif
