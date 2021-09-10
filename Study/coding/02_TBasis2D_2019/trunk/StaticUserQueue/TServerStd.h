#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <typeinfo.h>
#include <list>
#include <queue>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};