//observer는 '관찰하는 사람', '관찰자'란 뜻이다. 관찰자는 무엇인가 감시하고 있다가 
//관찰 대상에 변화가 생기면 이를 즉시 감지하고 알려주는 역할을 한다.
//이처럼 어떤 클래스에 변화가 일어났을 때, 이를 감지하여 다른 클래스에 통보해주는 것이 observer 패턴이다. 
#include <iostream>
#include "scorecard.h"
#include "subject.h"
#include "observer.h"
using namespace std;

void main()
{
	ScoreData termScore;
	BarGraph bar(&termScore);
	LineGraph line(&termScore);

	ScoreCard stu1, stu2, stu3;
	stu1.name_ = "철수";
	stu1.motherLangScore_ = 70;
	stu1.englishScore_ = 60;
	stu1.mathScore_ = 90;

	stu2.name_ = "영희";
	stu2.motherLangScore_ = 70;
	stu2.englishScore_ = 80;
	stu2.mathScore_ = 50;

	stu3.name_ = "순이";
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

	bar.ChangeScore("철수", ENGLISH_SUBJECT, 85);

	cout << "-----------------------" << endl;
	bar.PrintOut();
	cout << "-----------------------" << endl;
	line.PrintOut();
}
