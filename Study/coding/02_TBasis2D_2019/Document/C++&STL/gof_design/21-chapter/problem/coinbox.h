#ifndef COINBOX_CLASS_H
#define COINBOX_CLASS_H

#include "lamp.h"
#include "billbox.h"
using namespace std;

class Mixer;
class BillBox;

class CoinBox {
#define MAX_COIN_CNT    500
#define MIN_COIN_CNT    10

#define COIN_ACCEPT_MODE  1
#define COIN_REJECT_MODE  0

  public :
    CoinBox();

    void SetLamp(Lamp *pLamp);
    void SetMixer(Mixer *pMixer);
    void SetBillBox(BillBox *pBillBox);

    void InsertCoin();
    void RejectCoin();
    void PayOutCoin(int payOutCnt);
    void SetCoinBoxMode(int mode);
  protected :
    bool IsValid();
  private :
    Lamp *pLamp_;
    Mixer *pMixer_;
    BillBox *pBillBox_;
    int coinCnt_;
    int mode_;
};

#endif

