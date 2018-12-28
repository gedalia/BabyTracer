#pragma once

#include <vector>

#include "Camera.h"
#include "Image.h"

class Material;
class Hitable;
class Texture;

struct HitRecord
{
   HitRecord() :t(0), mPoint(glm::vec3(0, 0, 0)), mNormal(glm::vec3(0, 0, 0)), mObject(nullptr), u(0), v(0) { ; }
   glm::vec3 mPoint;
   glm::vec3 mNormal;
   float u;
   float v;
   float t;
   Hitable * mObject;
};

class Material
{
public:
   Material(Texture * a) : mAlbedo(a)
   {
   }

   Material() : mAlbedo(NULL)
   {
   }
   virtual bool scatter(const Ray & r_in, const HitRecord & rec, glm::vec3 & attenuation, Ray & scattered) const= 0;
   virtual  glm::vec3 applyLight(const glm::vec3 & lightDir, const glm::vec3 & lightColor, const Ray & r_in, const HitRecord & rec) = 0;

   virtual bool receivesShadows() { return false; }
   virtual bool castsShadows() { return true; }
   virtual void emissivity(const Ray & ray, const HitRecord & rec, glm::vec3 & color) { ; }
   Texture * mAlbedo;

};

class Hitable
{
public:
   Hitable() :mVisible(true) { ; }
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const { return false; }
   virtual void calcNormal(const Ray & ray, HitRecord & rec) const { ; }
	virtual void calcUV(const Ray & ray, HitRecord & rec) const { ; }
   virtual bool boundingBox(float t0, float t1, AABoundingBox & box) const { return false; }
   virtual Material * getMaterial() = 0;
   virtual const char * className() = 0;
   bool mVisible;
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
      size_t listSize = mList.size();
      int hitIndex = -1;
      for (int i = 0; i < listSize; i++)
      {
         if (mList[i]->mVisible && mList[i]->hit(ray, tmin, closest, shadow_ray, temp_rec)) {
            hit_something = true;
            closest = temp_rec.t;
            rec = temp_rec;
            hitIndex = i;
         }
      }
      if (hitIndex != -1)
      {
         // finish off the calculation
         rec.mObject = mList[hitIndex];
         mList[hitIndex]->calcNormal(ray, rec);
         mList[hitIndex]->calcUV(ray, rec);
      }
      return hit_something;
   }
   std::vector<Hitable *>mList;
   virtual Material * getMaterial() { return NULL; }
   virtual const char * className() { return "HitableList"; }

};



class RayTracer
{
public:
   RayTracer() :mIncludeLight(false), mIncludeSkyLight(true) {
      mLightDir = glm::normalize(glm::vec3(-.2, .7, .67));
      mLightColor = glm::vec3(0.5f, 0.5f, 0.5f);
      mSkyColor = glm::vec3(.2, .5, 1.0);
      mMaxBounces = 50;
   }
   glm::vec3 colorFunction(const Ray & ray, const HitableList & world, int & hits, int bounces = 0);

   void rayTraceScene(Image &myImage, int num_samples, bool accumulate_samples, bool debug_output = false);

   Camera mCamera;
   HitableList mWorld;
   bool mIncludeLight;
   bool mIncludeSkyLight;
   glm::vec3 mLightDir;
   glm::vec3 mLightColor;
   glm::vec3 mSkyColor;
   int mMaxBounces;
};
