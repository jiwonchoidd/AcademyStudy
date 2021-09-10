#include <iostream>
#include "subject.h"
#include "observer.h"
using namespace std;

BarGraph::BarGraph(ScoreData *pScoreData) : pScoreData_(pScoreData) {
  pScoreData_->Attach(this);
}

void
BarGraph::Update() {
  list<ScoreCard*> scoreList;
  scoreList = pScoreData_->GetScoreList();

  int nthStudent = 0;
  list<ScoreCard*>::iterator iter;
  for (iter=scoreList.begin(); iter != scoreList.end(); iter++) {
    ScoreCard *pTmpScore = *iter;
    name_[nthStudent] = pTmpScore->name_;
    motherLangScore_[nthStudent] = pTmpScore->motherLangScore_;
    englishScore_[nthStudent] = pTmpScore->englishScore_;
    mathScore_[nthStudent] = pTmpScore->mathScore_;
    nthStudent++;
  }
}

void
BarGraph::PrintOut() {
  for (int i = 0; i < MAX_STUDENT_NUM; i++) {
    if (name_[i].empty()) break;
    cout << name_[i] << "=>" << motherLangScore_[i] 
      << ":" << englishScore_[i] << ":" << mathScore_[i] << endl;
 }
}

void
BarGraph::ChangeScore(string name, int subjectType, int score) {
  pScoreData_->SetScore(name, subjectType, score);
}

// ---------------------------------------------------

PieChart::PieChart(string student, ScoreData *pScoreData) 
  : pScoreData_(pScoreData) { 
  name_ = student; 
  pScoreData_->Attach(this);
}

void
PieChart::Update() {
  list<ScoreCard*> scoreList;
  scoreList = pScoreData_->GetScoreList();

  list<ScoreCard*>::iterator iter;
  for (iter=scoreList.begin(); iter != scoreList.end(); iter++) {
    ScoreCard *pTmpScore = *iter;
    if (name_ == pTmpScore->name_) {
      motherLangScore_ = pTmpScore->motherLangScore_;
      englishScore_ = pTmpScore->englishScore_;
      mathScore_ = pTmpScore->mathScore_;
      break;
    }
  }
}

void
PieChart::PrintOut() {
  cout << name_ << "=>" << motherLangScore_ 
      << ":" << englishScore_ << ":" << mathScore_ << endl;
}

void
PieChart::ChangeScore(string name, int subjectType, int score) {
  pScoreData_->SetScore(name, subjectType, score);
}

// ---------------------------------------------------

LineGraph::LineGraph(ScoreData *pScoreData) : pScoreData_(pScoreData) {
  pScoreData_->Attach(this);
}

void
LineGraph::Update() {
  list<ScoreCard*> scoreList;
  scoreList = pScoreData_->GetScoreList();

  int nthStudent = 0;
  list<ScoreCard*>::iterator iter;
  for (iter=scoreList.begin(); iter != scoreList.end(); iter++) {
    ScoreCard *pTmpScore = *iter;
    name_[nthStudent] = pTmpScore->name_;
    motherLangScore_[nthStudent] = pTmpScore->motherLangScore_;
    englishScore_[nthStudent] = pTmpScore->englishScore_;
    mathScore_[nthStudent] = pTmpScore->mathScore_;
    nthStudent++;
  }
}

void
LineGraph::PrintOut() {
  for (int i = 0; i < MAX_STUDENT_NUM; i++) {
    if (name_[i].empty()) break;
    cout << name_[i] << "=>" << motherLangScore_[i] 
      << ":" << englishScore_[i] << ":" << mathScore_[i] << endl;
  }
}

void
LineGraph::ChangeScore(string name, int subjectType, int score) {
  pScoreData_->SetScore(name, subjectType, score);
}
