#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int UNDEF_SENTENCE=0;
const int NORMAL_SENTENCE=1;
const int INTERROGATIVE_SENTENCE=2;
const int IMPERATIVE_SENTENCE=3;

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

class Translator {
  public :
    Manual GetResult() { return result_; }

    virtual void TransNormalSentence(string s)=0;
    virtual void TransInterrogativeSentence(string s)=0;
    virtual void TransImperativeSentence(string s)=0;

  protected :
    Manual result_;
};

class EnglishTranslator : public Translator {
  public : 
    void TransNormalSentence(string s)
    {
      string output;
      // -- s를 영어로 번역
      result_.AddContents(output);
    }

    void TransInterrogativeSentence(string s)
    {
      string output;
      // -- s를 영어로 번역
      result_.AddContents(output);
    }

    void TransImperativeSentence(string s)
    {
      string output;
      // -- s를 영어로 번역
      result_.AddContents(output);
    }
};

class JapaneseTranslator : public Translator {
  public : 
    void TransNormalSentence(string s)
    {
      string output;
      // -- s를 일어로 번역
      result_.AddContents(output);
    }

    void TransInterrogativeSentence(string s)
    {
      string output;
      // -- s를 일어로 번역
      result_.AddContents(output);
    }

    void TransImperativeSentence(string s)
    {
      string output;
      // -- s를 일어로 번역
      result_.AddContents(output);
    }
};

class FrenchTranslator : public Translator {
  public : 
    void TransNormalSentence(string s)
    {
      string output;
      // -- s를 프랑스어로 번역
      result_.AddContents(output);
    }

    void TransInterrogativeSentence(string s)
    {
      string output;
      // -- s를 프랑스어로 번역
      result_.AddContents(output);
    }

    void TransImperativeSentence(string s)
    {
      string output;
      // -- s를 프랑스어로 번역
      result_.AddContents(output);
    }
};

class Director {
  public : 
    void DoTranslate(char* pInFile, Translator& t)
    {
      ifstream ifs(pInFile);
      if (!ifs) {
        cout << "Can't Open File : " << pInFile << endl;
        return;
      }

      Sentence next;
      while (!(next=GetSentence(ifs)).GetString().empty()) {
        switch (next.GetType()) {
          case NORMAL_SENTENCE : // -- 평서문
            t.TransNormalSentence(next.GetString());
            break;
          case INTERROGATIVE_SENTENCE : // -- 의문문
            t.TransInterrogativeSentence(next.GetString());
            break;
          case IMPERATIVE_SENTENCE : // -- 명령문
            t.TransImperativeSentence(next.GetString());
            break;
          default :
            cout << "Untranslatable sentence type" << endl;
            return;
        }
      }
    }

  protected:
    Sentence GetSentence(ifstream& ifs) 
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
};

void main()
{
  Director d;
  EnglishTranslator t;
  d.DoTranslate("input.txt", t);

  Manual out = t.GetResult();
  cout << out.GetContents() << endl;
}
