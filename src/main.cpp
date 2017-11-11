#define GLM_FORCE_RADIANS 
#include <glm/glm.hpp>
#include <iostream>
#include<fstream>
#include <vector>
#include <iomanip>
#include<string>

#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Sphere.h"
#include "Walls.h"
#include "Plane.h"
#include "Algorithms.h"
typedef glm::vec3 Color;
const int imgWidth = 40;
const int imgHeight = 40;
const int samplePerPixel = 200;

void FineArt() {
	std::cout << "\n\n\n\n\n\n";
	std::cout << "   ______   _______ __   __ _______ ______   _______ _______ _______ ______   " << std::endl;
	std::cout << "  |    _ | |   _   |  | |  |       |    _ | |   _   |       |       |    _ |  " << std::endl;
	std::cout << "  |   | || |  |_|  |  |_|  |_     _|   | || |  |_|  |       |    ___|   | ||  " << std::endl;
	std::cout << "  |   |_||_|       |       | |   | |   |_||_|       |       |   |___|   |_||_ " << std::endl;
	std::cout << "  |    __  |       |_     _| |   | |    __  |       |      _|    ___|    __  |" << std::endl;
	std::cout << "  |   |  | |   _   | |   |   |   | |   |  | |   _   |     |_|   |___|   |  | |" << std::endl;
	std::cout << "  |___|  |_|__| |__| |___|   |___| |___|  |_|__| |__|_______|_______|___|  |_|" << std::endl;
	std::cout << " " << std::setfill(' ') << std::setw(62) << "Anton Osterblad,  Mikael Zackrisson,  Aron Tornberg" << std::endl;
}
Image* readfile(char* filename) {
	std::ifstream ifs;
	
	

	try
	{
		ifs.open(filename, std::ios::binary);
		if (ifs.fail())
		{
			throw("Can't open output file");
			
		}
		char r, g, b;
		int width,height,maxcolval;
		std::string line;
		ifs >> line;
		ifs >> width;
		ifs >> height;
		ifs >> maxcolval;
		Image* img = new Image(width,height);
		//char* pixels = new char[imgWidth*imgHeight*3];
		//ifs.read(pixels, imgWidth*imgHeight*3);
		for (int i = 0; i < width*height; i++)
		{
			ifs.read(&r, 1);
			ifs.read(&g, 1);
			ifs.read(&b, 1);
			glm::vec3* pixel = new glm::vec3(static_cast<float> (static_cast<unsigned char>(g)) / maxcolval, 
				static_cast<float>(static_cast<unsigned char>(b)) / maxcolval, 
				static_cast<float>(static_cast<unsigned char>(r)) / maxcolval);
			img->SetPixel(i%height, i / width, *pixel);
		}
		return img;
	}
	catch (const std::exception&)
	{

	}
	return new Image();
	
}
void LoadScreen(int MAX_COUNT,int misscount) {
	static int counter = 0.0f;
	float percent_done = 100*counter/MAX_COUNT;
	std::cout << "\r" << std::setfill(' ') << std::setw(10) << std::setprecision(4) << percent_done << "% "<< misscount;
	for(int i = 0; i < percent_done/2; i ++) {
		char c = 178;
		std::cout << c;
	}
	for(int i = percent_done/2; i < 100/2; i ++) {
		char c = 176;
		std::cout << c;
	}
	++counter;
}
Color operator/ (Color v, float t) {
	v.r /= t;
	v.g /= t;
	v.b /= t;
	return v;
}
float distance(Color a, Color b) {
	return (a.r - b.r)*(a.r - b.r) + (a.g - b.g)*(a.g - b.g) + (a.b - b.b)*(a.b - b.b);
}
bool slowstartsample(int x, int y,Image* img,Image* imgptr,Camera* cam,Scene* scene,Algorithms* alg) {
	bool midbreak = false;
	glm::vec3 color = glm::vec3(0.0f);
	const int beginsamplenum = 16;
	int currentsamples = beginsamplenum;
	while (currentsamples < samplePerPixel / 2)
	{
		if (distance(img->getpixel(x, y), imgptr->getpixel(x, y)) < 0.001) {
			//std::cout << currentsamples <<' '<< x <<' '<< y <<' '<<count<< std::endl;
			midbreak = true;
			break;
		}
		for (int p = 0; p < currentsamples; p++) {

			Ray cameraRay = cam->GetRandomRayDirection(x, y, img);
			color += alg->Radiance(cameraRay, scene);
		}

		color /= currentsamples;
		if (currentsamples != beginsamplenum)
		{
			color = (color + img->getpixel(x, y)) / 2;
		}
		currentsamples *= 2;
		img->SetPixel(x, y, color);
		
	}
	return midbreak;
}
void constitersample(int x, int y, Image* img, Image* imgptr, Camera* cam, Scene* scene, Algorithms* alg) {
	glm::vec3 color = glm::vec3(0.0f);
	int currentsamples = samplePerPixel;
	for (int p = 0; p < currentsamples; p++) {

		Ray cameraRay = cam->GetRandomRayDirection(x, y, img);
		color += alg->Radiance(cameraRay, scene);
	}
	color /= currentsamples;
	img->SetPixel(x, y, color);
}
int main() {
	char* readImage = (char*)"imagecool5000.ppm";
	/*Image* simpleimgptr = new Image(10, 10);
	for (size_t i = 0; i < 100; i++)
	{
		glm::vec3* color =new glm::vec3(static_cast<float>(i) / 255);
		simpleimgptr->SetPixel(i % 10, i / 10, *color);
	}
	simpleimgptr->WritePPM(readImage);
	*/
	Image* imgptr = readfile(readImage);
	//char* outimg = (char*)"imagecooltest.ppm";
	//imgptr->WritePPM(outimg);
	//return 0;
	Scene *scene = new Scene();
	Shape *sh;

	//----------------------------
	//---------INIT SCENE---------
	//----------------------------
	sh = new Walls();
	scene->addObject(sh);

	// Two white spheres
	sh = new Sphere(0.3f);
	sh->SetPosition(glm::vec3(-0.55f, -0.2f, -0.55f));
	sh->SetColor(glm::vec3(1.0f));
	sh->roughness = 0.0f;
	scene->addObject(sh);

	sh = new Sphere(0.3f);
	sh->SetPosition(glm::vec3(0.55f, -0.2f, -0.55f));
	sh->SetColor(glm::vec3(1.0f));
	sh->roughness = 1.0f;
	scene->addObject(sh);

	// Two glass spheres
	sh = new Sphere(0.3f);
	sh->isTrans = true;
	sh->SetPosition(glm::vec3(-0.55f, -0.7f, 0.55f));
	scene->addObject(sh);

	sh = new Sphere(0.3f);
	sh->isTrans = true;
	sh->SetPosition(glm::vec3(0.55f, -0.7f, 0.55f));
	scene->addObject(sh);

	// Two lights
	sh = new Sphere(0.25f);
	sh->isLight = true;
	sh->SetColor(glm::vec3(6.0f));
	sh->SetPosition(glm::vec3(-0.3f, 1.0f, -0.3f));
	scene->addObject(sh);
	scene->addLight(sh);

	sh = new Sphere(0.25f);
	sh->isLight = true;
	sh->SetColor(glm::vec3(6.0f));
	sh->SetPosition(glm::vec3(0.3f, 1.0f, 0.3f));
	scene->addObject(sh);
	scene->addLight(sh);

	Image* img = new Image(imgWidth, imgHeight);
	Camera* cam = new Camera();
	Algorithms* alg = new Algorithms();

	//----------------------------
	//-----------RENDER-----------
	//----------------------------
	FineArt();
	int count = 0;
	#pragma omp parallel for
	for(int x = 0; x < img->GetWidth(); x++) {
		for(int y = 0; y < img->GetHeight(); y++) {
			constitersample(x, y, img, imgptr, cam, scene, alg);
			/*glm::vec3 color = glm::vec3(0.0f);
			const int beginsamplenum = 16;
			int currentsamples = beginsamplenum;
			while (currentsamples<samplePerPixel/2)
			{
				if (distance(img->getpixel(x, y), imgptr->getpixel(x, y)) < 0.001) {
					//std::cout << currentsamples <<' '<< x <<' '<< y <<' '<<count<< std::endl;
					count++;
					break;
				}
				for (int p = 0; p < currentsamples; p++) {
					
					Ray cameraRay = cam->GetRandomRayDirection(x, y, img);
					color += alg->Radiance(cameraRay, scene);
				}
				
				color /= currentsamples;
				if (currentsamples!=beginsamplenum)
				{
					color = (color + img->getpixel(x,y))/2;
				}
				currentsamples *= 2;
				img->SetPixel(x, y, color);
			}
			*/
			
		}
		#pragma omp critical
		LoadScreen(img->GetWidth(),count);
		//std::cout << count << std::endl;
		//std::cout << x << std::endl;
	}

	//----------------------------
	//-----------EXPORT-----------
	//----------------------------
	char* outImage = (char*)"imagecool.ppm";
	img->WritePPM(outImage);
	return EXIT_SUCCESS;
}