//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once


#include "../System/Sal.h"

#ifdef FX_LIB_WINDOWS
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif
#endif

#ifdef FX_LIB_WINDOWS
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include <atlbase.h>
#endif

#include <stdio.h>
#include <string>
#include <atomic>
#include <vector>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace std
{
	namespace filesystem
	{
		using namespace std::experimental::filesystem;
	}
}


#ifdef FX_LIB_WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new (_CLIENT_BLOCK, __FILE__, __LINE__)
#endif
#endif


#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


#include "../System/System.h"
#include "../Logging/Logging.h"