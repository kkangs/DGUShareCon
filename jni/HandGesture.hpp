/*
 * HandGesture.hpp
 *
 *  Created on: 2015. 6. 5.
 *      Author: gihyun
 */

#ifndef HANDGESTURE_HPP_
#define HANDGESTURE_HPP_

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <vector>
#include <string>

#include "myImage.hpp"
#include "main.hpp"

using namespace cv;
using namespace std;

class HandGesture {
public:
	MyImage m;
	HandGesture();
	vector<vector<Point> > contours;
	vector<vector<int> > hullI;
	vector<vector<Point> > hullP;
	vector<vector<Vec4i> > defects;
	vector<Point> fingerTips;
	Rect bRect;
	bool isHand;
	int frameNumber;
	int cIdx;
	int nrOfDefects;
	double bRect_width;
	double bRect_height;
	bool detectIfHand();
	void initVectors();
	void eleminateDefects(MyImage *m);
	void printGesturInfo(Mat src);
	void getFingerTips(MyImage *m);
	void drawFingerTips(MyImage *m);
private:
	int fontFace;
	float distanceP2P(Point a, Point b);
	float getAngle(Point s, Point f, Point e);
	void removeRedundantEndPoints(vector<Vec4i> newDefects, MyImage *m);
	void analyzeContours();
	void checkForOneFinger(MyImage *m);
	string intToString(int number);
	string bool2string(bool tf);
};


#endif /* HANDGESTURE_HPP_ */
