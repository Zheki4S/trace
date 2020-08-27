#pragma once
#include<vector>
#include<cmath>
#include<limits.h>
class Vector{
	public:
		Vector(double, double, double);
		Vector();
        void setX(double x);
        void setY(double y);
        void setZ(double z);
		double getX() const;
		double getY() const;
		double getZ() const;
        void Print();
        Vector& operator = (const Vector&);
        Vector operator + (const Vector&);
        Vector operator - (const Vector&);
        double operator * (const Vector&);
        Vector operator * (const double&);
        friend Vector operator * (const double&, const Vector&);
        Vector operator / (const double&);
        double norm();
	private:
		double x, y, z;
};
Vector operator * (const double&, const Vector&);
Vector vp(const Vector&, const Vector&);
double square_equal(int, double, double, double);
int phong_model(int, Vector, Vector, Vector, Vector, double);
void trio(Vector, Vector &, Vector &);

class Object {
    public:
        bool if_point_exist(Vector, Vector, Vector&);
        void setColor(Vector);
        Vector getColor();
    private:
        Vector color; 
};

class Sphere : public Object {
    public:
        Sphere(const Vector&, double);
        bool if_point_exist(Vector, Vector, Vector&);
        double getRad();
        Vector getCenter();
    private:
        Vector center;
        double rad;  
};
class Square : public Object {
    public:
        Square(const Vector&, const Vector&);
        bool if_point_exist(Vector, Vector, Vector&);
        double getA();
        double getB();
        Vector getVertex();
    private:
        Vector vertex;
        double a, b; 
};
class Picture{
    public:
        Picture(const Vector&, const Vector&, const Vector&, const std::vector<Sphere>&, int, int, double);
        Vector* draw();
    private:
        int width;
        int height;
        double distance;
        Vector d;
        std::vector<Sphere> obj;
        Vector eye;
        Vector light;
};