#ifndef LAMP_CLASS_H
#define LAMP_CLASS_H

class Lamp {
#define LAMP_ON     1
#define LAMP_OFF    0
  public :
    Lamp();
    void TurnOnOffMixerOut(int onOff);
    void TurnOnOffResourceLack(int onOff);
    void TurnOnOffCoinLack(int onOff);
    void TurnOnOffCoinFull(int onOff);
    void TurnOnOffBillFull(int onOff);

    bool IsResourceLack();
    bool IsCoinLack();
    bool IsCoinFull();
    bool IsBillFull();
  private :
    int mixerOutLamp_;
    int resourceLackLamp_;
    int coinLackLamp_;
    int coinFullLamp_;
    int billFullLamp_;
};

#endif
