/*
 * model_vao.h
 *
 *  Created on: 30 Mar 2020
 *      Author: Francisco Dominguez
 */

#ifndef MODEL_VAO_H_
#define MODEL_VAO_H_
#include "solido.h"
#include "shader.h"
#include "model_mesh_articulated.h"
class KeyFrameJoint{
	float timeStamp;
	vector<Mat> localJointTransforms;
public:
};
class AnimationJoints{
	vector<KeyFrameJoint> keyFrames;
public:
	Mat interpolate(Mat m0,Mat m1,float t){
		Mat dm,vt,mt;
		dm=m1*m0.inv();
		Rodrigues(dm,vt);
		vt=vt*t;
		Rodrigues(vt,mt);
		return mt*m0;
	}
};
/*class AnimatorArticulated{
	SolidArticulated sa;
	AnimationJoints animation;
public:
};*/
class SolidArticulated: public Solido{
	Joint jointsRoot;
	vector<string>  jointNames;
	map<string,int> jointIdx;
	vector<Mat> jointTransforms;
public:
	void setJointNames(vector<string> &jn){
		jointNames=jn;
		jointTransforms=vector<Mat>(jn.size());
		for(unsigned int i=0;i<jn.size();i++) jointIdx[jn[i]]=i;
	}
	vector<Mat> &getJointTransforms(){
		return jointTransforms;
	}
	inline Joint &getJointsRoot(){return jointsRoot;}
	vector<Mat> &buildJointTransforms(){
		addJoints(jointsRoot,jointTransforms);
		for(Mat &m:jointTransforms)	cout << m << endl;
		cout<<"buildJointTransforms"<<endl;
		return jointTransforms;
	}
	void addJoints(Joint &joint,vector<Mat> &jm){
		jm[joint.getIdx()]=joint.getAnimatedTransform();
		for(Joint &j:joint.getChildren()){
			addJoints(j,jm);
		}
	}
	void init(ModelMeshArticulated &ma){
		jointsRoot=ma.getJointsRoot();
		jointsRoot.calcInverseBindTransform(Mat::eye(4,4,CV_32F));
		setJointNames(ma.getJointNames());
		buildJointTransforms();
	}
	void render(){
		//cout << "render..."<< endl;
		for(Mat &m:jointTransforms){
			float x=m.at<float>(0,3);
			float y=m.at<float>(1,3);
			float z=m.at<float>(2,3);
			setPos(Vector3D(x,y,z));
			Solido::render();
			glPushMatrix();
			//cout << getPos() << endl;
			glMultTransposeMatrixf((GLfloat*)m.data);
			glPushMatrix();
			glColor3f(1,0,0);
			glScalef(0.6,0.05,0.05);
			glTranslatef(0.5,0,0);
			glutSolidCube(1);
			glPopMatrix();
			glPushMatrix();
			glColor3f(0,1,0);
			glScalef(0.05,0.6,0.05);
			glTranslatef(0,0.5,0);
			glutSolidCube(1);
			glPopMatrix();
			glPushMatrix();
			glColor3f(0,0,1);
			glScalef(0.05,0.05,0.6);
			glTranslatef(0.0,0,0.5);
			glutSolidCube(1);
			glPopMatrix();
			glPopMatrix();
		}
		//cout << "endl..."<< endl;
	}
};
class SolidArticulatedVAO: SolidArticulated{
	GLSLVAO vao;
	GLSLShaderProgram* shaderProgram;
	Uniform jointTransforms;
	SolidArticulatedVAO(GLSLShaderProgram* p=nullptr):shaderProgram(p),jointTransforms("jointTransforms"){
		jointTransforms.setLocation(shaderProgram->id());
	}
	void init(ModelMeshArticulated &pm){
		ModelMeshArticulated m;//=pm.buildShaderReadyMeshModel();
		vao.init();
		vao.createIndexBuffer(m.getIvertices());
		vector<GLfloat> vf=m.getFloatFromVertices();
		vao.createAttribute(0,vf,3);
		vector<GLfloat> vt=m.getFloatFromTextures();
		vao.createAttribute(1,vt,2);
		vector<GLfloat> vn=m.getFloatFromNormals();
		vao.createAttribute(2,vn,3);
		vector<GLuint> vj=m.getJoints();
		vao.createAttribute(3,vj,3);//?????????????????????????????????????????????????????????????????
		vector<GLfloat> vw=m.getWeights();
		vao.createAttribute(4,vn,3);
	}
};
/*
 * The point of this class is to use one instance as master, the owner of the pointer vao
 * and other clones are used to share the vao attribute of the master instance.
 * Then we can render lots of different objects with the same vao.
 */
class ModelVAO: public Solido {
	GLSLShaderProgram* shaderProgram;
	GLSLVAO* vao;
	Uniform T;
	bool isMaster;
public:
	ModelVAO(GLSLShaderProgram* p,GLSLVAO *pvao=nullptr):shaderProgram(p),vao(pvao),T("T"),isMaster(false){
		T.setLocation(shaderProgram->id());
		if(vao==nullptr){
			//this is the master instance the owner of vao
			vao=new GLSLVAO();
			isMaster=true;
		}
	}
	virtual ~ModelVAO(){
		if(isMaster) delete vao;
	}
	ModelVAO *clone(){return new ModelVAO(*this);}
	// This only have to call on the master instance
	void init(ModelMesh &pm){
	    ModelMesh m=pm.buildShaderReadyMeshModel();
		vao->init();
		vao->createIndexBuffer(m.getIvertices());
		vector<GLfloat> vf=m.getFloatFromVertices();
		vao->createAttribute(0,vf,3);
		vector<GLfloat> vt=m.getFloatFromTextures();
		vao->createAttribute(1,vt,2);
		vector<GLfloat> vn=m.getFloatFromNormals();
		vao->createAttribute(2,vn,3);
	}
	void render(){
		shaderProgram->start();
	    vao->bindAll();
		T=posEulerAnglesToTransformationMatrix(getPos(),getRot());
		glDrawElements(GL_TRIANGLES, vao->getIndexCount(), GL_UNSIGNED_INT,0); // Empezar desde el vértice 0S; 3 vértices en total -> 1 triángulo
		vao->unbindAll();
		shaderProgram->stop();
	}
	GLSLVAO* getPtrVAO(){return vao;}
};

#endif /* MODEL_VAO_H_ */
