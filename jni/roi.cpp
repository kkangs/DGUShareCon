/*
 * roi.cpp
 *
 *  Created on: 2015. 6. 5.
 *      Author: gihyun
 */

#include "myImage.hpp"
#include "roi.hpp"

#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

/* Region Of Interest */

MY_ROI::MY_ROI() {
	upper_corner = Point(0, 0);
	lower_corner = Point(0, 0);
}

MY_ROI::MY_ROI(Point u_corner, Point l_corner, Mat src) {
	upper_corner = u_corner;
	lower_corner = l_corner;
	color = Scalar(0, 255, 0);
	border_thickness = 2;
	roi_ptr = src(Rect(u_corner.x, u_corner.y, l_corner.x-u_corner.x, l_corner.y-u_corner.y));

}

void MY_ROI::draw_rectangle(Mat src) {
	rectangle(src, upper_corner, lower_corner, color, border_thickness);
}

