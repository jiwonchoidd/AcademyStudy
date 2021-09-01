/*
   IdxOverloading1.cpp   
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
	int GetElem(int i);   // 요소를 참조하는 함수.
	void SetElem(int i, int elem);  // 저장된 요소를 변경하는 함수.
	void AddElem(int elem);  // 배열에 데이터 저장하는 함수. 
	void ShowAllData();
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

int main(void)
{
	Arr arr;
	arr.AddElem(1);
	arr.AddElem(2);
	arr.AddElem(3);
	arr.ShowAllData();

	// 개별 요소 접근 및 변경
	arr.SetElem(0, 10);
	arr.SetElem(1, 20);
	arr.SetElem(2, 30);

	cout<<arr.GetElem(0)<<endl;
	cout<<arr.GetElem(1)<<endl;
	cout<<arr.GetElem(2)<<endl;

	return 0;
}
