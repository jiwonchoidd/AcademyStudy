#ifndef MEDIATOR_CLASS_H
#define MEDIATOR_CLASS_H

class Lamp;
class Mixer;
class CoinBox;
class BillBox;

class Mediator {
#define MIXER_OUT_OF_ORDER    0
#define MIXER_OK              1

#define RESOURCE_LACK         0
#define RESOURCE_OK           1

#define COIN_LACK             0
#define COIN_NOT_LACK         1
#define COIN_FULL             2
#define COIN_NOT_FULL         3

#define BILL_FULL             0
#define BILL_NOT_FULL         1

  public :
    Mediator(Lamp *pLamp, Mixer *pMixer, CoinBox *pCoinBox, BillBox *pBillBox);

    bool IsResourceLack();
    bool IsCoinLack();
    bool IsCoinFull();
    bool IsBillFull();

    void InformMixerState(int state);
    void InformResourceState(int state);
    void InformCoinState(int state);
    void InformBillState(int state);

    int TakeOutCup(int takeOutCnt=1);
    void PayOutCoin(int payOutCnt);
  private :
    Lamp *pLamp_;
    Mixer *pMixer_;
    CoinBox *pCoinBox_;
    BillBox *pBillBox_;
};

#endif
