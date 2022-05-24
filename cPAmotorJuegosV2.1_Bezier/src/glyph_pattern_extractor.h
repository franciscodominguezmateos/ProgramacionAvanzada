/*
 * glyph_pattern_detector.h
 *
 *  Created on: 6 Mar 2022
 *      Author: Francisco Dominguez
 */

#pragma once
#include "glyph_pattern.h"

class PatternExtractor{
public:
	virtual Pattern getPattern(Mat& img)=0;
};
/**
 * Extract the pattern of a 5x5 black with white background square cells
 * Que square of the border are all black then there is a 3x3 of valid squares
 * The square image is a 100x100 binary(0-255)then each cell is 20x20
 */
class PatternExtractorNxN: public PatternExtractor{
	uint size;
	float cell_size;
public:
	PatternExtractorNxN(uint size=8,uint img_cols=100):size(size),cell_size(img_cols/float(size)){}
	Point2i getImgCoord(uint row,uint col){
		uint r=row*cell_size+cell_size/2;
		uint c=col*cell_size+cell_size/2;
		return Point2i(c,r);
	}
	uchar getCellValue(Mat& img,uint row,uint col){
		uint r=row*cell_size+cell_size/2;
		uint c=col*cell_size+cell_size/2;
		return img.at<uchar>(r,c);
	}
	void drawCircles(Mat& img){
		//first thing to work out
		//this allow dynamic cell_size
		cell_size=img.cols/float(size);
		Scalar black(0),white(255);
		uint w2=cell_size/4;
		for(uint i=1;i<size-1;i++)
			for(uint j=1;j<size-1;j++){
				uint u=getCellValue(img,i,j);
				Scalar color=u<128?white:black;
				Point2i p=getImgCoord(i,j);
				circle(img,p,w2,color);
			}
	}
	/**
	 * Border must be all black
	 * black pattern not allowed
	 */
	bool validPattern(Mat& img){
		//first thing to work out
		//this allow dynamic cell_size
		cell_size=img.cols/float(size);
		for(uint i=0;i<size;i++){
			//black border
			if(getCellValue(img,     0,     i)>128) return false;
			if(getCellValue(img,size-1,     i)>128) return false;
			if(getCellValue(img,     i,     0)>128) return false;
			if(getCellValue(img,     i,size-1)>128) return false;
		}
		//No black pattern allowed -> unless a white cell
		for(uint i=1;i<size-1;i++)
			for(uint j=1;j<size-1;j++){
				uint u=getCellValue(img,i,j);
				if(u>128) return true;
			}
		return false;
	}
	Pattern getPattern(Mat& img){
		//first thing to work out
		//this allow dynamic cell_size
		cell_size=img.cols/float(size);
		string pattern;
		if(validPattern(img)){
			for(uint i=1;i<size-1;i++)
				for(uint j=1;j<size-1;j++){
					uint u=getCellValue(img,i,j);
					pattern.push_back(u>128?'1':'0');
				}
			return Pattern(pattern);
		}
		throw runtime_error("Not valid pattern at PatternExtractor3x3::getPattern");
	}
};

