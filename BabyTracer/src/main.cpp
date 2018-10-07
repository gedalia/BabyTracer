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
 
   //int n = 500;
  // world.add(new Sphere(glm::vec3(0, -1000, 0), 1000, new Metal(glm::vec3(.7, .6, .5), 0.3)));

  world.add(new Sphere(glm::vec3(0, -1000, 0), 1000, new Lambertian(glm::vec3(.5, .5, .5), .9)));

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
            if (choose_mat < .8) {
               world.add(new Sphere(center, radius, new Lambertian(glm::vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()), drand48())));
            }
            else if (choose_mat < .95) {
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


int main()
{
   
//   const int IMAGE_WIDTH = 1000;
//   const int IMAGE_HEIGHT = 500;
   const int IMAGE_WIDTH = 400;
   const int IMAGE_HEIGHT = 200;
//   glm::vec3 lookFrom(6, 2, 2);
   glm::vec3 lookFrom(5, 2, 3);
   glm::vec3 lookAt(0, 1, 0);
   float aperture = .050f;
   //   float aperture = .0000f;
   RayTracer rt;
   rt.mCamera = Camera(lookFrom, lookAt, glm::vec3(0, 1, 0), 70, float(IMAGE_WIDTH) / float(IMAGE_HEIGHT), aperture);


   RandomScene(rt.mWorld);
   rt.mWorld.add(new Sphere(glm::vec3(0, 1, 0), 1.0, new Dielectric(1.5)));
   rt.mWorld.add(new Sphere(glm::vec3(-3, 1, 0), 1.0, new Lambertian(glm::vec3(.4, .2, .1), .1)));
   rt.mWorld.add(new Sphere(glm::vec3(3, 1, 0), 1.0, new Metal(glm::vec3(.7, .6, .5), 0.0)));

   Image myImage(IMAGE_WIDTH, IMAGE_HEIGHT);

   printf("starting generation\n");
   std::chrono::high_resolution_clock clock;
   std::chrono::high_resolution_clock::time_point start_time = clock.now();

   int num_samples = 1;
   rt.RayTraceScene(myImage, num_samples, 1, true);

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
