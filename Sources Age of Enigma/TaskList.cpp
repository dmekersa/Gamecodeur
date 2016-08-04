/*
 *  TaskList.cpp
 *  enigma
 *
 *  Created by Rockford on 11/01/11.
 *  Copyright 2011 Casual Games France. All rights reserved.
 *
 */

#include "TaskList.h"
#include "FontBank.h"
#include "GlobalBank.h"
#include "ImageBank.h"
#include "MyGame.h"
#include "ESceneDirector.h"
#include "ESequenceTextFade.h"
#include <math.h>
#include "EFancyString.h"
#include "EFancyStringMultiline.h"
#include "EFancyStringAnim.h"
#include "EPointer.h"

#define PAGE_2 440

#define DECALAGE_X_OBJECTIVE PAGE_2
#define DECALAGE_X_QUESTION PAGE_2 + 15
#define DECALAGE_X_ANSWER PAGE_2
#define WIDTH_MAX_STRING 330
// 165 pour avoir la même hauteur qu'avant
// modifier selon la nouvelle taille de l'image de fond
#define POS_Y_NO_THANKS 550
#define POS_Y_NB_HINT 165

#define POS_X_BOOK 106
#define POS_Y_BOOK_INIT -600
#define POS_Y_BOOK 50

ETaskList::ETaskList()
{
   _pVectListObjectiv = NULL;
   m_needCalcPosition = true;
   m_pSequenceTextFade = NULL;

   m_hasClicked = false;

   memset(_szUniverseName, 0, 256);
   
   _bVisible = false;
   _eMode = TASKLIST_MODE_NORMAL;

   yOffset = 0;

//   _lpFontBold28 = EFontBank::getFont("bellb.ttf", 28);
//   _lpFont28 = EFontBank::getFont("bell.ttf", 28);   
//   _lpFont16 = EFontBank::getFont("bell.ttf", 16);  
//   _lpFontBold20 = EFontBank::getFont("bellb.ttf", 20);
//   _lpFont20 = EFontBank::getFont("bell.ttf", 20);   
   _lpFontBold28 = EFontBank::getFont("armadillo.ttf", 28);
   _lpFont28 = EFontBank::getFont("armadillo.ttf", 28);   
   _lpFont16 = EFontBank::getFont("armadillo.ttf", 16);  
   _lpFontBold20 = EFontBank::getFont("armadillo.ttf", 20);
   _lpFont20 = EFontBank::getFont("armadillo.ttf", 20);   
}

ETaskList::~ETaskList() {
   XDELETE(m_pSequenceTextFade);
   XDELETE(_pVectListObjectiv);
   XDELETE(_lpFontBold28)
   XDELETE(_lpFont28);   
   XDELETE(_lpFont16);  
   XDELETE(_lpFontBold20);
   XDELETE(_lpFont20);
}

bool ETaskList::isVisible() const {
   
   return false;
}

void ETaskList::Check()
{
   // si la liste des objectifs est null, on la remplie
   if(!_pVectListObjectiv)
      this->maj();

   if(m_pSequenceTextFade) {
      m_pSequenceTextFade->Check();
   }
}

void ETaskList::Logic() {
   
   if(m_pSequenceTextFade) {
      m_pSequenceTextFade->Logic();
   }
}

void ETaskList::maj() {

   // remplir liste d'objectifs -> inutile de le faire à chaque frame, donc le faire au moment de l'affichage
   fillListObjective();
}

void ETaskList::addObjective(const char* objective) {
   
   if(!_pVectListObjectiv)
      _pVectListObjectiv = new std::vector<Objective>();

   if(addObjectiveToList(objective, true)) {
     XDELETE(m_pSequenceTextFade);
     m_pSequenceTextFade = new ESequenceTextFade(1024/2, 768/2, (std::string(getTrad("UI_TASK_NEWOBJECTIF")) + std::string(getTrad(objective))).c_str());
     m_pSequenceTextFade->Start();
     m_needCalcPosition = true;
   }
}

