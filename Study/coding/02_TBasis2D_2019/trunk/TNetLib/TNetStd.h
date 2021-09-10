#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TProtocol.h"

#if NDEBUG
#pragma comment (lib, "TNetLib_R.lib")
#else
#pragma comment (lib, "TNetLib_D.lib")
#endif

