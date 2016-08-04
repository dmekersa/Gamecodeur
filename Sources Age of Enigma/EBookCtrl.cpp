#include "EBookCtrl.h"
#include "TaskList.h"
#include "EDiary.h"
#include "global.h"
#include "ImageBank.h"
#include "ESceneDirector.h"
#include "EPointer.h"

#define POS_X_BOOK 127
#define POS_Y_BOOK_INIT -650

#define POS_Y_BOOK 63

const int posXButtonClose = 915;
const int posYButtonClose = 25;

const int posXButtonDiary = 300;
const int posYButtonDiary = -30;

const int posXButtonTask = 600;
const int posYButtonTask = -27;

EBookCtrl::EBookCtrl() {
   
   _lpTaskList = new ETaskList();
   _lpDiary = new EDiary();

   _bVisible = false;
    m_isClosing = false;

   KGraphic *lpGraphicTask = EImageBank::getImage("diary/house_diary_croped","png");
   _lpsimgTaskManager = new ESmartImage(lpGraphicTask,POS_X_BOOK,POS_Y_BOOK_INIT, true, false, true, 300);
   _lpsimgTaskManager->SetVisible(true, true);

   _lpLogo = EImageBank::getImage("logo-for-notepad", "png");

   _imgClose = EImageBank::getImage("diary/onglet_diary_close", "png");
   _lpsimgClose = new ESmartImage(_imgClose, posXButtonClose, POS_Y_BOOK_INIT + posYButtonClose, true, false, true, 300);
   _lpsimgClose->SetVisible(true, true);

   _imgDiary = EImageBank::getImage("diary/onglet_diary_simple", "png");
   _imgTask = EImageBank::getImage("diary/onglet_diary_simple", "png");

   _fontOnglet = EFontBank::getFont("tahoma.ttf", 16);  

   

   m_currentState = NONE;
}

EBookCtrl::~EBookCtrl() {

   delete _lpTaskList;
   delete _lpDiary;
   delete _lpsimgTaskManager;
   delete _lpsimgClose;
   delete _fontOnglet;
}

void EBookCtrl::Check() {
   
   // test clic sur l'onglet Diary
   if(Mouse.LeftEvent() && Mouse.x >= posXButtonDiary && Mouse.x <= (posXButtonDiary + _imgDiary->getWidth()) && 
      Mouse.y >= (posYButtonDiary + _lpsimgTaskManager->getCurrentY()) && Mouse.y <= (posYButtonDiary + _imgDiary->getHeight() + _lpsimgTaskManager->getCurrentY())) {
      
      SCENEDIRECTOR->ShowDiaryManager(true);
      Mouse.EventClose();
      return;
   }

   // test clic onglet TaskList
   if(Mouse.LeftEvent() && Mouse.x >= posXButtonTask && Mouse.x <= (posXButtonTask + _imgDiary->getWidth()) && 
      Mouse.y >= (posYButtonTask + _lpsimgTaskManager->getCurrentY()) && Mouse.y <= (posYButtonTask + _imgTask->getHeight() + _lpsimgTaskManager->getCurrentY())) {     
      SCENEDIRECTOR->SetTaskManager(true);
      Mouse.EventClose();
      return;
   }

   if(m_currentState == TASK_LIST) {
      _lpTaskList->Check();
      
      if(isVisible(TASK_LIST) && Mouse.LeftEvent()) {
         if(Mouse.x < POS_X_BOOK || Mouse.x > 880 || Mouse.y < 50 || Mouse.y > 660) {

            if(! (Mouse.x >= 148 && Mouse.x <= 215 && Mouse.y >= 660 && Mouse.y <= 734))
               SCENEDIRECTOR->SetTaskManager(false);
         }
         Mouse.EventClose();
      }
   }
   if(m_currentState == DIARY)
      _lpDiary->Check();
}

bool EBookCtrl::isClosing() const {
   return m_isClosing;
}

void EBookCtrl::Logic() {
   
   // if(m_currentState == TASK_LIST)
   _lpTaskList->Logic();
   _lpDiary->Logic();
}

void EBookCtrl::drawDiaryButton() {

   _imgDiary->blitAlphaRectF(0, 0, _imgDiary->getWidth(), _imgDiary->getHeight(), posXButtonDiary, posYButtonDiary + _lpsimgClose->getCurrentY() - 25);

   _fontOnglet->setBlitColor(1.0f, 1.0f, 1.0f, 1.0f);
   _fontOnglet->setColor(1.0f, 1.0f, 1.0f, 1.0f);

   // texte diary
   _fontOnglet->drawStringCentered(KStr("UI_DIARY"), posXButtonDiary, posXButtonDiary + _imgDiary->getWidth(), posYButtonDiary + _lpsimgClose->getCurrentY() + 5 - 25, 0);
}

