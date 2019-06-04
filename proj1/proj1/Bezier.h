#include "common.h"
#include "vector3.h"

class Bezier {
	Vector3 points[10];
	Vector3 pos;
	double x[10];
	double y[10];
	int point_cnt;
	double maxX;
	double minX;
	double maxY;
	double minY;
public:
	Vector3 lastNorm;
	Bezier(int num, double* xx, double* yy, Vector3 p);
	double inline getX(double t);
	double inline getY(double t);
	double inline getDX(double t);
	double inline getDY(double t);
	double inline getCoefficient(int n, int i, double u);
	double inline getDCoefficient(int n, int i, double u);
	std::pair<double, bool> intersect(Ray r);
	Vector3 initNewton(Ray r);
	Vector3 getF(Ray r, double t, double u, double theta);
	double** getJF(Ray r, double t, double u, double theta);
	Vector3 getNormal(double u, double angle);
	Vector3 curve3d(double u, double angle);
	void write(std::string file_name);

	static inline int fac(int b);
	static inline int C(int m,int n);
	static double** inverse(double**);
	static double** matrixMul(double**,double**);
	static Vector3 mxv(double** m, Vector3 v);
};