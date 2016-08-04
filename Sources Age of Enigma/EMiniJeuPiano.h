#ifndef EMINIJEUPIANO_H
#define EMINIJEUPIANO_H

#include "EMiniJeuBase.h"
#include "EMiniJeuHelper.h"
#include "KPTK.h"

class ScoreNote;

class EMiniJeuPiano: public EMiniJeuBase
{
public:
	EMiniJeuPiano(void);
	virtual ~EMiniJeuPiano(void);

   static void Preload();

private:	
	virtual void GameCheck();
	virtual void GameLogic();
	virtual void GameDrawSet();

	virtual bool GameIsResolve();
	virtual void GameResolve();
	virtual void GameReset();
	virtual void GameSetHint(int nHintLevel);	

   EMiniJeuPart *_lpScore;
   std::vector< int > _lpSolution;
   std::vector< Coord > _lpPianoKey;
   std::vector< KSound * > _lpNotes;
   std::vector< ScoreNote * > _lpScoreNotes;

   ScoreNote *_lpBadNote;
   Polygone _pSelClavier;

   int _nOkNote;
};

class ScoreNote: public EMiniJeuPart {
public:
   ScoreNote(KGraphic *rune_off, KGraphic *rune_on, KSound *sound, Coord pos);
   ~ScoreNote();

   KGraphic* GetImageOn();
   KGraphic* GetImageOff();
   void SetImageOn(KGraphic *imgOn);
   void SetImageOff(KGraphic *imgOff);

   void LightOn(int nDuration=700);
   void LightOff(int nDuration=700);
   void playSample();

   void displayFalse();

   void Draw(bool bDisplayBaseNote=true);

private:
   KGraphic *_lpImgNoteOff;
   KSound *_lpSound;
};
#endif