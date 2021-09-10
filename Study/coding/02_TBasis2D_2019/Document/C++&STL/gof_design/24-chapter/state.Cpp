#include <iostream>
#include <string>
using namespace std;

class GameLevel {
  public :
    static GameLevel* CreateInstance() { return 0; }
    virtual void SimpleAttack() = 0;
    virtual void TurnAttack() = 0;
    virtual void FlyingAttack() = 0;
  protected :
    GameLevel() {}
};

class GameLevel0 : public GameLevel {
  public :
    static GameLevel* CreateInstance() {
      if (pInstance_ == 0) pInstance_ = new GameLevel0;
      return pInstance_;
    }

    void SimpleAttack() { cout << "Simple Attack" << endl; }
    void TurnAttack() { cout << "Not Allowed" << endl; }
    void FlyingAttack() { cout << "Not Allowed" << endl; }
  protected :
    GameLevel0() { }
  private : 
    static GameLevel0 *pInstance_;
};
GameLevel0* GameLevel0::pInstance_ = 0;

class GameLevel1 : public GameLevel {
  public :
    static GameLevel* CreateInstance() {
      if (pInstance_ == 0) pInstance_ = new GameLevel1;
      return pInstance_;
    }

    void SimpleAttack() { cout << "Simple Attack" << endl; }
    void TurnAttack() { cout << "Turn Attack" << endl; }
    void FlyingAttack() { cout << "Not Allowed" << endl; }
  protected :
    GameLevel1() { }
  private : 
    static GameLevel1 *pInstance_;
};
GameLevel1* GameLevel1::pInstance_ = 0;

class GameLevel2 : public GameLevel {
  public :
    static GameLevel* CreateInstance() {
      if (pInstance_ == 0) pInstance_ = new GameLevel2;
      return pInstance_;
    }

    void SimpleAttack() { cout << "Simple Attack" << endl; }
    void TurnAttack() { cout << "Turn Attack" << endl; }
    void FlyingAttack() { cout << "Flying Attack" << endl; }
  protected :
    GameLevel2() { }
  private : 
    static GameLevel2 *pInstance_;
};
GameLevel2* GameLevel2::pInstance_ = 0;

class GamePlayer {
  public :
    GamePlayer() { pGameLevel_ = GameLevel0::CreateInstance(); }

    void UpgradeLevel(GameLevel* pLevel) {
      pGameLevel_ = pLevel;
    }

    void SimpleAttack() { pGameLevel_->SimpleAttack(); }
    void TurnAttack() { pGameLevel_->TurnAttack(); }
    void FlyingAttack() { pGameLevel_->FlyingAttack(); }
  private :
    GameLevel *pGameLevel_;
};

void
main()
{
  GamePlayer user1;

  user1.SimpleAttack();
  user1.TurnAttack();
  user1.FlyingAttack();
  cout << "--------------------------" << endl;

  GameLevel *pGameLevel1 = GameLevel1::CreateInstance();
  user1.UpgradeLevel(pGameLevel1);

  user1.SimpleAttack();
  user1.TurnAttack();
  user1.FlyingAttack();
  cout << "--------------------------" << endl;

  GameLevel *pGameLevel2 = GameLevel2::CreateInstance();
  user1.UpgradeLevel(pGameLevel2);

  user1.SimpleAttack();
  user1.TurnAttack();
  user1.FlyingAttack();
}
