#ifndef RT_PPM_IMAGE_H
#define RT_PPM_IMAGE_H


#include "interfaces\RTImage.h"


namespace RTRender
{

	class CPPMImage : public IImage
	{
	public:
		CPPMImage(unsigned int width, unsigned int height);
		CPPMImage(const unsigned char* imgData, unsigned int width, unsigned int height, unsigned int size);
		virtual ~CPPMImage();

		virtual bool SetPixel(unsigned int x, unsigned int y, const CColor& color);

		///Format of input array is R|G|B|R|G|B|...|R|G|B
		bool Copy(unsigned char* imgBuffer, unsigned int size);

		//virtual bool LoadFromFile(std::string filename);
		virtual bool SaveToFile(const std::string& filename);
	private:
		CPPMImage();
		CPPMImage(const CPPMImage& img);
	};

}

#endif