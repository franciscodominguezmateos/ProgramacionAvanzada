/*
 * solid_articulated.h
 *
 *  Created on: 2 Apr 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include <vector>
#include <map>
#include <string>
#include "solido.h"
#include "model_mesh_articulated.h"

class SolidArticulated: public Solido{
	Joint jointsRoot;
	vector<string>  jointNames;
	map<string,int> jointIdx;
	//local pose transform by name
	SkeletonPose currentPose;
	//global pose transforms
	vector<Mat> jointTransforms;
	float ax=1,ay=1,az=1;
public:
	float L;
	vector<string> &getJointNames(){return jointNames;}
	void            setJointNames(vector<string> &jn){
		jointNames=jn;
		jointTransforms=vector<Mat>(jn.size());
		for(unsigned int i=0;i<jn.size();i++) jointIdx[jn[i]]=i;
	}
	SkeletonPose &getPose(){return currentPose;}
	virtual void          setPose(SkeletonPose &cp){
		currentPose=cp;
		getJointsRoot().applyPose(currentPose);
		getJointsRoot().fillJointTransforms(jointTransforms);
	}
	void          setJointsRoot(Joint &j){jointsRoot=j;}
	inline Joint &getJointsRoot() {return jointsRoot;}
	void initJointTransforms(int i){jointTransforms=vector<Mat>(i);}
	vector<Mat> &getJointTransforms(){
		return jointTransforms;
	}
	void init(ModelMeshArticulated &ma){
		L=1;
		jointsRoot=ma.getJointsRoot();
		setJointNames(ma.getJointNames());
		getJointsRoot().fillJointTransforms(jointTransforms);
	}
	void render(){
		//cout << "render..."<< endl;
		//int i=0;
		ax=L;ay=L,az=L;
		for(unsigned int i=0;i<jointTransforms.size();i++){
			Mat       &m=jointTransforms[i];
			string &text=jointNames[i];
			glPushMatrix();
				//cout << getPos() << endl;
				glMultTransposeMatrixf((GLfloat*)m.data);
				glColor3f(1,1,1);
				//glRasterPos3f( x, y , z);
				glRasterPos3f( 0, 0 , 0);
				for(char c:text)
				    glutBitmapCharacter( GLUT_BITMAP_9_BY_15, c );
				glPushMatrix();
				glColor3f(1,0,0);
				glScalef(ax,ax/20.0,ax/20.0);
				glTranslatef(0.5,0,0);
				glutSolidCube(1);
				glPopMatrix();
				glPushMatrix();
				glColor3f(0,1,0);
				glScalef(ay/20.0,ay,ay/20.0);
				glTranslatef(0,0.5,0);
				glutSolidCube(1);
				glPopMatrix();
				glPushMatrix();
				glColor3f(0,0,1);
				glScalef(az/20.0,az/20.0,az);
				glTranslatef(0.0,0,0.5);
				glutSolidCube(1);
				glPopMatrix();
			glPopMatrix();
		}
		//cout << "endl..."<< endl;
	}
};

