#pragma once
#include "Texture.h"

class Lambertian : public Material
{
public:
   Lambertian(Texture* albedo, float rougness) : Material(albedo), mRoughness(rougness)
   {

   }

   float calcFresnel(float F0, const glm::vec3 & vDir, const glm::vec3 & perturbednormal) const
   {
      glm::vec3 halfangle = glm::normalize(vDir + reflect(vDir, perturbednormal));
      float vdh = glm::dot(vDir, halfangle);
      float ndv = glm::dot(vDir, perturbednormal);
      // float sphg = pow(2.0, (-5.55473*vdh - 6.98316) * vdh);
      float sphg = pow(vdh, 5.0f);
      float res = F0 + (1.0f - F0) * sphg;
      return res;
   }

   virtual bool scatter(const Ray & r_in, const HitRecord & rec, glm::vec3 & attenuation, Ray & scattered) const
   {
      if (1) 
      {

         glm::vec3 vDir = glm::normalize(r_in.mDir);

        // float val = 1.01f - glm::dot(-vDir, glm::normalize(rec.mNormal));
        // float F0 = glm::mix(.04f, 1.0f, pow(val, 5.0));
         //if (F0 > 1.0f) {
            // shouldn't happen
         //   F0 = 1.0f;
         //}
         glm::vec3 randomDir = random_in_unit_sphere();
         glm::vec3 perturbednormal = glm::normalize(rec.mNormal + mRoughness*randomDir);
         float fres = (1.0f-mRoughness)*calcFresnel(.04, vDir, perturbednormal);
         // totally unjustified hack fade off faster.
         fres *= fres;
         // use fres prob to decide if rays reflect or scatter
         if (drand48() < fres)
         {
            glm::vec3 reflected = reflect(vDir, perturbednormal);
            scattered = Ray(rec.mPoint, reflected + perturbednormal, r_in.mTime);
            attenuation = glm::mix(glm::vec3(1, 1, 1), mAlbedo->value(rec.u, rec.v, rec.mPoint), 1-glm::dot(rec.mNormal, perturbednormal));
            return true;
         }

         scattered = Ray(rec.mPoint, rec.mNormal + randomDir,  r_in.mTime);
         attenuation = mAlbedo->value(rec.u, rec.v, rec.mPoint) * glm::dot(scattered.mDir, rec.mNormal);
      }
      else 
      {
         scattered = Ray(rec.mPoint, rec.mNormal + random_in_unit_sphere(), r_in.mTime);
         attenuation = mAlbedo->value(rec.u, rec.v, rec.mPoint);
      }
      //      scattered = Ray(rec.mPoint+ rec.mNormal, rec.mNormal);
    
//      return false;
      return true;
   }
   
   virtual void emissivity(const Ray & ray, const HitRecord & rec, glm::vec3 & color)
   { 
      

     // float fres = calcFresnel(ray, rec);
     
//            color += glm::vec3(fres, fres, fres);
   
   }

   virtual bool receivesShadows() { return true; };
   

   virtual glm::vec3 applyLight(const glm::vec3 & lightDir, const glm::vec3 & lightColor, const Ray & r_in,  const HitRecord & rec)
   {

      glm::vec3 refl = reflect(lightDir, rec.mNormal);
      float specExp = exp2(9 * (1 - mRoughness) + 1);
      float specPower = glm::max(glm::dot(refl, r_in.mDir), 0.0f);
      float F0 = .04f;
      /*
      float val = 1.01 - glm::dot(-glm::normalize(r_in.mDir), glm::normalize(rec.mNormal));
      float F0 =  glm::mix(.04f, 1.0f, pow(val, 5.0));
      */
      float spec = F0*((specExp + 8) / 8.0f)*pow(specPower, specExp);
      if (spec > 1.0) {
         spec = 1.0;
      }

      float diffuse = glm::max((glm::dot(rec.mNormal, lightDir)), 0.0f);

      diffuse -= spec;
      diffuse = glm::max(diffuse, 0.0f);
     
      return  lightColor*(mAlbedo->value(rec.u, rec.v, rec.mPoint)*diffuse + spec);
   }

   float mRoughness;
};


class Metal : public Material
{
public:
   Metal(const glm::vec3& albedo, float rough) :  mRoughness(rough)
   {
      mAlbedo = new ConstantTexture(albedo);
   }
   virtual bool scatter(const Ray & r_in, const HitRecord & rec, glm::vec3 & attenuation, Ray & scattered) const
   {
      glm::vec3 reflected = reflect(glm::normalize(r_in.mDir), rec.mNormal);
      
      scattered = Ray(rec.mPoint, reflected + mRoughness*random_in_unit_sphere(), r_in.mTime);
      attenuation = mAlbedo->value(rec.u, rec.v, rec.mPoint);

      return (glm::dot(scattered.mDir, rec.mNormal) > 0);
   }

