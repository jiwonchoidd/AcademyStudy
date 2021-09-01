// -- imgptr.C
#include <iostream>
#include <string>
using namespace std;

class Point {
  public :
    Point(int x, int y) { x_ = x; y_ = y; }
    int x_, y_;
};

class Image {
  public :
    Image(const char *pFn) { fn_ = pFn; }
    void Draw(Point p) { cout << fn_ << ": " << p.x_ << ", " << p.y_ << endl; }
  private :
    string fn_;
};

Image* LoadAnImageFile(const char* pFn) { return new Image(pFn); }

class ImagePtr {
  public :
    ImagePtr(const char* pFn) { pImage_ = 0; fn_ = pFn; }
    virtual ~ImagePtr() {}

    virtual Image* operator->() { return LoadImage(); }
    virtual Image& operator*() { return *LoadImage(); }
  private :
    Image* LoadImage() { 
      if (pImage_ == 0) 
        pImage_ = LoadAnImageFile(fn_.data());
      return pImage_;
    }

    Image* pImage_;
    string fn_;
};

void main()
{
  ImagePtr image = ImagePtr("anImageFileName");
  image->Draw(Point(50, 100));
    // (image.operator->())->Draw(Point(50, 100))
}
