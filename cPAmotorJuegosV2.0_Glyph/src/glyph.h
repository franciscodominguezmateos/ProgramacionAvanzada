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
	RNG rng;
	Mat img,gray,edges;
	vector<vector<Point> > contours;
	vector<Point2f> approx;
	Mat drawing;// = Mat::zeros( edges.size(), CV_8UC3 );
	Mat square;
	PatternExtractor3x3 extractor;
public:
	Glyph(){}
	Glyph(String s){}
	Glyph(vector<char> v){}
	Glyph(vector<bool> v){}
	Glyph(Mat square){}
	Glyph(Mat img,vector<Point2f> quad){}
	Mat &getImage(){return img;}

	void drawQuadrilateral(Mat &drawing,Contour approx){
	  	   cv::line(drawing,approx[0],approx[1],Scalar(255,0,0),3);
	  	   cv::line(drawing,approx[1],approx[2],Scalar(255,255,0),3);
	  	   cv::line(drawing,approx[2],approx[3],Scalar(255,0,255),3);
	  	   cv::line(drawing,approx[3],approx[0],Scalar(  0,0,255),3);
	}
	Mat &quadrilateralAsSquare(Mat &gray,Contour approx){
	  	   vector<Point2f> dst;
	  	   dst.push_back(Point2f( 0, 0));
	  	   dst.push_back(Point2f(99, 0));
	  	   dst.push_back(Point2f(99,99));
	  	   dst.push_back(Point2f( 0,99));
	  	   Mat T=cv::getPerspectiveTransform(approx,dst);
	  	   cv::warpPerspective(gray,square,T,Size(100,100));
		   threshold(square, square, 0, 255, THRESH_OTSU);
	  	   return square;
	}
	void findContours(){
		//Mat drawing;
		//img.copyTo(drawing);
		cvtColor(img,gray, COLOR_BGR2GRAY);
	    namedWindow( "Contours", WINDOW_AUTOSIZE );
		vector<Contour> quads=quadDetector.detect(img);
		for(unsigned int i=0;i<quads.size();i++){
			Contour c=quads[i];
		//for(Contour c:quads){
		  	   Rect r=boundingRect(c);
		  	   cv::rectangle(quadDetector.drawing,r,Scalar(0,255,0),1);
		  	   square=quadrilateralAsSquare(gray,c);
		       drawQuadrilateral(img,c);
		       try{
			  	   Pattern pattern=extractor.getPattern(square);
			  	   cout << pattern.asString() << endl;
			  	   drawPattern();
				   imshow("square"+pattern.asString(),square);
		       }
		       catch(exception& e){

		       }
		}
		imshow( "Contours", img );
		imshow( "Drawing", quadDetector.drawing );
	}
	void drawPattern(){
		vector<uchar> pattern;
		string s;
		int w=10;
		int w2=w/2;
		uint cell_col=square.cols/w;
		unsigned int cell_row=square.rows/w;
		unsigned int row1=cell_row*3;
		unsigned int row2=cell_row*5;
		unsigned int row3=cell_row*7;
		unsigned int col1=cell_col*3;
		unsigned int col2=cell_col*5;
		unsigned int col3=cell_col*7;
		cv::circle(square,Point(row1,col1),w2,Scalar(255,0,0));
		cv::circle(square,Point(row1,col2),w2,Scalar(255,0,0));
		cv::circle(square,Point(row1,col3),w2,Scalar(255,0,0));
		cv::circle(square,Point(row2,col1),w2,Scalar(255,0,0));
		cv::circle(square,Point(row2,col2),w2,Scalar(255,0,0));
		cv::circle(square,Point(row2,col3),w2,Scalar(255,0,0));
		cv::circle(square,Point(row3,col1),w2,Scalar(255,0,0));
		cv::circle(square,Point(row3,col2),w2,Scalar(255,0,0));
		cv::circle(square,Point(row3,col3),w2,Scalar(255,0,0));
	}
};





