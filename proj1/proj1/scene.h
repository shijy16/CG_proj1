#ifndef SCENE_H
#define SCENE_H
#include "objects.h"

struct IntersectPoint {
	Object* obj;
	float t;
	int obj_id;
	bool inside;
	IntersectPoint(){}
	IntersectPoint(Object* o,float _t,bool in,int id):obj(o),t(_t),inside(in),obj_id(id){}
	void set(Object* o, float _t, bool in, int id) { obj = o; t = _t; inside = in; id = obj_id; }
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