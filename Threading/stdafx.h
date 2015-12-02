//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <Windows.h>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <process.h>
#include <memory>
#include <atomic>
#include <thread>
#include <map>
#include <algorithm>
#include <unordered_map>

#include "../System/System.h"

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif
