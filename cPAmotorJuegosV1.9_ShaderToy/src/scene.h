/*
 * scene.h
 *
 *  Created on: 3 Dec 2020
 *      Author: Francisco Dominguez
 *  A scene is:
 *  - One View
 *  - One Camera
 *  - One Stage
 *  - ¿One Background like ¿sky_box??
 *  - ¿One Foreground to be done?
 *  Lights are first think to add in a stage
 *  It is the owner of the object
 */

#pragma once
#include "view.h"
#include "camera.h"
#include "stage.h"
class Scene;
using ScenePtr=Scene*;

class Scene {
	ViewPtr   view;
	CameraPtr camera;
	StagePtr  stage;
public:
	Scene(ViewPtr view,CameraPtr camera,StagePtr stage):view(view),camera(camera),stage(stage){}
	virtual ~Scene(){
		delete view;
		delete camera;
		delete stage;
	}
	CameraPtr &getCameraPtr() {return camera;}
	Camera&    getCamera(){return *camera;}
	void       setCameraPtr(CameraPtr camera) {this->camera = camera;}
	StagePtr  &getStagePtr()  {return stage;}
	Stage&     getStage(){return *stage;}
	void       setStagePtr(StagePtr stage) {this->stage = stage;}
	ViewPtr   &getViewPtr()   {return view;}
	View&      getView(){return *view;}
	void       setViewPtr(ViewPtr view) {this->view = view;}
	virtual void render(){
		view->render();
	    glLoadIdentity();
		camera->render();
		stage->render();
	}
};


