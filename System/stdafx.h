//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Sal.h"

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

#ifdef FX_LIB_LINUX

#include <sys/types.h>
#include <unistd.h>
#include <linux/limits.h>
#include <linux/uuid.h>
#include <sys/time.h>

#endif


#include "Std.h"

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

#include "Types.h"