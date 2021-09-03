#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include "scorecard.h"
using namespace std;

#define MAX_STUDENT_NUM     10

class Subject;
class ScoreData;

class Observer {
  public :
    virtual void Update()=0;
};

class BarGraph : public Observer {
  public :
    BarGraph(ScoreData *pScoreData);
    void Update();
    void PrintOut();
    void ChangeScore(string name, int subjectType, int score);
  private :
    ScoreData *pScoreData_;
    string name_[MAX_STUDENT_NUM];
    int motherLangScore_[MAX_STUDENT_NUM];
    int englishScore_[MAX_STUDENT_NUM];
    int mathScore_[MAX_STUDENT_NUM];
};

class PieChart : public Observer {
  public :
    PieChart(string student, ScoreData *pScoreData);
    void Update();
    void PrintOut();
    void ChangeScore(string name, int subjectType, int score);
  private :
    ScoreData *pScoreData_;
    string name_;
    int motherLangScore_;
    int englishScore_;
    int mathScore_;
};

class LineGraph : public Observer {
  public :
    LineGraph(ScoreData *pScoreData);
    void Update();
    void PrintOut();
    void ChangeScore(string name, int scoreType, int score);
  private :
    ScoreData *pScoreData_;
    string name_[MAX_STUDENT_NUM];
    int motherLangScore_[MAX_STUDENT_NUM];
    int englishScore_[MAX_STUDENT_NUM];
    int mathScore_[MAX_STUDENT_NUM];
};

#endif
