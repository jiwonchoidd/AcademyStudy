#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <TBasisStd.h>
#include "TProtocol.h"
#include <chrono>

typedef std::chrono::high_resolution_clock TClock; // TClock::now()
//auto t1 = TClock::now();
//auto t2 = TClock::now();
//std::cout << "Delta t2-t1: "
//			<< std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
//			<< " nanoseconds" << std::endl;

#if NDEBUG
	#pragma comment (lib, "TNetCore_R.lib")
#else
	#pragma comment (lib, "TNetCore_D.lib")
#endif

namespace TBASIS_SERVER_LIB
{
}
namespace T_SERVER = TBASIS_SERVER_LIB; // º°Äª »ç¿ë