   virtual glm::vec3 applyLight(const glm::vec3 & lightDir, const glm::vec3 & lightColor,  const Ray & r_in, const HitRecord & rec)
   {
      glm::vec3 refl = reflect(lightDir, rec.mNormal);

      float specExp = exp2(9 * (1-mRoughness) + 1);
      float spec = ((specExp +8)/8.0f)*pow(glm::max(glm::dot(refl, r_in.mDir),0.0f), specExp);
      if (spec > 1.0) {
         spec = 1.0;
      }
      return lightColor* mAlbedo->value(rec.u, rec.v, rec.mPoint)*spec;
   }

   float mRoughness;
   virtual bool receivesShadows() { return false; };
};


class Dielectric : public Material
{
public:
   Dielectric(float ri) : mRef_idx(ri)
   {

   }

   virtual bool scatter(const Ray & r_in, const HitRecord & rec, glm::vec3 & attenuation, Ray & scattered) const
   {
      glm::vec3 outward_normal;
      glm::vec3 reflected = reflect(r_in.mDir, rec.mNormal);
      float ni_over_nt;
      attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
      glm::vec3 refracted;
      float reflect_prob;
      float cosine;
      float dotprod = glm::dot(r_in.mDir, rec.mNormal);
      if (dotprod > 0) {
         outward_normal = -rec.mNormal;
         ni_over_nt = mRef_idx;
         cosine = mRef_idx *dotprod / glm::length(r_in.mDir);
      }
      else {
         outward_normal = rec.mNormal;
         ni_over_nt = 1.0 / mRef_idx;
         cosine = -dotprod / glm::length(r_in.mDir);
      }

      if (refract(r_in.mDir, outward_normal, ni_over_nt, refracted)) {
         reflect_prob = schlick(cosine, mRef_idx);
      }
      else {
         scattered = Ray(rec.mPoint, reflected, r_in.mTime);
         reflect_prob = 1.0;
      }

      float rand_num = drand48();
      if (rand_num < reflect_prob) {
         scattered = Ray(rec.mPoint, reflected,r_in.mTime);
      }
      else {
         scattered = Ray(rec.mPoint, refracted, r_in.mTime);
      }
      return true;
   }
   
   virtual glm::vec3 applyLight(const glm::vec3 & lightDir, const glm::vec3 & lightColor, const Ray & r_in, const HitRecord & rec)
   {
      glm::vec3 refl = reflect(lightDir, rec.mNormal);
      float spec = ((128 + 8) / 8.0f)*pow(glm::max(glm::dot(refl, r_in.mDir), 0.0f), 1.0 * 128);
      if (spec > 1.0) {
         spec = 1.0;
      }
      return lightColor*(spec);
   }

   virtual bool castsShadows() { return false; }
   virtual bool receivesShadows() { return false; };
   float mRef_idx;
};

class EmissiveMat : public Material
{
public:
	EmissiveMat(float multiplier, Texture * texture) : mTextureMultiplier(multiplier)
	{
      mAlbedo = texture;
	}
   EmissiveMat(Texture * texture) : mTextureMultiplier(1)
   {
      mAlbedo = texture;
   }
	EmissiveMat(float multiplier, glm::vec3& albedo) : mTextureMultiplier(multiplier)
   {
      mAlbedo = new ConstantTexture(albedo);
   }
   virtual bool scatter(const Ray & r_in, const HitRecord & rec, glm::vec3 & attenuation, Ray & scattered) const
   {
		attenuation = mTextureMultiplier * mAlbedo->value(rec.u, rec.v, rec.mPoint);
		return false;
   }

   virtual glm::vec3 applyLight(const glm::vec3 & lightDir, const glm::vec3 & lightColor, const Ray & r_in, const HitRecord & rec)
   {
      return glm::vec3(0,0,0);
   }
	virtual void emissivity(const Ray & ray, const HitRecord & rec, glm::vec3 & color) 
	{ 
      color += mTextureMultiplier*mAlbedo->value(rec.u, rec.v, rec.mPoint);
	}

	float mTextureMultiplier;
   virtual bool receivesShadows() { return false; };
   virtual bool castsShadows() { return true; }
};

