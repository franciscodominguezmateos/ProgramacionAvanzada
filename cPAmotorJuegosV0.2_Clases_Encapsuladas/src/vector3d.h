class Vector3D{
 private:
  float x,y,z;
 public:
  Vector3D operator+(Vector3D b);
  Vector3D operator-(Vector3D b);
  inline float getX(){return x;}
  float getY();
  float getZ();
  inline void setX(float px){x=px;}
  inline void setY(float py){y=py;}
  inline void setZ(float pz){z=pz;}
};
inline float Vector3D::getY(){
 return y;
}
inline float Vector3D::getZ(){
 return z;
}
Vector3D Vector3D::operator+(Vector3D b){
 Vector3D ret;
 ret.x=x+b.x;
 ret.y=y+b.y;
 ret.z=z+b.z;
 return ret;
}
Vector3D Vector3D::operator-(Vector3D b){
 Vector3D ret;
 ret.x=x-b.x;
 ret.y=y-b.y;
 ret.z=z-b.z;
 return ret;
}
