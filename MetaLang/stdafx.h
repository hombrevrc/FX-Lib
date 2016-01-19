//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

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