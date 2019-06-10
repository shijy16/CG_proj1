#include "common.h"
#include "vector3.h"
#include "Eigen/Dense"
using namespace Eigen;

class Bezier {
	Vector3 points[10];
	double x[10];
	double y[10];
	int point_cnt;
public:
	Vector3 pos;
	double maxX;
	double minX;
	double maxY;
	double minY;
	Vector3 lastNorm;
	Bezier(int num, double* xx, double* yy, Vector3 p);
	double inline getX(double t);
	double inline getY(double t);
	double inline getDX(double t);
	double inline getDY(double t);
	double inline getCoefficient(int n, int i, double u);
	double inline getDCoefficient(int n, int i, double u);
	double intersect(Ray r);
	Vector3 initNewton(Ray r,int);
	Vector3 getF(Ray r, double t, double u, double theta);
	Matrix3d getJF(Ray r, double t, double u, double theta);
	Vector3 getNormal(double u, double angle);
	Vector3 curve3d(double u, double angle);
	void write(std::string file_name);

	static inline int fac(int b);
	static inline int C(int m,int n);
	static void inverse(double**,double**);
	static void matrixMul(double**,double**,double**);
	static Vector3 mxv(double** m, Vector3 v);
};