#include "objects.h"
Vector::Vector(double x, double y, double z){
            this -> x = x;
            this -> y = y;
            this -> z = z;
}
Vector::Vector(){
	x = 0;
	y = 0;
	z = 0;
}
Vector& Vector::operator = (const Vector& other){
	this -> x = other.x;
	this -> y = other.y;
	this -> z = other.z;
	return *this;
}
Vector Vector::operator + (const Vector& other){
	Vector res(this -> x + other.x, this -> y + other.y, this -> z + other.z);
 	return res;
}
Vector Vector::operator - (const Vector& other){
	Vector res(this -> x - other.x, this -> y - other.y, this -> z - other.z);
 	return res;
}
double Vector::operator * (const Vector& other){
	return this -> x * other.x + this -> y * other.y + this -> z * other.z;
}
Vector Vector::operator * (const double& k){
	Vector res(x * k, y * k, z * k);
	return res;
}
Vector Vector::operator / (const double& k){
	Vector res(x / k, y / k, z / k);
	return res;
}
double Vector::norm(){
	return sqrt(x * x + y * y + z * z);
}
void Vector::setX(double x){
	this -> x = x;
}
void Vector::setY(double y){
	this -> y = y;
}
void Vector::setZ(double z){
	this -> z = z;
}
double Vector::getX() const{
	return x;
}
double Vector::getY() const{
	return y;
}
double Vector::getZ() const{
	return z;
}
Vector operator * (const double& k, const Vector& vec) {
	Vector res(vec.x * k, vec.y * k, vec.z * k);
	return res;
}
Vector vp(const Vector& v1, const Vector& v2){
	Vector res(v1.getY() * v2.getZ() - v1.getZ() * v2.getY(), v1.getZ() * v2.getX() - v1.getX() * v2.getZ(), v1.getX() * v2.getY() - v1.getY() * v2.getX());
	return res;
}
double square_equal(int s, double a, double b, double c){
	double D = b * b - 4 * a * c;
	switch(s){
		case 1:
			return (-b + sqrt(D)) / 2 / a;
		case 2:
			return (-b - sqrt(D)) / 2 / a;
		default:
			return 0;
	}
}
void Object::setColor(Vector c){
	color = c;
}
Vector Object::getColor(){
	return color;
}
Sphere::Sphere(const Vector &p, double rad): center(p){
	this -> rad = rad;   
}
int phong_model(int c0, Vector surface, Vector eye, Vector norm, Vector light, double shine){
	Vector L = light - surface;
	L = L / L.norm();
	Vector N = norm;
	N = N / N.norm();
	Vector Rm = 2 * (L * N) * N - L;
	Rm = Rm / Rm.norm();
	Vector V = eye - surface;
	V = V / V.norm();
	double c;
	c = c0 - c0 * std::max(0., -1 * L * N) + (255 - c0) * pow(std::max(0., Rm * V), shine);
	return c;	
}
void trio(Vector dir, Vector &kdir, Vector &ndir){
	dir = dir / dir.norm();
   	double dx, dy, dz, kx, ky, kz;
   	kz = 0;
   	dx = dir.getX();
  	dy = dir.getY();
   	dz = dir.getZ();
    if(dx * dx + dy * dy == 0)
        kdir = Vector(1, -1, 0);
    else {
        kx = square_equal(1, dx * dx + dy * dy, 2 * dx * dz * kz, dz * dz * kz * kz - dy * dy + dy * dy * kz * kz);
      	ky = square_equal(2, dx * dx + dy * dy, 2 * dy * dz * kz, dz * dz * kz * kz - dx * dx + dx * dx * kz * kz);
       	kdir = Vector(kx, ky, 0);
    }
  	ndir = vp(dir, kdir);
   	kdir = kdir / kdir.norm();
  	ndir = ndir / ndir.norm();
}
bool Sphere::if_point_exist(Vector p, Vector cam, Vector &surf){
	Vector pc;
	Vector dir;
	Vector test;
	pc = center - p;
	dir = p - cam;
	double cos_a = pc * dir / pc.norm() / dir.norm();
	if(cos_a > 0){
		if(sqrt(1 - cos_a * cos_a) * pc.norm() <= rad){
			double l = pc.norm() * cos_a - sqrt(rad * rad - pc.norm() * pc.norm() * (1 - cos_a * cos_a));
			dir = dir / dir.norm();
			surf = dir * l;
			surf = surf + p;
			return true;
		}
		else
			return false;
	} else
		return false;
}
double Sphere::getRad(){
	return rad;
}
Vector Sphere::getCenter(){
	return center;
}
Picture::Picture(const Vector &cam, const Vector &dir, const Vector &lig, const std::vector<Sphere> &objects, int width, int height, double distance):  eye(cam), d(dir), light(lig), obj(objects){
	this -> width = width;
	this -> height = height;
	this -> distance = distance;
}
Vector* Picture::draw(){
	Vector k;
    d = d / d.norm();
   	double dx, dy, dz, kx, ky, kz;
   	kz = 0;
   	dx = d.getX();
  	dy = d.getY();
   	dz = d.getZ();
    if(dx * dx + dy * dy == 0)
        k = Vector(1, -1, 0);
    else {
        kx = square_equal(1, dx * dx + dy * dy, 2 * dx * dz * kz, dz * dz * kz * kz - dy * dy + dy * dy * kz * kz);
      	ky = square_equal(2, dx * dx + dy * dy, 2 * dy * dz * kz, dz * dz * kz * kz - dx * dx + dx * dx * kz * kz);
       	k = Vector(kx, ky, 0);
    }
  	Vector n = vp(d, k);
   	k = k / k.norm();
  	n = n / n.norm();
  	Vector p;
  	Vector surface;
  	Vector ray;
	Vector rad;
  	double dif;
  	double refl;
  	double l;
  	double lmin;
  	int kmin;
  	double r, g, b;
	Vector *pixels = new Vector[height * width];
	for(int i = -height / 2; i < height / 2; i++){
		for(int j = -width / 2; j < width / 2; j++){
			kmin = -1;	
			p = (eye + d * distance) + (n * i) + (k * j);
			lmin = INT_MAX;
			for(int k = 0; k < obj.size(); k++){
				if(obj[k].if_point_exist(p, eye, surface)){
					if((surface - eye).norm() < lmin){
						lmin = (surface - eye).norm();
						kmin = k;
					}	
				} 
			}
			if(kmin == -1){
				r = 88;
				g = 124;
				b = 236;
			} else {
				obj[kmin].if_point_exist(p, eye, surface);
				rad = surface - obj[kmin].getCenter();
				double r0 = obj[kmin].getColor().getX();
				double g0 = obj[kmin].getColor().getY();
				double b0 = obj[kmin].getColor().getZ();
				double shine = 200;
				r = phong_model(r0, surface, p, rad, light, shine);
				g = phong_model(g0, surface, p, rad, light, shine);
				b = phong_model(b0, surface, p, rad, light, shine);
			}
			pixels[(i + height / 2) * width + (j + width / 2)] = Vector(r, g, b);
		}
	}
	return pixels;
}