/*
   PointArr2.cpp
*/

#include<iostream>
using std::cout;
using std::endl;

class Point
{
	int x;  
	int y;  
public:
	Point(){
		cout<<"Point() call!"<<endl;
		x=y=0;
	}
	Point(int _x, int _y){
		x=_x;
		y=_y;
	}
	int GetX(){	return x; }
	int GetY(){	return y; }

	void SetX(int _x){ x=_x; }
	void SetY(int _y){ y=_y; }
};

int main()
{
	Point* arr[5];

	for(int i=0; i<5; i++)
	{
		arr[i]=new Point(i*2, i*3);  // new俊 狼茄 按眉 悼利 积己.
	}

	for(int j=0; j<5; j++)
	{
		cout<<"x: "<<arr[j]->GetX()<<' ';
		cout<<"y: "<<arr[j]->GetY()<<endl;
	}

	for(int k=0; k<5; k++)
	{
		delete arr[k];  // 赛俊 历厘等 按眉 家戈.
	}

	return 0;
}
