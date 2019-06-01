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
	printf("RayTracer begin\n");
	result = cv::Mat::zeros(imgWidth, imgHeight,CV_8UC3);
	int last_obj = -1;
	int cur_obj = -1;
//#pragma omp parallel
//#pragma omp for schedule(dynamic,2)
	for (int i = 0; i < imgHeight; i++) {
		printf("%.2lf%%\r", i * 100.0 / imgHeight);
		for (int j = 0; j < imgWidth; j++) {
			Ray* r = camera->getCameraRay(i, j);
			float a = 0.0;
			Color c = trace(r,0,0,1.0f,a, cur_obj);
			free(r);
			//边缘超采样
			if (cur_obj != last_obj) {
				//printf("ohhh\n");
				last_obj = cur_obj;
				c = Color(0, 0, 0);
				#pragma omp parallel
				#pragma omp for schedule(dynamic,1)
				for (int tx = -10; tx < 11; tx++) {
					for (int ty = -10; ty < 11; ty++) {
						Ray* tr = camera->getCameraRay(float(i) + float(tx) / 20.0f, float(j) + float(ty) / 20.0f);
						//tr->show();
						c += trace(tr, 0, 0, 1.0f, a, cur_obj);
						free(tr);
					}
				}
				//printf("\n>>>>>>>>>>>\n");
				c = c / (21.0f*21.0f);
			}
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[0] = (int(c.getZ() * 255) > 255 ? 255 : int(c.getZ() * 255));
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[1] = (int(c.getY() * 255) > 255 ? 255 : int(c.getY() * 255));
			result.at<cv::Vec3b>(imgHeight - i - 1, j)[2] = (int(c.getX() * 255) > 255 ? 255 : int(c.getX() * 255));
		}
	}
	showImg();
	writeImg();
}

Color RayTracer::trace(Ray* r,int depth,float length,float refract_idx,float &inter_l,int &inter_id) {
	if (depth > MAX_DEPTH) return Color(0, 0, 0);
	if(length > MAX_LIGHT_LEN) return Color(0, 0, 0);
	inter_l = 100000.0f;
	IntersectPoint* inter = scene->getIntersectObj(*r);
	if (inter == NULL) {
		inter_id = -1;
		return Color(0, 0, 0);
	}

	Object* intersectObj = inter->obj;
	float intersectT = inter->t;
	inter_l = intersectT;
	Vector3 intersectPos = r->o + r->dir*intersectT;
	float reflect = intersectObj->getReflect();
	float refract = intersectObj->getRefract();
	float diffuse = intersectObj->getDiffuse();
	float specular = intersectObj->getSpecular();
	
	bool isInsideObj = inter->inside;
	inter_id = inter->obj_id;
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
						bool in;
						float t = scene->getObj(j)->intersect(*inter2plight,in);
						//被遮挡
						if (t > 0.0f && t < L_len) {
							shadow = 0.0f;
							break;
						}
					}
				}
				if (shadow <= 0.0f) continue;

				//漫反射
				if (diffuse > 0) {
					float dot = Vector3::dot(N,L);
					if (dot > 0) {
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
			float a = 0.0f;
			int temp = 0;
			Color t = trace(new Ray(intersectPos + rf_light * 0.01f, rf_light),depth + 1,length + inter->t, refract_idx,a,temp);
			c += Vector3::mul(t,intersectColor)*reflect;	//反射光线出发点是物体外一点点
			/*if (t.getX() > 0.0f) {
				t.show();
				printf("\t");
				c.show();
				printf("\n");
			}*/
		}

		//折射
		if (refract > 0.0f) {
			float n = refract_idx / refract;
			if (isInsideObj) N = N*(-1.0f);		//在物体内部
			float cosI = - Vector3::dot(N,r->dir);
			float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
			if (cosT2 > 0.0f){
				Vector3 T = (n * r->dir) + (n * cosI - sqrtf(cosT2)) * N;
				float inter_len = 10000.0;
				int t = 0;
				Color c_t = trace(new Ray(intersectPos + T * 0.001f, T), depth + 1,length + inter->t, refract,inter_len,t);
				Color absorbance = intersectColor * 0.0005f * (-inter_len);
				Color transparency = Color(expf(absorbance.getX()), expf(absorbance.getY()), expf(absorbance.getZ()));
				c += c_t* transparency;
			}
		}
		free(inter);
		return c;
	}
	
	return Color(0, 0, 0);
}