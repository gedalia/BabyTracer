#pragma once
#include "BaseClasses.h"

class AAPlanes : public Hitable
{
public:
   AAPlanes() : mFilpNormal(false), mMaterial (nullptr){

   }
   AAPlanes( Material * mat, bool flipNormal = false) :mFilpNormal(flipNormal), mMaterial(mat) {
      ;
   }

   virtual  ~AAPlanes()
   {
      delete mMaterial;
   }
   virtual Material * getMaterial() { return mMaterial; }

   virtual bool boundingBox(float t0, float t1, AABoundingBox & box) const
   {
      box = mBounds;
      return true;
   }

   AABoundingBox mBounds;
   glm::vec3 mVelocity;
   Material * mMaterial;
   bool mFilpNormal;
   virtual const char * className() { return "AAPlanes"; }

};

class XYRect : public AAPlanes
{
public:
   XYRect() : AAPlanes() { ; }
   XYRect(float x0, float x1, float y0, float y1, float k, Material * mat, bool flipNormal = false) : AAPlanes(mat, flipNormal)
   {
      mBounds = AABoundingBox(glm::vec3(x0, y0, k-.0001), glm::vec3(x1, y1, k + .0001));
      mVelocity = glm::vec3(0, 0, 0);
   }
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const;
   virtual void calcNormal(const Ray & ray, HitRecord & rec) const;
   virtual void calcUV(const Ray & ray, HitRecord & rec) const;
   virtual const char * className() { return "XYRect"; }

};

void XYRect::calcUV(const Ray & ray, HitRecord & rec) const
{

	rec.u = (rec.mPoint.x - mBounds.mMin[0]) / (mBounds.mMax[0] - mBounds.mMin[0]);
	rec.v = (rec.mPoint.y - mBounds.mMin[1]) / (mBounds.mMax[1] - mBounds.mMin[1]);

}

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

   rec.t = t;
	rec.mPoint = ray.point_at_parameter(rec.t);
	return true;
}

void XYRect::calcNormal(const Ray & ray, HitRecord & rec) const
{
   rec.mNormal = glm::vec3(0, 0, mFilpNormal ? -1 : 1);
}

class XZRect : public AAPlanes
{
public:
   XZRect() { ; }
   XZRect(float x0, float x1, float z0, float z1, float k, Material * mat, bool flipNormal = false) : 
      AAPlanes(mat, flipNormal)
   {
      mBounds = AABoundingBox(glm::vec3(x0, k - .0001, z0), glm::vec3(x1, k + .0001, z1));
      mVelocity = glm::vec3(0, 0, 0);
   }
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const;
   virtual void calcNormal(const Ray & ray, HitRecord & rec) const;
   virtual void calcUV(const Ray & ray, HitRecord & rec) const;
   virtual const char * className() { return "XZRect"; }
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
	rec.t = t;
	rec.mPoint = ray.point_at_parameter(rec.t);
   return true;
}

void XZRect::calcNormal(const Ray & ray, HitRecord & rec) const
{
   rec.mNormal = glm::vec3(0, mFilpNormal ? -1 : 1, 0 );
}

void XZRect::calcUV(const Ray & ray, HitRecord & rec) const
{
	rec.u = ( rec.mPoint.x - mBounds.mMin[0]) / (mBounds.mMax[0] - mBounds.mMin[0]);
	rec.v = ( rec.mPoint.z - mBounds.mMin[2]) / (mBounds.mMax[2] - mBounds.mMin[2]);
}




class YZRect : public AAPlanes
{
public:
   YZRect() { ; }
   YZRect(float y0, float y1, float z0, float z1, float k, Material * mat, bool flipNormal = false) :
   AAPlanes(mat, flipNormal)
   {
      mBounds = AABoundingBox(glm::vec3(k - .0001, y0,  z0), glm::vec3(k + .0001,y1, z1));
      mVelocity = glm::vec3(0, 0, 0);
   }
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const;
   virtual void calcNormal(const Ray & ray, HitRecord & rec) const;
   virtual void calcUV(const Ray & ray, HitRecord & rec) const;
   virtual const char * className() { return "YZRect"; }

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
	
   rec.t = t;
	rec.mPoint = ray.point_at_parameter(rec.t);
	return true;
}

void YZRect::calcNormal(const Ray & ray, HitRecord & rec) const
{
    rec.mNormal = glm::vec3(mFilpNormal ? -1 : 1, 0, 0);
}

void YZRect::calcUV(const Ray & ray, HitRecord & rec) const
{

	rec.u = (rec.mPoint.y - mBounds.mMin[1]) / (mBounds.mMax[1] - mBounds.mMin[1]);
	rec.v = (rec.mPoint.z - mBounds.mMin[2]) / (mBounds.mMax[2] - mBounds.mMin[2]);

}
