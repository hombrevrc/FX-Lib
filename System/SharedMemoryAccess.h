//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

enum class SharedMemoryAccess
{
	None = 0,
	Read = 1,
	Write = 2,
	ReadAndWrite = 3 // = Read | Write
};