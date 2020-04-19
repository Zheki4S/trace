#pragma once
class Point{
	public:
		Point(int, int, int);
		Point();
		int getX();
		int getY();
		int getZ();
	private:
		int x, y, z;
};
class Sphere{
    public:
        Sphere(const Point&, int);
        bool if_point_exist(Point);
        int getRad();
        Point getCenter();
    private:
        Point center;
        int rad;  
};
class Picture{
    public:
        Picture(const Point&, const Point&, const Sphere&, const Sphere&, int[3], int, int, int);
        void draw();
    private:
        int width;
        int height;
        int distance;
        int direction[3];
        Sphere sp1;
        Sphere sp2;
        Point eye;
        Point light;
};