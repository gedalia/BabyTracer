#include "BaseClasses.h"
#include "Image.h"

#include "CV_SDK/cvmarkersobj.h"
using namespace Concurrency::diagnostic;


void RayTracer::rayTraceScene(Image &image, int num_samples, bool accumulate_samples, bool debug_output)
{
   marker_series series;
   span rtspan(series, _T("ray trace"));

   float nx_inv = 1.0f / float(image.mWidth);
   float ny_inv = 1.0f / float(image.mHeight);
   float rand_scale = 1;
  int chunk_size = 1;;
  /*  if (image.mNumSamples < 25) {
      chunk_size = 10;
   }
   else if (image.mNumSamples < 50) {
      chunk_size = 4;
   }
   else if (image.mNumSamples < 100) {
       chunk_size = 2;
   }*/

   {
#pragma omp parallel for
    
      for (int i = 0; i < image.mWidth; i+= chunk_size)
      {
         if (debug_output && i % (image.mWidth / 10) == 0) {
            printf(".");
         }
         for (int j = 0; j < image.mHeight; j+= chunk_size)
         {
            int hits = 0;
            glm::vec3 color(0, 0, 0);
            float samples = 0;
            for (int s = 0; s < num_samples; s++)
            {
               float rx = rand_scale*(drand48() - .5f);
               float ry = rand_scale*(drand48() - .5f);
               float u = float(i + rx)*nx_inv;
               float v = float(j + ry)*ny_inv;
               Ray camRay = mCamera.get_ray(u, v);
               color += colorFunction(camRay, mWorld, hits);
               samples++;
               if (s == 10 && (hits == 0 || hits == 1)) {
                  break;
               }
            }
            if (accumulate_samples)
            {
               image.mData[i][j] += color ;
            }
            else {
               image.mData[i][j] = color / samples;
            }
            glm::vec3 result = image.mData[i][j];
            if (chunk_size > 1) {
               for (int q = 0; q < chunk_size; q++) {
                  for (int r = 0; r < chunk_size; r++) {
                     image.mData[i + q][j + r] = result;
                  }
               }
            }
         }
      }
   }
   if (accumulate_samples) {
      image.mNumSamples += num_samples;
   }
   else {
      image.mNumSamples = 1;
   }
}

glm::vec3 RayTracer::colorFunction(const Ray & ray, const HitableList & world, int & hits, int bounces)
{

   HitRecord rec;

   if (world.hit(ray, 0.001, 1000000, false, rec))
   {

      hits++;
      Ray scattered;
      scattered.mTime = ray.mTime;

      glm::vec3 attenuation;
      glm::vec3 GI = glm::vec3(0, 0, 0);

      if (bounces < mMaxBounces && rec.mMaterial->scatter(ray, rec, attenuation, scattered)) {
         GI = attenuation*colorFunction(scattered, world, hits, bounces + 1);
      }
      
      GI += rec.mMaterial->mEmissivity;

      glm::vec3 light_contrib(0, 0, 0);
      if (mIncludeLight && bounces < 3)
      {
         bool addLight = true;
         if (rec.mMaterial->receivesShadows())
         {
            // shadows
            HitRecord shadow_rec;
            if (world.hit(Ray(rec.mPoint, mLightDir + .1f*scattered.mDir, ray.mTime), .001f, 1000000, true, shadow_rec)) {
               light_contrib = glm::vec3(0.0f, 0.0f, 0.0f);
               addLight = false;
            }
         }

         if (addLight)
         {
            light_contrib = rec.mMaterial->applyLight(mLightDir, mLightColor, ray, rec);
         }
      }

      return (GI + light_contrib);
   }

   if (mIncludeSkyLight) 
   {
      glm::vec3 unit_dir = glm::normalize(ray.mDir);
      float t = 0.5f*(unit_dir[1] + 1.0f);
         return glm::mix(glm::vec3(1.0, 1.0, 1.0), mSkyColor, t);
      //return glm::mix(glm::vec3(1.0, 1.0, 1.0), glm::vec3(1., .5, .2), t);
   }
   return glm::vec3(0.01, 0.01, 0.01);

}

