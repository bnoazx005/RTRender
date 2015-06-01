#ifndef RT_TGA_IMAGE_H
#define RT_TGA_IMAGE_H


#include "interfaces\RTImage.h"


namespace RTRender
{

	enum E_TGA_IMAGE_TYPE
	{
		TIT_TRUE_COLOR_24 = 24,
	};

	class CTGAImage : public IImage
	{
		public:
			CTGAImage(unsigned int width, unsigned int height, E_TGA_IMAGE_TYPE type);
			CTGAImage(const unsigned char* imgData, unsigned int width, unsigned int height, E_TGA_IMAGE_TYPE type, unsigned int size);
			virtual ~CTGAImage();

			virtual bool SetPixel(unsigned int x, unsigned int y, const CColor& color);

			///Medhod automatically reverses r and b components in array
			///Format of input array is R|G|B|R|G|B|...|R|G|B
			bool Copy(unsigned char* imgBuffer, unsigned int size);

			//virtual bool LoadFromFile(std::string filename);
			virtual bool SaveToFile(const std::string& filename);
		private:
			CTGAImage();
			CTGAImage(const CTGAImage& img);
		private:

			#pragma pack(push, 1)

			struct TTGAHeader
			{
				char mIdLength;
				char mColorMapType;
				char mImageType;

				short mFirstEntryIndex;
				short mColorMapLength;
				char mColorMapEntrySize;

				short mXOrigin;
				short mYOrigin;
				short mWidth;
				short mHeight;
				char mBitsPerPixel;
				char mImageDescriptor;
			};

			#pragma pack(pop)

		private:
	};

}

#endif