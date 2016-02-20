//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#ifdef FX_LIB_WINDOWS
#ifdef new
#pragma push_macro("new")
#undef new
#define FX_STD_NEW
#endif
#endif

#include <stdint.h>
#include <assert.h>
#include <string>
#include <atlbase.h>
#include <exception>
#include <sstream>
#include <chrono>
#include <filesystem>
#include <mutex>

namespace std
{
	namespace filesystem
	{
		using namespace std::experimental::filesystem;
	}
}

#ifdef FX_LIB_WINDOWS
#ifdef FX_STD_NEW
#pragma pop_macro("new")
#undef FX_STD_NEW
#endif
#endif
