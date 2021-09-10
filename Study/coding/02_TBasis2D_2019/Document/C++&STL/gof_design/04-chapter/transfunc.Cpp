#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int UNDEF_SENTENCE=0;
const int NORMAL_SENTENCE=1;
const int INTERROGATIVE_SENTENCE=2;
const int IMPERATIVE_SENTENCE=3;

const int TO_ENGLISH=1;
const int TO_JAPANESE=2;
const int TO_FRENCH=3;

class Sentence {
  public :
    Sentence() { data_ = ""; type_ = UNDEF_SENTENCE;}

    int GetType() { return type_; }
    string GetString() { return data_; }

    void SetSentenceData(string s) { 
      SetSenteceType(s);
      data_ = s; 
    }

  protected :
    void SetSenteceType(string s) {
      // -- 문장 유형을 판단하여 type_에 설정, default는 평서문
      type_ = NORMAL_SENTENCE;
    }

  private :
    string data_;
    int type_;
};

class Manual {
  public :
    string GetContents() { return contents_; }
    void AddContents(string s) { contents_ += s; }

  private : 
    string contents_;
};

Sentence GetSentence(ifstream&);
string TransNormalSentence(string, int);
string TransInterrogativeSentence(string, int);
string TransImperativeSentence(string, int);

void
DoTranslate(char* pInFile, Manual& out, int wantedLang)
{
  ifstream ifs(pInFile);
  if (!ifs) {
    cout << "Can't Open File : " << pInFile << endl;
    return;
  }

  string result;
  Sentence next;
  while (!(next=GetSentence(ifs)).GetString().empty()) {
    switch (next.GetType()) {
      case NORMAL_SENTENCE : // -- 평서문
        result = TransNormalSentence(next.GetString(), wantedLang);
        break;
      case INTERROGATIVE_SENTENCE : // -- 의문문
        result = TransInterrogativeSentence(next.GetString(), wantedLang);
        break;
      case IMPERATIVE_SENTENCE : // -- 명령문
        result = TransImperativeSentence(next.GetString(), wantedLang);
        break;
      default :
        cout << "Untranslatable sentence type" << endl;
        return;
    }

    out.AddContents(result);
  }
}

Sentence 
GetSentence(ifstream& ifs) 
{
  int c;
  string s;
  Sentence sentence;
  while ((c=ifs.get()) != EOF) {
    s += c;
    if (c == '?' || c == '.') 
      break;
  }
  sentence.SetSentenceData(s);
  return sentence;
}

string
TransNormalSentence(string s, int wantedLang)
{
  string output;
  switch (wantedLang) {
    case TO_ENGLISH : // -- 영어로 문장 번역
                      break;
    case TO_JAPANESE : // -- 일어로 문장 번역
                      break;
    case TO_FRENCH : // -- 프랑스어로 문장 번역
                      break;
  }

  return output;
}

string
TransInterrogativeSentence(string s, int wantedLang)
{
  string output;
  switch (wantedLang) {
    case TO_ENGLISH : // -- 영어로 문장 번역
                      break;
    case TO_JAPANESE : // -- 일어로 문장 번역
                      break;
    case TO_FRENCH : // -- 프랑스어로 문장 번역
                      break;
  }

  return output;
}

string
TransImperativeSentence(string s, int wantedLang)
{
  string output;
  switch (wantedLang) {
    case TO_ENGLISH : // -- 영어로 문장 번역
                      break;
    case TO_JAPANESE : // -- 일어로 문장 번역
                      break;
    case TO_FRENCH : // -- 프랑스어로 문장 번역
                      break;
  }

  return output;
}

void main()
{
  Manual out;
  DoTranslate("input.txt", out, TO_ENGLISH);
  cout << out.GetContents() << endl;
}
