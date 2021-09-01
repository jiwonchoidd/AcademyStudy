#include <iostream>
#include <list>
#include <iterator>
#include <map>
using namespace std;

class Graphic {
  public :
    virtual void Draw() = 0;
};

class ComposedGraphic : public Graphic {
  public :
    void Draw() { }

    void Add(Graphic *pObj) {
      cout << "ComposedGraphic" << endl;
    }
};

class SpecificComposedGraphic : public ComposedGraphic {
  public :
    void Draw() { }

    void Add(Graphic *pObj) {
      cout << "Specific-ComposedGraphic" << endl;
    }
};

void
main()
{
  Graphic *pObj = new SpecificComposedGraphic;
  ((ComposedGraphic*)(pObj))->Add(pObj);
  dynamic_cast<ComposedGraphic*>(pObj)->Add(pObj);
}
