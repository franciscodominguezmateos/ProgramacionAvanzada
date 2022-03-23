/*
 * game_standard.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 *  Just a scene with a view, a camera and a stage.
 *  Default mouse and key events to cope with camera
 *  You can inherit and redefine init(), update() and event methods
 *  May it could be better to have a SceneAR?!
 */
#pragma once
#include "game.h"
#include "projection_camera.h"
#include "camera_ar.h"
#include "background_texture.h"
#include "sensor_observer_standard.h"

/**
 * This class is an AR game where you need to provide:
 * a intrinsic matrix K from camera calibration
 * a image as background with setBackgroundImage()
 * a pose from the image with setPose()
 * Image must be undistorted
 */
class GameAR;
using GameARPtr=GameAR*;
class GameAR: public Game,public SensorObserverStandard {
	CameraARPtr ca;
	BackgroundTexturePtr pBgt;
public:
	GameAR(string title,Mat K,CameraARPtr camera=new CameraAR()):Game(title),ca(camera){
		addScene(new View(0,0,1,1,new ProjectionCamera(K)),ca,new Stage());
		Scene&  scene =getScene();
		pBgt=new BackgroundTexture();
		scene.setBackgroundPtr(pBgt);
		addSensorObserver(this);
	}
	void init(){pBgt->init();}
	void setBackgroundImage(Mat& img){pBgt->setImage(img);}
	void setPose(Mat& rvec,Mat& tvec){ca->setPose(rvec,tvec);}
	void add(SolidPtr s){getStage().add(s);}
	virtual void mouseMoved(int x, int y){}
	virtual void mousePress(int button, int state, int x, int y){}
	virtual void keyPressed(char key,int x,int y){}
	inline Scene&    getScene()  {return *Game::getScene(0);}
	inline Stage&    getStage()  {return getScene().getStage();}
	inline View&     getView()   {return getScene().getView();}
	inline CameraAR& getCamera() {return *ca;}
	virtual ~GameAR(){}
};

