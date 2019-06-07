#include "Raytracer.h"
#define WIN

void RayTracer::showImg() {
	cv::imshow("raytracer", result); 
}

void RayTracer::writeImg() {
#ifdef WIN
	struct tm t;
	time_t now; 
	time(&now);
	localtime_s(&t, &now);
	char* date = new char[20];
	sprintf_s(date, 20, "result/%d%.2d%d.png", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
	cv::imwrite(std::string(date), result);
#endif
#ifdef UBUNTU
	cv::imwrite("result/ubuntu.png", result);
#endif
}


void RayTracer::run() {
	printf("RayTracer begin\n");
	result = cv::Mat::zeros(imgWidth, imgHeight,CV_8UC3);
	int last_obj = -1;
	int cur_obj = -1;
//#pragma omp parallel
//#pragma omp for schedule(dynamic,2)
	//for (int i = 0; i < imgWidth; i++) {
	//	printf("%.2lf%%\r", i * 100.0 / imgWidth);
	//	for (int j = 0; j < imgHeight; j++) {
	//		Ray* r = camera->getCameraRay(i, j);
	//		double a = 0.0;
	//		Color c = trace(r,0,0,1.0f,a, cur_obj);
	//		free(r);
	//		//边缘超采样
	//		if (cur_obj != last_obj) {
	//			//printf("ohhh\n");
	//			last_obj = cur_obj;
	//			c = Color(0, 0, 0);
	//			#pragma omp parallel
	//			#pragma omp for schedule(dynamic,1)
	//			for (int tx = -10; tx < 11; tx++) {
	//				for (int ty = -10; ty < 11; ty++) {
	//					Ray* tr = camera->getCameraRay(double(i) + double(tx) / 20.0f, double(j) + double(ty) / 20.0f);
	//					//tr->show();
	//					c += trace(tr, 0, 0, 1.0f, a, cur_obj);
	//					free(tr);
	//				}
	//			}
	//			//printf("\n>>>>>>>>>>>\n");
	//			c = c / (21.0f*21.0f);
	//		}
	//		result.at<cv::Vec3b>(imgWidth - i - 1, j)[0] = (int(c.getZ() * 255) > 255 ? 255 : int(c.getZ() * 255));
	//		result.at<cv::Vec3b>(imgWidth - i - 1, j)[1] = (int(c.getY() * 255) > 255 ? 255 : int(c.getY() * 255));
	//		result.at<cv::Vec3b>(imgWidth - i - 1, j)[2] = (int(c.getX() * 255) > 255 ? 255 : int(c.getX() * 255));
	//	}
	//}

	for (int i = 0; i < imgWidth; i++) {
		if (i % 20 == 0) {
			cv::imwrite("result/cur.png", result);
		}
#ifdef WIN
		printf("sampling: %.2lf% %\r", i * 100.0 / imgWidth);
#endif
#ifdef UBUNTU
		printf("sampling: %.2lf% %\n\r", i * 100.0 / imgWidth);
#endif
		for (int j = 0; j < imgHeight; j++) {
			Ray* r = camera->getCameraRay(i, j);
			double a = 0.0;
			Color c = trace(r, 0, 0, 1.0f, a, cur_obj);
			free(r);
			result.at<cv::Vec3b>(imgWidth - i - 1, j)[0] = (int(c.getZ() * 255) > 255 ? 255 : int(c.getZ() * 255));
			result.at<cv::Vec3b>(imgWidth - i - 1, j)[1] = (int(c.getY() * 255) > 255 ? 255 : int(c.getY() * 255));
			result.at<cv::Vec3b>(imgWidth - i - 1, j)[2] = (int(c.getX() * 255) > 255 ? 255 : int(c.getX() * 255));
		}
	}
	//重采样
	for (int i = 0; i < 0; i++) {
		if (i % 20 == 0) {
			cv::imwrite("result/cur.png", result);
	}
#ifdef WIN
		printf("resampling: %.2lf% %\r", i * 100.0 / imgWidth);
#endif
#ifdef UBUNTU
		printf("resampling: %.2lf% %\n\r", i * 100.0 / imgWidth);
#endif
		for (int j = 0; j < imgHeight; j++) {
			if ((i == 0 || result.at<cv::Vec3b>(imgWidth - i - 1, j) == result.at<cv::Vec3b>(imgWidth - i, j) && (i == imgWidth - 1 || result.at<cv::Vec3b>(imgWidth - i - 1, j) == result.at<cv::Vec3b>(imgWidth - i - 2, j)) &&
				(j == 0 || result.at<cv::Vec3b>(imgWidth - i - 1, j) == result.at<cv::Vec3b>(imgWidth - i - 1, j - 1)) && (j == imgHeight - 1 || result.at<cv::Vec3b>(imgWidth - i - 1, j) == result.at<cv::Vec3b>(imgWidth - i - 1, j + 1))))
					continue;
			Color c = Color(0, 0, 0);
			double a = 0.0;
			#pragma omp parallel
			#pragma omp for schedule(dynamic,1)
			for (int tx = -1; tx < 2; tx++) {
				for (int ty = -1; ty < 2; ty++) {
					Ray* tr = camera->getCameraRay(double(i) + double(tx) / 2.0f, double(j) + double(ty) / 2.0f);
					c += trace(tr, 0, 0, 1.0f, a, cur_obj);
					free(tr);
				}
			}
				//printf("\n>>>>>>>>>>>\n");
				c = c / (9.0f);
			result.at<cv::Vec3b>(imgWidth - i - 1, j)[0] = (int(c.getZ() * 255) > 255 ? 255 : int(c.getZ() * 255));
			result.at<cv::Vec3b>(imgWidth - i - 1, j)[1] = (int(c.getY() * 255) > 255 ? 255 : int(c.getY() * 255));
			result.at<cv::Vec3b>(imgWidth - i - 1, j)[2] = (int(c.getX() * 255) > 255 ? 255 : int(c.getX() * 255));


	}
	}
	showImg();
	writeImg();
}

double RayTracer::get_shadow(Object* obj, Vector3 intersectPos, int objId, Vector3& L) {
	//点光源产生阴影
	double shadow = 1.0;
	L = obj->getLightCenter() - intersectPos;
	if (obj->getType() == Object::SPHERE) {
		double L_len = L.getLength();
		L.normalize();
		Ray* inter2plight = new Ray(intersectPos + EPSILON*L, L);
		for (int j = 0; j < scene->getObjCnt(); j++) {
			if (j == objId) continue;
			bool in;
			double t = scene->getObj(j)->intersect(*inter2plight, in);
			//被遮挡
			if (t > 0.0f && t < L_len) {
				shadow = 0.0f;
			}
		}
		free(inter2plight);
	}
	else if (obj->getType() == Object::AREA) {
		L.normalize();
		shadow = 0.0f;
		Area* areaLight = (Area*)obj;
		for (int i = -areaLight->getSize(); i < areaLight->getSize() + 1; i+=scale*10) {
			for (int j = -areaLight->getSize(); j < areaLight->getSize() + 1; j+= scale * 10) {
				//printf("%d,%d\n", i, j);
				Vector3 lightPos = Vector3(areaLight->getLightCenter().getX() + i, areaLight->getLightCenter().getY() + j, areaLight->getLightCenter().getZ());
				Vector3 tmpDir = lightPos - intersectPos;
				double L_len = tmpDir.getLength();
				tmpDir.normalize();

				Ray* inter2plight = new Ray(intersectPos + EPSILON *tmpDir, tmpDir);
				bool sha = true;
				for (int m = 0; m < scene->getObjCnt(); m++) {
					if (m == objId) continue;
					bool in;
					double t = scene->getObj(m)->intersect(*inter2plight, in);
					//被遮挡
					if (t > 0.0f && t < L_len) {
						sha = false;
					}
				}
				if (sha) {
					shadow += 1.0f / 100.0f;
				}
				free(inter2plight);
			}
		}
	}
	return shadow;
}

Color RayTracer::trace(Ray* r,int depth,double length,double refract_idx,double &inter_l,int &inter_id) {
	if (depth > MAX_DEPTH) return Color(0, 0, 0);
	if(length > MAX_LIGHT_LEN) return Color(0, 0, 0);
	inter_l = 100000.0f;
	IntersectPoint* inter = scene->getIntersectObj(*r);
	if (inter == NULL) {
		inter_id = -1;
		return Color(0, 0, 0);
	}

	Object* intersectObj = inter->obj;
	double intersectT = inter->t;
	inter_l = intersectT;
	Vector3 intersectPos = r->o + r->dir*intersectT;
	double reflect = intersectObj->getReflect();
	double refract = intersectObj->getRefract();
	double diffuse = intersectObj->getDiffuse();
	double diff_reflect = intersectObj->getDiffReflect();
	double specular = intersectObj->getSpecular();
	
	bool isInsideObj = inter->inside;
	inter_id = inter->obj_id;
	Vector3 N = intersectObj->getNormal(intersectPos);	//交点法向量
	Color intersectColor = intersectObj->getColor(intersectPos);
	if (intersectObj->isLight()) {
		return intersectColor;
	}
	else {
		Color c = Color();
		for (int i = 0; i < scene->getObjCnt(); i++) {
			Object* obj = scene->getObj(i);
			if (obj->isLight()) {
				Vector3 L;	//交点到光源
				N.normalize();

				//点光源产生阴影
				double shadow =get_shadow(obj,intersectPos,i,L);
				if (shadow <= 0.0f) continue;

				//漫反射
				if (diffuse > 0) {
					double dot = Vector3::dot(N,L);
					if (dot > 0) {
						double diff = dot * diffuse*shadow;
						obj->getColor(obj->getLightCenter());
						c += Vector3::mul(obj->getColor(obj->getLightCenter()),intersectObj->getColor(intersectPos))*diff;
					}
				}

				//高光
				if (specular > 0.0f) {
					//光源在物体表面反射
					Vector3 rf_l = L - N * (Vector3::dot(L, N))*2.0f;
					double dot = Vector3::dot(rf_l, r->dir);
					if (dot > 0) {
						double t = powl(dot, 20)*specular*shadow;
						c += t * obj->getColor(obj->getLightCenter());
					}
				}

			}
		}
		if (reflect > 0.0f) {
			Vector3 rf_light = r->dir - N*(Vector3::dot(r->dir,N))*2.0f;		//光碰到物体后反射
			rf_light.normalize();
			//模糊反射
			if (diff_reflect > 0.0f && depth < 2) {
				//printf(">>>>>>%f",diff_reflect);
				Vector3 RN1 = Vector3(rf_light.getZ(), rf_light.getY(), -rf_light.getX());
				Vector3 RN2 = Vector3::cross(RN1, rf_light);
				reflect *= 1.0f / 32.0f;
				
				for (int i = 0; i < 32; i++) {
					double x_off = (double)rand() / RAND_MAX*2 - 1.0f;
					double y_off = (double)rand() / RAND_MAX*2 - 1.0f;
					while (x_off*x_off + y_off * y_off > diff_reflect*diff_reflect) {
						x_off = (double)rand() / RAND_MAX * 2 - 1.0f;
						y_off = (double)rand() / RAND_MAX * 2 - 1.0f;
					}
					Vector3 new_rf_light = rf_light + x_off * RN1 + y_off * RN2;
					new_rf_light.normalize();
					double a = 0.0f;
					int temp = 0;
					Ray* tl = new Ray(intersectPos + new_rf_light * EPSILON, new_rf_light);
					Color rf_c = trace(tl, depth + 1, length + inter->t, refract_idx, a, temp);
					free(tl);
					c += rf_c * reflect;
				}
			}
			//镜面反射
			else {
				double a = 0.0f;
				int temp = 0;
				Ray* tl = new Ray(intersectPos + rf_light * EPSILON, rf_light);
				Color t = trace(tl, depth + 1, length + inter->t, refract_idx, a, temp);
				free(tl);
				c += Vector3::mul(t, intersectColor)*reflect;	//反射光线出发点是物体外一点点
			}
		}

		//折射
		if (refract > 0.0f) {
			double n = refract_idx / refract;
			if (isInsideObj) N = N*(-1.0f);		//在物体内部
			double cosI = - Vector3::dot(N,r->dir);
			double cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
			if (cosT2 > 0.0f){
				Vector3 T = (n * r->dir) + (n * cosI - sqrtl(cosT2)) * N;
				double inter_len = 10000.0;
				int t = 0;
				Ray* tl = new Ray(intersectPos + T * EPSILON, T);
				Color c_t = trace(tl, depth + 1,length + inter->t, refract,inter_len,t);
				free(tl);
				//Beer's Law
				Color absorbance = intersectColor * 0.0005f * (-inter_len);
				Color transparency = Color(expl(absorbance.getX()), expl(absorbance.getY()), expl(absorbance.getZ()));
				c += c_t* transparency;
			}
		}

		free(inter);
		return c;
	}
	
	return Color(0, 0, 0);
}