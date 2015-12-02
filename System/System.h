//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Nullable.h"
#include "Delegate.h"
#include "Time.h"
#include "Type.h"

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
