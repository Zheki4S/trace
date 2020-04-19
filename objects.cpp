#include<cmath>
#include<fstream>
#include "objects.h"
Point::Point(int x, int y, int z){
            this -> x = x;
            this -> y = y;
            this -> z = z;
}
Point::Point(){
	x = 0;
	y = 0;
	z = 0;
}
int Point::getX(){
	return x;
}
int Point::getY(){
	return y;
}
int Point::getZ(){
	return z;
}
Sphere::Sphere(const Point &p, int rad): center(p){
	this -> rad = rad;   
}
bool Sphere::if_point_exist(Point p){
	if(pow(center.getX() - p.getX(), 2) + pow(center.getY() - p.getY(), 2) + pow(center.getZ() - p.getZ(), 2) <= rad * rad)
		return true;
	else
		return false;
}
int Sphere::getRad(){
	return rad;
}
Point Sphere::getCenter(){
	return center;
}
Picture::Picture(const Point &cam, const Point &lig, const Sphere &sphere1, const Sphere &sphere2, int direction[3], int width, int height, int distance): sp1(sphere1), sp2(sphere2), eye(cam), light(lig){
	this -> width = width;
	this -> height = height;
	this -> distance = distance;
	this -> direction[0] = direction[0];
	this -> direction[1] = direction[1];
	this -> direction[2] = direction[2];
}
void Picture::draw(){
	std::ofstream img("view.ppm");
	img<<"P3"<<std::endl;
	img<<width<<" "<<height<<std::endl;
	img<<"255"<<std::endl;
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			unsigned int r = 88;
			unsigned int g = 124;
			unsigned int b = 236;
			int x0 = eye.getX();
			int y0 = eye.getY();
			int z0 = eye.getZ();
			int x1 = x0 - width / 2 + j;
			int y1 = y0 - height / 2 + i;
			int z1 = z0 + distance;
			double l = hypot(hypot(x1 - x0, y1 - y0), z1 - z0);
			for(int t = 0; t < 600; t++){
				Point foton(x0 + (x1 - x0) / l * t, y0 + (y1 - y0) / l * t, z0 + direction[2] *(z1 - z0) / l * t);
				if(sp1.if_point_exist(foton)){
					Point center = sp1.getCenter();
					int ax = light.getX() - foton.getX();
					int ay = light.getY() - foton.getY();
					int az = light.getZ() - foton.getZ();
					int bx = center.getX() - foton.getX();
					int by = center.getY() - foton.getY();
					int bz = center.getZ() - foton.getZ();
					double cos = (ax * bx + ay * by + az * bz) / sqrt(ax * ax + ay * ay + az * az) / sqrt(bx * bx + by * by + bz * bz);
					r = 0 + (int)(185 * std::max(0., cos));
					g = 70 + (int)(185 * std::max(0., cos));
					b = 0 + (int)(185 * std::max(0., cos));
					break;
				}
				if(sp2.if_point_exist(foton)){
					Point center = sp2.getCenter();
					int ax = light.getX() - foton.getX();
					int ay = light.getY() - foton.getY();
					int az = light.getZ() - foton.getZ();
					int bx = center.getX() - foton.getX();
					int by = center.getY() - foton.getY();
					int bz = center.getZ() - foton.getZ();
					double cos = (ax * bx + ay * by + az * bz) / sqrt(ax * ax + ay * ay + az * az) / sqrt(bx * bx + by * by + bz * bz);
					r = 100 + (int)(105 * std::max(0., cos));
					g = 0 + (int)(205 * std::max(0., cos));
					b = 150 + (int)(105 * std::max(0., cos));
					break;
				}
			}
			img<<r<<" "<<g<<" "<<b<<std::endl;
			r = 88;
			g = 124;
			b = 236;
		}
	}
	img.close();
}