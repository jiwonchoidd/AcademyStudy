#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;
//https://docs.microsoft.com/ko-kr/cpp/cpp/how-to-create-and-use-unique-ptr-instances?view=msvc-160

struct MediaAsset
{
	virtual ~MediaAsset() = default; // make it polymorphic
};

struct Song : public MediaAsset
{
	std::wstring artist;
	std::wstring title;
	Song() {}
	Song(const std::wstring& artist_, const std::wstring& title_) :
		artist{ artist_ }, title{ title_ } {}
	void Show()
	{
		std::wcout << title;
	}
};

struct Photo : public MediaAsset
{
	std::wstring date;
	std::wstring location;
	std::wstring subject;
	Photo(
		const std::wstring& date_,
		const std::wstring& location_,
		const std::wstring& subject_) :
		date{ date_ }, location{ location_ }, subject{ subject_ } {}
};
unique_ptr<Song> SongFactory(const std::wstring& artist, const std::wstring& title)
{
	// Implicit move operation into the variable that stores the result.
	return make_unique<Song>(artist, title);
}

void MakeSongs()
{
	// Create a new unique_ptr with a new object.
	auto song = make_unique<Song>(L"Mr. Children", L"Namonaki Uta");

	// Use the unique_ptr.
	vector<wstring> titles = { song->title };

	// Move raw pointer from one unique_ptr to another.
	unique_ptr<Song> song2 = std::move(song);

	// Obtain unique_ptr from function that returns by value.
	auto song3 = SongFactory(L"Michael Jackson", L"Beat It");
}

void SongVector()
{
	vector<unique_ptr<Song>> songs;

	// Create a few new unique_ptr<Song> instances
	// and add them to vector using implicit move semantics.
	songs.push_back(make_unique<Song>(L"B'z", L"Juice"));
	songs.push_back(make_unique<Song>(L"Namie Amuro", L"Funky Town"));
	songs.push_back(make_unique<Song>(L"Kome Kome Club", L"Kimi ga Iru Dake de"));
	songs.push_back(make_unique<Song>(L"Ayumi Hamasaki", L"Poker Face"));

	// Pass by const reference when possible to avoid copying.
	for (const auto& song : songs)
	{
		wcout << L"Artist: " << song->artist << L"   Title: " << song->title << endl;
	}
}


class MyClass
{
private:
	// MyClass owns the unique_ptr.
	unique_ptr<Song> factory;
public:

	// Initialize by using make_unique with ClassFactory default constructor.
	MyClass() : factory(make_unique<Song>())
	{
	}

	void MakeClass()
	{
		factory->Show();
	}
};

class Manager
{
public:
	vector<unique_ptr<Song>> songs;
	void Add(const std::wstring& artist_, const std::wstring& title_)
	{
		songs.push_back(make_unique<Song>(artist_, title_));
	}
	Song& Get(std::wstring artist)
	{
		for (auto& data : songs)
		{
			if (data->artist == artist)
			{
				return *data;
			}
		}
	}
};
void main()
{
	//MakeSongs();
	//SongVector();

	//MyClass c;
	//c.MakeClass();


	Manager mgr;
	mgr.Add(L"B'z", L"Juice");
	mgr.Add(L"Namie Amuro", L"Funky Town");
	mgr.Add(L"Kome Kome Club", L"Kimi ga Iru Dake de");
	mgr.Add(L"Ayumi Hamasaki", L"Poker Face");
	{
		Song& pSong = mgr.Get(L"Namie Amuro");
		pSong.artist = L"aa";
	}
	//delete pSong;
	///////////////////////////////////////////////////
	// Create a unique_ptr to an array of 5 integers.
	//auto p = make_unique<int[]>(5);

	//// Initialize the array.
	//for (int i = 0; i < 5; ++i)
	//{
	//	p[i] = i;
	//	wcout << p[i] << endl;
	//}
}