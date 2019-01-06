#pragma once
#include "findColorsHandle.h"
struct component
{
	vector<Point> points;
	Point3d color;
	int size() {
		return (int)points.size();
	}
	int x_min, x_max, y_min, y_max;
	Point center() {
		Point result;
		result.x = (x_min + x_max) / 2;
		result.y = (y_min + y_max) / 2;
		return result;
	}
};

class findComponentsHandle : public findColorsHandle
{
private:
	const int center_sizeX = 50;
	const int center_sizeY = 50;
	vector<component> components;
	vector<vector<bool>> used;
	void prepare_used();
	void add_point(Point point, queue<Point>& queue, Point3d& calibrated_color, Point3i paint_color);
	bool border(int x, int y);
	component BFS(Point start_point, Point3d& calibrated_color, Point3i paint_color);
	Point find_center_of_biggest_component(Point3d& calibrated_color);

public:
	Point comp1, comp2;
	findComponentsHandle() {
		findColorsHandle();
		comp1 = Point(-1, -1);
		comp2 = Point(-1, -1);
	}
	void findComponents();
	void show_components_info();
	void result_rounding();
	bool in_center(Point& comp);
	bool on_image(Point& comp);
	bool in_center_X(Point& comp);
	bool in_center_Y(Point& comp);
	bool in_angle(int angle);
}; 