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


class GLSLShader{
	GLuint shaderID;
public:
	GLSLShader(GLuint shaderType):shaderID(glCreateShader(shaderType)){
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
