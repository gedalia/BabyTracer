#include "stdafx.h"
#include "glad/glad.h"

#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <stdlib.h>
#include <string>
#include <limits.h>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include <chrono>
#include "Image.h"
#include "Camera.h"
#include "BaseClasses.h"
#include "Materials.h"
#include "Sphere.h"
#include "ConstantMedium.h"
#include "AAPlanes.h"
#include "Box.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define M_PI       3.14159265358979323846   // pi

double drand48();

void LaunchUI(RayTracer * rt, Image * image);
//float drand48()
//{
//   return   rand() / (float)(RAND_MAX + 1.0);
//}


using namespace std; 


void RandomScene(HitableList &world)
{
   world.add(new Sphere(glm::vec3(0, 1, 0), 1.0, new Dielectric(1.5)));
   world.add(new Sphere(glm::vec3(-3, 1, 0), 1.0, new Lambertian(new ConstantTexture(glm::vec3(.4, .2, .1)), .1)));
   world.add(new Sphere(glm::vec3(3, 1, 0), 1.0, new Metal(glm::vec3(.7, .6, .5), 0.0)));

   //int n = 500;
  // world.add(new Sphere(glm::vec3(0, -1000, 0), 1000, new Metal(glm::vec3(.7, .6, .5), 0.3)));
   Material * mat = new Lambertian(new CheckerTexture(
      new ConstantTexture(glm::vec3(.2, .3, .1)),
      new ConstantTexture(glm::vec3(.9, .9, .9)) ), .9);
   world.add(new Sphere(glm::vec3(0, -1000, 0), 1000, mat));

   const int worldsize = 2;
   int i = 1;
   for (int a = -worldsize; a < worldsize; a++)
   {
      for (int b = -worldsize; b < worldsize; b++)
      {
         float choose_mat = drand48();
         glm::vec3 center(a + .9*drand48(), .2, b + .9*drand48());
         if (glm::length(center - glm::vec3(4, .2, 0)) > .9)
         {
            float radius = .2f + .1*drand48();
            center[1] = radius;
            if (choose_mat < .8) 
            {
               Sphere * sphere = new Sphere(center, radius, new Lambertian(
                  new ConstantTexture(glm::vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())), drand48()));
               sphere->mVelocity = glm::vec3(0, drand48(), 0);
               world.add(sphere);
            }
            else if (choose_mat < .95) 
            {
               glm::vec3 alb = glm::vec3(.5*(1 + drand48()), .5*(1 + drand48()), .5*(1 + drand48()));
               world.add(new Sphere(center, radius, new Metal(alb, drand48())));
            }
            else {
               world.add(new Sphere(center, radius, new Dielectric(1.5)));
            }
         }
      }
   }
}

void CornellBox(HitableList &world)
{
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("data/earth.jpg", &nx, &ny, &nn, 0);
	Material *earth_mat = new EmissiveMat(2.25, new ImageTexture(tex_data, nx, ny));
	Material *earth_mat2 = new EmissiveMat(1, new ImageTexture(tex_data, nx, ny));

//	world.add(new Sphere(glm::vec3(0, -1000, 0), 1000, mat));
//	world.add(new Sphere(glm::vec3(0, 0, 0), 1, mat));
//	return;
	world.add(new Sphere(glm::vec3(0, 1, 0), 1, earth_mat));

	world.add(new Sphere(glm::vec3(0, -1000, 0), 1000, new Lambertian(new ConstantTexture(glm::vec3(.5, .5, .5)), .9)));
   // rt.mWorld.add(new Sphere(glm::vec3(-2, 5, 2), .5, new EmissiveMat(new ConstantTexture(glm::vec3(20, 20, 20)))));


   world.add(new Sphere(glm::vec3(0, 2.75, 0), .750, new Dielectric(1.5)));
   world.add(new Sphere(glm::vec3(-3, 1, 0), 1.0, new Lambertian(new ConstantTexture(glm::vec3(.4, .2, .1)), .1)));
   world.add(new Sphere(glm::vec3(3, 1, 0), 1.0, new Metal(glm::vec3(.7, .6, .5), 0.0)));
   world.add(new ConstantMedium(new Sphere(glm::vec3(0, 3.5, 3), 1.5, NULL), .9, new ConstantTexture(glm::vec3(1, 1, 1))));

   world.add(new XZRect(-4, 4, -4, 4, 8, new Lambertian(new ConstantTexture(glm::vec3(.5, .5, .5)), .5)));
  // light
	//world.add(new XZRect(-2, 2, -2, 2, 7.9, new EmissiveMat(4.0f*glm::vec3(.5, .5, .5))));

	// ceiling
//   world.add(new XYRect(-4, 4, 0, 8, 4, new Lambertian(new ConstantTexture(glm::vec3(.0, .9, 0)), .5)));
	world.add(new XYRect(-4, 4, 0, 8, 4, earth_mat2));

	//  rt.mWorld.add(new XYRect(0, 5, 1, 3, -3, new Metal(glm::vec3(.7, .6, .5), 0.0)));
   // rt.mWorld.add(new XYRect(0, 5, 1, 3, 3, new Metal(glm::vec3(.7, .6, .5), 0.0)));
   world.add(new YZRect(0, 8, -3.5, 4, 4, new Lambertian(new ConstantTexture(glm::vec3(.9, 0, 0)), .5)));
   world.add(new YZRect(0, 8, -3.5, 4, -4, new Lambertian(new ConstantTexture(glm::vec3(0, 0, .9)), .5)));
}

