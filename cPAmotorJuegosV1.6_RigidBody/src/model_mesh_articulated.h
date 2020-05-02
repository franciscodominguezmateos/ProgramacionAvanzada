/*
 * model_mesh_articulated.h
 *
 *  Created on: 30 Mar 2020
 *      Author: Francisco Dominguez
 */
#pragma once
#include <GL/glut.h>
#include "model_mesh.h"
#include "model_joint.h"
//Each vertex has a number of joints and weights acting on it
//in our case for our shader is 3
class VertexSkinData{
	vector<GLuint>  joints;
	vector<GLfloat> weights;
public:
	void addJointEffect(GLuint jointId,GLfloat weight){
		for(unsigned int i=0;i<weights.size();i++){
			if(weight>weights[i]){
				 joints.insert( joints.begin()+i,jointId);
				weights.insert(weights.begin()+i,weight);
				return;
			}
		}
		 joints.push_back(jointId);
		weights.push_back(weight);
	}
	template<class T=GLuint>
	vector<T> getTopJoints(int max){
		vector<T> vj;
		int i=0;
		for(T j:joints){
			if(i<max){
				vj.push_back(j);
			}
			else{
				return vj;
			}
			i++;
		}
		// fill up with 0
		for(;i<max;i++){
			vj.push_back(0);
		}
		return vj;
	}
	template<class T=GLfloat>
	vector<T> normalize(vector<T> &v){
		T sum=0;
		for(T f:v) sum+=f;
		for(T &f:v) f/=sum;
		return v;
	}
	template<class T=GLfloat>
	vector<T> getTopWeights(int max){
		vector<T> vw;
		int i=0;
		for(T w:weights){
			if(i<max){
				vw.push_back(w);
			}
			else{
				return normalize(vw);
			}
			i++;
		}
		for(;i<max;i++){
			vw.push_back(0);
		}
		return normalize(vw);
	}
	friend ostream &operator<<(ostream &os,const VertexSkinData &vsd);
};
ostream &operator<<(ostream &os,const VertexSkinData &vsd){
	os<<"joints =";for(auto i:vsd.joints ) os<< i <<",";os<<endl;
	os<<"weights=";for(auto i:vsd.weights) os<< i <<",";os<<endl;
	return os;
}
class ModelMeshArticulated:public ModelMesh {
	vector<VertexSkinData>  vertexSkinData;
	Joint jointsRoot;
	vector<string>  jointNames;
	map<string,int> jointIdx;
	vector<Mat> inverseBindTransforms;
public:
	inline void setJointNames(vector<string> &jn){
		jointNames=jn;
		for(unsigned int i=0;i<jn.size();i++) jointIdx[jn[i]]=i;
	}
	inline vector<string> &getJointNames(){return jointNames;}
	inline int getJointIdx(string name){return jointIdx[name];}
	inline void addVertexSkinData (VertexSkinData &vsd){vertexSkinData.push_back(vsd);}
	inline void setJointsRoot(Joint j){jointsRoot=j;}
	inline void setInverseBindTransforms(vector<Mat> &ivt){inverseBindTransforms=ivt;}
	inline Joint &getJointsRoot(){return jointsRoot;}
	inline vector<Mat> &getInverseBindTransforms(){return inverseBindTransforms;}
	inline vector<GLuint> getJoints(int max=3){
		vector<GLuint> vj;
		for(VertexSkinData &v:vertexSkinData){
			for(GLuint j:v.getTopJoints(max)){
				vj.push_back(j);
			}
		}
		return vj;
	}
	inline vector<GLfloat> getWeights(int max=3){
		vector<GLfloat> vw;
		for(VertexSkinData &v:vertexSkinData){
			for(GLfloat w:v.getTopWeights(max)){
				vw.push_back(w);
			}
		}
		return vw;
	}
};

