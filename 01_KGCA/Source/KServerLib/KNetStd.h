#pragma once
#include <winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h> // inet_ntop
#include <iostream>
#include <list>
#include <string>
#pragma comment	(lib, "ws2_32.lib")

#ifdef DEBUG
#pragma comment	(lib, "KServerLib_d.lib")
#endif // DEBUG
