#ifndef COMMON_H
#define COMMON_H
#include "vector3.h"
#include <iostream>
#include <opencv2/opencv.hpp>

#define PI 3.1415926
typedef Vector3 Color;

class Point {
public:
	float x, y;
	Point(float _x,float _y):x(_x),y(_y){}
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
	float radius;
	float brightness;
	Light():o(Vector3(0,0,0)),color(Color(0,0,0)),radius(10000),brightness(0) {}
	Light(Vector3 origin, Color c, float r,float b) : o(origin),color(c),radius(r),brightness(b){}
	void set(Vector3 origin, Color c, float r,float b) {
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


#endif
