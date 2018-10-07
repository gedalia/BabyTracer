#include "BaseClasses.h"
#include "Image.h"

#define NBOUNCES 50

void RayTracer::RayTraceScene(Image &image, int num_samples, bool accumulate_samples, bool debug_output)
{
   float nx_inv = 1.0f / float(image.mWidth);
   float ny_inv = 1.0f / float(image.mHeight);
   float rand_scale = 1;
   {
#pragma omp parallel for
      for (int i = 0; i < image.mWidth; i++)
      {
         if (debug_output && i % (image.mWidth / 10) == 0) {
            printf(".");
         }
         for (int j = 0; j < image.mHeight; j++)
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
               color += color_func(camRay, mWorld, hits);
               samples++;
               if (s == 10 && (hits == 0 || hits == 1)) {
                  break;
               }
            }
            if (accumulate_samples)
            {
               image.mData[i][j] += color / samples;
            }
            else {
               image.mData[i][j] = color / samples;
            }
         }
      }
   }
   if (accumulate_samples) {
      image.mNumSamples++;
   }
   else {
      image.mNumSamples = 1;
   }
}

glm::vec3 RayTracer::color_func(const Ray & ray, const HitableList & world, int & hits, int bounces)
{
   if (bounces >= NBOUNCES) {
      return glm::vec3(0, 0, 0);
   }


   HitRecord rec;


   if (world.hit(ray, 0.001, 1000000, false, rec))
   {
      hits++;
      Ray scattered;
      glm::vec3 attenuation;
      glm::vec3 GI = glm::vec3(0, 0, 0);

      if (rec.mMaterial->Scatter(ray, rec, attenuation, scattered)) {
         GI = attenuation*color_func(scattered, world, hits, bounces + 1);
      }
      
      glm::vec3 light_contrib(0, 0, 0);
      if (mIncludeLight && bounces < 3)
      {
         bool addLight = true;
         if (rec.mMaterial->receivesShadows())
         {
            // shadows
            HitRecord shadow_rec;
            if (world.hit(Ray(rec.mPoint, mLightDir + .1f*scattered.mDir), .001f, 1000000, true, shadow_rec)) {
               light_contrib = glm::vec3(0.0f, 0.0f, 0.0f);
               addLight = false;
            }
         }

         if (addLight)
         {
            light_contrib = rec.mMaterial->ApplyLight(mLightDir, mLightColor, ray, rec);
         }
      }


      return (GI + light_contrib);
   }
   if (mIncludeSkyLight) {
      glm::vec3 unit_dir = glm::normalize(ray.mDir);
      float t = 0.5f*(unit_dir[1] + 1.0f);
         return glm::mix(glm::vec3(1.0, 1.0, 1.0), mSkyColor, t);
      //return glm::mix(glm::vec3(1.0, 1.0, 1.0), glm::vec3(1., .5, .2), t);
   }
   return glm::vec3(0, 0, 0);

}

