// "ShaderToy Tutorial - Ray Marching for Dummies!" 
// by Martijn Steinrucken aka BigWings/CountFrolic - 2018
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
//
// This shader is part of a tutorial on YouTube
// https://youtu.be/PGtv-dBi2wE

#define MAX_STEPS 300
#define MAX_DIST 300.
#define SURF_DIST .001

float maxv3(vec3 d){return max(d.x,max(d.y,d.z));}

//y plane at origin
float sdPlaneYO( vec3 p ){return p.y;}
//plane at hight s
float sdPlaneY( vec3 p, float s ){return p.y-s;}

//sphere at origin
float sdSphereO( vec3 p, float s ){return length(p)-s;}
//sphere at any point w is radious
float sdSphere( vec3 p, vec4 s){return length(p-s.xyz)-s.w;}

//box at origin
float sdBox( vec3 p, vec3 b ){vec3 d = abs(p) - b; return min(maxv3(d),0.0) + length(max(d,0.0));}

//hard union vs soft union
float opHU(float d1,float d2){return min(d1,d2);}
// http://iquilezles.org/www/articles/smin/smin.htm
float opSU( float a, float b, float k=0.25 ){float h = max(k-abs(a-b),0.0); return min(a, b) - h*h*0.25/k;}

float getDist(vec3 p) { 
    float d = sdSphere(p,vec4( 0,0,0,1.0));
    d = opSU(d,sdSphere(p,vec4( cos(iTime),1,sin(iTime),1)));
    d = opHU(d,sdSphere(p,vec4(-2,0,0,1)));
    d = opSU(d,sdSphere(p,vec4(-2,2,0,1)));
    d = opHU(d,sdSphere(p,vec4( 10,0, 10,1)));
    d = opHU(d,sdSphere(p,vec4(-10,0, 10,1)));
    d = opHU(d,sdSphere(p,vec4( 10,0,-10,1)));
    d = opHU(d,sdSphere(p,vec4(-10,0,-10,1)));
    d = opHU(d,sdPlaneYO(p));
    return d;
}

float rayMarch(vec3 ro, vec3 rd) {
	float dO=0.;
    
    for(int i=0; i<MAX_STEPS; i++) {
    	vec3 p = ro + rd*dO;
        float dS = getDist(p);
        dO += dS;
        if(dO>MAX_DIST || dS<SURF_DIST) break;
    }
    
    return dO;
}

vec3 getNormal(vec3 p) {
	float d = getDist(p);
    vec2 e = vec2(.01, 0);
    
    vec3 n = d - vec3(
        getDist(p-e.xyy),
        getDist(p-e.yxy),
        getDist(p-e.yyx));
    
    return normalize(n);
}

float getLight(vec3 p) {
    vec3 lightPos = vec3(0, 15, 6);
    float alpha=iTime /2;
    lightPos.xz += vec2(sin(alpha), cos(alpha))*20.;
    vec3 l = normalize(lightPos-p);
    vec3 n = getNormal(p);
    
    float dif = clamp(dot(n, l), 0.0, 1.);
    float d = rayMarch(p+n*SURF_DIST*2., l);
    if(d<length(lightPos-p)) dif *= .25;
    
    return dif;
}
//Pose of camera or Transformation from camera 2 world
uniform mat4 camera;   
//Camera matrix  or Transformation from world 2 camera
uniform mat4 cameraInv;
uniform mat4 projection;
uniform mat4 projectionInv;
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = 2.0*fragCoord/iResolution.xy-1.0;
    uv=(projectionInv*vec4(uv,1.0,1.0)).xy;
   
    vec3 rd = normalize(vec3(uv.x, uv.y, -1));
    vec3 ro=camera[3].xyz;
    rd=(camera*vec4(rd,0.0)).xyz;

    float d = rayMarch(ro, rd);
    
    vec3 p = ro + rd * d;
    
    vec3 col = vec3(0);    
    float dif = getLight(p);
    col = vec3(dif);
    if(p.y<0.01){
    	col=vec3(1.0,0.0,0.0)*dif;
    }
    if(length(p-vec3(10.0,0.0,-10.0))<1.001){
    	col=vec3(1.0,1.0,0.0)*dif;
    }
    if(length(p-vec3(-10.0,0.0,-10.0))<1.001){
    	col=vec3(0.0,1.0,0.0)*dif;
    }
    if(length(p-vec3(10.0,0.0,10.0))<1.001){
    	col=vec3(1.0,0.0,1.0)*dif;
    }
    if(length(p-vec3(-10.0,0.0,10.0))<1.001){
    	col=vec3(0.0,1.0,1.0)*dif;
    }
    col = pow(col, vec3(.4545));	// gamma correction
    fragColor = vec4(col,1.0);
    
    //12/01/2021 fill z buffer but it doesn't work 
    //13/01/2021 Solved:https://computergraphics.stackexchange.com/questions/6308/why-does-this-gl-fragdepth-calculation-work
    vec4 pi=cameraInv*vec4(p.x,p.y,p.z,1.0);
    //projection gives a number from [-1,1] but depth must be from [0,1]
    vec4 Pclip = projection * pi;
    float ndc_depth = Pclip.z / Pclip.w;
    gl_FragDepth = (ndc_depth+1.0)/2.0;
    //gl_FragDepth = (
    //       (gl_DepthRange.diff * ndc_depth) +
    //        gl_DepthRange.near + gl_DepthRange.far) / 2.0;
}