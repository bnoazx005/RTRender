#include "impls\RTTGAImage.h"
#include "impls\RTColor.h"
#include <fstream>
#include <iostream>


namespace RTRender
{

	CTGAImage::CTGAImage():
		IImage()
	{
		mFilename += ".tga";
	}

	CTGAImage::CTGAImage(const CTGAImage& img) :
		IImage(img)
	{
		mFilename += ".tga";
	}

	CTGAImage::CTGAImage(unsigned int width, unsigned int height, E_TGA_IMAGE_TYPE type) :
		IImage(width, height, static_cast<int>(type))
	{
		mFilename += ".tga";
	}

	CTGAImage::CTGAImage(const unsigned char* imgData, unsigned int width, unsigned int height, E_TGA_IMAGE_TYPE type, unsigned int size) :
		IImage(imgData, width, height, static_cast<int>(type), size)
	{
		mFilename += ".tga";
	}

	CTGAImage::~CTGAImage()
	{
	}

	bool CTGAImage::SetPixel(unsigned int x, unsigned int y, const CColor& color)
	{
		if (x < mWidth && y < mHeight)
		{
			unsigned short bytesPerPixel = mBitsPerPixel >> 3;
			unsigned int index = (y * mWidth + x) * bytesPerPixel;
			
			mpImageData[index]		= unsigned char(color.b * 255);
			mpImageData[index + 1]	= unsigned char(color.g * 255);
			mpImageData[index + 2]	= unsigned char(color.r * 255);

			return true;
		}

		return false;
	}

	bool CTGAImage::SaveToFile(const std::string& filename)
	{
		std::ofstream outfile(filename.c_str(), std::ios::binary);

		if (!outfile.is_open())
		{
			std::cerr << "Error: File can't be created\n";
			return false;
		}

		TTGAHeader header;
		memset(&header, 0, sizeof(header));

		header.mXOrigin = 0;
		header.mYOrigin = 0;
		header.mWidth = mWidth;
		header.mHeight = mHeight;
		header.mBitsPerPixel = mBitsPerPixel;
		header.mImageType = 2; //uncompressed True-color value
		header.mImageDescriptor = 0x20; //top-left orientation

		outfile.write((char*)&header, sizeof(header));

		if (!outfile.good())
		{
			std::cerr << "Error: Can't write TGA header\n";
			return false;
		}

		outfile.write((char*)mpImageData, mWidth * mHeight * (mBitsPerPixel >> 3));

		if (!outfile.good())
		{
			std::cerr << "Error: Can't write image data\n";
			return false;
		}

		outfile.close();

		return true;
	}

	bool CTGAImage::Copy(unsigned char* imgBuffer, unsigned int size)
	{
		if ((size > mWidth * mHeight * (mBitsPerPixel >> 3)) || !imgBuffer)
			return false;

		for (unsigned int i = 0; i <= size - 2; i += 3)
			std::swap(imgBuffer[i], imgBuffer[i + 2]);

		memcpy(mpImageData, imgBuffer, size);

		return true;
	}
}