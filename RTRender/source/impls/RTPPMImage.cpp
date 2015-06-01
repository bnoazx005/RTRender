#include "impls\RTPPMImage.h"
#include "impls\RTColor.h"
#include <fstream>
#include <iostream>


namespace RTRender
{

	CPPMImage::CPPMImage() :
		IImage()
	{
		mFilename += ".ppm";
	}

	CPPMImage::CPPMImage(const CPPMImage& img) :
		IImage(img)
	{
		mFilename += ".ppm";
	}

	CPPMImage::CPPMImage(unsigned int width, unsigned int height) :
		IImage(width, height, 24)
	{
		mFilename += ".ppm";
	}

	CPPMImage::CPPMImage(const unsigned char* imgData, unsigned int width, unsigned int height, unsigned int size) :
		IImage(imgData, width, height, 24, size)
	{
		mFilename += ".ppm";
	}

	CPPMImage::~CPPMImage()
	{
	}

	bool CPPMImage::SetPixel(unsigned int x, unsigned int y, const CColor& color)
	{
		if (x < mWidth && y < mHeight)
		{
			unsigned short bytesPerPixel = mBitsPerPixel >> 3;
			unsigned int index = (y * mWidth + x) * bytesPerPixel;

			mpImageData[index] = unsigned char(color.r * 255);
			mpImageData[index + 1] = unsigned char(color.g * 255);
			mpImageData[index + 2] = unsigned char(color.b * 255);

			return true;
		}

		return false;
	}

	bool CPPMImage::SaveToFile(const std::string& filename)
	{
		std::ofstream outfile(filename.c_str(), std::ios::binary);

		if (!outfile.is_open())
		{
			std::cerr << "Error: File can't be created\n";
			return false;
		}

		char infoStr[255];
		sprintf(infoStr, "P6\n %d %d\n255\n\0", mWidth, mHeight);
		outfile.write(infoStr, strlen(infoStr));

		if (!outfile.good())
		{
			std::cerr << "Error: Can't write PPM header\n";
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

	bool CPPMImage::Copy(unsigned char* imgBuffer, unsigned int size)
	{
		if ((size > mWidth * mHeight * (mBitsPerPixel >> 3)) || !imgBuffer)
			return false;
		
		memcpy(mpImageData, imgBuffer, size);

		return true;
	}
}