void PBRTester(HitableList &world)
{
   world.add(new Sphere(glm::vec3(0, -1000, 0), 1000, new Lambertian(new ConstantTexture(glm::vec3(.5, .5, .5)), .9)));
   world.add(new ConstantMedium(new Sphere(glm::vec3(0, 3.5, 3), 8, NULL), .9, new ConstantTexture(glm::vec3(1, 1, 1))));

   for (int i = 0; i < 10; i++) 
   {
      world.add(new Sphere(glm::vec3(-2.5f, .9f, 2 * i - 10), .9f, new Metal(glm::vec3(.7, .6, .5), i / 10.0f)));

      world.add(new Sphere(glm::vec3(-0.5f, .9f, 2 * i - 10), .9f, new Lambertian(new ConstantTexture(glm::vec3(.8, .1, .1)), i / 10.0f)));
      world.add(new Sphere(glm::vec3(1.5f, .9f, 2 * i - 10), .9f, new Lambertian(new ConstantTexture(glm::vec3(.1, .8, .1)), i / 10.0f)));
      world.add(new Sphere(glm::vec3(3.50f, .9f, 2 * i - 10), .9f, new Lambertian(new ConstantTexture(glm::vec3(.1, .1, .8)), i / 10.0f)));
      world.add(new Sphere(glm::vec3(5.5f, .9f, 2 * i - 10), .9f, new Lambertian(new ConstantTexture(glm::vec3(.1, .1, .1)), i / 10.0f)));
   }
   
   world.add(new XZRect(-2, 2, -2, 2, 8.5f, new EmissiveMat(8.0f, glm::vec3(.5, .5, .5))));

//   world.add(new Sphere(glm::vec3(-3, 1, 0), 1.0, new Lambertian(new ConstantTexture(glm::vec3(.4, .2, .1)), .1)));

   // rt.mWorld.add(new Sphere(glm::vec3(-2, 5, 2), .5, new EmissiveMat(new ConstantTexture(glm::vec3(20, 20, 20)))));

//   world.add(new Sphere(glm::vec3(0, 1, 0), 1.0, new Dielectric(1.5)));
//   world.add(new Sphere(glm::vec3(-3, 1, 0), 1.0, new Lambertian(new ConstantTexture(glm::vec3(.4, .2, .1)), .1)));

}


int main()
{
   
 //  const int IMAGE_WIDTH = 1000;
 //  const int IMAGE_HEIGHT = 500;
#ifdef _DEBUG
   const int IMAGE_WIDTH = 1000;
   const int IMAGE_HEIGHT = 500;

#else // DEBUG
   const int IMAGE_WIDTH = 1000;
   const int IMAGE_HEIGHT = 500;
#endif

   glm::vec3 lookFrom(7, 2, 0);
   glm::vec3 lookAt(0, 2, 0);
   float aperture = .050f;

   RayTracer rt;
   rt.mCamera = Camera(lookFrom, lookAt, glm::vec3(0, 1, 0), 70, float(IMAGE_WIDTH) / float(IMAGE_HEIGHT), aperture);

   //RandomScene(rt.mWorld);
/*
   rt.mWorld.add(new Sphere(glm::vec3(0, -1000, 0), 1000, new Lambertian(new ConstantTexture(glm::vec3(.5, .5, .5)), .9)));
   rt.mWorld.add(new Sphere(glm::vec3(-3, 1, 0), 1.0, new Lambertian(new ConstantTexture(glm::vec3(.28, .55, .24)), .3)));
   /*rt.mWorld.add(new Box(glm::vec3(2, 0, 0), glm::vec3(4, 2, 2), new Metal(glm::vec3(.7, .6, .5), 0.0)));
   rt.mWorld.add(new ConstantMedium(new Sphere(glm::vec3(0, 0, 0), 8, NULL), .01, new ConstantTexture(glm::vec3(1, 1, 1))));
   rt.mWorld.add(new Sphere(glm::vec3(-3, 1, 0), 1.0, new Lambertian(new ConstantTexture(glm::vec3(.28, .55, .24)), .3)));
   rt.mWorld.add(new Sphere(glm::vec3(0, 1, 0), 1.0, new Dielectric(1.5)));
   rt.mWorld.add(new Sphere(glm::vec3(0, 6, 1), .9, new EmissiveMat(10.0f,glm::vec3(1.1, 1, 1))));
*/

   CornellBox(rt.mWorld);

   //PBRTester(rt.mWorld);

   Image myImage(IMAGE_WIDTH, IMAGE_HEIGHT);

   printf("starting generation\n");
   std::chrono::high_resolution_clock clock;
   std::chrono::high_resolution_clock::time_point start_time = clock.now();

   int num_samples = 1;
   rt.rayTraceScene(myImage, num_samples, 1, true);

   std::chrono::high_resolution_clock::time_point end_time = clock.now();

   std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
   cout << std::endl << "elapsed time " << elapsed.count()/1000.0f << " secs" << std::endl;

   myImage.saveToPPM("out.ppm");
   printf("\nsaving data\n");
   

   printf("finished generation\n");
   LaunchUI(&rt, &myImage );

   exit(EXIT_SUCCESS);


   return 0;
}
