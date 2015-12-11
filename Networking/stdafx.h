//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <WinSock2.h>
#include <ws2tcpip.h>

#include <atomic>
#include <sstream>


#include <limits>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif