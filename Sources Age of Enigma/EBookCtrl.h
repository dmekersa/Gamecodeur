#ifndef E_BOOK_CTRL_H
#define E_BOOK_CTRL_H

#include "KPTK.h"
#include "ESmartImage.h"

// gère le livre et gère donc le taskList -> Diary et la transition entre les 2
// il faut passer par cette classe pour ouvrir le task list et diary

class ETaskList;
class EDiary;

enum CURRENT_DISPLAY_BOOK {
      TASK_LIST,
      DIARY,
      NONE
   };

class EBookCtrl {
   
public : 

   EBookCtrl();
   ~EBookCtrl();

   void Show(CURRENT_DISPLAY_BOOK typeDisplay);
   void Hide();
   void Check();
   void Logic();
   void Render();

   ETaskList* getTaskListPtr();
   EDiary* getDiaryPtr();

   bool isClosing() const;

   bool isVisible(CURRENT_DISPLAY_BOOK typeDisplay) const;

private : 

   void drawDiaryButton();
   void drawTaskButton();

   bool m_isClosing;
   // reference vers la taskList
   ETaskList *_lpTaskList;
   EDiary   *_lpDiary;

   CURRENT_DISPLAY_BOOK m_currentState;
   ESmartImage *_lpsimgTaskManager;

   KGraphic *_lpLogo;
   
   ESmartImage *_lpsimgClose;
   KGraphic* _imgClose;
   KGraphic* _imgDiary;
   KGraphic* _imgTask;

   bool _bVisible;

   KTextFace* _fontOnglet;
};
#endif