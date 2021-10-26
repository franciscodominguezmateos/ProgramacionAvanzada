 #version 330 core
 //layout(location = 0) in vec3 vertexPosition_modelspace;
 in vec3 position;
 in vec2 textureCoords;
 in vec3 normal;
 
 //out vec3 colour;
 out vec2 pass_textureCoords;
 out vec3 surfaceNormal;
 out vec3 toLightVector;
 
 uniform mat4 T;
 uniform mat4 cameraView;
 uniform mat4 projection;
 uniform vec3 lightPosition;
 
 void main(){
  vec4 worldPosition=T*vec4(position,1);
  
  gl_Position=projection*cameraView*worldPosition;
  pass_textureCoords=textureCoords;
  surfaceNormal=(T*vec4(normal,0.0)).xyz;
  toLightVector= lightPosition-worldPosition.xyz;
}
