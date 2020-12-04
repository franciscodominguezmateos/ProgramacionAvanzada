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
 *  It is the owner of the object
 */

#pragma once
#include "view.h"
#include "camera.h"
#include "stage.h"

class Scene {
	View*   view;
	Camera* camera;
	Stage*  stage;
public:
	Scene(View* view,Camera* camera,Stage* stage):view(view),camera(camera),stage(stage){}
	virtual ~Scene(){
		delete view;
		delete camera;
		delete stage;
	}
	Camera* &getCamera() {return camera;}
	void     setCamera(Camera* camera) {this->camera = camera;}
	Stage* &getStage() {return stage;}
	void    setStage(Stage* stage) {this->stage = stage;}
	View* &getView() {return view;}
	void   getView(View* view) {this->view = view;}
	inline void render(){
		view->render();
	    glLoadIdentity();
		camera->render();
		stage->render();
	}
};


