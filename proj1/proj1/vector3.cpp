#include "vector3.h"

Vector3::Vector3(Vector3* v){
	x = v->x;
	y = v->y;
	z = v->z;
}

Vector3::Vector3() {
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(float a, float b, float c){
	x = a;
	y = b;
	z = c;
}

float Vector3::getX(){
	return x;
}

float Vector3::getY(){
	return y;
}

float Vector3::getZ(){
	return z;
}


void Vector3::show(){
	printf("%f,%f,%f",x,y,z);
}

void Vector3::normalize(){
	float s = sqrt(x*x + y*y + z*z);
	x /= s;
	y /= s;
	z /= s;
}

float Vector3::getLength() {
	return sqrt(x*x + y*y + z*z);
}
