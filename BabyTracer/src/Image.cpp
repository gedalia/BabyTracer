#include "Image.h"
#include <iostream>
#include <fstream>

using namespace std;

Pixel * Image::toPixelData()
{
   if (!mPixelData) {
      mPixelData = new Pixel[mHeight*mWidth]();
   }
   for (int j = 0; j < mHeight; j++)
   {
      for (int i = 0; i < mWidth; i++)
      {
         int r = 255.99f * sqrt(mData[i][j][0] / mNumSamples);
         if (r > 255) {
            r = 255;
         }
         int g = 255.99f * sqrt(mData[i][j][1] / mNumSamples);
         if (g > 255) {
            g = 255;
         }
         int b = 255.99f * sqrt(mData[i][j][2]/mNumSamples);
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

