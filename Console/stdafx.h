//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <string>
#include <atomic>
#include <vector>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <iostream>

#include <fstream>

#include "../System/System.h"
#include "../Threading/Threading.h"
#include "../Logging/Logging.h"
#include "../Remoting/Remoting.h"
#include "../Pipes/Pipes.h"


#include <filesystem>

namespace std
{
	namespace filesystem
	{
		using namespace std::experimental::filesystem;
	}
}