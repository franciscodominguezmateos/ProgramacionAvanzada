class Cubo{
 public:
 Vector3D pos;
 Vector3D vel;
 Vector3D color;
 float s;
 inline Vector3D getPos(){return pos;}
 inline Vector3D getVel(){return vel;}
 inline Vector3D getCol(){return color;}
 inline float getS(){return s;}
 inline void setPos(Vector3D p){pos=p;}
 inline void setVel(Vector3D v){vel=v;}
 inline void setCol(Vector3D c){color=c;}
 inline void setS(float sp){s=sp;}
 void render();
};
void Cubo::render(){
   glPushMatrix();
     glColor3f(color.getX(),color.getY(),color.getZ());
     glTranslatef(pos.getX(),pos.getY(),pos.getZ());
     glutSolidCube(s);
   glPopMatrix();
}
