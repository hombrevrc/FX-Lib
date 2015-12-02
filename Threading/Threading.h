//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "ThreadPool.h"
#include "PhysicalSingleton.h"
#include "LiteSingleton.h"
#include "Timer.h"


#ifdef _WIN64

#ifndef NDEBUG
#pragma comment(lib,"./Debug/Threading.x64.lib")
#else
#pragma comment(lib,"./Release/Threading.x64.lib")
#endif

#else

#ifndef NDEBUG
#pragma comment(lib,"./Debug/Threading.x86.lib")
#else
#pragma comment(lib,"./Release/Threading.x86.lib")
#endif

#endif
