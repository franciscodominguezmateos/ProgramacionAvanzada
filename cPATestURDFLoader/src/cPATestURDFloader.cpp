//============================================================================
// Name        : cPATestURDFloader.cpp
// Author      : Francisco Dominguez
// Version     :
// Copyright   : Your copyright notice
// Description : LoaderURDF
// Date: 07/01/2021
//============================================================================
#include <iostream>
#include "game_engine.h"
#include "game_standard.h"
#include "axis.h"
#include "rectangle.h"

#include "loader_urdf.h"

#include "model_joint.h"
#include "solid_articulated.h"


using namespace std;

class TestURDFloader:public GameStandard{
	Texture texGround;
	Axis* ax;
	Axis* lookAtAxis;
    //GLSLShaderProgram shaderProgram;
    //SolidVAO mvao;
	LoaderURDF lurdf;
public:
	TestURDFloader(string title):GameStandard(title)/*,mvao(&shaderProgram)*/{}
	virtual void mouseMoved(int x, int y){
		GameStandard::mouseMoved(x,y);
	}
	virtual void mousePress(int button, int state, int x, int y){
		GameStandard::mousePress(button,state,x,y);
	}
	virtual void keyPressed(char key,int x,int y){
		GameStandard::keyPressed(key,x,y);
		Vector3D p=getCamera().getLookAtPos();
		lookAtAxis->setPos(p);
	}
	Mat stackH(Mat im1,Mat im2){
		Mat im3;
		hconcat(im1,im2,im3);
		return im3;
	}
	//TestURDFloader::init()
	void init(){
		Camera& cam   =getCamera();
		Stage&  stage =getStage();
		cam.setPos(Vector3D(0,0,10));
		Light* lightFront=new Light(Vector3D(150.0,150.0, 100.0));
		stage.add(lightFront);
		stage.add(new Light(Vector3D(-100.0,150.0, -150.0)));

		ax=new Axis(Vector3D(0.5,0.5,0.5));
		stage.add(ax);
		lookAtAxis=new Axis();
		lookAtAxis->setName(Text("LookAtAxis"));
		//stage.add(lookAtAxis);
		//Floor
		texGround.init();
		//Mat imgGround=imread("brick_pavement_0077_01_preview.jpg");
		Mat imgGround=imread("brown_brick_texture_map.jpg");
		texGround.setImage(imgGround);
		float l=10;
		Rectangle* r=new Rectangle(Vector3D(-l,0,-l*2),Vector3D(-l,0,l*2),Vector3D(l,0,l*2),Vector3D(l,0,-l*2));
		r->setTextura(texGround);
		r->setNU(1);
		r->setNV(1);
	    //stage.add(r);

    	lurdf.init(stage);
    	addShader(&lurdf.shaderProgram);

	}
	void update(double dt){
		if (waitKey(1) == 27) exit(0);
	}
};

TestURDFloader testURDFloader("cPATestURDFloader");

int main(int argc, char** argv) try{
	srand(10);
	GameEngine::setGame(&testURDFloader);
	GameEngine::gameInit(argc,argv);
	GameEngine::gameMainLoop();
	return 0;
}
catch (exception &e){
	cout << "Error in cPATestURDFloader::main(): "<<e.what() <<endl;
	return 1;
}
