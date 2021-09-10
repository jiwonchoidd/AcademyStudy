#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm> //include <algorithm> 
#include <iterator> // back_inserter
#include <memory>
using namespace std;

class TNode
{
public:
	int iData;
};
class MediaAsset
{
public:
	wstring title;
	virtual void print() { wcout << title; }
};
class Song : public MediaAsset
{
public:
	wstring artist;
	Song(wstring a, wstring b)
	{
		artist = a;
		title = b;
	}
}; 
class Photo : public MediaAsset
{
public:
	wstring date;
	wstring location_;
	Photo(wstring a, wstring b, wstring c )
	{
		date = a;
		location_ = b;
		title = c;
	}
};
class MyClass
{
private:
	// MyClass owns the unique_ptr.
	unique_ptr<MediaAsset> factory;
public:

	// Initialize by using make_unique with ClassFactory default constructor.
	MyClass() : factory(make_unique<MediaAsset>())
	{
	}

	void MakeClass()
	{
		factory->print();
	}
};
void PrintValue(TNode node)
{
	node.iData = 300;
	std::cout << node.iData << endl;;
}
void PrintPoint(TNode* node)
{
	node->iData = 600;
	std::cout << node->iData << endl;;
}
void PrintRef(const std::shared_ptr<TNode>& node)
{
	node->iData = 300;
	std::cout << node->iData << endl;;
}
// 참조만 가능하다.
void PrintUniqe(const std::unique_ptr<int[]>& node)
{
	node[0] = 300;
	std::cout << node[0] << endl;;
}

void UniqeCopy( std::unique_ptr<int[]>& node)
{
	std::unique_ptr<int[]> copyArray1 = std::move(node);
	//node[0] = 300; // 오류
	//std::cout << node[0];
	std::cout << copyArray1[0] << endl;
}

