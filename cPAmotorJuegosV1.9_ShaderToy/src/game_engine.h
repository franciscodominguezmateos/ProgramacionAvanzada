/*
 * game_engine.h
 *
 *  Created on: 31 Dec 2019
 *      Author: Francisco Dominguez
 * - 29/12/2020 Added campling to false but it doesn't seem to work
 */
#pragma once
//First thing to include in order to activate GLSL Shaders later on
#include "shader.h"
#include <string>

#include "vector3d.h"
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
	static void setWindowSize(double w,double h){setWindowSize(Vector3D(w,h));}
	static void setWindowSize(Vector3D v){windowSize=v;}
	static void setWindowPosition(double x,double y){setWindowSize(Vector3D(x,y));}
	static void setWindowPosition(Vector3D v){windowPosition=v;}
	// Callback glut functions
	static void cbDisplay()                                      {if(g!=nullptr) g->onDisplay();}
	static void cbIdle()                                         {if(g!=nullptr) g->onIdle();}
	static void cbReshape(int width,int height)                  {if(g!=nullptr) g->onReshape(width,height);}
	static void cbKeyPressed(unsigned char key,int x,int y)      {if(g!=nullptr) g->onKeyPressed(key,x,y);}
	static void cbMouseMoved(int x, int y)                       {if(g!=nullptr) g->onMouseMoved(x,y);}
	static void cbMousePress(int button, int state, int x, int y){if(g!=nullptr) g->onMousePress(button,state,x,y);}
	// GLUT abstraction
	static void gameInit(int argc, char** argv){
	    alutInit (&argc, argv);
		glutInit(&argc,argv);
		//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
		glutInitWindowSize(windowSize.getX(),windowSize.getY());
		glutInitWindowPosition(windowPosition.getX(),windowPosition.getY());
		if(g==nullptr)
			glutCreateWindow("No game object set :-P");
		else
			glutCreateWindow(g->getTitle().c_str());
		glEnable(GL_DEPTH_TEST);
	    glEnable(GL_LIGHTING);
	    glEnable(GL_LIGHT0);
	    glEnable(GL_LIGHT1);
	    glEnable(GL_COLOR_MATERIAL);
	    //This is in order to get no clamped info from fragment shaders in FBO
	    //but it doesn't seem to work FBO read data is clamped¿?¿?¿?¿?¿?¿?
	    //glClampColor(GL_CLAMP_READ_COLOR,     GL_FALSE);
	    //glClampColor(GL_CLAMP_VERTEX_COLOR,   GL_FALSE);
	    //glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
	    //Very important to activate transparency or alpha channel
        glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    if(g!=nullptr)
	    	g->init();
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
	static void exit(int i){
		 alutExit();
		 exit(i);
	}
};
Game* GameEngine::g=nullptr;
bool GameEngine::fullScreen=false;
Vector3D GameEngine::windowSize(640,480);
Vector3D GameEngine::windowPosition(300,300);
