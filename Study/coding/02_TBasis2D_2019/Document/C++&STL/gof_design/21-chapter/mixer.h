#ifndef MIXER_CLASS_H
#define MIXER_CLASS_H

class Mediator;

class Mixer {
#define MIN_MIX_LEVEL       5
#define MAX_MIX_LEVEL       100

#define MIN_CUP_CNT         1
#define MAX_CUP_CNT         100

  public :
    Mixer();
    void SetMediator(Mediator *pMediator);
    void Refill(int addLevel);
    void AddCup(int addCnt);
    int TakeOut(int wantedCupCnt=1);
    void OutOfOrder();
    void Repair();
  protected :
    bool IsValid();
  private :
    Mediator *pMediator_;
    int mixLevel_;
    int cupCnt_;
};

#endif
