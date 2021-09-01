#ifndef ITEM_H
#define ITEM_H

#include <string>
using namespace std;

class Item {
  public :
    Item(string str) : data_(str) {}
    string data_;
};

#endif
