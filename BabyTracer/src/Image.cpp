#include "Image.h"
#include <iostream>
#include <fstream>

using namespace std;

#include "CV_SDK/cvmarkersobj.h"
using namespace Concurrency::diagnostic;

Pixel * Image::toPixelData()
{

   if (!mPixelData) {
      mPixelData = new Pixel[mHeight*mWidth]();
   }

   float sampInv = 1.0f / mNumSamples;

   glm_f32vec4 numSampInv = _mm_set1_ps(sampInv);
   glm_f32vec4 trans = _mm_set1_ps(255.99f);
   for (int j = 0; j < mHeight; j++)
   {
#ifdef NDEBUG
#pragma omp parallel for
#endif 
      for (int i = 0; i < mWidth; i++)
      {
         __m128 pSrc1 = _mm_set_ps(mData[i][j][0], mData[i][j][1], mData[i][j][2], 0);

         glm_f32vec4 val = _mm_mul_ps(pSrc1, numSampInv);
         val = _mm_sqrt_ps(val);
         val = _mm_mul_ps(val, trans);

         float result[4];
         _mm_store_ps(result, val);

         int r = result[3];
         if (r > 255) {
            r = 255;
         }
         int g = result[2];
         if (g > 255) {
            g = 255;
         }
         int b = result[1];
         if (b > 255) {
            b = 255;
         }
         mPixelData[j*mWidth + i].rgb[0] = (unsigned char)(r);
         mPixelData[j*mWidth + i].rgb[1] = (unsigned char)(g);
         mPixelData[j*mWidth + i].rgb[2] = (unsigned char)(b);
      }
   }
   return mPixelData;
}

void Image::saveToPPM(const char * fname) const
{
   ofstream fout(fname);// getenv("OUTPUT_PATH"));
   fout << "P3\n" << mWidth << " " << mHeight << "\n255\n";
   for (int j = mHeight - 1; j >= 0; j--)
   {
      for (int i = 0; i < mWidth; i++)
      {
         glm::vec3 pixel = glm::vec3(sqrt(mData[i][j][0] / mNumSamples), sqrt(mData[i][j][1] / mNumSamples), sqrt(mData[i][j][2] / mNumSamples));

         int r = (int)(255.99f * pixel[0]);
         if (r > 255) {
            r = 255;
         }
         int g = (int)(255.99f * pixel[1]);
         if (g > 255) {
            g = 255;
         }
         int b = (int)(255.99f * pixel[2]);
         if (b > 255) {
            b = 255;
         }

         fout << r << " " << g << " " << b << "\n";
      }
   }
   fout.close();
}

