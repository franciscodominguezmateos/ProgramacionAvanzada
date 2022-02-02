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
#include "sensor_observer_standard.h"

class GameStandard: public Game,public SensorObserverStandard {
	int mx,my;
public:
	GameStandard(string title,CameraPtr camera=nullptr):	Game(title),mx(0),my(0){
		if(camera==nullptr)
			addScene(new View(),new Camera(),new Stage());
		else
			addScene(new View(),camera,new Stage());
		addSensorObserver(this);
		getCamera().setPos(Vector3D(0,0,10));
	}
	void add(SolidPtr s){getStage().add(s);}
	virtual void mouseMoved(int x, int y){
	    if (mx>=0 && my>=0) {
	    	Vector3D r=getCamera().getRot()+Vector3D(y-my,x-mx,0);
	    	getCamera().setRot(r);
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
	    	Vector3D pos=getCamera().getPos();
	    	pos=pos*0.9;
	    	getCamera().setPos(pos);
	    }
	    if (button==4){
	    	Vector3D pos=getCamera().getPos();
	    	pos=pos*1.1;
	    	getCamera().setPos(pos);
	    }
	}
	virtual void keyPressed(char key,int x,int y){
		Vector3D v;
		 switch(key){
		 case 'p':
			 v=getCamera().getLookAtPos()+Vector3D(0.05,0,0);
			 getCamera().setLookAtPos(v);
			 break;
		 case 'o':
			 v=getCamera().getLookAtPos()+Vector3D(-0.05,0,0);
			 getCamera().setLookAtPos(v);
			 break;
		 case 'q':
			 v=getCamera().getLookAtPos()+Vector3D(0,0,-0.05);
			 getCamera().setLookAtPos(v);
			 break;
		 case 'a':
			 v=getCamera().getLookAtPos()+Vector3D(0,0,0.05);
			 getCamera().setLookAtPos(v);
			 break;
		 case 't':
			 v=getCamera().getLookAtPos()+Vector3D(0, 0.05);
			 getCamera().setLookAtPos(v);
			 break;
		 case 'g':
			 v=getCamera().getLookAtPos()+Vector3D(0,-0.05);
			 getCamera().setLookAtPos(v);
			 break;
		 }
	}
	virtual ~GameStandard(){}
	inline Camera& getCamera() {return getScene(0)->getCamera();}
	inline Stage&  getStage()  {return getScene(0)->getStage();}
	inline View&   getView()   {return getScene(0)->getView();}
};

