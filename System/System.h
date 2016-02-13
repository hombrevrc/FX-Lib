//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Sal.h"
#include "Nullable.h"
#include "Delegate.h"
#include "Time.h"
#include "Type.h"
#include "SystemException.h"
#include "TimeoutException.h"
#include "SharedMemory.h"
#include "Functions.h"
#include "MemoryStream.h"
#include "Timeout.h"
#include "Enumeration.h"

#ifdef FX_LIB_WINDOWS
#ifdef _WIN64

#ifndef NDEBUG
#pragma comment(lib,"./Debug/System.x64.lib")
#else
#pragma comment(lib,"./Release/System.x64.lib")
#endif

#else

#ifndef NDEBUG
#pragma comment(lib,"./Debug/System.x86.lib")
#else
#pragma comment(lib,"./Release/System.x86.lib")
#endif

#endif

#endif