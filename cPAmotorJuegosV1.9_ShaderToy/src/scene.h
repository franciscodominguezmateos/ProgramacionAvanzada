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
	CameraPtr &getCamera() {return camera;}
	void       setCamera(CameraPtr camera) {this->camera = camera;}
	StagePtr  &getStage()  {return stage;}
	void       setStage(StagePtr stage) {this->stage = stage;}
	ViewPtr   &getView()   {return view;}
	void       setView(ViewPtr view) {this->view = view;}
	inline void render(){
		view->render();
	    glLoadIdentity();
		camera->render();
		stage->render();
	}
};


