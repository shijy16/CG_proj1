#ifndef COMMON_H
#define COMMON_H
#include "vector3.h"
#include <iostream>
#include <opencv2/opencv.hpp>

#define PI 3.1415926
#define EPSILON 0.0001
#define init3x3matrix(m) {m = new double*[3];for (int i = 0; i < 3; i++) {m[i] = new double[3];for (int j = 0; j < 3; j++) {m[i][j] = 0;}} }
typedef Vector3 Color;

class Point {
public:
	double x, y;
	Point(double _x,double _y):x(_x),y(_y){}
};

class Ray{
public:
	Vector3 o;
	Vector3 dir;
	Ray(Vector3 origin, Vector3 direction) :o(origin), dir(direction) { dir.normalize(); };
	Ray(Ray& r) : o(r.o), dir(r.dir) {};
	void set(Vector3 origin, Vector3 direction) {
		o = origin;
		dir = direction;
		dir.normalize();
	}
	void show() {
		printf("Ray:");
		o.show();
		printf("->");
		dir.show();
		printf("\n");
	}
};

class Light {
public:
	Vector3 o;
	Color color;
	double radius;
	double brightness;
	Light():o(Vector3(0,0,0)),color(Color(0,0,0)),radius(10000),brightness(0) {}
	Light(Vector3 origin, Color c, double r,double b) : o(origin),color(c),radius(r),brightness(b){}
	void set(Vector3 origin, Color c, double r,double b) {
		o = origin;
		color = c;
		radius = r;
		brightness = b;
	}
	void show() {
		printf("Light:\t");
		o.show();
		printf(",color:");
		color.show();
		printf(",radius:%f",radius);
		printf("\n");
	}
};

//包围盒
class AABBPlane {
	Vector3 n;
	double d;
public:
	AABBPlane() { n = Vector3(); d = 0.0f; }
	AABBPlane(Vector3 nor,double dd) {
		n = nor;
		d = dd;
		n.normalize();
	}
	double intersect(Ray& r) {
		double res = -1.0;
		double temp = Vector3::dot(r.dir,n);
		if (temp != 0.0) {
			res = -(Vector3::dot(n,r.o) - d) / temp;
			if (res <= 0.0) res = -1.0;
		}
		return res;
	}

};

class AABB {
private:
	double minX, minY, maxX, maxY, minZ, maxZ;
	//上下左右前后
	AABBPlane myPlane[6];
public:
	AABB(){}
	AABB(double minx, double maxx, double miny, double maxy, double minz, double maxz) {
		minX = minx;
		maxX = maxx;
		minY = miny;
		maxY = maxy;
		minZ = minz;
		maxZ = maxz;
		myPlane[0] = AABBPlane(Vector3(0, 0, 1), maxZ);
		myPlane[1] = AABBPlane(Vector3(0, 0, -1),minZ);
		myPlane[2] = AABBPlane(Vector3(0, -1, 0), minY);
		myPlane[3] = AABBPlane(Vector3(0, 1, 0), maxY);
		myPlane[4] = AABBPlane(Vector3(1, 0, 0), maxX);
		myPlane[5] = AABBPlane(Vector3(-1, 0, 0), minX);
	}

	bool inAABB(Vector3 p) {
		if (p.getX() <= maxX && p.getX() >= minX && p.getY() <= maxY && p.getY() >= minY && p.getZ() <= maxZ && p.getZ() >= minZ) {
			return true;
		}
		return false;
	}

	bool intersect(Ray& r) {
		r.dir.normalize();
		for (int i = 0; i < 6; i++) {
			double dist = myPlane[i].intersect(r);
			if (dist > 0.0f) {
				if (inAABB(r.o + r.dir*dist)) {
					return true;
				}
			}
		}
		return false;
	}
};

#endif