void ETaskList::Render()
{
   float r,v,b;

   // :dmekersa:20110323 On n'affiche plus la liste de tâche, qui sert uniquement à obtenir des indices maintenant
   if (_eMode == TASKLIST_MODE_NORMAL) {
      // :dmekersa:20110408 On n'affiche plus le guide en mode normal
      return;
   }

   float y = 0;
   float x = 105;

   y = 30.0f + yOffset;

   // Titre en mode Hint
   if (_eMode == TASKLIST_MODE_HINT) {

      _lpFont28->setColor(0.0f, 0.0f, 0.0f, 1.0f);
      _lpFont28->setBlend(1.0f);
      _lpFont28->drawMultiline(KStr("UI_TASK_GUIDE"), 330-(400/2), 330+(400/2), yOffset + 250, TEXTSTYLE_CENTER);

      _lpFont16->setColor(0.0f, 0.0f, 0.0f, 1.0f);
      _lpFont16->setBlend(1.0f);
      _lpFont16->drawMultiline(KStr("UI_TASK_HINTAVAILABLE"), 310-(400/2), 330+(400/2), yOffset + 280, TEXTSTYLE_CENTER);
   }

   KTextFace *lpFont = NULL;

   if (_eMode == TASKLIST_MODE_HINT)
      lpFont = _lpFontBold20;

   if(m_needCalcPosition) {
      calcPosition(lpFont);
   }

   lpFont->setColor(1.0f, 1.0f, 1.0f, 1.0f);
   lpFont->setBlend(1.0f);

   // Position de la souris
   short mx = KInput::getMouseX();
   short my = KInput::getMouseY();

   if(KInput::getLeftButtonState() == false)
      m_hasClicked = false;

   unsigned int size = 0;
   // tableau temporaire de pointeur d'objet graphique string
   std::vector<GraphicalStringTrad*> vectGraphicalString;

   if(_pVectListObjectiv) {
      size = this->_pVectListObjectiv->size();

      for(unsigned int i = 0; i < size; ++i) {
         vectGraphicalString.push_back(&_pVectListObjectiv->at(i).graphSzObjective);

         std::vector<QuestionAnswer> *vectQuestionAnswer = &_pVectListObjectiv->at(i).vectQuestionAnswer;
         for(unsigned int j = 0; j < vectQuestionAnswer->size(); ++j) {

            vectGraphicalString.push_back(&vectQuestionAnswer->at(j).graphSzQuestion);

            // si la réponse est affichée
            if(vectQuestionAnswer->at(j).graphSzAnswer.isVisible())
               vectGraphicalString.push_back(&vectQuestionAnswer->at(j).graphSzAnswer);
         }
      }
   }

   for(unsigned int i = 0; i < vectGraphicalString.size(); ++i) {
      GraphicalStringTrad *graphString = vectGraphicalString.at(i);

      // Test de survol
      bool bOver = false;
      if (_eMode == TASKLIST_MODE_HINT) {
         float mw = lpFont->getStringWidth(graphString->getTrad().c_str());
         int height = lpFont->getHeightPix();

         if (mx > (x + graphString->x) && mx <= (x+mw + graphString->x) && my > (y + graphString->y) && my < (y+height + graphString->y)) {
            bOver = true;
            graphString->setColor(0.7f, 0.7f, 0.7f);
         }
         else
            graphString->setColor(0.0f, 0.0f, 0.0f);
      }

      graphString->getRenderer()->x = x + graphString->x;
      graphString->getRenderer()->y = y + graphString->y;
      graphString->render();

      // Test de clic
      if (bOver && KInput::getLeftButtonState() == true) {

         if(m_hasClicked == false) {
            m_hasClicked = true;
            // check si on a cliqué sur une question
            clicOnTaskList(mx, my, x, y, lpFont);
         }
      }
      if (_eMode != TASKLIST_MODE_NORMAL)
         y += lpFont->getHeightPix() * /*1*/ 0.2f;
   }

   // Affichage du lien "Non merci"
   if (_eMode == TASKLIST_MODE_HINT) {
      bool bOverQuitTaskList = false;
      if (mx > 630 && mx <= 770 && my > (POS_Y_NO_THANKS+yOffset) && my < (POS_Y_NO_THANKS+yOffset)+lpFont->getHeightPix()) {
         bOverQuitTaskList = true;
         r = 0.7f;
         v = 0.7f;
         b = 0.7f;
      }
      else {
         r = 0.0f;
         v = 0.0f;
         b = 0.0f;
      }

      _lpFontBold28->setBlend(0.9f);
      _lpFontBold28->setBlitColor(r, v, b, 1.0f);
      _lpFontBold28->drawMultiline(KStr("UI_TASKHINTNO"), 700-(135/2), 700+(135/2), POS_Y_NO_THANKS+yOffset, TEXTSTYLE_CENTER);

      // !!!:dmekersa:20110415 Pas de test d'input dans un Render... Uniquement dans Check()
      // clic sur le non merci
      // quit aussi si on clic ailleur que sur le tasklist, peut etre utile si l'utilisateur n'a pas compris qu'il fallait le ferme en cliquant sur non merci
      if ((bOverQuitTaskList && KInput::getLeftButtonState() == true)) {
         SCENEDIRECTOR->SetTaskManager(false);
         //        Mouse.LeftEventClose();
      }
   }
}

