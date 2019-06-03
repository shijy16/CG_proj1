#ifndef SCENE_H
#define SCENE_H
#include "objects.h"

struct IntersectPoint {
	Object* obj;
	double t;
	int obj_id;
	bool inside;
	IntersectPoint(){}
	IntersectPoint(Object* o,double _t,bool in,int id):obj(o),t(_t),inside(in),obj_id(id){}
	void set(Object* o, double _t, bool in, int id) { obj = o; t = _t; inside = in; id = obj_id; }
};

class Scene {
private:
	Object* objs[20];
	int objCnt;
	int lightCnt;
	Light* light[20];
public:
	Scene();

	Meterial* getMeterial(double reflect, double refract,double diffuse, double specular);
	ColorTexture* getColorTexture(Color c);
	PicTexture* getPicTexture(std::string path);
	Sphere* getSphere(Meterial* m, Texture* t, Vector3 o, double r);
	Plane* getPlane(Meterial* m, Texture* t, Vector3 P, Vector3 n, double D);

	void addObj(Object* obj);
	int getObjCnt();
	Object* getObj(int index);

	void addLight(Light* l);
	Light* getLight(int idx);
	int getLightCnt();

	IntersectPoint* getIntersectObj(Ray &r);
};


#endif