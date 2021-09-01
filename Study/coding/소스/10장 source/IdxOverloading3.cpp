/*
   IdxOverloading3.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;
using std::ostream;

/***************** Point Class **************************/
class Point {
private:
	int x, y;
public:
	Point(int _x=0, int _y=0):x(_x), y(_y){}
	friend ostream& operator<<(ostream& os, const Point& p);
};

ostream& operator<<(ostream& os, const Point& p)
{
	os<<"["<<p.x<<", "<<p.y<<"]";	
	return os;
}

/***************** PointArr Class **************************/
const int SIZE=5;  // 저장소의 크기.

class PointArr {
private:
	Point arr[SIZE];
	int idx;
public:
	PointArr():idx(0){}
	void AddElem(const Point& elem);
	void ShowAllData();
	Point& operator[](int i); // 배열 요소에 접근.
};

void PointArr::AddElem(const Point& elem){
	if(idx>=SIZE) {
		cout<<"용량 초과!"<<endl;
		return ;
	}
	arr[idx++]=elem;
}
void PointArr::ShowAllData(){
	for(int i=0; i<idx; i++)
		cout<<"arr["<<i<<"]="<<arr[i]<<endl;
}

Point& PointArr::operator[](int i){
	return arr[i];
}

int main(void)
{
	PointArr arr; 

	arr.AddElem(Point(1, 1));
	arr.AddElem(Point(2, 2));
	arr.AddElem(Point(3, 3));
	arr.ShowAllData();

	// 개별 요소 접근 및 변경
	arr[0]=Point(10, 10); 
	arr[1]=Point(20, 20); 
	arr[2]=Point(30, 30); 

	cout<<arr[0]<<endl;
	cout<<arr[1]<<endl;
	cout<<arr[2]<<endl;

	return 0;
}
