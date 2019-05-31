#include <iostream>
#include "builder.h"
/*
	Reference
		http://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_1_Introduction.shtml
		https://mhy12345.xyz/category/tutorials/graph-render/
		SPPM:
			https://blog.csdn.net/libing_zeng/article/details/76222498


*/
float SIZE = 1;
int main(void){
	time_t start, stop;
	start = time(NULL);
	Scene s = Scene();
	Object** o;
	o = new Object*[10];

	Meterial* m1 = s.getMeterial(1.0f, 0, 0.0f, 0.6);
	ColorTexture* t1 = s.getColorTexture(Color((float)192 / (float)255, (float)192 / (float)255, (float)192 / (float)255));
	o[1] = s.getSphere(m1, t1, Vector3(150* SIZE, 400 * SIZE, 100 * SIZE), 100 * SIZE);

	Meterial* m2 = s.getMeterial(0.0f,0, 1.0f, 0.6);
	ColorTexture* t2 = s.getColorTexture(Color((float)128 / (float)255, (float)0 / (float)255, (float)128 / (float)255));
	o[0] = s.getSphere(m2, t2, Vector3(100 * SIZE, 550 * SIZE, 50 * SIZE), 50 * SIZE);

	Meterial* m3 = s.getMeterial(0.0f, 1.33f, 0.0f, 0.6f);
	ColorTexture* t3 = s.getColorTexture(Color((float)255 / (float)255, (float)255 / (float)255, (float)255 / (float)255));
	o[2] = s.getSphere(m3, t3, Vector3(250 * SIZE, 500 * SIZE, 300 * SIZE), 100 * SIZE);

	Builder* b = new Builder();
	b->buildCamera(Vector3(600 * SIZE, 500 * SIZE, 400 * SIZE), Vector3(400 * SIZE, 200 * SIZE, 100 * SIZE), Vector3(0, 0,1), Vector3(0, 1, 0),600 * SIZE,600 * SIZE);
	//b->buildCamera(Vector3(150 * SIZE, 500 * SIZE, 999 * SIZE), Vector3(600 * SIZE, 200 * SIZE, 800 * SIZE), Vector3(-1, 0, 0), Vector3(0, 1, 0), 600* SIZE, 600 * SIZE);
	b->buildScene(o, 3, SIZE);
	b->rayTracer_begin();
	stop = time(NULL);
	printf("Used Time:%ld\n", (stop - start));
	cv::waitKey(0);
	return 0;
}
