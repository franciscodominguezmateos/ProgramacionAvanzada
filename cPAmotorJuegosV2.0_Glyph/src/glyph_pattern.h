/*
 * glyph_pattern.h
 *
 *  Created on: 8 Mar 2022
 *      Author: Francisco Dominguez
 */

#pragma once
#include "opencv2/opencv.hpp"

#include "glyph_contour.h"

using namespace std;
using namespace cv;

//Just Square patterns as in:
class Pattern{
	string sPatern;
	// size of the square face in metres
	double size_face;
	// Image Corner Points
	Contour imageCornerPoints;
	// World Corner Points
	vector<Point3d> worldCornerPoints;
	// Solved Pose
	Mat rvec,tvec;
public:
	Pattern(string s, double sf=0.12):sPatern(s),size_face(sf){
		double h=size_face/2.0;
		worldCornerPoints.push_back(Point3d(-h, h,0));
		worldCornerPoints.push_back(Point3d(-h,-h,0));
		worldCornerPoints.push_back(Point3d( h,-h,0));
		worldCornerPoints.push_back(Point3d( h, h,0));
	}
	string asString(){return sPatern;}
	vector<Point3d>& getWorldCornerPoints(){return worldCornerPoints;}
	Contour&         getImageCornerPoints(){return imageCornerPoints;}
	Mat& getRvec(){return rvec;}
	Mat& getTvec(){return tvec;}
	void setImageCornerPoints(Contour c){imageCornerPoints=c;}
	void solvePose(Mat& K,Mat& dist){
		solvePnP(worldCornerPoints,imageCornerPoints,K,dist,rvec,tvec);
	}
	/*Mat asImg(){

	}*/
};

