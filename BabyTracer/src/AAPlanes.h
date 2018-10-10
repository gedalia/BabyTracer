#pragma once
#include "BaseClasses.h"

class AAPlanes : public Hitable
{
public:
   AAPlanes() : mMaterial (nullptr){

   }
   AAPlanes(Material * mat) : mMaterial(mat) { 
      ;
   }
   AABoundingBox mBounds;
   glm::vec3 mVelocity;

   virtual  ~AAPlanes()
   {
      delete mMaterial;
   }
   Material * mMaterial;

   bool boundingBox(float t0, float t1, AABoundingBox & box) const
   {
      box = mBounds;
      return true;
   }

};

class XYRect : public AAPlanes
{
public:
   XYRect() { ; }
   XYRect(float x0, float x1, float y0, float y1, float k, Material * mat) : AAPlanes(mat)
   {
      mBounds = AABoundingBox(glm::vec3(x0, y0, k-.0001), glm::vec3(x1, y1, k + .0001));
      mVelocity = glm::vec3(0, 0, 0);
   }
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const;
   virtual void calcNormalAndMaterial(const Ray & ray, HitRecord & rec) const;
};

bool XYRect::hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const
{
   if (shadow_ray && !mMaterial->castsShadows()) {
      return false;
   }
   float t = (mBounds.mMax[2]- ray.mOrigin[2]) / ray.mDir[2];
   if (t < tmin || t > tmax) {
      return false;
   }
   float x = ray.mOrigin[0] + t*ray.mDir[0];
   float y = ray.mOrigin[1] + t*ray.mDir[1];
   if (x < mBounds.mMin[0]|| x > mBounds.mMax[0] || y < mBounds.mMin[1] || y > mBounds.mMax[1]) {
      return false;
   }
   rec.u = (x - mBounds.mMin[0]) / (mBounds.mMax[0] - mBounds.mMin[0]);
   rec.v = (y - mBounds.mMin[1]) / (mBounds.mMax[1] - mBounds.mMin[1]);
   rec.t = t;
   return true;
}

void XYRect::calcNormalAndMaterial(const Ray & ray, HitRecord & rec) const
{
   rec.mPoint = ray.point_at_parameter(rec.t);
   rec.mMaterial = mMaterial;
   rec.mNormal = glm::vec3(0, 0, ray.mDir[2] > 0 ? -1 : 1);
}


class XZRect : public AAPlanes
{
public:
   XZRect() { ; }
   XZRect(float x0, float x1, float z0, float z1, float k, Material * mat) : AAPlanes(mat)
   {
      mBounds = AABoundingBox(glm::vec3(x0, k - .0001, z0), glm::vec3(x1, k + .0001, z1));
      mVelocity = glm::vec3(0, 0, 0);
   }
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const;
   virtual void calcNormalAndMaterial(const Ray & ray, HitRecord & rec) const;
};

bool XZRect::hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const
{
   if (shadow_ray && !mMaterial->castsShadows()) {
      return false;
   }
   float t = (mBounds.mMax[1] - ray.mOrigin[1]) / ray.mDir[1];
   if (t < tmin || t > tmax) {
      return false;
   }
   float x = ray.mOrigin[0] + t*ray.mDir[0];
   float z = ray.mOrigin[2] + t*ray.mDir[2];
   if (x < mBounds.mMin[0] || x > mBounds.mMax[0] || z < mBounds.mMin[2] || z > mBounds.mMax[2]) {
      return false;
   }
   rec.u = (x - mBounds.mMin[0]) / (mBounds.mMax[0] - mBounds.mMin[0]);
   rec.v = (z - mBounds.mMin[2]) / (mBounds.mMax[2] - mBounds.mMin[2]);
   rec.t = t;
   return true;
}

void XZRect::calcNormalAndMaterial(const Ray & ray, HitRecord & rec) const
{
   rec.mPoint = ray.point_at_parameter(rec.t);
   rec.mMaterial = mMaterial;
   rec.mNormal = glm::vec3(0, ray.mDir[2] > 0 ? -1 : 1, 0 );
}




class YZRect : public AAPlanes
{
public:
   YZRect() { ; }
   YZRect(float y0, float y1, float z0, float z1, float k, Material * mat) : AAPlanes(mat)
   {
      mBounds = AABoundingBox(glm::vec3(k - .0001, y0,  z0), glm::vec3(k + .0001,y1, z1));
      mVelocity = glm::vec3(0, 0, 0);
   }
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const;
   virtual void calcNormalAndMaterial(const Ray & ray, HitRecord & rec) const;
};

bool YZRect::hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const
{
   if (shadow_ray && !mMaterial->castsShadows()) {
      return false;
   }
   float t = (mBounds.mMax[0] - ray.mOrigin[0]) / ray.mDir[0];
   if (t < tmin || t > tmax) {
      return false;
   }
   float y = ray.mOrigin[1] + t*ray.mDir[1];
   float z = ray.mOrigin[2] + t*ray.mDir[2];
   if (y < mBounds.mMin[1] || y > mBounds.mMax[1] || z < mBounds.mMin[2] || z > mBounds.mMax[2]) {
      return false;
   }
   rec.u = (y - mBounds.mMin[1]) / (mBounds.mMax[1] - mBounds.mMin[1]);
   rec.v = (z - mBounds.mMin[2]) / (mBounds.mMax[2] - mBounds.mMin[2]);
   rec.t = t;
   return true;
}

void YZRect::calcNormalAndMaterial(const Ray & ray, HitRecord & rec) const
{
   rec.mPoint = ray.point_at_parameter(rec.t);
   rec.mMaterial = mMaterial;
   rec.mNormal = glm::vec3(ray.mDir[0] > 0 ? -1 : 1, 0, 0);
}