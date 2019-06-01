#include "scene.h"

Scene::Scene() {
	objCnt = 0;
	lightCnt = 0;
}

Meterial* Scene::getMeterial(float reflect,float refract,
					float diffuse,float specular) {
	return new Meterial(reflect, refract, diffuse, specular);
}

ColorTexture* Scene::getColorTexture(Color c) {
	return new ColorTexture(c);
}
PicTexture* Scene::getPicTexture(std::string path) {
	return new PicTexture(path);
}

Sphere* Scene::getSphere(Meterial* m,Texture* t,Vector3 o,float r) {
	return new Sphere(m, t, o, r);
}

Plane* Scene::getPlane(Meterial* m, Texture* t, Vector3 P, Vector3 n,float D) {
	return new Plane(m, t, P, n, D);
}

void Scene::addObj(Object* obj) {
	objs[objCnt++] = obj;
}

int Scene::getObjCnt() {
	return objCnt;
}

Object* Scene::getObj(int idx) {
	if (idx >= objCnt) {
		return NULL;
	}
	else {
		return objs[idx];
	}
}


void Scene::addLight(Light* l) {
	light[lightCnt++] = l;
}

Light* Scene::getLight(int idx) {
	if (idx >= lightCnt) {
		return NULL;
	}
	return light[idx];
}

int Scene::getLightCnt() {
	return lightCnt;
}

IntersectPoint* Scene::getIntersectObj(Ray &r) {
	IntersectPoint* res = NULL;
	float t = 10000;
	for (int i = 0; i < objCnt; i++) {
		bool inside = false;
		float dis = objs[i]->intersect(r, inside);
		if ( dis > 0 && dis < t ) {
			t = dis;
			if (res) {
				res->set(objs[i], t, inside,i);
			}
			else {
				res = new IntersectPoint(objs[i], t,inside,i);
			}
		}
	}
	return res;
}