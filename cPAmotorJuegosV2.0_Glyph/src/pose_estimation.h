/*
 * pose_estimation.h
 *
 *  Created on: 5 Mar 2022
 *      Author: Francisco Dominguez
 */
#pragma once
#include <opencv2/opencv.hpp>

using namespace cv;

class PoseEstimation{
protected:
	Mat rvec,tvec;
	Mat K,dist;
public:
	PoseEstimation(Mat K=Mat::eye(3,3,cv::DataType<double>::type),
			       Mat d=Mat::zeros(4,1,cv::DataType<double>::type)):
		rvec(Mat::zeros(3,1,cv::DataType<double>::type)),
		tvec(Mat::zeros(3,1,cv::DataType<double>::type)),
		K(K),
		dist(d){
		}
	virtual bool estimatePose(Mat& img)=0;
	const Mat& getTvec() const {return tvec;}
	void setTvec(const Mat& tvec) {	this->tvec = tvec;	}
	const Mat& getRvec() const {return rvec;}
	void setRvec(const Mat& rvec) {	this->rvec = rvec;	}
	const Mat& getDist() const {return dist;}
	void setDist(const Mat& dist) {this->dist = dist;	}
	const Mat& getK() const {return K;	}
	void setK(const Mat& k) {K = k;}
};

