/*
 * glyph_quadrilateral_detector.h
 *
 *  Created on: 6 Mar 2022
 *      Author: Francisco Dominguez
 */

#pragma once
#include "glyph_contour.h"

class QuadrilateralDetector{
public:
	RNG rng;
	Mat img,gray,edges,drawing;
	vector<vector<Point> > contours;
	Contour approx;
	Mat square;
	vector<Contour> quads;
public:
	Mat &getImage(){return img;}
	Mat &detectEdges(Mat &img){
		// Stage 2: Detect edges in image
		Mat gblur;
		cvtColor(img,gray, COLOR_BGR2GRAY);
		GaussianBlur(gray,gblur, Size(5,5), 0);
		Canny(gray,edges, 50, 150);
        // dilate canny output to remove potential
        // holes between edge segments
        dilate(edges, edges, Mat(), Point(-1,-1));
        erode(edges,edges,Mat(),Point(-1,-1));
        return edges;
	}
	bool isQuadrilateralContour(vector<Point> &contour){
		double perimeter=arcLength(contour,true);
		cv::approxPolyDP(contour,approx,0.01*perimeter,true);
		return approx.size()==4 &&
	           fabs(contourArea(approx)) > 16*16 &&
	           isContourConvex(approx) ;
	}
	// comparison function object
	static bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
	    double i = fabs( contourArea(cv::Mat(contour1)) );
	    double j = fabs( contourArea(cv::Mat(contour2)) );
	    return ( i < j );
	}
	void sortQuadrilateralPoints(){
		sort(approx.begin(),approx.end(),[](Point2f &p0,Point2f &p1){return p0.y<p1.y;});
		Point2f &p0=approx[0];
		Point2f &p1=approx[1];
		if(p0.x>p1.x)
			swap(p0,p1);
		Point2f &p2=approx[2];
		Point2f &p3=approx[3];
		if(p2.x<p3.x)
			swap(p2,p3);
	}
	void drawQuadrilateral(Mat &drawing,Contour c){
	  	   cv::line(drawing,c[0],c[1],Scalar(255,0,0),3);
	  	   cv::line(drawing,c[1],c[2],Scalar(255,255,0),3);
	  	   cv::line(drawing,c[2],c[3],Scalar(255,0,255),3);
	  	   cv::line(drawing,c[3],c[0],Scalar(  0,0,255),3);
	}
	void drawQuadrilaterals(Mat &drawing){
		for(Contour c:quads)
			drawQuadrilateral(drawing,c);
	}
	vector<Contour> &detect(Mat &img){
		img.copyTo(this->img);
		// Stage 3: Find contours
		vector<Vec4i> hierarchy;
		//clean quads
		quads.clear();
		//Detect edges
		edges=detectEdges(img);
		// Find contours
		//cv::findContours( edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
		cv::findContours( edges, contours, hierarchy, RETR_CCOMP , CHAIN_APPROX_SIMPLE, Point(0, 0) );
		sort(contours.begin(),contours.end(),compareContourAreas);
		reverse(contours.begin(),contours.end());
        /// Draw contours
		cvtColor(edges,drawing,COLOR_GRAY2BGR);
		addWeighted(img,0.1,drawing,0.9,0.0,drawing);
		for(unsigned int i = 0; i< (contours.size()>10?10:contours.size()); i++ ){
		     Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		     if(isQuadrilateralContour(contours[i])){
		    	 drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
	             //imshow("drawing",drawing);
		         sortQuadrilateralPoints();
		         quads.push_back(approx);
		     }
		 }
		 return quads;
	}
};