void main()
{
	//https://docs.microsoft.com/ko-kr/cpp/cpp/how-to-create-and-use-shared-ptr-instances?view=msvc-160
	std::shared_ptr<TNode> nodeA = std::make_shared<TNode>();
	TNode* A = nodeA.get();
	A->iData = 9;
	//delete pA; // 오류

	std::shared_ptr<TNode*> pNodeA = std::make_shared<TNode*>();
	TNode* pA = nodeA.get();
	pA->iData = 10;
	//delete pA; // 오류


	// 스마트 포인터
	//int* pData = int[10];		 
	//레퍼런스(참조) 카운팅을 지원한다.
	//명시적인 delete를 해주지 않아도 마지막에 참조하는 곳에서 
	//스코프를 벋어날때 자동으로 해제된다.	
	{
		std::shared_ptr<TNode> node(new TNode());
		node->iData = 100;
		std::cout << node.get()->iData << node->iData;

		auto mySmartPtr = std::make_shared<TNode>();
		mySmartPtr->iData = 888;
		// 대입연산자 -> 기존 값을 해제하고 대입한다.
		mySmartPtr = node;

		PrintValue(*node);      // 값
		PrintPoint(node.get()); // 주소
		PrintRef(mySmartPtr);   // 참조
	}
	{
		// unique_ptr은 대입 및 복제 생성자를 지원하지 않는다.
		// 그렇기 때문에 이동(std::move())만 가능하다. 주의-> const std::unique_ptr는 불가능하다.
		// reset() 메모리 해제
		std::unique_ptr<int[]> pArray(new int[10]);			
		pArray[0] = 11;
		pArray[1] = 22;
		// 대입 불가
		//std::unique_ptr<int[]> copy1 = pArray; // 에러
		// 복사생성자 불가
		//std::unique_ptr<int[]> copy2(pArray);  // 에러
		// 단, 다음처럼 포인터에 들어 있는 객체를 복사 및 참조가 가능하다.
		std::cout << endl;
		auto  ArrayValue = pArray[0];
		ArrayValue = 1111;
		std::cout << pArray[0] << "|" << ArrayValue << endl;

		auto& refArrayValue = pArray[1];
		int* pValue = &pArray[0]; 
		refArrayValue = 2222;
		std::cout << pArray[1] << "|" << refArrayValue << endl;;

		*pValue = 1234;
		std::cout << *pValue << pArray[0] << endl;
		PrintUniqe(pArray);
		//UniqeCopy(pArray);		
		
		std::unique_ptr<int[]> copyArray1 = std::move(pArray);
		const std::unique_ptr<int[]> copyArray2 = std::move(pArray);

		if (pArray != nullptr)
		{
			std::cout << "-------";
		}
		//std::cout << pArray[0] << std::endl; // 오류
		std::cout << copyArray1[0] << std::endl;
		std::cout << "-------";
	}

	// 방법1 :가능하면 make_shared를 사용하여 생성한다.
	auto sp1 = make_shared<Song>(L"The Beatles", L"Im Happy Just to Dance With You");	
	// 방법2
	shared_ptr<Song> sp2(new Song(L"Lady Gaga", L"Just Dance"));
	// nullptr 초기화 생성 이후 
	shared_ptr<Song> sp5(nullptr);
	sp5 = make_shared<Song>(L"Elton John", L"I'm Still Standing");
	////////////////////////////////////////////////////////////
	//이미 할당된 다른 shared_ptr하는 개체의 소유권을 공유하는 
	// shared_ptr 인스턴스를 어떻게 선언하고 초기화하는 예시.	
	//복사 생성자 초기화, 참조카운터 증가
	auto sp3(sp2);
	//할당을 통해 초기화,, 참조카운터 증가
	auto sp4 = sp2;
	sp4->artist = L"KGCA";
	// 교체
	sp1.swap(sp2);
	////////////////////////////////////////
	vector<shared_ptr<Song>> v;
	v.push_back(make_shared<Song>(L"Bob Dylan", L"The Times They Are A Changing"));
	v.push_back(make_shared<Song>(L"Aretha Franklin", L"Bridge Over Troubled Water"));
	v.push_back(make_shared<Song>(L"Thala", L"Entre El Mar y Una Estrella"));

	// 조건에 맞는 리스트 제외하고 복사한다.
	vector<shared_ptr<Song>> v2;
	remove_copy_if(v.begin(), v.end(), back_inserter(v2), [](shared_ptr<Song> s)
	{
		return s->artist.compare(L"Bob Dylan") == 0;
	});
	for (const auto& s : v2)
	{
		std::wcout << s->artist << L":" << s->title << endl;
	}

	////////////////////////////////////////////////////
	vector<shared_ptr<MediaAsset>> assets;
	assets.push_back(shared_ptr<Song>(new Song(L"Himesh Reshammiya", L"Tera Surroor")));
	assets.push_back(shared_ptr<Song>(new Song(L"Penaz Masani", L"Tu Dil De De")));
	assets.push_back(shared_ptr<Photo>(new Photo(L"2011-04-06", L"Redmond, WA", L"Soccer field at Microsoft.")));

	vector<shared_ptr<MediaAsset>> photos;

	copy_if(assets.begin(), assets.end(), back_inserter(photos), [](shared_ptr<MediaAsset> p) -> bool
	{
		shared_ptr<Photo> temp = dynamic_pointer_cast<Photo>(p);
		return temp.get() != nullptr;
	});

	for (const auto& p : photos)
	{
		wcout << "Photo location: " << (static_pointer_cast<Photo>(p))->location_ << endl;
	}

	//////////////////////////////////////////////////	
	auto song1 = new Song(L"Village People", L"YMCA");
	auto song2 = new Song(L"Village People", L"YMCA");	
	shared_ptr<Song> p1(song1);
	shared_ptr<Song> p2(song2);
	// 관련없는 shared_ptrs are never equal.
	wcout << "p1 < p2 = " << std::boolalpha << (p1 < p2) << endl;
	wcout << "p1 == p2 = " << std::boolalpha << (p1 == p2) << endl;
	// 관련 있는 shared_ptr instances are always equal.
	shared_ptr<Song> p3(p2);
	wcout << "p3 == p2 = " << std::boolalpha << (p3 == p2) << endl;


}