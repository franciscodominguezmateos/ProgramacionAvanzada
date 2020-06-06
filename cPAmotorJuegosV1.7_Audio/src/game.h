/*
 * game_engine.h
 *
 *  Created on: 31 Dec 2019
 *      Author: Francisco Dominguez
 */
#pragma once
#include "game_engine.h"
#include "sky_box.h"
#include <AL/alut.h>

using namespace cv;
using namespace std;

class Game{
	string title;
	vector<View*> views;
	vector<Camara*> cameras;
	vector<Stage*> scenes;
	vector<GLSLShaderProgram*> shaders;
	SensorEventProcessor seProcessor;
	SocketMJPEGServer  sms;
	GLSLFBO screen; //default frame buffer object is the screen if init() is not called
	SkyBox *skyBox;
	Mat img;
	double t;  // time in seconds
	double dt; // time increment in seconds
	//Sound
	vector<ALuint>     audioBuffers;
	map<string,ALuint> audioSources;
public:
	Game(string t="PAGame default;-P",int port=8881):title(t),seProcessor(port),screen(640,480),skyBox(nullptr),t(0),dt(0.1){}
	void addShader(GLSLShaderProgram* sp){shaders.push_back(sp);}
	void addStage(View* v,Camara* cam,Stage* e){
		if(skyBox==nullptr){
			skyBox=new SkyBox();
			Mat m=v->getProyeccion()->getMat();
			skyBox->setProjection(m);
		}
		views.push_back(v);
		cameras.push_back(cam);
		scenes.push_back(e);
	}
	void addSensorObserver(SensorObserver* so){seProcessor.addSensorObserver(so);}
	string &getTitle(){return title;}
	Mat &getImg(){return img;}
	virtual void onDisplay(){
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		for(unsigned int i=0;i<views.size();i++){
			View*   &view=views[i];
			Camara* &cam=cameras[i];
			Stage*  &e=scenes[i];
			Solido* s=cam;//->getSolido();
			Mat cameraViewMat=cam->getMat();
			Mat projection=view->getProyeccion()->getMat();
			for(GLSLShaderProgram* &sp:shaders){
				GLSLShaderProgram &spAnimation=*sp;
				spAnimation.start();
				spAnimation["projection"]=projection;
				spAnimation["cameraView"]=cameraViewMat;
				spAnimation.stop();
			}
			view->render();
		    glLoadIdentity();
			cam->render();
			e->render();
			skyBox->setCameraView(cameraViewMat);
			skyBox->render();
		}
		glutSwapBuffers();
		img=screen.toOpenCV();
		sms.setImg(img);
	}
	virtual void onIdle(){
		 t+=dt;
		 for(Stage* &e:scenes)
			 e->update(dt);
		 onDisplay();
	}
	virtual void onReshape(int width,int height){
		screen.setWidth(width);
		screen.setHeight(height);
		for(View* &view:views)
			view->reshape(width,height);
	}
	virtual void onKeyPressed(unsigned char key,int x,int y){
		SensorEventData e("device=keyboard&event=KeyPressed&id=0");
		e.add("key",key);
		e.add("x",x);
		e.add("y",y);
		seProcessor.dispatchSensorObserverEvent(e);
	}
	virtual void onMouseMoved(int x, int y){
		SensorEventData e("device=mouse&event=MouseMoved&id=0");
		e.add("x",x);
		e.add("y",y);
		seProcessor.dispatchSensorObserverEvent(e);
	}
	virtual void onMousePress(int button, int state, int x, int y){
		SensorEventData e("device=mouse&event=MousePress&id=0");
		e.add("button",button);
		e.add("state",state);
		e.add("x",x);
		e.add("y",y);
		seProcessor.dispatchSensorObserverEvent(e);
	}
	// Sound methods
	void addWav(string name,string fileName){
		   ALenum format;
		   ALsizei size;
		   ALvoid* data;
		   ALsizei freq;
		   ALboolean loop;
		   ALuint buffer, source;
		   // Load any of your favourite wav song here
		   // Load wav data into a buffer.
		   alGenBuffers(1, &buffer);
		   alutLoadWAVFile((ALbyte*)fileName.c_str(), &format, &data, &size, &freq, &loop);
		   alBufferData(buffer, format, data, size, freq);
		   alutUnloadWAV(format, data, size, freq);
		   alGenSources (1, &source);
		   alSourcei (source, AL_BUFFER, buffer);
		   audioBuffers.push_back(buffer);
		   audioSources[name]=source;
	}
	void playAudio(string name){
		ALuint source;
		source=audioSources[name];
		alSourcePlay(source);
	}

};
