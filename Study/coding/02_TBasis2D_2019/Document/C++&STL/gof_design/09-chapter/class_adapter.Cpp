#include <iostream>
using namespace std;

class Rectangle {  
  public :    
    Rectangle(int x1, int y1, int x2, int y2) {      
      x1_ = x1; y1_ = y1; x2_ = x2; y2_ = y2;    
    }    
    void Draw() {}  
  private :    
    int x1_, y1_, x2_, y2_;
};

class TextView {  
  public :     
    Rectangle GetExtent() {      
      return Rectangle(x1_, y1_, x1_ + width_, y1_ + height_);    
    }  
  private :     
    int x1_, y1_;    int width_, height_;
};

class Shape {  
  public :     
    virtual Rectangle BoundingBox() = 0;
};

class LineShape : public Shape {  
  public :    
    Rectangle BoundingBox() {      
      return Rectangle(x1_, y1_, x2_, y2_);    
    }  
  private :     
    int x1_, y1_, x2_, y2_;
};

class TextShape : public Shape, private TextView {
  public : 
    Rectangle BoundingBox() {
      return GetExtent();
    }
};

void DisplayBoundingBox(Shape *pSelectedShape) 
{  
  (pSelectedShape->BoundingBox()).Draw();
}

void 
main()
{
  TextShape text;
  DisplayBoundingBox(&text);
}