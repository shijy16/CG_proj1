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
	sprintf_s(date, 20, "result/%d%.2d%d.png", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
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
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[0] = int(c.getZ()*255) > 255 ? 255 : int(c.getZ() * 255);
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[1] = int(c.getY()*255) > 255 ? 255 : int(c.getY() * 255);
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[2] = int(c.getX()*255) > 255 ? 255 : int(c.getX() * 255);
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
				N.normalize();

				//点光源产生阴影
				float shadow = 1.0;
				if (obj->getType() == Object::SPHERE) {
					float L_len = L.getLength();
					L.normalize();
					Ray* inter2plight = new Ray(intersectPos + 0.01*L, L);
					for (int j = 0; j < scene->getObjCnt(); j++) {
						if (j == i) continue;
						float t = scene->getObj(j)->intersect(*inter2plight);
						//被遮挡
						if (t > 0.0f && t < L_len) {
							shadow = 0.0f;
							break;
						}
					}
				}


				if (diffuse > 0) {
					float dot = Vector3::dot(N,L);
					if (dot > 0) {
						//漫反射
						float diff = dot * diffuse*shadow;
						obj->getColor(obj->getLightCenter());
						c += Vector3::mul(obj->getColor(obj->getLightCenter()),intersectObj->getColor(intersectPos))*diff;
					}
				}

				//光源在物体上漫反射产生光晕
				if (specular > 0.0f) {
					//光源在物体表面反射光线
					Vector3 rf_l = L - N * (Vector3::dot(L, N))*2.0f;
					float dot = Vector3::dot(rf_l, r->dir);
					if (dot > 0) {
						float t = powf(dot, 20)*specular*shadow;
						c += t * obj->getColor(obj->getLightCenter());
					}
				}

			}
		}
		if (reflect > 0.0f) {
			Vector3 rf_light = r->dir - N*(Vector3::dot(r->dir,N))*2.0f;		//光碰到物体后反射
			rf_light.normalize();
			Color t = trace(new Ray(intersectPos + rf_light * 0.01f, rf_light),depth + 1);
			c += Vector3::mul(t,intersectColor)*reflect;	//反射光线出发点是物体外一点点
			/*if (t.getX() > 0.0f) {
				t.show();
				printf("\t");
				c.show();
				printf("\n");
			}*/
		}

		return c;
	}
	
	return Color(0, 0, 0);
}