#ifndef VECTOR3D_H_
#define VECTOR3D_H_
class Vector3D{
 private:
  float x,y,z;
 public:
  Vector3D():Vector3D(0,0,0){}
  Vector3D(float x,float y,float z):x(x),y(y),z(z){}
  Vector3D operator+(Vector3D b);
  Vector3D operator-(Vector3D b);
  inline float getX(){return x;}
  inline float getY(){return y;}
  inline float getZ(){return z;}
  inline void setX(float px){x=px;}
  inline void setY(float py){y=py;}
  inline void setZ(float pz){z=pz;}
};
#endif
