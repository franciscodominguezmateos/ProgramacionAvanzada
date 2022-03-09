/*
 * glyph_pattern.h
 *
 *  Created on: 8 Mar 2022
 *      Author: Francisco Dominguez
 */

#pragma once
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//Just Square patterns as in:
class Pattern{
	string sPatern;
	// size of the square face in metres
	double size_face;
public:
	Pattern(string s):sPatern(s),size_face(0.1){}
	string asString(){
		return sPatern;
	}
	/*Mat asImg(){

	}*/
	Mat getWorldCornerPoints(){
		Mat points=(Mat_<double>(4,3)<< -1,1,0, 1,1,0, -1,-1,0, -1,-1,0);
		return points*size_face/2.0;
	}
};

