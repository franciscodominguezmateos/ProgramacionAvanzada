/*
 * modelo.h
 *
 *  Created on: Nov 12, 2015
 *      Author: francisco
 */

#ifndef MODELO_H_
#define MODELO_H_
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glut.h>
#include "vector3d.h"
//#include "contorno.h"
#include "triangulo.h"

class Modelo: public Solido {
	vector<Vector3D *> vertices;
	vector<Vector3D *> textures;
	vector<Vector3D *> normals;
	vector<Triangulo *> triangulos;
	float maxX,maxY,maxZ;
	float minX,minY,minZ;
public:
	Modelo(){}

	Modelo(string s){cargar(s);}
	Modelo(const Modelo &m);
	Modelo *clone(){}

	virtual ~Modelo();

	Triangulo *centrar(Triangulo *t);
	inline float getAncho(){return maxX-minX;}
	inline float getAlto() {return maxY-minY;}
	inline float getProfundo(){return maxZ-minZ;}
	inline void addTriangulo(Triangulo *t){
		triangulos.push_back(t);
	}

	inline void setColor(Vector3D c){
		for(Triangulo *t:triangulos){
			t->setCol(c);
		}
	}
	inline void render(){
		Vector3D p=this->getPos();
		glPushMatrix();
		glTranslatef(p.getX(),p.getY(),p.getZ());
		glTranslatef(0,getAlto()/2.0,0);
		//glColor3f(1,0,0);
		//glutWireSphere(getAlto()/2.0,10,10);
		for(Triangulo *t:triangulos)
			t->render();
		glPopMatrix();
	}

	void cargar(string &nombreFichero);

 	Vector3D  *parseVector3D(string &linea);
 	Vector3D  *parseVector2D(string &linea);

	Triangulo *parseTriangulo(string &linea);
	void calculaExtremos(Vector3D *v);
	//inline Contorno *getContorno();

};

#endif /* MODELO_H_ */
