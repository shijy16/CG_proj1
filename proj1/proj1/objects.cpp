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

Color PicTexture::getColor(double x,double y, bool sphere) {
	if (sphere) {
		x *= pic.rows;
		y *= pic.cols;
	}

	int xx = int(x)%pic.rows;
	int yy = int(y)%pic.cols;
	while (xx < 0) xx += pic.rows;
	while (yy < 0) yy += pic.cols;
	double b = pic.at<cv::Vec3b>(xx, yy)[0];
	double g = pic.at<cv::Vec3b>(xx, yy)[1];
	double r = pic.at<cv::Vec3b>(xx, yy)[2];
	return Color(r,g,b);
}

/****************************************************/
/***************    ColorTexture   ******************/
/*****************************************************/

Color ColorTexture::getColor(double x, double y, bool sphere) {
	return textureColor; 
}


/****************************************************/
/******************    Plane   **********************/
/*****************************************************/
Plane::Plane(Meterial* m, Texture* t,Vector3 _P, Vector3 _n, double _D) :P(_P),n(_n), D(_D) {
	objMeterial = m;
	objTexture = t;
	if (n.getX() == 0) {
		dx = Vector3(1, 0, 0);
		if (n.getY() == 0) {
			dy = Vector3(0, 1, 0);
		}else {
			dy = Vector3(0, 0, 1);
		}
	}else {
		dx = Vector3(0, 0, 1);
		dy = Vector3(0, 1, 0);
	}
}

double Plane::intersect(Ray &r) {
	double deno = r.dir*n;
	if (deno == 0) return -1;
	double res = -(D + n*r.o);
	res /= deno;
	if (res > 0) return res;
	else return -1;
}

Color Plane::getColor(Vector3 &pos) {
	if (objTexture->getType() == Texture::PURE) {
		return objTexture->getColor(pos.getX(), pos.getY(),false);
	} else {
		double tx = ((pos - P) * dx) / dx.getLength();
		double ty = ((pos - P) * dy) / dy.getLength();
		return objTexture->getColor(tx, ty,false);
	}
}

/****************************************************/
/******************    Sphere  **********************/
/****************************************************/
Sphere::Sphere(Meterial* m, Texture* t, Vector3 _P, double _r) :P(_P), r(_r) {
	objMeterial = m;
	objTexture = t;
}

Color Sphere::getColor(Vector3 &pos) {
	if (objTexture->getType() == Texture::PURE) {
		return objTexture->getColor(0, 0,true);
	} else  {
		Vector3 tmp = (pos - P) *(1 / r);
		double x = asin(tmp.getZ()) + PI / double(2);
		double y = atan2(tmp.getX(), tmp.getY());
		return objTexture->getColor(x, y,true);
	}
}

double Sphere::intersect(Ray &ray) {
	Vector3 l = P - ray.o;
	double tp = l*ray.dir;
	double ll = l*l;
	double rr = r*r;
	if (ll > rr) {
		if (tp < 0) {
			return -1;
		}
	}
	double dd = ll - tp*tp;
	if (dd >= rr) return -1;
	double t_ = sqrt(rr - dd);
	if (ll >= rr) {
		return tp - t_;
	}
	else {
		return tp + t_;
	}
	return -1;
}