#include "objects.h"

Texture::Texture() {}

Texture::Type Texture::getType() {
	return type;
}
/****************************************************/
/******************    PicTexture   ******************/
/*****************************************************/
PicTexture::PicTexture(std::string p){
	type = PICTURE;
	picPath = p;
	loadPic();
	cv::Size size = cv::Size(pic.cols/5, pic.rows/5);
	cv::resize(pic, pic, size, CV_INTER_CUBIC);
}

void PicTexture::loadPic() {
	pic = cv::imread(picPath);
	cv::waitKey(0);
}



Color PicTexture::getColor(float x, float y, bool isSphere) {
	if (isSphere) {
		x *= pic.rows;
		y *= pic.cols;
	}
	else {
		x /= 10;
		y /= 10;
	}
	int u1 = ((int)x) % pic.rows;
	int v1 = ((int)y) % pic.cols;
	while (u1 < 0) u1 += pic.rows;
	while (v1 < 0) v1 += pic.cols;
	int u2 = (u1 + 1) %  pic.rows;
	int v2 = (v1 + 1) % pic.cols;

	float fracu = x - floorf(x);
	float fracv = y - floorf(y);

	float w1 = (1 - fracu) * (1 - fracv);
	float w2 = fracu * (1 - fracv);
	float w3 = (1 - fracu) * fracv;
	float w4 = fracu * fracv;

	Color c1 = Color(pic.at<cv::Vec3b>(u1, v1)[2], pic.at<cv::Vec3b>(u1, v1)[1], pic.at<cv::Vec3b>(u1, v1)[0]) / 255.0f;
	Color c2 = Color(pic.at<cv::Vec3b>(u2, v1)[2], pic.at<cv::Vec3b>(u2, v1)[1], pic.at<cv::Vec3b>(u2, v1)[0]) / 255.0f;
	Color c3 = Color(pic.at<cv::Vec3b>(u1, v2)[2], pic.at<cv::Vec3b>(u1, v2)[1], pic.at<cv::Vec3b>(u1, v2)[0]) / 255.0f;
	Color c4 = Color(pic.at<cv::Vec3b>(u2, v2)[2], pic.at<cv::Vec3b>(u2, v2)[1], pic.at<cv::Vec3b>(u2, v2)[0]) / 255.0f;

	Color c = c1 * w1 + c2 * w2 + c3 * w3 + c4 * w4;
	return c;
}

/****************************************************/
/***************    ColorTexture   ******************/
/*****************************************************/

Color ColorTexture::getColor(float x, float y,bool isSphere) {
	return textureColor; 
}


/****************************************************/
/******************    Plane   **********************/
/*****************************************************/
Plane::Plane(Meterial* m, Texture* t,Vector3 _P, Vector3 _n, float _D) :P(_P),n(_n), D(_D) {
	type = PLANE;
	objMeterial = m;
	objTexture = t;
	if (n.getX() == 0.0f) {
		dx = Vector3(1, 0, 0);
		if (n.getY() == 0.0f) {
			dy = Vector3(0, 1, 0);
		}else {
			dy = Vector3(0, 0, 1);
		}
	}else {
		dx = Vector3(0, 0, 1);
		dy = Vector3(0, 1, 0);
	}
}

float Plane::intersect(Ray &r, bool &inside) {
	inside = false;
	float deno = Vector3::dot(r.dir,n);
	if (deno == 0.0f) return -1;
	float res = -(D + Vector3::dot(n,r.o));
	res /= deno;
	if (res > 0.0f) return res;
	else return -1;
}

Color Plane::getColor(Vector3 &pos) {
	if (objTexture->getType() == Texture::PURE) {
		return objTexture->getColor(pos.getX(), pos.getY(),false);
	} else {
		float tx = Vector3::dot(pos - P, dx) / dx.getLength();
		float ty = Vector3::dot(pos - P, dy) / dy.getLength();
		return objTexture->getColor(tx, ty,false);
	}
}

Vector3 Plane::getLightCenter() {
	return P;
}

Vector3 Plane::getNormal(Vector3 pos) {
	return n;
}

/****************************************************/
/******************    Sphere  **********************/
/****************************************************/
Sphere::Sphere(Meterial* m, Texture* t, Vector3 _P, float _r) :P(_P), r(_r) {
	type = SPHERE;
	objMeterial = m;
	objTexture = t;
	vn = Vector3(0, 1, 0);
	ve = Vector3(1, 0, 0);
	vc = Vector3::cross(vn,ve);
}

Color Sphere::getColor(Vector3 &pos) {
	if (objTexture->getType() == Texture::PURE) {
		return objTexture->getColor(0, 0,true);
	} else  {
		Vector3 I = pos - P;
		I.normalize();
		float a = acos(-Vector3::dot(I, ve));
		float b = acos(std::min(std::max(Vector3::dot(I, vc) / sin(a), -1.0f), 1.0f));
		float u = a / PI;
		float v = b / 2.0f / PI;
		if (Vector3::dot(vc, ve) < 0) v = 1 - v;
		return objTexture->getColor(u, v,true);
	}
}

float Sphere::intersect(Ray &ray, bool &inside) {
	inside = false;
	Vector3 l = P - ray.o;
	float tp = Vector3::dot(l,ray.dir);
	float ll = Vector3::dot(l, l);
	float rr = r*r;
	if (ll > rr) {
		if (tp < 0) {
			return -1;
		}
	}
	float dd = ll - tp*tp;
	if (dd >= rr) return -1;
	float t_ = sqrt(rr - dd);
	if (ll >= rr) {
		return tp - t_;
	}
	else {
		inside = true;
		return tp + t_;
	}
	return -1;
}

Vector3 Sphere::getLightCenter() {
	return P;
}

Vector3 Sphere::getNormal(Vector3 pos) {
	return (pos - P)*r;
}


float Area::intersect(Ray &r, bool &inside) {
	inside = false;
	r.dir.normalize();
	Vector3 N = Vector3::cross(Dx, Dy);
	N.normalize();
	float d = Vector3::dot(N, r.dir);
	if (fabs(d) < 0.001) return -1;
	float l = Vector3::dot(N * Vector3::dot(O, N) - r.o, N) / d;
	if (l < 0.001) return -1;

	Vector3 C = r.o + r.dir*l - O;
	if (fabs(Vector3::dot(C, Dx)) > Vector3::dot(Dx, Dx)) return -1.0;
	if (fabs(Vector3::dot(C, Dy)) > Vector3::dot(Dy, Dy)) return -1.0;
	return l;
}