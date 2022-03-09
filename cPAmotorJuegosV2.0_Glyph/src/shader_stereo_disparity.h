/*
 * shader_stereo_disparity.h
 *
 *  Created on: 23 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
#include "shader_gpgpu_binop.h"
const string fragmentShaderDisparity=R"glsl(
#version 330 core

in vec3 pixel;

out vec4 out_color;

//left image
uniform sampler2D tex;
//right image
uniform sampler2D tex1;
uniform vec2 dim;

const float minDisp=10;
const float maxDisp=120;
const float ranDisp=maxDisp-minDisp;
//patch size patchxpatch pixels
const float patch =15;
const float patch2=patch/2;

int goodCoord(){return 1;}

//compare two patches of patchxpatch at dis coord
float similarity(float atDip){
 vec4 t=vec4(0,0,0,0);
 for(float i=-path2;i<=path2;i++){
   for(float j=-path2;j<=path2;j++){
     vec2 p0=(gl_FragCoord.xy+vec2(i,j))/dim;
     vec2 p1=(gl_FragCoord.xy+vec2(i-atDisp,j))/dim;
     vec4 c0=texture(tex ,p0);
     vec4 c1=texture(tex1,p1);
     vec4 dif=c1-c0;
     vec4 dif2=dif*dif;
     t+=dif2;
   }
 }
 //normalize
 t=sqrt(t)/(path*path);
 return (t.x+t.y+t.z+t.w)/4.0;
}

float disparity(){
 float minS=10.0;
 float d=0;
 for(float i=minDisp;i<maxDisp;i++){
   float s=similarity(i);
   if(s<minS){
     minS=s;
     d=i;
   }
 }
 //Normalize since we can't take out values out of 0-1
 return d/ranDisp;
}

void main(){
 if(goodCoord()){
   float d=disparity();
   out_color=vec4(d,d,d,1);
 }
 else{
   out_color=vec4(1,0,0,1);
 }
}
)glsl";
class ShaderStereoDisparity: public ShaderBinOp {
};

