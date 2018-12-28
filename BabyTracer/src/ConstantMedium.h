#pragma once
#include "BaseClasses.h"
#include "Texture.h"

#include <glm/glm.hpp>

class Isotropic : public Material
{
public:
   Isotropic(Texture * a) : Material(a) {
      ;
   }
   virtual bool scatter(const Ray & r_in, const HitRecord & rec, glm::vec3 & attenuation, Ray & scattered) const
   {
      scattered = Ray(rec.mPoint, random_in_unit_sphere());
      attenuation = mAlbedo->value(rec.u, rec.v, rec.mPoint);
      return true;
   }
   virtual  glm::vec3 applyLight(const glm::vec3 & lightDir, const glm::vec3 & lightColor, const Ray & r_in, const HitRecord & rec) { return glm::vec3() ; }
};

class ConstantMedium : public Hitable
{
public:
   ConstantMedium(Hitable *b, float d, Texture * a) :
      mBoundry(b), mDensity(d) {
      mMaterial = new Isotropic(a);
   }
   virtual const char * className() { return "Constant Medium"; }

   virtual Material * getMaterial() { return mMaterial; }
   virtual bool hit(const Ray & ray, float tmin, float tmax, bool shadow_ray, HitRecord & rec) const;
   virtual void calcNormal(const Ray & ray, HitRecord & rec) const { ; }
   virtual void calcUV(const Ray & ray, HitRecord & rec) const { ; }

   virtual bool boundingBox(float t0, float t1, AABoundingBox & box) const { return true; }

   Material * mMaterial;
   Hitable * mBoundry;
   float mDensity;
};
