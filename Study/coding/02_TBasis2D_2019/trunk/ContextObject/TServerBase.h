#pragma once
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <MSWSock.h> // AcceptEx
#include <Windows.h>
#include <atomic>
#include <thread>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "DbgHelp.lib")