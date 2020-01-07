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
	ModeloMaterial* getModelo(){return modelo;}
	Vector3D getRot(){
		Vector3D ax=getAxisX();
		double y=atan2(ax.getX(),ax.getZ());
		double degY=rad2deg(y);
		//cout << "degY="<<degY<<"ax="<<ax<<endl;
		return Vector3D(0,degY,0);
	}
	void setRotation(){
		float projectionMat[16];
		Vector3D ax=getAxisX();
		//Vector3D ay=getAxisY();
		Vector3D az=getAxisZ();
		Vector3D ay=ax.X(az);
		projectionMat[ 0]  = ax.getX();
		projectionMat[ 1]  = ax.getY();
		projectionMat[ 2]  = ax.getZ();
		projectionMat[ 3]  = 0;

		projectionMat[ 4]  = ay.getX();
		projectionMat[ 5]  = ay.getY();
		projectionMat[ 6]  = ay.getZ();
		projectionMat[ 7]  = 0;

		projectionMat[ 8]  = az.getX();
		projectionMat[ 9]  = az.getY();
		projectionMat[10]  = az.getZ();
		projectionMat[11]  = 0;

		projectionMat[12]  = 0;
		projectionMat[13]  = 0;
		projectionMat[14]  = 0;
		projectionMat[15]  = 1;
		glMultMatrixf(projectionMat);
	}
	void render(){
		//CajaElastica::render();
		glPushMatrix();
		//modelo->render();
		Vector3D p=getCenter();//this->getPos();
		glTranslatef(p.getX(),p.getY(),p.getZ());
		//cout << "p=" << p << endl;
		//glRotatef(getRot().getX(), 1, 0, 0);
		//glRotatef(getRot().getY(), 0, 1, 0);
		//glRotatef(getRot().getZ(), 0, 0, 1);
		setRotation();
		glScalef(getScale().getX(),getScale().getY(),getScale().getZ());
		/*
		glPushMatrix();
		  glTranslatef(0,getAlto()/2.0,0);
		  glColor3f(1,0,0);
		  glutWireSphere(getAlto()/2.0,10,10);
		glPopMatrix();
		*/
		for(Triangulo *t:modelo->getTriangulos())
			t->render();
		// Draw local object axis
		glPopMatrix();
		Vector3D c=getCenter();
		Vector3D ax=c+getAxisX()*getWidth();
		Vector3D ay=c+getAxisY()*getHeight();
		Vector3D az=c+getAxisZ()*getDepth();
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
		glPopMatrix();
	}
	void update(double dt){
		CajaElastica::update(dt);
		setPos(getCenter());
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
		axis=axis/4;
		axis.normalize();
		return axis;
	}
	Vector3D getAxisY(){
		auto &p=getParticulas();
		Vector3D axis;
		axis+=p[4]->getPos()-p[7]->getPos();
		axis+=p[0]->getPos()-p[3]->getPos();
		axis+=p[1]->getPos()-p[2]->getPos();
		axis+=p[5]->getPos()-p[6]->getPos();
		axis=axis/4;
		axis.normalize();
		return axis;
	}
	Vector3D getAxisZ(){
		auto &p=getParticulas();
		Vector3D axis;
		axis+=p[2]->getPos()-p[3]->getPos();
		axis+=p[6]->getPos()-p[7]->getPos();
		axis+=p[5]->getPos()-p[4]->getPos();
		axis+=p[1]->getPos()-p[0]->getPos();
		axis=axis/4;
		axis.normalize();
		return axis;
	}
	// work out scale estimate
	Vector3D getScale(){
		return Vector3D(getWidth()/w,getHeight()/h,getDepth()/d);
	}
};

// Z axis is front direction vector 0-1 or 2-3
// 0473 is front
// 1265 is rear
//     5---1
//    /   /|
//   4---0 |
//   |   | 2
//   |   |/
//   7---3
class MarioKart: public CajaModeloElastico{
public:
	MarioKart():CajaModeloElastico(new ModeloMaterial("mk_kart.obj")){
		//getModelo()->doScale(2);
		getModelo()->doCenter();
	}
	void update(double dt){
		//acumulaFuerzasElasticas();
		//Vector3D ax=getAxisX();
		//Vector3D ay=getAxisX();
		//Vector3D az=getAxisX();
		//const vector<Solido *> &particulas=getParticulas();
		CajaModeloElastico::update(dt);
	}

};



#endif /* CAJA_MODELO_ELASTICO_H_ */
