/*
   IdxOverloading2.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

const int SIZE=3;  // 저장소의 크기.

class Arr {
private:
	int arr[SIZE];
	int idx;
public:
	Arr():idx(0){}
	int GetElem(int i);   
	void SetElem(int i, int elem);  
	void AddElem(int elem); 
	void ShowAllData();
	int& operator[](int i); // 배열 요소에 접근.
};
int Arr::GetElem(int i){
	return arr[i];
}
void Arr::SetElem(int i, int elem){
	if(idx<=i){
		cout<<"존재하지 않는 요소!"<<endl;
		return;
	}
	arr[i]=elem;
}
void Arr::AddElem(int elem){
	if(idx>=SIZE) {
		cout<<"용량 초과!"<<endl;
		return ;
	}
	arr[idx++]=elem;
}
void Arr::ShowAllData(){
	for(int i=0; i<idx; i++)
		cout<<"arr["<<i<<"]="<<arr[i]<<endl;
}

int& Arr::operator[](int i){
	return arr[i];
}

int main(void)
{
	Arr arr;
	arr.AddElem(1);
	arr.AddElem(2);
	arr.AddElem(3);
	arr.ShowAllData();

	// 개별 요소 접근 및 변경
	arr[0]=10; 
	arr[1]=20; 
	arr[2]=30; 

	cout<<arr[0]<<endl;
	cout<<arr[1]<<endl;
	cout<<arr[2]<<endl;

	return 0;
}
