#include <string>
#include <list>
#include <algorithm>
#include "observer.h"
#include "subject.h"
using namespace std;

Subject::~Subject() {}

void
Subject::Attach(Observer *pObj) {
  observers_.push_front(pObj);
}

void
Subject::Detach(Observer *pObj) {
  list<Observer*>::iterator iter;
  for (iter = observers_.begin(); iter != observers_.end(); iter++)
    if (*iter == pObj) observers_.erase(iter);
}

void
Subject::Notify() {
  list<Observer*>::iterator iter;
  for (iter = observers_.begin(); iter != observers_.end(); iter++) 
    (*iter)->Update();
}

// -------------------------------------------------

void
ScoreData::AddScore(ScoreCard *pScore) {
  scoreList_.push_front(pScore);
  Notify();
}

void
ScoreData::RemoveScore(ScoreCard *pScore) {
  list<ScoreCard*>::iterator iter;
  iter = find(scoreList_.begin(), scoreList_.end(), pScore);
  if (iter != scoreList_.end()) {
    ScoreCard *pTmpScore = *iter;
    delete pTmpScore;
    scoreList_.erase(iter);
    Notify();
  }
}

list<ScoreCard*>
ScoreData::GetScoreList() { return scoreList_; }

void
ScoreData::SetScore(string name, int subjectType, int score) {
  list<ScoreCard*>::iterator iter;
  for (iter=scoreList_.begin(); iter != scoreList_.end(); iter++) {
    ScoreCard *pTmpScore = *iter;
    if (pTmpScore->name_ == name) {
      switch (subjectType) {
        case MOTHER_LANG_SUBJECT :
            pTmpScore->motherLangScore_ = score;
            break;
        case ENGLISH_SUBJECT :
            pTmpScore->englishScore_ = score;
            break;
        case MATH_SUBJECT :
            pTmpScore->mathScore_ = score;
            break;
      }
      Notify();
      break;
    }
  }
}
