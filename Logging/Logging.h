//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once



#ifdef _WIN64

#ifndef NDEBUG
#pragma comment(lib,"./Debug/Logging.x64.lib")
#else
#pragma comment(lib,"./Release/Logging.x64.lib")
#endif

#else

#ifndef NDEBUG
#pragma comment(lib,"./Debug/Logging.x86.lib")
#else
#pragma comment(lib,"./Release/Logging.x86.lib")
#endif

#endif
