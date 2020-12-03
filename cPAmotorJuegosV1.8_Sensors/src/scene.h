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
	const Camera* &getCamera() const {return camera;}
	void           setCamera(const Camera*& camera) {	this->camera = camera;}
	const Stage* &getStage() const {return stage;}
	void          setStage(const Stage*& stage) {this->stage = stage;}
	const View* &getView() const {return view;}
	void         setView(const View*& view) {this->view = view;}
	inline void render(){
		view->render();
	    glLoadIdentity();
		camera->render();
		stage->render();
	}
};


