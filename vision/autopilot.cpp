#include "ardrone.h"
#include "autopilot.h"
#include "findComponentsHandle.h"
#include <math.h>

const double Pi = 3.1415926535;

using namespace std;

const int time1 = 700;

void autopilot_front(ARDrone& ardrone, findComponentsHandle& main)
{
	cout << "Start Autopilot Front" << endl;
	while ()
	cout << "Finish Autopilot Front" << endl;
}

void move_front(ARDrone& ardrone)
{
	cout << "Start MOVE FRONT" << endl;
	int key = cv::waitKey(33);
	ardrone.move3D(1.0, 0.0, 0.0, 0.0);
	key = cv::waitKey(time1);
	ardrone.move3D(0.0, 0.0, 0.0, 0.0);
	cout << "Finish MOVE FRONT" << endl;
}

void move_back(ARDrone& ardrone)
{
	cout << "Start MOVE BACK" << endl;
	int key = cv::waitKey(33);
	ardrone.move3D(-1.0, 0.0, 0.0, 0.0);
	key = cv::waitKey(time1);
	ardrone.move3D(0.0, 0.0, 0.0, 0.0);
	cout << "Finish MOVE BACK" << endl;
}

void move_left(ARDrone& ardrone)
{
	cout << "Start MOVE LEFT" << endl;
	int key = cv::waitKey(33);
	ardrone.move3D(0.0, 1.0, 0.0, 0.0);
	key = cv::waitKey(time1);
	ardrone.move3D(0.0, 0.0, 0.0, 0.0);
	cout << "Finish MOVE LEFT" << endl;
}

void move_right(ARDrone& ardrone)
{
	cout << "Start MOVE RIGHT" << endl;
	int key = cv::waitKey(33);
	ardrone.move3D(0.0, -1.0, 0.0, 0.0);
	key = cv::waitKey(time1);
	ardrone.move3D(0.0, 0.0, 0.0, 0.0);
	cout << "Finish MOVE RIGHT" << endl;
}

/* side = { 1, -1} */
void around_column(ARDrone& ardrone, int side)
{
	int key = cv::waitKey(33);

	//first move
	for (double t = 0.0; t < 200; t += 4.0)
	{
		double vx = 0.0, vy = 0.0, vz = 0.0;
		vx = side * 1;
		vy = side * 0;
		ardrone.move(vx, vy, vz);
		std::cout << vx << " " << vy << std::endl;
		key = cv::waitKey(20);
		if (key == ' ')
			break;
	}
	ardrone.move(0.0, 0.0, 0.0);
	std::cout << "Pause" << std::endl;
	key = cv::waitKey(2000);

	//move around
	for (double t = 0.0; t < 200; t += 1.0)
	{
		double vx = 0.0, vy = 0.0, vz = 0.0;
		vx = side * cos(t * Pi / 200);
		vy = side * 0;
		ardrone.move(vx, vy, vz);
		std::cout << vx << " " << vy << std::endl;
		key = cv::waitKey(20);
		if (key == ' ') 
			break;
	}
	ardrone.move(0.0, 0.0, 0.0);
	std::cout << "Pause" << std::endl;
	key = cv::waitKey(2000);

	//comeback move
	for (double t = 0.0; t < 200; t += 4.0)
	{
		double vx = 0.0, vy = 0.0, vz = 0.0;
		vx = side * -1;
		vy = side * 0;
		ardrone.move(vx, vy, vz);
		std::cout << vx << " " << vy << std::endl;
		key = cv::waitKey(20);
		if (key == ' ')
			break;
	}
	ardrone.move(0.0, 0.0, 0.0);
}

