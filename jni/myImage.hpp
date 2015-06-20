/*
 * myImage.hpp
 *
 *  Created on: 2015. 6. 5.
 *      Author: gihyun
 */

#ifndef MYIMAGE_HPP_
#define MYIMAGE_HPP_

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

class MyImage {
public:
	MyImage(int webCamera);
	MyImage();

	/* 여기서 Mat이란 이미지를 행렬로 처리하는 클래스 */
	Mat srcLR;
	Mat src;
	Mat bw;

	vector<Mat> bwList;
	VideoCapture cap;

	int cameraSrc;
	void initWebCamera(int i);
};

#endif /* MYIMAGE_HPP_ */
