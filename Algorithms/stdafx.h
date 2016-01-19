//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif
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