/*
 * game_standard.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 *  Just a scene with a view, a camera and a stage.
 *  Default mouse and key events to cope with camera
 *  You can inherit and redefine init(), update() and event methods
 */

#pragma once
#include "game.h"

class GameStandard: public Game,public SensorObserver {
	View*   view;
	Camera* camera;
	Stage*  stage;
	int mx,my;
public:
	GameStandard(string title):
		Game(title),
		view  (new View()),
		camera(new Camera()),
		stage (new Stage()),mx(0),my(0){
		camera->setPos(Vector3D(0,0,10));
		addScene(view,camera,stage);
		addSensorObserver(this);
	}
	void add(Solid* s){stage->add(s);}
	virtual void mouseMoved(int x, int y){
	    if (mx>=0 && my>=0) {
	    	Vector3D r=camera->getRot()+Vector3D(y-my,x-mx,0);
	    	camera->setRot(r);
	    }
	    mx = x;
	    my = y;
	}
	virtual void mousePress(int button, int state, int x, int y){
	    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	        mx = x;
	        my = y;
	    }
	    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
	        mx = -1;
	        my = -1;
	    }
	    if (button==3){
	    	Vector3D pos=camera->getPos();
	    	pos=pos*0.9;
	    	camera->setPos(pos);
	    }
	    if (button==4){
	    	Vector3D pos=camera->getPos();
	    	pos=pos*1.1;
	    	camera->setPos(pos);
	    }
	}
	virtual void keyPressed(char key,int x,int y){
		Vector3D v;
		 switch(key){
		 case 'p':
			 v=camera->getLookAtPos()+Vector3D(0.01,0,0);
			 camera->setLookAtPos(v);
			 break;
		 case 'o':
			 v=camera->getLookAtPos()+Vector3D(-0.01,0,0);
			 camera->setLookAtPos(v);
			 break;
		 case 'q':
			 v=camera->getLookAtPos()+Vector3D(0,0,-0.01);
			 camera->setLookAtPos(v);
			 break;
		 case 'a':
			 v=camera->getLookAtPos()+Vector3D(0,0,0.01);
			 camera->setLookAtPos(v);
			 break;
		 case 't':
			 v=camera->getLookAtPos()+Vector3D(0, 0.01);
			 camera->setLookAtPos(v);
			 break;
		 case 'g':
			 v=camera->getLookAtPos()+Vector3D(0,-0.01);
			 camera->setLookAtPos(v);
			 break;
		 }
	}
	virtual void onSensorEvent(SensorEventData &e){
		if(e["device"]=="mouse"){
			if(e["event"]=="MouseMoved")
				mouseMoved(e.getInt("x"),e.getInt("y"));
			if(e["event"]=="MousePress")
				mousePress(e.getInt("button"),e.getInt("state"),e.getInt("x"),e.getInt("y"));
		}
		if(e["device"]=="keyboard")
			keyPressed(e.getChar("key"),e.getInt("x"),e.getInt("y"));
	}
	virtual ~GameStandard(){}
	Camera* &getCamera() {return camera;}
	void setCamera(Camera *&camera) {	this->camera = camera;}
	Stage*& getStage() {return stage;}
	void setStage(Stage *&stage) {this->stage = stage;}
	View*& getView() {return view;}
	void setView(View *&view) {this->view = view;}
};

