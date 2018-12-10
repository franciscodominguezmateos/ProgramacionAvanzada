#ifndef VECTOR3D_H_
#define VECTOR3D_H_
#include <cmath>
#include <ostream>
#include <cassert>

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
  inline float operator * (const Vector3D &v2){
	  Vector3D &v1=*this;
      return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }  inline Vector3D crossProduct(const Vector3D &v2){
	  Vector3D &v1=*this;
      return Vector3D(v1.y*v2.z - v1.z*v2.y,
                    v1.z*v2.x - v1.x*v2.z,
                    v1.x*v2.y - v1.y*v2.x);
  }
  inline float lengthSquared(){
      return x*x + y*y + z*z;
  }

  inline float length(){
      return sqrt(lengthSquared());
  }
  inline bool equalsZero(float a){
  	//float almost never is 0.0
  	//This value shoud be a const
  	return fabs(a)<0.00001;
  }
  inline void normalize(){
      float magnitude = length();
      assert(!equalsZero(magnitude));

      magnitude = 1.0f / magnitude;

      x *= magnitude;
      y *= magnitude;
      z *= magnitude;
  }
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
