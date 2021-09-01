#ifndef BILLBOX_CLASS_H
#define BILLBOX_CLASS_H

#include "lamp.h"
using namespace std;

class Mixer;
class CoinBox;

class BillBox {
#define MAX_BILL_CNT        100

#define BILL_ACCEPT_MODE    1
#define BILL_REJECT_MODE    0

  public :
    BillBox();

    void SetLamp(Lamp *pLamp);
    void SetMixer(Mixer *pMixer);
    void SetCoinBox(CoinBox *pCoinBox);

    void InsertBill(int wantedCupCnt=1);
    void RejectBill();
    void TakeOutBill(int takeOutCnt);
    void SetBillBoxMode(int mode);
  protected :
    bool IsValid();
  private :
    Lamp *pLamp_;
    Mixer *pMixer_;
    CoinBox *pCoinBox_;
    int billCnt_;
    int mode_;
};

#endif
