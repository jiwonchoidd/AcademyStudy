// -- namespace.C
#include <iostream>
using namespace std;

namespace X {
  class Y {
      static int i;
    public : 
      void f() { cout << "X::Y::f()" << endl; }
  };

  class Z;
  void foo() { cout << "X::foo()" << endl; }
}

int X::Y::i = 9;

class X::Z {
    int u, v, w;
  public :
    Z(int i) { u = v = w = i; }
    void g() { cout << u << endl; }
};

void main()
{
  X::Z  z(10);

  z.g();
}
