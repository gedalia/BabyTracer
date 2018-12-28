#pragma once

#include "BaseClasses.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB {
public:
   AABB() {}
   AABB(const glm::vec3& a, const glm::vec3& b) { _min = a; _max = b; }

   glm::vec3 min() const { return _min; }
   glm::vec3 max() const { return _max; }

   bool hit(const Ray& r, float tmin, float tmax) const {
      for (int a = 0; a < 3; a++) {
         float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a],
            (_max[a] - r.origin()[a]) / r.direction()[a]);
         float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a],
            (_max[a] - r.origin()[a]) / r.direction()[a]);
         tmin = ffmax(t0, tmin);
         tmax = ffmin(t1, tmax);
         if (tmax <= tmin)
            return false;
      }
      return true;
   }

   glm::vec3 _min;
   glm::vec3 _max;
};

AABB surrounding_box(AABB box0, AABB box1)
{
   glm::vec3 small(
      fmin(box0.min()[0], box1.min()[0]),
      fmin(box0.min()[1], box1.min()[1]),
      fmin(box0.min()[2], box1.min()[2]));
   glm::vec3 big(
      fmax(box0.max()[0], box1.max()[0]),
      fmax(box0.max()[1], box1.max()[1]),
      fmax(box0.max()[2], box1.max()[2]));
   return AABB(small, big);
}

