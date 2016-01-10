//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <Windows.h>
#pragma warning (disable : 4091) // 'typedef ': ignored on left of '' when no variable is declared
#pragma warning (push)
#include <DbgHelp.h>
#pragma warning (pop)
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <string>
#include <process.h>
#include <memory>
#include <atomic>
#include <thread>
#include <map>
#include <algorithm>
#include <exception>
#include <unordered_map>
#include <sstream>

#include "../System/System.h"

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif
