//============================================================================
// Name        : cPAGlyphTest.cpp
// Author      : Francisco Dominguez
// Version     :
// Copyright   : Your copyright notice
// Description : this class is for test Glyph and GameAR
// Date: 13/03/2022
//============================================================================

#include <iostream>
#include "game_engine.h"
#include "game_ar.h"
#include "rectangle.h"
#include "sphere.h"
#include "axis.h"
#include "background_texture.h"
#include "image_undistor.h"

#include "glyph.h"

using namespace std;

Mat K=(Mat_<double>(3,3) <<
		951.4741259832514, 0, 293.3416170604175,
		0, 950.3903009136468, 272.6355355349708,
		0, 0, 1);
Mat KP=(Mat_<double>(3,3)<<953.0537522907438, 0, 384.4876556396484, 0, 953.0537522907438, 276.9204998016357,  0, 0, 1);

//Mat dist=Mat::zeros(4,1,cv::DataType<double>::type); // Assuming no lens distortion
Mat dist=(Mat_<double>(4,1)<<-0.4953024672257269, 0.3330644888148587, -0.004006958790423586, 0.008548218245607087); // Assuming no lens distortion


template<class S=double>
Mat buildMatFromNumberList(uint rows,uint cols,vector<S>& v){
	Mat m=Mat::zeros(rows,cols,cv::DataType<S>::type);
	for(uint r=0;r<rows;r++)
		for(uint c=0;c<cols;c++){
			m.at<S>(r,c)=v[r*cols+c];
		}
	return m;
}
class ROSCameraInfo{
public:
	uint image_width;
	uint image_height;
	string camera_name;
	Mat camera_matrix;
	string distortion_model;
	Mat distortion_coefficients;
	Mat rectification_matrix;
	Mat projection_matrix;
	void keyVal(string& key,string& val,string& line){
		vector<string> kv=split_first(line,':');
		key=kv[0];
		val=trim(kv[1]);
	}
	vector<double> buildNumberList(string& line){
		string key,val;
		keyVal(key,val,line);
		string numbers=val.substr(1,val.size()-2);
		vector<double> vd=split_numbers(numbers,',');
		return vd;
	}
	void readYaml(string fileName){
		ifstream file(fileName);
		vector<string> vs;
		file >> vs;
		string key,val;
		for(uint i=0;i<vs.size();){
			keyVal(key,val,vs[i]);
			if(key=="image_width")  image_width =stoi(val);
			if(key=="image_height")	image_height=stoi(val);
			if(key=="camera_name")	camera_name =     val;
			if(key=="camera_matrix"){
				i+=3;
				vector<double> vd=buildNumberList(vs[i]);
				camera_matrix=buildMatFromNumberList(3,3,vd);
			}
			if(key=="distortion_model") distortion_model=val;
			if(key=="distortion_coefficients"){
				i+=3;
				vector<double> vd=buildNumberList(vs[i]);
				distortion_coefficients=buildMatFromNumberList(1,5,vd);
			}
			if(key=="rectification_matrix"){
				i+=3;
				vector<double> vd=buildNumberList(vs[i]);
				rectification_matrix=buildMatFromNumberList(3,3,vd);
			}
			if(key=="projection_matrix"){
				i+=3;
				vector<double> vd=buildNumberList(vs[i]);
				projection_matrix=buildMatFromNumberList(3,4,vd);
			}
			i++;
		}
	}
};

class GlyphTest:public GameAR{
	VideoCapture cap;
	Glyph g;
	RectanglePtr pR;
	SpherePtr pS;
	Undistor* undist;
public:
	GlyphTest(string title,	Undistor* und):
		GameAR(title,KP),cap(1),g(8),undist(und){
		cap.set(CV_CAP_PROP_FRAME_WIDTH,1280>>0);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT,960>>0);
	}
	void init(){
		ROSCameraInfo cinfL;
		//cinfL.readYaml("/home/francisco/.ros/camera_info/stereo/left.yaml");
		GameAR::init();
		undist->init();
		//Pose update speed
		getCamera().setAlpha(0.25);
		Stage&  stage =getStage();
		Light* lightFront=new Light(Vector3D(0.0,0.0, 10.0));
		stage.add(lightFront);

		double aspectRatio=480.0/640.0;
		double h=0.06*aspectRatio;
		double w=0.06*1;

		//pR=new Rectangle(Vector3D(-w,-h+h),Vector3D(w,-h+h),Vector3D(w,h+h),Vector3D(-w,h+h));
		//pR->getTextura().init();
		//stage.add(pR);

		stage.add(new Axis(0.06));
		pS=new Sphere(0.02);
		pS->setPos(Vector3D(0,0.02,0.4));
		pS->setCol(Vector3D(0.9,0.9,0.1));
		pS->hazFija();
		stage.add(pS);
	}
	void update(double dt){
		Mat img,uimg;
		cap>>g.img;
		//uimg=undist->process(img);
		//Convert to CV_8UC3
		//uimg.convertTo(g.img,img.type(),255.0);
		vector<Pattern>& patterns=g.findPatterns();
		for(Pattern& p:patterns){
			if(p.asString()=="100010101"){
				p.solvePose(KP,dist);
				setPose(p.getRvec(),p.getTvec());
			}
		}
		setBackgroundImage(g.img);
		//pR->getTextura().setImage(g.img);
		if(waitKey(1)==27)
			exit(0);
	}
	virtual void keyPressed(char key,int x,int y){
		GameAR::keyPressed(key,x,y);
		if(key=='w'){
			pS->setPos(pS->getPos()+Vector3D(0,0.001,0));
		}
		if(key=='s'){
			pS->setPos(pS->getPos()-Vector3D(0,0.001,0));
		}
		if(key=='e'){
			pS->setPos(pS->getPos()-Vector3D(0,0,0.001));
		}
		if(key=='r'){
			pS->setPos(pS->getPos()+Vector3D(0,0,0.001));
		}
		//cout << *pS <<endl;
	}
};
GlyphTest* pGlyphTest;

int main(int argc, char** argv) try{
	srand(10);
	Undistor* undist=new Undistor();
	pGlyphTest=new GlyphTest("Testing Glyph detector",undist);
	GameEngine::setGame(pGlyphTest);
	GameEngine::gameInit(argc,argv);
	GameEngine::gameMainLoop();
	return 0;
}
catch (exception &e){
	cout << "Error in cPAGlyphTest::main(): "<<e.what() <<endl;
	return 1;
}
