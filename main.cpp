#include "ardrone/ardrone.h"
#include "ardrone/autopilot.h"
#include "ardrone/findComponentsHandle.h"
#include "windows.h"
#include "ardrone/coordinates.h"
#include <math.h>

#define KEY_DOWN(key) (GetAsyncKeyState(key) & 0x8000)
#define KEY_PUSH(key) (GetAsyncKeyState(key) & 0x0001)

using namespace std;

void show_info_start(ARDrone& ardrone)
{
	std::cout << "Battery = " << ardrone.getBatteryPercentage() << "[%]" << std::endl;
	std::cout << "***************************************" << std::endl;
	std::cout << "*                                     *" << std::endl;
	std::cout << "*    'Space' -- Takeoff/Landing       *" << std::endl;
	std::cout << "*    'Up'    -- Move forward          *" << std::endl;
	std::cout << "* - Controls -                        *" << std::endl;
	std::cout << "*    'Down'  -- Move backward         *" << std::endl;
	std::cout << "*    'Left'  -- Move left             *" << std::endl;
	std::cout << "*    'Right' -- Move right            *" << std::endl;
	std::cout << "*    'Shift' -- Move upward           *" << std::endl;
	std::cout << "*    'Ctrl'  -- Move downward         *" << std::endl;
	std::cout << "*    'A'     -- Turn left             *" << std::endl;
	std::cout << "*    'D'     -- Turn right            *" << std::endl;
	std::cout << "*                                     *" << std::endl;
	std::cout << "* - Others -                          *" << std::endl;
	std::cout << "*    'Z'     -- Change camera         *" << std::endl;
	std::cout << "*    'Esc'   -- Exit                  *" << std::endl;
	std::cout << "*                                     *" << std::endl;
	std::cout << "***************************************" << std::endl;
}

void show_info(ARDrone& ardrone)
{
	cout << "roll: " << ardrone.getRoll() * RAD_TO_DEG << endl;
	cout << "pitch: " << ardrone.getPitch() * RAD_TO_DEG << endl;
	cout << "yaw: " << ardrone.getYaw() << endl;
	cout << "altitude: " << ardrone.getAltitude() << endl;
	cout << "velocity: " << ardrone.getVelocity() << endl << endl;
}

void movement_control(speeds& speed)
{
	if (KEY_DOWN(VK_UP))		speed.vx = 1.0;
	if (KEY_DOWN(VK_DOWN))		speed.vx = -1.0;
	if (KEY_DOWN(VK_LEFT))		speed.vy = 1.0;
	if (KEY_DOWN(VK_RIGHT))		speed.vy = -1.0;
	if (KEY_DOWN(VK_LSHIFT))	speed.vz = 1.0;
	if (KEY_DOWN(VK_LCONTROL))	speed.vz = -1.0;
	if (KEY_DOWN('A'))			speed.vr = 2.0;
	if (KEY_DOWN('D'))			speed.vr = -2.0;
}

void on_mouse(int event, int x, int y, int d, void *ptr)
{
	/* https://stackoverflow.com/questions/15570431/opencv-return-value-from-mouse-callback-function */

	findComponentsHandle*p = (findComponentsHandle*)ptr;

	if (event == EVENT_LBUTTONDOWN) {
		p->calibrate_color(x, y, p->B1);
		p->c1 = { x, y };
		std::cout << "Left button clicked in: " << x << " " << y << endl;
	}
	if (event == EVENT_RBUTTONDOWN) {
		p->calibrate_color(x, y, p->B2);
		p->c2 = { x, y };
		std::cout << "Right button clicked in: " << x << " " << y << endl;
	}
}

