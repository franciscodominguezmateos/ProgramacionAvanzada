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
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glut.h>
#include "vector3d.h"
//#include "contorno.h"
#include "triangulo.h"
#include "util.h"
#include "material.h"

using namespace std;

class ModeloMaterial: public Solido {
	vector<Vector3D *> vertices;
	vector<Vector3D *> textures;
	vector<Vector3D *> normals;
	vector<Triangulo *> triangulos;
	map<string,Material> materiales;
	float maxX,maxY,maxZ;
	float minX,minY,minZ;
	Vector3D scale;
public:
	ModeloMaterial(){}

	ModeloMaterial(string s){cargar(s);scale=Vector3D(1,1,1);}
	ModeloMaterial(const ModeloMaterial &m);
	ModeloMaterial *clone(){return nullptr;}

	virtual ~ModeloMaterial();

	Triangulo *centrar(Triangulo *t);
	inline float getAncho(){return maxX-minX;}
	inline float getAlto() {return maxY-minY;}
	inline float getProfundo(){return maxZ-minZ;}
	inline void setScale(Vector3D s){scale=s;}
	inline Vector3D getScale(){return scale;}
	void setTexture(Textura* tex){
		for(Triangulo* t:triangulos)
			t->setTextura(tex);
	}
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
		glRotatef(getRot().getX(), 1, 0, 0);
		glRotatef(getRot().getY(), 0, 1, 0);
		glRotatef(getRot().getZ(), 0, 0, 1);
		glScalef(scale.getX(),scale.getY(),scale.getZ());
		/*
		glPushMatrix();
		  glTranslatef(0,getAlto()/2.0,0);
		  glColor3f(1,0,0);
		  glutWireSphere(getAlto()/2.0,10,10);
		glPopMatrix();
		*/
		for(Triangulo *t:triangulos)
			t->render();
		glPopMatrix();
	}

	void cargarMateriales(string &nombreFichero);
	void cargar(string &nombreFichero);

 	Vector3D  *parseVector3D(string &linea);
 	Vector3D  *parseVector2D(string &linea);

	Triangulo *parseTriangulo(string &linea);
	Triangulo *parseTriangulos(string &linea);
	void calculaExtremos(Vector3D *v);
	//inline Contorno *getContorno();

};

#endif /* MODELO_H_ */
