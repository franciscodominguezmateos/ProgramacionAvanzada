/*
 * pose_estimation_chessboard.h
 *
 *  Created on: 2 Nov 2017
 *      Author: Francisco Dominguez
 */
#pragma once
#include "pose_stimation.h"

class PoseEstimationChessBoard: public PoseEstimation{
	int rowCorners;
	int colCorners;
	double xMin,yMin,squareSize;
	bool drawCorners;
public:
	PoseEstimationChessBoard(Mat K=Mat::eye(3,3,cv::DataType<double>::type),
			                 Mat d=Mat::zeros(4,1,cv::DataType<double>::type),
							 int rc=6,int cc=8,
							 double xm=-30,double ym=-30,double ss=10,
							 bool dc=true):
		PoseEstimation(K,d),
		rowCorners(rc),
		colCorners(cc),
		xMin(xm),yMin(ym),squareSize(ss),
		drawCorners(dc){
		}
	bool estimatePose(Mat &tablero){
		 Size patternsize(rowCorners,colCorners); //interior number of corners
		 Mat gray;
		 cvtColor(tablero,gray, COLOR_BGR2GRAY);

		 vector<Point2f> corners; //this will be filled by the detected corners
		 //CALIB_CB_FAST_CHECK saves a lot of time on images
		 //that do not contain any chessboard corners
		 bool patternfound = findChessboardCorners(gray, patternsize, corners,
		         CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
		         + CALIB_CB_FAST_CHECK);
		 if(patternfound){
			 cornerSubPix(gray, corners, Size(4, 4), Size(-1, -1),TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

			 if(drawCorners)
				 drawChessboardCorners(tablero, patternsize, Mat(corners), patternfound);

			 vector<Point3d> model_points;
			 double squareSize=10;
			 double x,y;
			 int i,j;
			 for(i=0,x=xMin;i<colCorners;i++,x+=squareSize){
				 for(j=0,y=yMin;j<rowCorners;j++,y+=squareSize){
					 model_points.push_back(Point3d(y,x,0));
					 //cout << "x="<<x<<"y="<<y<<endl;
				 }
			 }
			 //Solve for pose
			 solvePnP(model_points,corners,K,dist,rvec,tvec);
			 return true;
		 }
		 return false;
	}
	int getColCorners() const {return colCorners;}
	void setColCorners(int colCorners) {this->colCorners = colCorners;	}
	bool isDrawCorners() const {return drawCorners;	}
	void setDrawCorners(bool drawCorners) {	this->drawCorners = drawCorners;	}
	int getRowCorners() const {return rowCorners;}
	void setRowCorners(int rowCorners) {this->rowCorners = rowCorners;	}
	double getSquareSize() const {	return squareSize;	}
	void setSquareSize(double squareSize) {	this->squareSize = squareSize;	}
	double getXmin() const {return xMin;}
	void setXmin(double min) {	xMin = min;	}
	double getYmin() const {return yMin;	}
	void setYmin(double min) {yMin = min;}
};

