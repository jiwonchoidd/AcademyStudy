#include <iostream>
#include <list>
#include <iterator>
#include <map>
using namespace std;

class Graphic {
  public :
    virtual void Draw() = 0;
    virtual void Add(Graphic* pObj) {}
    virtual void Remove(Graphic* pObj) {}
    virtual Graphic* GetChild(int nth) { return 0; }
};

class Line : public Graphic {
  public :
    void Draw() { 
      // -- 라인 그리기 
    }
};

class Triangle : public Graphic {
  public :
    void Draw() { 
      // -- 삼각형 그리기 
    }
};

class Rectangle : public Graphic {
  public :
    void Draw() {
      // -- 사각형 그리기
    }
};

class ComposedGraphic : public Graphic {
  public :
    void Draw() {
      list<Graphic*>::iterator iter1;
      for (iter1 = components_.begin(); iter1 != components_.end(); iter1++) 
        (*iter1)->Draw();
    }

    void Add(Graphic *pObj) {
      components_.push_front(pObj);
    }

    void remove(Graphic *pObj) {
      list<Graphic*>::iterator iter1;
      for (iter1 = components_.begin(); iter1 != components_.end(); iter1++) {
        if (*iter1 == pObj) 
          components_.erase(iter1);
      }
    }

    Graphic* GetChild(int nth) {
      int i;
      list<Graphic*>::iterator iter1;
      for (i=0, iter1 = components_.begin(); 
                iter1 != components_.end(); iter1++, i++) {
        if (i == nth) return *iter1;
      }
      return 0;
    }

  private :
    list<Graphic *> components_;
};

class Palette {
  public :
    Palette() {
      Graphic* pGraphic = new Triangle;
      items_[1] = pGraphic;

      pGraphic = new Rectangle;
      items_[2] = pGraphic;

      // -- 필요한 만큼 기본 도형 등록
    }
    
    void RegisterNewGraphic(Graphic* pGraphic) {
      items_[items_.size()+1] = pGraphic;
    }

  private :
    map<int, Graphic*> items_;
};

void
main()
{
  Triangle aTriangle;
  Rectangle aRectangle;
  ComposedGraphic aComposedGraphic2;

  aComposedGraphic2.Add(&aTriangle);
  aComposedGraphic2.Add(&aRectangle);

  Line aLine;
  Rectangle aRectangle2;
  ComposedGraphic aComposedGraphic;

  aComposedGraphic.Add(&aComposedGraphic2);
  aComposedGraphic.Add(&aLine);
  aComposedGraphic.Add(&aRectangle2);
}
