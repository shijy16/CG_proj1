#include "Bezier.h"


Bezier::Bezier(int num, double* xx, double* yy, Vector3 p) {
	pos = p;
	point_cnt = 0;
	maxX = -100000;
	maxY = -100000;
	minX = 100000;
	minY = 100000;
	for (int i = 0; i < num; i++) {
		x[i] = xx[i];
		y[i] = yy[i];
		point_cnt++;
		minX = (minX < x[i]) ? minX : x[i];
		minY = (minY < y[i]) ? minY : y[i];
		maxX = (maxX > x[i]) ? maxX : x[i];
		maxY = (maxY > y[i]) ? maxY : y[i];
	}
	printf("%f,%f\n", minX, maxX);
}

//阶乘
int inline Bezier::fac(int n) {
	int ans = 1;
	for (int i = 1; i <= n; i++) {
		ans *= i;
	}
	return ans;
}

////组合数
//int inline Bezier::C(int m, int n) {
//	return fac(n) / fac(m) / fac(n - m);
//}
//
////系数
//double inline Bezier::getCoefficient(int n, int i, double u) {
//	return C(i, n)*powl(u, i)*powl(1.0f - u, n - i);
//}
//
////系数导数
//double inline Bezier::getDCoefficient(int n, int i, double u) {
//	return C(i, n)*(-(n - i)*powl(1.0f - u, n - i - 1)*powl(u, i) + i * powl(u, i - 1)*powl(1.0f - u, n - i));
//}

double inline Bezier::getX(double t) {
	//double tx = 0;
	//for (int i = 0; i < point_cnt; i++) {
	//	tx += x[i] * getCoefficient(point_cnt - 1, i, t);
	//}
	//return tx;
	return 1 * x[0] * (1 - t)*(1 - t)*(1 - t) +
		3 * x[1] * t*(1 - t)*(1 - t) +
		3 * x[2] * t*t*(1 - t) +
		1 * x[3] * t*t*t;
}

double inline Bezier::getY(double t) {
	/*double tx = 0;
	for (int i = 0; i < point_cnt; i++) {
		tx += y[i] * getCoefficient(point_cnt - 1, i, t);
	}
	return tx;*/
	return 1 * y[0] * (1 - t)*(1 - t)*(1 - t) +
		3 * y[1] * t*(1 - t)*(1 - t) +
		3 * y[2] * t*t*(1 - t) +
		1 * y[3] * t*t*t;
}

double inline Bezier::getDX(double t) {
	//double tx = 0;
	//for (int i = 0; i < point_cnt; i++) {
	//	tx += x[i] * getDCoefficient(point_cnt - 1, i, t);
	//}
	//return tx;
	return -3 * x[0] * (1 - t)*(1 - t) +
		3 * x[1] * (1 - t)*(1 - t) +
		-6 * x[1] * t*(1 - t) +
		6 * x[2] * (1 - t)*t +
		-3 * x[2] * t*t +
		3 * x[3] * t*t;
}

double inline Bezier::getDY(double t) {
	//double tx = 0;
	//for (int i = 0; i < point_cnt; i++) {
	//	tx += y[i] * getDCoefficient(point_cnt - 1, i, t);
	//}
	//return tx;
	return -3 * y[0] * (1 - t)*(1 - t) +
		3 * y[1] * (1 - t)*(1 - t) +
		-6 * y[1] * t*(1 - t) +
		6 * y[2] * (1 - t)*t +
		-3 * y[2] * t*t +
		3 * y[3] * t*t;
}

//x:t,y:u,z:theta 随机取一点
Vector3 Bezier::initNewton(Ray r,int j) {
	double u = abs(double(rand()) / double(RAND_MAX));
	if (j == 0) {
		u = 0;
	} else if (j == 1) {
		u = 1;
	}
	double theta = abs(double(rand()) / double(RAND_MAX) *2.0*PI);
	Vector3 p = curve3d(u, theta);
	double tx = p.getX();
	double t = (tx - r.o.getX()) / r.dir.getX();
	return Vector3(t, u, theta);
}


//牛顿迭代中F
Vector3 Bezier::getF(Ray r, double t, double u, double theta) {
	Vector3 F;
	r.dir.normalize();
	F = r.o + r.dir*t - curve3d(u, theta);
	return F;
}

