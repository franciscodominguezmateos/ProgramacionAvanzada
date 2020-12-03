/*
 * game_standard.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
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
		view(nullptr),
		camera(new Camera()),
		stage(new Stage()),mx(0),my(0){
		ProyeccionPerspectiva* proyeccion=new ProyeccionPerspectiva();
		view=new View(0.0,0.0,1,1,&proyeccion);
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
	}
	virtual void keyPressed(unsigned char key,int x,int y){}
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
};