void ETaskList::fillListObjective() {
   
   XDELETE(_pVectListObjectiv);
   _pVectListObjectiv = new std::vector<Objective>();

   // chaine univers
   char szUniverseTag[256] = {0};
   // On construit un bout de chaine de format "_univers_"
   // Car les tâches sont nommées ainsi : task_univers_nomdetask
   strcpy(szUniverseTag, "_");
   strcat(szUniverseTag, _szUniverseName);
   strcat(szUniverseTag, "_");

   // Parcours la liste pour en récupérer les valeurs
   EGlobalBank *o;
   bool bGotNext = EGlobalBank::g_ListGlobal.getHead (o);
   bool bHaveObjectives = false;

   while (bGotNext) {
      // stocke les objectifs
      // verifie l'univers et que l'objectif est available
      if ((o->_bTask && strstr(o->_szName,"objective_") == o->_szName && strstr(o->_szName,"question_") == NULL) 
          && strcmp(o->_szValue,"0") == 0 && strstr(o->_szName, szUniverseTag)) {
         addObjectiveToList(o->_szName, false);
         bHaveObjectives = true;
      }
      bGotNext = EGlobalBank::g_ListGlobal.getNext (o);
   }

   // stocke les questions
   bGotNext = EGlobalBank::g_ListGlobal.getHead (o);

   while (bGotNext) {
      // verifie l'univers et que la question est available
      if ((o->_bTask && strstr(o->_szName,"objective_") == NULL && strstr(o->_szName,"question_") == o->_szName) 
         && strcmp(o->_szValue,"0") == 0 && strstr(o->_szName, szUniverseTag)) {
         
         assignQuestionToObjective(o->_szName);
      }
      bGotNext = EGlobalBank::g_ListGlobal.getNext (o);
   }
   
   if (!bHaveObjectives) {
      addObjectiveToList("objective_none");
   }

   m_needCalcPosition = true;
}

void ETaskList::assignQuestionToObjective(const std::string & question) {
   unsigned int sizeVector = _pVectListObjectiv->size();

   // cherche l'index du "_" de "question_"
   int indexFirst_ = question.find_first_of("_");

   // on copie la chaine sans le "question_" et sans le dernier "_..."
   const std::string questionWithout_ = question.substr(indexFirst_ + 1, question.find_last_of("_") - (indexFirst_+1));
   
   // on cherche l'objectif correspondant à la question
   for(unsigned int i = 0; i < sizeVector; ++i) {
      std::string szTempObjective = _pVectListObjectiv->at(i).graphSzObjective.getString();
      szTempObjective.replace(0, std::string("objective_").size(), "");

      // test si la question se rapporte à cet objectif
      if(szTempObjective == questionWithout_) {

         QuestionAnswer questionAnswer;
         questionAnswer.graphSzQuestion.setString(question);
         questionAnswer.graphSzQuestion.setTrad(std::string("- ") + std::string(getTrad(question.c_str())));

         // stocke la réponse aussi
         std::string answer = std::string("answer_") + question.substr(indexFirst_ + 1);
         questionAnswer.graphSzAnswer.setString(answer);
         questionAnswer.graphSzAnswer.setTrad(std::string(getTrad(answer.c_str())));
         questionAnswer.graphSzAnswer.setColor(0.20f, 0.21f, 0.46f);
         questionAnswer.graphSzAnswer.setFreezeColor(true);

         // par defaut on n'affiche pas la réponse
         questionAnswer.graphSzAnswer.setVisible(false);

         // alors on le rajoute à la liste des questions s'y rapportant
         _pVectListObjectiv->at(i).vectQuestionAnswer.push_back(questionAnswer);
         return;
      }
   }
}

