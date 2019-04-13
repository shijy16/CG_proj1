#include "Raytracer.h"

void RayTracer::showImg() {
	cv::imshow("raytracer", result); 
}

void RayTracer::writeImg() {
	struct tm t;
	time_t now; 
	time(&now);
	localtime_s(&t, &now);
	char* date = new char[20];
	sprintf_s(date, 20, "%d%.2d%d.png", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
	cv::imwrite(std::string(date), result);
}

void RayTracer::run() {
	printf("RayRtacer begin\n");
	result = cv::Mat::zeros(imgWidth, imgHeight,CV_8UC3);
	for (int i = 0; i < imgHeight; i++) {
		printf("%.2lf%%\r", i * 100.0 / imgHeight);
		for (int j = 0; j < imgWidth; j++) {
			Ray* r = camera->getCameraRay(i, j);
			Color c = trace(r,0);
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[0] = int(c.getZ());
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[1] = int(c.getY());
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[2] = int(c.getX());
		}
	}
	showImg();
	cv::waitKey(0);
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