//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Sal.h"
#include "Std.h"


#ifdef FX_LIB_WINDOWS
#include "Types.h"
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
#include "FileSystem.h"
#include "SystemClock.h"
#include "DateTime.h"
#endif

#ifdef FX_LIB_LINUX
#include "Types.h"
#include "Nullable.h"
//#include "Delegate.h"
#include "Functions.h"
#include "FileSystem.h"
#endif