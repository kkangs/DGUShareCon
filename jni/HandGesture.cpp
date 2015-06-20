/*
 * HandGesture.cpp
 *
 *  Created on: 2015. 6. 5.
 *      Author: gihyun
*/

#include "HandGesture.hpp"
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <cmath>

using namespace cv;
using namespace std;


HandGesture::HandGesture() {
	frameNumber = 0;
	fontFace = FONT_HERSHEY_PLAIN;
}

void HandGesture::initVectors() {
	hullI = vector<vector<int> >(contours.size());
	hullP = vector<vector<Point> >(contours.size());
	defects = vector<vector<Vec4i> >(contours.size());
}

float HandGesture::distanceP2P(Point a, Point b) {
	float d = sqrt((float)(fabs((float)(pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0)))));

	return d;
}

float HandGesture::getAngle(Point s, Point f, Point e) {
	float l1 = distanceP2P(f, s);
	float l2 = distanceP2P(f, e);
	float dot = (s.x - f.x)*(e.x - f.x) + (s.y - f.y)*(e.y - f.y);
	float angle = acos(dot / (l1*l2));

	angle = angle * 180 / PI;

	return angle;
}

void HandGesture::removeRedundantEndPoints(vector<Vec4i> newDefects, MyImage *m) {
	Vec4i temp;
	float avgX, avgY;
	double tolerance = bRect_width / 6;
	int startidx, endidx, faridx;
	int startidx2, endidx2;

	for (int i = 0; i < newDefects.size(); i++) {
		for (int j = i; j < newDefects.size(); j++) {
			startidx = newDefects[i][0];
			Point ptStart(contours[cIdx][startidx]);
			endidx = newDefects[i][1];
			Point ptEnd(contours[cIdx][endidx]);

			startidx2 = newDefects[j][0];
			Point ptStart2(contours[cIdx][startidx2]);
			endidx2 = newDefects[j][1];
			Point ptEnd2(contours[cIdx][endidx2]);

			if (distanceP2P(ptStart, ptEnd2) < tolerance){
				contours[cIdx][startidx] = ptEnd2;
				break;
			}
			if (distanceP2P(ptEnd, ptStart2) < tolerance){
				contours[cIdx][startidx2] = ptEnd;
			}
		}
	}
}

void HandGesture::eleminateDefects(MyImage *m) {
	int tolerance = bRect_height / 5;
	float angleTol = 95;
	int startidx, endidx, faridx;

	vector<Vec4i> newDefects;
	vector<Vec4i>::iterator d = defects[cIdx].begin();

	while (d != defects[cIdx].end()) {
		Vec4i &v = (*d);

		startidx = v[0];
		Point ptStart(contours[cIdx][startidx]);
		endidx = v[1];
		Point ptEnd(contours[cIdx][endidx]);
		faridx = v[2];
		Point ptFar(contours[cIdx][faridx]);

		if (distanceP2P(ptStart, ptFar) > tolerance && distanceP2P(ptEnd, ptFar) > tolerance && getAngle(ptStart, ptFar, ptEnd)) {
			if (ptEnd.y > (bRect.y + bRect.height - bRect.height / 4)){

			}
			else if (ptStart.y > (bRect.y + bRect.height - bRect.height / 4)){

			}
			else {
				newDefects.push_back(v);
			}
		}

		d++;
	}

	nrOfDefects = newDefects.size();
	defects[cIdx].swap(newDefects);
	removeRedundantEndPoints(defects[cIdx], m);
}

void HandGesture::analyzeContours() {
	bRect_height = bRect.height;
	bRect_width = bRect.width;
}

bool HandGesture::detectIfHand() {
	analyzeContours();

	double h = bRect_height;
	double w = bRect_width;
	isHand = true;

	if (fingerTips.size() > 5) {
		isHand = false;
	}
	else if (h == 0 || w == 0) {
		isHand = false;
	}
	else if (h / 2 > 4 || w / h > 4) {
		isHand = false;
	}
	else if (bRect.x < 20) {
		isHand = false;
	}

	return isHand;
}

string HandGesture::intToString(int number) {
	stringstream ss;
	ss << number;
	string str = ss.str();
	return str;
}

string HandGesture::bool2string(bool tf) {
	if (tf) {
		return "true";
	}
	else {
		return "false";
	}
}

void HandGesture::printGesturInfo(Mat src) {
	int fontFace = FONT_HERSHEY_PLAIN;
	Scalar fColor(245, 200, 200);
	int xpos = src.cols / 1.5;
	int ypos = src.rows / 1.6;
	float fontSize = 0.7f;
	int lineChange = 14;
	string info = "Figure info: ";

	putText(src, info, Point(ypos, xpos), fontFace, fontSize, fColor);
	xpos += lineChange;
	info = string("Number of defects: ") + string(intToString(nrOfDefects));
	putText(src, info, Point(ypos, xpos), fontFace, fontSize, fColor);
	xpos += lineChange;
	info = string("bounding box height, width: ") + string(intToString(bRect_height)) + string(" , ") + string(intToString(bRect_width));
	putText(src, info, Point(ypos, xpos), fontFace, fontSize, fColor);
	xpos += lineChange;
	info = string("Is Hand: ") + string(bool2string(isHand));
	putText(src, info, Point(ypos, xpos), fontFace, fontSize, fColor);
}

void HandGesture::checkForOneFinger(MyImage *m) {
	int yTol = bRect.height / 6;
	Point highestP;
	highestP.y = m->src.rows;
	vector<Point>::iterator d = contours[cIdx].begin();

	while (d != contours[cIdx].end()) {
		Point v = (*d);

		if (v.y < highestP.y) {
			highestP = v;
			cout << highestP.y << endl;
		}

		d++;
	}

	int n = 0;
	d = hullP[cIdx].begin();

	while (d != hullP[cIdx].end()) {
		Point v = (*d);
		cout << "x " << v.x << " y " << v.y << " highstpY " << highestP.y << "ytol " << yTol << endl;

		if (v.y < highestP.y + yTol && v.y != highestP.y && v.x != highestP.x) {
			n++;
		}

		d++;

		if (n == 0) {
			fingerTips.push_back(highestP);
		}
	}
}

void HandGesture::getFingerTips(MyImage *m) {
	fingerTips.clear();
	int i = 0;
	vector<Vec4i>::iterator d = defects[cIdx].begin();

	while (d != defects[cIdx].end()) {
		Vec4i &v = (*d);

		int startidx = v[0];
		Point ptStart(contours[cIdx][startidx]);
		int endidx = v[1];
		Point ptEnd(contours[cIdx][endidx]);
		int faridx = v[2];
		Point ptFar(contours[cIdx][faridx]);

		if (i == 0) {
			fingerTips.push_back(ptStart);
			i++;
		}

		fingerTips.push_back(ptEnd);
		d++;
		i++;
	}

	if (fingerTips.size() == 0) {

	}
}

void HandGesture::drawFingerTips(MyImage *m) {
	Point p;
	int k = 0;

	for (int i = 0; i < fingerTips.size(); i++) {
		p = fingerTips[i];
		putText(m->src, intToString(i), p - Point(0, 30), fontFace, 1.2f, Scalar(200, 200, 200), 2);
		circle(m->src, p, 5, Scalar(100, 255, 100), 4);
	}
}
