#include"objects.h"
int main(){
    Point c1(300, 600, 400); // центр сферы 1
    Sphere sp1(c1, 300); // сфера 1
    Point c2(700, 300, 700); // центр сферы 2
    Sphere sp2(c2, 400); // сфера 2   
    int dir1[3] = {0, 0, 1}; // направление обзора, пока только {0, 0, 1} или {0, 0, -1}
    Point eye(500, 500, -100); // точка обзора
    Point light(10000, 10000, 10000); //точка освещения
    Picture p1(eye, light, sp1, sp2, dir1, 600, 480, 100);
    p1.draw();
    return 0;
}  