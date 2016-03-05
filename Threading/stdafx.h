//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
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

#pragma warning (disable : 4091) // 'typedef ': ignored on left of '' when no variable is declared
#pragma warning (push)
#include <DbgHelp.h>
#pragma warning (pop)

#endif

#include <stdint.h>
#include <assert.h>
#include <vector>
#include <string>
#include <memory>
#include <atomic>
#include <thread>
#include <map>
#include <algorithm>
#include <exception>
#include <unordered_map>
#include <sstream>
#include <mutex>
#include <condition_variable>

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