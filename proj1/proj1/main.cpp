#include <iostream>
#include "builder.h"
/*
	Reference
		http://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_1_Introduction.shtml
		https://mhy12345.xyz/category/tutorials/graph-render/
		SPPM:
			https://blog.csdn.net/libing_zeng/article/details/76222498


*/
int main(void){
	time_t start, stop;
	start = time(NULL);
	Scene s = Scene();
	Meterial* m = s.getMeterial(0, 0, 0, 0);
	ColorTexture* t = s.getColorTexture(Color(0, 255, 6));
	Object** o;
	o = new Object*[10];
	o[0] = s.getSphere(m, t, Vector3(0, 0, 500), 50);
	o[0]->setAsLight();
	ColorTexture* tt = s.getColorTexture(Color(0, 0, 255));
	o[1] = s.getSphere(m, tt, Vector3(0, 0, 0), 50);

	o[1]->setAsLight(new Light(Vector3(), Color(0, 0, 255), 1000, 0));
	Builder* b = new Builder();
	b->buildCamera(Vector3(1000, 500, 500), Vector3(600, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	b->buildScene(o, 2);
	b->rayTracer_begin();
	stop = time(NULL);
	printf("Used Time:%ld\n", (stop - start));
	system("pause");
	return 0;
}
