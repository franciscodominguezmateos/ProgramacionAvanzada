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
	//increment transform from
	// global bind pose to current global pose
	vector<Mat> animatedTransforms;
	float ax=1,ay=1,az=1;
public:
	float L;
	vector<Mat> &getAnimatedTransforms(){return animatedTransforms;}
	vector<string> &getJointNames(){return jointNames;}
	void            setJointNames(vector<string> &jn){
		jointNames=jn;
		jointTransforms   =vector<Mat>(jn.size());
		animatedTransforms=vector<Mat>(jn.size());
		for(unsigned int i=0;i<jn.size();i++) jointIdx[jn[i]]=i;
	}
	SkeletonPose &getPose(){return currentPose;}
	virtual void  setPose(SkeletonPose &cp){
		currentPose=cp;
		getJointsRoot().applyPose(currentPose);
		getJointsRoot().fillAnimatedTransforms(animatedTransforms);
		getJointsRoot().fillJointTransforms(jointTransforms);
	}
	void          setJointsRoot(Joint &j){jointsRoot=j;}
	inline Joint &getJointsRoot() {return jointsRoot;}
	vector<Mat>  &getJointTransforms(){
		return jointTransforms;
	}
	void init(ModelMeshArticulated &ma, float l=1){
		L=l;
		jointsRoot=ma.getJointsRoot();
		setJointNames(ma.getJointNames());
		getJointsRoot().fillAnimatedTransforms(animatedTransforms);
		getJointsRoot().fillJointTransforms(jointTransforms);
		//SkeletonPose pose=getJointsRoot().getLocalBindPose();
		//setPose(pose);
	}
	void render(){
		glPushMatrix();
		glTranslatef(getPos().getX(),getPos().getY(),getPos().getZ());
		glRotatef(getRot().getX(), 1, 0, 0);
		glRotatef(getRot().getY(), 0, 1, 0);
		glRotatef(getRot().getZ(), 0, 0, 1);
		ax=L;ay=L,az=L;
		for(unsigned int i=0;i<jointTransforms.size();i++){
			Mat       &m=jointTransforms[i];
			string &text=jointNames[i];
			glPushMatrix();
				//cout << getPos() << endl;
				glMultTransposeMatrixf((GLfloat*)m.data);
				glColor3f(1,1,1);
				glRasterPos3f( 0, 0 , 0);
				for(char c:text) glutBitmapCharacter( GLUT_BITMAP_9_BY_15, c );
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
		glPopMatrix();
	}
};

