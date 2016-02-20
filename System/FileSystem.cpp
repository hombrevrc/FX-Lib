//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "FileSystem.h"

#ifdef FX_LIB_WINDOWS
#include "FileSystem.Windows.hpp"
#endif

#ifdef FX_LIB_LINUX
#include "FileSystem.Linux.hpp"
#endif
