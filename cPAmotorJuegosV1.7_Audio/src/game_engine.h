/*
 * game_engine.h
 *
 *  Created on: 31 Dec 2019
 *      Author: Francisco Dominguez
 */
#pragma once
//First thing to include in order to activate GLSL Shaders later on
#include "shader.h"
#include <string>

#include "camera.h"
#include "shader.h"
#include "vector3d.h"
#include "view.h"
#include "sensors.h"
#include "stage.h"
#include "game.h"

using namespace cv;
using namespace std;

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
	    alutInit (&argc, argv);
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
		 alutExit ();
	}
};
Game* GameEngine::g=nullptr;
bool GameEngine::fullScreen=false;
