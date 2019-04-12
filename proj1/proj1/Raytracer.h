#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "scene.h"
#include "camera.h"
#include <omp.h>

class RayTracer {
private:
	Camera* camera;
	Scene* scene;
	cv::Mat result;
	int imgHeight;
	int imgWidth;
	double maxLightLen = 1000000;
public:
	RayTracer(Camera* c,Scene* s,int height,int width):camera(c),scene(s),imgHeight(height),imgWidth(width){}
	cv::Mat getImg() { return result; }
	void showImg() { cv::imshow("raytracer",result); }
	void writeImg() { cv::imwrite("res.png", result); }

	Color trace(Ray* r,double length);
	void run();
};


#endif
