#include "ardrone.h"
#include "autopilot.h"
#include "findComponentsHandle.h"
#include <math.h>

#define KEY_DOWN(key) (GetAsyncKeyState(key) & 0x8000)
#define KEY_PUSH(key) (GetAsyncKeyState(key) & 0x0001)

using namespace std;

void condition_move(speeds& speed, coordinates& position, time1& timer, int& flag_position, int& condition, Point2d goal)
{
	position.goal.x = goal.x;
	position.goal.y = goal.y;
	if ((!position.in_place()) && (flag_position == 0)) {
		cout << "NOT ON PLACE" << endl;
		position.preg(speed.vx, speed.vy);
	}
	else if (timer.show_ms() < 5000)
	{
		flag_position = 1;
		speed.vx = 0;
		speed.vy = 0;
	}
	else {
		flag_position = 0;
		condition += 1;
		position.zero();
		timer.clear();
	}
}

void condition_move(coordinates& position, speeds& speed, time1& timer, int& condition, double vx, double vy, double time1, double time2, bool stop)
{
	if (!stop) {
		if (timer.show_ms() < time1) {
			speed.vx = vx;
			speed.vy = vy;
		}
		else if (timer.show_ms() < (time1 + time2)) {
			speed.zero();
		}
		else if ((position.speed.x > 0.1) || (position.speed.y > 0.1)) {
			cout << "SPEED IS NOT ZERO" << endl;
			speed.zero();
		}
		else {
			condition += 1;
			timer.clear();
		}
	}
	else {
		cout << "I MUST STOP" << endl;
		if (timer.show_ms() < (time1 + time2)) {
			speed.zero();
		}
		else if ((position.speed.x > 0.1) || (position.speed.y > 0.1)) {
			cout << "SPEED IS NOT ZERO" << endl;
			speed.zero();
		}
		else {
			condition += 1;
			timer.clear();
		}
	}
}

void condition_move(speeds& speed, time1& timer, findComponentsHandle& main, int& condition, double vx, double vy)
{
	if (!main.on_image(main.comp1)) {
		speed.vx = vx;
		speed.vy = vy;
	}
	else {
		condition += 1;
		timer.clear();
	}
}
