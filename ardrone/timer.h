#pragma once

#include <time.h>
#include <windows.h>

class time1
{
private:
	clock_t own_time;
	clock_t start_time;

public:
	time1();
	int show_ms();
	double show_s();
	void clear();
};
