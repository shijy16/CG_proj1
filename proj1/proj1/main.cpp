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
	Object** o;
	o = new Object*[10];

	ColorTexture* tt = s.getColorTexture(Color(25, 100, 255));
	o[0] = s.getSphere(m, tt, Vector3(100, 550, 50), 50);
	o[0]->setAsLight();


	Texture* t = s.getPicTexture("texture.png");
	o[1] = s.getSphere(m, t, Vector3(150, 400, 100), 100);
	o[1]->setAsLight(new Light(Vector3(), Color(0, 125, 255), 1000, 0));


	Builder* b = new Builder();
	b->buildCamera(Vector3(600, 500, 500), Vector3(400, 200, 200), Vector3(0, 0,1), Vector3(0, 1, 0),600,600);
	b->buildScene(o, 2);
	b->rayTracer_begin();
	stop = time(NULL);
	printf("Used Time:%ld\n", (stop - start));
	return 0;
}
