#include <iostream>
#include <string>
#include <map>
using namespace std;

class Singleton {
  public:
    static void Register(const char* name, Singleton*);
    static Singleton* CreateInstance(string name);

  private:
    static Singleton* pInstance_;
    static map<string, Singleton*> registry_;
};

Singleton* Singleton::pInstance_ = 0;
map<string, Singleton*> Singleton::registry_;

void
Singleton::Register(const char* name, Singleton* pObj)
{
  registry_[name] = pObj;
}

Singleton* 
Singleton::CreateInstance(string name) 
{
  if (pInstance_ == 0) {
    pInstance_ = registry_[name];
  }
  return pInstance_;
}

class MySingleton : public Singleton {
  public : 
    MySingleton() {
      Singleton::Register("MySingleton", this);
    }
};

static MySingleton myObj;

void
main()
{
  Singleton* pObj = Singleton::CreateInstance("MySingleton");
}
