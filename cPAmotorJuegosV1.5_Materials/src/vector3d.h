#ifndef VECTOR3D_H_
#define VECTOR3D_H_
#include <cmath>
#include <ostream>
#include <cassert>

class Vector3D{
 private:
  double x,y,z;
 public:
  Vector3D():Vector3D(0,0,0){}
  Vector3D(double x,double y,double z=0):x(x)  ,y(y)  ,z(z){}
  Vector3D(const Vector3D &v)      :x(v.x),y(v.y),z(v.z){}
  Vector3D operator+(Vector3D b);
  Vector3D operator-(Vector3D b);
  inline Vector3D operator - (){return Vector3D(-x,-y,-z);}
  Vector3D operator*(double f);
  Vector3D operator/(double f);
  inline double operator * (const Vector3D &v2){
	  Vector3D &v1=*this;
      return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }  inline Vector3D crossProduct(const Vector3D &v2){
	  Vector3D &v1=*this;
      return Vector3D(v1.y*v2.z - v1.z*v2.y,
                    v1.z*v2.x - v1.x*v2.z,
                    v1.x*v2.y - v1.y*v2.x);
  }
  inline double lengthSquared(){
      return x*x + y*y + z*z;
  }

  inline double length(){
      return sqrt(lengthSquared());
  }
  inline bool equalsZero(double a){
  	//double almost never is 0.0
  	//This value shoud be a const
  	return fabs(a)<0.000001;
  }
  inline void normalize(){
      double magnitude = length();
      assert(!equalsZero(magnitude));

      magnitude = 1.0f / magnitude;

      x *= magnitude;
      y *= magnitude;
      z *= magnitude;
  }
  inline double getX(){return x;}
  inline double getY(){return y;}
  inline double getZ(){return z;}
  inline void setX(double px){x=px;}
  inline void setY(double py){y=py;}
  inline void setZ(double pz){z=pz;}
  friend std::ostream &operator << (std::ostream &os, const Vector3D &v);
};
inline std::ostream &operator<<(std::ostream &os, const Vector3D &v){
    os << '<' << v.x << ',' << v.y << ',' << v.z << '>';
    return os;
}
#endif
