#pragma once
#include <glm/vec3.hpp>
#include <glm/glm.hpp>


struct Pixel
{
   Pixel() {
      rgb[0] = rgb[1] = rgb[2] = 0;
   }
   unsigned char rgb[3];
};

class Image
{
public:
   Image(int width, int height) : mGlID(0)
   {
      mNumSamples = 0;
      mPixelData = nullptr;
      mWidth = width;
      mHeight = height;
      mData = new glm::vec3*[width];
      for (int j = 0; j < width; ++j) {
         mData[j] = new glm::vec3[mHeight];
         for (int i = 0; i < mHeight; ++i) {
            mData[j][i] = glm::vec3(0, 0, 0);
         }
      }
   }

   void saveToPPM(const char * fname) const;

   Pixel * toPixelData() ;

   int mWidth;
   int mHeight;
   int mNumSamples;
   glm::vec3 ** mData;
   Pixel * mPixelData;
   unsigned int mGlID;
};


