#include "common.h"
#include "vector3.h"

class Bezier {
	Vector3 points[10];
	Vector3 pos;
	double x[10];
	double y[10];
	int point_cnt;
public:
	Bezier(int num, double* xx, double* yy,Vector3 p) {
		pos = p;
		point_cnt = 0;
		for (int i = 0; i < num; i++) {
			x[i] = xx[i];
			y[i] = yy[i];
			point_cnt++;
		}
	}
	double getCoefficient(int n, int i, double u);
	double getDCoefficient(int n, int i, double u);
	Vector3 getNormal(Vector3 pos);
	Vector3 curve3d(double u, double angle);
	void write(std::string file_name);

	static int fac(int b);
	static int C(int m,int n);
};