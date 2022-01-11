#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <list>
#include <string>
#pragma comment	(lib, "ws2_32.lib")

#ifdef DEBUG
#pragma comment	(lib, "KServerLib_d.lib")
#endif // DEBUG
