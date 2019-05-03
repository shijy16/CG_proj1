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
#pragma omp parallel
#pragma omp for schedule(dynamic,2)
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
	writeImg();
}

Color RayTracer::trace(Ray* r,double depth) {
	if (depth > MAX_DEPTH) return Color(0, 0, 0);

	IntersectPoint* inter = scene->getIntersectObj(*r);
	if (inter == NULL) {
		return Color(0, 0, 0);
	}

	Object* intersectObj = inter->obj;
	double intersectT = inter->t;
	Vector3 intersectPos = r->o + r->dir*intersectT;
	double reflect = intersectObj->getReflect();
	double refract = intersectObj->getRefract();
	double diffuse = intersectObj->getDiffuse();
	double specular = intersectObj->getSpecular();
	Vector3 N = intersectObj->getNormal(intersectPos);
	Color intersectColor = intersectObj->getColor(intersectPos);
	if (intersectObj->isLight()) {
		return intersectColor;
	}
	else {
		Color c = new Color();
		for (int i = 0; i < scene->getObjCnt(); i++) {
			Object* obj = scene->getObj(i);
			if (obj->isLight()) {

				if (diffuse > 0) {
					Vector3 L = obj->getLightCenter() - intersectPos;
					L.normalize();
					N.normalize();
					double dot = N*L;
					if (dot > 0) {
						////Âþ·´Éä
						//double diff = dot * diffuse;
						//obj->getColor(obj->getLightCenter());
						//c += Vector3::mul(obj->getColor(obj->getLightCenter()),intersectObj->getColor(intersectPos))*diff;
						
						//·´Éä
						double ref = (L - N*dot*2.0)*r->dir;
						if (ref > 0) {
							pow(ref, 8);
							c += intersectColor*specular;
						}
					}

				}
			}
		}
		if (reflect > 0) {
			Vector3 d = r->dir - N*(r->dir*N)*2;
			d.normalize();
			c += Vector3::mul(trace(new Ray(intersectPos + d*0.01, d), depth + 1),intersectColor)*reflect;
		}
		return c;
	}
	
	return Color(0, 0, 0);
}