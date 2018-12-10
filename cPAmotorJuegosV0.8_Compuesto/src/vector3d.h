#ifndef VECTOR3D_H_
#define VECTOR3D_H_
#include <ostream>
class Vector3D{
 private:
  float x,y,z;
 public:
  Vector3D():Vector3D(0,0,0){}
  Vector3D(float x,float y,float z):x(x)  ,y(y)  ,z(z){}
  Vector3D(const Vector3D &v)      :x(v.x),y(v.y),z(v.z){}
  Vector3D operator+(Vector3D b);
  Vector3D operator-(Vector3D b);
  Vector3D operator*(float f);
  inline float getX(){return x;}
  inline float getY(){return y;}
  inline float getZ(){return z;}
  inline void setX(float px){x=px;}
  inline void setY(float py){y=py;}
  inline void setZ(float pz){z=pz;}
  friend std::ostream &operator << (std::ostream &os, const Vector3D &v);
};
inline std::ostream &operator<<(std::ostream &os, const Vector3D &v){
    os << '<' << v.x << ',' << v.y << ',' << v.z << '>';
    return os;
}
#endif