void ETaskList::calcPosition(KTextFace *lpFont) {
   
   float currentY = 0.0f;

   if(!_pVectListObjectiv)
      return;

   m_needCalcPosition = false;

   // parcourt les objectifs
   for(unsigned int i = 0; i < _pVectListObjectiv->size(); ++i) {

      Objective *objective = &_pVectListObjectiv->at(i);
      objective->graphSzObjective.x = DECALAGE_X_OBJECTIVE;
      objective->graphSzObjective.y = currentY;
      
      if(/*_oldFontUsed != lpFont || */!objective->graphSzObjective.getRenderer())
         objective->graphSzObjective.setRenderer(new EFancyStringMultiline(objective->graphSzObjective.getTrad().c_str(), _lpFontBold20, 0, WIDTH_MAX_STRING, 0));

      currentY += objective->graphSzObjective.getRenderer()->getHeight() + 5;

      for(unsigned int j = 0; j < objective->vectQuestionAnswer.size(); ++j) {

         QuestionAnswer *questionAnswer = &_pVectListObjectiv->at(i).vectQuestionAnswer.at(j);
         questionAnswer->graphSzQuestion.x = DECALAGE_X_QUESTION;
         questionAnswer->graphSzQuestion.y = currentY;

         if(!questionAnswer->graphSzQuestion.getRenderer())
            questionAnswer->graphSzQuestion.setRenderer(new EFancyStringMultiline(questionAnswer->graphSzQuestion.getTrad().c_str(), _lpFont20, 0, WIDTH_MAX_STRING, 0));

         currentY += questionAnswer->graphSzQuestion.getRenderer()->getHeight() + 5;
         
         if(questionAnswer->graphSzAnswer.isVisible()) {
            questionAnswer->graphSzAnswer.x = DECALAGE_X_ANSWER;
            questionAnswer->graphSzAnswer.y = currentY;

            if(!questionAnswer->graphSzAnswer.getRenderer())
               questionAnswer->graphSzAnswer.setRenderer(new EFancyStringAnim(questionAnswer->graphSzAnswer.getTrad().c_str(), _lpFont20, 0, WIDTH_MAX_STRING, 0));
        
            currentY += questionAnswer->graphSzAnswer.getRenderer()->getHeight() + 10;
            EFancyStringAnim* anim = dynamic_cast<EFancyStringAnim*>(questionAnswer->graphSzAnswer.getRenderer());
            if(anim)
               m_needCalcPosition = !anim->animEnded();
         }
      }
   }
}

