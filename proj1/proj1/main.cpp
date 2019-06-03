#include <iostream>
#include "builder.h"
#include "Bezier.h"
/*
	Reference
		http://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_1_Introduction.shtml
		https://mhy12345.xyz/category/tutorials/graph-render/
		SPPM:
			https://blog.csdn.net/libing_zeng/article/details/76222498


*/
double SIZE = 1;
int main(void){
	time_t start, stop;
	start = time(NULL);
	Scene s = Scene();
	Object** o;
	o = new Object*[10];

	//scene1
	//Meterial* m1 = s.getMeterial(1.0f, 0, 0.0f, 0.6);
	//ColorTexture* t1 = s.getColorTexture(Color((double)192 / (double)255, (double)192 / (double)255, (double)192 / (double)255));
	//o[1] = s.getSphere(m1, t1, Vector3(150* SIZE, 400 * SIZE, 100 * SIZE), 100 * SIZE);

	//Meterial* m2 = s.getMeterial(0.0f,0, 1.0f, 0.6);
	//ColorTexture* t2 = s.getColorTexture(Color((double)128 / (double)255, (double)0 / (double)255, (double)128 / (double)255));
	//o[0] = s.getSphere(m2, t2, Vector3(100 * SIZE, 550 * SIZE, 50 * SIZE), 50 * SIZE);

	//Meterial* m3 = s.getMeterial(0.2f, 1.33f, 0.0f, 1.3f);
	//ColorTexture* t3 = s.getColorTexture(Color((double)0 / (double)255, (double)0 / (double)255, (double)0 / (double)255));
	//o[2] = s.getSphere(m3, t3, Vector3(250 * SIZE, 500 * SIZE, 500 * SIZE), 100 * SIZE);

	//Builder* b = new Builder();
	//b->buildCamera(Vector3(600 * SIZE, 500 * SIZE, 400 * SIZE), Vector3(400 * SIZE, 200 * SIZE, 100 * SIZE), Vector3(0, 0,1), Vector3(0, 1, 0),600 * SIZE,600 * SIZE);
	//b->buildScene(o, 3, SIZE);

	//Scene 2
	Meterial* m1 = s.getMeterial(0.5f, 0, 0.2f, 0.6);
	ColorTexture* t1 = s.getColorTexture(Color((double)192 / (double)255, (double)192 / (double)255, (double)192 / (double)255));
	o[1] = s.getSphere(m1, t1, Vector3(150 * SIZE, 100 * SIZE, 150 * SIZE), 80 * SIZE);

	Meterial* m2 = s.getMeterial(0.2f, 1.33f, 0.5f, 0.8f);
	ColorTexture* t2 = s.getColorTexture(Color((double)133 / (double)255, (double)133 / (double)255, (double)133 / (double)255));
	o[0] = s.getSphere(m2, t2, Vector3(150 * SIZE, 300 * SIZE, 150 * SIZE), 80 * SIZE);

	Meterial* m3 = s.getMeterial(0.5f, 0, 0.2f, 0.6);
	//m3->diff_reflect = 0.5f;
	ColorTexture* t3 = s.getColorTexture(Color((double)192 / (double)255, (double)192 / (double)255, (double)192 / (double)255));
	o[2] = s.getSphere(m3, t3, Vector3(150 * SIZE, 500 * SIZE, 150 * SIZE), 80 * SIZE);

	Meterial* m4 = s.getMeterial(0.0f, 0, 0.8f, 0.6);
	PicTexture* t4 = s.getPicTexture("marble.png");
	o[3] = s.getSphere(m4, t4, Vector3(150 * SIZE, 700 * SIZE, 150 * SIZE), 80 * SIZE);

	Builder* b = new Builder(SIZE);
	b->buildCamera(Vector3(500 * SIZE, 400 * SIZE, 150 * SIZE), Vector3(400 * SIZE, 200 * SIZE, 50 * SIZE), Vector3(0, 0, 1), Vector3(0, 1, 0), 400 * SIZE, 200 * SIZE);
	b->buildScene2(o, 4);


	b->rayTracer_begin();
	stop = time(NULL);
	printf("Used Time:%ld\n", (stop - start));
	cv::waitKey(0);
	return 0;
}
