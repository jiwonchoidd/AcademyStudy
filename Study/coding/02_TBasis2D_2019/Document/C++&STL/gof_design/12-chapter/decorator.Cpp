#include <iostream>
using namespace std;

class Airplane {
  public :
    virtual void Attack() = 0;
};

class FrontAttackAirplane : public Airplane {
  public :
    void Attack() {
      // -- 전방 공격 수행
      cout << "전방공격" << endl;
    }
};

class Decorator : public Airplane {
  public :
    Decorator(Airplane* pObj) { pComponent_ = pObj; }
    virtual ~Decorator()=0;

    virtual void Attack() {
      if (pComponent_ != 0) 
        pComponent_->Attack();
    }

  private :
    Airplane* pComponent_;
};

Decorator::~Decorator() {}

class SideAttackAirplane : public Decorator {
  public :
    SideAttackAirplane(Airplane* pObj) : Decorator(pObj) {}

    void Attack() {
      Decorator::Attack();
      // -- 측방 공격 수행
      cout << "측방공격" << endl;
    }
};

class RearAttackAirplane : public Decorator {
  public : 
    RearAttackAirplane(Airplane* pObj) : Decorator(pObj) {}

    void Attack() {
      Decorator::Attack();
      // -- 후방 공격 수행
      cout << "후방공격" << endl;
    }
};

void
main()
{
  Airplane* pFrontAttackAirplane = new FrontAttackAirplane;
  Airplane* pSideAttackAirplane = new SideAttackAirplane(pFrontAttackAirplane);
  Airplane* pRearAttackAirplane = new RearAttackAirplane(pSideAttackAirplane);

  pRearAttackAirplane->Attack();
  delete pRearAttackAirplane;

  pSideAttackAirplane->Attack();
} 
