#include <iostream>
#include <cmath>
#include "coordinates.h"

using namespace std;

coordinates::coordinates()
{
	pos.x = 0;
	pos.y = 0;
	speed.x = 0;
	speed.y = 0;
	goal.x = 0;
	goal.y = 0;
	dx = 0;
	dy = 0;
	old_dx = 0;
	old_dy = 0;
	timer.clear();
}

double sign(double& value)
{
	if (value == 0) return 0;
	else if (value > 0) return 1;
	else return -1;
}

void coordinates::update()
{
	pos.x += timer.show_s() * speed.x;
	pos.y += timer.show_s() * speed.y;
	timer.clear();
}

void coordinates::zero()
{
	pos.x = 0;
	pos.y = 0;
}

void coordinates::preg(double& vx, double& vy)
{
	dx = goal.x - pos.x;
	dy = goal.y - pos.y;

	if (abs(dx) < 0.2)		vx = 0;
	else if (abs(dx) < 0.5)	vx = sign(dx) * 0.5;
	else if (abs(dx) < 1.0)	vx = sign(dx) * 0.8;
	else					vx = sign(dx) * 1.2;

	if (abs(dy) < 0.2)		vy = 0;
	else if (abs(dy) < 0.5)	vy = sign(dy) * 0.5;
	else if (abs(dy) < 1.0)	vy = sign(dy) * 0.8;
	else					vy = sign(dy) * 1.2;

	old_dx = dx;
	old_dy = dy;
}

bool coordinates::in_place()
{
	dx = goal.x - pos.x;
	dy = goal.y - pos.y;
	return ((abs(dx) < 0.3) && (abs(dy) < 0.3));
}
