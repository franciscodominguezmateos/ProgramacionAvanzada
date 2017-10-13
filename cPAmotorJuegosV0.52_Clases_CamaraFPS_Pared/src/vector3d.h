#ifndef VECTOR3D_H_
#define VECTOR3D_H_
class Vector3D{
 private:
  double x,y,z;
 public:
  Vector3D():Vector3D(0,0,0){}
  Vector3D(double x,double y,double z):x(x),y(y),z(z){}
  Vector3D operator+(Vector3D b);
  Vector3D operator-(Vector3D b);
  Vector3D operator*(double d);
  Vector3D operator/(double d);
  inline double getX(){return x;}
  inline double getY(){return y;}
  inline double getZ(){return z;}
  inline void setX(double px){x=px;}
  inline void setY(double py){y=py;}
  inline void setZ(double pz){z=pz;}
};
#endif
