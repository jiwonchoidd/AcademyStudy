#include <iostream>
using namespace std;

#define N_UNIT    100

#define ATTACK_UNIT   1
#define PROTECT_UNIT  2

class GameUnit {
  public :
    static void InitUnitArray() {
       for (int i=0; i < N_UNIT; i++)
         pUnitArray_[i] = 0;
    }

    static GameUnit* CreateInstance(int unitType);

    static void DestroyUnit(GameUnit* pUnit) {
      for (int i=0; i < N_UNIT; i++) {
        if (pUnitArray_[i] == pUnit) {
          delete pUnitArray_[i];
          pUnitArray_[i] = 0;
          return;
        }
      }
    }

    virtual void Display(int x, int y) {}
    virtual void DoAction() {}
  protected :
    GameUnit() {}
    GameUnit(const GameUnit& rhs) {}
    static GameUnit* pUnitArray_[N_UNIT];
};

class AttackUnit : public GameUnit {
  public : 
    void Display(int x, int y) {}
    void DoAction() {}
};

class ProtectUnit : public GameUnit {
  public : 
    void Display(int x, int y) {}
    void DoAction() {}
};

GameUnit* 
GameUnit::CreateInstance(int unitType) { 
  for (int i=0; i < N_UNIT; i++) {
    if (pUnitArray_[i] == 0) {
      if (unitType == ATTACK_UNIT)
        pUnitArray_[i] = new AttackUnit;
      else if (unitType == PROTECT_UNIT)
        pUnitArray_[i] = new ProtectUnit;
      else
        pUnitArray_[i] = new GameUnit;
      return pUnitArray_[i];
    }
  }

  return 0; 
}

// -- 클래스 변수 정의
GameUnit* GameUnit::pUnitArray_[N_UNIT];

void
main()
{
  GameUnit::InitUnitArray();

  GameUnit *pUnit1 = GameUnit::CreateInstance(ATTACK_UNIT);
  if (pUnit1 == 0)
    cout << "No More Create Unit" << endl;

  GameUnit *pUnit2 = GameUnit::CreateInstance(PROTECT_UNIT);
  if (pUnit2 == 0)
    cout << "No More Create Unit" << endl;

  GameUnit::DestroyUnit(pUnit1);
  GameUnit::DestroyUnit(pUnit2);
}
