
#pragma  once

class Texture
{
public:
   virtual glm::vec3 value(float u, float v, const glm::vec3& p) const = 0;
};

class ConstantTexture : public Texture
{
public:
   ConstantTexture(const glm::vec3 & color) : Texture(), mColor(color) {
      ;
   }
   ConstantTexture() : Texture(), mColor(.5, .5, .5) {
      ;
   }

   virtual glm::vec3 value(float u, float v, const glm::vec3& p) const {
      return mColor;
   }

   glm::vec3 mColor;
};

class CheckerTexture : public Texture
{
public:
   CheckerTexture(Texture * t0, Texture * t1 ) : mOdd(t0), mEven(t1) {
      ;
   }

   virtual glm::vec3 value(float u, float v, const glm::vec3& p) const {
      float sines = sin(10 * p[0])*sin(10 * p[1])*sin(10 * p[2]);
      if (sines < 0) {
         return mOdd->value(u, v, p);
      }
      else {
         return mEven->value(u, v, p);
      }
   }

   Texture * mOdd;
   Texture * mEven;
};

class ImageTexture : public Texture
{
public:
	ImageTexture() {
		;
	}
	ImageTexture(unsigned char * pixels, int A, int B) :  mRows(A), mCols(B)
	{
		mData = new glm::vec3[mRows*mCols];
		for (int i = 0; i < mRows*mCols; i++) {
			mData[i] = glm::vec3(((int)pixels[i * 3])/255.0f, ((int)pixels[i * 3 + 1]) / 255.0f, ((int)pixels[i * 3 + 2]) / 255.0f);
		}
	}

	virtual glm::vec3 value(float u, float v, const glm::vec3& p) const 
	{
		
		int i = (u)* mRows;
		int j = (1 - v)*mCols - .001;
		if (i < 0) { i = 0; }
		if (j < 0) { j = 0; }
		if (i > mRows - 1) { i = mRows-1; }
		if (j > mCols - 1) { j = mCols-1; }
		return mData[i + mRows*j];
	}

	glm::vec3 * mData;
	int mRows;
	int mCols;
};