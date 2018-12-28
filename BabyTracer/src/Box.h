#pragma once
#include "BaseClasses.h"
#include "AAPlanes.h"
#include "AABB.h"

class Box : public Hitable {
public:
   Box() {}
   Box(const glm::vec3& p0, const glm::vec3& p1, Material *ptr);
   virtual bool hit(const Ray& r, float t0, float t1, bool shadow, HitRecord& rec) const;
   virtual bool bounding_box(float t0, float t1, AABB& box) const {
      box = AABB(pmin, pmax);
      return true;
   }
   virtual const char * className() { return "Box"; }

   glm::vec3 pmin, pmax;
   HitableList list_ptr;
   Material * mMaterial;
   Material * getMaterial() { return mMaterial; }
   Sphere myBounds;
};

Box::Box(const glm::vec3& p0, const glm::vec3& p1, Material *pMat) {
   pmin = p0;
   pmax = p1;
   mMaterial = pMat;
   list_ptr.add(new XYRect(p0.x, p1.x, p0.y, p1.y, p1.z, pMat));
   list_ptr.add(new XYRect(p0.x, p1.x, p0.y, p1.y, p0.z, pMat, true));

   list_ptr.add(new XZRect(p0.x, p1.x, p0.z, p1.z, p1.y, pMat));
   list_ptr.add(new XZRect(p0.x, p1.x, p0.z, p1.z, p0.y, pMat, true));
   
   list_ptr.add(new YZRect(p0.y, p1.y, p0.z, p1.z, p1.x, pMat));
   list_ptr.add(new YZRect(p0.y, p1.y, p0.z, p1.z, p0.x, pMat, true));
   myBounds.mCenter = .5f*(p1 + p0);
   myBounds.setRadius(glm::max(glm::max(fabs(p0[0] - p1[0]), fabs(p0[1] - p1[1])), fabs(p0[2] - p1[2])));

}

bool Box::hit(const Ray& r, float t0, float t1, bool shadow, HitRecord& rec) const 
{
   HitRecord rec2;
   if (!myBounds.hit(r, t0, t1, shadow, rec2)) {
      return false;
   }
   return list_ptr.hit(r, t0, t1, shadow, rec);
}
