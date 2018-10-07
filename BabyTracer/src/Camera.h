#pragma once
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "MathUtils.h"


struct Ray
{
   Ray(): mOrigin(0,0,0), mDir(0,0,0){
      ;
   }
   Ray(const glm::vec3 & start, const glm::vec3 & dir) : mOrigin(start), mDir(dir) {
      ;
   }
   glm::vec3 mOrigin;
   glm::vec3 mDir;
   glm::vec3 point_at_parameter(float t) const { return mOrigin + t*mDir; }
};

class Camera
{
public:
   Camera() { ; }
   Camera(const glm::vec3 & lookfrom, const glm::vec3 & lookat, const glm::vec3 & vup, float vfov, float aspect,
      float aperture)
   {
      mLookFrom = lookfrom;
      mLookAt = lookat;
      mVup = vup;
      mFov = vfov;
      mAspect = aspect;
      mAperture = aperture;
      updateCamera();
   }

   void updateCamera()
   {
      float dist_to_focus = glm::length(mLookFrom - mLookAt);
      mLensRadius = mAperture / 2.0f;
      float theta = mFov*M_PI / 180.0f;
      float half_height = tan(theta / 2.0f);
      float half_width = mAspect* half_height;
      origin = mLookFrom;
      w = glm::normalize(mLookFrom - mLookAt);
      u = glm::normalize(glm::cross(mVup, w));
      v = glm::cross(w, u);

      lower_left_corner = origin - half_width*dist_to_focus*u - half_height*dist_to_focus*v - dist_to_focus* w;
      horz = 2 * half_width*dist_to_focus*u;
      vert = 2 * half_height*dist_to_focus*v;
   }

   Ray get_ray(float s, float t) {
      glm::vec3 rd = mLensRadius*random_in_unit_disk();
      glm::vec3 offset = u*rd[0] + v*rd[1];
      glm::vec3 direction = lower_left_corner + s*horz + t*vert - origin - offset;
      direction = glm::normalize(direction);
      return Ray(origin + offset, direction);
   }

   glm::vec3 mLookFrom;
   glm::vec3 mLookAt;
   glm::vec3 mVup;
   float mAperture;
   float mFov;
   float mAspect;

private:
   glm::vec3 lower_left_corner;
   glm::vec3 horz;
   glm::vec3 vert;
   glm::vec3 origin;
   glm::vec3 u, v, w;
   float mLensRadius;
};

