#ifndef IIMAGE_H
#define IIMAGE_H


#include <string>


namespace RTRender
{
	class CColor;


	enum E_IMAGE_TYPE
	{
		IT_TGA,
		IT_PPM
	};

	class IImage
	{
		public:
			IImage(unsigned int width, unsigned int height, unsigned int bpp);
			IImage(const unsigned char* imgData, unsigned int width, unsigned int height, unsigned int bpp, unsigned int size);
			virtual ~IImage();

			virtual bool SetPixel(unsigned int x, unsigned int y, const CColor& color) = 0;
			void SetFilename(const std::string& filename);

			virtual bool Copy(unsigned char* imgBuffer, unsigned int size) = 0;

			//virtual bool LoadFromFile(std::string filename) = 0;
			virtual bool SaveToFile(const std::string& filename) = 0;

			unsigned int GetBitsPerPixel() const;
			unsigned int GetWidth() const;
			unsigned int GetHeight() const;

			std::string GetFilename() const;
		protected:
			IImage();
			IImage(const IImage& img);
		protected:
			unsigned char* mpImageData;

			unsigned int mWidth;
			unsigned int mHeight;

			unsigned int mBitsPerPixel;

			std::string mFilename;
	};
}

#endif