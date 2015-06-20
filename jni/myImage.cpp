/*

 * myImage.cpp
 *
 *  Created on: 2015. 6. 5.
 *      Author: gihyun

*/

#include "myImage.hpp"

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace cv;

MyImage::MyImage() {

}

MyImage::MyImage(int webCamera) {
	cameraSrc = webCamera;
	//cap = VideoCapture(webCamera);
	cap = VideoCapture(webCamera);

}
