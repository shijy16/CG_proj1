#ifndef OBJECTS_H
#define OBJECTS_H
#include "common.h"
#include "Bezier.h"

struct Meterial {
	double reflect;		//·´Éä
	double refract;		//ÕÛÉä
	double diffuse;		//Âþ·´Éä
	double specular;		//¾µÃæ
	double diff_reflect;
	Meterial() {
		reflect = 0;
		refract = 0;
		diffuse = 0;
		specular = 0;
		diff_reflect = -1;
	}
	Meterial(double rfl,double rfr,double dfu,double spcl):reflect(rfl),refract(rfr),diffuse(dfu),specular(spcl){ diff_reflect = -1;}
};

/*		   Texture
	PicTexture  ColorTexture
*/
class Texture {
public:
	enum Type {
		PICTURE,
		PURE
	};
protected:
	Type type;

public:
	Texture();
	Type getType();
	virtual Color getColor(double x, double y, bool) = 0;
};

class PicTexture :public Texture {
private:
	std::string picPath;
	cv::Mat pic;
public:
	PicTexture(std::string);
	~PicTexture() {}
	void loadPic();
	Color getColor(double x, double y,bool);
};

class ColorTexture :public Texture {
private:
	Color textureColor;
public:
	ColorTexture(Color c) :textureColor(c) { type = PURE; };
	Color getColor(double x, double y, bool);
};

/*******************************************
					Object
			Sphere Plane Bezier
********************************************/
class Object {
protected:
	Meterial* objMeterial;
	Texture* objTexture;
	//std::string name;
	bool light = false;
	int type;
	Light* l;
public:
	const static int PLANE = 1;
	const static int SPHERE = 2;
	const static int BEZEIER = 3;
	const static int AREA = 4;
	Object(){}
	int getType() { return type; }
	//std::string getName() { return name; }
	double getReflect() { return objMeterial->reflect; }
	double getRefract() { return objMeterial->refract; }
	double getDiffuse() { return objMeterial->diffuse; }
	double getSpecular() { return objMeterial->specular; }
	double getDiffReflect() { return objMeterial->diff_reflect; }
	bool isLight() { return light; }
	void setAsLight(Light* _l) { light = true; l = _l; }
	void setAsLight() { light = true; l = new Light(); }
	void setAsLight(double brightness) { light = true; l = new Light(); l->brightness = brightness; }
	virtual Color getColor(Vector3 pos) = 0;
	virtual double intersect(Ray &r,bool &inside) = 0;
	virtual Vector3 getLightCenter() { return Vector3(); }
	virtual Vector3 getNormal(Vector3 pos) = 0;
};

class Plane :public Object {
private:
	Vector3 n;
	Vector3 P;
	Vector3 dx;
	Vector3 dy;
	double D;
public:
	Plane(Meterial* m, Texture* t, Vector3 _P, Vector3 _n, double _D);
	Color getColor(Vector3 pos);
	double intersect(Ray &r, bool &inside);
	Vector3 getLightCenter();
	Vector3 getNormal(Vector3 pos);
};

class Sphere :public Object {
private:
	Vector3 P,vn,ve,vc;
	double r;

public:
	Sphere(Meterial* m, Texture* t, Vector3 _P, double _r);
	Color getColor(Vector3 pos);
	double intersect(Ray &r, bool &inside);
	Vector3 getLightCenter();
	Vector3 getNormal(Vector3 pos);
};
class Area :public Object {
private:
	Vector3 O, Dx, Dy;
	int size;
public:
	Area(Meterial* m, Texture* t, Vector3 o, Vector3 dx, Vector3 dy,int s) :O(o), Dx(dx), Dy(dy),size(s) { type = AREA; objMeterial = m;objTexture = t;}
	Vector3 getNormal(Vector3 pos) { return Vector3(0, 0, -1); }
	Color getColor(Vector3 pos) { return l->color; };
	double intersect(Ray &r, bool &inside);
	Vector3 getLightCenter() { return O; };
	double getSize() { return size; }
};

class BezierObject :public Object {
private:
	Bezier* myBezier;
	AABB myAABB;
public:
	BezierObject(Meterial* m, Texture* t, Bezier* bezier);
	Color getColor(Vector3 pos);
	double intersect(Ray &r, bool &inside);
	Vector3 getLightCenter();
	Vector3 getNormal(Vector3 pos);
};
#endif