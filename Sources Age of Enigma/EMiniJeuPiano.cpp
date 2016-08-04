#include "assert.h"

#include "MyGame.h"
#include "ImageBank.h"
#include "SoundBank.h"
#include "EPointer.h"
#include "EGeometry.h"

#include "EMiniJeuPiano.h"

EMiniJeuPiano::EMiniJeuPiano(void)
{
	SetTextsTrad("PIANO");
   SetExitOnFail(false);
	SetGameType(MINIJEU_DIRECTGAME );
	SetFeatures(MINIJEU_OBSERVATION);
	SetBackGround(EImageBank::getImage("mg_pianobg","jpg"));
	SetGameNumber(11);

   _lpScore = new EMiniJeuPart(EImageBank::getImage("mg_piano_score.png"));
   Coord scorePos = Coord((1024 - _lpScore->Width())/2,(float)0);
   _lpScore->Move(scorePos);
   _lpScore->Hide();
   _lpScore->FadIn(500);

   _lpSolution.push_back(0);
   _lpSolution.push_back(3);
   _lpSolution.push_back(2);
   _lpSolution.push_back(1);
   _lpSolution.push_back(3);
   _lpSolution.push_back(1);

   _lpNotes.push_back(ESoundBank::getSound("pianocode_1"));
   _lpNotes.push_back(ESoundBank::getSound("pianocode_2_1"));
   _lpNotes.push_back(ESoundBank::getSound("pianocode_3"));
   _lpNotes.push_back(ESoundBank::getSound("pianocode_4_1"));

   scorePos = scorePos + Coord(150,150);
   _lpScoreNotes.push_back(new ScoreNote(EImageBank::getImage("mg_piano_rune1_off"),
      EImageBank::getImage("mg_piano_rune1_on"),_lpNotes[0],
      scorePos + Coord(_lpScoreNotes.size()*60,-_lpSolution[_lpScoreNotes.size()]*10)));
   _lpScoreNotes.push_back(new ScoreNote(EImageBank::getImage("mg_piano_rune4_off"),
      EImageBank::getImage("mg_piano_rune4_on"),
      _lpNotes[3],
      scorePos + Coord(_lpScoreNotes.size()*60,-_lpSolution[_lpScoreNotes.size()]*10)));
   _lpScoreNotes.push_back(new ScoreNote(EImageBank::getImage("mg_piano_rune3_off"),
      EImageBank::getImage("mg_piano_rune3_on"),
      _lpNotes[2],
      scorePos + Coord(_lpScoreNotes.size()*60,-_lpSolution[_lpScoreNotes.size()]*10)));
   _lpScoreNotes.push_back(new ScoreNote(EImageBank::getImage("mg_piano_rune2_off"),
      EImageBank::getImage("mg_piano_rune2_on"),
      _lpNotes[1],
      scorePos + Coord(_lpScoreNotes.size()*60,-_lpSolution[_lpScoreNotes.size()]*10)));
   _lpScoreNotes.push_back(new ScoreNote(EImageBank::getImage("mg_piano_rune4_off"),
      EImageBank::getImage("mg_piano_rune4_on"),
      ESoundBank::getSound("pianocode_4_2"),
      scorePos + Coord(_lpScoreNotes.size()*60,-_lpSolution[_lpScoreNotes.size()]*10)));
   _lpScoreNotes.push_back(new ScoreNote(EImageBank::getImage("mg_piano_rune2_off"),
      EImageBank::getImage("mg_piano_rune2_on"),
      ESoundBank::getSound("pianocode_2_2"),
      scorePos + Coord(_lpScoreNotes.size()*60,-_lpSolution[_lpScoreNotes.size()]*10)));
   
   _lpPianoKey.push_back(Coord(319,565)); 
   _lpPianoKey.push_back(Coord(507,511));
   _lpPianoKey.push_back(Coord(702,454));
   _lpPianoKey.push_back(Coord(903,397));

   _lpBadNote = new ScoreNote(EImageBank::getImage("mg_piano_rune1_off"),
      EImageBank::getImage("mg_piano_rune1_on"),
      ESoundBank::getSound("mg_pianocluster"),Coord(0,0));
   _lpBadNote->Hide();

   _pSelClavier.AddPoint(99,490);
   _pSelClavier.AddPoint(880,301);
   _pSelClavier.AddPoint(1015,382);
   _pSelClavier.AddPoint(117,651);

   Reset();
}

EMiniJeuPiano::~EMiniJeuPiano(void)
{
   _lpSolution.clear();
   _lpNotes.clear();
   _lpPianoKey.clear();

   for (int i=0; i<_lpScoreNotes.size(); i++) {
      XDELETE(_lpScoreNotes[i]);
   }
   XDELETE(_lpScore);
   XDELETE(_lpBadNote);
}

