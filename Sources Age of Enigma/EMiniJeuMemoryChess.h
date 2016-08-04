#ifndef EMINIJEUMEMORYCHESS_H
#define EMINIJEUMEMORYCHESS_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"
#include "EBox.h"

class EMemoryChessQuery;
class EMemoryChessboard;

class EMiniJeuMemoryChess :
	public EMiniJeuBase
{
public:
	EMiniJeuMemoryChess(void);
	virtual ~EMiniJeuMemoryChess(void);

   static void Preload();

private:
	/** 3 steps rendering **/
	void GameCheck ();
	void GameLogic ();
	void GameDrawSet ();

	bool GameIsResolve();
	void GameResolve();
	void GameReset();
	void GameSetHint(int nHintLevel);

	void SetQuery();

	float _fShowTimeDuration;

	EFont *_lpFont;
	EFont *_lpFontStart;
	EFont *_lpFontTimeout;

	char _cTextStart[254];
	char _cTextTimeout[254];
	char _cTextTimeoutSeconds[254];
	int _nSecondRemaining;

	KGraphic *_lpImgBg;
	KGraphic *_lpImgMask;
	EMiniJeuPart *_lpImgStart;

	bool _bBlockReset;
	bool _bDisplayStart;
	bool _bMaskSet;
	KCounter *_lpShowTime;

	int _nNumQuery, _nCurrentQuery;
	int _nResponse;
	EMemoryChessQuery *_qCurrentQuery;

	std::vector< EMemoryChessboard * > _vChessboards;
	std::vector< EMemoryChessQuery * > _vQuerySet;
	std::vector< EMemoryChessQuery * > _vQuery;
};


class EMemoryChessQuery {
public:
	
	typedef enum {
		SELECT,
		NUMERIC
	} QueryType;
	
	EMemoryChessQuery(const char *cQuery, int nAnswer, int nQueryMark = -1, int nType=SELECT);
	~EMemoryChessQuery(void);

	void Reset();
	void Check();
	bool IsValidAnswer(int res);
	bool ValidAnswer() {return _bGoodAnswer;}
	void Draw();
	
	void SetYPos(int y);

	int _nType;
	int _nQueryMark;
	int _nAnswer;

private:
	EFont *_lpFontQuery;
	int _nFontSize;

	float _fQueryZoneSize;
	float _fQueryTextWidth;
	char _cQuery[256];
	
	bool _bGoodAnswer;
	
	float _y;
	Coord _posImgQuery;
	int _nResponse;

	EBox *_lpQueryBox;
	KGraphic *_lpImg;
	Digit *_Digit;
//	Coord _pBlendBox1, _pBlendBox2;
};

class EMemoryChessboard : public EMiniJeuPart {
public:
	EMemoryChessboard(char *imgName, int x, int y);
	~EMemoryChessboard(void);
	
	void Draw();
	bool IsMouseOver();
	void Logic();

	void FadIn(double fDuration);
	void FadOut(double fDuration);

	void Select(bool fad=true);
	void DeSelect(bool fad=true);
	void Reset();
	bool _bShowQueryMask;

private: 
	KGraphic *_lpImgMask;
	KGraphic *_lpImgMaskQuery;
	KGraphic *_lpImgSelect;

	KCounter *_lpSelectFad;
	float _fAlphaSelect;
	bool _bSelect;

	std::vector< bool > _lpArrayMask;
	float _fAlphaMask;
};
#endif
