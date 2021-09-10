#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#define tNewData(newData) std::make_shared<newData>()

struct TStruct
{
	int k = 3;
	TStruct()
	{
		k = rand();
	}
	~TStruct()
	{
		int k = 0;
	}
};
struct TStruct2
{
	int k = 3;
	TStruct2()
	{
		k = rand();
	}
	~TStruct2()
	{
		int k = 0;
	}
};
using tData = std::shared_ptr<TStruct>;
using tData2 = std::shared_ptr<TStruct2>;


TStruct Get1()
{
	TStruct k;
	k.k = 10;
	return std::move(k);
}
void Get2(TStruct& ret)
{
	TStruct k;
	k.k = 10;
	ret = std::move(k);
}
void Put1(TStruct& ret)
{
	std::cout << ret.k;
}
void Put1(TStruct* ret)
{
	std::cout << ret->k;
}
void Put2(tData ret)
{
	std::cout << ret->k;
}
void Print(tData data)
{
	std::cout << data->k << " ";
}
struct tPrint
{
	void operator() (tData data)
	{
		std::cout << data->k << " ";
	}
};

tData Sum(tData data)
{
	data->k -= data->k;
	return data;
}
void main()
{
	{
		tData node(new TStruct);
		node->k = 9;
		auto newNode = std::make_shared<TStruct>();
		newNode->k = 8;
		newNode = node;
		Put2(newNode);
		Put2(node);
	}
	{
		std::vector<tData> array;
		array.push_back(tNewData(TStruct));
		array.push_back(std::make_shared<TStruct>());

		std::vector<tData2> array2;
		array2.push_back(tNewData(TStruct2));
	}

	{
		std::vector<tData> array3;
		array3.push_back(tData(new TStruct()));
		array3.push_back(tData(new TStruct()));


		// std::foreach, std::transform, std::getnerate
		std::for_each(array3.begin(), array3.end(), Print);
		std::for_each(array3.begin(), array3.end(), tPrint());
		std::for_each(array3.begin(), array3.end(),
			[](tData data) {std::cout << data->k << " "; });

		std::vector<tData> array4;
		std::copy(array3.begin(), array3.end(), std::back_inserter(array4));

		std::vector<tData> array6;
		std::copy(array3.begin(), array3.end(), std::back_inserter(array6));
		std::transform(array4.begin(), array4.end(),
			array4.begin(),
			Sum);

		auto ramda2 = [](tData data)->tData { data->k -= data->k;	return data; };
		std::transform(array4.begin(), array4.end(),
			array4.begin(),
			ramda2);

		std::transform(array4.begin(), array4.end(),
			array4.begin(),
			[](tData data)->tData
		{
			data->k -= data->k;	return data;
		}
		);

		std::transform(array4.begin(), array4.end(),
			array4.begin(), ramda2);

		std::transform(array4.begin(), array4.end(),
			std::back_inserter(array6),
			Sum);

		std::vector<tData> array7(3);
		std::generate(array7.begin(), array7.end(),
			[]() { return tNewData(TStruct); });
		std::generate_n(std::back_inserter(array7), 5,
			[]() { return tNewData(TStruct); });
		//std::generate(array4.begin(), array4.end(), tPrint());
		//std::generate(array4.begin(), array4.end(), ramda2);



	}
}