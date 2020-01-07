/*
 * game_engine.h
 *
 *  Created on: 31 Dec 2019
 *      Author: Francisco Dominguez
 */

#ifndef GAME_ENGINE_H_
#define GAME_ENGINE_H_
#include <string>
#include <GL/glut.h>
#include "vector3d.h"

void gameInit(int argc, char** argv){
	 glutInit(&argc,argv);
	 //glutInitDisplayMode(GLUT_SINGLE);
	 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	 glutInitWindowSize(640*2,480);
	 glutInitWindowPosition(300,300);
	 glutCreateWindow("Mario Kart Virtual Reality First Person :D");
}
void gameMainLoop(){
	 glutDisplayFunc(displayMe);
	 glutIdleFunc(idle);
	 glutReshapeFunc(reshape);
	 glutKeyboardFunc(keyPressed);
	 glutMotionFunc(&mouseMoved);
	 glutMouseFunc(&mousePress);
	 //glutFullScreen();
	 glutMainLoop();
}

class GameEngine {
	Vector3D windowSize,windowPosition;
	string windowTitle;
	bool fullScreen;
public:
	GameEngine(){}
	virtual ~GameEngine(){}
};

#endif /* GAME_ENGINE_H_ */
