#include "ConstantMedium.h"


bool ConstantMedium::hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const
{
   //bool db = (drand48() < .00001);
   //db = false;
   HitRecord rec1, rec2;
   if (mBoundry->hit(ray, -FLT_MAX, FLT_MAX, 0, rec1))
   {
      if (mBoundry->hit(ray, rec1.t + .0001, FLT_MAX, 0, rec2))
      {
         if (rec1.t < tmin) {
            rec1.t = tmin;
         }
         if (rec2.t > tmax) {
            rec2.t = tmax;
         }
         if (rec1.t >= rec2.t) {
            return false;
         }
         if (rec1.t < 0) {
            rec1.t = 0;
         }
         float distance_inside_boundry = glm::length((rec2.t - rec1.t)*ray.mDir);
         float hit_dist = (-1.0f / mDensity)*log(drand48());
         if (hit_dist < distance_inside_boundry)
         {
            rec.t = rec1.t + hit_dist / glm::length(ray.mDir);
            rec.mPoint = ray.point_at_parameter(rec.t);
            rec.mNormal = glm::vec3(1, 0, 0);
            rec.mObject = mBoundry;
            return true;
         }
      }
   }
   return false;
};

