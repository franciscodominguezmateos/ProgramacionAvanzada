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
#include "shader_toy.h"
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
	int getInt(vector<string>& vs,string keyName){
		string key,val;
		for(uint i=0;i<vs.size();i++){
			keyVal(key,val,vs[i]);
			if(key==keyName)  return stoi(val);
		}
		throw runtime_error("Key name='"+keyName+"' not found in ROSCameraInfo.getInt");
	}
	string getString(vector<string>& vs,string keyName){
		string key,val;
		for(uint i=0;i<vs.size();i++){
			keyVal(key,val,vs[i]);
			if(key==keyName)  return val;
		}
		throw runtime_error("Key name='"+keyName+"' not found in ROSCameraInfo.getString");
	}
	Mat getMat(vector<string>& vs,string keyName,uint rows=3,uint cols=3){
		string key,val;
		for(uint i=0;i<vs.size();i++){
			keyVal(key,val,vs[i]);
			if(key==keyName){
				i+=3;
				vector<double> vd=buildNumberList(vs[i]);
				Mat r=buildMatFromNumberList(rows,cols,vd);
				return r;
			}
		}
		throw runtime_error("Key name='"+keyName+"' not found in ROSCameraInfo.getMat");
	}
	void readYaml(string fileName){
		ifstream file(fileName);
		vector<string> vs;
		file >> vs;
		image_width            =getInt   (vs,"image_width");
		image_height           =getInt   (vs,"image_height");
		camera_name            =getString(vs,"camera_name");
		camera_matrix          =getMat   (vs,"camera_matrix");
		distortion_model       =getString(vs,"distortion_model");
		distortion_coefficients=getMat   (vs,"distortion_coefficients",1,5);
		rectification_matrix   =getMat   (vs,"rectification_matrix");
		projection_matrix      =getMat   (vs,"projection_matrix",3,4);
	}
};
class GlyphTest:public GameAR{
	VideoCapture cap;
	Glyph g;
	RectanglePtr pR;
	SpherePtr pS;
	Undistor* undist;
	ShaderToy* st;
public:
	GlyphTest(string title,	Undistor* und):
		GameAR(title,KP),cap(0),g(5),undist(und){
		cap.set(CV_CAP_PROP_FRAME_WIDTH,1280>>1);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT,960>>1);
	}
	void init(){
		ROSCameraInfo cinfL;
		cinfL.readYaml("/home/francisco/.ros/camera_info/stereo/left.yaml");
		GameAR::init();
		undist->init();

		//Pose update speed
		getCamera().setAlpha(0.2);
		Stage&  stage =getStage();
		Light* lightFront=new Light(Vector3D(0.0,0.0, 10.0));
		stage.add(lightFront);

		//It is going to be rendered in the rectangle pR
		st=new ShaderToy();
		st->init();
		st->setFbo(new GLSLFBO(320*2,240*2));
		//stage.add(st);

		double aspectRatio=480.0/640.0;
		double h=0.06*aspectRatio;
		double w=0.06*1;

		pR=new Rectangle(Vector3D(-w,-h+h),Vector3D(w,-h+h),Vector3D(w,h+h),Vector3D(-w,h+h));
		pR->getTextura().init();
		stage.add(pR);

		stage.add(new Axis(0.06));
		pS=new Sphere(0.02);
		pS->setPos(Vector3D(0,0.02,0.14));
		pS->setCol(Vector3D(0.9,0.9,0.1));
		pS->hazFija();
		stage.add(pS);
	}
	void update(double dt){
		Mat img,uimg;
		cap>>img;
		uimg=undist->process(img);
		//Convert to CV_8UC3
		uimg.convertTo(g.img,img.type(),255.0);
		vector<Pattern>& patterns=g.findPatterns();
		for(Pattern& p:patterns){
			if(p.asString()=="100010101"){
				p.solvePose(KP,dist);
				setPose(p.getRvec(),p.getTvec());
			}
		}
		setBackgroundImage(g.img);

		//Render the ShaderToy st in the Rectangle pR
		st->render();
		Mat imgRect=st->getFbo()->toOpenCV();
		pR->getTextura().setImage(imgRect);

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
