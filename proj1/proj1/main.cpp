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
	Meterial* m = s.getMeterial(0.5, 0.2, 0.1, 0.6);
	Object** o;
	o = new Object*[10];

	ColorTexture* tt = s.getColorTexture(Color(25, 100, 255));
	o[0] = s.getSphere(m, tt, Vector3(100, 550, 50), 50);


	Texture* t = s.getPicTexture("texture.png");
	o[1] = s.getSphere(m, t, Vector3(150, 400, 100), 100);


	Builder* b = new Builder();
	b->buildCamera(Vector3(600, 500, 500), Vector3(400, 200, 200), Vector3(0, 0,1), Vector3(0, 1, 0),600,600);
	b->buildScene(o, 2);
	b->rayTracer_begin();
	stop = time(NULL);
	printf("Used Time:%ld\n", (stop - start));
	cv::waitKey(0);
	return 0;
}
