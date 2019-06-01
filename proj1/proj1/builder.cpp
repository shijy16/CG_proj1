#include "builder.h"

void Builder::buildCamera(Vector3 eye, Vector3 origin, Vector3 dx, Vector3 dy,int x,int y) {
	camera = new Camera(eye, origin, dx, dy,x,y);
}

void Builder::buildScene(Object** objs, int num, float SIZE) {
	scene = new Scene();
	for (int i = 0; i < num; i++) {
		scene->addObj(objs[i]);
	}
	//build the room
	Meterial* wall_m = new Meterial(0, 0, 1,0);
	Meterial* groud_m = new Meterial(0.2f, 0, 0.7f, 0);
	Texture* wall_red = new ColorTexture(Color(1, 0, 0));
	Texture* wall_yellow = new ColorTexture(Color(1, 1, 0));
	Texture* wall_bl = new ColorTexture(Color(0, 1, 1));
	Texture* wall_white = new ColorTexture(Color(1, 1, 1));
	Texture* ground = new PicTexture("floor1.bmp");
	Texture* wall_paper = new PicTexture("texture.png");
	//Texture * ground = new ColorTexture(Color(149.0f/255.0f, 107.0f/255.0f, 8.0f/255.0f));
	Plane* w1 = new  Plane(wall_m, wall_paper, Vector3(0, 0, 0), Vector3(1, 0, 0),0);			//正面
	Plane* w2 = new  Plane(wall_m, wall_yellow, Vector3(0, 0, 0), Vector3(0, 1, 0),0);				//左边
	Plane* w3 = new  Plane(groud_m, ground, Vector3(0, 0, 0), Vector3(0, 0, 1),0);				//地板
	Plane* w4 = new  Plane(wall_m, wall_white, Vector3(1000*SIZE,0, 0), Vector3(-1, 0, 0),1000*SIZE);
	Plane* w5 = new  Plane(wall_m, wall_bl, Vector3(0, 1000*SIZE, 0), Vector3(0, -1, 0),1000*SIZE);		//右边
	Plane* w6 = new  Plane(wall_m, wall_bl, Vector3(0, 0, 1000*SIZE), Vector3(0, 0, -1),1000*SIZE);			//天花板
	Sphere* lightSource = new Sphere(new Meterial(0, 0, 0, 0),new ColorTexture(Color(1, 1, 1)), Vector3(200* SIZE, 500*SIZE, 999*SIZE),40.0);
	//Sphere* lightSource2 = new Sphere(new Meterial(0, 0, 0, 0), new ColorTexture(Color(1, 1, 1)), Vector3(200 * SIZE, 100 * SIZE, 200 * SIZE), 40.0);
	lightSource->setAsLight(new Light(Vector3(200*SIZE, 500*SIZE, 999*SIZE),Color(1,1,1),5000,1));
	//lightSource2->setAsLight(new Light(Vector3(200*SIZE, 100*SIZE, 200 * SIZE), Color(1, 1, 1), 5000, 1));
	scene->addObj(w1);
	scene->addObj(w2);
	scene->addObj(w3);
	scene->addObj(w4);
	scene->addObj(w5);
	scene->addObj(w6);
	scene->addObj(lightSource);
	//scene->addObj(lightSource2);
}

void Builder::buildScene2(Object** objs, int num, float SIZE) {
	scene = new Scene();
	for (int i = 0; i < num; i++) {
		scene->addObj(objs[i]);
	}
	//build the room
	Meterial* wall_m = new Meterial(0, 0, 1, 0);
	Meterial* groud_m = new Meterial(0.2f, 0, 0.7f, 0);
	Texture* wall_bl = new ColorTexture(Color(0, 1, 1));
	Texture* ground = new PicTexture("floor1.bmp");
	//Texture * ground = new ColorTexture(Color(149.0f/255.0f, 107.0f/255.0f, 8.0f/255.0f));
	Plane* w1 = new  Plane(wall_m, wall_bl, Vector3(0, 0, 0), Vector3(1, 0, 0), 0);			//正面
	Plane* w3 = new  Plane(groud_m, ground, Vector3(0, 0, 0), Vector3(0, 0, 1), 0);				//地板
	//Sphere* lightSource = new Sphere(new Meterial(0, 0, 0, 0), new ColorTexture(Color(1, 1, 1)), Vector3(200 * SIZE, 200 * SIZE, 500 * SIZE), 40.0);
	//lightSource->setAsLight(new Light(Vector3(200 * SIZE, 200 * SIZE, 500 * SIZE), Color(1, 1, 1), 5000, 1));
	Area* lightSource = new Area(new Meterial(0, 0, 0, 0), new ColorTexture(Color(1, 1, 1)), Vector3(190 * SIZE, 300 * SIZE, 450 * SIZE), Vector3(0, 50 * SIZE, 0), Vector3(50 * SIZE, 0, 0));
	lightSource->setAsLight(new Light(Vector3(200 * SIZE, 300 * SIZE, 500 * SIZE), Color(1, 1, 1), 5000, 1));
	scene->addObj(lightSource);
	scene->addObj(w1);
	scene->addObj(w3);
}
cv::Mat Builder::rayTracer_begin() {
	RayTracer tracer = RayTracer(camera, scene,camera->lenX, camera->lenY);
	tracer.run();
	return tracer.getImg();
}