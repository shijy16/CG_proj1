#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "scene.h"
#include "camera.h"
#include <omp.h>
#include <time.h>
#include<random>

class RayTracer {
private:
	Camera* camera;
	Scene* scene;
	cv::Mat result;
	int scale;
	int imgHeight;
	int imgWidth;
	double MAX_LIGHT_LEN = 1000000;
	int MAX_DEPTH = 3;
public:
	RayTracer(Camera* c, Scene* s, int height, int width,int sc) :camera(c), scene(s), imgHeight(height), imgWidth(width), scale(sc){}
	cv::Mat getImg() { return result; }
	void showImg();
	void writeImg();
	double get_shadow(Object* obj, Vector3 intersectPos, int i, Vector3& L);

	Color trace(Ray* r, int depth, double length, double refract_idx, double &inter_l, int &inter_id);
	void run();
};


#endif
