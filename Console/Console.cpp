//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "../Logging/BinLogger.h"


int main()
{
    std::filesystem::path path = GetStartupDirectory();
    BinLogger logger(path, L"test");
    BinStream("evet") << std::string("test") >> logger;
    BinStream("evet") << std::string("test") >> logger;
    BinStream("evet") << std::string("test") >> logger;
    BinStream("evet") << std::string("test") >> logger;
    BinStream("evet") << std::string("test") >> logger;
    BinLogger::StopFlushingQueue();
    //std::cin.get();
}