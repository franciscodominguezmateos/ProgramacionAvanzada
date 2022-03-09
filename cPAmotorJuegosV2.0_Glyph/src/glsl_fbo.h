/*
 * glsl_fbo.h
 *
 *  Created on: 14 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
//11/04/2020 To test this class
//13/04/2020 Works like a charm
class GLSLFBO;
using GLSLFBOPtr=GLSLFBO*;

class GLSLFBO{
	int width,height;
	GLuint id;
	Texture color;
	Texture depth;
public:
	//If we don't call init() toOpenCV get the default screen pixels
	GLSLFBO(int w,int h):width(w),height(h),id(0),color(w,h),depth(w,h){}
	virtual ~GLSLFBO(){glDeleteFramebuffers(1, &id);}//release textures too}
	void init(){
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		color.init();
		color.asRenderTexture(width,height);
		depth.init();
		depth.asDepthTexture(width,height);
		// define the index array for the outputs
		//GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
		//glDrawBuffers(1,  attachments);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw runtime_error("Error in GLSLFBO::init() not frame buffer complete.");
		//printLimits();
	}
	inline Texture &getColorTex(){return color;}
	inline Texture &getDepthTex(){return depth;}
	inline void setSize(int w,int h){width=w;height=h;}
	inline void setWidth(int w) {width=w; color.setSize(width,height);depth.setSize(width,height);}
	inline void setHeight(int h){height=h;color.setSize(width,height);depth.setSize(width,height);}
	inline int getWidth(){return width;}
	inline int getHeight(){return height;}
	inline void bind()  {glBindFramebuffer(GL_FRAMEBUFFER, id);}
	inline void unbind(){glBindFramebuffer(GL_FRAMEBUFFER, 0);}
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
	    cv::Mat img=Mat::zeros(height, width, CV_32FC4);

	    //glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3)?1:4);
	    //glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
	    glReadPixels(0, 0, img.cols, img.rows, GL_BGRA, GL_FLOAT, img.data);

        //21/01/2021 glGetTexImage it is supose to not clamp. TODO: Check it is true
	    //color.bind();
	    //glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3)?1:4);
	    //glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
	    //glGetTexImage(GL_TEXTURE_2D,0,GL_BGRA,GL_FLOAT,img.data);
	    //glGetnTexImage(GL_TEXTURE_2D,0,GL_BGRA,GL_FLOAT,img.cols* img.rows*4,img.data);
	    //glGetTextureImage(color.getId(),0,GL_BGRA,GL_FLOAT,img.cols* img.rows*4,img.data);
	    //color.unbind();
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
	void printLimits() {
	    int res;
	    //8
	    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &res);
	    printf("Max Color Attachments: %d\n", res);
	    //32768
	    glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &res);
	    printf("Max Framebuffer Width: %d\n", res);
	    //32768
	    glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &res);
	    printf("Max Framebuffer Height: %d\n", res);
	    //32
	    glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &res);
	    printf("Max Framebuffer Samples: %d\n", res);
	    //2048
	    glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &res);
	    printf("Max Framebuffer Layers: %d\n", res);
	}
	/*from:https://www.lighthouse3d.com/tutorials/opengl_framebuffer_objects/
	void printFramebufferInfo(GLenum target, GLuint fbo) {

	    int res, i = 0;
	    GLint buffer;

	    glBindFramebuffer(target,fbo);

	    do {
	        glGetIntegerv(GL_DRAW_BUFFER0+i, &buffer);

	        if (buffer != GL_NONE) {

	            printf("Shader Output Location %d - color attachment %d\n",
	                        i, buffer - GL_COLOR_ATTACHMENT0);

	            glGetFramebufferAttachmentParameteriv(target, buffer,
	                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &res);
	            printf("\tAttachment Type: %s\n",
	                        res==GL_TEXTURE?"Texture":"Render Buffer");
	            glGetFramebufferAttachmentParameteriv(target, buffer,
	                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &res);
	            printf("\tAttachment object name: %d\n",res);
	        }
	        ++i;

	    } while (buffer != GL_NONE);
	}*/
};




