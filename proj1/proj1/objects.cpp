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

Color PicTexture::getColor(float x,float y, bool sphere) {
	if (sphere) {
		x *= pic.rows;
		y *= pic.cols;
	}

	int xx = int(x)%pic.rows;
	int yy = int(y)%pic.cols;
	while (xx < 0) xx += pic.rows;
	while (yy < 0) yy += pic.cols;
	float b = pic.at<cv::Vec3b>(xx, yy)[0]/(float)255;
	float g = pic.at<cv::Vec3b>(xx, yy)[1]/ (float)255;
	float r = pic.at<cv::Vec3b>(xx, yy)[2]/ (float)255;
	return Color(r,g,b);
}

/****************************************************/
/***************    ColorTexture   ******************/
/*****************************************************/

Color ColorTexture::getColor(float x, float y, bool sphere) {
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

float Plane::intersect(Ray &r) {
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
}

Color Sphere::getColor(Vector3 &pos) {
	if (objTexture->getType() == Texture::PURE) {
		return objTexture->getColor(0, 0,true);
	} else  {
		Vector3 tmp = (pos - P) *(1 / r);
		float x = asin(tmp.getZ()) + PI / float(2);
		float y = atan2(tmp.getX(), tmp.getY());
		return objTexture->getColor(x, y,true);
	}
}

float Sphere::intersect(Ray &ray) {
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