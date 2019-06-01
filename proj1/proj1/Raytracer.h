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
	float MAX_LIGHT_LEN = 1000000;
	int MAX_DEPTH = 10;
public:
	RayTracer(Camera* c, Scene* s, int height, int width) :camera(c), scene(s), imgHeight(height), imgWidth(width) {}
	cv::Mat getImg() { return result; }
	void showImg();
	void writeImg();
	float get_shadow(Object* obj, Vector3 intersectPos, int i, Vector3& L);

	Color trace(Ray* r, int depth, float length, float refract_idx, float &inter_l, int &inter_id);
	void run();
};


#endif
