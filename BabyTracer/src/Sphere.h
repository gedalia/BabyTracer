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
      mVelocity = glm::vec3(0, 0, 0);
   }
   ~Sphere()
   {
      delete mMaterial;
   }
   glm::vec3 mCenter;
   glm::vec3 mVelocity;
   float mRadius;
   float mRadius2;
   float mInvRadius;
   Material * mMaterial;
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const;
   virtual void calcNormalAndMaterial(const Ray & ray, HitRecord & rec) const;

   virtual bool boundingBox(float t0, float t1, AABoundingBox & box) const;

};

bool Sphere::hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const
{
   if (shadow_ray && !mMaterial->castsShadows()) {
      return false;
   }

   glm::vec3 oc = ray.mOrigin  - (mCenter + ray.mTime*mVelocity);
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
         return true;
      }
      temp = (-b + sqrootres) / (a);
      if (temp < tmax && temp > tmin)
      {
         rec.t = temp;
         return true;
      }
   }
   return false;

}

void Sphere::calcNormalAndMaterial(const Ray & ray, HitRecord & rec) const
{
   rec.mPoint = ray.point_at_parameter(rec.t);
   rec.mNormal = (rec.mPoint - (mCenter + ray.mTime*mVelocity)) * mInvRadius;
   rec.mMaterial = mMaterial;
}


bool Sphere::boundingBox(float t0, float t1, AABoundingBox & box) const
{
   box = AABoundingBox(mCenter-glm::vec3(mRadius, mRadius, mRadius), mCenter + glm::vec3(mRadius, mRadius, mRadius));
   return true;
}