void find_center_cross(speeds& speed, coordinates& position, time1& timer_cross, double& cross_size)
{
	if (timer_cross.show_ms() < ((1 * 500) + (0 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((1 * 500) + (1 * 1000 * cross_size))) {
		speed.zero();
		speed.vx = 1.0;
	}
	else if (timer_cross.show_ms() < ((2 * 500) + (1 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((2 * 500) + (2 * 1000 * cross_size))) {
		speed.zero();
		speed.vx = -1.0;
	}
	else if (timer_cross.show_ms() < ((3 * 500) + (2 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((3 * 500) + (3 * 1000 * cross_size))) {
		speed.zero();
		speed.vx = -1.0;
	}
	else if (timer_cross.show_ms() < ((4 * 500) + (3 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((4 * 500) + (4 * 1000 * cross_size))) {
		speed.zero();
		speed.vx = 1.0;
	}
	// здесь мы вернулись в 0
	else if (timer_cross.show_ms() < ((5 * 500) + (4 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((5 * 500) + (5 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = 1.0;
	}
	else if (timer_cross.show_ms() < ((6 * 500) + (5 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((6 * 500) + (6 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = -1.0;
	}
	else if (timer_cross.show_ms() < ((7 * 500) + (6 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((7 * 500) + (7 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = -1.0;
	}
	else if (timer_cross.show_ms() < ((8 * 500) + (7 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((8 * 500) + (8 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = 1.0;
	}

	// снова в 0, первый наискосок
	else if (timer_cross.show_ms() < ((9 * 500) + (8 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((9 * 500) + (9 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = 0.7;
		speed.vx = 0.7;
	}
	else if (timer_cross.show_ms() < ((10 * 500) + (9 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((10 * 500) + (10 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = -0.7;
		speed.vx = -0.7;
	}
	else if (timer_cross.show_ms() < ((11 * 500) + (10 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((11 * 500) + (11 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = -0.7;
		speed.vx = -0.7;
	}
	else if (timer_cross.show_ms() < ((12 * 500) + (11 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((12 * 500) + (12 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = 0.7;
		speed.vx = 0.7;
	}
	// снова 0, второй наискосок
	else if (timer_cross.show_ms() < ((13 * 500) + (12 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((13 * 500) + (13 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = -0.7;
		speed.vx = 0.7;
	}
	else if (timer_cross.show_ms() < ((14 * 500) + (13 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((14 * 500) + (14 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = 0.7;
		speed.vx = -0.7;
	}
	else if (timer_cross.show_ms() < ((15 * 500) + (14 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((15 * 500) + (15 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = 0.7;
		speed.vx = -0.7;
	}
	else if (timer_cross.show_ms() < ((16 * 500) + (15 * 1000 * cross_size))) {
		speed.zero();
	}
	else if (timer_cross.show_ms() < ((16 * 500) + (16 * 1000 * cross_size))) {
		speed.zero();
		speed.vy = -0.7;
		speed.vx = 0.7;
	}
	else {
		timer_cross.clear();
		cross_size += 0.5;
	}
}

int main(int argc, char *argv[])
{
    ARDrone ardrone;
	findComponentsHandle main;
	coordinates position;
	bool calibrated = false;
	int flag_move = 0;
	int flag_position = 0; // для condition_move с координатами
	int condition = 0;
	time1 timer, timer_cross, timer_lap, timer_center, timer_angle, timer_start;
	int dx, dy;
	int dcx, dcy; // разница между компонентами
	speeds speed;
	Point last_comp1 = { -1, -1 }, last_comp2 = { -1, -1 };
	bool flag_last_comp1 = false, flag_last_comp2 = false;
	double cross_size = 0.5;
	int laps = -1;
	int last_lap_time = 0;

	const int flight_time_ms = 2000;
	const int big_flight_time_ms = 2200;
	const int small_flight_time_ms = 1800;
	const int xsmall_flight_time_ms = 1600;

	const int flight_waiting_time_ms = 1500;
	const int big_flight_waiting_time_ms = 2000;

	const int aligning_time_ms = 1000;
	int aligning_waiting_time_ms = 1500;

	namedWindow("img");

    if (!ardrone.open()) {
        std::cout << "Failed to initialize." << std::endl;
        return -1;
    }

	show_info_start(ardrone);

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		// Получаем картинку
		main.img = ardrone.getImage();
		main.resize_images();
		ardrone.update_velocity(position.speed.x, position.speed.y);
		position.update();
		speed.zero();

		// Обнуляем координаты
		if (KEY_PUSH('0')) {
			position.zero();
		}

		// Подъем до 1.5 метров
		if (KEY_DOWN('H')) {
			if (ardrone.getAltitude() < 1.5) speed.vz = 1.0;
		}

		// Подъем и приземление
		if (KEY_PUSH(VK_SPACE)) {
			if (ardrone.onGround()) ardrone.takeoff();
			else                    ardrone.landing();
		}

		// Удалить откалиброванные значения
		if (KEY_PUSH('X')) {
			main.delete_calibrated_values();
			calibrated = false;
		}

		// Сохранить откалиброванные значения
		if (KEY_PUSH('V')) {
			main.save_calibrated_values();
		}

		// Загрузить из файла откалиброванные значения
		if (KEY_PUSH('F')) {
			main.upload_calibrated_values();
			calibrated = true;
		}

		// Калибровка
		if (KEY_PUSH('C')) {
			cout << "Calibration started" << endl;
			setMouseCallback("img", on_mouse, &main);

			while (!KEY_PUSH('C')) {
				cv::imshow("img", main.img);
				cv::waitKey(10);
				if (KEY_DOWN(VK_ESCAPE)) break;
			}

			if (main.is_calibrate_finished()) {
				cout << "Calibration finished" << endl;
				calibrated = true;
			}
			else {
				cout << "Calibration not finished" << endl;
			}
			setMouseCallback("img", NULL, NULL);
		}

		// Поиск компоненты
		if (calibrated) {
			main.findComponents();
			cv::imshow("color", main.colored);
			if (main.on_image(main.comp1)) {
				last_comp1 = main.comp1;
			}
			if (main.on_image(main.comp2)) {
				last_comp2 = main.comp2;
			}
		}

		// Движение по клавиатуре
		movement_control(speed);

		// Смена камеры
		static int mode = 0;
		if (KEY_PUSH('Z')) {
			ardrone.setCamera(++mode % 4);
		}

		// Включение движения вокруг колонн
		if (KEY_PUSH('1')) {
			if (condition == 0) condition = 1;
			else condition = 0;
			timer.clear();
			timer_lap.clear();
			position.zero();
			laps = 0;
		}

		if (KEY_PUSH('8')) {
			if (condition == 0) condition = -1;
			else condition = 0;
			timer.clear();
		}

		if (condition == -1)
		{
			if (ardrone.onGround()) ardrone.takeoff();
			else if (ardrone.getAltitude() < 1.5)
			{
				speed.zero();
				speed.vz = 0.5;
				timer_start.clear();
			}
			else if (timer_start.show_ms() < 500) {
				speed.zero();
			}
			else if (timer_start.show_ms() < 1900) {
				speed.zero();
				speed.vy = -1.0;
			}
			else if (timer_start.show_ms() < 3900) {
				speed.zero();
			}
			else {
				condition = 10;
				timer.clear();
			}
		}

		// Вокруг колонн
		if (condition == 1) {
			condition_move(position, speed, timer, condition, 1.0, 0.0, 2000, big_flight_waiting_time_ms);
		}
		else if (condition == 2) {
			condition_move(position, speed, timer, condition, 0.0, -1.0, 2200, big_flight_waiting_time_ms);
			flag_last_comp1 = false;
			flag_last_comp2 = false;
		}
		else if (condition == 3) {
			condition_move(position, speed, timer, condition, -1.0, 0.0, 1800, big_flight_waiting_time_ms);
		}
		else if (condition == 4) {
			if (main.on_image(main.comp1)) {
				flag_last_comp1 = true;
			}
			if (main.on_image(main.comp2)) {
				flag_last_comp2 = true;
			}
			condition_move(position, speed, timer, condition, -0.8 , 0.8, 3000, big_flight_waiting_time_ms);
			timer_cross.clear();
			cross_size = 0.5;
		}
		if (condition == 5) condition = 8;
		
		else if ((condition == 5) || (condition == 12)) {
			if (main.on_image(main.comp1)) {
				flag_last_comp1 = true;
			}
			else if (main.on_image(main.comp2)) {
				flag_last_comp2 = true;
			}
			if (!flag_last_comp1 && !flag_last_comp2) { 
				// ИСПОЛЬЗОВАНИЕ КРЕСТА В СЛУЧАЕ, ЕСЛИ ПОТЕРЯЛИСЬ СОВСЕМ
				cout << "SEARCH BY CROSS" << endl;
				find_center_cross(speed, position, timer_cross, cross_size);
			}
			if (flag_last_comp1 || flag_last_comp2) {
				condition += 1;
			}
		}
		else if ((condition == 6) || (condition == 13)) { 
			std::cout << "TO_CENTER" << endl;
			if (flag_move == 0) {
				if (main.on_image(main.comp1) && main.on_image(main.comp2)) {
					dx = (main.img.cols / 2) - ((main.comp1.x + main.comp2.x) / 2);
					dy = (main.img.rows / 2) - ((main.comp1.y + main.comp2.y) / 2);
					flag_last_comp1 = true;
					flag_last_comp2 = true;
				}
				else if (main.on_image(main.comp1)) {
					dx = (main.img.cols / 2) - main.comp1.x;
					dy = (main.img.rows / 2) - main.comp1.y;
					flag_last_comp1 = true;
				}
				else if (main.on_image(main.comp2)) {
					dx = (main.img.cols / 2) - main.comp2.x;
					dy = (main.img.rows / 2) - main.comp2.y;
					flag_last_comp2 = true;
				}
				else if (flag_last_comp1 && flag_last_comp2) {
					dx = (main.img.cols / 2) - ((last_comp1.x + last_comp2.x) / 2);
					dy = (main.img.rows / 2) - ((last_comp1.y + last_comp2.y) / 2);
				}
				else if (flag_last_comp1) {
					dx = (main.img.cols / 2) - last_comp1.x;
					dy = (main.img.rows / 2) - last_comp1.y;
				}
				else if (flag_last_comp2) {
					dx = (main.img.cols / 2) - last_comp2.x;
					dy = (main.img.rows / 2) - last_comp2.y;
				}
				else {
					condition -= 1;
					timer_cross.clear();
					cross_size = 0.5;
				}
				flag_last_comp1 = false;
				flag_last_comp2 = false;
				timer_center.clear();
				if (sqrt(dx*dx + dy*dy) < 70) {
					flag_move = 2;		
				}
				else {
					flag_move = 1;
				}
			}
			else if (flag_move == 1) {
				cout << "ALIGNING BY TIME" << endl;
				// ПОДОГНАТЬ КОЭФФИЦИЕНТ
				const double k_time = 7.0;
				if (timer_center.show_ms() < (sqrt(dx*dx + dy*dy) * k_time)) {
					speed.zero();
					speed.vy = dx / sqrt(dx*dx + dy*dy);
					speed.vx = dy / sqrt(dx*dx + dy*dy);
					if (abs(speed.vx) < 0.3) speed.vx = 0.0;
					if (abs(speed.vy) < 0.3) speed.vy = 0.0;
					aligning_waiting_time_ms = std::max(sqrt(dx*dx + dy*dy) * k_time * 1.5, 500.0);
				}
				else if (timer_center.show_ms() < ((sqrt(dx*dx + dy*dy) * k_time) + aligning_waiting_time_ms)) {
					speed.zero();
				}
				else {
					flag_move = 0;
					if (main.on_image(main.comp1) && main.on_image(main.comp2)) {
						Point comp_center;
						comp_center.x = (main.comp1.x + main.comp2.x) / 2;
						comp_center.y = (main.comp1.y + main.comp2.y) / 2;
						if (main.in_center(comp_center)) {
							condition += 1;
							speed.zero();
							flag_last_comp1 = false;
							flag_last_comp2 = false;
						}
					}	
				}
			}
			else if (flag_move == 2) {
				cout << "ALIGNING BY PREG" << endl;
				if (main.on_image(main.comp1) && main.on_image(main.comp2)) {
					dx = (main.img.cols / 2) - ((main.comp1.x + main.comp2.x) / 2);
					dy = (main.img.rows / 2) - ((main.comp1.y + main.comp2.y) / 2);
					flag_last_comp1 = true;
					flag_last_comp2 = true;
				}
				else if (main.on_image(main.comp1)) {
					dx = (main.img.cols / 2) - main.comp1.x;
					dy = (main.img.rows / 2) - main.comp1.y;
					flag_last_comp1 = true;
				}
				else if (main.on_image(main.comp2)) {
					dx = (main.img.cols / 2) - main.comp2.x;
					dy = (main.img.rows / 2) - main.comp2.y;
					flag_last_comp2 = true;
				}
				else if (flag_last_comp1 && flag_last_comp2) {
					dx = (main.img.cols / 2) - ((last_comp1.x + last_comp2.x) / 2);
					dy = (main.img.rows / 2) - ((last_comp1.y + last_comp2.y) / 2);
				}
				else if (flag_last_comp1) {
					dx = (main.img.cols / 2) - last_comp1.x;
					dy = (main.img.rows / 2) - last_comp1.y;
				}
				else if (flag_last_comp2) {
					dx = (main.img.cols / 2) - last_comp2.x;
					dy = (main.img.rows / 2) - last_comp2.y;
				}
				else {
					condition -= 1;
					timer_cross.clear();
					cross_size = 0.5;
				}
				if (abs(dx) < 10) dx = 0;
				if (abs(dy) < 10) dy = 0;
				if (timer_center.show_ms() < aligning_time_ms) {
					speed.zero();
					speed.vy = ((double(dx) / 50) / 3);
					speed.vx = ((double(dy) / 50) / 3);
				}
				else if (timer_center.show_ms() < aligning_time_ms + aligning_waiting_time_ms) {
					speed.zero();
				}
				else {
					flag_move = 0;
					if (main.on_image(main.comp1) && main.on_image(main.comp2)) {
						Point comp_center;
						comp_center.x = (main.comp1.x + main.comp2.x) / 2;
						comp_center.y = (main.comp1.y + main.comp2.y) / 2;
						if (main.in_center(comp_center)) {
							condition += 1;
							speed.zero();
							flag_last_comp1 = false;
							flag_last_comp2 = false;
						}
					}
				}
			}
		}
		else if ((condition == 7) || (condition == 14)) {
			std::cout << "TO_ANGLE" << endl;
			if (main.on_image(main.comp1)) {
				flag_last_comp1 = true;
			}
			if (main.on_image(main.comp2)) {
				flag_last_comp2 = true;
			}
			if (main.in_center(main.comp1) && main.on_image(main.comp2)) {
				if (!main.in_angle(2)) {
					dcx = main.comp2.x - main.comp1.x;
					dcy = main.comp2.y - main.comp1.y;
					cout << "DCX: " << dcx << endl;
					if (abs(dcx) < 1)
					{
						cout << "SPEED ZERO" << endl;
						speed.zero();
					}
					else {
						if (dcx > 0) {
							cout << "SPEED PLUS" << endl;
							speed.zero();
							speed.vr = 0.3;
						}
						else {
							cout << "SPEED MINUS" << endl;
							speed.zero();
							speed.vr = -0.3;
						}
					}
					timer_angle.clear();
				}
				else if (timer_angle.show_ms() < 300) {
					cout << "WAIT" << endl;
					speed.zero();
				}
				else {
					speed.zero();
					position.zero();
					timer.clear();
					condition += 1;
				}
			}
			else { 
				speed.zero();
				condition -= 1;
			}
		}
		
		else if (condition == 8) {
			condition_move(position, speed, timer, condition, -1.0, 0.0, 2500, big_flight_waiting_time_ms);
		}
		else if (condition == 9) {
			condition_move(position, speed, timer, condition, 0.0, -1.0, 2500, big_flight_waiting_time_ms);
			flag_last_comp1 = false;
			flag_last_comp2 = false;
		}
		else if (condition == 10) {
			condition_move(position, speed, timer, condition, 1.0, 0.0, 2000, big_flight_waiting_time_ms);
		}
		else if (condition == 11) {
			if (main.on_image(main.comp1)) {
				flag_last_comp1 = true;
			}
			if (main.on_image(main.comp2)) {
				flag_last_comp2 = true;
			}
			condition_move(position, speed, timer, condition, 0.8, 0.8, 2500, big_flight_waiting_time_ms);
			timer_cross.clear();
			cross_size = 0.5;
		}
		if (condition == 12) condition = 1;
		else if (condition == 15) {
			condition = 1;
			last_lap_time = timer_lap.show_s();
			timer_lap.clear();
			laps += 1;
		}
	
		/*
		if ((condition == 6) || (condition == 13))
		{
			if (ardrone.getAltitude() < 1.5) {
				speed.vz = 0.5;
			}
			if (ardrone.getAltitude() > 1.7) {
				speed.vz = -0.5;
			}
		}
		*/

		//system("cls");
		std::cout.precision(3);
		std::cout << std::fixed;
		std::cout << "On image:  " << main.on_image(main.comp1) << " " << main.on_image(main.comp2) << endl;
		std::cout << "In center: " << main.in_center(main.comp1) << " " << main.in_center(main.comp2) << endl;
		std::cout << "Flag last: " << flag_last_comp1 << " " << flag_last_comp2 << endl;
		std::cout << "Battery:   " << ardrone.getBatteryPercentage() << endl;
		std::cout << "Altitude:  " << ardrone.getAltitude() << endl;
		std::cout << "Condition: " << condition << endl;
		std::cout << "Speed:     " << position.speed.x << " " << position.speed.y << endl;
		std::cout << "Timer:     " << timer.show_ms() << endl;
		std::cout << "Last lap:  " << last_lap_time << endl;
		std::cout << "Laps:      " << laps << endl << endl;
		
		ardrone.move3D(speed.vx, speed.vy, speed.vz, speed.vr);
		cv::imshow("img", main.img);
		cv::waitKey(33);
	}
    ardrone.close();
    return 0;
}