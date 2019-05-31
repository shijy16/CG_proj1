#ifndef SCENE_H
#define SCENE_H
#include "objects.h"

struct IntersectPoint {
	Object* obj;
	float t;
	bool inside;
	IntersectPoint(){}
	IntersectPoint(Object* o,float _t,bool in):obj(o),t(_t),inside(in){}
	void set(Object* o, float _t) { obj = o; t = _t; }
};

class Scene {
private:
	Object* objs[20];
	int objCnt;
	int lightCnt;
	Light* light[20];
public:
	Scene();

	Meterial* getMeterial(float reflect, float refract,float diffuse, float specular);
	ColorTexture* getColorTexture(Color c);
	PicTexture* getPicTexture(std::string path);
	Sphere* getSphere(Meterial* m, Texture* t, Vector3 o, float r);
	Plane* getPlane(Meterial* m, Texture* t, Vector3 P, Vector3 n, float D);

	void addObj(Object* obj);
	int getObjCnt();
	Object* getObj(int index);

	void addLight(Light* l);
	Light* getLight(int idx);
	int getLightCnt();

	IntersectPoint* getIntersectObj(Ray &r);
};


#endif