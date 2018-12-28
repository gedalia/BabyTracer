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
   virtual const char * className() { return "Sphere"; }

   virtual Material * getMaterial() { return mMaterial; }
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const;
   virtual void calcNormal(const Ray & ray, HitRecord & rec) const;
   virtual void calcUV(const Ray & ray, HitRecord & rec) const;

   virtual bool boundingBox(float t0, float t1, AABoundingBox & box) const;

   glm::vec3 mCenter;
   glm::vec3 mVelocity;
   Material * mMaterial;

   float getRadius() const { return mRadius; }
   void setRadius(float rad);

protected:
   float mRadius;
   float mRadius2;
   float mInvRadius;

};
