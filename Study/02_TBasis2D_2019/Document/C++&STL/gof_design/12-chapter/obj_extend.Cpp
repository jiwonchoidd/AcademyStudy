#include <iostream>
using namespace std;

#define FRONT_DIRECTION   1
#define SIDE_DIRECTION    2
#define REAR_DIRECTION    4

class Item {
  public : 
    Item(int dir) { direction_ = dir; } 
    int GetDirection() { return direction_; }

  private :
    int direction_;
};

class Airplane {
  public :
    void AddItem(Item* pItem) {
      direction_ += pItem->GetDirection();
    }

    void Attack() {
      if (direction_ & SIDE_DIRECTION) {
        // -- 측방 공격 수행
        cout << "측방공격" << endl;
      }
      if (direction_ & REAR_DIRECTION) {
        // -- 후방 공격 수행
        cout << "후방공격" << endl;
      }

      // -- 전방 공격 수행
      cout << "전방공격" << endl;
    }

  private :
    int direction_;
};

void
main()
{
  Item side(SIDE_DIRECTION);
  Airplane onePlayer;

  onePlayer.AddItem(&side);
  onePlayer.Attack();
} 
