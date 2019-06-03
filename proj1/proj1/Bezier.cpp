#include "Bezier.h"

int Bezier::fac(int n) {
	int ans = 1;
	for (int i = 1; i <= n; i++) {
		ans *= i;
	}
	return ans;
}

int Bezier::C(int m, int n) {
	return fac(n) / fac(m) / fac(n - m);
}

Vector3 Bezier::curve3d(double u, double angle) {
	double tx = 0;
	double ty = 0;
	for (int i = 0; i < point_cnt; i++) {

	}
	return Vector3(0, 0, 0);
}