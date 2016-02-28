//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

std::filesystem::path GetStartupDirectory()
{
	//init executable url
	char buffer[PATH_MAX] = "";
	sprintf(buffer, "/proc/%d/exe", getpid());
    std::string st = buffer;
    readlink(st.c_str(), buffer, sizeof(buffer));
	std::filesystem::path result = buffer;
	result = result.parent_path();
	return result;
}