void ETaskList::clicOnTaskList(int x, int y, int containerX, int containerY, KTextFace *lpFont) {
   
   if(!_pVectListObjectiv)
      return;

   // ne veut récupérer que les clics sur les questions

   // parcourt les objectifs
   for(unsigned int i = 0; i < _pVectListObjectiv->size(); ++i) {
      //parcourt les questions
      for(unsigned int j = 0; j < _pVectListObjectiv->at(i).vectQuestionAnswer.size(); ++j) {
         QuestionAnswer *questionAnswer = &_pVectListObjectiv->at(i).vectQuestionAnswer.at(j);
         
         const char* charTrad = questionAnswer->graphSzQuestion.getTrad().c_str();
         float mw = lpFont->getStringWidth(charTrad);
         int height = lpFont->getMultilineHeight(charTrad, 0, WIDTH_MAX_STRING, 0, TEXTSTYLE_LEFT);

         if (x > (containerX + questionAnswer->graphSzQuestion.x) && x <= (containerX+mw + questionAnswer->graphSzQuestion.x) && 
            y > (containerY + questionAnswer->graphSzQuestion.y) && y < (containerY+height+ questionAnswer->graphSzQuestion.y)) {

            m_needCalcPosition = true;

            // si la réponse était visible, on la cache
            if(questionAnswer->graphSzAnswer.isVisible()) {
               questionAnswer->graphSzAnswer.setVisible(false);
               EFancyStringAnim* inst = dynamic_cast<EFancyStringAnim*>(questionAnswer->graphSzAnswer.getRenderer());
               if(inst)
                  inst->resetAnim();
               return;
            }

            // si clique sur une autre question, cache toute les réponses pour qu'il n'y en ait pas 2 d'ouvertes en même temps
            hideAllAnswer();

      //      char szTaskPaid[256] = {0};
            // affichage réponse
       //     strcpy(szTaskPaid, questionAnswer->graphSzQuestion.getString().c_str());
       //     strcat(szTaskPaid, "_hintdone");

            // Si on a assez d'énergie ou si la tache avait déjà été achetée
            // La tache n'avait pas déjà été achetée
         //   if (!EGlobalBank::isExist(szTaskPaid)) {
               // Non ! On enregistre la tache comme étant "achetée" si on a les sous
       //        if (SCENEDIRECTOR->isHintAvailable()) {
       //           SetGlobal(szTaskPaid,"1");
       //           SCENEDIRECTOR->SetHintLevel(0.0f);
                  // On affiche le Hint
			       //  SCENEDIRECTOR->OpenDialogboxSpirit(szHintString);
                  questionAnswer->graphSzAnswer.setVisible(true);
       //        }
       //        else
       //           questionAnswer->graphSzAnswer.setVisible(true);
                  //         SCENEDIRECTOR->OpenDialogboxSpirit(KStr("UI_TASK_HINTNOTAVAILABLE_MORE"));
       //     }
       //     else {
               // On avait déjà acheté, on affiche à nouveau le hint sans payer le prix
               //SCENEDIRECTOR->OpenDialogboxSpirit(szHintString);
       //        questionAnswer->graphSzAnswer.setVisible(true);
       //     }

            // On ferme le hint system et on demande au Director l'affichage de l'aide
           // SCENEDIRECTOR->SetTaskManager(false);
            return;
         }
      }
   }
}

void ETaskList::hideAllAnswer() {
   
   for(unsigned int i = 0; i < _pVectListObjectiv->size(); ++i) {
      //parcourt les questions
      for(unsigned int j = 0; j < _pVectListObjectiv->at(i).vectQuestionAnswer.size(); ++j) {
         _pVectListObjectiv->at(i).vectQuestionAnswer.at(j).graphSzAnswer.setVisible(false);
         EFancyStringAnim* inst = dynamic_cast<EFancyStringAnim*>(_pVectListObjectiv->at(i).vectQuestionAnswer.at(j).graphSzAnswer.getRenderer());
         if(inst)
            inst->resetAnim();
      }
   }
}

void ETaskList::SetMode(eTaskListMode mode)
{
    _eMode = mode;
    m_needCalcPosition = true;

    fillListObjective();
}
 
eTaskListMode ETaskList::getMode() const
{
    return _eMode;
}
  
void ETaskList::SetUniverse(const char *szUniverseName)
{
    strcpy(_szUniverseName, szUniverseName);
}

const char* ETaskList::getTrad(const char* chaine) {
   
   const char* trad = KStr(chaine);
   if(trad == NULL)
      return chaine;
   else
      return trad;
}

bool ETaskList::addObjectiveToList(const char* objective, bool checkExist) {

   KAssert(_pVectListObjectiv != NULL && objective != NULL);

   // check si l'objective est déjà dans la liste
   if(checkExist) {
      
      std::string stdObj(objective);
      for(unsigned int i = 0; i < _pVectListObjectiv->size(); ++i) {
         // deja present
         if(_pVectListObjectiv->at(i).graphSzObjective.getString() == stdObj)
            return false;
      }
   }

   // pas présent ou s'il ne fallait pas checker, alors on le rajoute direct
   Objective obj;
   obj.graphSzObjective.setString(std::string(objective));
   obj.graphSzObjective.setTrad(std::string(getTrad(objective)));
   obj.graphSzObjective.setColor(1.0f, 0.0f, 0.0f);
   obj.graphSzObjective.setFreezeColor(true);
   _pVectListObjectiv->push_back(obj);

   return true;
}

void ETaskList::renderSequence() {
   
   if(m_pSequenceTextFade)
      m_pSequenceTextFade->Draw();
}