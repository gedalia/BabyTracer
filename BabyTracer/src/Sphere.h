#pragma once
#include "BaseClasses.h"

class Sphere : public Hitable
{
public:
   Sphere() { ; }
   Sphere(const glm::vec3 & center, float r, Material * mat) : mCenter(center), mRadius(r), mMaterial(mat)
   {
      mRadius2 = mRadius*mRadius;
      mInvRadius = 1.0 / mRadius;
   }
   ~Sphere()
   {
      delete mMaterial;
   }
   glm::vec3 mCenter;
   float mRadius;
   float mRadius2;
   float mInvRadius;
   Material * mMaterial;
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec);
};

bool Sphere::hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec)
{
   if (shadow_ray && !mMaterial->CastsShadows()) {
      return false;
   }

   glm::vec3 oc = ray.mOrigin - mCenter;
   float a = glm::dot(ray.mDir, ray.mDir);
   float b = glm::dot(oc, ray.mDir);
   float c = dot(oc, oc) - mRadius2;
   float discrimant = b*b - a*c;

   if (discrimant > 0.0f) 
   {
      float sqrootres = sqrt(discrimant);
      float temp = (-b - sqrootres) / (a);
      if (temp < tmax && temp > tmin) 
      {
         rec.t = temp;
         rec.mPoint = ray.point_at_parameter(temp);
         rec.mNormal = (rec.mPoint - mCenter) *mInvRadius;
         rec.mMaterial = mMaterial;
         return true;
      }
      temp = (-b + sqrootres) / (a);
      if (temp < tmax && temp > tmin)
      {
         rec.t = temp;
         rec.mPoint = ray.point_at_parameter(temp);
         rec.mNormal = (rec.mPoint - mCenter) * mInvRadius;
         rec.mMaterial = mMaterial;
         return true;
      }
   }
   return false;

}

