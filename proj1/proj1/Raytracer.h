#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "scene.h"
#include "camera.h"
#include <omp.h>
#include <time.h>

class RayTracer {
private:
	Camera* camera;
	Scene* scene;
	cv::Mat result;
	int imgHeight;
	int imgWidth;
	float maxLightLen = 1000000;
	float MAX_DEPTH = 1;
public:
	RayTracer(Camera* c,Scene* s,int height,int width):camera(c),scene(s),imgHeight(height),imgWidth(width){}
	cv::Mat getImg() { return result; }
	void showImg();
	void writeImg();

	Color trace(Ray* r,float length);
	void run();
};


#endif
