//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

uint64_t GetTimeCounterNow();

double GetMillisecondsFromTimeCounter(uint64_t timeCounter);
double GetMicrosecondsFromTimeCounter(uint64_t timeCounter);
double GetSecondsFromTimeCounter(uint64_t timeCounter);

double GetMillisecondsFromTimeCounter(double timeCounter);
double GetMicrosecondsFromTimeCounter(double timeCounter);
double GetSecondsFromTimeCounter(double timeCounter);

uint64_t GetTicksFromMicroseconds(double timeInMks);
