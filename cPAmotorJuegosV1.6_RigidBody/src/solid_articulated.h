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
	vector<Mat> jointTransforms;
	float ax=1,ay=1,az=1;
public:
	float L;
	void setJointNames(vector<string> &jn){
		jointNames=jn;
		jointTransforms=vector<Mat>(jn.size());
		for(unsigned int i=0;i<jn.size();i++) jointIdx[jn[i]]=i;
	}
	void setJointsRoot(Joint &j){jointsRoot=j;}
	void initJointTransforms(int i){jointTransforms=vector<Mat>(i);}
	vector<Mat> &getJointTransforms(){
		return jointTransforms;
	}
	inline Joint &getJointsRoot(){return jointsRoot;}
	vector<Mat> &buildJointTransforms(){
		addJoints(jointsRoot,jointTransforms);
		//for(Mat &m:jointTransforms)	cout << m << endl;
		//cout<<"buildJointTransforms"<<endl;
		return jointTransforms;
	}
	void addJoints(Joint &joint,vector<Mat> &jm){
		jm[joint.getIdx()]=joint.getAnimatedTransform()* joint.getBindTransform();
		for(Joint &j:joint.getChildren()){
			addJoints(j,jm);
		}
	}
	inline void loadInverseBindTransforms(Joint &joint,vector<Mat> &ibts,Mat &parentBindTransform){
		Mat &ibt=ibts[joint.getIdx()];
		joint.setInverseBindTransform(ibt);
		Mat lbt=ibt*parentBindTransform;
		joint.setLocalBindTransform(lbt);
		for(Joint &j:joint.getChildren())
			loadInverseBindTransforms(j,ibts,joint.getBindTransform());
	}
	void init(ModelMeshArticulated &ma){
		L=1;
		jointsRoot=ma.getJointsRoot();
		Mat upf=Mat::eye(4,4,CV_32F);
		//Mat upf=posEulerAnglesToTransformationMatrix<float>(Vector3D(),Vector3D(-90,0,0));
		jointsRoot.calcInverseBindTransform(upf);
		loadInverseBindTransforms(jointsRoot,ma.getInverseBindTransforms(),upf);
		setJointNames(ma.getJointNames());
		buildJointTransforms();
		/*vector<Mat> jt;
		for(Mat &m:ma.getInverseBindTransforms()){
			cout<<"m="<<m.inv()<<endl;
			jt.push_back(m.inv());
		}
		jointTransforms=jt;*/
	}
	void render(){
		//cout << "render..."<< endl;
		//int i=0;
		ax=L;ay=L,az=L;
		for(unsigned int i=0;i<jointTransforms.size();i++){
			Mat &m=jointTransforms[i];
			string &text=jointNames[i];
			float x=m.at<float>(0,3);
			float y=m.at<float>(1,3);
			float z=m.at<float>(2,3);
			setPos(Vector3D(x,y,z));
			//if(i==0) cout << getPos() << endl;
			//i++;
			Solido::render();
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

