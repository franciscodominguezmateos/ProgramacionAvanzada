/*
 * shader.h
 *
 *  Created on: 3 Jan 2020
 *      Author: Francisco Dominguez
 *  Inspired by: https://www.youtube.com/user/ThinMatrix
 */

#ifndef SHADER_H_
#define SHADER_H_
#include "util.h"

// In order to use GLSL
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
// In order to use GLSL
#include <GL/glext.h>

class GLSLVBO{
	static const GLuint BYTES_PER_INT=4;
	static const GLuint BYTES_PER_FLOAT=4;
	GLuint id;
	GLuint type;
public:
	GLSLVBO():id(0),type(0){}
	GLSLVBO(GLuint type):type(type){
		glGenBuffers(1,&id);}
	inline void bind(){glBindBuffer(type,id);}
	inline void unbind(){glBindBuffer(type,0);}
	inline void storeData(vector<GLfloat> &data){
		glBufferData(type,data.size()*BYTES_PER_FLOAT,data.data(),GL_STATIC_DRAW);}
	inline void storeData(vector<GLuint>  &data){
		glBufferData(type,data.size()*BYTES_PER_INT,data.data(),GL_STATIC_DRAW);}
	inline void deleleteBuffer(){
		glDeleteBuffers(1,&id);}
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
	inline void bind(){glBindVertexArray(id);}
	inline void unbind(){glBindVertexArray(0);}
	void bind(vector<GLuint> attributes){
		bind();
		for(GLuint i:attributes) glEnableVertexAttribArray(i);
	}
	void bindAll(){
		bind();
		for(GLuint i=0;i<VBOs.size();i++) glEnableVertexAttribArray(i);
	}
	void unbind(vector<GLuint> attributes){
		for(GLuint i:attributes) glDisableVertexAttribArray(i);
		unbind();
	}
	void unbindAll(){
		for(GLuint i=0;i<VBOs.size();i++) glDisableVertexAttribArray(i);
		unbind();
	}
	void createIndexBuffer(vector<GLuint> &indices){
		//This is already done in the constructor
		indexVBO=GLSLVBO(GL_ELEMENT_ARRAY_BUFFER);
		indexVBO.bind();
		indexVBO.storeData(indices);
		indexCount=indices.size();
	}
	void createAttribute(GLuint attribute,vector<GLfloat> &data,GLuint attrSize){
		GLSLVBO vbo(GL_ARRAY_BUFFER);
		vbo.bind();
		vbo.storeData(data);
		//glVertexAttribPointer(attribute,attrSize,GL_FLOAT,GL_FALSE,attrSize*BYTES_PER_FLOAT,0);
		glVertexAttribPointer(attribute,attrSize,GL_FLOAT,GL_FALSE,0,0);
		vbo.unbind();
		VBOs.push_back(vbo);
	}
	void createAttribute(GLuint attribute,vector<GLuint> &data,GLuint attrSize){
		GLSLVBO vbo(GL_ARRAY_BUFFER);
		vbo.bind();
		vbo.storeData(data);
		//glVertexAttribIPointer(attribute,attrSize,GL_INT,attrSize*BYTES_PER_INT,0);
		glVertexAttribIPointer(attribute,attrSize,GL_INT,0,0);
		vbo.unbind();
		VBOs.push_back(vbo);
	}
	void deleteVBOs(){
		glDeleteVertexArrays(1,&id);
		for(GLSLVBO &vbo:VBOs) vbo.deleleteBuffer();
		indexVBO.deleleteBuffer();
	}
};

class GLSLShader{
	GLuint shaderID;
public:
	GLSLShader(GLuint shaderType):shaderID(glCreateShader(shaderType)){	}
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
		return shaderID;
	}
};
class GLSLShaderProgram {
	GLuint programID;
public:
	GLSLShaderProgram(){}
	void compileFromFileNames(string vertexFileName,string fragmentFileName){
		cout << vertexFileName <<endl;
		cout << fragmentFileName<<endl;
		// Compiling vertes shader
		GLSLShader vs(GL_VERTEX_SHADER);
	    GLuint VertexShaderID=vs.compileFromFileName(vertexFileName);
	    // Compiling frame shader
		GLSLShader fs(GL_FRAGMENT_SHADER);
		GLuint FragmentShaderID=fs.compileFromFileName(fragmentFileName);
		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Vincular el programa por medio del ID
		printf("Linking program\n");
		programID = glCreateProgram();
		glAttachShader(programID, VertexShaderID);
		glAttachShader(programID, FragmentShaderID);
		glLinkProgram(programID);

		// Revisar el programa
		glGetProgramiv(programID, GL_LINK_STATUS, &Result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if ( InfoLogLength > 0 ){
			std::vector<char> ProgramErrorMessage(InfoLogLength+1);
			glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			throw runtime_error(&ProgramErrorMessage[0]);
		}
		glDetachShader(programID, VertexShaderID);
		glDetachShader(programID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);
	}
	~GLSLShaderProgram(){
		stop();
		glDeleteProgram(programID);
	}
	void bindAttribute(GLuint attribID,string s){
		glBindAttribLocation(programID,attribID,s.c_str());
	}

	void start(){
		glUseProgram(programID);
	}
	void stop(){
		glUseProgram(0);
	}
};

#endif /* SHADER_H_ */
