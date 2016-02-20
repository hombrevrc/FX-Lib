//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#ifdef FX_LIB_WINDOWS
namespace std
{
	typedef wstring tstring;
	typedef wstringstream tstringstream;
	typedef wostringstream tostringstream;
}
#endif

#ifdef FX_LIB_LINUX
namespace std
{
	typedef string tstring;
	typedef stringstream tstringstream;
	typedef ostringstream tostringstream;
}
#endif
