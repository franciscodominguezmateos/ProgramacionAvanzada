//============================================================================
// Name        : cPATestShaderToy.cpp
// Author      : Francisco Dominguez
// Version     :
// Copyright   : Your copyright notice
// Description : Testing classes Pose and Twist
// Date: 05/01/2021
//============================================================================

#include <iostream>
#include "game_engine.h"
#include "game_standard.h"
#include "axis.h"
#include "rectangle.h"
#include "shader_toy.h"

using namespace std;

//GameStandard poseTest("Testing class Pose");
class TestShaderToy:public GameStandard{
	Texture texGround;
	Axis* ax;
	Axis* lookAtAxis;
	vector<Axis> axisPool;
	unsigned int axisPoolIdx;
	ShaderToy* st;
public:
	TestShaderToy(string title):GameStandard(title),axisPool(100),axisPoolIdx(0){}
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
	//Return an axis from the AxisPool
	Axis &newAxis(){
		Axis &r=axisPool[axisPoolIdx];
		axisPoolIdx++;
		if(axisPoolIdx==axisPool.size()) axisPoolIdx=0;
		return r;
	}
	//cPATestShaderToy::init()
	void init(){
		Camera* pCam  =getCamera();
		Stage*  pStage=getStage();
		Camera &cam   =*pCam;
		Stage  &stage =*pStage;
		cam.setPos(Vector3D(0,0,10));
		Luz* lightFront=new Luz(Vector3D(150.0,150.0, 100.0));
		stage.add(lightFront);
		stage.add(new Luz(Vector3D(-100.0,150.0, -150.0)));

		Mat imgGround=imread("brown_brick_texture_map.jpg");

		st=new ShaderToy();
		st->initFromFileName("happy_jumping.stc");
		//st->initFromFileName("shader_toy_sample.stc");
		//st->initFromFileName("path_tracing_cornellbox_with_MIS.stc");

		stage.add(st);

		ax=new Axis();
		stage.add(ax);
		lookAtAxis=new Axis();
		lookAtAxis->setName(Text("LookAtAxis"));
		stage.add(lookAtAxis);
		//Floor
		texGround.init();
		//Mat imgGround=imread("brick_pavement_0077_01_preview.jpg");
		//Mat imgGround=imread("brown_brick_texture_map.jpg");
		//texGround.setImage(imgGround);
		float l=10;
		Rectangle* r=new Rectangle(Vector3D(-l,0,-l*2),Vector3D(-l,0,l*2),Vector3D(l,0,l*2),Vector3D(l,0,-l*2));
		r->setTextura(texGround);
		r->setNU(10);
		r->setNV(10);
	    stage.add(r);
	}
	void update(double dt){
		if (waitKey(1) == 27) exit(0);
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
