//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "targetver.h"

#include <windows.h>
#include <stdint.h>
#include <string>
#include <atomic>
#include <chrono>
#include <thread>
#include <algorithm>
#include <assert.h>
#include <map>
#include <vector>
#include <unordered_map>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include "../System/Sal.h"
#include "../System/System.h"
#include "../Threading/Threading.h"
#include "../Remoting/Remoting.h"