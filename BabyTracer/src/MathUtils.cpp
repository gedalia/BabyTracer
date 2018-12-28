#include "MathUtils.h"
#include <glm/glm.hpp>
#include <vector>

glm::vec3 random_in_unit_sphere() 
{
   /*
   glm::vec3 p;
   float dotval = 0;
   do {
      p = 2.0f*glm::vec3(drand48(), drand48(), drand48()) - glm::vec3(1.0, 1.0, 1.0);
      dotval = glm::dot(p, p);
   } while (dotval >= 1.0f);
   return p;
   */   static thread_local std::vector<glm::vec3> random_numbers;
   static thread_local int last_int = 0;
   if (random_numbers.size() == 0)
   {
      random_numbers.reserve(10000);
      while (random_numbers.size() < 10000) {
         glm::vec3 p;
         float dotval = 0;
         do {
            p = 2.0f*glm::vec3(drand48(), drand48(), drand48()) - glm::vec3(1.0, 1.0, 1.0);
            dotval = glm::dot(p, p);
         } while (dotval >= 1.0f);
         random_numbers.push_back(glm::normalize(p));
      }
   }
   if (last_int >= random_numbers.size()) {
      last_int = (int)(drand48() * 1000);
   }
   return random_numbers[last_int++];

}

glm::vec3 random_in_unit_disk() 
{
   static thread_local std::vector<glm::vec3> random_numbers;
   static thread_local int last_int = 0;
   if (random_numbers.size() == 0)
   {
      random_numbers.reserve(10000);
      while (random_numbers.size() < 10000)
      {
         glm::vec3 p;
         do {
            p = 2.0f*glm::vec3(drand48(), drand48(), 0) - glm::vec3(1.0, 1.0, 0.0);
         } while (glm::dot(p, p) >= 1.0f);

         random_numbers.push_back(p);
      }
   }

   if (last_int >= random_numbers.size()) {
      last_int = (int)(drand48() * 1000);
   }
   return random_numbers[last_int++];
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


