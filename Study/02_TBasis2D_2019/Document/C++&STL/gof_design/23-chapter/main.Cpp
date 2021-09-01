#include <iostream>
#include "scorecard.h"
#include "subject.h"
#include "observer.h"
using namespace std;

void
main() 
{
  ScoreData termScore;
  BarGraph bar(&termScore);
  LineGraph line(&termScore);

  ScoreCard stu1, stu2, stu3;
  stu1.name_ = "Ã¶¼ö";
  stu1.motherLangScore_ = 70;
  stu1.englishScore_ = 60;
  stu1.mathScore_ = 90;

  stu2.name_ = "¿µÈñ";
  stu2.motherLangScore_ = 70;
  stu2.englishScore_ = 80;
  stu2.mathScore_ = 50;

  stu3.name_ = "¼øÀÌ";
  stu3.motherLangScore_ = 95;
  stu3.englishScore_ = 90;
  stu3.mathScore_ = 85;

  termScore.AddScore(&stu1);
  termScore.AddScore(&stu2);
  termScore.AddScore(&stu3);

  cout << "-----------------------" << endl;
  bar.PrintOut();
  cout << "-----------------------" << endl;
  line.PrintOut();

  bar.ChangeScore("Ã¶¼ö", ENGLISH_SUBJECT, 85);

  cout << "-----------------------" << endl;
  bar.PrintOut();
  cout << "-----------------------" << endl;
  line.PrintOut();
}
