#pragma once

#include "timer.h"
#include "findComponentsHandle.h"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

double sign(double& value);

class coordinates
{
private:
	double dx, dy;
	time1 timer;
	const double kx = 0.5;
	const double ky = 0.5;
	const double kdx = 0.5;
	const double kdy = 0.5;
	double old_dx, old_dy;

public:
	coordinates();
	Point2d pos, goal, speed;
	void update();
	void zero();
	void preg(double& vx, double& vy);
	bool in_place();
};