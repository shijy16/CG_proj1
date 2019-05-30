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
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[0] = int(c.getZ()*255);
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[1] = int(c.getY()*255);
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[2] = int(c.getX()*255);
		}
	}
	showImg();
	writeImg();
}

Color RayTracer::trace(Ray* r,float depth) {
	if (depth > MAX_DEPTH) return Color(0, 0, 0);

	IntersectPoint* inter = scene->getIntersectObj(*r);
	if (inter == NULL) {
		return Color(0, 0, 0);
	}

	Object* intersectObj = inter->obj;
	float intersectT = inter->t;
	Vector3 intersectPos = r->o + r->dir*intersectT;
	float reflect = intersectObj->getReflect();
	float refract = intersectObj->getRefract();
	float diffuse = intersectObj->getDiffuse();
	float specular = intersectObj->getSpecular();
	Vector3 N = intersectObj->getNormal(intersectPos);	//交点法向量
	Color intersectColor = intersectObj->getColor(intersectPos);
	if (intersectObj->isLight()) {
		return intersectColor;
	}
	else {
		Color c = new Color();
		for (int i = 0; i < scene->getObjCnt(); i++) {
			Object* obj = scene->getObj(i);
			if (obj->isLight()) {
				Vector3 L = obj->getLightCenter() - intersectPos;	//交点到光源
				L.normalize();
				N.normalize();
				if (diffuse > 0) {
					float dot = Vector3::dot(N,L);
					if (dot > 0) {
						//漫反射
						float diff = dot * diffuse;
						obj->getColor(obj->getLightCenter());
						c += Vector3::mul(obj->getColor(obj->getLightCenter()),intersectObj->getColor(intersectPos))*diff;
					
					}

				}
			}
		}
		if (reflect > 0.0f) {
			Vector3 rf_light = r->dir - N*(Vector3::dot(r->dir,N))*2.0;		//反射光线
			rf_light.normalize();
			c += Vector3::mul(trace(new Ray(intersectPos + rf_light*0.01, rf_light), depth + 1),intersectColor)*reflect;	//反射光线出发点是物体外一点点
		}

		return c;
	}
	
	return Color(0, 0, 0);
}