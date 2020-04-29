/*
 * text.h
 *
 *  Created on: 29 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include <GL/glut.h>
#include "vector3d.h"
#include "solido.h"
class Text: public Solido{
 string text;
public:
 Text(string t="Default text"):Solido(),text(t){}
 Text(const Text &c):Solido(c),text(c.text){}
 Text *clone(){
	 return new Text(*this);
 }
 inline string getText(){return text;}
 inline void setText(string t){text=t;}
 void render(){
	 glPushMatrix();
	  glColor3f(this->getCol().getX(),this->getCol().getY(),this->getCol().getZ());
	  glTranslatef(this->getPos().getX(),this->getPos().getY(),this->getPos().getZ());
	  for(char c:text)
	    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, c );
	 glPopMatrix();

 }
 friend std::ostream &operator << (std::ostream &os, const Text &v);
};
inline std::ostream &operator<<(std::ostream &os, const Text &c){
	os << static_cast<const Solido&>( c );
	os <<",text="<< c.text;
    return os;
}
