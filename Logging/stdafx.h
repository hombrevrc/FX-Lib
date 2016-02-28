//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

#include "../System/Sal.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <atomic>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <filesystem>

#ifdef FX_LIB_WINDOWS
#include <windows.h>
#include <atlbase.h>
#endif

namespace std
{
	namespace filesystem
	{
		using namespace std::experimental::filesystem;
	}
}

#ifdef _DEBUG
#include <crtdbg.h>
#define new new (_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include "../System/System.h"
#include "../Threading/Threading.h"
