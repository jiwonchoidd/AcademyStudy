//프로그램을 작성하다보면 기존에 만들어놓은 클래스와 유사한 경우가 있다.
//하지만 그 클래스를 재사용하고 싶어도 조금이라도 수정하지 않고 사용하기란 어려울 것이다.
//adapter 패턴은 기존 클래스를 재사용할 수 있도록 중간에서 맞춰주는 역할을 한다.
//즉 호환성이 없는 기존 클래스의 인터페이스를 변환해 재사용할 수 있도록 해준다.
//adapter 패턴은 다음 두 가지 형태로 사용된다. 1) 클래스아답터 2) 오브젝트 아답터


#include <iostream>
using namespace std;
//#define CLASS_ADAPTER
#define OBJECT_ADAPTER

class Rectangle 
{
public:
	Rectangle(int x1, int y1, int x2, int y2) 
	{
		x1_ = x1; y1_ = y1; x2_ = x2; y2_ = y2;
	}
	void Draw() {}
private:
	int x1_, y1_, x2_, y2_;
};

class TextView 
{
public:
	Rectangle GetExtent() 
	{
		return Rectangle(x1_, y1_, x1_ + width_, y1_ + height_);
	}
private:
	int x1_, y1_;    
	int width_, height_;
};

class Shape 
{
public:
	virtual Rectangle BoundingBox() = 0;
};

class LineShape : public Shape 
{
public:
	Rectangle BoundingBox() 
	{
		return Rectangle(x1_, y1_, x2_, y2_);
	}
private:
	int x1_, y1_, x2_, y2_;
};
#if defined(CLASS_ADAPTER)
class TextShape : public Shape, private TextView 
{
public:
	Rectangle BoundingBox()
	{
		return GetExtent();
	}
};
void DisplayBoundingBox(Shape *pSelectedShape)
{
	(pSelectedShape->BoundingBox()).Draw();
}
#elif defined(OBJECT_ADAPTER)
class LineShape : public Shape 
{
public:
	Rectangle BoundingBox() 
	{
		return Rectangle(x1_, y1_, x2_, y2_);
	}
private:
	int x1_, y1_, x2_, y2_;
};
class TextShape : public Shape 
{
public:
	TextShape() 
	{
		pText_ = new TextView;
	}
	Rectangle BoundingBox()
	{
		return pText_->GetExtent();
	}
private:
	TextView * pText_;
};
void DisplayBoundingBox(Shape *pSelectedShape)
{
	(pSelectedShape->BoundingBox()).Draw();
}
#else
void DisplayBoundingBox(Shape *pSelectedShape, TextView *pSelectedText)
{
	if (pSelectedText != 0) {
		(pSelectedText->GetExtent()).Draw();
	}
	else if (pSelectedShape != 0) {
		(pSelectedShape->BoundingBox()).Draw();
	}
	else {}
}
#endif



void main()
{
#if defined(CLASS_ADPTER)||defined(OBJECT_ADAPTER)
	TextShape text;
	DisplayBoundingBox(&text);
#else
	TextView text;
	DisplayBoundingBox(0, &text);
#endif	
}