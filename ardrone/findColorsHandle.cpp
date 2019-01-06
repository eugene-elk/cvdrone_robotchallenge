#include "findColorsHandle.h"
#include <string>
#include <fstream>
void findColorsHandle::calibrate_color(int x, int y, Point3d& B)
{
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			B.z += img.at<Vec3b>(j, i)[0] / 9.0;
			B.y += img.at<Vec3b>(j, i)[1] / 9.0;
			B.x += img.at<Vec3b>(j, i)[2] / 9.0;
		}
	}
	cout << "Calibrated color: " << B.x << " " << B.y << " " << B.z << endl;
}

bool findColorsHandle::check_pixel(int x, int y, Point3d& B, Point3i paint_color, double angle_beta)
{
	Point3d A, C;
	C.z = img.at<Vec3b>(y, x)[0];
	C.y = img.at<Vec3b>(y, x)[1];
	C.x = img.at<Vec3b>(y, x)[2];

	double BC = sqrt((B.x - C.x)*(B.x - C.x) + (B.y - C.y)*(B.y - C.y) + (B.z - C.z)*(B.z - C.z));
	double AB = sqrt((B.x - A.x)*(B.x - A.x) + (B.y - A.y)*(B.y - A.y) + (B.z - A.z)*(B.z - A.z));
	double AC = sqrt((A.x - C.x)*(A.x - C.x) + (A.y - C.y)*(A.y - C.y) + (A.z - C.z)*(A.z - C.z));

	double angle_alpha = acos((AB * AB + AC * AC - BC * BC) / (2 * AB * AC));
	angle_alpha = angle_alpha / CV_PI * 180.0;

	if (abs(angle_alpha) < angle_beta) {
		//cout << "coloring pixel start: " << x << " " << y << endl;
		colored.at<Vec3b>(y, x)[0] = (uchar)paint_color.z;
		colored.at<Vec3b>(y, x)[1] = (uchar)paint_color.y;
		colored.at<Vec3b>(y, x)[2] = (uchar)paint_color.x;
		return true;
	}
	else {
		return false;
	}
}

void findColorsHandle::findColors()
{
	for (int i = 0; i < img.cols; i++) {
		for (int j = 0; j < img.rows; j++) {
			check_pixel(i, j, B1, Point3i(0, 255, 0));
			check_pixel(i, j, B2, Point3i(255, 0, 0));
		}
	}
}

bool findColorsHandle::is_calibrate_finished()
{
	return ((c1.x != -1) && (c1.y != -1) && (c2.x != -1) && (c2.y != -1));
}

void findColorsHandle::delete_calibrated_values()
{
	c1 = { -1, -1 };
	c2 = { -1, -1 };
	cout << "Values deleted" << endl;
}

void findColorsHandle::resize_images()
{
	resize(img, img, cv::Size(), down_scale, down_scale);
	medianBlur(img, img, 3);
	resize(img, colored, cv::Size(), 1, 1);
}

void findColorsHandle::save_calibrated_values()
{
	ofstream fout("saved_colors.txt");
	fout << B1.x << " " << B1.y << " " << B1.z << endl;
	fout << B2.x << " " << B2.y << " " << B2.z << endl;
	fout.close();
	cout << "Values saved" << endl;
}

void findColorsHandle::upload_calibrated_values()
{
	ifstream fin("saved_colors.txt");
	fin >> B1.x >> B1.y >> B1.z;
	fin >> B2.x >> B2.y >> B2.z;
	fin.close();
	cout << "Values uploaded" << endl;
}

