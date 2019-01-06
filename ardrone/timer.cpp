#include "timer.h"

time1::time1()
{
	clear();
}

void time1::clear()
{
	start_time = clock();
	own_time = 0;
}

int time1::show_ms()
{
	own_time = clock() - start_time;
	return (int)own_time;
}

double time1::show_s()
{
	double result = show_ms();
	return result / 1000;
}
