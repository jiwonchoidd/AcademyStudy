#include <iostream>
#include <chrono>
#include <random>
using namespace std;
using namespace chrono;

void main()
{
	auto start = system_clock::now();
	double d = 0;
	for (int i = 0; i < 1000; ++i)
	{
		d += sqrt(sin(i) * cos(i));
	}
	auto end = system_clock::now();

	auto diff = end - start;
	cout << duration<double, milli>(diff).count() << "ms" << endl;


	// ·£´ý¾¾µå	
	random_device rndDevice;
	mt19937 eng(rndDevice());

	// ³­¼ö¹ß»ý	
	uniform_int_distribution<int> dist(1, 99);
	cout << dist(eng) << endl;

	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
	std::default_random_engine e(seed);
	cout << e() << endl;
	// range : [e.min(), e.max()]

	// Range : 
	cout << e() % 6 << endl;
	// 1. Bad quality of randomness
	// 2. Can only provide uniform distribution 

	// Generate integer value
	std::uniform_int_distribution<int> distr(0, 5);
	// range : [0, 5]
	cout << distr(e) << endl;
	// random engine 'e' provides the source of randomness
	// 'distr' provides the way to distrubute the random number within certain range 

	// Generate real value 
	std::uniform_real_distribution<double> distrR(0, 5);
	cout << distrR(e) << endl;
	// range : [0, 5)

	std::poisson_distribution<int> distrP(1.0); // mean
	cout << distrP(e) << endl;

	cout << "noraml_distribution: " << endl;
	std::normal_distribution<double> distrN(10.0, 3.0);
	vector<int> v(20);
	for (int i = 0; i < 800; i++)
	{
		int num = distrN(e);
		if (num >= 0 && num < 20)
			v[num]++;
	}

	for (int i = 0; i < 20; i++)
	{
		std::string data = std::string(v[i], '*');
		cout << i << ": " << data.c_str() << endl;
	}
	cout << endl;

	cin.get();
	return;	
}