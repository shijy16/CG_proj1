#ifndef BUILDER_H
#define BUILDER_H
#include "camera.h"
#include "scene.h"
#include "Raytracer.h"

class Builder {
	Scene* scene;
	Camera* camera;
	int SIZE;
public:
	Builder(int s) { SIZE = s; }
	void buildCamera(Vector3 eye, Vector3 origin, Vector3 dx, Vector3 dy, int x, int y);
	void buildScene(Object** objs, int num);
	void buildScene2(Object** objs, int num);
	void buildScene3(Object** objs, int num);
	cv::Mat rayTracer_begin();
};

#endif