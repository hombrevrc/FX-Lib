//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "PipeAcceptor.h"
#include "PipeConnector.h"

#ifdef _WIN64

#ifndef NDEBUG
#pragma comment(lib,"./Debug/Pipes.x64.lib")
#else
#pragma comment(lib,"./Release/Pipes.x64.lib")
#endif

#else

#ifndef NDEBUG
#pragma comment(lib,"./Debug/Pipes.x86.lib")
#else
#pragma comment(lib,"./Release/Pipes.x86.lib")
#endif

#endif
