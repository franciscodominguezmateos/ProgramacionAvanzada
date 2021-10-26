#version 330 core
in vec2 pass_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;

out vec4 out_color;

uniform sampler2D textureSampler;
uniform vec3 lightColor;

void main(){
  vec3 unitNormal=normalize(surfaceNormal);
  vec3 unitLight =normalize(toLightVector);
  float ndot=dot(unitNormal,unitLight);
  float brightness=max(ndot,0.2);
  out_color = brightness*vec4(lightColor,1)*texture(textureSampler,pass_textureCoords);
}