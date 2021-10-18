//============================================================================
// Name        : cPATestShaderToy.cpp
// Author      : Francisco Dominguez
// Version     :
// Copyright   : Your copyright notice
// Description : Testing class ShaderToy
// Date: 05/01/2021
//============================================================================

#include <iostream>
#include "game_engine.h"
#include "game_standard.h"
#include "axis.h"
#include "rectangle.h"
#include "shader_toy.h"
#include "pose_util.h"
#include <omp.h>

using namespace std;

//GameStandard poseTest("Testing class Pose");
class TestShaderToy:public GameStandard{
	Texture texGround;
	Axis* ax;
	Axis* lookAtAxis;
	ShaderToy* st;
	double stVel;
public:
	TestShaderToy(string title):GameStandard(title){}
	virtual void mouseMoved(int x, int y){
		GameStandard::mouseMoved(x,y);
		Vec4 v={float(x),float(y),0,0};
		st->setiMouse(v);
	}
	virtual void mousePress(int button, int state, int x, int y){
		GameStandard::mousePress(button,state,x,y);
	}
	virtual void keyPressed(char key,int x,int y){
		GameStandard::keyPressed(key,x,y);
		lookAtAxis->setPos(getCamera()->getLookAtPos());
	}
	virtual void onReshape(int width,int height){
		Game::onReshape(width,height);
		if(st!=nullptr) st->setiResolution(width,height);
	}
	//cPATestShaderToy::init()
	void init(){
#pragma omp parallel
{
  // Code inside this region runs in parallel.
  printf("Hello! %d\n",omp_get_thread_num());
}
#pragma omp parallel for
for(int n=0; n<10; ++n)
{
  printf(" %d,%d", n, omp_get_thread_num());
}
printf(".\n");
		Camera* pCam  =getCamera();
		Stage*  pStage=getStage();
		Camera &cam   =*pCam;
		Stage  &stage =*pStage;
		cam.setPos(Vector3D(0,1.65,10));
		Light* lightFront=new Light(Vector3D(150.0,150.0, 100.0));
		stage.add(lightFront);
		stage.add(new Light(Vector3D(-100.0,150.0, -150.0)));

		//Mat imgGround=imread("brown_brick_texture_map.jpg");

		st=new ShaderToy();
		//st->init();
		//st->initFromFileName("heart_beats_shader_toy.glsl");
		//st->initFromFileName("happy_jumping_shader_toy.glsl");
		st->initFromFileName("shader_toy_sample.stc");
		//st->initFromFileName("distance_pn_continous_shader_toy.glsl");
		stage.add(st);
	}
	void update(double dt){
		if (waitKey(1) == 27) exit(0);
		if(st!=nullptr){
			GLSLShaderProgram &sp=st->getShaderProgram();
			try{
				sp.start();
				Mat camera=getCamera()->getMat();
				//Pose of camera or Transformation from camera 2 world
				sp["camera"   ]=camera.inv();
				//Camera matrix  or Transformation from world 2 camera
				sp["cameraInv"]=camera;
				Mat p=getView()->getProjection()->getMat();
				sp["projectionInv"]=p.inv();
				sp["projection"   ]=p;
				sp.stop();
			}
			catch(runtime_error &re){
				//do not pay attention to uniform errors
				//cout << re.what()<<endl;
			}
		}
	}
} testShaderToy("cPATestShaderToy");

int main(int argc, char** argv) try{
	srand(10);
	GameEngine::setGame(&testShaderToy);
	GameEngine::gameInit(argc,argv);
	GameEngine::gameMainLoop();
	return 0;
}
catch (exception &e){
	cout << "Error in cPATestShaderToy::main(): "<<e.what() <<endl;
	return 1;
}
