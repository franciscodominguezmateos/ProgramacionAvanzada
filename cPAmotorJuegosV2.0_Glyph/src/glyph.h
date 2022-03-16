/*
 * glyph.h
 *
 *  Created on: 6 Mar 2022
 *      Author: Francisco Dominguez
 */

#pragma once
#include "glyph_quadrilateral_detector.h"
#include "glyph_pattern_extractor.h"

class Glyph{
public:
	QuadrilateralDetector quadDetector;
	Mat img,gray;
	//vector<vector<Point> > contours;
	//vector<Point2f> approx;
	//Mat drawing;// = Mat::zeros( edges.size(), CV_8UC3 );
	Mat square;
	PatternExtractorNxN extractor;
	vector<Pattern> patterns;
public:
	Glyph(uint size=5):extractor(size){}
	Mat &getImage(){return img;}
	void drawQuadrilateral(Mat &drawing,Contour approx){
	  	   cv::line(drawing,approx[0],approx[1],Scalar(255,0,0),2);
	  	   cv::line(drawing,approx[1],approx[2],Scalar(255,255,0),2);
	  	   cv::line(drawing,approx[2],approx[3],Scalar(255,0,255),2);
	  	   cv::line(drawing,approx[3],approx[0],Scalar(  0,0,255),2);
	}
	/**
	 * Extract the black and white thresholded square image from img and a contour
	 * use perspectiveTransform and wrapPerspective
	 */
	Mat &quadrilateralAsSquareImage(Mat &img,Contour approx,uint size=100){
		float sz1=size-1;
		cvtColor(img,gray, COLOR_BGR2GRAY);
	  	vector<Point2f> dst;
	  	dst.push_back(Point2f(  0,  0));
	  	dst.push_back(Point2f(sz1,  0));
	  	dst.push_back(Point2f(sz1,sz1));
	  	dst.push_back(Point2f(  0,sz1));
	  	Mat T=cv::getPerspectiveTransform(approx,dst);
	  	cv::warpPerspective(gray,square,T,Size(size,size));
		threshold(square, square, 0, 255, THRESH_OTSU);
	  	return square;
	}
	vector<Pattern>& findPatterns(){
		patterns.clear();
		vector<Contour> quads=quadDetector.detect(img);
		for(Contour c:quads){
		  	   square=quadrilateralAsSquareImage(img,c);
		       drawQuadrilateral(img,c);
		       try{
			  	   Pattern pattern=extractor.getPattern(square);
			  	   pattern.setImageCornerPoints(c);
			  	   //cout << pattern.asString() << endl;
			  	   extractor.drawCircles(square);
				   imshow(pattern.asString(),square);
				   patterns.push_back(pattern);
		       }
		       catch(exception& e){ }
		}
		//imshow( "Contours", img );
		imshow( "Drawing", quadDetector.drawing );
		return patterns;
	}
};





