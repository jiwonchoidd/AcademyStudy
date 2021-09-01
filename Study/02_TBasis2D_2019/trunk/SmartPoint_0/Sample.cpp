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
// ������ �����ϴ�.
void PrintUniqe(const std::unique_ptr<int[]>& node)
{
	node[0] = 300;
	std::cout << node[0] << endl;;
}

void UniqeCopy( std::unique_ptr<int[]>& node)
{
	std::unique_ptr<int[]> copyArray1 = std::move(node);
	//node[0] = 300; // ����
	//std::cout << node[0];
	std::cout << copyArray1[0] << endl;
}

void main()
{
	//https://docs.microsoft.com/ko-kr/cpp/cpp/how-to-create-and-use-shared-ptr-instances?view=msvc-160
	std::shared_ptr<TNode> nodeA = std::make_shared<TNode>();
	TNode* A = nodeA.get();
	A->iData = 9;
	//delete pA; // ����

	std::shared_ptr<TNode*> pNodeA = std::make_shared<TNode*>();
	TNode* pA = nodeA.get();
	pA->iData = 10;
	//delete pA; // ����


	// ����Ʈ ������
	//int* pData = int[10];		 
	//���۷���(����) ī������ �����Ѵ�.
	//������� delete�� ������ �ʾƵ� �������� �����ϴ� ������ 
	//�������� ����� �ڵ����� �����ȴ�.	
	{
		std::shared_ptr<TNode> node(new TNode());
		node->iData = 100;
		std::cout << node.get()->iData << node->iData;

		auto mySmartPtr = std::make_shared<TNode>();
		mySmartPtr->iData = 888;
		// ���Կ����� -> ���� ���� �����ϰ� �����Ѵ�.
		mySmartPtr = node;

		PrintValue(*node);      // ��
		PrintPoint(node.get()); // �ּ�
		PrintRef(mySmartPtr);   // ����
	}
	{
		// unique_ptr�� ���� �� ���� �����ڸ� �������� �ʴ´�.
		// �׷��� ������ �̵�(std::move())�� �����ϴ�. ����-> const std::unique_ptr�� �Ұ����ϴ�.
		// reset() �޸� ����
		std::unique_ptr<int[]> pArray(new int[10]);			
		pArray[0] = 11;
		pArray[1] = 22;
		// ���� �Ұ�
		//std::unique_ptr<int[]> copy1 = pArray; // ����
		// ��������� �Ұ�
		//std::unique_ptr<int[]> copy2(pArray);  // ����
		// ��, ����ó�� �����Ϳ� ��� �ִ� ��ü�� ���� �� ������ �����ϴ�.
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
		//std::cout << pArray[0] << std::endl; // ����
		std::cout << copyArray1[0] << std::endl;
		std::cout << "-------";
	}

	// ���1 :�����ϸ� make_shared�� ����Ͽ� �����Ѵ�.
	auto sp1 = make_shared<Song>(L"The Beatles", L"Im Happy Just to Dance With You");	
	// ���2
	shared_ptr<Song> sp2(new Song(L"Lady Gaga", L"Just Dance"));
	// nullptr �ʱ�ȭ ���� ���� 
	shared_ptr<Song> sp5(nullptr);
	sp5 = make_shared<Song>(L"Elton John", L"I'm Still Standing");
	////////////////////////////////////////////////////////////
	//�̹� �Ҵ�� �ٸ� shared_ptr�ϴ� ��ü�� �������� �����ϴ� 
	// shared_ptr �ν��Ͻ��� ��� �����ϰ� �ʱ�ȭ�ϴ� ����.	
	//���� ������ �ʱ�ȭ, ����ī���� ����
	auto sp3(sp2);
	//�Ҵ��� ���� �ʱ�ȭ,, ����ī���� ����
	auto sp4 = sp2;
	sp4->artist = L"KGCA";
	// ��ü
	sp1.swap(sp2);
	////////////////////////////////////////
	vector<shared_ptr<Song>> v;
	v.push_back(make_shared<Song>(L"Bob Dylan", L"The Times They Are A Changing"));
	v.push_back(make_shared<Song>(L"Aretha Franklin", L"Bridge Over Troubled Water"));
	v.push_back(make_shared<Song>(L"Thala", L"Entre El Mar y Una Estrella"));

	// ���ǿ� �´� ����Ʈ �����ϰ� �����Ѵ�.
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
	// ���þ��� shared_ptrs are never equal.
	wcout << "p1 < p2 = " << std::boolalpha << (p1 < p2) << endl;
	wcout << "p1 == p2 = " << std::boolalpha << (p1 == p2) << endl;
	// ���� �ִ� shared_ptr instances are always equal.
	shared_ptr<Song> p3(p2);
	wcout << "p3 == p2 = " << std::boolalpha << (p3 == p2) << endl;


}