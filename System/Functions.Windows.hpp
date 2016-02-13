//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

std::wstring GenerateGuid()
{
	GUID guid;
	ZeroMemory(&guid, sizeof(guid));
	HRESULT hr = CoCreateGuid(&guid);
	if (FAILED(hr))
	{
		throw SystemException("Couldn't create a new guid");
	}
	CComBSTR st = guid;
	std::wstring result = st;
	return result;
}
