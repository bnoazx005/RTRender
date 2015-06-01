#include "interfaces\RTImage.h"


namespace RTRender
{
	
	IImage::IImage() :
		mpImageData(nullptr), mWidth(0), mHeight(0), mBitsPerPixel(0), mFilename("result")
	{
	}

	IImage::IImage(const IImage& img)
	{
	}

	IImage::IImage(unsigned int width, unsigned int height, unsigned int bpp) :
		mpImageData(nullptr), mWidth(width), mHeight(height), mBitsPerPixel(bpp), mFilename("result")
	{
		unsigned long bytesCount = mWidth * mHeight * (mBitsPerPixel >> 3);

		if (width > 0 && height > 0 && bpp > 0)
			mpImageData = new unsigned char[bytesCount];

		memset(mpImageData, 0, bytesCount);
	}

	IImage::IImage(const unsigned char* imgData, unsigned int width, unsigned int height, unsigned int bpp, unsigned int size) :
		mpImageData(nullptr), mWidth(width), mHeight(height), mBitsPerPixel(bpp), mFilename("result")
	{
		unsigned long bytesCount = mWidth * mHeight * (mBitsPerPixel >> 3);

		if (bytesCount == size)
			mpImageData = new unsigned char[bytesCount];

		memcpy(mpImageData, imgData, size);
	}

	IImage::~IImage()
	{
		if (mpImageData)
			delete[] mpImageData;

		mpImageData = nullptr;
	}

	void IImage::SetFilename(const std::string& filename)
	{
		mFilename = filename;
	}

	unsigned int IImage::GetBitsPerPixel() const
	{
		return mBitsPerPixel;
	}

	unsigned int IImage::GetWidth() const
	{
		return mWidth;
	}

	unsigned int IImage::GetHeight() const
	{
		return mHeight;
	}

	std::string IImage::GetFilename() const
	{
		return mFilename;
	}

}