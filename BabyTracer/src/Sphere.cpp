#include "Sphere.h"

void
Sphere::setRadius(float rad)
{
   mRadius = rad;
   mRadius2 = rad * rad;
   mInvRadius = 1.0f/mRadius;
}

bool Sphere::hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const
{
   if (shadow_ray && mMaterial && !mMaterial->castsShadows()) {
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

void Sphere::calcNormal(const Ray & ray, HitRecord & rec) const
{
   rec.mPoint = ray.point_at_parameter(rec.t);
   rec.mNormal = (rec.mPoint - (mCenter + ray.mTime*mVelocity)) * mInvRadius;
}

void Sphere::calcUV(const Ray & ray, HitRecord & rec) const
{
	glm::vec3 p = (rec.mPoint - mCenter)/mRadius;
	float phi = atan2(p[2], p[0]);
	float theta = asin(p[1]);
	rec.u = 1 - (phi + M_PI) / (2.0f * M_PI);
	rec.v = (theta + M_PI / 2.0f) / M_PI;
}

bool Sphere::boundingBox(float t0, float t1, AABoundingBox & box) const
{
   box = AABoundingBox(mCenter-glm::vec3(mRadius, mRadius, mRadius), mCenter + glm::vec3(mRadius, mRadius, mRadius));
   return true;
}