void EBookCtrl::drawTaskButton() {

   _imgTask->blitAlphaRectF(0, 0, _imgTask->getWidth(), _imgTask->getHeight(), posXButtonTask, posYButtonTask + _lpsimgClose->getCurrentY() - 25);

   _fontOnglet->setBlitColor(1.0f, 1.0f, 1.0f, 1.0f);
   _fontOnglet->setColor(1.0f, 1.0f, 1.0f, 1.0f);

   // texte guide
   _fontOnglet->drawStringCentered(KStr("UI_HINT"), posXButtonTask, posXButtonTask + _imgTask->getWidth(), posYButtonTask + _lpsimgClose->getCurrentY() + 5 - 25, 0);

}

void EBookCtrl::Render() {

   if(m_currentState == NONE)
      return;


   // animation livre
   if (_bVisible) {
      // La liste doit se dérouler
      if (_lpsimgTaskManager->getCurrentY() == POS_Y_BOOK_INIT) {
         m_isClosing = false;
        _lpsimgTaskManager->AnimateP2P(POS_X_BOOK, POS_Y_BOOK, 600, false, K_COUNTER_EASEOUT, 0);

        _lpsimgClose->AnimateP2P(posXButtonClose, POS_Y_BOOK + posYButtonClose, 600, false, K_COUNTER_EASEOUT, 0);
      }
      // On force le curseur souris normal
      SCENEDIRECTOR->PrepareMouseCursor(DIRECTOR_MOUSECURSOR_POINT);
      SCENEDIRECTOR->GetCurrentScene()->SetMouseOverInfo("", "");
   } else {
      // La liste doit s'enrouler
      if (_lpsimgTaskManager->getCurrentY() != POS_Y_BOOK_INIT && !_lpsimgTaskManager->isAnimateP2P()) {
         m_isClosing = true;
         _lpsimgTaskManager->AnimateP2P(POS_X_BOOK, POS_Y_BOOK_INIT, 600, false, K_COUNTER_EASEOUT, 0);
         _lpsimgClose->AnimateP2P(posXButtonClose, POS_Y_BOOK_INIT + posYButtonClose, 600, false, K_COUNTER_EASEOUT, 0);

         if (SCENEDIRECTOR->GetCurrentScene()) {
            SCENEDIRECTOR->GetCurrentScene()->ResetMouseOver();
         }
      }
   }

   // livre tout en haut
   if(_lpsimgTaskManager->getCurrentY() == POS_Y_BOOK_INIT && !_bVisible) {
      m_isClosing = false;     
      return;
   }
   
   _lpsimgClose->Draw();
//   _imgClose->blitAlphaRectF(0, 0, _imgClose->getWidth(), _imgClose->getHeight(), posXButtonClose, posYButtonClose + _lpsimgTaskManager->getCurrentY());

   if(m_currentState == TASK_LIST)
      drawDiaryButton();
   else
      drawTaskButton();

   // Dessine l'image du livre
   _lpsimgTaskManager->Draw();

   if(m_currentState != TASK_LIST)
      drawDiaryButton();
   else
      drawTaskButton();

   // dessine le logo si mode taskList
   if(m_currentState == TASK_LIST)
      _lpLogo->blitAlphaRectF(0, 0, _lpLogo->getWidth(), _lpLogo->getHeight(), 210, _lpsimgTaskManager->getCurrentY() + 70);

   

   
   // affiche l'animation du livre et ensuite affiche le task list ou diary
   if(m_currentState == TASK_LIST) {
      _lpTaskList->yOffset = _lpsimgTaskManager->getCurrentY();
      _lpTaskList->Render();
   }
   else if(m_currentState == DIARY) {
      _lpDiary->yOffset = _lpsimgTaskManager->getCurrentY();
      _lpDiary->Render();
   }
}

void EBookCtrl::Show(CURRENT_DISPLAY_BOOK typeDisplay) {
   
   if(!this->isVisible(typeDisplay))
      ESoundBank::getSound("book_pageturn")->playSample();
   m_currentState = typeDisplay;
   _bVisible = true;
   m_isClosing = false;

   if(m_currentState == TASK_LIST)
      _lpTaskList->SetMode(TASKLIST_MODE_HINT);
}

void EBookCtrl::Hide() {
   if(_bVisible) {
       m_isClosing = true;
       ESoundBank::getSound("book_pageturn")->playSample();
   }
   _bVisible = false;
  
}

ETaskList* EBookCtrl::getTaskListPtr() {
   
   return _lpTaskList;
}

EDiary* EBookCtrl::getDiaryPtr() {
   
   return _lpDiary;
}

bool EBookCtrl::isVisible(CURRENT_DISPLAY_BOOK typeDisplay) const {
   return (_bVisible && (m_currentState == typeDisplay) && (_lpsimgTaskManager->getCurrentY() != POS_Y_BOOK_INIT));
}