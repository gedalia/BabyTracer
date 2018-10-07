#include "MathUtils.h"
#include <glm/glm.hpp>

glm::vec3 random_in_unit_sphere() {
   glm::vec3 p;
   float dotval = 0;
   do {
      p = 2.0f*glm::vec3(drand48(), drand48(), drand48()) - glm::vec3(1.0, 1.0, 1.0);
      dotval = glm::dot(p, p);
   } while (dotval >= 1.0f);
   return p;
}

glm::vec3 random_in_unit_disk() {
   glm::vec3 p;
   do {
      p = 2.0f*glm::vec3(drand48(), drand48(), 0) - glm::vec3(1.0, 1.0, 0.0);
   } while (glm::dot(p,p) >= 1.0f);
   return p;
}

glm::vec3 reflect(const glm::vec3 & v, const glm::vec3 & n) {
   return v - 2.0f*glm::dot(v, n)*n;
}


float schlick(float cosine, float ref_idx)
{
   float r0 = (1 - ref_idx) / (1 + ref_idx);
   r0 = r0*r0;
   return r0 + (1 - r0)*pow((1 - cosine), 5);
}

bool refract(const glm::vec3 & v, const glm::vec3 & n, float ni_over_nt, glm::vec3 & refracted) {
   glm::vec3 uv = glm::normalize(v);
   float dt = glm::dot(uv, n);
   float discriminat = 1.0f - ni_over_nt*ni_over_nt*(1.0f - dt*dt);
   if (discriminat > 0) {
      refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminat);
      return true;
   }
   return false;
}


