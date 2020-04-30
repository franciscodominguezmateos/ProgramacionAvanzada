/*
 * game_engine.h
 *
 *  Created on: 31 Dec 2019
 *      Author: Francisco Dominguez
 */
#pragma once
#include "game_engine.h"
#include "sky_box.h"

using namespace cv;
using namespace std;

//24/04/2020 this doesn't work yet
void renderString( int x, int y, string ss )
{
  glColor3f( 0.9f, 0.9f, 0.0f );
  //glRasterPos2i( x, y );
  glRasterPos3f( x, y , 0);
  //glWindowPos3f(10, 10, 0);
  for(char c:ss)
    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, c );
}
void drawBitmapText(const char *string) {

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos3f(-1,0.8,-1);
    glColor3f( 0.9f, 0.9f, 0.9f );

    const char *c;
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
class Game{
	string title;
	vector<View*> views;
	vector<Camara*> cameras;
	vector<Stage*> scenes;
	SensorEventProcessor seProcessor;
	SocketMJPEGServer  sms;
	GLSLFBO screen; //default frame buffer object is the screen if init() is not called
	SkyBox *skyBox;
	Mat img;
	double t;  // time in seconds
	double dt; // time increment in seconds
public:
	Game(string t="PAGame default;-P",int port=8881):title(t),seProcessor(port),screen(640,480),t(0),dt(0.1),skyBox(nullptr){}
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
			View* &view=views[i];
			Camara* &cam=cameras[i];
			Stage* &e=scenes[i];
			view->render();
		    glLoadIdentity();
			cam->render();
			//TESTING drawBitmapText
			CamaraTPS* ctps=(CamaraTPS*)cam;
			Solido* s=ctps->getSolido();
			renderString((int)s->getPos().getX(),(int)s->getPos().getY(),title);
			renderString(0,0,title);
			e->render();
			Mat cameraViewMat=posEulerAnglesToTransformationMatrix<float>(-cam->getPos(),cam->getRot());
			skyBox->setCameraView(cameraViewMat);
			skyBox->render();
			drawBitmapText(title.c_str());
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
};
