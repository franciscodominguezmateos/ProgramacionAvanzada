/*
 * game_engine.h
 *
 *  Created on: 31 Dec 2019
 *      Author: Francisco Dominguez
 */
#pragma once
#include "game_engine.h"
#include "scene.h"
#include "sensors.h"
#include "sky_box.h"
#include <AL/alut.h>

using namespace cv;
using namespace std;

class Game{
	string title;
	vector<ScenePtr> scenes;
	vector<GLSLShaderProgramPtr> shaders;
	SensorEventProcessor seProcessor;
	SocketMJPEGServer  smserv;
	GLSLFBO screen; //default frame buffer object is the screen if init() is not called
	SkyBox *skyBox;//this should go inside scene
	Mat img;
	double t;  // time in seconds
	double dt; // time increment in seconds
	//Sound
	vector<ALuint>     audioBuffers;
	map<string,ALuint> audioSources;
public:
	Game(string t="PAGame default;-P",int port=8881):title(t),seProcessor(port),screen(640,480),skyBox(nullptr),t(0),dt(0.1){}
	virtual void init(){}
	virtual void update(double dt){}
	double getTime(){return t;}
	double getDTime(){return dt;}
	GLSLFBO &getScreen(){return screen;}
	void   setDTime(double d){dt=d;}
	vector<ScenePtr> &getScenes(){return scenes;}
	ScenePtr &getScene(int i){return scenes[i];}
	void addShader(GLSLShaderProgramPtr sp){shaders.push_back(sp);}
	void addScene(ScenePtr &scene){scenes.push_back(scene);}
	void addScene(ViewPtr v,CameraPtr cam,StagePtr s){
		//if(skyBox==nullptr){
			//skyBox=new SkyBox();
			//Mat m=v->getProyeccion()->getMat();
			//skyBox->setProjection(m);
		//}
		Scene* e=new Scene(v,cam,s);
		scenes.push_back(e);
	}
	void addSensorObserver(SensorObserver* so){seProcessor.addSensorObserver(so);}
	string &getTitle(){return title;}
	Mat &getImg(){return img;}
	void setShaderProgramTransformations(ScenePtr &scene){
		ViewPtr   &view =scene->getViewPtr();
		CameraPtr &cam  =scene->getCameraPtr();
		Mat cameraViewMat=cam->getMat();
		Mat projection=view->getProjection()->getMat();
		for(GLSLShaderProgramPtr &pSp:shaders){
			GLSLShaderProgram &sp=*pSp;
			sp.start();
			sp["projection"]=projection;
			sp["cameraView"]=cameraViewMat;
			sp.stop();
		}
	}
	virtual void onDisplay(){
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		for(Scene* &scene:scenes){
			setShaderProgramTransformations(scene);
			scene->render();
			//skyBox->setCameraView(scene->getCamera()->getMat());
			//skyBox->render();
		}
		glutSwapBuffers();
		img=screen.toOpenCV();
		smserv.setImg(img);
		//this snippet shows the depth image
        /*
		double min, max;
		Mat fboDepth=screen.toOpenCVdepth();
		minMaxLoc(fboDepth, &min, &max);
		cout << "maxdfbo="<<min<<","<<max<<endl;
		imshow("holad",fboDepth);
		*/
	}
	virtual void onIdle(){
		 t+=dt;
		 update(dt);
		 for(Scene* &e:scenes)
			 e->getStagePtr()->update(dt);
		 onDisplay();
	}
	virtual void onReshape(int width,int height){
		screen.setWidth(width);
		screen.setHeight(height);
		for(Scene* &e:scenes)
			e->getViewPtr()->reshape(width,height);
	}
	//Sensor events: Keyboard and mouse
	virtual void onKeyPressed(unsigned char key,int x,int y){
		SensorEventData e("device=keyboard&event=KeyPressed&id=0");
		//Press ESC to exit :-)
		if(key==27) exit(0);
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
