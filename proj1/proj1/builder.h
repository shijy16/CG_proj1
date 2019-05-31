#ifndef BUILDER_H
#define BUILDER_H
#include "camera.h"
#include "scene.h"
#include "Raytracer.h"

class Builder {
	Scene* scene;
	Camera* camera;
public:
	Builder(){}
	void buildCamera(Vector3 eye, Vector3 origin, Vector3 dx, Vector3 dy, int x, int y);
	void buildScene(Object** objs, int num, float SIZE);
	cv::Mat rayTracer_begin();
};

#endif