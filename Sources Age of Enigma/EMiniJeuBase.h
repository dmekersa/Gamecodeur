#ifndef EMINIJEUBASE_H
#define EMINIJEUBASE_H

#include "KCounter.h"
#include "KGraphic.h"
#include "KSound.h"
#include "KVideo.h"
#include "KGame.h"
#include "KPTK.h"
#include "ParticleFactory.h"
#include "EFont.h"

class EBox;
class ETextBox;
class EMiniJeuPart;
struct Coord;

#define MINIJEU_ZONEX_DEFAULT 260
#define MINIJEU_ZONEY_DEFAULT 100

#define MINIJEU_ZONEWIDTH_DEFAULT 680
#define MINIJEU_ZONEHEIGHT_DEFAULT 515

#define MINIJEU_LOGIC 0x01
#define MINIJEU_MEMORY 0x02
#define MINIJEU_OBSERVATION 0x04

#define MINIJEU_STR_SIZE 256

typedef enum {
	MINIJEU_PREAMBUL = -1,
	MINIJEU_NORMAL = 0,
	MINIJEU_HINT1,
	MINIJEU_HINT2,
	MINIJEU_HINT3,
	MINIJEU_RESOLVED,
	MINIJEU_FAILED,
	MINIJEU_SKIPED
} MiniJeuGameStatus;

typedef enum {
	MINIJEU_DIRECTGAME,
	MINIJEU_SUBMITGAME,
} MiniJeuGameType;

class EMiniJeuBase
{
public:

   static void init();
   static void cleanup();
   
   EMiniJeuBase(void);
	virtual ~EMiniJeuBase(void);

	/* Position à l'écran */
	void Move(int nX, int nY) {
		_nX = nX;
		_nY = nY;
		_nW = 1024 - _nX;
		_nH = 768 - _nY;
	};

	/** 3 steps rendering **/
	void Check ();
	void Logic ();
	void Draw ();

   static void Preload();

	bool IsResolved();
   bool IsShowingEnd();
	void Resolve();
	void Reset();
	
	const char* GetHintQuery();
	void ActivateHint(int nHintLevel);
	void ShowHint(int nHintLevel);

	void SetSimpleUI(bool bSimple=true) {_bSimpleUI = bSimple;}
	bool HasSimpleUI() { return _bSimpleUI;}
	
	const char* Title();
	const char* Rule();
	int   GameType() {return _nGameType;}
	int   GameStatus() {return _nGameStatus;}
	int   HintLevel() {return _nHintLevel;}
	const char* HintText(int nHintLevel);
	const char* HintQuery();

	static int X() { return _nX;}
	static int Y() { return _nY;}
	static int W() { return _nW;}
	static int H() { return _nH;}
	MiniJeuGameType Type() { return (MiniJeuGameType)_nGameType;}

	bool End(); 
	
	void SetGameNumber(int i, int numberOfVariation=1);
	void SetBackGround(KGraphic *imgBg, int x=0, int y=0);
	int GetGameNumber() {return _nGameNumber;}
   void SetExitOnFail(bool exit) {_bExitOnFail = exit;}

protected:
	static int _nX, _nY;		// Positionnement relatif du jeux
	static int _nW, _nH;		// Taille de la zone de jeux
	static KGame *_lpMyGame;	// Référence a l'instance de jeux KGame courante

	virtual void GameCheck() = 0;
	virtual void GameCheckPreambul();
	virtual void GameLogic() = 0;

	virtual void GameDrawSet() = 0;	// Dessine les composant après le dessin du backgroun
	virtual void GameDrawPreambul(); // Dessine les composant apèrs l'afficahge du plateau de jeux
	virtual void GameDrawGrab() {;}	// Dessine les composant après l'affichage du menu
	
	virtual bool GameIsResolve() = 0;
	virtual void GameResolve() =0; 
	virtual void GameSetHint(int nHintLevel) = 0;
	virtual void GameReset() = 0;
	void SetGameFailed();
	void SetGameResolved();

	void SetTextsTrad(const char* cGameTextRef);
	void SetText(char* cDst, const char* cSrc);
	
	void SetGameType(int nGameType);
	void SetTitle(const char* cTitle);
	void SetRule(const char* cRule);
	void SetHintText1(const char* cHint1);
	void SetHintQuery1(const char* cHintQuery1);
	void SetHintText2(const char* cHint2);
	void SetHintQuery2(const char* cHintQuery1);

	void SetHintText(const char* cHintText);
	
	void SetFeatures(unsigned char bFeatures);
	
	void SetHoldEndTimeout(float _fTimeout=0);
	float GetHoldEndTimeout();

	void SetSoundSuccess(KSound *sound);
	void SetSoundFail(KSound *sound);

//	virtual void DrawHint();

	void DrawEnd();
	void AnimateEnding();

	void SetRelWorldView();
	void ResetWorldView();

	void SetMultiSubmit() {_bLastSubmit = false;}
	void SetLastSubmit() {_bLastSubmit = true;}
	
	int _nVariation;
	int _nGameType;
	int _nGameStatus;
	int _nHintLevel;

private:
   static void GenerateCacheFailSuccess();

   void DrawBG();
	void DrawMenu();
   void DrawSuccess();


	KCounter *_lpAnim;

	KGraphic *_lpImgIHM;
	EMiniJeuPart *_lpImgHint1;
	EMiniJeuPart *_lpImgHint2;

	KSound*	_lpSoundFinal;
	KSound* _lpSoundSuccess;
	KSound* _lpSoundFail;
	
	int _nGameNumber;
	char _cGameNumber[3];

	bool _bSimpleUI;
	bool _bHoldOffShowEnd;
	bool _bHOSEState;
	bool _bShowRules;
	bool _bLastSubmit;
	bool _bExitOnFail;
	
	bool _bQueryHint1Show;
	bool _bQueryHint2Show;

	unsigned char _bFeatures;

	float _fSplashTimeOut;
	float _fTimeoutHoldOff;
	float _fTimeoutEndAnim;

	int _nXImgBg, _nYImgBg;
	int _nFontRuleSize;

	EBox *_lpRulesBox;
	ETextBox *_lpTextRulesBox;

	int _nNumberOfVariation;

   static EFont *_lpFontButtons;
   static bool _bPreloadDone; 

   // Victory display   
   static ParticleEmitter* _pEmitterv;
   static ParticleEmitter* _pEmitterf;

   static const char* _textTab[];	
   static KGraphic* _imgTab[];
   static Coord   _imgTabPos[];
};
#endif