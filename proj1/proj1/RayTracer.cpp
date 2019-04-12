#include "Raytracer.h"

void RayTracer::run() {
	printf("RayRtacer begin\n");
	result = cv::Mat::zeros(imgWidth, imgHeight,CV_8UC3);
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			Ray* r = camera->getCameraRay(i, j);
			Color c = trace(r,0);
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[0] = int(c.getZ());
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[1] = int(c.getY());
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[2] = int(c.getX());
		}
	}
	showImg();
	cv::waitKey();
	writeImg();
}

Color RayTracer::trace(Ray* r,double length) {
	if (length > maxLightLen) return Color(0, 0, 0);

	IntersectPoint* inter = scene->getIntersectObj(*r);
	if (inter == NULL) {
		return Color(0, 0, 0);
	}

	Object* intersectObj = inter->obj;
	double intersectT = inter->t;
	Vector3 intersectPos = r->o + r->dir*intersectT;
	if (intersectObj->isLight()) {
		return intersectObj->getColor(intersectPos)*((double)1 - length/maxLightLen);
	}
	
	return Color(0, 0, 0);
}