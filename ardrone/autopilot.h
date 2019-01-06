#pragma once

#include "findComponentsHandle.h"
#include "coordinates.h"
#include "timer.h"

const double Pi = 3.1415926535;

struct speeds
{
	double vx, vy, vz, vr;
	void zero()
	{
		vx = 0;
		vy = 0;
		vz = 0;
		vr = 0;
	};
	speeds()
	{
		zero();
	};
};

void condition_move(speeds& speed, coordinates& position, time1& timer, int& flag_position, int& condition, Point2d goal);
void condition_move(coordinates& position, speeds& speed, time1& timer, int& condition, double vx, double vy, double time1, double time2, bool stop = 0);
void condition_move(speeds& speed, time1& timer, findComponentsHandle& main, int& condition, double vx, double vy);