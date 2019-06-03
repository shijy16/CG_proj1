#include "Bezier.h"

//阶乘
int inline Bezier::fac(int n) {
	int ans = 1;
	for (int i = 1; i <= n; i++) {
		ans *= i;
	}
	return ans;
}

//组合数
int inline Bezier::C(int m, int n) {
	return fac(n) / fac(m) / fac(n - m);
}

//系数
double inline Bezier::getCoefficient(int n, int i, double u) {
	return C(i, n)*powl(u, i)*powl(1.0f - u, n - i);
}

//系数导数
double inline Bezier::getDCoefficient(int n, int i, double u) {
	return C(i, n)*(-(n - i)*powl(1.0f - u, n - i - 1)*powl(u, i) + i * powl(u, i - 1)*powl(1.0f - u, n - i));
}

double inline Bezier::getX(double t) {
	double tx = 0;
	for (int i = 0; i < point_cnt; i++) {
		tx += x[i] * getCoefficient(point_cnt - 1, i, t);
	}
	return tx;
}

double inline Bezier::getY(double t) {
	double tx = 0;
	for (int i = 0; i < point_cnt; i++) {
		tx += y[i] * getCoefficient(point_cnt - 1, i, t);
	}
	return tx;
}

//3D坐标垂线
Vector3 Bezier::getNormal(double u,double v) {
	double dx, dy;
	for (int i = 0; i < point_cnt; i++) {
		double dCoeffient = getDCoefficient(point_cnt - 1, i, u);
		dx += x[i] * dCoeffient;
		dy += y[i] * dCoeffient;
	}
	return Vector3(dy * cos(v), -dx, dy * sinl(v));
}

//获取3D坐标点
Vector3 Bezier::curve3d(double u, double v) {
	double tx = getX(u);
	double ty = getY(u);
	//return Vector3(pos.getX() + cosl(v)*tx, pos.getY()+ty, pos.getZ()+tx*sinl(v));
	return Vector3(pos.getX() + cosl(v)*tx, pos.getY()+ tx * sinl(v), pos.getZ()+ ty);
}

//写出
void Bezier::write(std::string fileName) {
	std::ofstream outf;
	outf.open(fileName);
	int u = 100, v = 100;
	double h = 1.0f / 100.0f;
	for (int i = 0; i <= u; i++) {
		for (int j = 0; j <= v; j++) {
			double tx = getX(double(i)*h);
			double ty = getY(double(i)*h);
			double angle = double(j)*h*2.0f*PI;
			Vector3 p = Vector3(pos.getX() + cosl(angle)*tx, pos.getY() + ty, pos.getZ() + tx * sinl(angle));
			outf << "v "<< p.getX() << " " << p.getY() << " " << p.getZ() << "\n";
		}
	}
	for (int j = 0; j < v; j++) {
		for (int i = 0; i < u; i++) {
			outf << "f " << (i + 1 + j * (u + 1)) << " " << (i + 2 + j * (u + 1)) << " " << (i + 1 + (j + 1)*(u + 1)) << "\n";
			outf << "f " << (i + 2 + j * (u + 1)) << " " << (i + 1 + (j + 1)*(u + 1)) << " " << (i + 2 + (j + 1)*(u + 1)) << "\n";
		}
	}
	outf.close();
}
