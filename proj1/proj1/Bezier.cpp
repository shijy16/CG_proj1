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
		tx += x[i]*C(i, point_cnt - 1)*powl(u, i)*powl(1.0f - u, point_cnt - 1 - i);
		ty += y[i]*C(i, point_cnt - 1)*powl(u, i)*powl(1.0f - u, point_cnt - 1 - i);
	}
	return Vector3(pos.getX() + cosl(angle)*tx, pos.getY()+ty, pos.getZ()+tx*sinl(angle));
}