/*
 * caja_modelo_elastico.h
 *
 *  Created on: 15 Dec 2018
 *      Author: francisco
 */

#ifndef CAJA_MODELO_ELASTICO_H_
#define CAJA_MODELO_ELASTICO_H
#include "caja_elastica.h"

/** This class simulate a PseudoRigidBody over a Model
 *  The center of mass is suppose to be in the center of the model
 */
class CajaModeloElastico:public CajaElastica{
	ModeloMaterial* modelo;
public:
	CajaModeloElastico(ModeloMaterial* m):
		CajaElastica(m->getAncho(),
				     m->getAlto(),
					 m->getProfundo()),
		modelo(m){}
	void render(){
		CajaElastica::render();
		modelo->render();
		Vector3D c=getCenter();
		Vector3D ax=c+getAxisX();
		Vector3D ay=c+getAxisY();
		Vector3D az=c+getAxisZ();
		glLineWidth(2);
		glBegin(GL_LINES);
		  glColor3f(1.0,0,0);
		  glVertex3f(c.getX(),c.getY(),c.getZ());
		  glVertex3f(ax.getX(),ax.getY(),ax.getZ());
		  glColor3f(0,1,0);
		  glVertex3f(c.getX(),c.getY(),c.getZ());
		  glVertex3f(ay.getX(),ay.getY(),ay.getZ());
    	  glColor3f(0,0,1);
		  glVertex3f(c.getX(),c.getY(),c.getZ());
		  glVertex3f(az.getX(),az.getY(),az.getZ());
		glEnd();

	}
	void update(double dt){
		CajaElastica::update(dt);
		modelo->setPos(getCenter());
		modelo->setScale(getScale());
	}
	//     5---1
	//    /   /|
	//   4---0 |
	//   |   | 2
	//   |   |/
	//   7---3
	// They are mean values because they can be deformed
	double getWidth(){
		auto &p=getParticulas();
		double width=0;
		width+=p[4]->getPos().distancia(p[0]->getPos());
		width+=p[5]->getPos().distancia(p[1]->getPos());
		width+=p[6]->getPos().distancia(p[2]->getPos());
		width+=p[7]->getPos().distancia(p[3]->getPos());
		return width/4;
	}
	double getHeight(){
		auto &p=getParticulas();
		double height=0;
		height+=p[4]->getPos().distancia(p[7]->getPos());
		height+=p[0]->getPos().distancia(p[3]->getPos());
		height+=p[5]->getPos().distancia(p[6]->getPos());
		height+=p[1]->getPos().distancia(p[2]->getPos());
		return height/4;
	}
	double getDepth(){
		auto &p=getParticulas();
		double depth=0;
		depth+=p[4]->getPos().distancia(p[5]->getPos());
		depth+=p[0]->getPos().distancia(p[1]->getPos());
		depth+=p[7]->getPos().distancia(p[6]->getPos());
		depth+=p[3]->getPos().distancia(p[2]->getPos());
		return depth/4;
	}
	//     5---1
	//    /   /|
	//   4---0 |
	//   |   | 2
	//   |   |/
	//   7---3
	// They are mean values because they can be deformed
	Vector3D getAxisX(){
		auto &p=getParticulas();
		Vector3D axis;
		axis+=p[0]->getPos()-p[4]->getPos();
		axis+=p[1]->getPos()-p[5]->getPos();
		axis+=p[2]->getPos()-p[6]->getPos();
		axis+=p[3]->getPos()-p[7]->getPos();
		//axis.normalize();
		return axis/4;
	}
	Vector3D getAxisY(){
		auto &p=getParticulas();
		Vector3D axis;
		axis+=p[4]->getPos()-p[7]->getPos();
		axis+=p[0]->getPos()-p[3]->getPos();
		axis+=p[1]->getPos()-p[2]->getPos();
		axis+=p[5]->getPos()-p[6]->getPos();
		//axis.normalize();
		return axis/4;
	}
	Vector3D getAxisZ(){
		auto &p=getParticulas();
		Vector3D axis;
		axis+=p[2]->getPos()-p[3]->getPos();
		axis+=p[6]->getPos()-p[7]->getPos();
		axis+=p[5]->getPos()-p[4]->getPos();
		axis+=p[1]->getPos()-p[0]->getPos();
		//axis.normalize();
		return axis/4;
	}
	// work out scale estimate
	Vector3D getScale(){
		return Vector3D(getWidth()/w,getHeight()/h,getDepth()/d);
	}
};




#endif /* CAJA_MODELO_ELASTICO_H_ */
