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
class PatternExtractor3x3: public PatternExtractor{
	uint size;
	uint cell_size;
public:
	PatternExtractor3x3():size(5),cell_size(100/size){}
	uchar getCellValue(Mat& img,uint row,uint col){
		uint r=row*cell_size+cell_size/2;
		uint c=col*cell_size+cell_size/2;
		return img.at<uchar>(r,c);
	}
	/**
	 * Border must be all black
	 */
	bool validPattern(Mat& img){
		for(uint i=0;i<size;i++){
			if(getCellValue(img,     0,     i)>128) return false;
			if(getCellValue(img,size-1,     i)>128) return false;
			if(getCellValue(img,     i,     0)>128) return false;
			if(getCellValue(img,     i,size-1)>128) return false;
		}
		return true;
	}
	Pattern getPattern(Mat& img){
		string pattern;
		if(validPattern(img)){
			for(uint i=1;i<size-1;i++)
				for(uint j=1;j<size-1;j++){
					uint u=getCellValue(img,i,j);
					pattern.push_back(u>128?'1':'0');
				}
			if(pattern=="000000000") throw runtime_error("Black pattern not allowed in PatternExtractor3x3::getPattern");
			return Pattern(pattern);
		}
		throw runtime_error("Not valid pattern at PatternExtractor3x3::getPattern");
	}
};

