/*
 * roi.hpp
 *
 *  Created on: 2015. 6. 5.
 *      Author: gihyun
 */

#ifndef ROI_HPP_
#define ROI_HPP_

#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;

/* Region Of Interest */
class MY_ROI {
public:
	MY_ROI();
	MY_ROI(Point upper_corner, Point lower_corner, Mat src);
	Point upper_corner, lower_corner;
	Mat roi_ptr;
	Scalar color;

	int border_thickness;
	void draw_rectangle(Mat src);
};

#endif /* ROI_HPP_ */
