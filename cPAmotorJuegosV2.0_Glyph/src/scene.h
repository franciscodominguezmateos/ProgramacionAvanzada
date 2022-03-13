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
 *  13/03/2022
 *  added background and skybox they should be mutualexclusive
 */

#pragma once
#include "view.h"
#include "camera.h"
#include "stage.h"
#include "background_texture.h"
#include "background_skybox.h"

class Scene;
using ScenePtr=Scene*;
class Scene {
	ViewPtr   view;
	CameraPtr camera;
	StagePtr  stage;
	SkyBoxPtr skyBox;
	BackgroundPtr background;
public:
	Scene(ViewPtr view,CameraPtr camera,StagePtr stage):
		view(view),camera(camera),stage(stage),skyBox(nullptr),background(nullptr){}
	virtual ~Scene(){
		delete view;
		delete camera;
		delete stage;
		if(skyBox!=nullptr) delete skyBox;
		if(background!=nullptr) delete background;
	}
	CameraPtr &getCameraPtr() {return  camera;}
	Camera&    getCamera()    {return *camera;}
	void       setCameraPtr(CameraPtr camera) {this->camera = camera;}
	StagePtr  &getStagePtr()  {return  stage;}
	Stage&     getStage()     {return *stage;}
	void       setStagePtr(StagePtr stage) {this->stage = stage;}
	ViewPtr   &getViewPtr()   {return  view;}
	View&      getView()      {return *view;}
	void       setViewPtr(ViewPtr view) {this->view = view;}
	BackgroundPtr   &getBackgroundPtr()   {return  background;}
	Background&      getBackground()      {return *background;}
	void       setBackgroundPtr(BackgroundPtr background) {this->background = background;}
	SkyBoxPtr  getSkyBoxPtr() {return  skyBox;}
	SkyBox&    getSkyBox()    {return *skyBox;}
	void       setSkyBoxPtr(SkyBoxPtr sbp){
			skyBox=sbp;
			Mat m=view->getProjection()->getMat();
			skyBox->setProjection(m);
	}
	virtual void render(){
		if(background!=nullptr) background->render();
		view->render();
	    glLoadIdentity();
		camera->render();
		stage->render();
		if(skyBox!=nullptr){skyBox->setCameraView(camera->getMat());skyBox->render();}
	}
};


