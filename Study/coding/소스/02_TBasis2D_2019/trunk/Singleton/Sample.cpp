#include <iostream>
#include <map>
#include <string>
using namespace std;

template<class T>
class Singleton
{
public:
	virtual string GetName() = 0;
public:
	static bool Register(const char* name, T* obj);
	static T* CreateInstance(string name);
private:
	static T* pInstance;
	static map<string, T*> registerList;
};
template<class T> T* Singleton<T>::pInstance = 0;
template<class T> map<string, T*> Singleton<T>::registerList;

template<class T>
bool Singleton<T>::Register(const char* name, T* obj) 
{
	if (registerList.size() < 2)
	{
		string szfullname = name + std::to_string(registerList.size());
		registerList[szfullname] = obj;
		return true;
	}
	return false;
}
template<class T>
T* Singleton<T>::CreateInstance(string name)
{
	auto obj = registerList.find(name);
	if (obj== registerList.end())
	{
		static T newObj;
		if (Register(name.c_str(), &newObj))
		{
			return &newObj;
		}
		else
		{
			return nullptr;
		}
	}
	if(pInstance == 0) pInstance = registerList[name];
	else pInstance = registerList[name];
	return pInstance;
}

class MySingletonA : public Singleton<MySingletonA>
{
	friend class Singleton<MySingletonA>;
public:
	string szName;
	virtual string GetName() override
	{
		return szName;
	}
protected:
	MySingletonA() {
		//Singleton::Register("MySingletonA", this);
		szName = "MySingletonA";
	}
public:
	virtual ~MySingletonA() {}
};
class MySingletonB : public Singleton<MySingletonB>
{
	friend class Singleton<MySingletonB>;
public:
	string szName;
	virtual string GetName() override
	{
		return szName;
	}
protected:
	MySingletonB() {
		//Singleton::Register("MySingletonB", this);
		szName = "MySingletonB";
	}
public:
	virtual ~MySingletonB() {}
};


void main()
{
	MySingletonA* objA = Singleton<MySingletonA>::CreateInstance("MySingletonA");
	MySingletonA* objB = Singleton<MySingletonA>::CreateInstance("MySingletonA");
	MySingletonA* objC = Singleton<MySingletonA>::CreateInstance("MySingletonA");
	MySingletonA* objD = Singleton<MySingletonA>::CreateInstance("MySingletonA1");
	MySingletonB* objE = Singleton<MySingletonB>::CreateInstance("MySingletonB");
	std::cout << objA->GetName().c_str() << std::endl;
	std::cout << objB->GetName().c_str() << std::endl;
	//std::cout << objC->GetName().c_str() << std::endl;
	std::cout << objD->GetName().c_str() << std::endl;
	std::cout << objE->GetName().c_str() << std::endl;
	std::cout << "end";
}
