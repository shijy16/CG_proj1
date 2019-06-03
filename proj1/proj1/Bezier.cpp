#include "Bezier.h"

//阶乘
int Bezier::fac(int n) {
	int ans = 1;
	for (int i = 1; i <= n; i++) {
		ans *= i;
	}
	return ans;
}

//组合数
int Bezier::C(int m, int n) {
	return fac(n) / fac(m) / fac(n - m);
}

//系数
double Bezier::getCoefficient(int n, int i, double u) {
	return C(i, n)*powl(u, i)*powl(1.0f - u, n - i);
}

//系数倒数
double Bezier::getDCoefficient(int n, int i, double u) {
	return C(i, n)*(-(n - i)*powl(1.0f - u, n - i - 1)*powl(u, i) + i * powl(u, i - 1)*powl(1.0f - u, n - i));
}

//3D中的垂线
Vector3 Bezier::getNormal(Vector3 pos) {
	Vector3 res, temp;
	for (int i = 0; i < point_cnt; i++) {
		double db_du = getDCoefficient(i, point_cnt - 1, pos.getY());
		temp = temp + Vector3(x[i] * db_du, 0, y[i] * db_du);
	}
	res = Vector3(temp.getZ() * cos(pos.getZ()), -temp.getX(), temp.getZ() * sinl(pos.getZ()));
	return res;
}

Vector3 Bezier::curve3d(double u, double angle) {
	double tx = 0;
	double ty = 0;
	for (int i = 0; i < point_cnt; i++) {
		tx += x[i]* getCoefficient(point_cnt - 1,i,u);
		ty += y[i]* getCoefficient(point_cnt - 1, i, u);
	}
	return Vector3(pos.getX() + cosl(angle)*tx, pos.getY()+ty, pos.getZ()+tx*sinl(angle));
}

void Bezier::write(std::string fileName) {
	std::ofstream outf;
	outf.open(fileName);

	bool ini = false;
	double min_x = -1.0, min_y = -1.0, min_z = -1.0, max_x = -1.0, max_y = -1.0, max_z = -1.0;
	int u = 120, v = 120;
	Vector3** interation_helper = new Vector3*[u + 1];
	Vector3** normal_helper = new Vector3*[u + 1];
	for (int i = 0; i <= u; i++) {
		interation_helper[i] = new Vector3[v + 1];
		normal_helper[i] = new Vector3[v + 1];
		for (int j = 0; j <= v; j++) {
			double _u = i;
			double _v = j;
			_v *= 2 * PI;
			_u /= u;
			_v /= v;
			Vector3 hh = curve3d(_u, _v);
			if (!ini) {
				ini = true;
				min_x = max_x = hh.getX();
				min_y = max_y = hh.getY();
				min_z = max_z = hh.getZ();
			}
			else {
				min_x = std::min(min_x, hh.getX()); max_x = std::max(max_x, hh.getX());
				min_y = std::min(min_y, hh.getY()); max_y = std::max(max_y, hh.getY());
				min_z = std::min(min_z, hh.getZ()); max_z = std::max(max_z, hh.getZ());
			}
			interation_helper[i][j] = hh;
			normal_helper[i][j] = getNormal(Vector3(0.0, _u, _v));
			outf << "v" << " ";
			outf << hh.getX() << " " << hh.getY() << " " << hh.getZ() << std::endl;
		}
	}
	for (int j = 0; j < v; j++) {
		for (int i = 0; i < u; i++) {
			outf << "f ";
			outf << (i + 1 + j * (u + 1)) << " " << (i + 2 + j * (u + 1)) << " " << (i + 1 + (j + 1)*(u + 1)) << std::endl;
			outf << "f ";
			outf << (i + 2 + j * (u + 1)) << " " << (i + 1 + (j + 1)*(u + 1)) << " " << (i + 2 + (j + 1)*(u + 1)) << std::endl;
		}
	}
	outf.close();
}