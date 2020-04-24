/*
 * game_engine.h
 *
 *  Created on: 31 Dec 2019
 *      Author: Francisco Dominguez
 */
#pragma once
#include <string>
#include "shader.h"
#include "vector3d.h"
#include "camara.h"
#include "view.h"
#include "escena.h"
//Declaration needed in next .h
//Mat global_img;
#include "sensors.h"

using namespace cv;
using namespace std;

Mat opengl_default_frame_to_opencv() {
	//cv::Mat img(480, 640*2, CV_8UC3);
    cv::Mat img(1080, 1920, CV_8UC3);
    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3)?1:4);
    glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
    glReadPixels(0, 0, img.cols, img.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, img.data);
    cv::Mat flipped(img);
    cv::flip(img, flipped, 0);
    return img;
}
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
	vector<Escena*> scenes;
	SensorCGIProcessor sprocessor;
	SocketMJPEGServer  sms;
	Mat img;
	double t;  // time in seconds
	double dt; // time increment in seconds
public:
	Game(string t="PAGame default;-P",int port=8881):title(t),sprocessor(port),t(0),dt(0.1){}
	void addStage(View* v,Camara* cam,Escena* e){
		views.push_back(v);
		cameras.push_back(cam);
		scenes.push_back(e);
	}
	void addSensorObserver(SensorObserver* so){sprocessor.addSensorObserver(so);}
	string &getTitle(){return title;}
	Mat &getImg(){return img;}
	virtual void onDisplay(){
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		for(unsigned int i=0;i<views.size();i++){
			View* &view=views[i];
			Camara* &cam=cameras[i];
			Escena* &e=scenes[i];
			view->render();
		    glLoadIdentity();
			cam->render();
			CamaraTPS* ctps=(CamaraTPS*)cam;
			Solido* s=ctps->getSolido();
			renderString((int)s->getPos().getX(),(int)s->getPos().getY(),title);
			renderString(0,0,title);
			e->render();
			drawBitmapText(title.c_str());
		}
		glutSwapBuffers();
		img=opengl_default_frame_to_opencv();
		sms.setImg(img);
	}
	virtual void onIdle(){
		 t+=dt;
		 for(Escena* &e:scenes)
			 e->update(dt);
		 onDisplay();
	}
	virtual void onReshape(int width,int height){
		for(View* &view:views)
			view->reshape(width,height);
	}
	virtual void onKeyPressed(unsigned char key,int x,int y){
		CGI e("device=keyboard&event=KeyPressed&id=0");
		e.add("key",key);
		e.add("x",x);
		e.add("y",y);
		sprocessor.dispatchSensorObserverEvent(e);
	}
	virtual void onMouseMoved(int x, int y){
		CGI e("device=mouse&event=MouseMoved&id=0");
		e.add("x",x);
		e.add("y",y);
		sprocessor.dispatchSensorObserverEvent(e);
	}
	virtual void onMousePress(int button, int state, int x, int y){
		CGI e("device=mouse&event=MousePress&id=0");
		e.add("button",button);
		e.add("state",state);
		e.add("x",x);
		e.add("y",y);
		sprocessor.dispatchSensorObserverEvent(e);
	}
};
class GameEngine {
	static Vector3D windowSize,windowPosition;
	static string windowTitle;
	static bool fullScreen;
	static Game* g;
public:
	static void setGame(Game* gm){g=gm;}
	// Callback glut functions
	static void cbDisplay(){
		if(g!=nullptr) g->onDisplay();
	}
	static void cbIdle(){
		if(g!=nullptr) g->onIdle();
	}
	static void cbReshape(int width,int height){
		if(g!=nullptr) g->onReshape(width,height);
	}
	static void cbKeyPressed(unsigned char key,int x,int y){
		if(g!=nullptr) g->onKeyPressed(key,x,y);
	}
	static void cbMouseMoved(int x, int y){
		if(g!=nullptr) g->onMouseMoved(x,y);
	}
	static void cbMousePress(int button, int state, int x, int y){
		if(g!=nullptr) g->onMousePress(button,state,x,y);
	}
	// GLUT abstraction
	static void gameInit(int argc, char** argv){
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		glutInitWindowSize(640*2,480);
		glutInitWindowPosition(300,300);
		if(g==nullptr)
			glutCreateWindow("No game object set :-P");
		else
			glutCreateWindow(g->getTitle().c_str());
		glEnable(GL_DEPTH_TEST);
	    glEnable(GL_LIGHTING);
	    glEnable(GL_LIGHT0);
	    glEnable(GL_LIGHT1);
	    glEnable(GL_COLOR_MATERIAL);
	    //Very important to activate transparency or alpha channel
        glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	static void gameMainLoop(){
		 glutDisplayFunc(cbDisplay);
		 glutIdleFunc(cbIdle);
		 glutReshapeFunc(cbReshape);
		 glutKeyboardFunc(cbKeyPressed);
		 glutMotionFunc(&cbMouseMoved);
		 glutMouseFunc(&cbMousePress);
		 if(fullScreen) glutFullScreen();
		 glutMainLoop();
	}
};
Game* GameEngine::g=nullptr;
bool GameEngine::fullScreen=false;
