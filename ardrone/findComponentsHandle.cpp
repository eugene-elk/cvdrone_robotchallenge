#include "findColorsHandle.h"
#include "findComponentsHandle.h"

bool findComponentsHandle::border(int x, int y)
{
	bool border = false;
	if ((y < 0) || (x < 0) || (x >= img.cols) || (y >= img.rows)) {
		border = true;
	}
	return border;
}

void findComponentsHandle::add_point(Point point, queue<Point>& queue, Point3d& calibrated_color, Point3i paint_color)
{
	if (!border(point.x, point.y) && !used[point.x][point.y]) {
		if (check_pixel(point.x, point.y, calibrated_color, paint_color)) {
			used[point.x][point.y] = true;
			queue.push(point);
		}
	}
}

component findComponentsHandle::BFS(Point start_point, Point3d& calibrated_color, Point3i paint_color)
{
	queue<Point> queue;
	queue.push(start_point);
	component new_component;

	new_component.color = calibrated_color;
	new_component.x_max = start_point.x;
	new_component.x_min = start_point.x;
	new_component.y_max = start_point.y;
	new_component.y_min = start_point.y;

	while (!queue.empty()) {
		Point last_point = queue.front();
		queue.pop();

		new_component.points.push_back(last_point);
		if (last_point.x > new_component.x_max) new_component.x_max = last_point.x;
		if (last_point.x < new_component.x_min) new_component.x_min = last_point.x;
		if (last_point.y > new_component.y_max) new_component.y_max = last_point.y;
		if (last_point.y < new_component.y_min) new_component.y_min = last_point.y;

		add_point(Point(last_point.x + 1, last_point.y), queue, calibrated_color, paint_color);
		add_point(Point(last_point.x - 1, last_point.y), queue, calibrated_color, paint_color);
		add_point(Point(last_point.x, last_point.y + 1), queue, calibrated_color, paint_color);
		add_point(Point(last_point.x, last_point.y - 1), queue, calibrated_color, paint_color);
	}

	return new_component;
}

void findComponentsHandle::prepare_used()
{
	components.clear();
	used.resize(img.cols);
	for (int i = 0; i < img.cols; i++) {
		used[i].resize(img.rows);
	}
	for (int i = 0; i < img.cols; i++) {
		for (int j = 0; j < img.rows; j++) {
			used[i][j] = false;
		}
	}
}

void findComponentsHandle::findComponents()
{
	prepare_used();
	for (int i = 0; i < img.cols; i++) {
		for (int j = 0; j < img.rows; j++) {
			if (!used[i][j]) {
				if (check_pixel(i, j, B1, Point3i(0, 255, 0))) {
					components.push_back(BFS(Point(i, j), B1, Point3i(0, 255, 0)));
				}
				if (check_pixel(i, j, B2, Point3i(255, 0, 0))) {
					components.push_back(BFS(Point(i, j), B2, Point3i(255, 0, 0)));
				}
			}
			used[i][j] = true;
		}
	}
	comp1 = find_center_of_biggest_component(B1);
	comp2 = find_center_of_biggest_component(B2);
	rectangle(img, Point((img.cols / 2) - center_sizeX, (img.rows / 2) - center_sizeY), 
		Point((img.cols / 2) + center_sizeX, (img.rows / 2) + center_sizeY), Scalar(255, 255, 255));
}

void findComponentsHandle::show_components_info()
{
	for (int i = 0; i < components.size(); i++) {
		cout << "component ¹" << i + 1 << endl;
		cout << components[i].color.z << " " << components[i].color.y << " " << components[i].color.x << endl;
		cout << components[i].size() << endl << endl;
	}
}

Point findComponentsHandle::find_center_of_biggest_component(Point3d& calibrated_color)
{
	Point result = { -1, -1 };
	int max_size = 0;
	int number_of_max_size = -1;
	for (int i = 0; i < components.size(); i++) {
		if (components[i].color == calibrated_color) {
			if (components[i].size() > max_size) {
				max_size = components[i].size();
				number_of_max_size = i;
			}
		}
	}
	if (number_of_max_size != -1) {
		if (max_size > 50) {
			result = components[number_of_max_size].center();
		}
		circle(img, result, 5, Scalar(255, 255, 255));
	}
	return result;
}

void findComponentsHandle::result_rounding()
{
	comp1.x = (comp1.x * 30) / 30;
	comp1.y = (comp1.y * 25) / 25;
	comp2.x = (comp2.x * 30) / 30;
	comp2.y = (comp2.y * 25) / 25;
}

bool findComponentsHandle::in_center_X(Point& comp)
{
	int dx = comp.x - (img.cols / 2);
	return (abs(dx) < center_sizeX);
}

bool findComponentsHandle::in_center_Y(Point& comp)
{
	int dy = comp.y - (img.rows / 2);
	return (abs(dy) < center_sizeY);
}

bool findComponentsHandle::in_center(Point& comp)
{
	return (in_center_X(comp) && in_center_Y(comp));
}

bool findComponentsHandle::on_image(Point& comp)
{
	return ((comp.x > 0) && (comp.y > 0));
}

bool findComponentsHandle::in_angle(int angle)
{
	int dcx = comp2.x - comp1.x;
	int dcy = comp2.y - comp1.y;
	return ((abs(dcx) < angle) && (dcy > 0));
}
