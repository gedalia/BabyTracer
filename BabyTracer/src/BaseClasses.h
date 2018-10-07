#pragma once

#include <vector>

#include "Camera.h"
#include "Image.h"

class Material;


struct HitRecord
{
   HitRecord() :t(0), mPoint(glm::vec3(0, 0, 0)), mNormal(glm::vec3(0, 0, 0)), mMaterial(nullptr) { ; }
   float t;
   glm::vec3 mPoint;
   glm::vec3 mNormal;
   Material * mMaterial;
};

class Material
{
public:
   Material()
   {
   }
   virtual bool Scatter(const Ray & r_in, const HitRecord & rec, glm::vec3 & attenuation, Ray & scattered) = 0;
   virtual  glm::vec3 ApplyLight(const glm::vec3 & lightDir, const glm::vec3 & lightColor, const Ray & r_in, const HitRecord & rec) = 0;

   virtual bool receivesShadows() { return false; }
   virtual bool CastsShadows() { return true; }

};

class Hitable
{
public:
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) { return false; }
};

class HitableList : public Hitable
{
public:
   HitableList() {
      ;
   }

   void add(Hitable * item) {
      mList.push_back(item);
   }

   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const
   {
      HitRecord temp_rec;
      bool hit_something = false;
      float closest = tmax;
      int listSize = mList.size();
      for (int i = 0; i < listSize; i++)
      {
         if (mList[i]->hit(ray, tmin, closest, shadow_ray, temp_rec)) {
            hit_something = true;
            closest = temp_rec.t;
            rec = temp_rec;
         }
      }
      return hit_something;
   }
   std::vector<Hitable *>mList;
};



class RayTracer
{
public:
   RayTracer() :mIncludeLight(true), mIncludeSkyLight(true) {
      mLightDir = glm::normalize(glm::vec3(-.2, .7, .67));
      mLightColor = glm::vec3(0.5f, 0.5f, 0.5f);
      mSkyColor = glm::vec3(.2, .5, 1.0);
   }
   glm::vec3 color_func(const Ray & ray, const HitableList & world, int & hits, int bounces = 0);

   void RayTraceScene(Image &myImage, int num_samples, bool accumulate_samples, bool debug_output = false);

   Camera mCamera;
   HitableList mWorld;
   bool mIncludeLight;
   bool mIncludeSkyLight;
   glm::vec3 mLightDir;
   glm::vec3 mLightColor;
   glm::vec3 mSkyColor;
};