/** 3 steps rendering **/
void EMiniJeuPiano::GameCheck ()
{
   if (!Mouse.LeftEvent() || !_pSelClavier.IsMouseOver())
		return;

   for (int i=0; i<_lpPianoKey.size(); i++) {
      if (EGeometry::IsInRange(_lpPianoKey[i],20,Mouse)) {
         if (_lpSolution[_nOkNote] == i) {
            _lpScoreNotes[_nOkNote]->LightOn();
            _lpScoreNotes[_nOkNote]->playSample();
            _nOkNote++;
         } else {
            _lpBadNote->SetImageOn(_lpScoreNotes[_nOkNote]->GetImageOn());
            _lpBadNote->SetImageOff(_lpScoreNotes[_nOkNote]->GetImageOff());
            _lpBadNote->playSample();
            _lpBadNote->FadIn(300);
            _lpBadNote->Move(Coord(_lpScoreNotes[_nOkNote]->Pos().x,_lpScore->Pos().y + 150 - i*10));
            int fadDuration = 800;
            for (int i=0; i<_lpScoreNotes.size(); i++) {
               _lpScoreNotes[i]->LightOff(fadDuration - 70*i);
            }
            SetGameFailed();
         }
         IsResolved();
         return;
      }
   }

   ESoundBank::getSound("mg_forbid")->playSample();
}

void EMiniJeuPiano::GameLogic ()
{
}

void EMiniJeuPiano::GameDrawSet ()
{
   _lpScore->Draw();

   for (int i=0; i<_lpScoreNotes.size(); i++) {
      _lpScoreNotes[i]->Draw();
   }


   _lpBadNote->GetImg()->setBlitColor(1,0,0,1);
   _lpBadNote->Draw(false);
   _lpBadNote->GetImg()->setBlitColor(1,1,1,1);
}

// Retourne true si le mini jeu est résolu
bool EMiniJeuPiano::GameIsResolve()
{
   return _nOkNote >= _lpSolution.size();
}

// Resoud automatiquement le mini jeu (ou indique la bonne réponde dans le cas d'un quiz)
void EMiniJeuPiano::GameResolve() {
   
   for (int i=0; i<_lpSolution.size(); i++) {
      _lpScoreNotes[i]->LightOn();
   }
   _nOkNote = _lpSolution.size();
   IsResolved();
}

void EMiniJeuPiano::GameReset()
{
   _lpBadNote->FadOut(300);
   _nOkNote = 0;

   for (int i=0; i<_lpScoreNotes.size(); i++) {
      _lpScoreNotes[i]->LightOff(0);
   }
}

// Active un niveau d'indices (nHintLevel = 1, 2 ou 3)
void EMiniJeuPiano::GameSetHint(int nHintLevel)
{
	switch (nHintLevel) {
	  case MINIJEU_HINT1:
		  ShowHint(MINIJEU_HINT1);
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


ScoreNote::ScoreNote(KGraphic *rune_off, KGraphic *rune_on, KSound *sound, Coord pos): EMiniJeuPart(rune_on,1) {
   _lpImgNoteOff = rune_off;
   _lpSound = sound;
   Move(pos);
   Hide();
}

ScoreNote::~ScoreNote() {
}

KGraphic* ScoreNote::GetImageOn() {
   return _lpImg;
}
KGraphic* ScoreNote::GetImageOff() {
   return _lpImgNoteOff;
}
void ScoreNote::SetImageOn(KGraphic *imgOn) {
   _lpImg = imgOn;
}
void ScoreNote::SetImageOff(KGraphic *imgOff) {
   _lpImgNoteOff = imgOff;
}

void ScoreNote::playSample() {
   _lpSound->playSample();
}
void ScoreNote::LightOn(int nDuration) {
   FadIn(nDuration);
}

void ScoreNote::LightOff(int nDuration) {
   FadOut(nDuration);
}

void ScoreNote::Draw(bool bDisplayBaseNote) {
   if (bDisplayBaseNote)
      _lpImgNoteOff->blitAlphaRectF(0,0,_w,_h,_x,_y-89);
   EMiniJeuPart::Draw();
}

// Preload images et sons
void EMiniJeuPiano::Preload() {
   EMiniJeuBase::Preload();

   EImageBank::getImage("mg_pianobg","jpg",true,true);
   EImageBank::getImage("mg_piano_score.png","",true,true);
   EImageBank::getImage("mg_piano_rune1_on","",true,true);
   EImageBank::getImage("mg_piano_rune1_off","",true,true);
   EImageBank::getImage("mg_piano_rune2_on","",true,true);
   EImageBank::getImage("mg_piano_rune2_off","",true,true);
   EImageBank::getImage("mg_piano_rune3_on","",true,true);
   EImageBank::getImage("mg_piano_rune3_off","",true,true);
   EImageBank::getImage("mg_piano_rune4_on","",true,true);
   EImageBank::getImage("mg_piano_rune4_off","",true,true);

   ESoundBank::getSound("pianocode_1",false,true);
   ESoundBank::getSound("pianocode_2_1",false,true);
   ESoundBank::getSound("pianocode_3",false,true);
   ESoundBank::getSound("pianocode_4_1",false,true);
   ESoundBank::getSound("pianocode_4_2",false,true);
   ESoundBank::getSound("pianocode_2_2",false,true);
   ESoundBank::getSound("mg_pianocluster",false,true);
}