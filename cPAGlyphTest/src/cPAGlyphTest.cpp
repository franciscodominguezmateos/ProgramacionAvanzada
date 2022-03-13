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

//Mat dist=Mat::zeros(4,1,cv::DataType<double>::type); // Assuming no lens distortion
Mat dist=(Mat_<double>(4,1)<<-0.4953024672257269, 0.3330644888148587, -0.004006958790423586, 0.008548218245607087); // Assuming no lens distortion

class GlyphTest:public GameAR{
	VideoCapture cap;
	Glyph g;
	RectanglePtr pR;
	SpherePtr pS;
public:
	GlyphTest(string title):
		GameAR(title,K),cap(0){
		cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
	}
	void init(){
		GameAR::init();
		//Pose update speed
		getCamera().setAlpha(0.1);
		Stage&  stage =getStage();
		Light* lightFront=new Light(Vector3D(0.0,0.0, 10.0));
		stage.add(lightFront);

		double aspectRatio=480.0/640.0;
		double h=0.06*aspectRatio;
		double w=0.06*1;
		pR=new Rectangle(Vector3D(-w,-h+h),Vector3D(w,-h+h),Vector3D(w,h+h),Vector3D(-w,h+h));
		pR->getTextura().init();
		stage.add(pR);

		stage.add(new Axis(0.06));
		pS=new Sphere(0.02);
		pS->setPos(Vector3D(0,0.02,0.4));
		pS->setCol(Vector3D(0.9,0.9,0.1));
		pS->hazFija();
		stage.add(pS);
	}
	void update(double dt){
		cap>>g.img;
		vector<Pattern>& patterns=g.findPatterns();
		for(Pattern& p:patterns){
			if(p.asString()=="100010101"){
				p.solvePose(K,dist);
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

} glyphTest("Testing Glyph detector");

int main(int argc, char** argv) try{
	srand(10);
	GameEngine::setGame(&glyphTest);
	GameEngine::gameInit(argc,argv);
	GameEngine::gameMainLoop();
	return 0;
}
catch (exception &e){
	cout << "Error in cPAGlyphTest::main(): "<<e.what() <<endl;
	return 1;
}
