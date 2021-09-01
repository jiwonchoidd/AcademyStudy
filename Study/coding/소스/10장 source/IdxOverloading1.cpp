/*
   IdxOverloading1.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

const int SIZE=3;  // ������� ũ��.

class Arr {
private:
	int arr[SIZE];
	int idx;
public:
	Arr():idx(0){}
	int GetElem(int i);   // ��Ҹ� �����ϴ� �Լ�.
	void SetElem(int i, int elem);  // ����� ��Ҹ� �����ϴ� �Լ�.
	void AddElem(int elem);  // �迭�� ������ �����ϴ� �Լ�. 
	void ShowAllData();
};
int Arr::GetElem(int i){
	return arr[i];
}
void Arr::SetElem(int i, int elem){
	if(idx<=i){
		cout<<"�������� �ʴ� ���!"<<endl;
		return;
	}
	arr[i]=elem;
}
void Arr::AddElem(int elem){
	if(idx>=SIZE) {
		cout<<"�뷮 �ʰ�!"<<endl;
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

	// ���� ��� ���� �� ����
	arr.SetElem(0, 10);
	arr.SetElem(1, 20);
	arr.SetElem(2, 30);

	cout<<arr.GetElem(0)<<endl;
	cout<<arr.GetElem(1)<<endl;
	cout<<arr.GetElem(2)<<endl;

	return 0;
}
