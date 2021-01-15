//============================================================================
// Name        : cPATestSolidRayMarching.cpp
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
#include "solid_raymarching.h"
#include "pose_util.h"
#include "modelo_material.h"

using namespace std;

//GameStandard poseTest("Testing class Pose");
class TestSolidRayMarching:public GameStandard{
	Texture texGround;
	AxisPtr ax;
	AxisPtr lookAtAxis;
	SolidRayMarchingPtr st;
	double stVel;
public:
	TestSolidRayMarching(string title):GameStandard(title){}
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
		CameraPtr pCam  =getCamera();
		StagePtr  pStage=getStage();
		Camera &cam   =*pCam;
		Stage  &stage =*pStage;
		cam.setPos(Vector3D(0,1.65,10));
		Luz* lightFront=new Luz(Vector3D(150.0,150.0, 100.0));
		stage.add(lightFront);
		stage.add(new Luz(Vector3D(-100.0,150.0, -150.0)));

		st=new SolidRayMarching();
		st->initFromFileName("raymarching_for_dummies_shader_toy.glsl");
		stage.add(st);

		ax=new Axis();
		stage.add(ax);
		lookAtAxis=new Axis();
		lookAtAxis->setName(Text("LookAtAxis"));
		stage.add(lookAtAxis);
		//Floor
		texGround.init();
		Mat imgGround=imread("brick_pavement_0077_01_preview.jpg");

		//Mat imgGround=imread("brown_brick_texture_map.jpg");
		texGround.setImage(imgGround);
		float l=10;
		Rectangle* r=new Rectangle(Vector3D(-l,1.1,-l),Vector3D(-l,1.1,l),Vector3D(l,0,l),Vector3D(l,0,-l));
		r->setTextura(texGround);
		r->setNU(10);
		r->setNV(10);
	    stage.add(r);

		ModeloMaterial* mm=new ModeloMaterial("TheAmazingSpiderman.obj");
		mm->setPos(Vector3D(0,0,-3));
		mm->setScale(1.125);
		mm->setVel(Vector3D(getRand(1,-1),0,getRand(1,-1)));
		stage.add(mm);

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
} testSolidRayMarching("cPATestSolidRayMarching");

int main(int argc, char** argv) try{
	srand(10);
	GameEngine::setGame(&testSolidRayMarching);
	GameEngine::gameInit(argc,argv);
	GameEngine::gameMainLoop();
	return 0;
}
catch (exception &e){
	cout << "Error in cPATestSolidRayMarching::main(): "<<e.what() <<endl;
	return 1;
}
