#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include "windows.h"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

class findColorsHandle
{
protected:
	bool check_pixel(int x, int y, Point3d& B, Point3i paint_color, double angle_beta = 12);

private:
	const double down_scale = 0.5;
	const double up_scale = 2.0;

public:
	Point3d B1, B2;
	Point c1, c2;
	Mat img, colored;
	void calibrate_color(int x, int y, Point3d& B);
	void findColors();
	void delete_calibrated_values();
	void save_calibrated_values();
	void upload_calibrated_values();
	void resize_images();
	bool is_calibrate_finished();

	findColorsHandle() {
		delete_calibrated_values();
	}
};