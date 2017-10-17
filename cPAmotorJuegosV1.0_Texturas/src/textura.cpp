/*
 * textura.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: francisco
 */

#include "textura.h"

Textura::Textura(){
}
void Textura::init(){
	glGenTextures(1,&idTextura);
	glBindTexture(GL_TEXTURE_2D, idTextura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Textura::~Textura() {
	// TODO Auto-generated destructor stub
}

