//============================================================================
// Name        : cPAMountainFly.cpp
// Author      : Francisco Dominguez
// Version     :
// Copyright   : Your copyright notice
// Description : Play with shader toy elevation
//               just press: q,a,o,p
// Date: 10/01/2021
//============================================================================

#include <iostream>
#include "game_engine.h"
#include "game_standard.h"
#include "axis.h"
#include "rectangle.h"
#include "shader_toy.h"

using namespace std;

//GameStandard poseTest("Testing class Pose");
class MountainFly:public GameStandard{
	ShaderToy* st;
	double stVel;
public:
	MountainFly(string title):GameStandard(title){}
	virtual void mouseMoved(int x, int y){
		GameStandard::mouseMoved(x,y);
		Vec4 v={float(x),float(y),0,0};
		st->setiMouse(v);
	}
	virtual void mousePress(int button, int state, int x, int y){
		GameStandard::mousePress(button,state,x,y);
	}
	virtual void keyPressed(char key,int x,int y){
		if(st!=nullptr){
			if(key=='o'){
				st->setRot(st->getRot()+Vector3D(0,1,0));
			}
			if(key=='p'){
				st->setRot(st->getRot()+Vector3D(0,-1,0));
			}
			if(key=='q'){
				stVel*=1.1;
			}
			if(key=='a'){
				stVel*=0.9;
			}
			double alfa=st->getRot().getY()*DEG2RAD;
			double cs=cos(alfa);
			double sn=sin(alfa);
			st->setVel(Vector3D(sn*stVel,0,cs*stVel));
		}
	}
	virtual void onReshape(int width,int height){
		Game::onReshape(width,height);
		if(st!=nullptr) st->setiResolution(width,height);
	}
	//cPAMountainFly::init()
	void init(){
		Camera* pCam  =getCamera();
		Stage*  pStage=getStage();
		Camera &cam   =*pCam;
		Stage  &stage =*pStage;
		cam.setPos(Vector3D(0,0,10));
		Light* lightFront=new Light(Vector3D(150.0,150.0, 100.0));
		stage.add(lightFront);
		stage.add(new Light(Vector3D(-100.0,150.0, -150.0)));


		st=new ShaderToy();
		st->initFromFileName("elevated_shader_toy.glsl");
		stVel=100;
		st->setVel(Vector3D(0,0,stVel));
		st->hazFija();
	    Mat img(256, 256, CV_32FC4);
	    randu(img, Scalar(0, 0, 0,0), Scalar(255, 255, 255,255));
	    img/=255;
	    st->setImage(img);
		stage.add(st);
	}
	void update(double dt){
		if (waitKey(1) == 27) exit(0);
		if(st!=nullptr){
			st->setPos(st->getPos()+st->getVel()*dt);
			GLSLShaderProgram &sp=st->getShaderProgram();
			sp.start();
			sp["pro"]=st->getPos();
			sp["pta"]=st->getPos()+st->getVel()*dt*stVel;
			sp.stop();
		}
	}
} mountainFly("cPAMountainFly");

int main(int argc, char** argv) try{
	srand(10);
	GameEngine::setGame(&mountainFly);
	GameEngine::gameInit(argc,argv);
	GameEngine::gameMainLoop();
	return 0;
}
catch (exception &e){
	cout << "Error in cPATestShaderToy::main(): "<<e.what() <<endl;
	return 1;
}