//F的jaccobi行列式
Matrix3d Bezier::getJF(Ray r, double t, double u, double theta) {
	Matrix3d res;
	r.dir.normalize();
	res << r.dir.getX(), -sinl(theta)*getDX(u), -cosl(theta)*getX(u),
		r.dir.getY(), -cosl(theta)*getDX(u), sinl(theta)*getX(u),
		r.dir.getZ(), -getDY(u), 0;

	return res;
}
//
////求逆矩阵
//void Bezier::inverse(double** mat,double **res) {
//	double determinant = 0;
//	for (int i = 0; i < 3; i++)
//		determinant = determinant + (mat[0][i] * (mat[1][(i + 1) % 3] * mat[2][(i + 2) % 3] - mat[1][(i + 2) % 3] * mat[2][(i + 1) % 3]));
//	
//	
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 3; j++) {
//			if (determinant == 0.0f) {
//				res[i][j] = 0;
//			}
//			else {
//				res[i][j] = ((mat[(j + 1) % 3][(i + 1) % 3] * mat[(j + 2) % 3][(i + 2) % 3]) - (mat[(j + 1) % 3][(i + 2) % 3] * mat[(j + 2) % 3][(i + 1) % 3])) / determinant;
//			}
//		}
//	}
//}
//
////矩阵乘法
//void Bezier::matrixMul(double** m1 , double** m2,double** res) {
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 3; j++) {
//			res[i][j] = 0;
//			for (int n = 0; n < 3; n++) {
//				res[i][j] += m1[i][n] * m2[n][j];
//				
//			}
//			printf("%f\t",res[i][j]);
//		}
//		printf("\n");
//	}
//}


//3D坐标垂线
Vector3 Bezier::getNormal(double u,double v) {
	double dx = getDX(u), dy = getDY(u);
	double xx = getX(u);
	
	Vector3 t1(dx*sinl(v),dx*cosl(v),dy);
	Vector3 t2(cosl(v)*xx, -sinl(v)*xx, 0);
	t1 = Vector3::cross(t1, t2);
	t1.normalize();
	return t1;
}

//获取3D坐标点
Vector3 Bezier::curve3d(double u, double v) {
	double tx = getX(u);
	double ty = getY(u);
	//return Vector3(pos.getX() + cosl(v)*tx, pos.getY()+ty, pos.getZ()+tx*sinl(v));
	return Vector3(pos.getX() + sinl(v)*tx, pos.getY()+ tx * cosl(v), pos.getZ()+ ty);
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
			Vector3 p = Vector3(pos.getX() + sinl(angle)*tx, pos.getY() + ty, pos.getZ() + tx * cosl(angle));
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

Vector3 Bezier::mxv(double** m, Vector3 v) {
	Vector3 res;
	res = Vector3(0, 0, 0);
	res = res + Vector3(m[0][0] * v.getX() + m[0][1] * v.getY() + m[0][2] * v.getZ(), 0, 0);
	res = res + Vector3(0, m[1][0] * v.getX() + m[1][1] * v.getY() + m[1][2] * v.getZ(), 0);
	res = res + Vector3(0, 0, m[2][0] * v.getX() + m[2][1] * v.getY() + m[2][2] * v.getZ());
	return res;
}

double Bezier::intersect(Ray r) {
	r.dir.normalize();
	bool has_ans = false;
	/*if (r.dir.getX() == 0.0) {
		r.dir = r.dir + Vector3(1e-10, 0, 0);
	}
	if (r.dir.getY() == 0.0) {
		r.dir = r.dir + Vector3(0, 1e-10, 0);
	}
	if (r.dir.getZ() == 0.0) {
		r.dir = r.dir + Vector3(0, 0, 1e-10);
	}*/
	Vector3d arg(0,0,0);
	Vector3d ans(1e80,0,0);
	Vector3d xk;
	#pragma omp parallel
	#pragma omp for schedule(dynamic,2)
	for (int j = 0; j < 50; j++) {
		arg = initNewton(r,j).toEigen();
		if (arg.x() < 0.0f) continue;
		bool solved = false;
		for (int i = 0; i < 50; i++) {
			double t = arg.x();
			double u = arg.y();
			double theta = arg.z();
			Matrix3d JF;
			Vector3d F;
			F = getF(r, t, u, theta).toEigen();
			if (u < -0.5 || u > 1.5) break;
			JF = getJF(r, t, u, theta);
	
			if (std::max(std::max(std::abs(F.x()), std::abs(F.y())), std::abs(F.z())) < 1e-7) {
				solved = true;
				break;
			}

			arg = arg - JF.inverse()*F*0.7f;

		}
		if (solved) {
			if (arg.x() < 0) continue;
			if (arg.y() < 0 || arg.y() > 1) continue;
			//printf("solved: %f\n", arg.getX());
			#pragma omp critical
			{
				if (arg.x() <= ans.x()) {
					has_ans = true;
					ans = arg;
				}
			}
		}
	}
	if (!has_ans) {
		return -1;
	}
	double t = ans.x();
	
	//保证法向量指向光线射来的方向
	lastNorm = getNormal(ans.y(), ans.z());
	/*if (Vector3::dot(r.dir, lastNorm) > 0) {
		lastNorm = Vector3(0,0,0) - lastNorm;
	}*/
	return t;
}