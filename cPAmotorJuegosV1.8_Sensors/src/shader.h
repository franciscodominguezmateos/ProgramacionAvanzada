/*
 * shader.h
 *
 *  Created on: 3 Jan 2020
 *      Author: Francisco Dominguez
 *  Inspired by: https://www.youtube.com/user/ThinMatrix
 */
#pragma once
#include "util.h"
#include "vector3d.h"

// In order to use GLSL
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
// In order to use GLSL
#include <GL/glext.h>
#include "texture.h"

struct Vec2{float x,y;};
struct Vec3{float x,y,z;};
struct Vec4{float x,y,z,w;};

class Uniform{
	static const GLint NOT_FOUND=-1;
	string name;
	GLint location;
public:
	Uniform(string n=""):name(n),location(NOT_FOUND){}
	//void setLocation(GLSLShaderProgram &p){
	//	setLocation(p.id());
	//}
	void setLocation(GLuint programID){
		location=glGetUniformLocation(programID,name.c_str());
		if(location == GL_INVALID_VALUE)
			throw runtime_error("Error in Uniform::storeUniformLocation() programID is not a value generated by OpenGL");
		if(location == GL_INVALID_OPERATION)
			throw runtime_error("Error in Uniform::storeUniformLocation() programID program is not a program object");
		if(location == GL_INVALID_OPERATION)
			throw runtime_error("Error in Uniform::storeUniformLocation() programID has not been successfully linked");
		if(location == NOT_FOUND)
			throw runtime_error("Error in Uniform::storeUniformLocation() uniform name="+name+" not found in programID");
	}
	inline GLint getLocation(){return location;}
    // FLOAT
	inline void load(GLfloat v){glUniform1f(location,v);}
	inline Uniform &operator=(GLfloat f){load(f);return *this;}
    // INT
	inline void load(GLint   v){glUniform1i(location,v);}
	inline Uniform &operator=(GLint f){load(f);return *this;}
    // BOOLEAN
	inline void load(bool v){glUniform1f(location,v?1:0);}
	inline Uniform &operator=(bool f){load(f);return *this;}
    // Vector 2D
	inline void load(GLfloat x,GLfloat y){glUniform2f(location,x,y);}
	inline void load(Vec2 v){load(v.x,v.y);}
	inline Uniform &operator=(Vec2 f){load(f);return *this;}
    // Vector 3D
	inline void load(GLfloat x,GLfloat y,GLfloat z){glUniform3f(location,x,y,z);}
	inline void load(Vec3 v){load(v.x,v.y,v.z);}
	inline Uniform &operator=(Vec3 f){load(f);return *this;}
	inline void load(Vector3D v){load((GLfloat)v.getX(),(GLfloat)v.getY(),(GLfloat)v.getZ());}
	inline Uniform &operator=(Vector3D f){load(f);return *this;}
    // Vector 4D
	inline void load(GLfloat x,GLfloat y,GLfloat z,GLfloat w){glUniform4f(location,x,y,z,w);}
	inline void load(Vec4 v){load(v.x,v.y,v.z,v.w);}
	inline Uniform &operator=(Vec4 f){load(f);return *this;}
    // Vector case size: 2->2D 3->3D 4->4D 9->Mat3x3 16->Mat4x4
	// Matrices here are in OpenGL format in column mayor.
	// that is the reason of 'false' in transpose parameter
	inline void load(vector<GLfloat> v){
		GLfloat* d=v.data();
		if(v.size()== 2) glUniform2fv(location,1,d);
		if(v.size()== 3) glUniform3fv(location,1,d);
		if(v.size()== 4) glUniform4fv(location,1,d);
		//if(v.size()== 4) glUniformMatrix2fv(location,1,true,d);
		if(v.size()== 9) glUniformMatrix3fv(location,1,false,d);
		if(v.size()==16) glUniformMatrix4fv(location,1,false,d);
	}
	inline Uniform &operator=(vector<GLfloat> v){load(v);return *this;}
    // Matrix 2x2, 3x3 and 4x4
	// Matrices are in OpenCV or traditional order mayor row not in OpenGL format
	// that is the reason of 'true' in transpose parameter
	inline void load(Mat m0){
		Mat_<GLfloat> m(m0);
		GLfloat* d=(GLfloat*)m.data;
		if(m.cols==2 && m.rows==2) glUniformMatrix2fv(location,1,true,d);
		if(m.cols==3 && m.rows==3) glUniformMatrix3fv(location,1,true,d);
		if(m.cols==4 && m.rows==4) glUniformMatrix4fv(location,1,true,d);
	}
	inline Uniform &operator=(Mat f){load(f);return *this;}
    // Vector of Matrix 2x2, 3x3 and 4x4 for joints
	inline void load(vector<Mat> v){
		if(!v.empty()){
			//take as granted that all Mat are same dimension
			Mat_<GLfloat> m(v[0]);
			unsigned int stride=m.cols*m.rows;
			unsigned int n=v.size();
			GLfloat* d=new GLfloat[n*stride];
			unsigned int idx=0;
			for(Mat &m:v)
				for(int i=0;i<m.rows;i++)
					for(int j=0;j<m.cols;j++)
						d[idx++]=(GLfloat)m.at<GLfloat>(i,j);
			if(m.cols==2 && m.rows==2) glUniformMatrix2fv(location,n,true,d);
			if(m.cols==3 && m.rows==3) glUniformMatrix3fv(location,n,true,d);
			if(m.cols==4 && m.rows==4) glUniformMatrix4fv(location,n,true,d);
			delete[] d;
		}
	}
	inline Uniform &operator=(vector<Mat> &v){load(v);return *this;}
};
class GLSLVBO{
	static const GLuint BYTES_PER_INT=4;
	static const GLuint BYTES_PER_FLOAT=4;
	GLuint id;
	GLuint type;
	GLuint data_type;
	GLuint attrSize;
public:
	GLSLVBO():id(0),type(0){}
	GLSLVBO(GLuint type,GLuint dt=GL_FLOAT,GLuint as=3):type(type),data_type(dt),attrSize(as){glGenBuffers(1,&id);}
	inline GLuint getDataType(){return data_type;}
	inline GLuint getAttrSize(){return attrSize;}
	inline void bind(){glBindBuffer(type,id);}
	inline void unbind(){glBindBuffer(type,0);}
	inline void storeData(vector<GLfloat> &data){glBufferData(type,data.size()*BYTES_PER_FLOAT,data.data(),GL_STATIC_DRAW);}
	inline void storeData(vector<GLint>   &data){glBufferData(type,data.size()*BYTES_PER_INT  ,data.data(),GL_STATIC_DRAW);}
	inline void storeData(vector<GLuint>  &data){glBufferData(type,data.size()*BYTES_PER_INT  ,data.data(),GL_STATIC_DRAW);}
	inline void deleleteBuffer(){glDeleteBuffers(1,&id);}
};
class GLSLVAO{
	static const GLuint BYTES_PER_INT=4;
	static const GLuint BYTES_PER_FLOAT=4;
	GLuint id;
	vector<GLSLVBO> VBOs;
	GLSLVBO indexVBO;
	GLuint indexCount;
public:
	GLSLVAO():id(0),indexCount(0){}
	inline void init(){
		glGenVertexArrays(1, &id);
		bind();
	}
	GLuint getIndexCount(){return indexCount;}
	vector<GLSLVBO> &getVBOs(){return VBOs;}
	void setVBOs(vector<GLSLVBO> &vbos){
		VBOs=vbos;
		//this makes the asumption that attributes location
		//are the same that position in the vector
		for(unsigned int i=0;i<vbos.size();i++){
			GLSLVBO &vbo=vbos[i];
			vbo.bind();
			setVertexAttribPointer(i,vbo);
			vbo.unbind();
		}
	}
	inline void bind()  {glBindVertexArray(id);}
	inline void unbind(){glBindVertexArray(0);}
	inline void bind(vector<GLuint> &attributes){bind();for(GLuint i:attributes) glEnableVertexAttribArray(i);}
	inline void bind(GLuint atts...){vector<GLuint> attributes(atts);bind(attributes);}
	inline void bindAll(){bind();for(GLuint i=0;i<VBOs.size();i++) glEnableVertexAttribArray(i);}
	inline void unbind(vector<GLuint> &attributes){for(GLuint i:attributes) glDisableVertexAttribArray(i);unbind();}
	inline void unbind(GLuint atts...){vector<GLuint> attributes(atts);unbind(attributes);}
	inline void unbindAll(){for(GLuint i=0;i<VBOs.size();i++) glDisableVertexAttribArray(i);unbind();}
	void createIndexBuffer(vector<GLint> &indices){
		indexVBO=GLSLVBO(GL_ELEMENT_ARRAY_BUFFER);
		indexVBO.bind();
		indexVBO.storeData(indices);
		indexCount=indices.size();
	}
	void createIndexBuffer(vector<GLuint> &indices){
		indexVBO=GLSLVBO(GL_ELEMENT_ARRAY_BUFFER);
		indexVBO.bind();
		indexVBO.storeData(indices);
		indexCount=indices.size();
	}
	void setVertexAttribPointer(GLuint attribute,GLSLVBO &vbo){
		if(vbo.getDataType()==GL_FLOAT)
			glVertexAttribPointer(attribute,vbo.getAttrSize(),GL_FLOAT,GL_FALSE,0,0);
		else
			glVertexAttribIPointer(attribute,vbo.getAttrSize(),vbo.getDataType(),0,0);
	}
	void createAttribute(GLuint attribute,vector<GLfloat> &data,GLuint attrSize){
		GLSLVBO vbo(GL_ARRAY_BUFFER,GL_FLOAT,attrSize);
		vbo.bind();
		vbo.storeData(data);
		setVertexAttribPointer(attribute,vbo);
		vbo.unbind();
		VBOs.push_back(vbo);
	}
	void createAttribute(GLuint attribute,vector<GLint> &data,GLuint attrSize){
		GLSLVBO vbo(GL_ARRAY_BUFFER,GL_INT,attrSize);
		vbo.bind();
		vbo.storeData(data);
		setVertexAttribPointer(attribute,vbo);
		vbo.unbind();
		VBOs.push_back(vbo);
	}
	void createAttribute(GLuint attribute,vector<GLuint> &data,GLuint attrSize){
		GLSLVBO vbo(GL_ARRAY_BUFFER,GL_UNSIGNED_INT,attrSize);
		vbo.bind();
		vbo.storeData(data);
		setVertexAttribPointer(attribute,vbo);
		vbo.unbind();
		VBOs.push_back(vbo);
	}
	void deleteVBOs(){
		glDeleteVertexArrays(1,&id);
		for(GLSLVBO &vbo:VBOs) vbo.deleleteBuffer();
		indexVBO.deleleteBuffer();
	}
};
//11/04/2020 To test this class
//13/04/2020 Works like a charm
class GLSLFBO{
	int width,height;
	GLuint id;
	Texture color;
	Texture depth;
public:
	//If we don't call init() toOpenCV get the default screen pixels
	GLSLFBO(int w,int h):width(w),height(h),id(0){}
	void init(){
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		color.init();
		color.asRenderTexture(width,height);
		depth.init();
		depth.asDepthTexture(width,height);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw runtime_error("Error in GLSLFBO::init() not frame buffer complete.");
	}
	inline void setWidth(int w) {width=w;}
	inline void setHeight(int h){height=h;}
	inline void bind()  {glBindFramebuffer(GL_FRAMEBUFFER, id);}
	inline void unbind(){glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	Mat toOpenCV() {
		bind();
	    cv::Mat img(height, width, CV_8UC3);
	    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3)?1:4);
	    glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
	    glReadPixels(0, 0, img.cols, img.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, img.data);
	    cv::Mat flipped(img);
	    cv::flip(img, flipped, 0);
	    unbind();
	    return img;
	}
	Mat toOpenCV32FC4() {
		bind();
		//I want to read unclamped data but this doesn't seem to work
		//I made changes in GameEngine to solve the problem but it doesn't work
		//glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
		//glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
	    cv::Mat img(height, width, CV_32FC4);
	    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3)?1:4);
	    glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
	    glReadPixels(0, 0, img.cols, img.rows, GL_BGRA, GL_FLOAT, img.data);
	    cv::Mat flipped(img);
	    cv::flip(img, flipped, 0);
	    unbind();
	    return img;
	}
	//27/04/2020
	// TODO: test if it works fine
	//28/12/020 Work perfect!!!
	Mat toOpenCVdepth() {
		bind();
	    cv::Mat img(height, width, CV_32F);
	    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3)?1:4);
	    glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
	    glReadPixels(0, 0, img.cols, img.rows, GL_DEPTH_COMPONENT, GL_FLOAT, img.data);
	    cv::Mat flipped(img);
	    cv::flip(img, flipped, 0);
	    unbind();
	    return img;
	}
};
class GLSLShader{
	GLuint shaderID;
    //30/04/2020
	//TODO: automatic extraction of uniform name variables
	//      just parse lines and extract names the ones that begin with uniform
	vector<string> uniformNames;
public:
	GLSLShader(GLuint shaderType):shaderID(glCreateShader(shaderType)){	}
    vector<string> &getUniformNames(){return uniformNames;}
	void parseUniformNames(string sourceCode){
	    //30/04/2020
		//06/05/2020: automatic extraction of uniform name variables
		//      just parse lines and extract names the ones that begin with uniform
		stringstream sstr(sourceCode);
		vector<string> lines;
		sstr>>lines;
		for(string line:lines){
			trim(line);
			trim(line,";");
			vector<string> words=split(line);
			if(words.size()>0)
			if(words[0]=="uniform"){
				string uniformName=words[2];
				vector<string> vs=split(uniformName,'[');
				if(vs.size()==1)
					uniformNames.push_back(uniformName);
				else
					uniformNames.push_back(vs[0]);
			}
		}
	}
	GLuint compileFromFileName(string fileName){
		cout << "Compiling shader: "<<fileName<<endl;
		string sourceCode;
		ifstream sf(fileName);
		sf>>sourceCode;
		return compile(sourceCode);
	}
	GLuint compile(string sourceCode){
		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile shader
		char const * sourcePointer = sourceCode.c_str();
		glShaderSource(shaderID, 1, &sourcePointer , NULL);
		glCompileShader(shaderID);

		// Check errors
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if ( InfoLogLength > 0 ){
			std::vector<char> shaderErrorMessage(InfoLogLength+1);
			glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderErrorMessage[0]);
			throw runtime_error(&shaderErrorMessage[0]);
		}
		parseUniformNames(sourceCode);
		return shaderID;
	}
};
class GLSLShaderProgram {
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	map<string,Uniform> uniforms;
public:
	GLSLShaderProgram(){}
	void compileFromFileNames(string vertexFileName,string fragmentFileName){
		// Compiling vertes shader
		GLSLShader vs(GL_VERTEX_SHADER);
	    vertexShaderID=vs.compileFromFileName(vertexFileName);
	    addUniformsNames(vs.getUniformNames());
	    // Compiling frame shader
		GLSLShader fs(GL_FRAGMENT_SHADER);
		fragmentShaderID=fs.compileFromFileName(fragmentFileName);
	    addUniformsNames(fs.getUniformNames());
		link();
	}
	void compileFromStrings(string vertexSourceCode,string fragmentSourceCode){
		// Compiling vertes shader
		GLSLShader vs(GL_VERTEX_SHADER);
	    vertexShaderID=vs.compile(vertexSourceCode);
	    addUniformsNames(vs.getUniformNames());
	    // Compiling frame shader
		GLSLShader fs(GL_FRAGMENT_SHADER);
		fragmentShaderID=fs.compile(fragmentSourceCode);
	    addUniformsNames(fs.getUniformNames());
		link();
	}
	void link(){
		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Vincular el programa por medio del ID
		//printf("Linking program\n");
		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		// Revisar el programa
		glGetProgramiv(programID, GL_LINK_STATUS, &Result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if ( InfoLogLength > 0 ){
			std::vector<char> ProgramErrorMessage(InfoLogLength+1);
			glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			throw runtime_error(&ProgramErrorMessage[0]);
		}
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		setUniformLocations();
	}
	~GLSLShaderProgram(){stop();glDeleteProgram(programID);}
	void setUniformsLocation(vector<Uniform> &uniforms){for(Uniform &u:uniforms)u.setLocation(programID);}
	// I don't know how this work jet. Any help out there? ->
	//           Old->       https://docs.microsoft.com/en-us/cpp/cpp/functions-with-variable-argument-lists-cpp?view=vs-2019
	//           C++11       https://en.cppreference.com/w/cpp/utility/initializer_list
	//           C++11       https://en.cppreference.com/w/cpp/language/parameter_pack
	//                       https://thispointer.com/c11-variadic-template-function-tutorial-examples/
	//void setUniformsLocation(Uniform uniforms...){for(Uniform &u:uniforms)u.setLocation(programID);}
    //void setUniforms(initializer_list<string> strings){
    void setUniforms(vector<string> uniformNames){
     	for(auto s:uniformNames){
    		Uniform u(s);
    		u.setLocation(programID);
    		uniforms[s]=u;
    	}
    }
    void setUniformLocations(){
    	for(auto pair:uniforms){
    		uniforms[pair.first].setLocation(programID);
    	}
    }
    void addUniformsNames(vector<string> uniformNames){
         	for(auto s:uniformNames){
        		Uniform u(s);
        		//u.setLocation(programID);
        		uniforms[s]=u;
        	}
    }
    Uniform &getUniform(string s){return uniforms[s];}
	Uniform &operator[](string s){
		if(uniforms.count(s)==0)
			throw runtime_error(" in GLSLShaderProgram::operataor[] uniform name='"+s+ "' doesn't exist.");
		return uniforms[s];
	}
	void bindAttribute(GLuint attribID,string s){
		glBindAttribLocation(programID,attribID,s.c_str());
	}
	void start(){glUseProgram(programID);}
	void stop() {glUseProgram(0);}
	inline GLuint id(){return programID;}
};