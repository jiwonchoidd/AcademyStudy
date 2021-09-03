#include <iostream>
using namespace std;

class Singleton {
  public : 
    static Singleton* CreateInstance() {
      if (pInstance_ == 0)
        pInstance_ = new Singleton;
      return pInstance_;    
    }

    int GetData() { return data_; }
    void SetData(int val) { data_ = val; }

  protected :
    Singleton() { data_ = 10; }

  private : 
    static Singleton* pInstance_;
    int data_;
};

Singleton* Singleton::pInstance_ = 0;

int
main() {
  Singleton *p = Singleton::CreateInstance();
  Singleton a(*p); // -- 복제 생성자 호출

  a.SetData(20);

  cout << "p->data = " << p->GetData() << endl; // -- p->data = 10
  cout << "a.data = " << a.GetData() << endl;   // -- a.data = 20

  return 0;
}
